
#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "Includes.hpp"

class Location
{
    private :
        std::string                                             Path;
        std::map < std::string, std::vector< std::string > >    Commands;
    public :
        Location();
        Location( const Location&  copy );
        Location&   operator=( const Location&  copy );
        ~Location();   
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::string&                                            GetPath();
        void                                                    SetPath( std::string& s );
        void                                                    SetCommands( std::map < std::string, std::vector< std::string > >& Ref );
};

#endif