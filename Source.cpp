#include "Kolejka procesow.hpp"
#include "ZarzadzanieProcesami.h"
#include "dysk.h"
#include <iostream>

int main()
{
	ZarzadzanieProcesami zarzadzanie;
	Dysk dysk;
	dysk.utworzFolder("Dokumenty"); dysk.wypiszDrzewo();
	std::cout << "----------------------------" << std::endl;
	dysk.utworzPlik("cv", "pdf"); dysk.wypiszDrzewo();
	std::cout << "----------------------------" << std::endl;
	dysk.dodajPlikDoKatalogu("Dokumenty", "cv", "pdf");dysk.wypiszDrzewo();
	std::cout << "----------------------------" << std::endl;
	/*
	zarzadzanie.dodajProces("nazwa", "init");
	zarzadzanie.dodajProces("nazwa2", "init");
	zarzadzanie.dodajProces("nazwa3", "init");
	PCB*local=zarzadzanie.dodajProces("nazwa4", "init");
	local->ustawStatus(3);
	local->ustawStatus(2);
	zarzadzanie.wyswietlProces("nazwa4");
	zarzadzanie.dodajProces("nazwa6", "init");
	zarzadzanie.dodajProces("nazwa7", "init");
	zarzadzanie.dodajProces("nazwa8", "init");
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	*/
	return 0;
}
