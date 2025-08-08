#include "../headers/webserver.hpp"

void	Server::SetAddrServer(struct sockaddr_in *addr)
{
    struct addrinfo hints;
    struct addrinfo *res;
    int status = 0;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
    if (status)
    {
        freeaddrinfo(res);
        perror("IP:Port");
        return ;
    }
    result = res;
	memset(addr, 0, sizeof(struct sockaddr_in));
    memcpy(addr, res->ai_addr, sizeof(sockaddr_in));
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
    InitializeServerSettings();
    fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    if (fd == -1)
        return false;
    if (setsockopt(fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		return false;
	SetAddrServer(&addr);
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
        close(fd);
        SetServer();
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

void Server::SetServers( Block& block )
{
	std::vector<Block>& children = block.GetBlocks();
	int 	i;
	i = -1;
    while ( ++i < (int)children.size() )
	{
		if ( children[i].GetLvl() == 1 )
            StringToMap(children[i].GetArg(), Commands, 1);
		else if ( children[i].GetLvl() == 2 )
		{
            std::vector<std::string> lst;
            lst = split( children[i].GetName(), " " );
			std::map < std::string, std::vector< std::string > >  Com;
			Location NewLocation;
			StringToMap( children[i].GetArg(), Com, 0 );
			NewLocation.SetCommands( Com );
            if ( lst.size() != 1 )
                NewLocation.SetPath( lst[1] );
            else
                status = false;
			Locations.push_back( NewLocation );
		}
        SetServers( children[i] );
    }
}

void	Server::StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag )
{
	std::vector< std::string > 	tmp;
	std::string					key;
	std::vector< std::string >  values;
	int 						i;

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
}

int Server::Getfd() const{
    return (fd);
}

void    Server::InitializeServerSettings()
{
    if (GetValuesFromKeys(Commands, "listen") != "")
        port = GetValuesFromKeys(Commands, "listen");
    if (GetValuesFromKeys(Commands, "server_name") != "")
        server_name = GetValuesFromKeys(Commands, "server_name");
    if (GetValuesFromKeys(Commands, "host") != "")
        ip = GetValuesFromKeys(Commands, "host");
    if (GetValuesFromKeys(Commands, "root") != "")
        root = GetValuesFromKeys(Commands, "root");
    if (GetValuesFromKeys(Commands, "index") != "")
        index = GetValuesFromKeys(Commands, "index");
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
    freeaddrinfo(result);
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
