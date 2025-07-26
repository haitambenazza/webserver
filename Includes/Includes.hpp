/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:48:05 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/19 01:34:58 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "Location.hpp"
#include <sys/epoll.h>

std::vector<std::string>    split(std::string s, std::string delimiter);
void                        MaskInsideBrace( std::string& s );
void                        RemoveNewLine( std::string& s );
void                        TrimSpaces( std::string& s );
std::vector<std::string>	FillVector( std::vector<std::string> Src );
void	                    PrintMap(std::map<std::string , std::vector <std::string> > Commands);
bool                        CheckBrackets( std::string s );

#endif

