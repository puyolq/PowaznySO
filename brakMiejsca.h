#pragma once
#include <exception>

class brakMiescja : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Brak miejsca na dysku.";
	}
};
