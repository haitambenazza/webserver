#pragma once

#include "Includes.hpp"

enum HttpStatus {
    OK = 200,
    BadRequest = 400,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestUriTooLong = 414,
    NotImplemented = 501,
    HttpVersionNotSupported = 505
};

class Request
{
    private:
        std::string                         method;
        std::string                         uri;
        std::string                         version;
        std::map<std::string, std::string>  headers;
        std::string                         body;
        HttpStatus                          status_code;

    public:
        Request();
        Request(const std::string& request_string);
        Request(const Request& other);
        Request& operator=(const Request& other);
        ~Request();

        void parse(const std::string& request_string);

        std::string getMethod() ;
        std::string getUri() const;
        std::string getVersion() const;
        std::string getHeaderValue(const std::string& header_name);
        std::map<std::string, std::string> getHeaders();
        std::string getBody() const;
        std::string GetContentType();
        int         getStatusCode();
        void        printRequestData() const;

        void        stripCR(std::string &s);
        bool        parseRequestLine(const std::string &line);
        void        parseHeaders(std::stringstream &str);
        void        parseBody(std::stringstream &str);
        void        SetHeaders( std::string s );
        void        SetStatusCode( HttpStatus val );
};
