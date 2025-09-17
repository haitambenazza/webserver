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
	{
		running = false;
		return (false);
	}
	for (int i = 0; i < (int)servers.size(); i++)
	{
		if (servers[i].GetStatus() == false)
		{
			std::cerr << servers[i].GetServerName() << " \033[31m ENCOUNTERED AN ERROR\033[0m\n";
			running = false;
			return (false);
		}
		if (servers.size() == 0)
			return (false);
	}
	for(int serv = 0 ; serv < (int)servers.size() ; serv++)
	{
		if (servers[serv].GetStatus() == false)
			return (false);
		servers[serv].SetErrorMap();
		servers[serv].PrintData();
	}
	return true;
}

bool	SetEventEpoll(Multiplexer &multi)
{
	multi.SetNumFd(epoll_wait(multi.GetEpollFd(), multi.GetEvents(), MAX_EVENT, 0));
	// std::cout << "num == " << multi.GetNumFd() << std::endl;
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
	// fcntl(m.GetEvents()[fd].data.fd, F_SETFL, O_NONBLOCK);
	NewClient.Settime(time(NULL));
	m.SetClientFd(val);
	NewClient.SetClient(m.GetClientFd());
	NewClient.SetServerIndex(GetServerIndex(s, m.GetEvents()[fd].data.fd));
	m.AddClient(NewClient);
}
bool	ChangeClientEvent(Multiplexer &m , int i , int event)
{
	m.GetEvents()->events = event;
	(void)i;
	return (true);
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
	epoll_client.data.fd = m.GetClientFd();
	epoll_client.events = EPOLLIN;

	if (AddToEpoll(m.GetEpollFd(),  EPOLL_CTL_ADD,  m.GetClientFd(), &epoll_client) == false)
        return (std::cout << "00", false);
	std::cout << "\033[32mNew client [" << m.GetClientFd() << "] connected to " << GetAddrServer(m,s,fd) << "\033[0m\n";
	return true;
}

bool	appendToHeader(Multiplexer &m, int i, char *tmp, size_t bytes_read)
{
	m.GetClient()[i].appendToBuffer(tmp, bytes_read, true);
	if(m.GetClient()[i].GetRequest().parse(tmp) == false)
		return false;

	if (m.GetClient()[i].getBuffer(true).find("\r\n\r\n") != std::string::npos)
	{
		m.GetClient()[i].changeStatusRead(true);
		size_t pos = m.GetClient()[i].getBuffer(true).find("\r\n\r\n") + 4;
		m.GetClient()[i].appendToBuffer(m.GetClient()[i].getBuffer(true).substr(pos, m.GetClient()[i].getBuffer(true).size() - pos).c_str(), m.GetClient()[i].getBuffer(true).size() - pos, false);
		if (m.GetClient()[i].GetRequest().parse(m.GetClient()[i].getBuffer(true)) == false)
			return (false);
	}
	m.GetClient()[i].SetReadSize(m.GetClient()[i].getBuffer(false).size());
	return (true);
}

void	disconnectClient(Multiplexer &m, int i)
{
	std::cout << "\033[33mClient disconnected from " << m.GetEvents()[i].data.fd << "\033[0m\n";
	if (AddToEpoll(m.GetEpollFd(),  EPOLL_CTL_DEL,  m.GetEvents()[i].data.fd, &m.GetEvents()[i]) == false)
    {
		close(m.GetEvents()[i].data.fd);
		std::cout << "01\n";
		return ;
	}
	close(m.GetEvents()[i].data.fd);
	m.RemoveClient(i);
}

bool	isPostValid(Multiplexer &m, int &i)
{
	bool post = m.GetClient()[i].GetRequest().getMethod() == "POST";
	bool contentlength = m.GetClient()[i].GetRequest().getHeaderValue("Content-Length").empty();
	if (contentlength)
		return (false);
	bool doneRead = atoll(m.GetClient()[i].GetRequest().getHeaderValue("Content-Length").c_str()) ==
	(long long)m.GetClient()[i].GetReadSize();
	return (doneRead && !contentlength && post);
}

int	ReadData( Multiplexer &m, int &i)
{
	char	tmp[BUFFER_SIZE];
	int		bytes_read;

	bzero(tmp, BUFFER_SIZE);
	if ((bytes_read = read(m.GetEvents()[i].data.fd, &tmp, sizeof(tmp))) > 0)
	{
		tmp[bytes_read] = 0;
		if (!m.GetClient()[i].getStatusRead())
		{
			if (appendToHeader(m, i, tmp, bytes_read) == false)
				return (1);
			if (ValidCgiExtention((ReturnExtention(m.GetClient()[i].GetRequest().getUri()))) && (m.GetClient()[i].GetRequest().getMethod() != "DELETE"))
			{
				m.GetClient()[i].SetCgiStatus(true);
				return 0;
			}
		}
		else
		{
			m.GetClient()[i].SetReadSize(bytes_read);
			m.GetClient()[i].appendToBuffer(tmp, bytes_read, false);
		}
		if (m.GetClient()[i].GetRequest().getMethod() != "POST" && m.GetClient()[i].getStatusRead())
			return (1);
	}

	if (bytes_read == -1)
		;;
	if (isPostValid(m, i))
		return (1);
	if (bytes_read == 0)
	{
		disconnectClient(m, i);
		return (0);
	}
	return (0);
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

void	ExecCgi(Multiplexer &m, Server& s, int S)
{
	HandleCgi(s, m, S); //fork()
	return;
}
bool gci_working = false;

bool EventRoutine(std::vector<Server> &server, Multiplexer &multiplexer)
{
	int	isServer = 0;

	if (SetEventEpoll(multiplexer) == false)
	return (false);

	for (int j = 0; j < multiplexer.GetNumFd(); j++)
	{
		isServer = IsServerSocket(multiplexer, server, j);
		if (isServer != -1)
		{
			if (AcceptNewClient(multiplexer, j, server) == false)
			return (false);
		}
	}
	for (int i = 0; i < (int)multiplexer.GetClient().size(); i++)
	{
		if (i < (int)multiplexer.GetClient().size())
		{
			registerTime(multiplexer, i);
			if ((multiplexer.GetEvents()[i].events & EPOLLIN) && multiplexer.GetClient()[i].GetCgiStatus() == false)
			{
				if (ReadData(multiplexer, i) == 1)
				{
					multiplexer.GetEvents()[i].events = EPOLLOUT;
					if (AddToEpoll(multiplexer.GetEpollFd(),  EPOLL_CTL_MOD, multiplexer.GetEvents()[i].data.fd, &multiplexer.GetEvents()[i]) == false)
						return (false);
				}
			}
			if ((multiplexer.GetEvents()[i].events & EPOLLOUT) && multiplexer.GetClient()[i].GetCgiStatus() == false)
				return(GetRequest(server[multiplexer.GetClient()[i].GetserverIndex()], multiplexer, i));
			else if (multiplexer.GetClient()[i].GetCgiStatus())
			{
				HandleCgi(server[multiplexer.GetClient()[i].GetserverIndex()] , multiplexer, i);
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
	return true;
}
