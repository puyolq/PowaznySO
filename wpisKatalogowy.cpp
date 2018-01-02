#include "wpisKatalogowy.h"
#include <iostream>

wpis::wpis()
{
	this->nazwa = "";
	this->rozszerzenie = "";
	this->numerIwezela = -1;
}

wpis::wpis(std::string nazwa, std::string rozszerzenie, short numer)
{
	this->nazwa = nazwa;
	this->rozszerzenie = rozszerzenie;
	this->numerIwezela = numer;
}

void wpis::czysc()
{
	this->nazwa = "";
	this->rozszerzenie = "";
	this->numerIwezela = -1;
}

void wpis::wypisz()
{
	std::cout << "Nazwa: " << nazwa <<"."<<rozszerzenie << "\n";
	std::cout << "Iwezel: " << numerIwezela <<"\n";
	
}

void wpis::ustawNazwe(std::string nazwa)
{
	this->nazwa = nazwa;
}

std::string wpis::pobierzNazwe()
{
	return this->nazwa;
}

void wpis::ustawRozszrzenie(std::string rozszerzenie)
{
	this->rozszerzenie = rozszerzenie;
}

std::string wpis::pobierzRozszerzenie()
{
	return this->rozszerzenie;
}

void wpis::ustawNumerIwezla(short numer)
{
	this->numerIwezela = numer;
}

short wpis::pobierzNumer()
{
	return this->numerIwezela;
}

