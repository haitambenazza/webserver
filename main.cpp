/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:16:20 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/19 01:51:06 by kbassim          ###   ########.fr       */
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

std::vector<std::string> GetServers( std::string& s )
{
	std::vector<std::string> ServersData;
	size_t	i;
	size_t	pos;
	size_t	pos0;
	std::string server("server");

	if (s.empty())
		return (ServersData);
	i = 0;
	while (s[i])
	{
		pos = s.find(server, i);
		if ( pos == std::string::npos )
			break ;
		pos0 = s.find(server, i + server.length());
		if (pos0 == std::string::npos)
			pos0 = s.length();
		if (!CheckBrackets(s.substr(pos, pos0 - pos)))
		{
			std::cerr << "Nested server detected" << std::endl;
			return ( ServersData);
		}
		ServersData.push_back(s.substr(pos, pos0 - pos));
		i = pos0;
	}
	return ( ServersData );
}

std::vector<Server>   GetFullServers( char* FileName )
{
	std::vector<Server> 		srvs;
	std::vector<std::string> 	lst;
	int 						i;
	int 						x;
	int 						y;
	Block 						NewBlock;
	File hey( FileName );
	hey.SetExtention();
	hey.OpenFile();
	hey.ReadLines();
	lst = GetServers( hey.GetRawString() );
	hey.GetRawString().clear();
	i = 0;
	while ( i < (int)lst.size() )
	{
		Server NewServer;
		x = 0;
		y = 0;
		NewBlock.FillBlock(lst[i], NewBlock, x, y);
		srvs.push_back( NewServer );
		srvs.back().SetServer( NewBlock );
		i++;
	}
	return (srvs);
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
	std::vector<Server> 		srvs;
	std::string buffer;
	// char tmp[100] = {0};
	// int read;

	(void)envp;
	if (ac != 2)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		std::cerr << "./Webserv file_name.conf" << std::endl;
		return (1);
	}
	//init data
	srvs = GetFullServers( av[1] );
	// for(int serv = 0 ; serv < (int)srvs.size() ; serv++)
	// {
	// 	srvs[serv].InitializeServerSettings();
	// 	srvs[serv].PrintData();
	// }

	// for (int i = 0; i <(int)srvs.size(); i++)
	// 	srvs[i].PrintData();

	// std::vector<std::string> vct  = srvs[0].GetKeys();
	// for(int j = 0 ; j < (int)srvs.size(); j++)
	// 	std::cout << vct[j] << std::endl;
	// while (true)
	// {
	// 	for (int i = 0; i < (int)srvs.size(); i++)
	// 	{
	// 		srvs[i].Setfd_endpoint(accept(srvs[i].Getfd(), NULL, NULL));
	// 		if (srvs[i].Getfd_endpoint() != -1)
	// 		{
	// 			fcntl(srvs[i].Getfd_endpoint(), F_SETFL, O_NONBLOCK);
	// 			std::cout << "a new client is connected to " << srvs[i].Getfd_endpoint() << '\n';
	// 			while ( (read = recv(srvs[i].Getfd_endpoint(), &tmp, 10, 0)) > 0)
	// 				buffer += tmp;
	// 			std::cout << buffer << '\n';
	// 			buffer.clear();
	// 			srvs[i].CloseFd();
	// 		}
	// 	}
	// }
	return (0);
}
