/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:16:20 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/20 22:23:18 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/webserver.hpp"

void	PrintServer( Server& Serv )
{
	int size;
	int	i;

	PrintMap(Serv.GetCommands());
	size = Serv.GetLocations().size();
	i = 0;
	while ( i < size )
	{
		std::cout << "              ";
		PrintMap(Serv.GetLocations()[i].GetCommands());
		i++;
	}
	std::cout << "----------------" << std::endl;
}

bool	CheckBrackets( std::string s )
{
	int i;
	int k;

	i = 0;
	k = 0;
	while ( s[i] )
	{
		if (s[i] == '{')
			k++;
		else if (s[i] == '}')
			k--;
		i++;
	}
	return (k == 0);
}

bool	IsPresent(const std::vector<std::string>& vctr, std::string s)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (i < vctr.size())
	{
		if (vctr[i] == s)
			count++;
		i++;
	}
	return (count != 0);
}


bool Check_if_valid(const std::vector<std::string> str)
{
	int j;

	j = 0;
	std::vector<std::string> valid_keys;
	valid_keys.push_back("listen");
	valid_keys.push_back("server_name");
	valid_keys.push_back("host");
	valid_keys.push_back("root");
	valid_keys.push_back("index");
	valid_keys.push_back("location");
	valid_keys.push_back("error_page");
	valid_keys.push_back("client_max_body_size");
	valid_keys.push_back("allow_methods");
	valid_keys.push_back("return");
	valid_keys.push_back("autoindex");
	valid_keys.push_back("cgi_path");
	valid_keys.push_back("cgi_ext");
	valid_keys.push_back("try_files");

	size_t i = 0;
	while (i < str.size())
	{
		std::cout << str[i] << '\n';
		if (!IsPresent(valid_keys, str[i]))
		{
			std::cout << str[i] << " : is not valid. ";
			return (false);
		}
		i++;
	}
	return true;
}

int main( int ac, char **av, char **envp )
{
	(void)envp;
	if (ac == 2)
	{
		Server server(av[1]);

	}
	else
		return (std::cerr << "Wrong number of arguments\n", 1);
	return (0);
}
