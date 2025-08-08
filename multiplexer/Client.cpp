#include "../headers/webserver.hpp"

Client::Client()
{
    fd = -1;
    ServerIndex = -1;
}

Client::Client( const Client& copy )
{
    fd = copy.fd;
    connectedTime = copy.connectedTime;
    ServerIndex = copy.ServerIndex;
}

Client& Client::operator=( const Client& copy )
{
    if (this != &copy)
    {
        fd = copy.fd;
        ServerIndex = copy.ServerIndex;
    }
    return (*this);
}

Client::~Client()
{
    //close (fd);
}

void	Client::SetClient(int16_t val)
{
    fd = val;
}
void	Client::SetServerIndex(int16_t val)
{
    ServerIndex = val;
}
int16_t     Client::GetClientFd() const
{
    return (fd);
}

int16_t	    Client::GetserverIndex() const
{
    return (ServerIndex);
}

time_t    Client::GetTime() const
{
    return (connectedTime);
}

void        Client::Settime(time_t time)
{
    connectedTime = time;
}
