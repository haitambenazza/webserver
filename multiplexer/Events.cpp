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
			servers.erase(servers.begin() + i);
			i--;
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
	multi.SetNumFd(epoll_wait(multi.GetEpollFd(), multi.GetEvents(), MAX_EVENT, -1));
	if (multi.GetNumFd() == -1)
	{
		perror("epoll_wait()");
		return false;
	}
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
	std::cout << "i == " << i << std::endl;
	return (i);
}

bool	AcceptNewClient(Multiplexer &m, int fd, std::vector<Server> &s)
{
	struct epoll_event epoll_client;
	Client NewClient;

	m.SetClientFd(accept(m.GetEvents()[fd].data.fd, NULL, NULL));
	
	NewClient.SetClient(m.GetClientFd());
	NewClient.SetServerIndex(GetServerIndex(s, m.GetEvents()[fd].data.fd));
	m.AddClient(NewClient);
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
	std::cout << "\033[32mNew client [" << m.GetClientFd() << "] connected to " << GetAddrServer(m,s,fd) << "\033[0m\n";
	return true;
}

void	ReadData(Multiplexer &m, int &i, Server &s)
{
	char tmp[4096] = {0};
	std::string buffer;
	Request req;
	int bytes_read;

	if ((bytes_read = recv(m.GetEvents()[i].data.fd, &tmp, sizeof(tmp), 0)) > 0)
	{
		buffer += tmp;
		if (!buffer.empty())
			GetRequest(buffer, s);
		memset(&tmp, 0, sizeof(tmp));
		buffer.clear();
		m.GetEvents()[i].events = EPOLLOUT | EPOLLET;
		if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_MOD, m.GetEvents()[i].data.fd, &m.GetEvents()[i]))
		{
			perror("epoll_ctl()");
			return ;
		}
	}
	if (bytes_read == 0)
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

int	IsServerSocket(Multiplexer &m, std::vector<Server> &server, int j)
{
	for (int i = 0; i < (int)server.size(); i++)
	{
		if (m.GetEvents()[j].data.fd == server[i].Getfd())
			return (i);
	}
	return -1;
}

bool SendData(Multiplexer &m, int i)
{
	std::string response("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 883\r\n\r\n");
	std::ifstream file("www/index.html");
	std::stringstream html;

	if (!file.is_open())
		std::cout << "FAILED\n";
	html << file.rdbuf();
	send(m.GetEvents()[i].data.fd, response.c_str(), response.size(), 0);
	send(m.GetEvents()[i].data.fd, html.str().c_str(), html.str().size(), 0);
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
		if (IsServerSocket(multiplexer, server, i) != -1)
		{
			if (AcceptNewClient(multiplexer, i, server) == false)
				return (false);
		}
		else
		{
			if (multiplexer.GetEvents()[i].events & EPOLLIN)
				ReadData(multiplexer, i, server[multiplexer.GetClient().back().GetserverIndex()]);
			else if (multiplexer.GetEvents()[i].events & EPOLLOUT)
				SendData(multiplexer, i);
		}
	}
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
		EventRoutine(servers, multiplexer);
	}
	return true;
}
