
#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "Includes.hpp"
#include "Server.hpp"



class Location
{
    private :
        std::string                                             Path;
        std::map < std::string, std::vector< std::string > >    Commands;
        std::string                                             CgiEnabled;
        std::string                                             AutoIndex;
        bool                                                    AutoIndexStatus;
        std::string                                             Redirect;
        std::vector<std::string>                                AllowedMethods;
        std::map<std::string, std::string>                      PathsToCgi;
    public :
        Location();
        Location( const Location&  copy );
        Location&   operator=( const Location&  copy );
        ~Location();
        void                                                    SetLocation();
        std::map < std::string, std::vector< std::string > >    GetCommands() const;
        std::string&                                            GetPath();
        void                                                    SetPath( std::string s );
        void                                                    SetCommands( std::map < std::string, std::vector< std::string > >& Ref );
        std::string&                                            GetCgiStatus();
        std::string&                                            GetAutoIndex();
        void                                                    SetAutoIndexStatus(bool stat);
        void                                                    SetCgiStatus( std::string s );
        void                                                    SetLocationStatus( std::map < std::string, std::vector< std::string > >& Map );
        std::vector< std::string>                               GetItemsFromServer( std::string s, Server& Serv );
        std::vector<std::string>                                GetValuesLocation(std::string key);
        bool                                                    GetAutoIndexStatus() const;
        std::vector<std::string>                                GetAllowedMethods() const;
        bool                                                    SetAllowedMethods( std::vector<std::string> s );
        void                                                    SetRedirect(std::string s);
        std::string                                             GetRedirect() const;
        void                                                    SetCgiPathMap( std::string &s );
        std::map<std::string, std::string>                      GetCgiPathMap() const;
};
#endif
