/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:33:58 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/27 00:20:22 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>


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
#include "../Includes/Events.hpp"


#endif
