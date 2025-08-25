#pragma once

#include "Request.hpp"

class Cgi
{
    private:
        std::string                 script_path;
        std::string                 script_name;
        pid_t                       child_pid;
        std::vector<std::string>    env;

    public:
        Cgi();
        ~Cgi();
        Cgi(const Cgi &other);
        Cgi& operator=(const Cgi &other);
        void        SetEnv( Request& Req );
};