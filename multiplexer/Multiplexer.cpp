#include "../headers/webserver.hpp"

bool        Multiplexer::InitMultiplexer( const std::vector<Server>& server )
{
    EpollFd = epoll_create1(EPOLL_CLOEXEC);
    if ( EpollFd == -1 )
    {
		perror("Epoll_create");
        return (false);
    }
	for (int i = 0; i < (int)server.size(); i++)
	{
		struct epoll_event Event;
		Event.events = EPOLLIN;
		Event.data.fd = server[i].Getfd();
    	if (epoll_ctl(EpollFd, EPOLL_CTL_ADD, Event.data.fd, &Event) == -1)
    	{
    	    perror("Epoll_ctl");
    	    return (false);
    	}
	}
    return (true);
}

Multiplexer::Multiplexer()
{
    EpollFd = -1;
    NumFds = -1;
    NewConnection = -1;
}


Multiplexer::Multiplexer(std::vector<Server> &server)
{
    EpollFd = -1;
    NumFds = -1;
    NewConnection = -1;
    InitMultiplexer(server);
}

Multiplexer::~Multiplexer()
{
    std::cout << "CLIENT IS DEAD\n";
    close(NewConnection);
    close(EpollFd);
}

int Multiplexer::GetClientFd() const
{
    return (NewConnection);
}

Multiplexer& Multiplexer::operator=( const Multiplexer& copy )
{
    (void)copy;
    return (*this);
}

int Multiplexer::GetEpollFd() const
{
    return (EpollFd);
}

int Multiplexer::GetNumFd() const
{
    return (NumFds);
}

void         Multiplexer::SetEpollFd(int epollFd)
{
    EpollFd = epollFd;
}
void         Multiplexer::SetNumFd(int numfd)
{
    NumFds = numfd;
}
void         Multiplexer::SetClientFd(int clientfd)
{
    NewConnection = clientfd;
}

struct epoll_event*      Multiplexer::GetEvents()
{
    return (Events);
}

