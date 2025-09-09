#pragma once

#include "Request.hpp"

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

    public:
        Cgi();
        Cgi( Multiplexer& m, Request Req , Location& loc, std::string& filepath  );
        ~Cgi();
        Cgi(const Cgi &other);
        Cgi& operator=(const Cgi &other);
        void        SetEnv( Request& Req );
        std::vector<char *> GetEnvCgi();

        std::string     GetOutput();
        void            ExecuteCgi( Multiplexer& m, Request& Req , Location& loc, std::string filepath );
        void            SetOutput(std::string& s);
};
void	HandleCgi( Server& server, Multiplexer& m, int &i );