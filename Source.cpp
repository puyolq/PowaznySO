#include "Kolejka procesow.hpp"
#include "ZarzadzanieProcesami.h"
#include <iostream>

int main()
{
	ZarzadzanieProcesami zarzadzanie;
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
	return 0;
}
