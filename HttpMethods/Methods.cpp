#include "../headers/webserver.hpp"

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

Method::Method(){}
// Method::Method( const Method& copy )
// {

// }
// Method& Method::operator=( const Method& copy )
// {

// }
// int Method::GetMethod()
// {

// }

int Method::PostMethod( Request& Req )
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

int Method::DeleteMethod( Request& Req )
{
    (void)Req;
    return (OK);
}
Method::~Method(){}
// 	//std::string FullPath(s.GetRoot() + (req.getUri().c_str() + 1)); // plus one to skip the root /
// 	return (true);
// }

int	GetRequestedLocation(std::vector<Location> &l, std::string &path)
{
	for (int i = 0; i < (int)l.size(); i++)
	{
		if (l[i].GetPath() == path)
			return (i);
	}
	return (-1);
}

bool	RunGet(Request &req, Server &s)
{
	std::string FullPath(s.GetRoot() + (req.getUri().c_str() + 1)); // plus one to skip the root
	int	location = GetRequestedLocation(s.GetLocations(), FullPath);

    std::cout << FullPath << '\n';
	if (location != -1)
	{
		std::cout << "location "<< s.GetLocations()[location].GetItemsFromServer("root", s)[0] << " full path " ;
	}
    // req.printRequestData();
    return (true);
}

bool	GetRequest(std::string buffer, Server &server)
{
	Request request(buffer);

	if (request.getMethod() == "GET")
		return (RunGet(request, server));
	return true;
}
