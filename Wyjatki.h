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

class blednaNazwaPliku : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Nie odnaleziono pliku.";
	}
};

class brakDostepuDoPliku : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Semafor zablokowany, brak dostepu.";
	}
};

class brakMiejsca : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Brak miejsca na dysku.";
	}
};

class brakWolnychSynow : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "W folderze nie mozna utworzyc nowych podfolderow.";
	}
};

class niejednoznacznaNazwa : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Nazwa nie jest jednoznaczna.";
	}
};