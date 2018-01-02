#pragma once
#include <exception>


class niejednoznacznaNazwa : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Nazwa nie jest jednoznaczna.";
	}
};
