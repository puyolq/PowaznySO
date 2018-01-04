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
	
	zarzadzanie.dodajProces("nazwa", "init");
	zarzadzanie.dodajProces("nazwa2", "init");
	zarzadzanie.dodajProces("nazwa3", "init");
	PCB*local=zarzadzanie.dodajProces("nazwa4", "init");
	local->ustawStatus(3);
	local->ustawStatus(2);
	std::cout << "KOLEJKA OCZEKUJACYCH" << std::endl;
	kolejkaOczekujacych.wyswietlKolejke();
	
	zarzadzanie.dodajProces("nazwa5", "init");
	std::cout << "Normalna kolejka bez 4" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(false);
	std::cout << "1Cykl" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(true);
	std::cout << "2Usun poczatek" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(true);
	std::cout << "3Usun poczatek" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(false);
	std::cout << "4Cykl" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(false);
	std::cout << "5Cykl" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(true);
	std::cout << "6Usun poczatek" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(false);
	std::cout << "7Cykl" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(false);
	std::cout << "8Cykl" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(true);
	std::cout << "9Usun poczatek" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(true);
	local->ustawStatus(1);
	std::cout << "10Usun poczatek, obudz 4" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();

	kolejkaGotowych.uruchomEkspedytor(true);
	std::cout << "11Usun poczatek" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(false);
	std::cout << "12Cykl" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	zarzadzanie.dodajProces("nazwa1", "init");
	zarzadzanie.dodajProces("nazwa2", "init");
	std::cout << "13Dwa procesy dodane" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(false);
	std::cout << "14Cykl" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(true);
	std::cout << "15Usun poczatek" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(true);
	std::cout << "16Usun poczatek" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(true);
	std::cout << "17Usun poczatek" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();
	kolejkaGotowych.uruchomEkspedytor(false);
	std::cout << "18Cykl" << std::endl;
	kolejkaGotowych.wyswietlKolejke();
	zarzadzanie.wyswietlWszystkieProcesy();


	return 0;
}
