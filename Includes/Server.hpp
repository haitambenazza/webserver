/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 03:12:13 by kbassim           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/19 19:46:36 by hbenazza         ###   ########.fr       */
=======
/*   Updated: 2025/07/19 21:29:41 by kbassim          ###   ########.fr       */
>>>>>>> origin/main
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Includes.hpp"
#include "../Includes/Block.hpp"
#include "../headers/webserver.hpp"

class Server
{
    private :
        std::vector <std::string>                               Data;
        std::vector <std::string>                               keys;
        std::map < std::string, std::vector< std::string > >    Commands;
        std::vector < Location >                                Locations;
        int16_t	                                                fd;
        int16_t	                                                fd_endpoint;
		std::string	                                            server_name;
		std::string                                             root;
		std::string                                             index;
		u_int64_t	                                            max_body_size;
		// std::map<u_int16_t , std::string>                       error_map;
		std::string	                                            port;
		std::string	                                            ip;

        public :
            Server();
            Server( const Server& copy );
            Server( bool flag );
            Server& operator=( const Server& copy );
            ~Server();

            void                                                    SetServer( Block& block);
            std::map < std::string, std::vector< std::string > >    GetCommands();
            std::vector < Location >&                               GetLocations();
            void	                                                StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag );
            std::vector<std::string>                                GetKeys();
            bool                                                    SetServer();
            int                                                     Getfd() const;
            void	                                                Setfd_endpoint(int16_t fd);
            int16_t	                                                Getfd_endpoint() const;
            std::string                                             GetServerName()const;
            int                                                     CloseFd();
            void                                                    InitializeServerSettings();
            void                                                    PrintData();
            void                                                    SetValue(std::string s1, std::string s2);
};
