#include "../headers/webserver.hpp"

Cgi::Cgi(){}
Cgi::~Cgi(){}

void    Cgi::SetEnv( Request& Req )
{
    env.push_back("REQUEST_METHOD=" + Req.getMethod());
    env.push_back("SCRIPT_NAME=" + Req.GetScriptName());
    env.push_back("SCRIPT_FILENAME=" + Req.GetScriptPath());
    env.push_back("QUERY_STRING=" + Req.getQueryString());
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    // env.push_back("SERVER_SOFTWARE=YourServer/1.0");
    env.push_back("REQUEST_URI=" + Req.getUri());
}