#pragma once

#include "../headers/webserver.hpp"

class Method
{
    private:
        
    public:
        Method();
        Method( const Method& copy );
        Method& operator=( const Method& copy );
        ~Method();
        int GetMethod();
        int PostMethod();
        int DeleteMethod();
};


bool	GetRequest(std::string buffer, Server &server);
std::string GetValuesFromKeysReq(std::map<std::string, std::string >& map, std::string key);
