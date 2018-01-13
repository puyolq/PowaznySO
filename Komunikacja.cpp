
#include "Komunikacja.h"
#include "ZarzadzanieProcesami.h"

KomunikacjaMiedzyprocesowa komunikacja;

//--------------------- klasa Wiadomosc - przechowuje komunikaty wraz z numerami ID nadawcow
Wiadomosc::Wiadomosc() {}

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
	Semafory semafor(0);
	this->semaforKanalu = semafor;
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
	PCB* wskaznikNaProcesWlasciciela = zarzadzanieProcesami.znajdzProces(numerIDWlasciciela);
	wskaznikNaProcesWlasciciela->ustawDeskryptorGniazda(0);
	//ustawienie numeru ID wlasciciela Gniazda na 0
	this->numerIDWlasciciela = 0;
}

void Gniazdo::zablokujWlasciciela()
{
	PCB* wskaznikNaProces = zarzadzanieProcesami.znajdzProces(numerIDWlasciciela);
	semaforKanalu.wait(wskaznikNaProces);
}

void Gniazdo::odblokujWlasciciela()
{
	PCB* wskaznikNaProces = zarzadzanieProcesami.znajdzProces(numerIDWlasciciela);
	semaforKanalu.signal(wskaznikNaProces);
}

bool Gniazdo::czyWlascicielZablokowany()
{
	if (semaforKanalu.dlugosc() == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//------------------- Klasa KomunikacjaMiedzyprocesowa - uruchamiana wraz z rozruchem Systemu Operacyjnego

//Funkcja sprawdzajaca, czy dany deskryptor jest wolny. true - wolny, false - zajêty
bool KomunikacjaMiedzyprocesowa::sprawdzCzyDanyDeskryptorJestWolny(int testowanyDeskryptorGniazda)
{
	if (spisWszystkichGniazd.size() == 0)
		return true;
	for (Gniazdo sprawdzaneGniazdo : spisWszystkichGniazd) {
		if (sprawdzaneGniazdo.pobierzDeskryptor() == testowanyDeskryptorGniazda) {
			//jesli w spisie jest ju¿ gniazdo o takim deskryptorze funkcja zwraca wartosc flase
			return false;
		}
	}
	//jesli w spisie nie ma gniazda o takim deskryptorze, funkcja zwraca wartosc true
	return true;
}

//Konstruktor
KomunikacjaMiedzyprocesowa::KomunikacjaMiedzyprocesowa() {}

//Funkcja zwracaj¹ca wskaznik na gniazdo w spisie (nullptr gdy w spisie nie ma gniazda o podanym deskryptorze)
Gniazdo* KomunikacjaMiedzyprocesowa::znajdzGniazdoWSpisie(int deskryptorPoszukiwanegoGniazda)
{
	for (int i = 0; i < spisWszystkichGniazd.size(); i++)
	{
		if (spisWszystkichGniazd[i].pobierzDeskryptor() == deskryptorPoszukiwanegoGniazda)
			return &spisWszystkichGniazd[i];
	}

	//Jezeli w spisie nie znaleziono gniazda o podanym deskryptorze, to funkcja zwraca nullptr
	return nullptr;
}

//Funkcja tworzy nowe gniazdo o podanym deskryptorze
void KomunikacjaMiedzyprocesowa::stworzNoweGniazdo(int numerIDProcesu, int deskryptorGniazda)
{
	PCB* wskaznikNaProces = zarzadzanieProcesami.znajdzProces(numerIDProcesu);
	//Jesli podany deskryptor jest juz zajety, nie mozna utworzy gniazda o tym deskryptorze
	if (sprawdzCzyDanyDeskryptorJestWolny(deskryptorGniazda) == false)
	{
		std::cout << "Blad komunikacji : podany deskryptor gniazda jest juz zajety" << std::endl;
		wskaznikNaProces->ustawBlad(true);
		return;
	}
	Gniazdo noweGniazdo;
	noweGniazdo.ustawDeskryptor(deskryptorGniazda);
	spisWszystkichGniazd.push_back(noweGniazdo);
}

//Funkcja wi¹¿¹ca gniazdo o podanym deskryptorze z procesem o podanym ID
void KomunikacjaMiedzyprocesowa::powiazanieGniazdaZProcesem(int numerIDProcesu, int deskryptorGniazda)
{
	PCB* wskaznikNaProces = zarzadzanieProcesami.znajdzProces(numerIDProcesu);
	if (wskaznikNaProces->dajDeskryptorGniazda() != 0)
	{
		std::cout << "Blad komunikacji : proces ma juz gniazdo o deskryptorze: " << wskaznikNaProces->dajDeskryptorGniazda() << std::endl;
		wskaznikNaProces->ustawBlad(true);
		return;
	}
	Gniazdo* wskaznikNaGniazdo = znajdzGniazdoWSpisie(deskryptorGniazda);
	if (wskaznikNaGniazdo == nullptr)
	{
		std::cout << "Blad komunikacji : nie znaleziono gniazda o podanym deskryptorze" << std::endl;
		wskaznikNaProces->ustawBlad(true);
		return;
	}
	if (wskaznikNaGniazdo->pobierzIDWlasciciela() != 0)
	{
		std::cout << "Blad komunikacji : gniazdo o podanym deskryptorze jest juz powiazane z procesem o ID: " << wskaznikNaProces->dajId() << std::endl;
		wskaznikNaProces->ustawBlad(true);
		return;
	}
	//Po sprawdzeniu, ze podany proces nie ma gniazda, a podane gniazdo istnieje i nie ma wlasciciela
	//Zapisujemy numer ID w gniezdzie
	wskaznikNaGniazdo->ustawIDWlasciciela(wskaznikNaProces->dajId());
	//Zapisujemy deskryptor gniazda w bloku PCB procesu
	wskaznikNaProces->ustawDeskryptorGniazda(wskaznikNaGniazdo->pobierzDeskryptor());
}

//Pierwszy rozkaz dla Interpretera - odebranie komunikatu
Wiadomosc KomunikacjaMiedzyprocesowa::rozkazOdebraniaKomunikatu(int numerIDProcesuOdbiorcy)
{
	Wiadomosc odebranaWiadomosc(0, "");
	PCB* wskaznikNaProcesOdbiorcy = zarzadzanieProcesami.znajdzProces(numerIDProcesuOdbiorcy);
	if (wskaznikNaProcesOdbiorcy->dajDeskryptorGniazda() == 0)
	{
		std::cout << "Blad komunikacji: podany proces nie ma gniazda" << std::endl;
		wskaznikNaProcesOdbiorcy->ustawBlad(true);
		return odebranaWiadomosc;
	}
	Gniazdo* wskaznikNaGniazdo = znajdzGniazdoWSpisie(wskaznikNaProcesOdbiorcy->dajDeskryptorGniazda());
	if (wskaznikNaGniazdo == nullptr)
	{
		std::cout << "Blad komunikacji : nie znaleziono gniazda o podanym deskryptorze" << std::endl;
		wskaznikNaProcesOdbiorcy->ustawBlad(true);
		return odebranaWiadomosc;
	}
	if (wskaznikNaGniazdo->pobierzWiadomosc(odebranaWiadomosc) == false)
	{
		//ZABLOKUJ PROCES O NUMERZE ID = numerIDProcesuOdbiorcy;
		//wskaznikNaProcesOdbiorcy->ustawStatus(2);
		wskaznikNaGniazdo->zablokujWlasciciela();
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
void KomunikacjaMiedzyprocesowa::rozkazWyslaniaKomunikatu(int numerIDProcesuNadawcy, int deskryptorGniazdaOdbiorcy, std::string komunikat)
{
	Wiadomosc nowaWiadomoscDoWyslania(numerIDProcesuNadawcy, komunikat);
	PCB* wskaznikNaProcesNadawcy = zarzadzanieProcesami.znajdzProces(numerIDProcesuNadawcy);
	Gniazdo* wskaznikNaGniazdoOdbiorcy = znajdzGniazdoWSpisie(deskryptorGniazdaOdbiorcy);
	//Jesli proces nadawca nie ma swojego gniazda - ERROR
	if (wskaznikNaProcesNadawcy->dajDeskryptorGniazda() == 0)
	{
		std::cout << "Blad komunikacji : proces nadawcy nie ma gniazda" << std::endl;
		wskaznikNaProcesNadawcy->ustawBlad(true);
		return;
	}
	//Jesli nie istnieje gniazdo o podanym deskryptorze - ERROR
	if (wskaznikNaGniazdoOdbiorcy == nullptr)
	{
		std::cout << "Blad komunikacji : nie istnieje gniazdo o podanym deskryptorze" << std::endl;
		wskaznikNaProcesNadawcy->ustawBlad(true);
		return;
	}
	wskaznikNaGniazdoOdbiorcy->dodajWiadomosc(nowaWiadomoscDoWyslania);
	if (wskaznikNaGniazdoOdbiorcy->czyWlascicielZablokowany() == true)
		wskaznikNaGniazdoOdbiorcy->odblokujWlasciciela();
}

void KomunikacjaMiedzyprocesowa::wyswietlSpisWszystkichGniazd()
{
	for (Gniazdo wyswietlaneGniazdo : spisWszystkichGniazd)
	{
		std::cout << "Deskryptor: " << wyswietlaneGniazdo.pobierzDeskryptor() << " Wlasciciel: " << wyswietlaneGniazdo.pobierzIDWlasciciela() << std::endl;
	}
}

//Usuniecie gniazda
void KomunikacjaMiedzyprocesowa::usunGniazdo(int numerIDProcesu)
{
	PCB* wskaznikNaProces = zarzadzanieProcesami.znajdzProces(numerIDProcesu);
	if (wskaznikNaProces->dajDeskryptorGniazda() == 0)
	{
		std::cout << "Blad komunikacji : ten proces nie ma przypisanego gniazda" << std::endl;
		wskaznikNaProces->ustawBlad(true);
		return;
	}
	else
	{
		int deskryptorGniazdaDoUsuniecia = wskaznikNaProces->dajDeskryptorGniazda();
		//Znalezienie w spisie gniazda o poszukiwanym deskryptorze
		for (auto it = spisWszystkichGniazd.begin(); it != spisWszystkichGniazd.end(); ++it)
		{
			if (it->pobierzDeskryptor() == deskryptorGniazdaDoUsuniecia)
			{
				//wykonanie funkcji zamknij gniazdo z klasy Gniazdo
				it->zamknijGniazdo();
				//usuniecie Gniazda ze spisu wszystkich gniazd
				spisWszystkichGniazd.erase(it);
				break;
			}
		}
	}
}