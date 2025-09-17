#pragma once

#include "Request.hpp"
#include "Client.hpp"


class Multiplexer;
class Client;
class Cgi
{
    private:
        std::string                 script_path;
        std::string                 script_name;
        std::string                 output;
        pid_t                       child_pid;
        std::vector<std::string>    env;
        int16_t                     fdchild;
        bool                        IsExecuted;
        time_t                      ForkTime;
        std::string                 filepath;
        bool                        isdone;
        pid_t                       pidchild;
        bool                        readDone;
        std::string                 tmpfile;
        int                         pipes[2];
    public:
        Location                    location;
        Cgi();
        Cgi(Request Req, Location& loc, std::string& filepath);
        ~Cgi();
        Cgi(const Cgi& other);
        Cgi& operator=(const Cgi& other);
        void        SetEnv( Request& Req );
        std::vector<char *> GetEnvCgi();

        std::string     GetOutput();
        HttpStatus      ExecuteCgi(Client &cl, Request & Req, Location& loc, std::string filepath);
        void            SetOutput(std::string s);
        bool            CgiFork();
        void            ExecCgiChild(std::string Method, std::vector<char *> envp, std::string& CgiPath , std::string& filepath);
        bool            CheckExitStatus();
        pid_t           GetChildPid() const;
        void            SetChildPid(pid_t child);
        time_t          GetForkTime() const;
        bool            GetExecutedStatus()const;
        int16_t         GetFdChild() const;
        void            SetFilePath(std::string& path);
        std::string     GetFilePath();
        Location        GetLocation();
        void            SetLocation(Location& loc);
        bool            ReadStatus() const;
        void            SetReadStatus(bool stat);
        bool            GetExecutionstatus() const;
        std::string     GetTmpFile() const;
    };
void	HandleCgi( Server& server, Multiplexer& m, int &i);

