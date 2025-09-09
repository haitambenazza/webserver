#include "../headers/webserver.hpp"

bool AddToEpoll(int epfd, int op, int fd, epoll_event *ev)
{
    if (-1 == epoll_ctl(epfd, op, fd, ev))
    {
        perror("epoll_ctl()");
        return false;
    }
    return (true);
}
bool        Multiplexer::InitMultiplexer( const std::vector<Server>& server )
{
    EpollFd = epoll_create(1);
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
        if (AddToEpoll(EpollFd,  EPOLL_CTL_ADD, Event.data.fd, &Event) == false)
            return (std::cout << "03\n",false);
	}
    return (true);
}

Multiplexer::Multiplexer()
{
    EpollFd = -1;
    NumFds = -1;
    NewConnection = -1;
}
void                 Multiplexer::SetDataRead( const unsigned char *buff, size_t size )
{
    DataRead.assign(buff, buff + size);
}

std::vector<unsigned char>       Multiplexer::GetDataRead()
{
    return (DataRead);
}

Multiplexer::Multiplexer( std::vector<Server> &server )
{
    EpollFd = -1;
    NumFds = -1;
    NewConnection = -1;
    InitMultiplexer(server);
}

Multiplexer::~Multiplexer()
{
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

void               Multiplexer::AddClient( Client& NewClient )
{
    Clients.push_back(NewClient);
}

void    Multiplexer::RemoveClient(int i)
{
    this->Clients.erase(this->Clients.begin() + i);
}

std::vector<Client>&	Multiplexer::GetClient()
{
	return Clients;
}