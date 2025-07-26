#include "../Includes/Request.hpp"


Request::Request() : method(""), uri(""), version(""), body("") {}

Request::Request(const std::string& request_string) {
    this->parse(request_string);
}

Request::Request(const Request& other) 
    : method(other.method), uri(other.uri), version(other.version), headers(other.headers), body(other.body) {}

Request& Request::operator=(const Request& other) {
    if (this != &other) {
        method = other.method;
        uri = other.uri;
        version = other.version;
        headers = other.headers;
        body = other.body;
    }
    return *this;
}

Request::~Request() {}

std::string Request::getBody() const
{
    return this->body;
}

std::string Request::getUri() const
{
    return this->uri;
}

std::string Request::getVersion() const
{
    return this->version;
}

std::string Request::getMethod() const
{
    return this->method;
}

std::string Request::getHeaderValue(const std::string& header_name) const
{
    std::map<std::string , std::string >::const_iterator it;
    
    it = this->headers.find(header_name);

    if (it != this->headers.end())
    {
        return it->second;
    }
    
    return "";
}

std::map<std::string , std::string> Request::getHeaders() const
{
    return this->headers;
}


void Request::parse(const std::string& request_string)
{
    std::stringstream str(request_string);
    std::string line;

    std::getline(str , line);

    std::stringstream request_line(line);

    request_line >> method >> uri >> version;

    while (std::getline(str , line) && !line.empty() && line != "\r")
    {
        size_t double_dots = line.find(':');
        if (double_dots != std::string::npos)
        {
            std::string key = line.substr(0 , double_dots);
            std::string value = line.substr(double_dots + 2);

            if (!value.empty() && value[value.size() - 1] == '\r')
            {
                value.erase(value.size() - 1);
            }
            headers.insert(std::make_pair(key , value));
        }
        
    }

    if (str)
    {
        std::stringstream   body_string;
        body_string << str.rdbuf();
        this->body = body_string.str();
    }
}

void Request::printRequestData() const
{
    std::cout << "\nMethod: " << this->method << std::endl;
    std::cout << "URI: " << this->uri << std::endl;
    std::cout << "Version: " << this->version << std::endl;

    std::cout << "--- Headers ---" << std::endl;
    
    std::map<std::string, std::string>::const_iterator it;
    for (it = this->headers.begin(); it != this->headers.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "--- Body ---" << std::endl;
    std::cout << this->body << std::endl;
}