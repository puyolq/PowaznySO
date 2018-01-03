#include "ZarzadzanieProcesami.h"
#include "Kolejka procesow.hpp"
#include <iostream>

PCB::PCB(int _id, std::string _nazwa, PCB* _rodzic)
{
	id = _id;
	nazwa = _nazwa;
	rodzic = _rodzic;
	status = 1;
}

PCB::~PCB()
{
}

PCB* PCB::dodajProces(int id, std::string nazwa, std::string rodzic)
{
	if(znajdzProces(nazwa)!=nullptr) return nullptr;
	PCB* znajdz = znajdzProces(rodzic);
	PCB* local;
	if (znajdz != nullptr)
	{
		local = new PCB(id, nazwa, znajdz);
		znajdz->dodajPotomka(local);
	}
	else
	{
		local = new PCB(id, nazwa, this);
		dodajPotomka(local);
	}
	return local;
}


void PCB::ustawStatus(int _status)
{
	if(_status>0 && _status<5)
	{
		//zmiana z nowy na gotowy
		if (status == 0 && _status == 1)
		{
			status = _status;
			kolejkaGotowych.dodajDoKolejki(this);
		}
		//zmiana z gotowy na aktywny
		else if (status == 1 && _status == 3) status = _status;
		//zmiana z aktywny na gotowy
		else if (status == 3 && _status == 1) status = _status;
		//zmiana z aktywnego na oczekuj¹cy
		else if (status == 3 && _status == 2)
		{
			status = _status;
			uspijProces(this);
		}
		//zmiana z oczekuj¹cego na gotowy
		else if (status == 2 && _status == 1)
		{
			status = _status;
			obudzProces(this);
		}
		//zmiana z aktywnego na zakoñczony
		else if(status==3 && _status==4)
		{
			status = _status;
			usunProces(this->dajNazwe());
		}
		
	}
}

int PCB::dajStatus()
{
	return status;
}

void PCB::ustawRej1(int wartosc)
{
	rej1 = wartosc;
}

int PCB::dajRej1()
{
	return rej1;
}

void PCB::ustawRej2(int wartosc)
{
	rej2 = wartosc;
}

int PCB::dajRej2()
{
	return rej2;
}

void PCB::ustawRej3(int wartosc)
{
	rej3 = wartosc;
}

int PCB::dajRej3()
{
	return rej3;
}

void PCB::ustawRej4(int wartosc)
{
	rej4 = wartosc;
}

int PCB::dajRej4()
{
	return rej4;
}

void PCB::ustawLicznikRozkazow(int wartosc)
{
	licznikRozkazow = wartosc;
}

int PCB::dajLicznikRozkazow()
{
	return licznikRozkazow;
}

Socket* PCB::dajSocket()
{
	return socket;
}

void PCB::ustawSocket(Socket* _socket)
{
	socket = _socket;
}



void PCB::ustawRamLokalizacja(int wartosc)
{
	ramLokalizacja = wartosc;
}

int PCB::dajRamLokalizacja()
{
	return ramLokalizacja;
}

void PCB::ustawRamRozmiar(int wartosc)
{
	ramRozmiar = wartosc;
}

int PCB::dajRamRozmiar()
{
	return ramRozmiar;
}

std::string PCB::dajNazwe()
{
	return nazwa;
}

std::vector<PCB*> PCB::dajPotomkow()
{
	return std::vector<PCB*>();
}

void PCB::dodajPotomka(PCB* potomek)
{
	potomkowie.push_back(potomek);
}

void PCB::wyswietlPotomkow(int lvl)
{
	for (int i = 0; i < lvl; i++) std::clog << "\t";
	std::clog << "-(" << dajId() << ")" << dajNazwe() << std::endl;
	if (potomkowie.size() > 0)
	{
		lvl++;
		for (auto e : potomkowie)
		{
			e->wyswietlPotomkow(lvl);
		}
	}
}

void PCB::wyswietlProces(std::string nazwa)
{
	PCB* local = znajdzProces(nazwa);
	std::clog << "\n\t************\n";
	std::clog << "\tId: " << local->dajId() << std::endl;
	std::clog << "\tNazwa: " << local->dajNazwe() << std::endl;
	std::clog << "\tStan: ";
	int status = local->dajStatus();
	if (status == 0) std::clog << "0(nowy)";
	else if (status == 1) std::clog << "1(gotowy)";
	else if (status == 2) std::clog << "2(oczekujacy)";
	else if (status == 3) std::clog << "3(aktywny)";
	else if (status == 4) std::clog << "4(zakonczony)";
	std::clog << std::endl;
	std::clog << "\tIlosc potomkow: " << local->zliczProcesy() - 1 << std::endl;
	std::clog << "\t************\n\n";
}


PCB* PCB::znajdzProces(std::string _nazwa)
{
	if (dajNazwe() == _nazwa) return this;
	if (potomkowie.size() > 0)
	{
		for (auto e : potomkowie)
		{
			PCB* local = e->znajdzProces(_nazwa);
			if (local != nullptr)return local;
		}
	}
	return nullptr;
}

PCB* PCB::dajRodzica()
{
	return rodzic;
}

int PCB::dajId()
{
	return id;
}

void PCB::usunProces(std::string nazwa)
{
	PCB* local = znajdzProces(nazwa);
	if (local != nullptr)
	{
		przeniesPotomkow(this, local);
		PCB* ojciec = local->dajRodzica();
		ojciec->usunPotomka(nazwa);
		kolejkaGotowych.usunProces(local->dajId());
		delete local;
	}
}

void PCB::usunPotomka(std::string nazwa)
{
	for (auto i = potomkowie.begin(); i != potomkowie.end(); ++i)
	{
		if ((*i)->dajNazwe() == nazwa)
		{
			potomkowie.erase(i);
			break;
		}
	}
}

void PCB::przeniesPotomkow(PCB* init, PCB* doPrzeniesienia)
{
	if (doPrzeniesienia->potomkowie.size() > 0)
	{
		for (auto e : doPrzeniesienia->potomkowie)
		{
			e->ustawRodzica(init);
			init->dodajPotomka(e);
		}
	}
}


void PCB::ustawRodzica(PCB* _rodzic)
{
	rodzic = _rodzic;
}

int PCB::zliczProcesy()
{
	int suma = 1;
	if (potomkowie.size() > 0)
	{
		for (auto e : potomkowie)
		{
			suma += e->zliczProcesy();
		}
	}
	return suma;
}


/* ZARZADZANIE PROCESAMI */

int ZarzadzanieProcesami::dajLicznik()
{
	idLicznik++;
	return idLicznik;
}

ZarzadzanieProcesami::ZarzadzanieProcesami()
{
	std::clog << ">> Proces init uruchomiony.\n";
	idLicznik = 0;
	init = new PCB(0, "init", nullptr);
}

ZarzadzanieProcesami::~ZarzadzanieProcesami()
{
}

void ZarzadzanieProcesami::wyswietlIloscProcesow()
{
	std::clog << "Ilosc procesow: " << init->zliczProcesy() << std::endl;
}

void ZarzadzanieProcesami::wyswietlWszystkieProcesy()
{
	init->wyswietlPotomkow(0);
}


void ZarzadzanieProcesami::wyswietlProces(std::string nazwa)
{
	init->wyswietlProces(nazwa);
}


PCB* ZarzadzanieProcesami::dodajProces(std::string nazwa, std::string rodzic)
{
	PCB*local = init->dodajProces(dajLicznik(), nazwa, rodzic);
	kolejkaGotowych.dodajDoKolejki(local);
	return local;
}

void ZarzadzanieProcesami::usunProces(std::string nazwa)
{
	init->usunProces(nazwa);
}

