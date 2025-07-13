/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 05:28:16 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/13 19:43:32 by kbassim          ###   ########.fr       */
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

std::map < std::string, std::vector< std::string > >    Server::GetCommands()
{
    return (Commands);
}

Server::~Server()
{
    
}