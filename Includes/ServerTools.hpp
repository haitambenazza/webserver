
#pragma once

#include "Server.hpp"

u_int32_t 			StrToIp(std::string ip);
std::string			GetValuesFromKeys(std::map<std::string, std::vector<std::string> >& map, std::string key);
std::vector<Server>	GetFullServers( char* FileName );
