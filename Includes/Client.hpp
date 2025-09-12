
#pragma once
#include "Request.hpp"
#include "CGI.hpp"

class Client
{
    private :
        int16_t         fd;
        int16_t         ServerIndex;

        time_t          connectedTime;
        std::string     headers;
        std::string     body;
        Request         Req;
        bool            readDone;
        size_t          readeSize;
        size_t          FileSize;
	    size_t          sent;
        bool            IsCgi;
        Cgi             cgi;
        bool            CgiExecuted;
    public :
        Client();
        Client( const Client& copy );
        Client& operator=( const Client& copy );
        ~Client();

        void	        SetClient(int16_t val);
        void	        SetServerIndex(int16_t val);
        int16_t	        GetClientFd() const;
        int16_t	        GetserverIndex() const ;
        const time_t    &GetTime() const;
        void            Settime(time_t time);
        void            appendToBuffer(const char *tmp, size_t size, bool which);
        std::string     getBuffer(bool which) const;
        void            changeStatusRead(bool stat);
        bool            getStatusRead() const;
        Request&        GetRequest();
        size_t          GetReadSize() const;
        void            SetReadSize( size_t val );
        void            clearBuffer();
        void            SetFileSize( size_t size );
        void            SetSentSize( size_t size );
        size_t          GetFileSize() const;
        size_t          GetSentSize() const;
        void            SetCgiStatus(bool stat);
        bool            GetCgiStatus() const;
        void            SetCgi( Cgi tmp );
        Cgi&            GetCgi();
        const Cgi&      GetCgi() const;
        bool            GetCgiflag() const;
        void            SetCgiFlag(bool flag);
};
