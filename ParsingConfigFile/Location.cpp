#include "../headers/webserver.hpp"

Location::Location(  )
{
    UploadEnable = "off";
    CgiEnabled = "off";
}

Location::Location( const Location&  copy )
{
    Path = copy.Path;
    Commands = copy.Commands;
    UploadEnable = copy.UploadEnable;
    CgiEnabled = copy.CgiEnabled;
}

Location&   Location::operator=( const Location&  copy )
{
    if ( this != &copy )
    {
        Path = copy.Path;
        Commands = copy.Commands;
        UploadEnable = copy.UploadEnable;
        CgiEnabled = copy.CgiEnabled;
    }
    return (*this);
}

void        Location::SetCommands( std::map < std::string, std::vector< std::string > >& Ref )
{
    Commands = Ref;
}

std::string&       Location::GetUploadStatus()
{
    return (UploadEnable);
}
std::string&       Location::GetCgiStatus()
{
    return (CgiEnabled);
}
void               Location::SetUploadStatus( std::string s )
{
    UploadEnable = s;
}
void               Location::SetCgiStatus( std::string s )
{
    CgiEnabled = s;
}
void               Location::SetLocationStatus( std::map < std::string, std::vector< std::string > >& Map )
{
    std::map< std::string , std::vector< std::string> >::iterator it;

    it = Map.begin();
    while (it != Map.end())
    {
        if (it->first == "cgi_enable" && it->second[0] == "on")
            CgiEnabled = "on";
        else if (it->first == "upload_enable" && it->second[0] == "on")
            UploadEnable = "on";
        it++;
    }
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
