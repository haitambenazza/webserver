
#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <map>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/epoll.h>

std::vector<std::string>    split(std::string s, std::string delimiter);
void                        MaskInsideBrace( std::string& s );
void                        RemoveNewLine( std::string& s );
void                        TrimSpaces( std::string& s );
bool                        AllDigit( std::string s );
bool                        CheckIp( std::string s );
std::string                 AutoIndex( std::string root, std::string path );
std::string                 ReturnExtention(std::string s);
bool                        ValidCgiExtention(std::string extention);
std::string                 Matchkeytoextention(std::string& s);

#endif

