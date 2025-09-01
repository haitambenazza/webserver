#pragma once
#include "Includes.hpp"
#include "../Includes/Block.hpp"

class Location;
class Server
{
    private :
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
		std::map<int , std::string>                             error_map;
        bool                                                    status;
        std::vector<int16_t>                                    ClientFd;

    public :
        Server();
        Server( const Server& copy );
        Server& operator=( const Server& copy );
        ~Server();

        bool                                                    SetServers( Block& block );
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::vector < Location >&                               GetLocations();
        bool	                                                StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag );
        std::vector<std::string>                                GetKeys();
        bool                                                    SetServer();
        int                                                     Getfd() const;
        std::string                                             GetServerName()const;
        bool                                                    InitializeServerSettings();
        void                                                    PrintData();
        void                                                    SetDefaultValue();
        bool	                                                SetAddrServer(struct sockaddr_in *addr);
        std::string                                             GetIp() const;
        std::string                                             GetPort() const;
        bool                                                    GetStatus() const;
        void                                                    SetStatus(bool stat);
        std::string                                             GetRoot() const;
        void                                                    AddNewClient(int16_t fd);
        std::vector<int16_t>                                    GetClients() const;
        void                                                    SetMaxBodySize( std::string val );
        std::map<int , std::string>                             GetErrorMap() const;
        void                                                    SetErrorMap( int key, std::string value );
        u_int64_t                                               GetMaxBodySize() const;
};

std::vector<std::string>	FillVector( std::vector<std::string> Src );
void	                    PrintMap(std::map<std::string , std::vector <std::string> > Commands);
bool                        CheckBrackets( std::string s );
void	                    PrintServer( Server& Serv );
bool	                    CheckBrackets( std::string s );
std::vector<std::string>    GetServers( std::string& s );
std::vector<Server>         GetFullServers( char* FileName );
bool	                    IsPresent(const std::vector<std::string>& vctr, std::string s);
bool                        CheckValidKeys(const std::vector<std::string> str);
bool	                    CheckLocationParams( Server &server );