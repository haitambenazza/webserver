/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:16:20 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/13 19:42:05 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/File.hpp"
#include "Includes/Block.hpp"
#include "Includes/Server.hpp"
#include "Includes/Location.hpp"

std::vector<std::string>	FillVector( std::vector<std::string> Src )
{
	int i;
	std::vector<std::string> Dst;

	i = 1;
	while ( i < (int)Src.size() )
	{
		Dst.push_back(Src[i]);
		i++;
	}
	return (Dst);
}

void	StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp )
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
		values = FillVector( split(tmp[i], " ") );
		Mp.insert(std::make_pair(key, values));
		i++;
	}
}

void	PrintMap(std::map<std::string , std::vector <std::string> > Commands)
{
	std::map<std::string, std::vector<std::string> >::iterator k = Commands.begin();
	while (k != Commands.end())
	{
		std::cout << k->first << " : ";
		std::vector<std::string> lol = k->second;
		std::vector<std::string>::iterator lolit;

		lolit = lol.begin();
		while ( lolit != lol.end())
		{
			std::cout << *lolit << " "  ;
			lolit++;
		}
		std::cout << std::endl;
		k++;
	}
}

void	PrintServer( Server& Serv )
{
	int size;
	int	i;

	PrintMap(Serv.GetCommands());
	size = Serv.GetLocations().size();
	i = 0;
	while ( i < size )
	{
		std::cout << "  ";
		PrintMap(Serv.GetLocations()[i].GetCommands());
		i++;
	}
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

int main( int ac, char **av, char **envp )
{
	(void)envp;
	
	Block bl;
	Server srvr;

	int x = 0;
	int y = 0;

	if (ac != 2)
		return (1);
	File hey( av[1] );
	hey.SetExtention();
	hey.OpenFile();
	hey.ReadLines();
	bl.FillBlock(hey.GetRawString(), bl, x, y);
	srvr.SetServer( bl ) ;
	PrintServer(srvr);
	return (0);
}
