#include "../headers/webserver.hpp"

Location::Location(  )
{

}

Location::Location( const Location&  copy )
{
    Path = copy.Path;
    Commands = copy.Commands;
}

Location&   Location::operator=( const Location&  copy )
{
    if ( this != &copy )
    {
        Path = copy.Path;
        Commands = copy.Commands;
    }
    return (*this);
}

void        Location::SetCommands( std::map < std::string, std::vector< std::string > >& Ref )
{
    Commands = Ref;
}


std::string&         Location::GetPath()
{
    return (Path);
}
void                 Location::SetPath( std::string& s )
{
    Path = s;
}

std::map < std::string, std::vector< std::string > >    Location::GetCommands()
{
    return (Commands);
}

Location::~Location()
{

}
