#include "../headers/webserver.hpp"

// std::string fileExtention(std::string file)
// {

// }

bool SendData(Multiplexer &m, int i, std::string &path)
{
	std::stringstream response("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 1066\r\n\r\n");
	std::ifstream file(path.c_str());
	std::stringstream html;

	if (!file.is_open())
		std::cout << "FAILED\n";
	html << file.rdbuf();
	response << response.str() << html.str();
	send(m.GetEvents()[i].data.fd, response.str().c_str(), response.str().size(), 0);
	m.GetEvents()[i].events = EPOLLIN;
	if (-1 == epoll_ctl(m.GetEpollFd(), EPOLL_CTL_MOD, m.GetEvents()[i].data.fd, &m.GetEvents()[i]))
	{
		perror("epoll_ctl()");
		return false;
	}
	return true;
}

std::string GetValuesFromKeysReq(std::map<std::string, std::string > map, std::string key)
{
    std::map<std::string, std::string >::iterator  				it;
    std::string                                   				values;

    it = map.find(key);
    if (it != map.end())
    {
        values = it->second;
        return (values);
    }
    return "";
}


int PostMethod( Request& Req )
{
    struct stat     info;
    std::ofstream   Target;
    std::string     s;
    std::ostringstream tm;
    std::map<std::string, std::string >::iterator it;


    if (stat(Req.getUri().c_str(), &info) == -1)
        return (NotFound);
    Req.printRequestData();
    if (!Req.getHeaders().empty())
    {
        if (GetValuesFromKeysReq(Req.getHeaders(), "Content-Type").find("form-data") == std::string::npos)
            s = "lol";
        else if (GetValuesFromKeysReq(Req.getHeaders(), "Content-Type").find("form-data") != std::string::npos)
            s = ".mp4";
        else
            s = "." + split(GetValuesFromKeysReq(Req.getHeaders(), "Content-Type"), "/")[1];
    }
    tm << time(NULL);
    std::string name(tm.str() + s.c_str());
    Target.open( name.c_str(), std::ios::out | std::ios::binary );
    if (!Target.is_open())
    {
        std::cout << "cannot open file\n";
        return (1);
    }
    Target << Req.getBody();
    return (OK);
}

int DeleteMethod( Request& Req )
{
    (void)Req;
    return (OK);
}

int	GetRequestedLocation(std::vector<Location> &l, const std::string &path)
{
	for (int i = 0; i < (int)l.size(); i++)
	{
		if (l[i].GetPath() == path)
			return (i);
	}
	return (-1);
}

bool	RunGet(Request &req, Server &s, Multiplexer &m, int &i)
{
	int	location = GetRequestedLocation(s.GetLocations(), req.getUri());
    int status = 0;

    std::cout << req.getUri() << '\n';
	if (location != -1)
	{
        Location loc(s.GetLocations()[location]);
		if (!loc.GetValuesLocation("root").empty() && !loc.GetValuesLocation("index").empty())
		{
			std::string path = loc.GetValuesLocation("root")[0] + loc.GetValuesLocation("index")[0];
			std::cout << "location "<< location << " full path " << loc.GetValuesLocation("root")[0] << loc.GetValuesLocation("index")[0] << '\n';
			SendData(m, i, path);
			status = OK;
		}
    }
    else
	{
		std::string error = "error_pages/404.html";
		SendData(m, i, error);
	}
    return (true);
}

bool	GetRequest(std::string buffer, Server &server, Multiplexer &m, int &i)
{
	Request request(buffer);

	request.printRequestData();
	if (request.getMethod() == "GET")
		return (RunGet(request, server, m, i));
	else if (request.getMethod() == "POST")
		return (PostMethod(request));
	return true;
}
