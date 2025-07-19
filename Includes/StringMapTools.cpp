/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringMapTools.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 02:39:13 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/19 02:39:19 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Includes.hpp"
int     IsWhiteSpace( char c )
{
    return (c == 32 || (c >= 9 && c <= 13));
}

void    TrimSpaces( std::string& s )
{
    int start;
    int end;

	if (s.empty())
		return ;
    start = 0;
    end = (int)s.length();
    while (s[end] && IsWhiteSpace(s[end]))
        end--;
    while (s[start] && IsWhiteSpace(s[start]))
        start++;
    s = s.substr(start, end - start + 1);
}

std::vector<std::string>	FillVector( std::vector<std::string> Src )
{
	int i;
	std::vector<std::string> Dst;

	i = 1;
	while ( i < (int)Src.size() )
	{
		Dst.push_back(Src[i]);
		i++;
	}
	return (Dst);
}

void	PrintMap(std::map<std::string , std::vector <std::string> > Commands)
{
	std::map<std::string, std::vector<std::string> >::iterator k = Commands.begin();
	while (k != Commands.end())
	{
		std::cout << k->first << " : ";
		std::vector<std::string> lol = k->second;
		std::vector<std::string>::iterator lolit;

		lolit = lol.begin();
		while ( lolit != lol.end())
		{
			std::cout << *lolit << " "  ;
			lolit++;
		}
		std::cout << std::endl;
		k++;
	}
}
