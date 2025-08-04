#pragma once

#include "../headers/webserver.hpp"

bool	EventRoutine(std::vector<Server> &server, Multiplexer &multiplexer);
bool	RunServers(std::vector<Server> &servers);
bool	SendData(Multiplexer &m, int i);
int		IsServerSocket(Multiplexer &m, std::vector<Server> &server, int j);
void	ReadData(Multiplexer &m, int &i, std::vector<Server> &server);
bool	AcceptNewClient(Multiplexer &m, int fd, std::vector<Server> &s);
bool	SetEventEpoll(Multiplexer &multi);
bool	InitServers(std::vector<Server> &servers, char *filename);
