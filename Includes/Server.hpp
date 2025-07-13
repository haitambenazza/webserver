/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 03:12:13 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/13 19:16:05 by kbassim          ###   ########.fr       */
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
        Server( Server& copy );
        Server& operator=( Server& copy );
        ~Server();
        
        void            SetData( std::string s );
        void            SetServer( Block& block);
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::vector < Location >&                               GetLocations();
        
};
#endif