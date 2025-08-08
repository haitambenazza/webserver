
#pragma once

class Client
{
    private :
        int16_t   fd;
        int16_t   ServerIndex;
        time_t      connectedTime;
    public :
        Client();
        Client( const Client& copy );
        Client& operator=( const Client& copy );
        ~Client();

        void	SetClient(int16_t val);
        void	SetServerIndex(int16_t val);
        int16_t	GetClientFd() const;
        int16_t	GetserverIndex() const ;
        time_t    GetTime() const;
        void      Settime(time_t time);
};
