#include "../headers/webserver.hpp"


bool	MatchLocationWithUri(std::string Uri, std::string Loc)
{
	std::vector<std::string> vec;

	if (Uri.empty() || Loc.empty())
		return (false);
	Loc = Loc.substr(1, Loc.size());
	vec = split(Uri, "/");
	if (vec[0] == Loc)
	{
		return (true);
	}
	return(false);
}

std::string	GetRoot(Server &server, Location loc)
{
	std::string path;
	std::map<std::string, std::vector<std::string> > mp;

	mp = loc.GetCommands();
	if (mp.find("root") != mp.end())
		path += mp.find("root")->second[0];
	else
	{
		if (server.GetCommands().find("root") != server.GetCommands().end())
			path += server.GetCommands().find("root")->second[0];
		else
		{
			std::cerr << "root not found" << "\n";
			return path;
		}
	}
	return (path.substr(0, path.size() - 1));
}

std::string SetFullPath(Server &server, Location loc, std::string Uri)
{
	std::string path;
	std::string root;

	std::map<std::string, std::vector<std::string> > mp;
	std::map<std::string, std::vector<std::string> >::iterator it;

	mp = loc.GetCommands();
	root = GetRoot(server, loc);
	if (loc.GetPath() != "/")
		path += loc.GetPath();
	else
		path += "";
	if ( MatchLocationWithUri(Uri, path) || (!MatchLocationWithUri(Uri, path) && loc.GetPath() == "/"))
	{
		it = mp.find("index");
		if (it != mp.end())
		path += "/" + it->second[0];
		else
		{
			if (Uri[Uri.size() - 1] == '/')
				Uri = Uri.substr(0, Uri.size() - 1);
			if (loc.GetAutoIndex() == "on")
				return (std::cout<< ">---------> " << AutoIndex(root , Uri)<< std::endl, AutoIndex(root , Uri));
			else if (loc.GetAutoIndex() == "on" && Uri != path)
				return (root + Uri);
			else
				return ("");
		}
		if (path.find(Uri) != std::string::npos)
			return (root + path);
	}
	return ("");
}

std::string	FullPath(Server &server, std::string Uri)
{
	std::vector<Location>	Tmp;
	std::string				path;

	Tmp = server.GetLocations();
	for (int i = 0; i < (int)Tmp.size(); i++)
	{
		path = SetFullPath(server, Tmp[i], Uri);
		if (!path.empty())
		{
			return (path);
		}
	}
	return (path);
}

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

std::string	BuildResponse(std::string type, int size, int status, std::string method)
{
	std::string	response("HTTP/1.1");
	std::stringstream sizefile;
	std::stringstream code;

	(void)method;
	code << status;
	sizefile << size;
	response += " " + code.str();
	switch (status)
	{
		case OK:
			response += " OK\r\n";
			break;
		case Created:
			response += " Created\r\n";
			break;
		case NotFound:
			response += " Not Found\r\n";
		case NotImplemented:
			response += " Not Implemented\r\n";
	}
	response += "Content-Type: " + type + "\r\nContent-Length: " + sizefile.str() + "\r\n\r\n";
	return (response);
}

bool SendData(Server& s, Multiplexer &m, int i, int status)
{
	std::stringstream response;
	std::stringstream data;
	std::string		  path;
	std::map<int, std::string>::iterator it;
	std::map<int, std::string> map;

	map = s.GetErrorMap();
	it = s.GetErrorMap().find(status);
	// exit(1);
	if (it != map.end())
	{
		path = it->second;
	}
	std::ifstream file(path.c_str());
	if ( !file.is_open() )
		status = NotFound;
	data << file.rdbuf();
	response << BuildResponse(GetContentType(path), data.str().size(), status, m.GetClient()[i].GetRequest().getMethod());
	response << data.str();
	std::cout << "RESPONSE ---------------------------------\n";
	// std::cout << response.str() << '\n';
	send(m.GetEvents()[i].data.fd, response.str().c_str(), response.str().size(), 0);
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

bool	RunGet(Server &s, Multiplexer &m, int &i, std::string location)
{
	if (!location.empty() && location[location.size() - 1] != '/')
			SendData(s, m, i, OK);
	else
		SendData(s, m, i, NotFound);
	return (true);
}

std::string	GetFileName(Request&	req)
{
	std::string				FileType;
	std::string				FileName;
	std::ostringstream		tme;

	if (split(req.getHeaderValue("Content-Type"), "/").empty())
		return (FileType);
	FileType = split(req.getHeaderValue("Content-Type"), "/")[1];
	if (FileType.empty())
		return (std::string("bad request"));
	tme << time(NULL);
	FileName = tme.str() + "." + FileType;
	return FileName;
}

int		Post(Server& s , Multiplexer &m, int &i, std::string body,Request& req)
{
	std::ofstream 	file;

	if (body.empty() || req.getHeaderValue("Content-Length").empty() )
		return (BadRequest);
	file.open(("www/upload/" + GetFileName(req)).c_str(), std::ios::out | std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "file error" << std::endl;
		return (InternalServerError);
	}
	file << body;
	SendData(s, m, i, Created);
	return (OK);
}

int		Delete( std::string path)
{
	if (access(path.c_str(), F_OK) == -1)
		return (NotFound);
	std::remove(path.c_str());
	return (OK);
}
bool	GetRequest(Server &server, Multiplexer &m, int &i)
{
	// std::map<>
	std::string path = FullPath(server, m.GetClient()[i].GetRequest().getUri());
	// m.GetClient()[i].GetRequest().printRequestData();
	if (m.GetClient()[i].GetRequest().getMethod() == "GET")
		RunGet(server, m, i,path);
	else if (m.GetClient()[i].GetRequest().getMethod() == "POST")
	{
		Post(server, m, i, m.GetClient()[i].GetRequest().getBody(), m.GetClient()[i].GetRequest());

	}
	else if (m.GetClient()[i].GetRequest().getMethod() == "DELETE")
		Delete(FullPath(server, m.GetClient()[i].GetRequest().getUri()));
	else
	{
		SendData(server, m, i, m.GetClient()[i].GetRequest().getStatusCode());
	}
	std::cout << "YAAAA =================  " <<  m.GetClient()[i].GetRequest().getStatusCode()<< "\n";
	disconnectClient(m, i);
	return true;
}
