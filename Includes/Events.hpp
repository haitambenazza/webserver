#pragma once

#include "../headers/webserver.hpp"

bool	EventRoutine(std::vector<Server> &server, Multiplexer &multiplexer, bool& fl);
bool	RunServers(std::vector<Server> &servers);
bool    SendData(Server& s, Multiplexer &m, int i, int status);
std::string FullPath(int status, Server &server, std::string Uri);
int		IsServerSocket(Multiplexer &m, std::vector<Server> &server, int j);
int     ReadData(Multiplexer &m, int &i);
bool	AcceptNewClient(Multiplexer &m, int fd, std::vector<Server> &s);
bool	SetEventEpoll(Multiplexer &multi);
bool	InitServers(std::vector<Server> &servers, char *filename);
void	disconnectClient(Multiplexer &m, int i);
bool    SendCgiData(Multiplexer& m, int i);
