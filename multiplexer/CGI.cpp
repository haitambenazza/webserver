#include "../headers/webserver.hpp"

Cgi::Cgi()
    : script_path(""),
      script_name(""),
      output(""),
      child_pid(-1),
      IsExecuted(false)
{
    ForkTime = 0;
}

Cgi::Cgi( Request Req , Location& loc, std::string& filepath)
{
    child_pid = -1;
    IsExecuted = false;
    script_name = "";
    script_path = "";
    output = "";
    ForkTime = 0;
    fdchild = -1;
    ExecuteCgi(  Req , loc, filepath);
}

Cgi::Cgi(const Cgi &other)
{
    script_path = other.script_path;
    script_name = other.script_name;
    output = other.output;
    child_pid = other.child_pid;
    env = other.env;
    IsExecuted = other.IsExecuted;
    ForkTime = other.ForkTime;
    fdchild = other.fdchild;
}

Cgi&    Cgi::operator=(const Cgi &other)
{
    if (this != &other)
    {
        script_path = other.script_path;
        script_name = other.script_name;
        output = other.output;
        child_pid = other.child_pid;
        env = other.env;
        IsExecuted = other.IsExecuted;
        ForkTime = other.ForkTime;
        fdchild = other.fdchild;
    }
    return (*this);
}

Cgi::~Cgi(){}

std::string Cgi::GetOutput() {
    return output;
}

int16_t         Cgi::GetFdChild() const
{
    return (fdchild);
}

pid_t           Cgi::GetChildPid() const
{
    return child_pid;
}
void            Cgi::SetChildPid(pid_t child)
{
    child_pid = child;
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

void            Cgi::SetOutput(std::string s)
{
    output += s;
}

time_t        Cgi::GetForkTime() const
{
    return (ForkTime);
}

bool Cgi::CgiFork()
{
    child_pid = fork();
    SetChildPid(child_pid);
    ForkTime = time(NULL);
    if (child_pid == -1)
    {
        std::cerr << "Fork failed" << std::endl;
        return false;
    }
    return (true);
}

void       Cgi::ExecCgiChild(std::vector<char *> envp, std::string& CgiPath , std::string& filepath)
{
    std::cout << "INSIDE CHILD " << fdchild << '\n';
    if (dup2(fdchild, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close (fdchild);
    }
    close(fdchild);
    char* cmds[3] = { (char *)CgiPath.c_str(), (char *)(filepath.c_str()), NULL};
    execve(cmds[0] , cmds , &envp[0]);
    exit(1);
}

bool Cgi::CheckExitStatus()
{
    int status = 0;
    pid_t state = waitpid(child_pid, &status, WNOHANG);

    if (state == -1)
    {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "CGI script exited with status: " << WEXITSTATUS(status) << std::endl;
            output = "Status: 500 Internal Server Error\r\n\r\nCGI execution failed";
        }
        return true;
    }
    else if ( !state )
        return true;
    return (false);
}

bool           Cgi::GetExecutedStatus()const
{
    return IsExecuted;
}

void    Cgi::ExecuteCgi(Request & Req, Location& loc, std::string filepath)
{
    SetEnv(Req);
    std::vector<char *> envp = GetEnvCgi();

    std::string CgiPath = ReturnCgiPath(filepath, loc.GetCgiPathMap());
    if (CgiPath.empty())
    {
        std::cerr << "CGI path not found for file: " << filepath << std::endl;
        return;
    }
    
    if (!IsExecuted)
    {
        std::remove("www/temporary/tmpfile");
        fdchild = open("www/temporary/tmpfile", O_RDWR | O_CREAT | O_TRUNC , 0644);
        if (fdchild == -1)
        {
            perror("Failed to create tmp file");
            return;
        }
        
        if (!CgiFork())
            return;
        
        if (child_pid == 0)
        {
            ExecCgiChild(envp, CgiPath, filepath);
        }
        else
        {
            std::cout << "PARENT " << fdchild << '\n';
            IsExecuted = true;
        }
    }
    else
    {
        std::cout << "after child " << fdchild << '\n';
        return;
    }
}
