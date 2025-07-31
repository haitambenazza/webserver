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

    std::getline(str, line);
    std::stringstream request_line(line);

    // Try to parse as HTTP request
    std::string temp_method, temp_uri, temp_version;
    request_line >> temp_method >> temp_uri >> temp_version;

    // Check if this looks like a valid HTTP request
    // Valid HTTP methods: GET, POST, PUT, DELETE, HEAD, OPTIONS, PATCH, etc.
    if ((temp_method == "GET" || temp_method == "POST" || temp_method == "DELETE")
        && !temp_uri.empty() &&
         (temp_version.find("HTTP/") == 0))
    {
        // This is a valid HTTP request - parse normally
        method = temp_method;
        uri = temp_uri;
        version = temp_version;

        // Parse headers
        while (std::getline(str, line) && !line.empty() && line != "\r")
        {
            size_t double_dots = line.find(':');
            if (double_dots != std::string::npos)
            {
                std::string key = line.substr(0, double_dots);
                std::string value = line.substr(double_dots + 2);

                if (!value.empty() && value[value.size() - 1] == '\r')
                {
                    value.erase(value.size() - 1);
                }
                headers.insert(std::make_pair(key, value));
            }
        }

        // Parse body
        if (str)
        {
            std::stringstream body_string;
            body_string << str.rdbuf();
            this->body = body_string.str();
        }
    }
    else
    {
        // This is NOT a valid HTTP request - treat entire input as raw data
        // Put the entire request_string as body
        this->body = request_string;

        std::cout << "[INFO] Non-HTTP request detected, treating as raw data" << std::endl;
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
