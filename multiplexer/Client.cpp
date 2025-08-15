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
    connectedTime = copy.connectedTime;
    ReadDone = false;
    Headers.clear();
    Body.clear();
}

Client& Client::operator=( const Client& copy )
{
    if (this != &copy)
    {
        fd = copy.fd;
        ServerIndex = copy.ServerIndex;
        connectedTime = copy.connectedTime;
    }
    return (*this);
}

Client::~Client()
{
    //close (fd);
    Headers.clear();
    Body.clear();
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

const time_t    &Client::GetTime() const
{
    return (connectedTime);
}

void        Client::Settime(time_t time)
{
    connectedTime = time;
}
void       Client::SetHeaders(char *tmp)
{
    Headers << tmp;
}

void            Client::WriteToBody( std::string s )
{
    Body << s;
}
std::string Client::GetBody() const
{
    return (Body.str());
}
std::string Client::GetHeaders() const
{
    return Headers.str();
}

void        Client::SetReadStatus(bool flag)
{
    ReadDone = flag;
}
bool        Client::GetReadStatus() const{
    return ReadDone;
}

void            Client::ResetFile()
{
    this->Body.str("");
}
