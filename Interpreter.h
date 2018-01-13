#pragma once
#include"RAM.hpp"
#include"Kolejka procesow.hpp"
#include"ZarzadzanieProcesami.h"
#include"dysk.h"
#include"Komunikacja.h"

class Interpreter {
public:
	int A, B, C, D;
	int LicznikRozkazow;
	int StanProcesu;
	int LokalizacjaProgramu;
	int NumerRozkazu;
	int MiejsceNaDane;
	std::string Rozkaz; //pojedynczy rozkaz asemblerowy wraz z argumentami
	std::string Program; //program txt przypisany do danego procesy
public:
	// Konstruktor domyslnie wypelniajacy zmienne zerami
	Interpreter();
	//Destruktor
	~Interpreter();
	//Wpisywanie programu z TXT do RAM          
	void WpiszDoRam(PCB *pcb, int PamNaDane=0);
	//Wypisywanie stanow rejestrow aktualnie zapisanych w interpreterze 
	void StanRejestrow();
	//metoda do pobierania pojedynczego rozkazu z ramu
	void PobierzRozkaz(PCB *pcb);
	//Pobieranie Rejestrow od pcb i ustawianie w interpreterze
	void PobierzRejestry();
	//Kopiowanie rejestrow z interpretera do PCB
	void UstawRejestr();
	//blad przy operacjach na dysku
	void obsluzBledy(short blad);
	// Sprawdza czy string jest liczba
	bool CzyStringLiczba(std::string& s);
	//sprawdzanie poprawnosci komend asemblerowych
	bool Interpreter::KOMENDA();
	//Metoda wykonuj¹ca 1 rozkaz asemblerowy
	void WykonywanieProgramu();
};
extern Interpreter interpreter;