#pragma once
#include "ZarzadzanieProcesami.h"

struct Wezel
{
	PCB* proces;
	Wezel* nastepny;
	Wezel(PCB* proces);
	Wezel();
};

class KolejkaProcesow
{
public:
	std::string nazwa;
	Wezel *glowa, *ogon;	//glowa to ten aktywny process
	KolejkaProcesow(std::string);
	~KolejkaProcesow();

	//Zarz¹dzanie procesami
	void dodajDoKolejki(PCB*);

	//Interpreter
	void uruchomEkspedytor(const bool&);	//sprawdza czy proces siê wykona³, jeœli tak to usuwa go z kolejki i na jego miejsce wchodzi kolejny,
											//jeœli nie to proces l¹duje na koñcu kolejki
	//Shell
	void usunProces(const short&);	//szuka proces po PID i usuwa go z kolejki
	void wyswietlKolejke();			//do zmiany - musi wyœwietlaæ ³adniej procesy np z wykorzystaniem funkcji Zarz¹dzaniaProcesami

	//INNE
	short rozmiarKolejki();			//zwraca liczbê procesów w kolejce
	void sprawdzBezczynnosc();		//sprawdza czy w kolejce znajduje siê jakiœ proces, jeœli nie to ustawia proces bezczynnoœci
	void usunZPoczatku();
	void usunZKonca();
};
//DLA TYCH CO USYPIAJ¥ I BUDZ¥
void zmienKolejke(PCB*);	//jeœli proces jest na CZELE kolejki gotowych to wstawia go na koniec oczekuj¹cych i odwrotnie
void zmienKolejke(Wezel*);
void uspijProces(PCB*);		//usuwa proces z DOWOLNEGO MIEJSCA kolejki gotowych jeœli tam jest i wstawia go na koniec oczekuj¹cych
void uspijProces(Wezel*);
void obudzProces(PCB*);		//usuwa proces z DOWOLNEGO MIEJSCA kolejki oczekuj¹cych jeœli tam jest i wstawia go na koniec gotowych
void obudzProces(Wezel*);

//tymczasowy proces bezczynnosci

//globalne kolejki procesow
extern KolejkaProcesow kolejkaGotowych;
extern KolejkaProcesow kolejkaOczekujacych;


