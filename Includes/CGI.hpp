#pragma once

#include "Request.hpp"

class Multiplexer;
class Cgi
{
    private:
        std::string                 script_path;
        std::string                 script_name;
        std::string                 output;
        pid_t                       child_pid;
        std::vector<std::string>    env;
        int                         ParentFd[2];
        int                         ChildFd[2];
        bool                        IsExecuted;
        time_t                      ForkTime;

    public:
        Cgi();
        Cgi(Multiplexer& m, Request Req, Location& loc, std::string& filepath);
        ~Cgi();
        Cgi(const Cgi& other);
        Cgi& operator=(const Cgi& other);
        void        SetEnv( Request& Req );
        std::vector<char *> GetEnvCgi();

        std::string     GetOutput();
        void            ExecuteCgi( Multiplexer& m, Request& Req , Location& loc, std::string filepath);
        void            SetOutput(std::string s);
        bool            PipePipes();
        bool            CgiFork();
        void            ExecCgiChild(std::vector<char *> envp, std::string& CgiPath , std::string& filepath);
        bool            AddToEpollCgi(Multiplexer& m);
        bool            CheckExitStatus();
        int             Getpipefd() const;
        pid_t           GetChildPid() const;
        void            SetChildPid(pid_t child);
        time_t          GetForkTime() const;
};
void	HandleCgi( Server& server, Multiplexer& m, int &i);
