#include "../headers/webserver.hpp"


bool	MatchLocationWithUri(std::string Uri, std::string Loc)
{
	std::vector<std::string> vec;

	
	if (Uri.empty() || Loc.empty())
		return (false);
	Loc = Loc.substr(1, Loc.size());
	if (Loc.empty())
		return true;
	vec = split(Uri, "/");
	if (vec.size() == 0)
		return true;
	if (!vec.empty() && vec[0] == Loc)
	{
		return (true);
	}
	return(false);
}

std::string ReturnErrorPath(Server &server, int code)
{
	std::string path;

	if (code < 400)
		return (path);
	std::map<int, std::string> mp = server.GetErrorMap();
	std::map<int, std::string>::iterator it = mp.find(code);
	if (it != mp.end())
		path = it->second;
	else
	{
		mp = server.GetDefaultErrorMap();
		it = mp.find(code);
		if (it != mp.end())
			path = it->second;
	}
	return (path);
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

std::string HandleRootLocation(Server &server, Location loc, std::string Uri)
{
	std::string path;
	std::string root;
	std::map<std::string, std::vector<std::string> > mp;
	std::map<std::string, std::vector<std::string> >::iterator it;

	root = GetRoot(server, loc);
	mp = loc.GetCommands();
	it = mp.find("index");
	if ( it != mp.end() )
		path = root + loc.GetPath() + it->second[0] ;
	if ( loc.GetAutoIndex() == "on" )
	{
		return (AutoIndex(root ,Uri));
	}
	return (path);
}

std::string HandleOtherLocation(Server &server, Location loc, std::string Uri)
{
	std::string path;
	std::string root;
	std::map<std::string, std::vector<std::string> > mp;
	std::map<std::string, std::vector<std::string> >::iterator it;
	
	root = GetRoot(server, loc);
	mp = loc.GetCommands();
	it = mp.find("index");
	if ( it != mp.end() )
		path = root + loc.GetPath()+ "/" + it->second[0] ;
	else
		path = root + Uri;
	
	if ( loc.GetAutoIndex() == "on" )
		return (AutoIndex(root , Uri));
	return (path);
}

std::string SetFullPath(Server &server, Location loc, std::string Uri)
{
	std::string path;
	std::string root;

	std::map<std::string, std::vector<std::string> > mp;
	std::map<std::string, std::vector<std::string> >::iterator it;
	
	
	if (MatchLocationWithUri(Uri, loc.GetPath()))
	{
		
		if (loc.GetPath() == "/")
			return (HandleRootLocation(server, loc, Uri));
		else
			return HandleOtherLocation(server,loc, Uri);
	}
	return "";
}

std::string	FullPath(int status, Server &server, std::string Uri)
{
	std::vector<Location>	Tmp;
	std::string				path;
	(void) status;
	(void)Uri;
	Tmp = server.GetLocations();
	for (int i = 0; i < (int)Tmp.size(); i++)
	{
		std::cout << "path == " << Tmp[i].GetPath() << " uri " << Uri << std::endl;
		if (Tmp[i].GetPath() ==  Uri )
		{
			path = SetFullPath(server, Tmp[i], Uri);
			if (!path.empty())
			{
				return (path);
			}
		}
		//return (ReturnErrorPath(server, status));
	}
	return (path);
}

std::string GetContentType(std::string file)
{
	std::string type;
	size_t	pos = file.find('.');

	// std::cout << file << std::endl;
	type = file.substr(pos + 1);
	std::cout << "type == " << type << std::endl;
	if (type == "html")
		return ("text/html");
	else if (type == "pdf")
		return ("application/pdf");
	else if (type == "jpeg")
		return ("image/jpeg");
	else if (type == "jpg")
		return ("image/jpg");
	else if (type == "png")
		return ("image/png");
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

	std::cout << "status == " << status << std::endl;
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
bool SendCgiData(Server& s, Multiplexer& m, int i)
{
	(void)s;
	
    std::stringstream response;
    std::string cgi_output = m.GetClient()[i].GetCgi().GetOutput();

    // Check if CGI output la deja endo HTTP headers
    if (cgi_output.find("Content-Type:") != std::string::npos) {

        response << "HTTP/1.1 200 OK\r\n" << cgi_output; // add status;
    } else {
        // Ila makanoch lheaders foutput zidhum
        response << BuildResponse("text/html", cgi_output.size(), 200);
        response << cgi_output;
    }
	
    m.GetClient()[i].SetFileSize(response.str().size());
    size_t toSend = m.GetClient()[i].GetFileSize() - m.GetClient()[i].GetSentSize();
    if (toSend > 0)
    {
		ssize_t sent = send(m.GetClient()[i].GetClientFd(), response.str().c_str() + m.GetClient()[i].GetSentSize(), toSend, MSG_NOSIGNAL);
        if (sent > 0)
			m.GetClient()[i].SetSentSize((size_t) sent);	
    }
    return true;
}



bool SendData( Server&s ,Multiplexer &m, int i, int status, std::string FullPath )
{
	std::stringstream response;
	std::stringstream data;
	std::map<int, std::string> mp;
	std::map<int, std::string>::iterator it;

	mp = s.GetErrorMap();
	std::cout << "FullPath == "<< FullPath << std::endl;
	std::ifstream file(FullPath.c_str());
	if ( !file.is_open() )
	{
		status = NotFound;
	}
	
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

std::string GetUploadLocation(Server &s)
{
	std::vector<Location> locs = s.GetLocations();
	std::map<std::string, std::vector<std::string> > mp;
	std::map<std::string, std::vector<std::string> >::iterator it;
	for(size_t j = 0; j < locs.size(); j++)
	{
		if (locs[j].GetPath() == "/upload")
		{
			std::string root = GetRoot(s, locs[j]);
			mp = locs[j].GetCommands();
			it = mp.find("upload_location");
			if (it != mp.end())
				root += "/" + it->second[0];
			else
				root += "/upload";
			return (root);
		}
	}
	return "";
}
int		Post(Server &s, Multiplexer& m, std::string body,Request& req, int& i , std::string path)
{
	std::ofstream 	file;

	std::string LocationPath = GetUploadLocation(s);

	if (body.empty() || req.getHeaderValue("Content-Length").empty() )
		return (BadRequest);
	file.open((LocationPath + GetFileName(req)).c_str(), std::ios::out | std::ios::binary);
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

void	HandleCgi( Server& server, Multiplexer& m, int &i)
{
	if (!m.GetClient()[i].GetCgiflag())
	{
		std::vector<Location> locs = server.GetLocations();
		for(size_t j = 0; j < locs.size(); j++)
		{
			if (locs[j].GetCgiStatus() == "on")
			{
				std::string root = GetRoot(server, locs[j]);
				std::string cgi_path = root + m.GetClient()[i].GetRequest().getUri();
				if (ValidCgiExtention(ReturnExtention(cgi_path)))
				{
					Cgi cgi(m ,m.GetClient()[i].GetRequest(), locs[j], cgi_path);
					m.GetClient()[i].SetCgi(cgi);
					m.GetClient()[i].SetCgiFlag(true);
				}
			}
		}
	}
}


bool	GetRequest(Server &server, Multiplexer &m, int &i)
{
	std::string path = FullPath(m.GetClient()[i].GetRequest().getStatusCode(), server, m.GetClient()[i].GetRequest().getUri());

	// int status = m.GetClient()[i].GetRequest().getStatusCode();
	if (path.empty() || access(path.c_str(), R_OK) == -1)
	{
		m.GetClient()[i].GetRequest().SetStatusCode(NotFound);
		path = ReturnErrorPath(server, m.GetClient()[i].GetRequest().getStatusCode());
	}
	// std::cout << "akfjdsfhsd == " << status <<'\n';
	// SendData(server, m, i, 404, path);
	if (m.GetClient()[i].GetRequest().getMethod() == "GET")
	{
		std::cout << "00\n" ;
		RunGet(server, m, i,path);
	}
	else if (m.GetClient()[i].GetRequest().getMethod() == "POST")
	{
		std::cout << "01\n" ;
		Post(server, m, m.GetClient()[i].getBuffer(false), m.GetClient()[i].GetRequest(), i, path);
	}
	else if (m.GetClient()[i].GetRequest().getMethod() == "DELETE")
	{
		std::cout << "02\n" ;
		Delete(FullPath(m.GetClient()[i].GetRequest().getStatusCode(), server, m.GetClient()[i].GetRequest().getUri()));
	}
    else
    {
		std::cout << "03 path == " << path  << std::endl;
        SendData(server, m, i, m.GetClient()[i].GetRequest().getStatusCode(), path);
    }
	if (m.GetClient()[i].GetSentSize() == m.GetClient()[i].GetFileSize())
	{
		disconnectClient(m, i);
	}
	return true;
}
