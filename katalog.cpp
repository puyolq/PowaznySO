#include "katalog.h"		
#include <iostream>

katalog::katalog()
{
	this->nazwa = "";
	this->podfoldery = 0;
	wskaznikNaOjca = -1;
	for (short wskaznik_na_syna : wskaznikNaSyna)
	{
		wskaznik_na_syna = -1;
	}
}

katalog::katalog(std::string nazwa)
{
	this->nazwa = nazwa;
	wskaznikNaOjca = -1;

	for(int i=0; i<wskaznikNaSyna.size(); i++)
	{
		wskaznikNaSyna[i] = -1;
	}
}

katalog::katalog(std::string nazwa, short wskaznikNaOjca)
{
	this->nazwa = nazwa;
	this->wskaznikNaOjca = wskaznikNaOjca;
	
	for(int i=0; i<wskaznikNaSyna.size(); i++)
	{
		wskaznikNaSyna[i] = -1;
	}
}

void katalog::wypiszKatalog()
{
	printf("Nazwa: %s\n", this->nazwa);
	printf("Zawarte iWezly:\n");
	for (short element : this->numeryIwezlow)
	{
		std::cout << element << " ";
	}
	std::cout << "\n";
}

void katalog::ustawNazwe(std::string nazwa)
{
	this->nazwa = nazwa;
}

std::string katalog::pobierzNazwe()
{
	return this->nazwa;
}

void katalog::dodajNumerIwezela(short numer)
{
	this->numeryIwezlow.push_back(numer);
}

void katalog::usunNumerIwezla(short numer)
{
	this->numeryIwezlow.erase(std::find(this->numeryIwezlow.begin(), this->numeryIwezlow.end(), numer) + 0);
}

std::vector<short> katalog::pobierzNumery()
{
	return this->numeryIwezlow;
}

void katalog::ustawOjca(short ojciec)
{
	this->wskaznikNaOjca = ojciec;
}

short katalog::pobierzOjca()
{
	return this->wskaznikNaOjca;
}

void katalog::ustawSyna(short syn)
{
	for(int i=0; i<wskaznikNaSyna.size(); i++)
	{
		if(wskaznikNaSyna[i]==-1)
		{
			wskaznikNaSyna[i] = syn;
			break;
		}
	}
}

std::array<short, 32> katalog::pobierzSynow()
{
	return this->wskaznikNaSyna;
}

void katalog::usunSyna(short syn)
{
	for(int i=0; i<wskaznikNaSyna.size(); i++)
	{
		if(wskaznikNaSyna[i]==syn)
		{
			wskaznikNaSyna[i] = -1;
		}
	}
}

short katalog::ilePodfolderow()
{
	return podfoldery;
}

void katalog::ustawPodfoldery(int i)
{
	podfoldery = i;
}



