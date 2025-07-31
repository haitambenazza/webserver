#pragma once

#include "../headers/webserver.hpp"

class Method
{
	private:
		std::string Type;
	public:
		Method();
		Method( const Method & m);
		Method &operator=(const Method &other);
		~Method();
		std::string	GetMethodType() const;
		void		SetMethodType(std::string type);
};
