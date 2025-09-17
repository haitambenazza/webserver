#include "../headers/webserver.hpp"

Client::Client()
{
    fd = -1;
    ServerIndex = -1;
    readDone = false;
    readeSize = 0;
    FileSize = 0;
    sent = 0;
    headers = "";
    body = "";
    IsCgi = false;
    CgiExecuted = false;
    CgiRunning = false;
    // cgi = NULL;
    status = 0;
}

// Fixed Client.cpp copy constructor
Client::Client(const Client& copy)
    : fd(copy.fd),
      ServerIndex(copy.ServerIndex),
      connectedTime(copy.connectedTime),
      headers(copy.headers),
      body(copy.body),
      Req(copy.Req),
      readDone(copy.readDone),
      readeSize(copy.readeSize),
      FileSize(copy.FileSize),
      sent(copy.sent),
      IsCgi(copy.IsCgi),
      cgi(copy.cgi),
      CgiExecuted(copy.CgiExecuted),
      CgiRunning(copy.CgiRunning),
        status(copy.status)// THIS IS THE KEY - use initializer list for Cgi
{
    // Empty body - everything initialized above
}

// Also fix the assignment operator to be consistent
Client& Client::operator=(const Client& copy)
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
        IsCgi = copy.IsCgi;
        cgi = copy.cgi;
        CgiExecuted = copy.CgiExecuted;
        CgiRunning = copy.CgiRunning;  // This is fine for assignment operator
        status = copy.status;
    }
    return *this;
}

Client::~Client()
{
    // if (IsCgi)
    //     cgi.CheckExitStatus();
}

void            Client::SetCgi( Cgi tmp )
{
    cgi = tmp;
}

Cgi&             Client::GetCgi()
{
    return (cgi);
}

bool            Client::GetCgiRunning() const
{
    return CgiRunning;
}
void            Client::SetCgiRunning(bool flag)
{
    CgiRunning = flag;
}

const Cgi& Client::GetCgi() const  // Const version
{
    return cgi;
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

void            Client::SetStatus(int stat)
{
    status = stat;
}
int             Client::Getstatus() const
{
    return (status);
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

void            Client::SetCgiStatus(bool stat)
{
    IsCgi = stat;
}

bool            Client::GetCgiStatus() const
{
    return IsCgi;
}


bool            Client::GetCgiflag() const
{
    return (CgiExecuted);
}

void            Client::SetCgiFlag(bool flag)
{
    CgiExecuted = flag;
}
