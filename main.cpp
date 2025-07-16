/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:16:20 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/16 19:50:07 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/File.hpp"
#include "Includes/Block.hpp"
#include "Includes/Server.hpp"
#include "Includes/Location.hpp"

void	CheckBrackets(std::string s);

void	PrintServer( Server& Serv )
{
	int size;
	int	i;

	PrintMap(Serv.GetCommands());
	size = Serv.GetLocations().size();
	i = 0;
	while ( i < size )
	{
		std::cout << "              ";
		PrintMap(Serv.GetLocations()[i].GetCommands());
		i++;
	}
	std::cout << "----------------" << std::endl;
}


std::vector<std::string> GetServers( std::string& s )
{
	std::vector<std::string> ServersData;
	size_t	i;
	size_t	pos;
	size_t	pos0;
	std::string server("server");

	i = 0;
	while (s[i])
	{
		pos = s.find(server, i);
		if ( pos == std::string::npos )
			break ;
		pos0 = s.find(server, i + server.length());
		if (pos0 == std::string::npos)
			pos0 = s.length();
		// the indexing is wrong it doesnt count the last character
		CheckBrackets(s.substr(pos, pos0 - pos));
		ServersData.push_back(s.substr(pos, pos0 - pos));
		i = pos0;
	}
	return ( ServersData );
}

void	CheckBrackets(std::string s)
{
	int	i;
	int	j = 0;

	i = 0;
	while (s[i])
	{
		if (s[i] == '{')
			j++;
		else if (s[i] == '}')
			j--;
		i++;
	}
	if (j != 0)
	{
		std::cout << "\nUnclosed brackets" << std::endl;
		return ;
	}
}
std::vector<Server>   GetFullServers( char* FileName )
{
	std::vector<Server> 		srvs;
	std::vector<std::string> 	lst;
	int 						i;
	int 						x;
	int 						y;
	Block 						NewBlock;
	Server 						NewServer;

	File hey( FileName );
	hey.SetExtention();
	hey.OpenFile();
	hey.ReadLines();
	lst = GetServers( hey.GetRawString() );
	i = 0;
	while ( i < (int)lst.size() )
	{
		x = 0;
		y = 0;
		NewBlock.FillBlock(lst[i], NewBlock, x, y);
		srvs.push_back( NewServer );
		srvs.back().SetServer( NewBlock );
		i++;
	}
	return (srvs);
}

int main( int ac, char **av, char **envp )
{
	std::vector<Server> 		srvs;
	(void)						envp;

	if (ac != 2)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		std::cerr << "./Webserv file_name.conf" << std::endl;
		return (1);
	}
	srvs = GetFullServers( av[1] );
	for (std::vector<Server>::iterator i = srvs.begin(); i != srvs.end(); i++)
	{
		std::cout << i->GetFd() << '\n';
	}
	// size_t i = 0;
	// while (i < srvs.size())
	// {
	// 	std::cout << srvs[i].GetCommands()
	// }
	return (0);
}
