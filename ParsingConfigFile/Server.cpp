/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 05:28:16 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/13 20:33:02 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

Server::Server()
{

}

Server::Server( Server& copy )
{
    Data = copy.Data;
    Locations = copy.Locations;
    Commands = copy.Commands;
}

Server& Server::operator=( Server& copy )
{
    if (this != & copy)
    {
        Data = copy.Data;
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
			std::cout << children[i].GetName() << std::endl;
			std::map < std::string, std::vector< std::string > >  Com;
			Location NewLocation;

			StringToMap( children[i].GetArg(), Com );
			NewLocation.SetCommands( Com );
			Locations.push_back(NewLocation);
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
    
}