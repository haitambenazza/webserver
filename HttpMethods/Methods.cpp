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

// Method::Method()
// {

// }
// Method::Method( const Method& copy )
// {

// }
// Method& Method::operator=( const Method& copy )
// {

// }
// Method::~Method()
// {

// }
// int Method::GetMethod()
// {

// }
// int Method::PostMethod()
// {

// }
// int Method::DeleteMethod()
// {

// }
// bool	RunGet(Request &req, Server s)
// {

// 	//std::string FullPath(s.GetRoot() + (req.getUri().c_str() + 1)); // plus one to skip the root /
// 	return (true);
// }

bool	RunGet(Request &req, Server &s)
{
    //(void)s;
	std::string FullPath(s.GetRoot() + (req.getUri().c_str() + 1)); // plus one to skip the root
    std::cout << FullPath << '\n';
    return (true);
}

bool	GetRequest(std::string buffer, Server &server)
{
	Request request(buffer);

    (void)server;
	if (request.getMethod() == "GET")
		return (true);
	return true;
}
