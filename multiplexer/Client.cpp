#include "../headers/webserver.hpp"

Client::Client()
{
    fd = -1;
    ServerIndex = -1;
    readDone = false;
    readeSize = 0;
    FileSize = 0;
    sent = 0;
}

Client::Client( const Client& copy )
{
    fd = copy.fd;
    connectedTime = copy.connectedTime;
    ServerIndex = copy.ServerIndex;
    readDone = copy.readDone;
    readeSize = copy.readeSize;
    FileSize = copy.FileSize;
    sent = copy.sent;
    Req = copy.Req;
    headers = copy.headers;
    body = copy.body;
}

Client& Client::operator=( const Client& copy )
{
    if (this != &copy)
    {
        fd = copy.fd;
        connectedTime = copy.connectedTime;
        ServerIndex = copy.ServerIndex;
        readDone = copy.readDone;
        readeSize = copy.readeSize;
        FileSize = copy.FileSize;
        sent = copy.sent;
        Req = copy.Req;
        headers = copy.headers;
        body = copy.body;
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

Request&         Client::GetRequest()
{
    return (Req);
}
int16_t     Client::GetClientFd() const
{
    return (fd);
}

void            Client::SetFileSize( size_t size )
{
    FileSize = size;
}
void            Client::SetSentSize( size_t size )
{
    sent += size;
}
size_t            Client::GetFileSize() const
{
    return (FileSize);
}
size_t            Client::GetSentSize() const
{
    return (sent);
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

void            Client::clearBuffer()
{
    headers = std::string();
    body = std::string();
}
