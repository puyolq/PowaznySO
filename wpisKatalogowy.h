#pragma once
#include <string>


class wpis 
{
public:
	wpis ();
	wpis(std::string nazwa, std::string rozszerzenie, short numer);
	void czysc(); // "Zwruje: wpis
	void wypisz();
#pragma region operacje na prywatnych polach
	void ustawNazwe(std::string nazwa);
	std::string pobierzNazwe();

	void ustawRozszrzenie(std::string rozszerzenie);
	std::string pobierzRozszerzenie();

	void ustawNumerIwezla(short numer);
	short pobierzNumer();
#pragma endregion operacje na prywatnych polach
private:
	std::string nazwa;
	std::string rozszerzenie;
	short numerIwezela;
};

