#pragma once
#include <array>
#include "iwezel.h"
#include "katalog.h"
#include "wpisKatalogowy.h"
#include "Semafory.h"
#include "ZarzadzanieProcesami.h"
class dysk 
{
public:
	dysk ();
	void zamknijPlik(std::string nazwa, std::string rozszerzenie, std::string dane, PCB* proces, std::string nazwaFolderu = "Dysk"); // Odblokuje dostep do pliku podnocz¹c semafor 

#pragma region obsluga plikow
	void utworzPlik(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu = "Dysk"); // Tworzy pusty plik i zajmuje jeden blok dyskowy.
	void usunPlik(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu = "Dysk"); // Zwalnia bloki, czyœci wpis na dysku
	void zmienNazwePliku(std::string nazwa, std::string rozszerzenie, std::string nowaNazwa, std::string nazwaFolderu = "Dysk"); // Zmiania nazwê pliku
	#pragma region po tych metodach wywo³aj zaknij plik
		void zapiszDoPliku(std::string nazwa, std::string rozszerzenie,std::string dane, PCB* proces, std::string nazwaFolderu ="Dysk"); //Pozwala na dopisywanie danych na koñcu pliku.
		std::string pobierzDane(std::string nazwa, std::string rozszerzenie, PCB* proces, std::string nazwaFolderu = "Dysk"); // Zwraca dane z pliku.
		void otowrzStratnie(std::string nazwa, std::string rozszerzenie, PCB* proces, std::string nazwaFolderu = "Dysk"); // Czyœwi plik, zwalnia bloki poza pierwszym blokiem.
	#pragma endregion po tych metodach wywo³aj zaknij plik
	
#pragma  endregion obsluga plikow

#pragma region obsluga folderow
	void utworzFolder(std::string nazwa, std::string nazwaNadrzednego = "Dysk"); // Tworzy folder podrzêdny w zadanym folderze 
	void dodajPlikDoKatalogu(std::string nazwaDolcelowego, std::string nazwaPliku, std::string rozszerzenie, std::string nazwaFolderuZPlikiem = "Dysk"); //Do skoñczenie
	void usunFolder(int pozycja); // Usuwa folder o zadanej pozycji nale¿y wywol³ywaæ w po³¹czniu z znajdzFolder, prawid³owy przyk³ad: dysk.usunFolder(dysk.znajdzFolder("Folder"));
	short znajdzFolder(std::string nazwa);
#pragma endregion obsluga folderow

#pragma region wpypisywanie elementow dyskowych
	void wypiszDrzewo(); // Wypisuje "drzewo" plików i katalogów na dysku.
	void wypiszDysk(); // Wypisuje na ekran sam¹ tablicê dyskow¹.
	void wypiszWektorBitowy();
	void wypiszTabliceIwezelow();
	void wypiszTabliceWpisow();
#pragma endregion  wpypisywanie elementow dyskowych
	
private:
#pragma region dysk
	std::array<iWezel, 32> tablicaIwezlow; // Tablica w której przechowywane s¹ iWezel
	std::array<char, 1024> tablicaDysk; // Tablica na dane
	std::array<katalog, 1024> tablicaKatalogow; // Tablica z danymi katalogów
	std::array<wpis, 32> tablicaWpisow; // Tablica z wpisami katalogowymi plikow
	std::array<bool, 32> wektorBitowy; // 1 Blok wolny, 0 Blok zajêty
	std::array<Semafory, 32> tablicaSemaforow; // Ka¿demu iWêz³owi odpowiada 1 semafor o pozycji tego iWêz³a;
#pragma endregion dysk

#pragma region zmienne pomocnicze
	short wielkoscBloku;
	int wielkoscDysku;
	short wolneBloki;
	int ileFolderow;
#pragma endregion zmienne pomocnicze

#pragma region metody pomocnicze
	void ileWolnych();
	short znajdzPlik(std::string nazwa, std::string rozszerzenie); // Metoda zwraca numer iWez³a w kolekcji w którym znajduje siê plik, jeœli pliku nie odnazleziono zwraca -1
	short znajdzWolnyBlok(); // Metoda zwraca numer pierwszego wolnego bloku na dysku, jeœli brak zwraca -1
	short znajdzIwezel(); // Metoda zwraca numer pierwszego wolnego iWêz³a, jeœli brak zwraca -1
	std::string pobierzNazweFolder(short poz);
#pragma endregion metody pomocnicze


	void otworzPlik(std::string nazwa, std::string rozszerzenie, std::string dane, PCB* proces, std::string nazwaFolderu = "Dysk"); // Blokuje dostêp do pliku opuczaj¹c semafor
};

