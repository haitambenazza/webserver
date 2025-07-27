/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTools.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:31:52 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/26 04:16:21 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

u_int32_t 			StrToIp(std::string ip);
std::string			GetValuesFromKeys(std::map<std::string, std::vector<std::string> >& map, std::string key);
std::vector<Server>	GetFullServers( char* FileName );
