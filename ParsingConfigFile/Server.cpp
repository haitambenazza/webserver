/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 05:28:16 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/21 00:20:00 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserver.hpp"


std::string Server::GetIp() const
{
    return (ip);
}
std::string Server::GetPort() const
{
    return (port);
}

void	SetAddrServer(struct sockaddr_in *addr, const Server &serv)
{
    (void)serv;
	memset(addr, 0, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = htonl(StrToIp(serv.GetIp().c_str()));
	addr->sin_port = htons(atoi(serv.GetPort().c_str()));
}

void    Server::SetDefaultValues()
{
    ip = IP_ADDRESS;
    port = PORT;
    max_body_size = MAX_CLIENT_BODY;
    server_name = "";
    fd = -1;
    fd_endpoint = -1;
    root = "/";
    index = "/index.html";
}

bool    Server::SetServer()
{
    struct sockaddr_in addr;
    int opt;

    opt = 1;
    this->SetDefaultValues();
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    return (perror("Socket"), false);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    setsockopt(fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT | SO_LINGER, &opt, sizeof(opt));
	this->InitializeServerSettings();
    SetAddrServer(&addr, *this);
	if (!server_name.empty() && (bind(fd, (sockaddr*)&addr, sizeof(addr))) == -1)
        return (perror("Bind"), false);
    if (!server_name.empty() && (listen(fd, SOMAXCONN)) == -1)
        return (perror("Listen"), false);
    return true;
}

void Server::Setfd_endpoint(int16_t fd)
{
    fd_endpoint = fd;
}

std::string GetValuesFromKeys(std::map<std::string, std::vector<std::string> >& map, std::string key)
{
    std::map<std::string, std::vector<std::string> >::iterator  it;
    std::vector< std::string>                                   values;

    it = map.find(key);
    if (it != map.end())
    {
        values = it->second;
        return (values[0]);
    }
    return "";
}

void    Server::InitializeServerSettings()
{
    if (GetValuesFromKeys(Commands, "listen").size())
        port = GetValuesFromKeys(Commands, "listen");
    if (GetValuesFromKeys(Commands, "server_name").size())
        server_name = GetValuesFromKeys(Commands, "server_name");
    if (GetValuesFromKeys(Commands, "host").size())
        ip = GetValuesFromKeys(Commands, "host");
    if (GetValuesFromKeys(Commands, "root").size())
        root = GetValuesFromKeys(Commands, "root");
    if (GetValuesFromKeys(Commands, "index").size())
        index = GetValuesFromKeys(Commands, "index");
}

void Server::PrintData()
{
    std::cout << "FD : " << fd << "\n";
    std::cout << "IP : " << ip << "\n";
    std::cout << "PORT : " << port << "\n";
    std::cout << "INDEX : " << index << "\n";
    std::cout << "ROOT : " << root << "\n";
    std::cout << "MAX_BODY_SIZE : " << max_body_size << "\n";
    std::cout << "server_name : " << server_name << "\n";
}


int16_t	Server::Getfd_endpoint() const
{
    return (fd_endpoint);
}

std::string Server::GetServerName()const
{
    return (server_name);
}

Server::Server()
{
    SetDefaultValues();
}

Server::Server( bool flag )
{
    (void)flag;
    SetDefaultValues();
}

Server::Server( const Server& copy )
{
    Data = copy.Data;
    keys = copy.keys;
    Locations = copy.Locations;
    Commands = copy.Commands;
    SetDefaultValues();
}

Server::Server(const char *filename)
{
    Block 	                    NewBlock;
	std::string              	data;
	int 						x;
	int 						y;
	File                        file( filename );

	if (file.SetExtention() == 1)
		return ;
	file.OpenFile();
	file.ReadLines();
	data = GetServers( file.GetRawString() );
	if (data.empty())
        return ;
    x = 0;
    y = 0;
    if (data.size())
        NewBlock.FillBlock(data, NewBlock, x, y);
    SetServer(NewBlock);
    SetServer();
}

Server& Server::operator=( const Server& copy )
{
    if (this != & copy)
    {
        keys = copy.keys;
        Data = copy.Data;
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

void Server::SetServer( Block& block)
{
	std::vector<Block>& children = block.GetBlocks();
	size_t 	i;

	i = 0;
    while ( i < children.size() )
	{
		if ( children[i].GetLvl() == 1 )
        {
			StringToMap(children[i].GetArg(), Commands, 1);
        }
		else if ( children[i].GetLvl() == 2 )
		{
            std::vector<std::string> lst;
            lst = split(children[i].GetName(), " ");
			std::map < std::string, std::vector< std::string > >  Com;
			Location NewLocation;
			StringToMap( children[i].GetArg(), Com, 0 );
			NewLocation.SetCommands( Com );
            if ( lst.size() != 1 )
                NewLocation.SetPath( lst[1] );
            else
            {
                std::cerr << "Location has no path " << std::endl;
                return ;
            }
			Locations.push_back( NewLocation );
		}
        SetServer( children[i] );
		i++;
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


std::map < std::string, std::vector< std::string > >    Server::GetCommands()
{
    return (Commands);
}

int Server::CloseFd()
{
    return (close(fd_endpoint));
}

Server::~Server()
{
    if (fd != -1)
        close(fd);
}
