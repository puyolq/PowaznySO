#pragma once
#include <exception>

class brakDostepuDoPliku : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Semafor zablokowany, brak dostepu.";
	}
};
