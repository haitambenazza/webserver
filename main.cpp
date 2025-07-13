/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:16:20 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/13 20:33:45 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/File.hpp"
#include "Includes/Block.hpp"
#include "Includes/Server.hpp"
#include "Includes/Location.hpp"


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
	//PrintServer(srvr);
	return (0);
}
