#pragma once

#include <exception>


class blednaNazwaFolderu : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Bledna nazwa folderu.";
	}
};
