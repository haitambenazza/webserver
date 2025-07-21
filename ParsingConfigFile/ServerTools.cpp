/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTools.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:26:43 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/20 22:46:37 by hbenazza         ###   ########.fr       */
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

std::string GetServers( std::string& s )
{
	size_t	i;
	size_t	pos;
	size_t	pos0;

	if (s.empty())
		return ("");
	i = 0;
	while (s[i])
	{
		pos = s.find("server", i);
		if ( pos == std::string::npos )
			break ;
		pos0 = s.find("server", i + 6);
		if ( pos0 != std::string::npos && s.substr(pos0, 11) != "server_name")
		{
			if (!CheckBrackets(s.substr(pos0, pos0 - pos)))
			{
				std::cout << s.substr(pos, pos0 - pos) << '\n';
				std::cerr << "Nested Server detected" << '\n';
			}
			else
				std::cerr << "Virtual host is not available\n";
			return ("");
		}
		else
			pos0 = s.find("server", pos0 + 6);
		i = pos0;
	}

	return ( s.substr(pos, pos0 - pos) );
}

