#include "../headers/webserver.hpp"

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

	if (loc.GetPath() == "/")
		path += "";
	else
		path += loc.GetPath();
	it = mp.find("index");
	if (it != mp.end())
	{
		if (!it->second.empty())
		path += "/" + it->second[0];
	}
	if ( Uri == path || Uri == "/")
		return root + path;
	return("");
}

std::string	FullPath(Server &server, std::string Uri)
{
	std::vector<Location>	Tmp;
	std::string				path;
	(void)Uri;
	Tmp = server.GetLocations();
	for (int i = 0; i < (int)Tmp.size(); i++)
	{
		if (!SetFullPath(server, Tmp[i], Uri).empty())
			return (SetFullPath(server, Tmp[i], Uri));
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

bool SendData(Multiplexer &m, int i, std::string path, int status)
{
	std::stringstream response;
	std::stringstream data;

	if ( path.empty() || access(path.c_str(), R_OK) == -1)
	{
		path = "error_pages/404.html";
		status = NotFound;
	}
	std::ifstream file(path.c_str());
	if ( !file.is_open() )
		status = NotFound;
	data << file.rdbuf();
	response << BuildResponse(GetContentType(path), data.str().size(), status);
	response << data.str();
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

bool	RunGet(Multiplexer &m, int &i, std::string location)
{
	std::string error = "error_pages/404.html";

	std::cout << location;
	if (!location.empty())
			SendData(m, i, location, OK);
	else
		SendData(m, i, error, NotFound);
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

int		Post( std::string body,Request& req )
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
	// Cgi cg(m.GetClient()[i].GetRequest());
	// build the path std::string path = GetPath()
	std::string path = FullPath(server, m.GetClient()[i].GetRequest().getUri());

	std::cout << path << '\n';

	m.GetClient()[i].GetRequest().printRequestData();
	if (m.GetClient()[i].GetRequest().getMethod() == "GET")
		RunGet(m, i,path);
	else if (m.GetClient()[i].GetRequest().getMethod() == "POST")
	{
		std::cout << "POST is up\n";
		return (Post(m.GetClient()[i].getBuffer(false), m.GetClient()[i].GetRequest()));
	}
	else if (m.GetClient()[i].GetRequest().getMethod() == "DELETE")
	{
		std::cout << "DELETE is up\n";
		return(Delete(FullPath(server, m.GetClient()[i].GetRequest().getUri())));
	}
	disconnectClient(m, i);
	return true;
}
