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
	std::string Rozkaz; //pojedynczy rozkaz asemblerowy wraz z argumentami
public:
	// Konstruktor domyslnie wypelniajacy zmienne zerami
	Interpreter();
	//Destruktor
	~Interpreter();
	//Wpisywanie programu z TXT do RAM          
	void WpiszDoRam(PCB *pcb, std::string program);
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
	//Metoda wykonuj¹ca 1 rozkaz asemblerowy
	void WykonywanieProgramu();
};
extern Interpreter interpreter;