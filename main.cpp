
#include "headers/webserver.hpp"

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

	File hey( FileName );
	hey.SetExtention();
	hey.OpenFile();
	hey.ReadLines();
	lst = GetServers( hey.GetRawString() );
	hey.GetRawString().clear();
	i = 0;
	while ( i < (int)lst.size() )
	{
		Block 						NewBlock;
		Server NewServer;
		x = 0;
		y = 0;
		NewBlock.FillBlock( lst[i], NewBlock, x, y );
		srvs.push_back( NewServer );
		srvs.back().SetServer( NewBlock );
		if (NewBlock.GetStatus() == false)
		srvs.back().SetStatus(NewBlock.GetStatus());
		if (!CheckBrackets(lst[i]))
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
bool Check_if_valid(const std::vector<std::string> str)
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
		if (!IsPresent(valid_keys, str[i]))
		{
			std::cout << str[i] << " : is not valid. ";
			return (false);
		}
		i++;
	}
	return true;
}

int main( int ac, char **av, char **envp )
{
	std::vector<Server> 		servers;

	(void)envp;
	if (ac != 2)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		std::cerr << "./Webserv file_name.conf" << std::endl;
		return (1);
	}
	if (InitServers(servers, av[1]) == false)
		return (std::cerr << "failed to init servers\n", 1);
	if (RunServers(servers) == false)
		return (1);
	return (0);
}
