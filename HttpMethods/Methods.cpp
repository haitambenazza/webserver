#include "../headers/webserver.hpp"

std::string GetContentType(std::string file)
{
	std::string type;
	size_t	pos = file.find('.');

	type = file.substr(pos + 1);
	if (type == "html")
		return ("text/html");
	else if (type == "pdf")
		return ("application/pdf");
	else if (type == "jpeg")
		return ("image/jpeg");
	else if (type == "javascript")
		return ("text/javascript");
	else if (type == "mp4")
		return ("video/mp4");
	else if (type == "mpeg")
		return ("video/mpeg");
	else if (type == "plain")
		return ("text/plain");
	return ("application/octet-stream");
}

std::string	BuildResponse(std::string type, int size, int status)
{
	std::string	response("HTTP/1.1");
	std::stringstream sizefile;
	std::stringstream code;

	code << status;
	sizefile << size;
	response += " " + code.str();
	if (status == 200)
		response += " OK\r\n";
	if (status == 201)
		response += " Created\r\n";
	else if (status == NotFound)
		response += " Not Found\r\n";
	response += "Content-Type: " + type + "\r\nContent-Length: " + sizefile.str() + "\r\n\r\n";
	return (response);
}

bool SendData(Multiplexer &m, int i, std::string &path, int status)
{
	std::stringstream response;
	std::ifstream file(path.c_str());
	std::stringstream data;

	if (!file.is_open())
		status = NotFound;
	data << file.rdbuf();
	response << BuildResponse(GetContentType(path), data.str().size(), status);
	response << data.str();
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

	if (map.empty() || key.empty())
	{
		return "";
	}
    it = map.find(key);
    if (it != map.end())
    {
        values = it->second;
        return (values);
    }
    return "";
}


int PostMethod( std::string s, Request& Req )
{
    struct stat     	info;
    std::ofstream   	Target;
	std::string			FileName;
    std::ostringstream 	tm;
    std::map<std::string, std::string >::iterator it;


    if (stat(Req.getUri().c_str(), &info) == -1)
	{
        return (NotFound);
	}
	if (!Req.getHeaders().empty())
	{
		FileName = "." + split(GetValuesFromKeysReq(Req.getHeaders(), "Content-Type"), "/")[1];
	}
	tm << time(NULL);
	std::string name(tm.str() + FileName.c_str());
	Target.open( name.c_str(), std::ios::binary);
	if (!Target.is_open())
	{
		std::cout << "cannot open file\n";
		return (1);
	}
	Target << s;
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
	std::string error = "error_pages/404.html";

	if (location != -1)
	{
		Location loc(s.GetLocations()[location]);
		if (!loc.GetValuesLocation("root").empty() && !loc.GetValuesLocation("index").empty())
		{
			// req.printRequestData();
			std::string path = loc.GetValuesLocation("root")[0] + loc.GetValuesLocation("index")[0];
			std::cout << path << "--------------------\n";
			SendData(m, i, path, OK);
		}
		else
		{
			SendData(m, i, error, NotFound);
			return (false);
		}
	}
    else
	{
		SendData(m, i, error, NotFound);
	}
    return (true);
}

int	GetMatchedLocation(Server &server, std::string URI)
{
	std::vector<Location>	Tmp;

	Tmp = server.GetLocations();
	for (int i = 0; i < (int)Tmp.size(); i++)
	{
		if (Tmp[i].GetPath() == URI)
			return (i);
	}
	return (-1);
}

bool	GetRequest(Server &server, Multiplexer &m, int &i)
{
	// (void)server;
	
	if (GetMatchedLocation(server, m.GetClient()[i].GetRequest().getUri()) == -1)
	{
		std::cerr << "No matching Location\n";
		exit(1);
	}
	else
	{
		std::cerr << "matched location\n";
		exit(1);
	}
	if (m.GetClient()[i].GetRequest().getMethod() == "GET")
		std::cout << "GET is up\n";
	else if (m.GetClient()[i].GetRequest().getMethod() == "POST")
		std::cout << "POST is up\n";
	else if (m.GetClient()[i].GetRequest().getMethod() == "DELETE")
		std::cout << "DELETE is up\n";
	return true;
}
