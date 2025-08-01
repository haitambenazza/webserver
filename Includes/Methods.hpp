#pragma once

#include "../headers/webserver.hpp"

bool	GetRequest(std::string buffer, Server server);
std::string GetValuesFromKeysReq(std::map<std::string, std::string >& map, std::string key);
