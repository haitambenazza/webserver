/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 03:12:13 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/13 23:42:36 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "Includes.hpp"
#include "../Includes/Block.hpp"

class Server
{
    private :
        std::vector <std::string>                               Data;
        std::map < std::string, std::vector< std::string > >    Commands;
        std::vector < Location >                                Locations;
    public :
        Server();
        Server( const Server& copy );
        Server& operator=( const Server& copy );
        ~Server();
        
        void            SetData( std::string s );
        void            SetServer( Block& block);
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::vector < Location >&                               GetLocations();
        
};
#endif