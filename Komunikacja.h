#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>

class Wiadomosc {
private:
	std::string komunikat;
	int numerPIDNadawcy;

public:
	//Konstruktor klasy wiadomosc
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
	bool wyswietlWiadomosc();

	void zamknijGniazdo();
};

//Klasa zarzadzajaca cala komunikacja. Tworzona przy uruchomieniu systemu operacyjnego
class KomunikacjaMiedzyprocesowa {
private:
	std::vector<Gniazdo> spisWszystkichGniazd;

	//Znajduje nowy, unikalny deskryptor gniazda
	int znajdzNowyDeskryptor();

	//Funkcja przyporz¹dkowuje nowe gniazdo procesowi o podanym ID
	void stworzNoweGniazdoDlaProcesu(int numerIDProcesu);

	//Funkcja zwracajaca indeks danego gniazda w wektorze gniazd dla podanego deskryptora
	int znajdzIndeksWSpisie(int deskryptorPoszukiwanegoGniazda);

public:
	//Konstruktor
	KomunikacjaMiedzyprocesowa();

	//Pierwszy rozkaz dla Interpretera - odebranie komunikatu
	bool rozkazOdebraniaKomunikatu(int numerIDProcesuOdbiorcy);

	//Drugi rozkaz dla Interpretera - wyslanie komunikatu
	bool rozkazWyslaniaKomunikatu(int numerIDProcesuNadawcy, std::string nazwaProcesuOdbiorcy, std::string komunikat);

	//Usuniecie gniazda - wywolywane wraz z zamknieciem procesu
	void usunGniazdo(int deskryptorGniazdaDoUsuniecia);
};

extern KomunikacjaMiedzyprocesowa komunikacja;