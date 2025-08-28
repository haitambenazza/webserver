#include "../headers/webserver.hpp"

Cgi::Cgi(){}
Cgi::Cgi( Request& Req ){
    SetEnv(  Req );
}
Cgi::~Cgi(){}

void    Cgi::SetEnv( Request& Req )
{
    env.push_back("REQUEST_METHOD=" + Req.getMethod());
    env.push_back("SCRIPT_NAME=" + Req.GetScriptName());
    env.push_back("SCRIPT_FILENAME=" + Req.GetScriptPath());
    env.push_back("QUERY_STRING=" + Req.getQueryString());
    env.push_back("CONTENT_TYPE=" + Req.getHeaderValue("Content-Type"));
    env.push_back("CONTENT_LENGTH=" + Req.getHeaderValue("Content-Length"));

    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    // env.push_back("SERVER_SOFTWARE=YourServer/1.0");
    env.push_back("REQUEST_URI=" + Req.getUri());
    ExecuteCgi(Req);
}

std::vector<char *> Cgi::GetEnvCgi()
{
    std::vector<char *> EnvVars;
    for (size_t i = 0; i < env.size(); i++)
    {
        EnvVars.push_back((char *)env[i].c_str());
        std::cout << EnvVars.back() << std::endl;
    }
    EnvVars.push_back(NULL);
    return (EnvVars);
}

void    Cgi::ExecuteCgi(Request &req)
{
    if (pipe(fd) < 0)
    {
        std::cerr << "Pipe creation failed" << std::endl;
        return;
    }
    
    child_pid = fork();
    if (child_pid == -1)
    {
        std::cerr << "Fork failed" << std::endl;
        close(fd[0]);
        close(fd[1]);
        return;
    } 
    else if (child_pid == 0)
    {
        close(fd[0]);
        dup2(fd[1] , STDOUT_FILENO);
        close(fd[1]);

        std::vector<char *> envVars = GetEnvCgi();

        char *cmds[3];
        cmds[0] = (char *)req.GetScriptPath().c_str();
        cmds[1] = (char *)req.GetScriptName().c_str();
        cmds[2] = NULL;
        if (execve(cmds[0] , cmds , envVars.data()) == -1)
            perror("execve");
        exit(1);
    }
    else
    {
        close(fd[1]);
        char buffer[4096];
        ssize_t bytes_read;
        output.clear();

        while ((bytes_read = read(fd[0] , buffer , sizeof(buffer) - 1)) > 0)
        {
            buffer[bytes_read] = '\0';
            output += buffer;
        }
        close(fd[0]);

        int status;

        waitpid(child_pid , &status , 0);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cerr << "CGI script execution failed" << std::endl;
            output = "Content-Type: text/html\r\n\r\n<h1>500 Internal Server Error</h1>";
        }
    }
}