#include "../Includes/File.hpp"
#include "../Includes/Block.hpp"
#include "../Includes/Includes.hpp"

File::File( std::string name )
{
    this->name = name;
}

File::File( const File& copy )
{
    name = copy.name;
    extention = copy.extention;
}

File& File::operator=( const File& copy )
{
    if (this != &copy)
    {
        name = copy.name;
        extention = copy.extention;
    }
    return (*this);
}

std::string File::GetName() const
{
    return (name);
}

std::string File::GetExtention()const
{
    return (extention);
}

void         File::SetExtention()
{
    int i;

    i = name.length();
    while (i >= 0)
    {
        if (name[i] == '.')
            break ;
        i--;
    }
    extention = name.substr(i, name.length() - i);
    if (extention != ".conf")
    {
        std::cerr << "wrong extention, webserver takes only .conf\n";
        exit(1);
    }
}

std::string&         File::GetRawString()
{
    return (RawString);
}


void    File::ReadLines()
{
    std::string                 line;
    std::string                 tmp;
    int                         i;
    int                         fl;
    int                         j;

    i = 0;
    j = 0;
    fl = 0;
    while (std::getline(file, line))
    {
        TrimSpaces( line );
        if (line.empty() || line[0] == '#')
            continue ;
        if (line[line.size() - 1] != ';')
        {
            if (line[line.size() - 1] != '{' && line[line.size() - 1] != '}')
            {
                i++;
                RawString += line;
                tmp = line;
                std::getline(file, line);
                TrimSpaces( line );
                if (!line.compare("{"))
                {
                    if (!line.compare("{"))
                    {
                        i++;
                        RawString += line;
                        continue ;
                    }
                }
                else
                {
                    std::cout<< "Error at line " << i  << " " << tmp << std::endl;
                    RawString.clear();
                    file.close();
                    return ;
                }
            }
        }
        RawString += line;
        i++;
    }
    if (i == 0)
    {
        std::cerr << "empty file" << std::endl;
        file.close();
        return ;
    }
    if (!CheckBrackets(RawString))
    {
        std::cerr << "Invalid Brackets" << std::endl;
        file.close();
        return ;
    }
    file.close();
}

int         File::OpenFile()
{
    file.open(name.c_str(), std::fstream::in);
    if (!file.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        return (1);
    }
    return (0);
}

int          File::CheckExtention()
{
    return (extention.compare(".conf"));
}

File::~File(){}
