#include <iostream>

class apa
{
    private :
        const std::string name;
        int appa;
    public :
        apa();
        ~apa();
        apa &operator=( const apa& other );
};

apa::apa():name("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), appa(69){}
apa& apa::operator=( const apa& other )
{
    if (this != &other)
    {
        //name = other.name;
        appa = other.appa;
    }
    return (*this);
}
apa::~apa(){}