#include "../headers/webserver.hpp"

Location::Location(  )
{
    UploadEnable = "off";
    CgiEnabled = "off";
    AutoIndex = "off";
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

std::string&       Location::GetAutoIndex()
{
    return (AutoIndex);
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

std::vector<std::string> Location::GetValuesLocation( std::string key)
{
    std::map<std::string, std::vector<std::string> >::iterator  it;
    std::vector< std::string>                                   values;

    it = Commands.find(key);
    if (it != Commands.end())
    {
        values = it->second;
        return (values);
    }
    else
        return std::vector<std::string>();
}

std::vector< std::string>      Location::GetItemsFromServer( std::string s, Server& Serv )
{
    std::map<std::string, std::vector<std::string > > map;
    std::map<std::string, std::vector<std::string > >::iterator it;

    map = Serv.GetCommands();
    it = map.find(s);
    std::cout << "find has found  === " << it->first << std::endl;
    if (it != map.end())
    {
        return (it->second);
    }
    return std::vector< std::string >();
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
        else if (it->first == "autoindex" && it->second[0] == "on")
            AutoIndex = "on";
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
