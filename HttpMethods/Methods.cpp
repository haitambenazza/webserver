#include "../headers/webserver.hpp"


bool	MatchLocationWithUri(std::string Uri, std::string Loc)
{
	std::vector<std::string> vec;

	if (Uri.empty() || Loc.empty())
		return (false);
	Loc = Loc.substr(1, Loc.size());
	vec = split(Uri, "/");
	if (!vec.empty() && vec[0] == Loc)
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
	
	std::cout << loc.GetCgiStatus() << std::endl;
	if (loc.GetCgiStatus() == "on") 
	{
		std::cout << "aaaaaaa7\n";
		return (root + Uri);

	}
	mp = loc.GetCommands();
	root = GetRoot(server, loc);
	it = mp.find("redirect");
	if (it != mp.end())
	{
		path += it->second[0];
		// std::cout << "root + path == " <<  root + path << std::endl;
		return (root + path);
	}
	else if (loc.GetPath() != "/")
	{
		path += loc.GetPath();
	}
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
			if (loc.GetAutoIndex() == "on" && loc.GetRedirect().empty())
			{
				return (AutoIndex(root , Uri));
			}
			else if (loc.GetAutoIndex() == "on" && Uri != path && loc.GetRedirect().empty())
				return (root + Uri);
			else if (!loc.GetRedirect().empty())
				return (root + loc.GetPath());
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

	// std::cout << file << std::endl;
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

	// std::cout << "status == " << status << std::endl;
	code << status;
	sizefile << size;
	response += " " + code.str();

	switch (status)
	{
		case 200:
			response += " OK\r\n";
			break;
		case 201:
			response += " Created\r\n";
			break;
		case 400:
			response += " Bad Request\r\n";
			break;
		case 403:
			response += " Forbidden\r\n";
			break;
		case 404:
			response += " Not Found\r\n";
			break;
		case 405:
			response += " Method Not Allowed\r\n";
			break;
		case 413:
			response += " Payload Too Large\r\n";
			break;
		case 414:
			response += " URI Too Long\r\n";
			break;
		case 415:
			response += " Unsupported Media Type\r\n";
			break;
		case 500:
			response += " Internal Server Error\r\n";
			break;
		case 501:
			response += " Not Implemented\r\n";
			break;
		case 505:
			response += " HTTP Version Not Supported\r\n";
			break;
		default:
			response += " Unknown Status\r\n";
			break;
	}
	response += "Content-Type: " + type + "\r\nContent-Length: " + sizefile.str() + "\r\n\r\n";
	return (response);
}

bool SendData( Server&s ,Multiplexer &m, int i, int status, std::string FullPath )
{
	std::stringstream response;
	std::stringstream data;
	std::map<int, std::string> mp;
	std::map<int, std::string>::iterator it;

	mp = s.GetErrorMap();
	if (status >= 400 )
	{
		if (mp.empty())
			return (false);
		it = mp.find(status);
		if (it != mp.end())
			FullPath = it->second;
		else
		{
			FullPath = "error_pages/404.html";
			status = NotFound;
		}
	}
	std::ifstream file(FullPath.c_str());
	if ( !file.is_open() )
		status = NotFound;
	// std::cout << "status == " << status << std::endl;
	data << file.rdbuf();
	response << BuildResponse(GetContentType(FullPath), data.str().size(), status);
	response << data.str();

	m.GetClient()[i].SetFileSize(response.str().size());
	size_t toSend =  m.GetClient()[i].GetFileSize() - m.GetClient()[i].GetSentSize();
	if (toSend > 0)
	{
		ssize_t sent = send(m.GetEvents()[i].data.fd, response.str().c_str() + m.GetClient()[i].GetSentSize(), toSend , MSG_NOSIGNAL);
		if (sent >= 0)
			m.GetClient()[i].SetSentSize((size_t) sent);
	}
	// std::cout << response.str() << std::endl;
	return true;
}

std::string GetValuesFromKeysReq(std::map<std::string, std::string > map, std::string key)
{
    std::map<std::string, std::string >::iterator  	it;
    std::string                                   	values;

	if (map.empty() || key.empty())
		return "";
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

bool	RunGet(Server& s, Multiplexer &m, int &i, std::string location)
{
	SendData(s, m, i, m.GetClient()[i].GetRequest().getStatusCode(), location);
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

int		Post(Server &s, Multiplexer& m, std::string body,Request& req, int& i , std::string path)
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
    SendData(s, m, i, Created, path);
	return (OK);
}

int		Delete(  std::string path  )
{
	if (access(path.c_str(), F_OK) == -1)
		return (NotFound);
	std::remove(path.c_str());
	return (OK);
}
bool	GetRequest(Server &server, Multiplexer &m, int &i)
{
	std::vector<Location> locs = server.GetLocations();

	std::string path = FullPath(server, m.GetClient()[i].GetRequest().getUri());
	std::cout << "path == " << path << std::endl;
	if (path.empty())
		path = "error_pages/404.html";
    std::map<int, std::string> map;
    std::map<int, std::string>::iterator it;
	
	for(size_t i = 0; i < locs.size(); i++)
	{
		if (locs[i].GetCgiStatus() == "on")
		{
			// Request Req = 
			Cgi cg(m.GetClient()[i].GetRequest(), locs[i], path);
		}
	}
	if (m.GetClient()[i].GetRequest().getMethod() == "GET")
	{
		RunGet(server, m, i,path);
	}
	else if (m.GetClient()[i].GetRequest().getMethod() == "POST")
		Post(server, m, m.GetClient()[i].getBuffer(false), m.GetClient()[i].GetRequest(), i, path);
	else if (m.GetClient()[i].GetRequest().getMethod() == "DELETE")
		Delete(FullPath(server, m.GetClient()[i].GetRequest().getUri()));
    else
    {
        SendData(server, m, i, m.GetClient()[i].GetRequest().getStatusCode(), path);
    }
	if (m.GetClient()[i].GetSentSize() == m.GetClient()[i].GetFileSize())
		disconnectClient(m, i);
	return true;
}