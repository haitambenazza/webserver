#include "../headers/webserver.hpp"

bool	running = true;

bool	CheckLocationParams( Server &server )
{
	for (int i = 0; i < (int)server.GetLocations().size(); i++)
	{
		if (server.GetLocations()[i].GetPath().empty())
			return (false);
	}
	return (true);
}

bool	InitServers(std::vector<Server> &servers, char *filename)
{
	servers = GetFullServers(filename);
	if (servers.empty())
		return (false);
	for (int i = 0; i < (int)servers.size(); i++)
	{
		if (servers[i].GetStatus() == false)
		{
			std::cerr << servers[i].GetServerName() << " \033[31m ENCOUNTERED AN ERROR\033[0m\n";
			running = false;
			//servers.clear();
			return (false);
		}
		if (servers.size() == 0)
			return (false);
	}
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
	multi.SetNumFd(epoll_wait(multi.GetEpollFd(), multi.GetEvents(), MAX_EVENT, EPOLL_TIMEOUT));
	return (true);
}


std::string GetAddrServer(Multiplexer &m, std::vector<Server> &s, int fd)
{
	for (int i = 0; i < (int)s.size(); i++)
	{
		if (m.GetEvents()[fd].data.fd == s[i].Getfd())
			return (s[i].GetIp() + ":" + s[i].GetPort());
	}
	return ("");
}

int16_t		GetServerIndex(std::vector<Server> &s, int fd)
{
	int16_t i;

	i = 0;
	while (i < (int16_t)s.size())
	{
		if (s[i].Getfd() == fd)
			break ;
		i++;
	}
	return (i);
}

void	SetNewClient(Multiplexer &m, int fd, std::vector<Server> &s)
{
	Client NewClient;
	int val = accept(m.GetEvents()[fd].data.fd, NULL, NULL);

	if (val == -1)
	{
		perror("accept");
		close(m.GetClientFd());
	}
	NewClient.Settime(time(NULL));
	m.SetClientFd(val);
	NewClient.SetClient(m.GetClientFd());
	NewClient.SetServerIndex(GetServerIndex(s, m.GetEvents()[fd].data.fd));
	m.AddClient(NewClient);
}

bool	AcceptNewClient(Multiplexer &m, int fd, std::vector<Server> &s)
{
	struct epoll_event epoll_client;


	SetNewClient(m, fd, s);
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
	epoll_client.events = EPOLLIN;
	if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_ADD, m.GetClientFd(), &epoll_client))
	{
		perror("epoll_ctl()");
		return (false);
	}
	std::cout << "\033[32mNew client [" << m.GetClientFd() << "] connected to " << GetAddrServer(m,s,fd) << "\033[0m\n";
	return true;
}

void	ReadData(Multiplexer &m, int &i, Server &s)
{
	char tmp[4096];
	std::string buffer;
	Request req;
	int bytes_read;
	(void)s;

	if ((bytes_read = recv(m.GetEvents()[i].data.fd, &tmp, sizeof(tmp) - 1, 0)) > 0)
	{
		tmp[bytes_read] = '\0';
		buffer += tmp;
		m.GetEvents()[i].events = EPOLLOUT;
		if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_MOD, m.GetEvents()[i].data.fd, &m.GetEvents()[i]))
		{
			perror("epoll_ctl()");
			return ;
		}
		if (!buffer.empty())
			GetRequest(buffer, s, m, i);
	}
	if (bytes_read == 0)
	{
		std::cout << "\033[33mClient disconnected from " << m.GetEvents()[i].data.fd << "\033[0m\n";
		if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_DEL, m.GetEvents()[i].data.fd, &m.GetEvents()[i]))
		{
			perror("epoll_ctl()");
			close(m.GetEvents()[i].data.fd);
			return ;
		}
		close(m.GetEvents()[i].data.fd);
	}
}

int	IsServerSocket(Multiplexer &m, std::vector<Server> &server, int j)
{
	for (int i = 0; i < (int)server.size(); i++)
	{
		if (m.GetEvents()[j].data.fd == server[i].Getfd())
			return (i);
	}
	return -1;
}

void	CheckTimeout(Multiplexer &m)
{
	if (m.GetNumFd() == 0)
	{
		for (int i = 0; i < (int)m.GetClient().size(); i++)
		{
			if (time(NULL) - m.GetClient()[i].GetTime() >= TIMEOUT_CLIENT)
			{
				std::cout << "\033[33mClient timeout" << "\033[0m\n";
				close(m.GetClient()[i].GetClientFd());
				m.RemoveClient(i);
			}
		}
	}
}

void	registerTime(Multiplexer &m, int i)
{
	for (int j = 0; j < (int)m.GetClient().size(); j++)
	{
		if (m.GetClient()[j].GetClientFd() == m.GetEvents()[i].data.fd)
		{
			m.GetClient()[j].Settime(time(NULL));
		}
	}
}

bool EventRoutine(std::vector<Server> &server, Multiplexer &multiplexer)
{
	int	isServer = 0;

	if (SetEventEpoll(multiplexer) == false)
		return (false);
	for (int i = 0; i < multiplexer.GetNumFd(); i++)
	{
		isServer = IsServerSocket(multiplexer, server, i);
		if (isServer != -1)
		{
			if (AcceptNewClient(multiplexer, i, server) == false)
				return (false);
		}
		else if (isServer == -1)
		{
			registerTime(multiplexer, i);
			if (multiplexer.GetEvents()[i].events & EPOLLIN)
				ReadData(multiplexer, i, server[multiplexer.GetClient().back().GetserverIndex()]);
			// else if (multiplexer.GetEvents()[i].events & EPOLLOUT)
			// 	SendData(multiplexer, i);
			else if (multiplexer.GetEvents()[i].events & (EPOLLHUP | EPOLLERR))// still testing...not working for now
			{
				std::cout << "client disconnected\n";
				close(multiplexer.GetEvents()[i].data.fd);
			}
		}
	}
	CheckTimeout(multiplexer);
	return (true);
}

void	ChangeServerStatus(int signal, siginfo_t * sig, void * context)
{
	(void)sig;
	(void)context;
	if (signal == SIGINT)
		running = false;
}

bool RunServers(std::vector<Server> &servers)
{
	Multiplexer multiplexer(servers);
	struct sigaction sign ;

	memset(&sign, 0, sizeof(sign));
	sign.sa_flags = SA_SIGINFO;
	sign.sa_sigaction = &ChangeServerStatus;
	if (sigaction(SIGINT, &sign, NULL) == -1)
		perror("sigaction");

	while (running)
	{
		if (!EventRoutine(servers, multiplexer))
			break ;
	}
	for (int i = 0; i < (int)multiplexer.GetClient().size(); i++)
		close(multiplexer.GetClient()[i].GetClientFd());
	return true;
}
