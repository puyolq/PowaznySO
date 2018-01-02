#pragma once
#include <string>
#include <vector>


struct Socket
{
};

//

//
/* STANY
 * 0 - nowy
 * 1 - gotowy
 * 2 - oczekuj¹cy
 * 3 - aktywny
 * 4 - zakoñczony
 */


class PCB
{
private:
	//ogólne sk³adniki
	int id;
	std::string nazwa;
	std::vector<PCB*> potomkowie;
	PCB* rodzic;
public:
	PCB(int id, std::string nazwa, PCB* rodzic);
	PCB()
	{
		id = 0;
		//potomkowie = nullptr;
		rodzic = nullptr;
	}
	~PCB();
	PCB* dodajProces(int id, std::string nazwa, std::string rodzic);
	std::string dajNazwe();
	std::vector<PCB*> dajPotomkow();
	void dodajPotomka(PCB* potomek);
	void wyswietlPotomkow(int lvl);
	void wyswietlProces(std::string nazwa);
	PCB* znajdzProces(std::string nazwa);
	PCB* dajRodzica();
	int dajId();
	void usunProces(std::string nazwa);
	void usunPotomka(std::string nazwa);
	void przeniesPotomkow(PCB* init, PCB* doPrzeniesienia);
	void ustawRodzica(PCB* _rodzic);
	int zliczProcesy();


	//dla ¯egalskiego (interpreter)
private:
	int status;
	int rej1, rej2, rej3, rej4;
	int licznikRozkazow;
public:
	void ustawStatus(int _status);
	int dajStatus();
	void ustawRej1(int wartosc);
	int dajRej1();
	void ustawRej2(int wartosc);
	int dajRej2();
	void ustawRej3(int wartosc);
	int dajRej3();
	void ustawRej4(int wartosc);
	int dajRej4();
	void ustawLicznikRozkazow(int wartosc);
	int dajLicznikRozkazow();


	//dla Cezarego (komunikacja miêdzyprocesowa)
private:
	std::vector<Socket*> listaSocketow;
	Socket* socket;
public:
	Socket*dajSocket();
	void ustawSocket(Socket*_socket);
	std::vector<Socket*>dajListeSocketow();
	void usunSocket(Socket*_socket);
	void dodajSocket(Socket*_socket);

	//dla Mariana (RAM)
public:
	int ramLokalizacja;
	int ramRozmiar;

	void ustawRamLokalizacja(int wartosc);
	int dajRamLokalizacja();
	void ustawRamRozmiar(int wartosc);
	int dajRamRozmiar();

	//dla Mariusza (semafory)


};

class ZarzadzanieProcesami
{
private:
	int idLicznik;
	PCB* init;
	int dajLicznik();

public:
	ZarzadzanieProcesami();
	~ZarzadzanieProcesami();

	void wyswietlIloscProcesow();
	void wyswietlWszystkieProcesy();
	void wyswietlProces(std::string nazwa);

	PCB* dodajProces(std::string nazwa, std::string rodzic);
	void usunProces(std::string nazwa);
};
