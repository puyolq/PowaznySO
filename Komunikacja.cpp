
#include "Komunikacja.h"
#include "ZarzadzanieProcesami.h"

KomunikacjaMiedzyprocesowa komunikacja;

//--------------------- klasa Wiadomosc - przechowuje komunikaty wraz z numerami ID nadawcow
Wiadomosc::Wiadomosc(int numerPIDNadawcy, std::string komunikat) {
	this->numerPIDNadawcy = numerPIDNadawcy;
	this->komunikat = komunikat;
}

std::string Wiadomosc::pobierzKomunikat() {
	return this->komunikat;
}

int Wiadomosc::pobierzNumerPIDNadawcy() {
	return this->numerPIDNadawcy;
}

//---------------------------- Klasa Gniazdo
Gniazdo::Gniazdo() {
	this->numerIDWlasciciela = 0;
	this->deskryptorGniazda = 0;
	listaOdebranychWiadomosci.clear();
}

int Gniazdo::pobierzDeskryptor()
{
	return this->deskryptorGniazda;
}

void Gniazdo::ustawDeskryptor(int nowyDeskryptorGniazda)
{
	this->deskryptorGniazda = nowyDeskryptorGniazda;
}

int Gniazdo::pobierzIDWlasciciela()
{
	return numerIDWlasciciela;
}

//odpowiednik bind, laczy proces z gniazdem
void Gniazdo::ustawIDWlasciciela(int nowyNumerID)
{
	this->numerIDWlasciciela = nowyNumerID;
}

//dodaj wiadomoœæ na listê odebranych wiadomoœci - funkcja wywolywana przez inne gniazda
void Gniazdo::dodajWiadomosc(Wiadomosc wyslanaWiadomosc)
{
	listaOdebranychWiadomosci.push_back(wyslanaWiadomosc);
}

//pobierz wiadomoœæ z listy wiadomoœci - pobiera pierwsz¹ wiadomoœæ z listy odebranych wiadomoœci i wyswietla ja w konsoli
//jeœli lista jest pusta, zwraca false. Jesli na liscie jest przynajmniej jedna wiadomosc, zwraca true
bool Gniazdo::pobierzWiadomosc(Wiadomosc &odebranaWiadomosc) {
	if (listaOdebranychWiadomosci.size() == 0)
		return false;
	//Pobranie Wiadomosci i usuniecie jej z odebranych
	odebranaWiadomosc = listaOdebranychWiadomosci.front();
	listaOdebranychWiadomosci.pop_front();
	return true;
}

void Gniazdo::zamknijGniazdo() {
	//wyczyszczenie listy odebranych wiadomosci
	listaOdebranychWiadomosci.clear();
	//ustawienie deskryptora gniazda w PCB jego wlasciciela na 0
	// ????????????????
	PCB* wskaznikNaProcesWlasciciela = zarzadzanieProcesami.znajdzProces(numerIDWlasciciela);
	wskaznikNaProcesWlasciciela->ustawDeskryptorGniazda(0);
	//ustawienie numeru ID wlasciciela Gniazda na 0
	this->numerIDWlasciciela = 0;
}

//------------------- Klasa KomunikacjaMiedzyprocesowa - uruchamiana wraz z rozruchem Systemu Operacyjnego
//Znajduje nowy, unikalny deskryptor gniazda
int KomunikacjaMiedzyprocesowa::znajdzNowyDeskryptor()
{
	int nowyDeskryptorGniazda = 1;
	for (int i = 0; i < spisWszystkichGniazd.size(); i++)
	{
		//jesli ktores z gniazd ma juz ten deskryptor
		if (spisWszystkichGniazd[i].pobierzDeskryptor() == nowyDeskryptorGniazda)
		{
			//sprawdz kolejny deskryptor
			nowyDeskryptorGniazda++;
			//od poczatku spisu gniazd
			i = 0;
		}
	}
	return nowyDeskryptorGniazda;
}

//Funkcja przyporz¹dkowuje nowe gniazdo procesowi o podanym ID
void KomunikacjaMiedzyprocesowa::stworzNoweGniazdoDlaProcesu(int numerIDProcesu) {
	Gniazdo noweGniazdo;
	noweGniazdo.ustawDeskryptor(znajdzNowyDeskryptor());
	//?????????????????????
	PCB* wskaznikNaProces = zarzadzanieProcesami.znajdzProces(numerIDProcesu);
	wskaznikNaProces->ustawDeskryptorGniazda(noweGniazdo.pobierzDeskryptor());
	noweGniazdo.ustawIDWlasciciela(numerIDProcesu);
	spisWszystkichGniazd.push_back(noweGniazdo);
}

//Funkcja zwracajaca indeks danego gniazda w wektorze gniazd dla podanego deskryptora
int KomunikacjaMiedzyprocesowa::znajdzIndeksWSpisie(int deskryptorPoszukiwanegoGniazda)
{
	for (int i = 0; i < spisWszystkichGniazd.size(); i++)
		if (spisWszystkichGniazd[i].pobierzDeskryptor() == deskryptorPoszukiwanegoGniazda) return i;
	//jesli w spisie nie ma gniazda o takim deskryptorze, funkcja zwraca wartosc -1
	return (-1);
}

//Konstruktor
KomunikacjaMiedzyprocesowa::KomunikacjaMiedzyprocesowa() {}

//Pierwszy rozkaz dla Interpretera - odebranie komunikatu
Wiadomosc KomunikacjaMiedzyprocesowa::rozkazOdebraniaKomunikatu(int numerIDProcesuOdbiorcy)
{
	Wiadomosc odebranaWiadomosc(0, "");
	PCB* wskaznikNaProcesOdbiorcy = zarzadzanieProcesami.znajdzProces(numerIDProcesuOdbiorcy);
	if (wskaznikNaProcesOdbiorcy->dajDeskryptorGniazda() == 0)
	{
		stworzNoweGniazdoDlaProcesu(numerIDProcesuOdbiorcy);
	}
	int indeksGniazdaOdbiorcy = znajdzIndeksWSpisie(wskaznikNaProcesOdbiorcy->dajDeskryptorGniazda());
	if (spisWszystkichGniazd[indeksGniazdaOdbiorcy].pobierzWiadomosc(odebranaWiadomosc) == false)
	{
		//ZABLOKUJ PROCES O NUMERZE ID = numerIDProcesuOdbiorcy;
		wskaznikNaProcesOdbiorcy->ustawStatus(2);
		return odebranaWiadomosc;
	}
	else {
		return odebranaWiadomosc;
	}

	/*
	//?????????????????????
	PCB* wskaznikNaProcesOdbiorcy = zarzadzanieProcesami.znajdzProces(numerIDProcesuOdbiorcy);
	if (wskaznikNaProcesOdbiorcy->dajDeskryptorGniazda() == 0)
	{
	stworzNoweGniazdoDlaProcesu(numerIDProcesuOdbiorcy);
	}
	int indeksGniazdaOdbiorcy = znajdzIndeksWSpisie(wskaznikNaProcesOdbiorcy->dajDeskryptorGniazda());
	if (spisWszystkichGniazd[indeksGniazdaOdbiorcy].wyswietlWiadomosc() == false)
	{
	//ZABLOKUJ PROCES O NUMERZE ID = numerIDProcesuOdbiorcy;
	wskaznikNaProcesOdbiorcy->ustawStatus(2);
	return false;
	}
	else {
	std::cout << "Komunikat zostal odebrany i wyswietlony. Program powinien kontynuowac dzialanie." << std::endl;
	}
	return true;
	*/
}

//Drugi rozkaz dla Interpretera - wyslanie komunikatu
bool KomunikacjaMiedzyprocesowa::rozkazWyslaniaKomunikatu(int numerIDProcesuNadawcy, std::string nazwaProcesuOdbiorcy, std::string komunikat)
{
	Wiadomosc nowaWiadomoscDoWyslania(numerIDProcesuNadawcy, komunikat);
	PCB* wskaznikNaProcesNadawcy = zarzadzanieProcesami.znajdzProces(numerIDProcesuNadawcy);
	PCB* wskaznikNaProcesOdbiorcy = zarzadzanieProcesami.znajdzProces(nazwaProcesuOdbiorcy);
	//Jesli proces, do ktorego chcesz wyslac wiadomosc nie istnieje lub nie ma swojego Gniazda
	if (wskaznikNaProcesOdbiorcy == nullptr || wskaznikNaProcesOdbiorcy->dajDeskryptorGniazda() == 0)
	{
		// ZAMKNIJ PROCES I ZAKONCZ JEGO DZIALANIE KOMPLETNIE
		wskaznikNaProcesNadawcy->ustawStatus(4);
		return false;
	}

	if (wskaznikNaProcesNadawcy->dajDeskryptorGniazda() == 0)
	{
		stworzNoweGniazdoDlaProcesu(wskaznikNaProcesNadawcy->dajId());
	}
	int indeksGniazdaOdbiorcy = znajdzIndeksWSpisie(wskaznikNaProcesOdbiorcy->dajDeskryptorGniazda());
	spisWszystkichGniazd[indeksGniazdaOdbiorcy].dodajWiadomosc(nowaWiadomoscDoWyslania);
	//?????????? ODBLOKOWANIE PROCESU JEŒLI JEST UŒPIONY
	if (wskaznikNaProcesOdbiorcy->dajStatus() == 2)
		wskaznikNaProcesOdbiorcy->ustawStatus(1);
	return true;
}

//Usuniecie gniazda - wywolywane wraz z zamknieciem procesu
void KomunikacjaMiedzyprocesowa::usunGniazdo(int deskryptorGniazdaDoUsuniecia)
{
	if (deskryptorGniazdaDoUsuniecia != 0)
	{
		//Znalezienie w spisie gniazda o poszukiwanym deskryptorze
		for (auto it = spisWszystkichGniazd.begin(); it != spisWszystkichGniazd.end();)
		{
			if (it->pobierzDeskryptor() == deskryptorGniazdaDoUsuniecia)
			{
				//wykonanie funkcji zamknij gniazdo z klasy Gniazdo
				it->zamknijGniazdo();
				//usuniecie Gniazda ze spisu wszystkich gniazd
				spisWszystkichGniazd.erase(it);
			}
		}
	}
}
