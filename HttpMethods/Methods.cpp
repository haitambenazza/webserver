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

bool	RunGet(Request &req, Server s)
{
	std::string FullPath(s.GetRoot() + (req.getUri().c_str() + 1)); // plus one to skip the root /
    req.printRequestData();
	return (true);
}

bool	GetRequest(std::string buffer, Server server)
{
	Request request(buffer);

	if (request.getMethod() == "GET")
		return (RunGet(request, server));
	return true;
}
