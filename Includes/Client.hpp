
#pragma once

class Client
{
    private :
        int16_t   fd;
        int16_t   ServerIndex;
    public :
        Client();
        Client( const Client& copy );
        Client& operator=( const Client& copy );
        ~Client();

        void	SetClient(int16_t val);
        void	SetServerIndex(int16_t val);
        int16_t	GetClient() const;
        int16_t	GetserverIndex() const ;
};
