/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/07/24 04:05:25 by amoubine         ###   ########.fr       */
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
	while (i < s.size())
	{
		pos = s.find("server", i);
		if ( pos == std::string::npos )
			break ;
		pos0 = s.find("server", i + 6);
		if ( pos0 != std::string::npos && s.substr(pos0, 11) == "server_name")
		{
			std::cerr << "virtual host not supported \n";
			ServersData.clear();
			return (ServersData);
		}
		else
			pos0 = s.find("server", pos0 + 6);
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

	File hey( FileName );
	hey.SetExtention();
	hey.OpenFile();
	hey.ReadLines();
	lst = GetServers( hey.GetRawString() );
	hey.GetRawString().clear();
	i = 0;
	while ( i < (int)lst.size() )
	{
		Block 						NewBlock;
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

bool	InitServers(std::vector<Server> &servers, char *filename)
{
	servers = GetFullServers(filename);
	if (servers.empty())
		return (false);
	for(int serv = 0 ; serv < (int)servers.size() ; serv++)
	{
		servers[serv].InitializeServerSettings();
		servers[serv].PrintData();
		std::cout << "------------------------\n";
	}
	return true;
}

bool	SetEventEpoll(Multiplexer &multi)
{
	multi.SetNumFd(epoll_wait(multi.GetEpollFd(), multi.GetEvents(), MAX_EVENT, -1));
	if (multi.GetNumFd() == -1)
	{
		perror("epoll_wait()");
		return false;
	}
	return (true);
}

bool	AcceptNewClient(Multiplexer &m, Server &server)
{
	struct epoll_event epoll_client;

	m.SetClientFd(accept(server.Getfd(), NULL, NULL));
	if (m.GetClientFd() == -1)
	{
		perror("accept()");
		return false;
	}
	if (-1 == fcntl(m.GetClientFd(), F_SETFL, O_NONBLOCK ))
	{
		perror("fcntl()");
		return (false);
	}
	epoll_client.data.fd = m.GetClientFd();
	epoll_client.events = EPOLLIN ;
	if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_ADD, m.GetClientFd(), &epoll_client))
	{
		perror("epoll_ctl()");
		return (false);
	}
	std::cout << "New client connected to " << m.GetClientFd() << '\n';
	return true;
}

void	ReadData(Multiplexer &m, int &i)
{
	char tmp[1024] = {0};
	std::string buffer;
	std::string response("HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\n HELLO THERE\n");
	if (read(m.GetEvents()[i].data.fd, &tmp, 1024) > 0)
	{
		buffer += tmp;
		send(m.GetEvents()[i].data.fd, response.c_str(), response.size(), 0);
	}
	std::cout << buffer;
	memset(&tmp, 0, sizeof(tmp));
	buffer.clear();
}

bool EventRoutine(Server &server, Multiplexer &multiplexer)
{
	if (SetEventEpoll(multiplexer) == false)
		return (false);
	for (int i = 0; i < multiplexer.GetNumFd(); i++)
	{
		if (multiplexer.GetEvents()[i].data.fd == server.Getfd())
		{
			if (AcceptNewClient(multiplexer, server) == false)
				return (false);
		}
		else
		{
			if (multiplexer.GetEvents()[i].events & EPOLLIN)
				ReadData(multiplexer, i);
			// close(multiplexer.GetEvents()->data.fd);
		}
	}
	return (true);
}

bool RunServers(std::vector<Server> &servers)
{
	Multiplexer multiplexer(servers);

	while (true)
	{
		for(int i = 0; i < (int)servers.size(); i++)
		{
			EventRoutine(servers[i], multiplexer);
		}
	}
	return true;
}

int main( int ac, char **av, char **envp )
{
	std::vector<Server> 		servers;

	(void)envp;

	if (ac == 2)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		std::cerr << "./Webserv file_name.conf" << std::endl;
		return (1);
	}
	//init data
	if (InitServers(servers, av[1]) == false)
		return (std::cerr << "failed to init servers\n", 1);
	if (RunServers(servers) == false)
		return (1);
	return (0);
}
