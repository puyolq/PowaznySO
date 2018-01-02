#pragma once
#include <string>
#include <vector>
#include <array>


class katalog 
{
public:
	katalog ();
	katalog(std::string nazwa);
	katalog(std::string nazwa, short wskaznikNaOjca);

	void wypiszKatalog();

#pragma region operacje na zmiennych prywatnych
	void ustawNazwe(std::string nazwa); 
	std::string pobierzNazwe();

	void dodajNumerIwezela(short numer); // Dodaje przekazany numer iWezela do kolekcji numeryIwezlow
	void usunNumerIwezla(short numer); // Usuwa przekazany numer z kolejcji numeryIwezlow
	std::vector<short> pobierzNumery(); // Zwraca kolekcje numeryIwezlow

	void ustawOjca(short ojciec);
	short pobierzOjca();

	void ustawSyna(short syn);
	std::array<short, 32> pobierzSynow(); // Zwraca kolekcjê podfolderów
	void usunSyna(short syn);

	short ilePodfolderow();
	void ustawPodfoldery(int i);
#pragma endregion operacje na zmiennych prywatnych

private:
	std::string nazwa;
	std::vector<short> numeryIwezlow; // Przechowywanie numerów iWêz³ów nale¿¹cych do katalogu
	short wskaznikNaOjca;
	std::array<short, 32> wskaznikNaSyna;
	short podfoldery;
};

