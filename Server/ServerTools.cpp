
#include "../headers/webserver.hpp"

u_int32_t StrToIp(std::string ip)
{
	std::vector<std::string> list = split(ip, ".");
	u_int32_t result = 0;

	for (int i = 0; i < (int)list.size(); i++)
	{
		u_int16_t number = atoi(list[i].c_str());
		result = (result << 8) | number;
	}
	return (result);
}

std::string GetValuesFromKeys(std::map<std::string, std::vector<std::string> >& map, std::string key)
{
    std::map<std::string, std::vector<std::string> >::iterator  it;
    std::vector< std::string>                                   values;

    it = map.find(key);
    if (it != map.end())
    {
        values = it->second;
        return (values[0]);
    }
    else
        return "";
}

void	PrintServer( Server& Serv )
{
	int size;
	int	i;

	PrintMap(Serv.GetCommands());
	size = Serv.GetLocations().size();
	i = 0;
	while ( i < size )
	{
		std::cout << "              ";
		PrintMap(Serv.GetLocations()[i].GetCommands());
		i++;
	}
	std::cout << "----------------" << std::endl;
}

bool	CheckBrackets( std::string s )
{
	int i;
	int k;

	i = 0;
	k = 0;
	while ( s[i] )
	{
		if (s[i] == '{')
			k++;
		else if (s[i] == '}')
			k--;
		i++;
	}
	return (k == 0);
}

std::vector<std::string> GetServers( std::string& s )
{
	std::vector<std::string> ServersData;
	std::string	server("server");
	std::string	server_name("server_name");
	size_t i = 0;
	size_t pos;
	size_t pos0;

	if (s.empty())
		return ServersData;

	while (i < s.size())
	{
		pos = s.find(server, i);
		if (pos == std::string::npos)
			break;
		pos0 = s.find(server, pos + server.length());
		if (pos0 != std::string::npos && s.substr(pos0, server_name.length()) == server_name)
			pos0 = s.find(server, pos0 + server_name.length());
		if (pos0 == std::string::npos)
			pos0 = s.size();
		ServersData.push_back(s.substr(pos, pos0 - pos));
		i = pos0;
	}
	return ServersData;
}

std::vector<Server>   GetFullServers( char* FileName )
{
	std::vector<Server> 		srvs;
	std::vector<std::string> 	lst;
	int 						i;
	int 						x;
	int 						y;
	int							fl;

	File hey( FileName );
	hey.SetExtention();
	hey.OpenFile();
	hey.ReadLines();
	lst = GetServers( hey.GetRawString() );
	i = 0;
	fl = 0;
	while ( i < (int)lst.size() )
	{
		Block 						NewBlock;
		Server 						NewServer;
		x = 0;
		y = 0;
		NewBlock.FillBlock( lst[i], NewBlock, x, y );
		srvs.push_back( NewServer ); // segfault
		srvs.back().SetServers( NewBlock );
		//NewServer.SetStatus(NewBlock.GetStatus());
		srvs.back().SetStatus(NewBlock.GetStatus());
		if (!CheckBrackets(lst[i]))
		{
			srvs.back().SetStatus(false);
			i++;
		}
		if ( CheckLocationParams( srvs.back() ) == false  || CheckValidKeys(srvs.back().GetKeys()) == false )
			srvs.back().SetStatus(false);
		i++;
	}
	return (srvs);
}

bool	IsPresent(const std::vector<std::string>& vctr, std::string s)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (i < vctr.size())
	{
		if (vctr[i] == s)
			count++;
		i++;
	}
	return (count != 0);
}

bool CheckValidKeys(const std::vector<std::string> str)
{
	int j;

	j = 0;
	std::vector<std::string> valid_keys;
	valid_keys.push_back("listen");
	valid_keys.push_back("server_name");
	valid_keys.push_back("host");
	valid_keys.push_back("root");
	valid_keys.push_back("index");
	valid_keys.push_back("location");
	valid_keys.push_back("error_page");
	valid_keys.push_back("client_max_body_size");
	valid_keys.push_back("allow_methods");
	valid_keys.push_back("return");
	valid_keys.push_back("autoindex");
	valid_keys.push_back("cgi_path");
	valid_keys.push_back("cgi_ext");
	valid_keys.push_back("try_files");

	size_t i = 0;
	while (i < str.size())
	{
        //std::cout << str[i] << "\n";
		if (!IsPresent(valid_keys, str[i]))
		{
			std::cout << str[i] << " : is not valid. ";
			return (false);
		}
		i++;
	}
	return true;
}
