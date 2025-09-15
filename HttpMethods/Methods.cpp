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

	std::string root ;
	Tmp = server.GetLocations();
	for (int i = 0; i < (int)Tmp.size(); i++)
	{
		if (Uri != "/" && Uri[Uri.size() - 1] == '/')
			Uri = Uri.substr(0, Uri.size() - 1);

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
	root = GetRoot(server, Tmp[0]);
	if (!Uri.empty())
		return (root + Uri);
	return "";
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
bool SendCgiData(Multiplexer& m, int i)
{

    std::stringstream response;
    std::string cgi_output = m.GetClient()[i].GetCgi().GetOutput();

    if (cgi_output.find("Content-Type:") != std::string::npos) {

        response << "HTTP/1.1 200 OK\r\n" << cgi_output;
    } else {
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
	else
	{
		std::cout << "\033[33mClient disconnected from " << m.GetClient()[i].GetClientFd() << "\033[0m\n";
		if(AddToEpoll(m.GetEpollFd() , EPOLL_CTL_DEL , m.GetClient()[i].GetClientFd(), m.GetEvents()) == false)
		{
			perror("EPOLL CGI");
		}
		close(m.GetClient()[i].GetClientFd());
        m.RemoveClient(i);
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
	std::ifstream file(FullPath.c_str());
	if ( !file.is_open())
	{
		status = InternalServerError;
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
	return true;
}

bool checkUri(std::string uri)
{
	std::string s(ALLOWED_CHAR_URI);
	for (int j = 0; uri[j]; j++)
	{
		if (s.find(uri[j]) == std::string::npos )
			return (false);
	}
	return (true);
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
	if (!checkUri(m.GetClient()[i].GetRequest().getUri()))
	{
		return (SendData(s, m, i, BadRequest, location));
	}
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
				root += "/" + it->second[0] + "/";
			else
				root += "/upload/";
			return (root);
		}
	}
	return "";
}

bool checkcontentsize(Multiplexer &m, int &i, Server &s)
{
	size_t content_length = atoll(m.GetClient()[i].GetRequest().getHeaders().find("Content-Length")->second.c_str());
	size_t max;
	std::map<std::string , std::vector<std::string> > mp = s.GetCommands();
	std::map<std::string , std::vector<std::string> >::iterator it = mp.find("max_body_size");
	if (it == mp.end())
		max = MAX_CLIENT_BODY;
	else
		max = (size_t)atoll(it->second[0].c_str());
	if (max < content_length)
		return (false);
	return (true);
}

int		Post(Server &s, Multiplexer& m, std::string body,Request& req, int& i , std::string path)
{
	std::ofstream 	file;

	std::string LocationPath = GetUploadLocation(s);

	if (!checkcontentsize(m, i, s))
	{
		path = ReturnErrorPath(s, PayloadTooLarge);
		std::cout << " :: " << path << std::endl;
		return (SendData(s, m, i, PayloadTooLarge, path), PayloadTooLarge);
	}
	if (body.empty() || req.getHeaderValue("Content-Length").empty() || !checkUri(req.getUri()))
		return (SendData(s, m, i, BadRequest, path), BadRequest);
	std::cout << LocationPath + GetFileName(req) << std::endl;
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
	if (!m.GetClient()[i].GetCgiflag() && !m.GetClient()[i].GetCgiRunning())
	{
		std::vector<Location> locs = server.GetLocations();
		for(size_t j = 0; j < locs.size(); j++)
		{
			if (locs[j].GetCgiStatus() == "on")
			{
				std::string root = GetRoot(server, locs[j]);

				if (ValidCgiExtention(ReturnExtention(m.GetClient()[i].GetRequest().getUri())))
				{
					m.GetClient()[i]._cgi_path = root + m.GetClient()[i].GetRequest().getUri();
					Cgi cgi(m.GetClient()[i].GetRequest(), locs[j],m.GetClient()[i]._cgi_path);
					m.GetClient()[i].GetCgi().SetReadStatus(false);
					m.GetClient()[i].GetCgi().SetFilePath(m.GetClient()[i]._cgi_path);
					m.GetClient()[i].SetCgi(cgi);
					m.GetClient()[i].SetCgiFlag(true);
					m.GetClient()[i].SetCgiRunning(true);
					m.GetClient()[i].GetCgi().SetLocation(locs[j]);

				}
			}
		}
	}
	m.GetEvents()[i].events = EPOLLOUT;
	if (AddToEpoll(m.GetEpollFd(),  EPOLL_CTL_MOD, m.GetEvents()[i].data.fd, &m.GetEvents()[i]) == false)
		return ;
	HttpStatus status =  m.GetClient()[i].GetCgi().ExecuteCgi(m.GetClient()[i].GetRequest() , m.GetClient()[i].GetCgi().location ,m.GetClient()[i]._cgi_path);
	if (m.GetClient()[i].GetCgi().ReadStatus() && status != OK)
	{
		SendData(server, m, i, status, ReturnErrorPath(server, status));
		size_t toSend =  m.GetClient()[i].GetFileSize() - m.GetClient()[i].GetSentSize();
		if (!toSend)
			disconnectClient(m, i);
	}
	else if (m.GetClient()[i].GetCgi().ReadStatus() && status == OK)
	{
		SendData(server, m, i, status, m.GetClient()[i].GetCgi().GetTmpFile());
		size_t toSend =  m.GetClient()[i].GetFileSize() - m.GetClient()[i].GetSentSize();
		if (!toSend)
			disconnectClient(m, i);
	}
}


bool	checkAllowedMethods(Client &c, Server &s)
{
	std::vector<Location> locs = s.GetLocations();
	std::string Uri = c.GetRequest().getUri();

	if (Uri != "/" && Uri[Uri.size() - 1] == '/')
	{
		Uri = Uri.substr(0, Uri.size() - 1);
	}

	size_t j = 0;
	bool flag;
	while (j < locs.size())
	{
		if (MatchLocationWithUri(c.GetRequest().getUri(), locs[j].GetPath()))
		{
			flag = false;
			break ;
		}
		j++;
	}
	return (true);
	std::map<std::string , std::vector<std::string> > mp = locs[j].GetCommands();
	std::map<std::string , std::vector<std::string> >::iterator it = mp.find("allowed_methods");
	std::vector<std::string> allowed_methods = it->second;
	if (it == mp.end())
		return (true);
	for (size_t i = 0; i < allowed_methods.size(); i++)
	{
		std::cout << allowed_methods[i] << '\n';
		if (c.GetRequest().getMethod() == allowed_methods[i])
			return true;
	}
	return (false);
}

bool	GetRequest(Server &server, Multiplexer &m, int &i)
{
	std::string path = FullPath(m.GetClient()[i].GetRequest().getStatusCode(), server, m.GetClient()[i].GetRequest().getUri());

	if (path.empty() || access(path.c_str(), R_OK) == -1)
	{
		path = ReturnErrorPath(server, NotFound);
	}

	if (!checkAllowedMethods(m.GetClient()[i], server))
	{
		path = ReturnErrorPath(server, Forbidden);
		SendData(server, m, i, Forbidden, path);
	}
	else if (m.GetClient()[i].GetRequest().getMethod() == "GET")
		RunGet(server, m, i,path);
	else if (m.GetClient()[i].GetRequest().getMethod() == "POST")
		Post(server, m, m.GetClient()[i].getBuffer(false), m.GetClient()[i].GetRequest(), i, path);
	else if (m.GetClient()[i].GetRequest().getMethod() == "DELETE")
		Delete(FullPath(m.GetClient()[i].GetRequest().getStatusCode(), server, m.GetClient()[i].GetRequest().getUri()));
    else
	{
        SendData(server, m, i, m.GetClient()[i].GetRequest().getStatusCode(), path);
	}
	if (m.GetClient()[i].GetSentSize() == m.GetClient()[i].GetFileSize())
		disconnectClient(m, i);
	return true;
}
