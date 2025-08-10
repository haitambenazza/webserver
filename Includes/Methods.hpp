#pragma once

#include "../headers/webserver.hpp"
#include "Includes.hpp"

class Request;

class Method
{
    private:
        
    public:
        Method();
        Method( const Method& copy );
        Method& operator=( const Method& copy );
        ~Method();
        int GetMethod();
        int PostMethod( Request& Req );
        int DeleteMethod(  Request& Req  );
};

bool	    GetRequest(std::string buffer, Server &server);
std::string GetValuesFromKeysReq(std::map<std::string, std::string >& map, std::string key);

//post
//check if upload enabled 
//if so create file with name sent and save body in it
// return 201 (if file already exists then override ) or 200 // to check
// if open file or save in file failed return 500

