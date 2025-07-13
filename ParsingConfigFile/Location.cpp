/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 05:28:25 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/13 20:16:46 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Location.hpp"

Location::Location(  )
{
    
}

Location::Location( const Location&  copy )
{
    Commands = copy.Commands;
    Data = copy.Data;
}

Location&   Location::operator=( const Location&  copy )
{
    if ( this != &copy )
    {
        Commands = copy.Commands;
        Data = copy.Data;
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