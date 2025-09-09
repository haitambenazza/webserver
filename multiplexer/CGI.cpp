#include "../headers/webserver.hpp"

Cgi::Cgi(){
    ParentFd[0] = ParentFd[1] = -1;
    ChildFd[0] = ChildFd[1] = -1;
    child_pid = -1;
}
Cgi::Cgi( Multiplexer& m, Request Req , Location& loc, std::string& filepath )
{
    ParentFd[0] = ParentFd[1] = -1;
    ChildFd[0] = ChildFd[1] = -1;
    child_pid = -1;
    ExecuteCgi( m, Req , loc, filepath );

}
Cgi::~Cgi(){}

std::string Cgi::GetOutput() {
    return output;
}

std::string ReturnExtention(std::string s)
{
    if (s.empty())
        return "";
    size_t pos = s.find_last_of('.');
    if (pos != std::string::npos)
        return s.substr(pos);
    return "";
}

bool ValidCgiExtention(std::string extention)
{
    return (extention == ".cgi" ||
            extention == ".pl" ||
            extention == ".py" ||
            extention == ".php" ||
            extention == ".sh" ||
            extention == ".rb" ||
            extention == ".exe" ||
            extention == ".out");
}

std::string Matchkeytoextention(std::string& s)
{
    if (s == ".py")
        return ("_py");
    else if (s == ".pl")
        return ("_pl");
    else if (s == ".php")
        return ("_php");
    else if (s == ".sh")
        return ("_sh");
    else if (s == ".rb")
        return ("_rb");
    else if (s == ".exe")
        return ("_exe");
    else if (s == ".out")
        return ("_out");
    return ("");
}

std::string GetCgiPath(std::map<std::string, std::string>& mp, std::string key)
{
    if (mp.find(key) != mp.end())
        return (mp.find(key))->second;
    return ("");
}
void    Cgi::SetEnv( Request& Req )
{
    env.push_back("REQUEST_METHOD=" + Req.getMethod());
    env.push_back("SCRIPT_NAME=" + Req.GetScriptName());
    env.push_back("SCRIPT_FILENAME=" + Req.GetScriptPath());
    env.push_back("QUERY_STRING=" + Req.getQueryString());
    env.push_back("CONTENT_TYPE=" + Req.getHeaderValue("Content-Type"));
    env.push_back("CONTENT_LENGTH=" + Req.getHeaderValue("Content-Length"));
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("REQUEST_URI=" + Req.getUri());
}

std::vector<char *> Cgi::GetEnvCgi()
{
    std::vector<char *> EnvVars;
    for (size_t i = 0; i < env.size(); i++)
    {
        EnvVars.push_back((char *)env[i].c_str());
        // std::cout << EnvVars.back() << std::endl;
    }
    EnvVars.push_back(NULL);
    return (EnvVars);
}

std::string ReturnCgiPath(std::string s, std::map<std::string, std::string> mp)
{
    std::string ext = ReturnExtention(s);

    if (ext.empty())
        return ("");
    if (!ValidCgiExtention(ext))
        return ("");
    if (Matchkeytoextention(ext).empty())
        return ("");
    return (GetCgiPath(mp, Matchkeytoextention(ext)));
}

void            Cgi::SetOutput(std::string& s)
{
    output += s;
}

void    Cgi::ExecuteCgi( Multiplexer& m, Request & Req , Location& loc, std::string filepath )
{
    SetEnv( Req );
    std::vector<char *> envp = GetEnvCgi();
    struct epoll_event ev;

    std::string CgiPath = ReturnCgiPath(filepath, loc.GetCgiPathMap());
    // std::cout << "CGII PAATHH === " << CgiPath << std::endl;
    // std::cout << "FILEPAATHH === " << filepath << std::endl;
    if (CgiPath.empty())
    {
        std::cerr << "CGI path not found for file: " << filepath << std::endl;
        return;
    }
    if (pipe(ParentFd) < 0)
    {
        std::cerr << "Parent pipe creation failed" << std::endl;
        return;
    }
    if (pipe(ChildFd) < 0)
    {
        close(ParentFd[0]);
        close(ParentFd[1]);
        std::cerr << "Child pipe creation failed" << std::endl;
        return;
    }
    child_pid = fork();
    if (child_pid == -1)
    {
        std::cerr << "Fork failed" << std::endl;
        close(ParentFd[0]);
        close(ParentFd[1]);
        close(ChildFd[0]);
        close(ChildFd[1]);
        return;
    } 
    else if (child_pid == 0)
    {
        close(ParentFd[0]);
        close(ChildFd[1]);

        if (dup2(ParentFd[1] , STDOUT_FILENO) == -1)
        {
            perror("dup2_prnt");
            exit(1); 
        }
        close(ParentFd[1]);
        if (dup2(ChildFd[0] , STDIN_FILENO) == -1)
        {
            perror("dup2_child");
            exit(1);
        }
        close(ChildFd[0]);
        char* cmds[3] = { (char *)CgiPath.c_str(), (char *)(filepath.c_str()), NULL};
        execve(cmds[0] , cmds , &envp[0]);
        exit(1);
    }
    else
    {
        close(ParentFd[1]);
        close(ChildFd[0]);
        output.clear();
        output = "";
        
        ev.events = EPOLLIN;
        ev.data.fd = ParentFd[0];
        if (AddToEpoll(m.GetEpollFd(), EPOLL_CTL_ADD, ev.data.fd, &ev) == false)
        {
            close(ParentFd[0]);
            close(ChildFd[1]);
            return;
        }
        char buffer[4096];
        ssize_t bytes_read = 0;
        while ((bytes_read = read(ParentFd[0], buffer, sizeof(buffer) - 1)) > 0) 
        {
            buffer[bytes_read] = '\0';
            std::string tmp(buffer);
            SetOutput(tmp);
        }
        close(ParentFd[0]);
        close(ChildFd[1]);
        int status;
        if (waitpid(child_pid, &status, 0) == -1)
            perror("waitpid");
        
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 
        {
            std::cerr << "CGI script exited with status: " << WEXITSTATUS(status) << std::endl;
            output = "Status: 500 Internal Server Error\r\n\r\nCGI execution failed";
        }
    }
}
