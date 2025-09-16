#include "../headers/webserver.hpp"

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
    if (i <= 0)
        return ;
    extention = name.substr(i, name.length() - i);
    if (extention != ".conf")
    {
        std::cerr << "wrong extention, webserver takes only .conf\n";
        return ;
    }
}

std::string&         File::GetRawString()
{
    return (RawString);
}

bool iswtspaces(char c)
{
    return (c == 32 || (c >= 9 && c <= 13));
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
        {
            i++;
            continue ;
        }
        if (line[line.size() - 1] == ';')
        {
            i++;
            RawString += line;
            continue ;
        }
        else
        {
            if (line[line.size() - 1] != '{' && line[line.size() - 1] != '}')
            {
                i++;
                RawString += line;
                tmp = line;
                std::getline(file, line);
                TrimSpaces( line );
                if (!line.compare("{") || !line.compare("}"))
                {
                    i++;
                    RawString += line;
                    continue ;
                }
                else
                {
                    TrimSpaces( tmp );
                    size_t pos1 = tmp.find(';');
                    size_t pos2 = tmp.find('#');
                    if (tmp[tmp.size() - 1] == ';')
                        continue ;
                    if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1)
                    {
                        if (tmp[pos1] == ';')
                            pos1++;
                        while (iswtspaces(tmp[pos1]))
                            pos1++;
                        if (tmp[pos1] && tmp[pos1] != '#')
                        {
                            std::cerr << "Error at line " << i  << " " << tmp << std::endl;
                            RawString.clear();
                            file.close();
                            return ;
                        }
                        std::string trimmed = tmp.substr(0, pos2);
                        TrimSpaces( trimmed );
                        RawString += trimmed;
                    }
                    else
                    {
                        std::cerr << "Error at line " << i  << " " << tmp << std::endl;
                        RawString.clear();
                        file.close();
                        return ;
                    }
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
        std::cerr << "Error opening file" << std::endl;
        return (1);
    }
    return (0);
}

int          File::CheckExtention()
{
    return (extention.compare(".conf"));
}

File::~File(){}
