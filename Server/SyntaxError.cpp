#include "../headers/webserver.hpp"

bool AllDigit(std::string s)
{
    size_t  i;

    i = 0;
    while (i < s.size())
    {
        if (!isdigit(s[i]))
            return (false);
        i++;
    }
    return (true);
}   

bool CheckIp( std::string s )
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
                return (false);
            count++;
        }
        else if (!isdigit(s[i]))
            return (false);
        i++;
    }
    if (count != 3)
        return (false);
    lst = split(s, ".");
    if (lst.size() != 4)
        return (false);
    i = 0;
    while (i < lst.size())
    {
        if (atoi(lst[i].c_str()) < 0 || atoi(lst[i].c_str()) > 255)
            return (false);
        i++;
    }
    return (true);
}
