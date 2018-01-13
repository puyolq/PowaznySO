#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "Semafory.h"

class Wiadomosc {
private:
	std::string komunikat;
	int numerPIDNadawcy;

public:
	Wiadomosc();
	Wiadomosc(int numerPIDNadawcy, std::string komunikat);

	std::string pobierzKomunikat();
	int pobierzNumerPIDNadawcy();
};

class Gniazdo {
private:
	//lista wiadomosci
	std::list<Wiadomosc> listaOdebranychWiadomosci; //( stos )
	int numerIDWlasciciela; //unikalny numer ID procesu powiazanego z gniazdem
	int deskryptorGniazda; //unikalny numer gniazda

	Semafory semaforKanalu;
public:

	Gniazdo();
	int pobierzDeskryptor();
	void ustawDeskryptor(int nowyDeskryptorGniazda);

	int pobierzIDWlasciciela();

	//odpowiednik bind, laczy proces z gniazdem
	void ustawIDWlasciciela(int nowyNumerID);

	//dodaj wiadomoœæ na listê odebranych wiadomoœci - funkcja wywolywana przez inne gniazda
	void dodajWiadomosc(Wiadomosc wyslanaWiadomosc);

	//pobierz wiadomoœæ z listy wiadomoœci - pobiera pierwsz¹ wiadomoœæ z odebranych wiadomoœci i wyswietla ja w konsoli
	//jeœli lista jest pusta, zwraca false. Jesli na liscie jest przynajmniej jedna wiadomosc, zwraca true
	bool pobierzWiadomosc(Wiadomosc &odebranaWiadomosc);

	void zamknijGniazdo();

	void zablokujWlasciciela();

	void odblokujWlasciciela();

	bool czyWlascicielZablokowany();
};

//Klasa zarzadzajaca cala komunikacja. Tworzona przy uruchomieniu systemu operacyjnego
class KomunikacjaMiedzyprocesowa {
private:
	std::vector<Gniazdo> spisWszystkichGniazd;

	//Funkcja sprawdzajaca, czy dany deskryptor jest wolny. true - wolny, false - zajêty
	bool KomunikacjaMiedzyprocesowa::sprawdzCzyDanyDeskryptorJestWolny(int testowanyDeskryptorGniazda);

public:
	//Konstruktor
	KomunikacjaMiedzyprocesowa();

	//Funkcja zwracaj¹ca wskaznik na gniazdo w spisie (nullptr gdy w spisie nie ma gniazda o podanym deskryptorze)
	Gniazdo* znajdzGniazdoWSpisie(int deskryptorGniazda);

	//Funkcja przyporz¹dkowuje nowe gniazdo procesowi o podanym ID
	void KomunikacjaMiedzyprocesowa::stworzNoweGniazdo(int numerIDProcesu, int deskryptorGniazda);

	//Funkcja wi¹¿¹ca gniazdo o podanym deskryptorze z procesem o podanym ID
	void KomunikacjaMiedzyprocesowa::powiazanieGniazdaZProcesem(int numerIDProcesu, int deskryptorGniazda);

	//Pierwszy rozkaz dla Interpretera - odebranie komunikatu
	Wiadomosc rozkazOdebraniaKomunikatu(int numerIDProcesuOdbiorcy);

	//Drugi rozkaz dla Interpretera - wyslanie komunikatu
	void rozkazWyslaniaKomunikatu(int numerIDProcesuNadawcy, int deskryptorGniazdaOdbiorcy, std::string komunikat);

	void wyswietlSpisWszystkichGniazd();

	//Usuniecie gniazda - wywolywane wraz z zamknieciem procesu
	void KomunikacjaMiedzyprocesowa::usunGniazdo(int numerIDProcesu);
};

extern KomunikacjaMiedzyprocesowa komunikacja;