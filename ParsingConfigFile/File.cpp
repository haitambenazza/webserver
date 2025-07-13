#include "../Includes/File.hpp"
#include "../Includes/Block.hpp"

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

    i = 0;
    while (std::getline(file, line))
    {
        if (line[line.size() - 1] == '{' || line[line.size() - 1] == '}')
        {
            i++;
            TrimSpaces( line );
            RawString += line;
            continue ;
        }
        if (line[line.size() - 1] != ';')
        {
            std::cout<< "Error at line " << i + 1 << " " << line << std::endl;
            file.close();
            return ;
        }
        TrimSpaces( line );
        RawString += line;
        i++;
    }
    //std::cout << RawString << std::endl;
    if (i == 0)
    {
        std::cerr << "empty file" << std::endl;
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
        std::cout << "Error openning file" << std::endl;
        return (1);
    }
    return (0);
}

int          File::CheckExtention()
{
    return (extention.compare(".conf"));
}

File::~File(){}