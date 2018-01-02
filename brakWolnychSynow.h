#pragma once
#include <exception>
#include <string>

class brakWolnychSynow : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "W folderze nie mozna utworzyc nowych podfolderow.";
	}
};
