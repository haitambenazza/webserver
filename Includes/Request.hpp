#pragma once

#include "../headers/webserver.hpp"
#include "Includes.hpp"
#include "Server.hpp"



class Request
{
    private:
        std::string method;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> headers;
        std::string body;

    public:
        Request();
        Request(const std::string& request_string);
        Request(const Request& other);
        Request& operator=(const Request& other);
        ~Request();

        void parse(const std::string& request_string);

        std::string getMethod() const;
        std::string getUri() const;
        std::string getVersion() const;
        std::string getHeaderValue(const std::string& header_name) const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;

        void printRequestData() const;
};