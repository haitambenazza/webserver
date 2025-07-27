/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTools.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:26:43 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/24 02:19:44 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserver.hpp"

u_int32_t StrToIp(std::string ip)
{
	std::vector<std::string> list = split(ip, ".");
	u_int32_t result = 0;

	for (int i = 0; i < (int)list.size(); i++)
	{
		u_int16_t number = atoi(list[i].c_str());
		result = (result << 8) | number;
	}
	return (result);
}

std::string GetValuesFromKeys(std::map<std::string, std::vector<std::string> >& map, std::string key)
{
    std::map<std::string, std::vector<std::string> >::iterator  it;
    std::vector< std::string>                                   values;

    it = map.find(key);
    if (it != map.end())
    {
        values = it->second;
        return (values[0]);
    }
    else
        return "";
}
