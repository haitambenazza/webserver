/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:33:58 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/21 03:27:24 by amoubine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <sstream>

#include "../Includes/Block.hpp"
#include "../Includes/File.hpp"
#include "../Includes/Includes.hpp"
#include "../Includes/Block.hpp"
#include "../Includes/Location.hpp"
#include "../Includes/Macros.hpp"
#include "../Includes/Server.hpp"
#include "../Includes/Client.hpp"
#include "../Includes/Multiplexer.hpp"
#include "../Includes/ServerTools.hpp"
#include "../Includes/Request.hpp"


#endif
