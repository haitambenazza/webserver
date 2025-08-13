
#include "Includes.hpp"

void    RemoveNewLine( std::string& s )
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < (int)s.length() || j < (int)s.length())
    {
        if (s[i] == '\n')
        {
            while (s[j] == '\n')
                j++;
            s[i] = s[j];
        }
        i++;
        j++;
    }
}

void MaskInsideBrace( std::string& s )
{
    int i;
    int k;

    i = 0;
    while (s[i] != '{')
        i++;
    if (s[i] && s[i] == '{')
        i++;
    k = 0;
    while ( i < (int)s.length() - 1 )
    {
        if ( s[i] == '{' || s[i] == '}' )
            s[i] = '"';
        i++;
    }
}

std::vector<std::string> split( std::string s, std::string delimiter )
{  
    int                         st;
    int                         end;
    std::vector<std::string>    lst;
    bool                        InQuote;

    if (s.empty())
    {
        return std::vector<std::string>();
    }
    end = 0;
    st = 0;
    InQuote = false;
    while ( end < (int)s.length() )
    {
        while ( end < (int)s.length() && delimiter.find(s[end]) != std::string::npos && !InQuote )
            end++;
        st = end;
        while ( end < (int)s.length() && (InQuote || delimiter.find(s[end]) == std::string::npos ))
        {
            if (s[end] == '"')
                InQuote = !InQuote;
            end++;
        }
        if ( end > st )
            lst.push_back(s.substr(st, end - st));
    }
    return (lst);
}
