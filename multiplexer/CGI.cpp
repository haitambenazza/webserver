#include "../headers/webserver.hpp"
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
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
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
    if (pipe(ParentFd) < 0)
    {
        std::cerr << "Parent pipe creation failed" << std::endl;
        return;
    }
    if (pipe(ChildFd) < 0)
    {
        std::cerr << "Child Pipe creation failed" << std::endl;
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
        // std::cout << "YAAAAARBIIII" << std::endl;
        close(ParentFd[0]); // close stdout parent
        close(ChildFd[1]); // close stdin child
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
        
        std::vector<char *> envVars = GetEnvCgi();
        
        (void) req;
        // char *cmds[3];
        // cmds[0] = (char *)"/bin/python3";
        // cmds[1] = (char *)req.GetScriptPath().c_str();
        
        // std::cerr << "cmd[1] == " << req.GetScriptPath() << std::endl;
    
        // cmds[2] = NULL;
        char* cmds[3] = { (char *)"/bin/python3", (char *)"www/bin/hello.py", NULL };
        if (!access("www/bin/hello.py", F_OK)) {
            std::cerr << "aaaaaaaaaa" << std::endl;
        }
        if (execve(cmds[0] , cmds , NULL) == -1)
            perror("execve");
        exit(1);
    }
    else
    {
        close(ParentFd[1]);
        close(ChildFd[0]);

        output.clear();
        output = "";

        const char* input = "Hello\n";  // note newline
        if (write(ChildFd[1], input, strlen(input)) == -1) {
            perror("write");
        }
        close(ChildFd[1]);

        char buffer[4096];
        ssize_t bytes_read;

        while ((bytes_read = read(ParentFd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            std::cerr << "Bytes readed: " << bytes_read << std::endl;
            std::cerr << "buffer: " << buffer << std::endl;
            output += buffer;
        }
        if (bytes_read == -1) {
            perror("read");
        }
        close(ParentFd[0]);

        int status;
        waitpid(child_pid, &status, 0);

        std::cerr << "Output from child :\n" << output << std::endl;

    }
}
