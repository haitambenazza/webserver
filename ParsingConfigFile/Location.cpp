/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 05:28:25 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/29 21:54:45 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserver.hpp"

Location::Location(  )
{

}

Location::Location( const Location&  copy )
{
    Path = copy.Path;
    Commands = copy.Commands;
}

Location&   Location::operator=( const Location&  copy )
{
    if ( this != &copy )
    {
        Path = copy.Path;
        Commands = copy.Commands;
    }
    return (*this);
}

void        Location::SetCommands( std::map < std::string, std::vector< std::string > >& Ref )
{
    Commands = Ref;
}


std::string&         Location::GetPath()
{
    return (Path);
}
void                 Location::SetPath( std::string& s )
{
    Path = s;
}

std::map < std::string, std::vector< std::string > >    Location::GetCommands()
{
    return (Commands);
}

Location::~Location()
{

}
