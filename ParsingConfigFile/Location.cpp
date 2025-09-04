#include "../headers/webserver.hpp"

Location::Location(  )
{
    CgiEnabled = "off";
    AutoIndex = "off";
    AutoIndexStatus = false;
}

Location::Location( const Location&  copy )
{
    Path = copy.Path;
    Commands = copy.Commands;
    CgiEnabled = copy.CgiEnabled;
    AutoIndex = copy.AutoIndex;
    AutoIndexStatus = copy.AutoIndexStatus;  
}

Location&   Location::operator=( const Location&  copy )
{
    if ( this != &copy )
    {
        Path = copy.Path;
        Commands = copy.Commands;
        CgiEnabled = copy.CgiEnabled;
        AutoIndex = copy.AutoIndex;
        AutoIndexStatus = copy.AutoIndexStatus;
    }
    return (*this);
}

void        Location::SetCommands( std::map < std::string, std::vector< std::string > >& Ref )
{
    Commands = Ref;
}

std::vector<std::string>      Location::GetAllowedMethods() const
{
    return (AllowedMethods);
}

bool        Location::SetAllowedMethods( std::vector<std::string> s )
{
    int g = 0;
    int p = 0;
    int d = 0;

    if (s.size() == 0 || s.size() > 3)
        return (false);
    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == "POST")
            p++;
        else if (s[i] == "GET")
            g++;
        else if (s[i] == "DELETE")
            d++;
        else
            return (std::cerr<< "allowed methods : POST, GET, DELETE\n", false);
    }
    if (p < 2 && g < 2 && d < 2)
    {
        AllowedMethods = s;
        return (true);
    }
    return (false);
}



std::string&       Location::GetAutoIndex()
{
    return (AutoIndex);
}

std::string&       Location::GetCgiStatus()
{
    return (CgiEnabled);
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

void                Location::SetAutoIndexStatus(bool stat)
{
	AutoIndexStatus = stat;
}

std::map < std::string, std::vector< std::string > >    Location::GetCommands()
{
    return (Commands);
}

bool     Location::GetAutoIndexStatus() const
{
    return (AutoIndexStatus);
}

Location::~Location()
{

}
