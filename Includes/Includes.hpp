/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:48:05 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/12 03:48:56 by kbassim          ###   ########.fr       */
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
#include "Location.hpp"

std::vector<std::string>    split(std::string s, std::string delimiter);
void                        MaskInsideBrace( std::string& s );
void                        RemoveNewLine( std::string& s );
void                        TrimSpaces( std::string& s );
#endif

