#pragma once
#include <exception>


class blednaNazwaPliku : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Nie odnaleziono pliku.";
	}
};

