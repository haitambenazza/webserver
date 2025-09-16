#pragma once


#include "Request.hpp"
#include "Server.hpp"
#include "Multiplexer.hpp"
#include "Includes.hpp"

class Request;


int             GetMethod();
int             PostMethod(  std::string s, Request& Req );
int             DeleteMethod(  Request& Req  );
bool	        GetRequest(Server &server, Multiplexer &m, int &i);
std::string     GetValuesFromKeysReq(std::map<std::string, std::string >& map, std::string key);
bool	        RunGet(Server &s, Multiplexer &m, int &i, std::string location);
bool             SendData( Server&s ,Multiplexer &m, int i, int status, std::string FullPath );
