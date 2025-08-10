
#include "headers/webserver.hpp"

int main( int ac, char **av, char **envp )
{
	std::vector<Server> 		servers;
	Method				 M;
	std::string lol("/home/kbassim/Desktop/webserv");
	
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



