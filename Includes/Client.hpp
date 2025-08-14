
#pragma once

class Client
{
    private :
        int16_t         fd;
        int16_t         ServerIndex;
        time_t          connectedTime;
        std::string     buffer;
        bool            ReadDone;
        std::stringstream file;
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
        void            SetBuffer(char *tmp);
        std::string     GetBuffer() const;
        void            SetReadStatus(bool flag);
        bool            GetReadStatus() const;
        void            ReadToFile( std::string s );
        void            ResetFile();
        std::string GetFile() const;
};
