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
        int                         fd[2];
        int 						client_socket;
        bool 						use_chunked;

    public:
        Cgi();
        Cgi( Request& Req );
		Cgi( Request& Req , int client_fd , bool chuncked);
        ~Cgi();
        Cgi(const Cgi &other);
        Cgi& operator=(const Cgi &other);
        void        SetEnv( Request& Req );
        std::vector<char *> GetEnvCgi();
        void            ExecuteCgi(Request &req);
		void 			ExecuteCgiChunked(Request& req);


		//for chuncked
		std::string toHex(size_t value);
		void sendChunk(const std::string& data);
		void sendChunkedHeaders();
		void endChunking();
};