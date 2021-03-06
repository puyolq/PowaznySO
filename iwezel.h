#pragma once

class iWezel
{
public:
	iWezel();	
	void wypiszIwezel(); 
	
#pragma region operacje na zmiennych prywatnych
	void czysc(); // Zwalnianie struktury

	void ustawRozmiarPliku(short rozmiar);
	int pobierzRozmiarPliku();

	void ustawPierwszyBlok(short blok);
	short pobierzPierwszyBlok();

	void ustawDrugiBlok(short blok);
	short pobierzDrugiBlok();

	void ustawIndeksowyBlok(short blok);
	short pobierzIndeksowyBlok();
#pragma endregion operacje na zmiennych prywatnych

private:
	int rozmiarPliku;
	short pierwszyBlok;
	short drugiBlok;
	short indeksowyBlok;
};


