#include "Kolejka procesow.hpp"
#include "ZarzadzanieProcesami.h"
#include <iostream>
int main()
{
	ZarzadzanieProcesami zarzadzanie;
	zarzadzanie.dodajProces("nazwa", "init");
	zarzadzanie.dodajProces("nazwa2", "init");
	zarzadzanie.dodajProces("nazwa3", "init");
	zarzadzanie.dodajProces("nazwa4", "init");
	zarzadzanie.usunProces("nazwa4");
	zarzadzanie.dodajProces("nazwa6", "init");
	zarzadzanie.dodajProces("nazwa7", "init");
	zarzadzanie.dodajProces("nazwa8", "init");
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	return 0;
}
