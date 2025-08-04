#include "../headers/webserver.hpp"

Client::Client()
{

}

Client::Client( const Client& copy )
{
    ServerIndex = copy.ServerIndex;
}

Client& Client::operator=( const Client& copy )
{
    if (this != &copy)
    {
        ServerIndex = copy.ServerIndex;
    }
    return (*this);
}

Client::~Client()
{

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