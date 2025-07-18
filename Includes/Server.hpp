/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 03:12:13 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/18 18:34:21 by hbenazza         ###   ########.fr       */
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
        int16_t	fd;
        int16_t	fd_endpoint;
		std::string	server_name;
		// u_int16_t	port;
		// std::string	ip;
		// std::string root;
		// std::string index;
		// u_int64_t	max_body_size;
		// std::map<u_int16_t , std::string>error_map;
    public :
        Server();
        Server( const Server& copy );
        Server& operator=( const Server& copy );
        ~Server();

        void            SetServer( Block& block);
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::vector < Location >&                               GetLocations();
        void	        StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag );
        std::vector<std::string>                                GetKeys();
        bool SetServer();
        int Getfd() const;
		void	Setfd_endpoint(int16_t fd);
		int16_t	Getfd_endpoint() const;
        std::string GetServerName()const;
        int     CloseFd();

};
