#include "../headers/webserver.hpp"

Client::Client()
{
    fd = -1;
    ServerIndex = -1;
}

Client::Client( const Client& copy )
{
    fd = copy.fd;
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
    if (fd)
        close (fd);
}

void	Client::SetClient(int16_t val)
{
    fd = val;
}
void	Client::SetServerIndex(int16_t val)
{
    ServerIndex = val;
}
int16_t     Client::GetClient() const
{
    return (fd);
}

int16_t	    Client::GetserverIndex() const 
{
    return (ServerIndex);
}