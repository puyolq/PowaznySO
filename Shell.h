#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include "ZarzadzanieProcesami.h"
#include "RAM.hpp"
#include "dysk.h"
#include "Interpreter.h"
#include "Wyjatki.h"
#include <algorithm>


class Shell {
public:
	Shell();

	//tworzenie procesu
	void CP(std::string nazwa, std::string rodzic, std::string program);
	//usuwanie procesu
	void DP(std::string nazwa);
	//wyswietlanie wszystkich procesow
	void PP();
	//wyswietl blok kontrolny procesu
	void BC(std::string nazwa);
	//interpreter wywolanie rozkazu procesu
	void GO();


	//zawartosc pamieci od adresu, gdzie rozmiar to ilosc komorek, np MC(0, 10) ma pokazac od 1 do 10
	void MC(int adres, int rozmiar);
	//cala zawartosc pamieci
	void MS();


	//tworzenie pliku
	void MF(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu);
	//usuwanie pliku
	void DF(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu);
	//zmiana nazwy pliku
	void RF(std::string nazwa, std::string rozszerzenie, std::string nowaNazwa, std::string nazwaFolderu);

	//zapis do pliku
	void SF(std::string nazwa, std::string rozszerzenie, std::string dane, PCB* proces, std::string nazwaFolderu);

	//pobranie danych z pliku
	void PF(std::string nazwa, std::string rozszerzenie, PCB* proces, std::string nazwaFolderu);

	//otwieranie stratnie
	void XF(std::string nazwa, std::string rozszerzenie, PCB* proces, std::string nazwaFolderu = "Dysk");

	//PF ale bez zamykania :)
	void OF(std::string nazwa, std::string rozszerzenie, PCB* proces, std::string nazwaFolderu);

	//zamkniecie pliku
	void CF(std::string nazwa, std::string rozszerzenie, PCB* proces, std::string nazwaFolderu);

	//tworzenie folderu
	void MD(std::string nazwa, std::string nazwaNadrzednego);
	//dodawanie pliku do katalogu
	void AD(std::string nazwaDocelowego, std::string nazwaPliku, std::string rozszerzenie, std::string nazwaFolderuZPlikiem);
	//usuwanie folderu
	void DD(std::string nazwa);
	//szukanie folderu
	void FD(std::string nazwa);

	//wypisanie drzewa plikow i katalogow na dysku
	void PT();
	//wypisanie tablicy dyskowej
	void PD();
	//wypisanie wektora bitowego
	void PV();
	//wypisanie tablicy iwezloe
	void PN();
	//wypisanie tablicy wpisow
	void PX();

	//RQ kolejka gotowych
	void RQ();
	//WQ kolejka oczekujacych
	void WQ();

	void exit();
	void czytajWejscie(std::string komenda);
	std::ifstream skrypt;
	void ladujSkrypt(std::string nazwa);
	bool wlaczone = false;

	std::string random_string(size_t length);
	struct PlikProces {
		std::string nazwaProcesu;
		std::string nazwaPliku;
		std::string rozszerzeniePliku;
	};
	std::vector<PlikProces> PlikiProcesy;
};