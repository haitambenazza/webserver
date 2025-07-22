/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:16:20 by hbenazza          #+#    #+#             */
/*   Updated: 2025/07/22 23:58:21 by amoubine         ###   ########.fr       */
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

bool EventRoutine(Server &server, Multiplexer &multiplexer)
{
	char	tmp[1024] = {0};
	std::string buffer;

	multiplexer.SetNumFd(epoll_wait(multiplexer.GetEpollFd(), multiplexer.GetEvents(), MAX_EVENT, -1));
	if (multiplexer.GetNumFd() == -1)
    {
		perror("Epoll_wait");
        return (false);
    }
    for (int i = 0; i < multiplexer.GetNumFd(); i++)
    {
		if (multiplexer.GetEvents()[i].data.fd == server.Getfd())
        {
			multiplexer.SetClientFd(accept(server.Getfd(), NULL, NULL));
            if (multiplexer.GetClientFd() == -1)
            {
				perror("Accept");
                return (false);
            }
			// fcntl(multiplexer.GetClientFd(), F_SETFL, O_NONBLOCK);
			// usleep(1000);
			// std::cout << "NEW CLIENT "<< multiplexer.GetClientFd() << "FDS[" << multiplexer.GetNumFd() << "]\n";
			// while (recv(multiplexer.GetClientFd(), &tmp, 10, 0) > 0)
			// 	buffer += tmp;
			fcntl(multiplexer.GetClientFd() , F_SETFL , O_NONBLOCK);
			struct epoll_event client_event;
			client_event.events = EPOLLIN;
			client_event.data.fd = multiplexer.GetClientFd();
			if (epoll_ctl(multiplexer.GetEpollFd() , EPOLL_CTL_ADD , multiplexer.GetClientFd() , &client_event) == -1)
			{
				perror("epoll_ctl: client add");
                close(multiplexer.GetClientFd());
                continue;
			}
			std::cout << "NEW CLIENT " << multiplexer.GetClientFd() << " ADDED TO EPOLL\n";

			memset(&tmp, 0, sizeof(tmp));
			//
        }
        else
        {
			int client_fd = multiplexer.GetEvents()->data.fd;
			
			int bytes_received = recv(client_fd , tmp , sizeof(tmp) - 1 , 0);
			if (bytes_received > 0)
			{
				buffer += tmp;
				std::cout << "Received from client " << client_fd << ": \n" << buffer << std::endl;
				
				std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        		send(client_fd, response.c_str(), response.length(), 0);
			}
			buffer.clear();
			close(client_fd);
        }
		//else if (client is already connected)
			//HandleRequest;
		//else
			//Handle cgi
		// std::cout << "Still working on it \n";
		// break ;	
    }
	return (true);
}

int	RunServer(Server &server)
{
	// std::string	buffer;
	// char 		tmp[6969] = {0};
	Multiplexer multiplexer(server);

	while ( true)
	{
		EventRoutine(server, multiplexer);
	}
}

int main( int ac, char **av, char **envp )
{
	(void)envp;

	if (ac == 2)
	{
		Server server(av[1]);
		server.PrintData();
		RunServer(server);
	}
	else
		return (std::cerr << "Wrong number of arguments\n", 1);
	return (0);
}
