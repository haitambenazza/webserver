#include "../headers/webserver.hpp"

int AllDigit(std::string& s)
{
    size_t  i;

    i = 0;
    while (i < s.size())
    {
        if (!isdigit(s[i]))
            return (1);
        i++;
    }
    return (0);
}   

int CheckIp( std::string& s )
{
    size_t  i;
    int     count;
    std::vector <std::string> lst;

    i = 0;
    count = 0;
    while (i < s.size())
    {
        if (s[i] == '.')
        {
            if (s[i - 1] && s[i - 1] == '.')
                return (1);
            count++;
        }
        else if (!isdigit(s[i]))
            return (1);
        i++;
    }
    if (count != 3)
        return (1);
    lst = split(s, ".");
    if (lst.size() != 4)
        return (1);
    i = 0;
    while (i < lst.size())
    {
        if (atoi(lst[i].c_str()) < 0 || atoi(lst[i].c_str()) > 255)
            return (1);
        i++;
    }
    return (0);
}
