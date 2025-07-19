/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:46:14 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/19 02:29:03 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
#define FILE_HPP
#include "Includes.hpp"

class File
{
    private:
        std::string     name;
        std::string     extention;
        std::ifstream   file;
        std::string     RawString;
    public:
        File( std::string name );
        File( const File& copy );
        File& operator=( const File& copy );
        std::string  GetName() const;
        std::string  GetExtention() const;
        int         SetExtention();
        int          CheckExtention();
        int          OpenFile();
        void         ReadLines();
        std::string& GetRawString();
        ~File();
};

#endif