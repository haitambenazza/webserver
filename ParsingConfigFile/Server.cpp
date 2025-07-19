/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 05:28:16 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/19 01:53:27 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserver.hpp"

void	SetAddrServer(struct sockaddr_in *addr)
{
	memset(addr, 0, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr->sin_port = htons(8080);
}

bool    Server::SetServer()
{
    struct sockaddr_in addr;
    int opt = 1;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    if (fd == -1)
    {
        perror("Socket");
        return false;
    }
    setsockopt(fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	SetAddrServer(&addr);
	if ((bind(fd, (sockaddr*)&addr, sizeof(addr))) == -1)
    {
        perror("Bind");
        return false;
    }
    if ((listen(fd, SOMAXCONN)) == -1)
    {
        perror("Listen");
        return false;
    }
    return true;
}

void Server::Setfd_endpoint(int16_t fd)
{
    fd_endpoint = fd;
}


void    Server::InitializeServerSettings()
{
    for (size_t i = 0; i < this->keys.size(); ++i)
    {
        const std::string& currentKey = this->keys[i];

        std::map<std::string, std::vector<std::string> >::iterator it = this->Commands.find(currentKey);

        if (it != this->Commands.end()) {
            const std::vector<std::string>& values = it->second;
            if (currentKey == "listen")
            {
                if (!values.empty()) {
                    std::string listen_val = values[0];
                    size_t colon_pos = listen_val.find(':');
                    if (colon_pos != std::string::npos)
                        this->port = static_cast<u_int16_t>(atoi(listen_val.substr(colon_pos + 1).c_str()));
                    else
                    {
                        this->port = static_cast<u_int16_t>(atoi(listen_val.c_str()));
                    }
                }
            } else if (currentKey == "root")
            {
                if (!values.empty()) {
                    this->root = values[0];
                }
            } else if (currentKey == "index")
            {
                if (!values.empty()) {
                    this->index = values[0];
                }
            }
            else if (currentKey == "host")
            {
                if (values.empty())
                    this->ip = "0.0.0.0";
                else
                    this->ip = values[0];
            } else if (currentKey == "client_max_body_size")
            {
                if (!values.empty()) {
                    this->max_body_size = static_cast<u_int64_t>(atol(values[0].c_str()));
                }
            } else if (currentKey == "error_page")
            {
                if (values.size() >= 2) {
                    u_int16_t error_code = static_cast<u_int16_t>(atoi(values[0].c_str()));
                    std::string error_path = values[1];
                    this->error_map[error_code] = error_path;
                }
            }
        }
    }
}

void Server::PrintData()
{
    std::cout << "IP : " << this->ip << "\n";
    std::cout << "PORT : " << this->port << "\n";
    std::cout << "INDEX : " << this->index << "\n";
    std::cout << "ROOT : " << this->root << "\n";
    std::cout << "MAX_BODY_SIZE : " << this->max_body_size << "\n";
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
    if (this->SetServer() == false)
    {
        std::cerr << server_name <<" encountered an error\n";
        return ;
    }
}

Server::Server( const Server& copy )
{
    Data = copy.Data;
    keys = copy.keys;
    Locations = copy.Locations;
    Commands = copy.Commands;
    if (!SetServer())
    {
        std::cerr << server_name <<" encountered an error\n";
        return ;
    }
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
        if (key == "server")
        {
            std::cerr << " Nested server" << std::endl;
            exit(1);
        }
        if (flag)
        {
            keys.push_back(key);
        }
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
    close(fd);
}
