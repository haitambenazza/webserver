
#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "Includes.hpp"
#include "../headers/webserver.hpp"

class Server;

class Location
{
    private :
        std::string                                             Path;
        std::map < std::string, std::vector< std::string > >    Commands;
        std::string                                             AutoIndex;
        std::string                                             UploadEnable;
        std::string                                             CgiEnabled;
        // std::string                                             root;
        // std::string                                             index;
    public :
        Location();
        Location( const Location&  copy );
        Location&   operator=( const Location&  copy );
        ~Location();
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::string&                                            GetPath();
        void                                                    SetPath( std::string& s );
        void                                                    SetCommands( std::map < std::string, std::vector< std::string > >& Ref );
        std::string&                                            GetUploadStatus();
        std::string&                                            GetCgiStatus();
        void                                                    SetUploadStatus( std::string s );
        void                                                    SetCgiStatus( std::string s );
        void                                                    SetLocationStatus( std::map < std::string, std::vector< std::string > >& Map );
        std::vector< std::string>                               GetItemsFromServer( std::string& s, Server& Serv );
};

#endif
