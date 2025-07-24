/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/07/25 00:57:44 by amoubine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../headers/webserver.hpp"

void	Server::SetAddrServer(struct sockaddr_in *addr)
{
    (void)serv;
	memset(addr, 0, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = htonl(StrToIp(ip));
	addr->sin_port = htons((uint16_t)atoi(port.c_str()));
}

void    Server::SetDefaultValue()
{
    port = PORT;
    ip = IP;
    root = ROOT;
    fd = -1;
    index = INDEX;
    server_name = SERVER_NAME;
    max_body_size = MAX_CLIENT_BODY;
}

bool    Server::SetServer()
{
    int opt;

    SetDefaultValue();
    InitializeServerSettings();
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

Server::Server( const char *filename )
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
            std::cout << children[i].GetArg() << '\n';
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
        // std::cout << "  hey   "<<tmp[i] << '\n';
        if (key == "server")
        {
            std::cerr << " Nested server" << std::endl;
            exit(1);
        }
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


std::map < std::string, std::vector< std::string > >    Server::GetCommands()
{
    return (Commands);
}

Server::~Server()
{
    if (fd != -1)
        close(fd);
}
