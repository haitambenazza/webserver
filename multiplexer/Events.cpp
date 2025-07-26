#include "../headers/webserver.hpp"


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

bool	AcceptNewClient(Multiplexer &m, int fd)
{
	struct epoll_event epoll_client;

	m.SetClientFd(accept(m.GetEvents()[fd].data.fd, NULL, NULL));
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
	epoll_client.events = EPOLLIN | EPOLLET;
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

	int bytes_read = read(m.GetEvents()[i].data.fd, &tmp, 1024);
	if (bytes_read > 0)
	{
		buffer += tmp;
		std::cout << buffer;
		memset(&tmp, 0, sizeof(tmp));
		buffer.clear();
		m.GetEvents()[i].events = EPOLLOUT | EPOLLET;
		if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_MOD, m.GetEvents()[i].data.fd, &m.GetEvents()[i]))
		{
			perror("epoll_ctl()");
			return ;
		}
	}
	else if (bytes_read == 0)
	{
		std::cout << "Client disconnected from " << m.GetEvents()[i].data.fd << '\n';
		if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_DEL, m.GetEvents()[i].data.fd, &m.GetEvents()[i]))
		{
			perror("epoll_ctl()");
			close(m.GetEvents()[i].data.fd);
			return ;
		}
		close(m.GetEvents()[i].data.fd);
	}
}

bool	IsServerSocket(Multiplexer &m, std::vector<Server> &server, int j)
{
	for (int i = 0; i < (int)server.size(); i++)
	{
		if (m.GetEvents()[j].data.fd == server[i].Getfd())
		return (true);
	}
	return false;
}

bool SendData(Multiplexer &m, int i)
{
	std::string response("HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\n");


	send(m.GetEvents()[i].data.fd, response.c_str(), response.size(), 0);
	m.GetEvents()[i].events = EPOLLIN | EPOLLET;
	if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_MOD, m.GetEvents()[i].data.fd, &m.GetEvents()[i]))
	{
		perror("epoll_ctl()");
		return false;
	}
	return true;
}

bool EventRoutine(std::vector<Server> &server, Multiplexer &multiplexer)
{

	if (SetEventEpoll(multiplexer) == false)
	return (false);
	for (int i = 0; i < multiplexer.GetNumFd(); i++)
	{
		if (IsServerSocket(multiplexer, server, i))
		{
			if (AcceptNewClient(multiplexer, i) == false)
				return (false);
		}
		else
		{
			if (multiplexer.GetEvents()[i].events & EPOLLIN)
				ReadData(multiplexer, i);
			else if (multiplexer.GetEvents()[i].events & EPOLLOUT)
				SendData(multiplexer, i);
		}
	}
	return (true);
}

bool RunServers(std::vector<Server> &servers)
{
	Multiplexer multiplexer(servers);

	while (true)
	{
			EventRoutine(servers, multiplexer);
	}
	return true;
}
