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

int Method::PostMethod( std::string& path)
{
    struct stat     info;
    std::ofstream   Target;
    std::string     s;
    std::ostringstream tm;


    if (stat(path.c_str(), &info) == -1)
        return (404);
    s = ".pdf";
    tm << time(NULL);
    std::string name(tm.str() + s.c_str());
    Target.open( name.c_str() );
    if (!Target.is_open())
    {
        std::cout << "cannot open file\n";
        return (1);
    }
    int i = 0;
    while (i < 1000)
    {
        Target << "h\n";
        i++;
    }
    return (0);
}

// int Method::DeleteMethod( std::string& target )
// {
//     if ( access(target.c_str(), ) == -1)
//         return (404);
// }
// bool	RunGet(Request &req, Server s)
// {
    
Method::~Method(){}
// 	//std::string FullPath(s.GetRoot() + (req.getUri().c_str() + 1)); // plus one to skip the root /
// 	return (true);
// }

bool	RunGet(Request &req, Server &s)
{
	std::string FullPath(s.GetRoot() + (req.getUri().c_str() + 1)); // plus one to skip the root
    // std::cout << FullPath << '\n';

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
