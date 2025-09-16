#include "../headers/webserver.hpp"
int     IsWhiteSpace( char c );

bool	Server::SetAddrServer(struct sockaddr_in *addr)
{

	memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = htonl(StrToIp(ip.c_str()));
    addr->sin_port = htons(atoi(port.c_str()));
    return (true);
}
void     Server::SetDefaultMap()
{
    // DefaultError_map.insert(std::make_pair(200,"error_pages/200.html"));
    // DefaultError_map.insert(std::make_pair(201,"error_pages/201.html"));
    DefaultError_map.insert(std::make_pair(400,"error_pages/400.html"));
    DefaultError_map.insert(std::make_pair(403,"error_pages/403.html"));
    DefaultError_map.insert(std::make_pair(404,"error_pages/404.html"));
    DefaultError_map.insert(std::make_pair(405,"error_pages/405.html"));
    DefaultError_map.insert(std::make_pair(413,"error_pages/413.html"));
    DefaultError_map.insert(std::make_pair(414,"error_pages/414.html"));
    DefaultError_map.insert(std::make_pair(415,"error_pages/415.html"));
    DefaultError_map.insert(std::make_pair(500,"error_pages/500.html"));
    DefaultError_map.insert(std::make_pair(501,"error_pages/501.html"));
    DefaultError_map.insert(std::make_pair(505,"error_pages/505.html"));
}
void    Server::SetDefaultValue()
{
    root = ROOT;
    port = PORT;
    ip = IP;
    fd = -1;
    index = INDEX;
    server_name = SERVER_NAME;
    max_body_size = MAX_CLIENT_BODY;
}

bool    Server::SetServer()
{
    struct sockaddr_in addr;
    int opt = 1;

    SetDefaultValue();
    if (InitializeServerSettings() == false)
    {
        status = false;
        return (false);
    }
    fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd == -1)
        return false;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        return false;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1)
    {
        return false;
    }
	if (SetAddrServer(&addr) == false)
        return (false);
	if ((bind(fd, (sockaddr*)&addr, sizeof(addr))) == -1)
        return false;
    if ((listen(fd, SOMAXCONN)) == -1)
        return false;
    return true;
}

void Server::PrintData()
{
    std::cout << "NAME : " << this->server_name << "\n";
    std::cout << "IP : " << this->ip << "\n";
    std::cout << "PORT : " << this->port << "\n";
    std::cout << "INDEX : " << this->index << "\n";
    std::cout << "ROOT : " << this->root << "\n";
    std::cout << "MAX_BODY_SIZE : " << this->max_body_size << "\n";
}

std::string Server::GetServerName()const
{
    return (server_name);
}

Server::Server()
{
    status = true;
    if (!SetServer())
    {
        status = false;
        return ;
    }
}

Server::Server( const Server& copy ) : keys(copy.keys), Locations(copy.Locations)
{
    status = true;
    keys = copy.keys;
    Locations = copy.Locations;
    Commands = copy.Commands;
    Args = copy.Args;
    status = copy.status;
    error_map = copy.error_map;
    DefaultError_map = copy.DefaultError_map;
    max_body_size = copy.max_body_size;
    if (!SetServer())
    {
        status = false;
        return ;
    }
}

Server& Server::operator=( const Server& copy )
{
    if (this != & copy)
    {
        keys = copy.keys;
        Locations = copy.Locations;
        Commands = copy.Commands;
        Args = copy.Args;
        error_map = copy.error_map;
        DefaultError_map = copy.DefaultError_map;
        max_body_size = copy.max_body_size;
        close(fd);
        if (SetServer() == false)
            return (*this);
    }
    return (*this);
}

std::vector< Location >&     Server::GetLocations()
{
    return (Locations);
}

std::vector<std::string>    Server::GetKeys()
{
    return (keys);
}

std::map<int , std::string>   Server::GetErrorMap() const
{
    return (error_map);
}

std::string     Server::GetArgs() const
{
    return (Args);
}

void      Server::SetErrorMap()
{
    std::vector<std::string> tmp;

    tmp = split(Args, ";");
    if (tmp.empty())
    {
        return ;
    }
    for (size_t i = 0; i < tmp.size(); i++)
    {
        if ( tmp[i].find("error_page") != std::string::npos )
        {
            std::vector<std::string> tmp1;
            tmp1 = split(tmp[i], " ");
            if ( tmp1.empty() )
            {
                std::cerr << "invalid error_page"<< std::endl;
            }
            error_map.insert(std::make_pair(atoi(tmp1[1].c_str()), tmp1[2]));
        }
    } 
}

void       Server::SetArgs(std::string s)
{
    Args = s;
}

bool Server::SetServers( Block& block )
{
	std::vector<Block>& children = block.GetBlocks();
	int 	i;
	i = -1;
    while ( ++i < (int)children.size() )
	{
		if ( children[i].GetLvl() == 1 )
        {
            if (StringToMap(children[i].GetArg(), Commands, 1) == false)
                return (false);
        }
		else if ( children[i].GetLvl() == 2 )
		{
            std::vector<std::string> lst;
            lst = split( children[i].GetName(), " " );
			std::map < std::string, std::vector< std::string > >  Com;
			Location NewLocation;
			if (StringToMap( children[i].GetArg(), Com, 0 ) == false)
                return (false);
            NewLocation.SetLocationStatus( Com );
			NewLocation.SetCommands( Com );
            if ( lst.size() != 1 )
                NewLocation.SetPath( lst[1] );
            else
                status = false;
            if (NewLocation.GetCgiStatus() == "on")
            {
                NewLocation.SetCgiPathMap(children[i].GetArg());
            }
			Locations.push_back( NewLocation );
		}
        SetServers( children[i] );
    }
    return (true);
}

std::map<int , std::string>             Server::GetDefaultErrorMap() const
{
    return (DefaultError_map);
}

bool	Server::StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag )
{
	std::vector< std::string > 	tmp;
	std::string					key;
	std::vector< std::string >  values;
	int 						i;

    if (s.empty())
    {
        return (false);
    }
	tmp = split(s, ";");
	i = 0;
	while ( i < (int)tmp.size() )
	{
        key = split( tmp[i], " " )[0];
        if (flag)
            keys.push_back(key);
        values = FillVector( split(tmp[i], " ") );
        Mp.insert(std::make_pair(key, values));
		i++;
	}
    return (true);
}

int Server::Getfd() const{
    return (fd);
}

void              Server::SetMaxBodySize( std::string val )
{
    max_body_size = atoll(val.c_str());
}
u_int64_t         Server::GetMaxBodySize() const
{
    return (max_body_size);
}



bool CheckCommandServer(Server& s, std::string ToFind, size_t size)
{
    std::map<std::string , std::vector<std::string> >mp = s.GetCommands();
    std::map<std::string , std::vector<std::string> >::iterator it = mp.find(ToFind);

    if (it != mp.end())
    {
        if (it->second.size() != size)
            return (false);
    }
    return (true);
}

bool CheckCommandLocation(Location& L)
{
    std::map<std::string , std::vector<std::string> >mp = L.GetCommands();
    std::map<std::string , std::vector<std::string> >::iterator it = mp.begin();

    while (it != mp.end())
    {
        if (it->first == "redirect")
        {
            L.SetPath(it->second[0]);
        }
        if (it->first[0] == '_' && (!it->first[1] || IsWhiteSpace(it->first[1]) || it->first[1] == '_' || !std::isalpha(it->first[1]) ))
        {
            return (std::cerr<< "location " << L.GetPath() << " has invalid cgi extention\n", false);
        }
        if (it->first != "allowed_methods" && it->second.size() != 1)
            return (std::cerr<< "location " << L.GetPath() << " has invalid arguments\n", false);
        else if ((it->first == "cgi_enable" || it->first == "autoindex"))
        {
            if (it->second[0] != "on" && it->second[0] != "off")
                return (std::cerr<< "location " << L.GetPath() << " has invalid arguments\n", false);
        }
        else if ( it->first == "allowed_methods" )
        {
            if (L.SetAllowedMethods(it->second) == false)
                return (false);
        }
        it++;
    }
    return (true);
}

bool    Server::InitializeServerSettings()
{
    std::vector<Location> locs = Locations;
    
    if (GetValuesFromKeys(Commands, "listen") != "")
    {
        if (CheckCommandServer(*this, "listen", 1) == false || AllDigit( GetValuesFromKeys(Commands, "listen")) == false)
            return(std::cerr << "invalid port number\n", false);
        port = GetValuesFromKeys(Commands, "listen");
    }
    if ( GetValuesFromKeys(Commands, "server_name") != "")
    {
        if (CheckCommandServer(*this, "server_name", 1) == false)
            return(std::cerr << "invalid server_name\n", false);
        server_name = GetValuesFromKeys(Commands, "server_name");
    }
    if (GetValuesFromKeys(Commands, "host") != "")
    {
        if (CheckCommandServer(*this, "host", 1) == false || CheckIp(GetValuesFromKeys(Commands, "host")) == false)
            return (std::cerr << "invalid ip address\n", false);
        ip = GetValuesFromKeys(Commands, "host");
    }
    if (GetValuesFromKeys(Commands, "root") != "")
    {
        if (CheckCommandServer(*this, "root", 1) == false)
            return(std::cerr << "invalid root\n", false);
        root = GetValuesFromKeys(Commands, "root");
    }
    if (GetValuesFromKeys(Commands, "index") != "")
    {
        if (CheckCommandServer(*this, "index", 1) == false)
            return (std::cerr << "invalid index\n", false);
        index = GetValuesFromKeys(Commands, "index");
    }
    if (GetValuesFromKeys(Commands, "Max_Client_Body_size") != "")
    {
        if (CheckCommandServer(*this, "Max_Clent_Body_size", 1) == false || AllDigit( GetValuesFromKeys(Commands, "Max_Client_Body_size")) == false)
            return(std::cerr << "invalid Max_Client_Body_size\n", false);
        max_body_size = atoll(GetValuesFromKeys(Commands, "Max_Client_Body_size").c_str());
    }
    size_t i = 0;
    while (i < locs.size())
    {
        if (CheckCommandLocation(locs[i]) == false)
            return (false);
        i++;
    }
    return (true);
}

std::string Server::GetIp() const
{
    return (ip);
}

std::string Server::GetPort() const
{
    return (port);
}

std::map < std::string, std::vector< std::string > >    Server::GetCommands()
{
    return (Commands);
}

Server::~Server()
{
    if (fd)
        close(fd);
}

bool Server::GetStatus() const
{
    return (status);
}

void Server::SetStatus(bool stat)
{
    status = stat;
}

std::string Server::GetRoot() const
{
    return (root);
}

void    Server::AddNewClient(int16_t fd)
{
    this->ClientFd.push_back(fd);
}

std::vector<int16_t>    Server::GetClients() const
{
    return (ClientFd);
}