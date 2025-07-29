
#pragma once
#include "Includes.hpp"


class Client
{
    private :
        int16_t   fd;
        // int16_t    endpoint_fd;
    public :
        Client();
        Client( const Client& copy );
        Client& operator=( const Client& copy );
        bool	SetClient();
        ~Client();

};