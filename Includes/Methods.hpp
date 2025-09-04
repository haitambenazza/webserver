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
//post
//check if upload enabled
//if so create file with name sent and save body in it
// return 201 (if file already exists then override ) or 200 // to check
// if open file or save in file failed return 500

