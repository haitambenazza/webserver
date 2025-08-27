#include "../headers/webserver.hpp"

bool	Server::SetAddrServer(struct sockaddr_in *addr)
{

	memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = StrToIp(ip.c_str());
    addr->sin_port = htons(atoi(port.c_str()));
    return (true);
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
    if (setsockopt(fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		return false;
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
    status = copy.status;
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

std::map<u_int16_t , std::string>   Server::GetErrorMap() const
{
    return (error_map);
}

void      Server::SetErrorMap( u_int16_t key, std::string value )
{
    error_map.insert(std::make_pair(key, value));
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
			Locations.push_back( NewLocation );
		}
        SetServers( children[i] );
    }
    return (true);
}

bool	Server::StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag )
{
	std::vector< std::string > 	tmp;
	std::string					key;
	std::vector< std::string >  values;
	int 						i;

    (void)flag;
    if (s.empty())
    {
        return (false);
    }
	tmp = split(s, ";");
	i = 0;
	while ( i < (int)tmp.size() )
	{
        // if (flag)
        // {
        //     if (((split( tmp[i], " " )[0] != "error_page" ) && split( tmp[i], " " ).size() != 2))
        //     {
        //         tmp.clear();
        //         std::cerr << "wrong directive format\n";
        //         status = false;
        //         return false;
        //     }
        // }
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
        if (it->second.size() != 1)
            return (std::cerr<< "location " << L.GetPath() << " has invalid arguments\n", false);
        else if ((it->first == "cgi_enable" || it->first == "autoindex" || it->first == "upload_enable"))
        {
            if (it->second[0] != "on" && it->second[0] != "off")
                return (std::cerr<< "location00 " << L.GetPath() << " has invalid arguments\n", false);
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
    if (GetValuesFromKeys(Commands, "error_page") != "")
    {
        if ( CheckCommandServer(*this, "error_page", 2) == false || AllDigit( GetValuesFromKeys(Commands, "error_page")) == false)
            return(std::cerr << "invalid error_page\n", false);
        std::map<std::string, std::vector<std::string> >::iterator it;
        it = Commands.find("error_page");
        SetErrorMap(atoi(it->second[0].c_str()), it->second[1]);
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
    if (status)
        freeaddrinfo(result);
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
