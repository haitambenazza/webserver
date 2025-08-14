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
    buffer.clear();
    file.clear();
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
    buffer.clear();
    file.clear();
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
void       Client::SetBuffer(char *tmp)
{
    buffer += tmp;
}

void            Client::ReadToFile( std::string s )
{
    file << s;
}
std::string Client::GetFile() const
{
    return (file.str());
}
std::string Client::GetBuffer() const
{
    return buffer;
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
    this->file.str("");
}
