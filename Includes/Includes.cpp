#include "Includes.hpp"

int     IsWhiteSpace( char c )
{
    return (c == 32 || (c >= 9 && c <= 13));
}

void    TrimSpaces( std::string& s )
{
    int start;
    int end;


    start = 0;
    end = (int)s.length() - 1;
    while (s[end] && IsWhiteSpace(s[end]))
        end--;
    while (s[start] && IsWhiteSpace(s[start]))
        start++;
    s = s.substr(start, end - start + 1);
}
