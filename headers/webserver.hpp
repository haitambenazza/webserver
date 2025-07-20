/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:33:58 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/19 02:21:46 by hbenazza         ###   ########.fr       */
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


#endif
