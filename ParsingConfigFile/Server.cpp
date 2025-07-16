/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 05:28:16 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/16 19:52:50 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

Server::Server()
{
        std::cout << "SERVER  constructor\n";
    fd = -1;
}

u_int16_t Server::GetFd() const
{
    return (fd);
}

Server::Server( const Server& copy )
{
    std::cout << "server copy constructor\n";
    Data = copy.Data;
    Locations = copy.Locations;
    Commands = copy.Commands;
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
}

Server& Server::operator=( const Server& copy )
{
    if (this != & copy)
    {
        Data = copy.Data;
        fd = socket(AF_UNIX, SOCK_STREAM, 0);
        Locations = copy.Locations;
        Commands = copy.Commands;
    }
    return (*this);
}

void    Server::SetData( std::string s )
{
    Data = split(s, ";");
    int i = 0;
    while ( i < (int)Data.size())
    {
        std::cout << Data[i] << std::endl;
        i++;
    }
}

std::vector< Location >&     Server::GetLocations()
{
    return (Locations);
}


void Server::SetServer( Block& block)
{
	std::vector<Block>& children = block.GetBlocks();
	size_t 	i;

	i = 0;
    while ( i < children.size() )
	{
		if (children[i].GetLvl() == 1)
		{
			StringToMap(children[i].GetArg(), Commands);
		}
		else if (children[i].GetLvl() == 2)
		{
            std::vector<std::string> lst;
            lst = split(children[i].GetName(), " ");
			std::map < std::string, std::vector< std::string > >  Com;
			Location NewLocation;

			StringToMap( children[i].GetArg(), Com );
			NewLocation.SetCommands( Com );
            NewLocation.SetPath( lst[1] );
			Locations.push_back( NewLocation );
		}
        SetServer( children[i] );
		i++;
    }
}

std::map < std::string, std::vector< std::string > >    Server::GetCommands()
{
    return (Commands);
}

Server::~Server()
{
    std::cout << "destructor called \n";
    close(fd);
}
