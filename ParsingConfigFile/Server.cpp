/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 05:28:16 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/16 21:02:20 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

Server::Server()
{
    fd = 1;
}

Server::Server( const Server& copy )
{
    fd = copy.fd;
    Data = copy.Data;
    keys = copy.keys;
    Locations = copy.Locations;
    Commands = copy.Commands;
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
}

Server& Server::operator=( const Server& copy )
{
    if (this != & copy)
    {
        fd = copy.fd;
        keys = copy.keys;
        Data = copy.Data;
        fd = socket(AF_UNIX, SOCK_STREAM, 0);
        Locations = copy.Locations;
        Commands = copy.Commands;
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
       // std::cout << children[i].GetLvl() << std::endl;
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
            //std::cout<< lst[1] << std::endl;
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


std::map < std::string, std::vector< std::string > >    Server::GetCommands()
{
    return (Commands);
}

Server::~Server(){}
