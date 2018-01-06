#include "Kolejka procesow.hpp"
#include "ZarzadzanieProcesami.h"
#include "dysk.h"
#include <iostream>

int main()
{
	ZarzadzanieProcesami* zarzadzanie = new ZarzadzanieProcesami();
	std::vector<std::string> wynik;
	dysk dysk;
	dysk.utworzPlik("Plik", "txt");
	wynik = dysk.bledy();
	for (auto it = wynik.begin(); it != wynik.end(); it++) {
		std::cout << *it << "\n";
	}

	dysk.utworzFolder("Folder");
	wynik = dysk.bledy();
	for (auto it = wynik.begin(); it != wynik.end(); it++) {
		std::cout << *it << "\n";
	}

	dysk.dodajPlikDoKatalogu("Folder", "Plik", "txt", "as");
	wynik = dysk.bledy();
	for (auto it = wynik.begin(); it != wynik.end(); it++) {
		std::cout << *it << "\n";
	}
	
	dysk.wypiszDysk();
	dysk.wypiszDrzewo();
	bool k = dysk.pobierzBlednaNazwaFolderu();
	return 0;
}
