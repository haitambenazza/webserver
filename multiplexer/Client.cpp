#include "../headers/webserver.hpp"

Client::Client()
{
    fd = -1;
    ServerIndex = -1;
    readDone = false;
    readeSize = 0;
}

Client::Client( const Client& copy )
{
    fd = copy.fd;
    connectedTime = copy.connectedTime;
    ServerIndex = copy.ServerIndex;
    readDone = copy.readDone;
    readeSize = copy.readeSize;
    // Req = copy.Req;
    // headers = copy.headers;
    // body = copy.body;
}

Client& Client::operator=( const Client& copy )
{
    if (this != &copy)
    {
        fd = copy.fd;
        ServerIndex = copy.ServerIndex;
        connectedTime = copy.connectedTime;
        readeSize = 0;
    }
    return (*this);
}

Client::~Client()
{
    //close (fd);
    headers.clear();
    body.clear();
}

void	Client::SetClient(int16_t val)
{
    fd = val;
}
void	Client::SetServerIndex(int16_t val)
{
    ServerIndex = val;
}

Request&         Client::GetRequest()
{
    return (Req);
}
int16_t     Client::GetClientFd() const
{
    return (fd);
}

size_t          Client::GetReadSize() const
{
    return (readeSize);
}

void            Client::SetReadSize( size_t val )
{
    readeSize += val;
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

void            Client::appendToBuffer(const char *tmp, size_t size, bool which)
{
    if (which == true)
        headers.append(tmp, size);
    else
        body.append(tmp, size);
}
std::string     Client::getBuffer(bool which) const
{
    if (which)
        return (headers);
    return (body);
}

void            Client::changeStatusRead(bool stat)
{
    readDone = stat;
}
bool            Client::getStatusRead() const
{
    return readDone;
}

