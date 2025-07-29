
#pragma once
#include "Includes.hpp"
#include "../Includes/Block.hpp"
#include "../headers/webserver.hpp"

class Server
{
    private :
        std::vector <std::string>                               Data;
        std::vector <std::string>                               keys;
        std::map < std::string, std::vector< std::string > >    Commands;
        std::vector < Location >                                Locations;
        int16_t	                                                fd;

		std::string	                                            server_name;
		std::string	                                            port;
		std::string	                                            ip;
		std::string                                             root;
		std::string                                             index;
		u_int64_t	                                            max_body_size;
		std::map<u_int16_t , std::string>                       error_map;
        bool                                                    status;
    public :
        Server();
        Server( const Server& copy );
        Server& operator=( const Server& copy );
        ~Server();

        void                                                    SetServer( Block& block);
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::vector < Location >&                               GetLocations();
        void	                                                StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag );
        std::vector<std::string>                                GetKeys();
        bool                                                    SetServer();
        int                                                     Getfd() const;
        std::string                                             GetServerName()const;
        void    InitializeServerSettings();
        void    PrintData();
        void    SetDefaultValue();
        void	SetAddrServer(struct sockaddr_in *addr);
        std::string GetIp() const;
        std::string GetPort() const;
        bool GetStatus() const;
        void SetStatus(bool stat);
};
