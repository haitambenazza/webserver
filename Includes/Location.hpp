
#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "Includes.hpp"
#include "Server.hpp"



class Location
{
    private :
        std::string                                             Path;
        std::map < std::string, std::vector< std::string > >    Commands;
        std::string                                             UploadEnable;
        std::string                                             CgiEnabled;
        std::string                                             AutoIndex;
        bool                                                    AutoIndexStatus;
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
        std::string&                                            GetAutoIndex();
        void                                                    SetAutoIndexStatus(bool stat);
        void                                                    SetUploadStatus( std::string s );
        void                                                    SetCgiStatus( std::string s );
        void                                                    SetLocationStatus( std::map < std::string, std::vector< std::string > >& Map );
        std::vector< std::string>                               GetItemsFromServer( std::string s, Server& Serv );
        std::vector<std::string>                                GetValuesLocation(std::string key);
        bool                                                    GetAutoIndexStatus() const;
};
#endif
