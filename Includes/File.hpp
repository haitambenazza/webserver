
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
        void         SetExtention();
        int          CheckExtention();
        int          OpenFile();
        void         ReadLines();
        std::string& GetRawString();
        ~File();
};

#endif