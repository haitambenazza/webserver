#include "../Includes/Request.hpp"


Request::Request() : method(""), uri(""), version(""), body("") , status_code(OK) {}

Request::Request(const std::string& request_string) {
    this->parse(request_string);
}

Request::Request(const Request& other)
    : method(other.method), uri(other.uri), version(other.version), headers(other.headers), body(other.body), status_code(other.status_code) {}

Request& Request::operator=(const Request& other) {
    if (this != &other) {
        method = other.method;
        uri = other.uri;
        version = other.version;
        headers = other.headers;
        body = other.body;
        status_code = other.status_code;
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

std::string Request::getMethod()
{
    return this->method;
}

int Request::getStatusCode()
{
    return this->status_code;
}

std::string Request::getHeaderValue(const std::string& header_name)
{
    std::map<std::string , std::string >::const_iterator it;

    it = this->headers.find(header_name);

    if (it != this->headers.end())
    {
        return it->second;
    }

    return "";
}

std::map<std::string , std::string> Request::getHeaders()
{
    return this->headers;
}

std::string isBadRequest(std::string &buffer)
{
    if (buffer.empty() || buffer[buffer.size() - 1] != '\r')
        return "400 Bad Request";
    return "";
}

void Request::stripCR(std::string &s)
{
    if (!s.empty() && s[s.size() - 1] == '\r')
        s.erase(s.size() - 1);
}

bool Request::parseRequestLine(const std::string &line)
{
    std::stringstream ss(line);
    std::string temp_method, temp_uri, temp_version;
    ss >> temp_method >> temp_uri >> temp_version;

    if ((temp_method == "GET" || temp_method == "POST" || temp_method == "DELETE") &&
        !temp_uri.empty() &&
        (temp_version == "HTTP/1.0" || temp_version == "HTTP/1.1"))
    {
        method = temp_method;
        uri = temp_uri;
        version = temp_version;
        return true;
    }
    if ((temp_method != "GET" && temp_method != "POST" && temp_method != "DELETE"))
    {
        status_code = NotImplemented;
        return(false);
    }
    std::string extra;
    if (ss >> extra) {
        status_code = BadRequest;
        return false;
    }

    if (!temp_uri.empty())
    {
        if (temp_uri.size() > RequestUriTooLong)
        {
            status_code = RequestUriTooLong;
            return(false);
        }
        else if(temp_uri[0] != '/')
        {
            status_code = BadRequest;
            return false;
        }
    }

    if (temp_version != "HTTP/1.0" && temp_version != "HTTP/1.1")
    {
        status_code = HttpVersionNotSupported;
        return false;
    }
    
    method = temp_method;
    uri = temp_uri;
    version = temp_version;

    status_code = OK;
    return true;
}

void Request::parseHeaders(std::stringstream &str)
{
    std::string line;
    while (std::getline(str, line))
    {
        stripCR(line);
        if (line.empty()) 
            break; // end of headers

        size_t colon = line.find(':');
        if (colon != std::string::npos)
        {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            
            if (key.empty()) 
            {
                status_code = BadRequest;
                return;
            }
            for (size_t i = 0; i < key.size(); ++i) 
            {
                unsigned char c = key[i];
                // allowed characters
                if (!(std::isalnum(c) || c == '-')) 
                {
                    status_code = BadRequest;
                    return;
                }
            }
            if (key.find(' ') != std::string::npos || key.find('\t') != std::string::npos)
            {
                status_code = BadRequest;
                return;
            }

            while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
                value.erase(0, 1);

            headers[key] = value;
        }
        else
        {
            // no colon found
            status_code = BadRequest;
            return;
        }
    }
}

void Request::parseBody(std::stringstream &str)
{
    std::map<std::string, std::string>::iterator it = headers.find("Content-Length");
    if (it != headers.end())
    {
        size_t length = 0;
        std::istringstream(it->second) >> length;
        if (length > 0)
        {
            std::vector<char> buffer(length);
            str.read(&buffer[0], length);
            // if (str.gcount() != static_cast<std::streamsize>(length)) 
            // {
            //     std::cout << "str.gcount() ==== " << str.gcount() << "  static_cast<std::streamsize>(length) == " << static_cast<std::streamsize>(length) << "lenght == " << length << "\n";
            //     status_code = BadRequest;
            //     return;
            // }
            // for (size_t i = 0; i < length; i++)
            // {
            //     std::cout << "n ===== " << buffer[i];
            // }
            // std::cout << "BUFFER SIZE ===== " << buffer.size();
            body.assign(buffer.begin(), buffer.end());
            // std::cout << "----------BODY------------\n" ;
            // std::cout << body << std::endl;
            // std::cout << "BODY SIZE = " << body.size();
        }
    }
    else
    {
        std::stringstream body_stream;
        body_stream << str.rdbuf();
        body = body_stream.str();
    }
}

void Request::parse(const std::string& request_string)
{
    std::stringstream str(request_string);
    std::string line;

    if (!std::getline(str, line))
        return;
    stripCR(line);
    if (request_string.empty())
    {
        status_code = BadRequest;
    }

    if (!parseRequestLine(line))
    {
        body = request_string; // treat as raw data
        std::cout << "[INFO] Non-HTTP request detected, treating as raw data\n";
        return;
    }
    //std::cout  << " STTRRRRR ++++++++++ " << str.str() << std::endl;
    parseHeaders(str);
    parseBody(str);
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

    // std::cout << "--- Body ---" << std::endl;
    // std::cout << this->body << std::endl;
}

std::string Request::GetContentType()
{
    std::map<std::string, std::string>::iterator it;

    it = headers.begin();
    while (it != headers.end())
    {
        if (it->first == "Content-Type")
            return (split(it->second, "/")[1]);
        it++;
    }
    return ("");
}
