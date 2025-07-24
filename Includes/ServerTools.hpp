/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTools.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:31:52 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/24 04:05:42 by amoubine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

u_int32_t StrToIp(std::string ip);
std::string GetValuesFromKeys(std::map<std::string, std::vector<std::string> >& map, std::string key);
