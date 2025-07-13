/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 03:12:07 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/13 20:16:55 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "Includes.hpp"

class Location
{
    private :
        std::vector <std::string>                               Data;
        std::string                                             Path;
        std::map < std::string, std::vector< std::string > >    Commands;
    public :
        Location();
        Location( const Location&  copy );
        Location&   operator=( const Location&  copy );
        ~Location();   
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::string&                                            GetPath();
        void                                                    SetPath( std::string& s );
        void                                                    SetCommands( std::map < std::string, std::vector< std::string > >& Ref );
};

#endif