#include "dysk.h"
#include "Wyjatki.h"
#include <iostream>
//Rzucane wyjątki:
//1. brakMiescja
//2. niejednoznacznaNazwa
//3. blednaNazwaPliku



Dysk::Dysk()
{
	wolneBloki = 32;
	wielkoscBloku = 32;
	wielkoscDysku = 1024;
	ileFolderow = 0;
	for(int i=0; i<this->wektorBitowy.size(); i++)
	{
		this->wektorBitowy[i] = true;
	}

	for (int i = 0; i<this->tablicaDysk.size(); i++)
	{
		this->tablicaDysk[i] = '0';
	}
	katalog katalog("Dysk");
	
	tablicaKatalogow[ileFolderow]=(katalog);
	++ileFolderow;

}

void Dysk::otworzPlik(std::string nazwa, std::string rozszerzenie, std::string dane, PCB* proces, std::string nazwaFolderu)
{
	short pozycja = znajdzPlik(nazwa, rozszerzenie);
	short pozycjaFolderu = znajdzFolder(nazwaFolderu);

	if (pozycja == -1)
	{
		throw blednaNazwaPliku();
	}
	if (pozycjaFolderu == -1)
	{
		throw blednaNazwaFolderu();
	}
	tablicaSemaforow[pozycja].czekaj(proces);
}

void Dysk::zamknijPlik(std::string nazwa, std::string rozszerzenie, std::string dane, PCB * proces, std::string nazwaFolderu)
{
	short pozycja = znajdzPlik(nazwa, rozszerzenie);
	short pozycjaFolderu = znajdzFolder(nazwaFolderu);

	if (pozycja == -1)
	{
		throw blednaNazwaPliku();
	}
	if (pozycjaFolderu == -1)
	{
		throw blednaNazwaFolderu();
	}
	tablicaSemaforow[pozycja].rusz(proces);
}


void Dysk::utworzPlik(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu)
{
	// Sprawdz czy nie ma już takiego pliku
	// Znajdz miesjce na dysku
	// Znajdz iWęzeł
	short pozycjaPliku  = znajdzPlik(nazwa, rozszerzenie);
	/*if (pozycjaPliku != -1) {
		std::cout << tablicaWpisow[pozycjaPliku].pobierzNazwe() << "\n";
		std::cout << tablicaWpisow[pozycjaPliku].pobierzRozszerzenie();
	}*/
	short pozycjaFolderu = znajdzFolder(nazwaFolderu);
	bool plikJest = false;
	std::vector<short> pl = tablicaKatalogow[pozycjaFolderu].pobierzNumery();
	for (int i=0; i<pl.size(); i++)
	{
			if(tablicaWpisow[pl[i]].pobierzNazwe()==nazwa && tablicaWpisow[pl[i]].pobierzRozszerzenie() == rozszerzenie)
			{
				plikJest = true;
				break;
			}
	}
	if(plikJest)
	{
		throw niejednoznacznaNazwa();
	}
	if(pozycjaFolderu==-1)
	{
		throw blednaNazwaFolderu();
	}
	if(pozycjaPliku ==-1 && pozycjaFolderu!=-1) // Pliku nie znaleziono, nazwa jest jednoznaczna
	{
		if(this->wolneBloki!=0)
		{
			short blok = znajdzWolnyBlok();
			short iWezel = znajdzIwezel();
			this->tablicaKatalogow[pozycjaFolderu].dodajNumerIwezela(iWezel);
			wektorBitowy[blok] = false; // Zaznacznie wykorzystania bloku dyskowego
			tablicaIwezlow[iWezel].ustawPierwszyBlok(blok);
			tablicaIwezlow[iWezel].ustawRozmiarPliku(0);
			tablicaWpisow[iWezel].ustawNazwe(nazwa);
			tablicaWpisow[iWezel].ustawNumerIwezla(iWezel);
			tablicaWpisow[iWezel].ustawRozszrzenie(rozszerzenie);
			--wolneBloki;
		}
		else { throw brakMiejsca(); }
	}
	

}

void Dysk::zapiszDoPliku(std::string nazwa, std::string rozszerzenie, std::string dane, PCB* proces, std::string nazwaFolderu)
{
	short pozycja = znajdzPlik(nazwa, rozszerzenie);
	short pozycjaFolderu = znajdzFolder(nazwaFolderu);

	if(tablicaSemaforow[pozycja].dlugosc()<0)
	{
		throw brakDostepuDoPliku();
	}
	tablicaSemaforow[pozycja].czekaj(proces);
	if (pozycja == -1)
	{
		throw blednaNazwaPliku();
	}
	if(pozycjaFolderu ==-1)
	{
		throw blednaNazwaFolderu();
	}

	if(pozycja!=-1 && pozycjaFolderu!=-1)
	{
		for(int i=0; i<dane.length(); i++)
		{
			if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < 32)
			{
				tablicaDysk[tablicaIwezlow[pozycja].pobierzPierwszyBlok()*wielkoscBloku + tablicaIwezlow[pozycja].pobierzRozmiarPliku()] = dane[i];
				tablicaIwezlow[pozycja].ustawRozmiarPliku(tablicaIwezlow[pozycja].pobierzRozmiarPliku()+1);
			}

			if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() >= 32 && tablicaIwezlow[pozycja].pobierzRozmiarPliku() < 64)
			{
				if(tablicaIwezlow[pozycja].pobierzDrugiBlok()==-1) // Jeśli drugi blok nie jest zarezerwowany, zarezerwuj go.
				{
					short blok = znajdzWolnyBlok();
					this->wektorBitowy[blok] = false; // Zaznacznie wykorzystania bloku dyskowego
					tablicaIwezlow[pozycja].ustawDrugiBlok(blok);
					i++;
					--wolneBloki;
				}
				tablicaDysk[tablicaIwezlow[pozycja].pobierzDrugiBlok()*wielkoscBloku + tablicaIwezlow[pozycja].pobierzRozmiarPliku()%wielkoscBloku] = dane[i];
				tablicaIwezlow[pozycja].ustawRozmiarPliku(tablicaIwezlow[pozycja].pobierzRozmiarPliku() + 1);
			}
			if(tablicaIwezlow[pozycja].pobierzRozmiarPliku() >= 64)
			{
				int pozycjaZapisu=0;
				if(tablicaIwezlow[pozycja].pobierzIndeksowyBlok()==-1) // Rezerwacja bloku indeksowego.
				{
					--wolneBloki;
					short blok = znajdzWolnyBlok();
					this->wektorBitowy[blok] = false; // Zaznacznie wykorzystania bloku dyskowego.
					tablicaIwezlow[pozycja].ustawIndeksowyBlok(blok);
					for(int k = blok*wielkoscBloku; k<blok*wielkoscBloku+32; k++)
					{
						tablicaDysk[k] = -1;
					}
					i++;
				}
				if(tablicaIwezlow[pozycja].pobierzRozmiarPliku()%32==0) // Szukanie kolejnego bloku na dane.
				{
					for(int k=tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku; k<tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku+32; k++)
					{
						if(tablicaDysk[k]==-1)
						{
							short blok = znajdzWolnyBlok();
							this->wektorBitowy[blok] = false; // Zaznacznie wykorzystania bloku dyskowego.
							tablicaDysk[k] = blok;
							--wolneBloki;
							break;
						}
					}
				}
				
				for (int k = tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku; k<tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku + 31; k++)
				{
					if (tablicaDysk[k] != -1 && tablicaDysk[k+1] == -1)
					{
						pozycjaZapisu = tablicaDysk[k];
					}
				}

				tablicaDysk[pozycjaZapisu*wielkoscBloku + tablicaIwezlow[pozycja].pobierzRozmiarPliku() % wielkoscBloku] = dane[i];
				tablicaIwezlow[pozycja].ustawRozmiarPliku(tablicaIwezlow[pozycja].pobierzRozmiarPliku() + 1);
			}
		}
	}
}

std::string Dysk::pobierzDane(std::string nazwa, std::string rozszerzenie, PCB* proces, std::string nazwaFolderu)
{
	std::string doZwrotu = "";
	short ileBlokowOczytano = 0;
	short pozycja = znajdzPlik(nazwa, rozszerzenie);
	int pobraneDane = tablicaIwezlow[pozycja].pobierzRozmiarPliku();
	short ileDanychPierwszyBlok;
	short pozycjaFolderu = znajdzFolder(nazwaFolderu);

	if (tablicaSemaforow[pozycja].dlugosc()<0)
	{
		throw brakDostepuDoPliku();
	}
	tablicaSemaforow[pozycja].czekaj(proces);
	if (pozycja == -1)
	{
		throw blednaNazwaPliku();
	}
	if (pozycjaFolderu == -1)
	{
		throw blednaNazwaFolderu();
	}

	if(pozycja!=-1 && pozycjaFolderu !=-1)
	{
		if (tablicaIwezlow[pozycja].pobierzPierwszyBlok() != -1)
		{
			
			if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < 33)
			{
				ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku();
			}
			else { ileDanychPierwszyBlok = 32; }

			for (int i = tablicaIwezlow[pozycja].pobierzPierwszyBlok()*wielkoscBloku; i < tablicaIwezlow[pozycja].pobierzPierwszyBlok()*wielkoscBloku + ileDanychPierwszyBlok; i++)
			{
				doZwrotu += tablicaDysk[i];
			}
			++ileBlokowOczytano;
		}

		if (tablicaIwezlow[pozycja].pobierzDrugiBlok() != -1)
		{
			if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < 64)
			{
				ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku()-32;
			}
			else { ileDanychPierwszyBlok = 32; }

			for (int i = tablicaIwezlow[pozycja].pobierzDrugiBlok()*wielkoscBloku; i < tablicaIwezlow[pozycja].pobierzDrugiBlok()*wielkoscBloku + ileDanychPierwszyBlok; i++)
			{
				doZwrotu += tablicaDysk[i];
			}
			++ileBlokowOczytano;
		}
		if(tablicaIwezlow[pozycja].pobierzIndeksowyBlok()!=-1)
		{
			std::vector<short> blokiPozostale;
			for(int i = tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku; i<tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku+31; i++)
			{
				if(tablicaDysk[i]!=-1)
				{
					blokiPozostale.push_back(tablicaDysk[i]);
				}
				else
				{
					break;
				}
			}
			
			for(auto it=0; it!=blokiPozostale.size(); it++)
			{
				++ileBlokowOczytano;
				if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < ileBlokowOczytano*wielkoscBloku)
				{
					ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku() - (ileBlokowOczytano-1)*wielkoscBloku;
				}
				else { ileDanychPierwszyBlok = 32; }

				for (int i = blokiPozostale[it] *wielkoscBloku; i < blokiPozostale[it]*wielkoscBloku + ileDanychPierwszyBlok; i++)
				{
					doZwrotu += tablicaDysk[i];
				}
			}

		}
	}
 
	return doZwrotu;
}

void Dysk::otworzStratnie(std::string nazwa, std::string rozszerzenie, PCB* proces, std::string nazwaFolderu)
{
	short ileBlokowOczytano = 0;
	short pozycja = znajdzPlik(nazwa, rozszerzenie);
	int pobraneDane = tablicaIwezlow[pozycja].pobierzRozmiarPliku();
	short ileDanychPierwszyBlok;
	short pozycjaFolderu = znajdzFolder(nazwaFolderu);

	if (tablicaSemaforow[pozycja].dlugosc()<0)
	{
		throw brakDostepuDoPliku();
	}
	tablicaSemaforow[pozycja].czekaj(proces);
	if (pozycja == -1)
	{
		throw blednaNazwaPliku();
	}
	if (pozycjaFolderu == -1)
	{
		throw blednaNazwaFolderu();
	}
	if (pozycja != -1 && pozycjaFolderu!=-1)
	{
		if (tablicaIwezlow[pozycja].pobierzPierwszyBlok() != -1)
		{
			if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < 33)
			{
				ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku();
			}
			else { ileDanychPierwszyBlok = 32; }

			for (int i = tablicaIwezlow[pozycja].pobierzPierwszyBlok()*wielkoscBloku; i < tablicaIwezlow[pozycja].pobierzPierwszyBlok()*wielkoscBloku + ileDanychPierwszyBlok; i++)
			{
				tablicaDysk[i] = '0';
			}
			++ileBlokowOczytano;
		}

		if (tablicaIwezlow[pozycja].pobierzDrugiBlok() != -1)
		{
			if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < 64)
			{
				ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku() - 32;
			}
			else { ileDanychPierwszyBlok = 32; }

			for (int i = tablicaIwezlow[pozycja].pobierzDrugiBlok()*wielkoscBloku; i < tablicaIwezlow[pozycja].pobierzDrugiBlok()*wielkoscBloku + ileDanychPierwszyBlok; i++)
			{
				tablicaDysk[i] = '0';
			}
			++ileBlokowOczytano;
			wektorBitowy[tablicaIwezlow[pozycja].pobierzDrugiBlok()] = true;
			++wolneBloki;
		}
		if (tablicaIwezlow[pozycja].pobierzIndeksowyBlok() != -1)
		{
			std::vector<short> blokiPozostale;
			for (int i = tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku; i<tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku + 31; i++)
			{
				if (tablicaDysk[i] != -1)
				{
					blokiPozostale.push_back(tablicaDysk[i]);
				}
				else
				{
					break;
				}
			}
			wektorBitowy[tablicaIwezlow[pozycja].pobierzIndeksowyBlok()] = true;
			++wolneBloki;
			for (auto it = 0; it != blokiPozostale.size(); it++)
			{
				++wolneBloki;
				++ileBlokowOczytano;
				wektorBitowy[blokiPozostale[it]] = true;
				if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < ileBlokowOczytano*wielkoscBloku)
				{
					ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku() - (ileBlokowOczytano - 1)*wielkoscBloku;
				}
				else { ileDanychPierwszyBlok = 32; }

				for (int i = blokiPozostale[it] * wielkoscBloku; i < blokiPozostale[it] * wielkoscBloku + ileDanychPierwszyBlok; i++)
				{
					tablicaDysk[i] = '0';
				}
			}
			for(auto i =tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*32; i<tablicaIwezlow[pozycja].pobierzIndeksowyBlok() * 32+32; i++)
			{
				tablicaDysk[i] = '0';
			}
		}
		tablicaIwezlow[pozycja].ustawDrugiBlok(-1);
		tablicaIwezlow[pozycja].ustawIndeksowyBlok(-1);
		tablicaIwezlow[pozycja].ustawRozmiarPliku(1);
	}
	 
}

void Dysk::usunPlik(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu)
{
	short pozycja = znajdzPlik(nazwa, rozszerzenie);
	short pozycjaFolderu = znajdzFolder(nazwaFolderu);
	if (pozycja == -1)
	{
		throw blednaNazwaPliku();
	}
	if (pozycjaFolderu == -1)
	{
		throw blednaNazwaFolderu();
	}
	if(pozycja!=-1 && pozycjaFolderu!=-1)
	{
#pragma  region potrzeba
		short ileBlokowOczytano = 0;
		short pozycja = znajdzPlik(nazwa, rozszerzenie);
		int pobraneDane = tablicaIwezlow[pozycja].pobierzRozmiarPliku();
		short ileDanychPierwszyBlok;
		short pozycjaFolderu = znajdzFolder(nazwaFolderu);
		if (pozycja == -1)
		{
			throw blednaNazwaPliku();
		}
		if (pozycjaFolderu == -1)
		{
			throw blednaNazwaFolderu();
		}
		if (pozycja != -1 && pozycjaFolderu != -1)
		{
			if (tablicaIwezlow[pozycja].pobierzPierwszyBlok() != -1)
			{
				if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < 33)
				{
					ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku();
				}
				else { ileDanychPierwszyBlok = 32; }

				for (int i = tablicaIwezlow[pozycja].pobierzPierwszyBlok()*wielkoscBloku; i < tablicaIwezlow[pozycja].pobierzPierwszyBlok()*wielkoscBloku + ileDanychPierwszyBlok; i++)
				{
					tablicaDysk[i] = '0';
				}
				++ileBlokowOczytano;
			}

			if (tablicaIwezlow[pozycja].pobierzDrugiBlok() != -1)
			{
				if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < 64)
				{
					ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku() - 32;
				}
				else { ileDanychPierwszyBlok = 32; }

				for (int i = tablicaIwezlow[pozycja].pobierzDrugiBlok()*wielkoscBloku; i < tablicaIwezlow[pozycja].pobierzDrugiBlok()*wielkoscBloku + ileDanychPierwszyBlok; i++)
				{
					tablicaDysk[i] = '0';
				}
				++ileBlokowOczytano;
				wektorBitowy[tablicaIwezlow[pozycja].pobierzDrugiBlok()] = true;
				++wolneBloki;
			}
			if (tablicaIwezlow[pozycja].pobierzIndeksowyBlok() != -1)
			{
				std::vector<short> blokiPozostale;
				for (int i = tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku; i<tablicaIwezlow[pozycja].pobierzIndeksowyBlok()*wielkoscBloku + 31; i++)
				{
					if (tablicaDysk[i] != -1)
					{
						blokiPozostale.push_back(tablicaDysk[i]);
					}
					else
					{
						break;
					}
				}
				wektorBitowy[tablicaIwezlow[pozycja].pobierzIndeksowyBlok()] = true;
				++wolneBloki;
				for (auto it = 0; it != blokiPozostale.size(); it++)
				{
					++wolneBloki;
					++ileBlokowOczytano;
					wektorBitowy[blokiPozostale[it]] = true;
					if (tablicaIwezlow[pozycja].pobierzRozmiarPliku() < ileBlokowOczytano*wielkoscBloku)
					{
						ileDanychPierwszyBlok = tablicaIwezlow[pozycja].pobierzRozmiarPliku() - (ileBlokowOczytano - 1)*wielkoscBloku;
					}
					else { ileDanychPierwszyBlok = 32; }

					for (int i = blokiPozostale[it] * wielkoscBloku; i < blokiPozostale[it] * wielkoscBloku + ileDanychPierwszyBlok; i++)
					{
						tablicaDysk[i] = '0';
					}
				}
				for (auto i = tablicaIwezlow[pozycja].pobierzIndeksowyBlok() * 32; i<tablicaIwezlow[pozycja].pobierzIndeksowyBlok() * 32 + 32; i++)
				{
					tablicaDysk[i] = '0';
				}
			}
			tablicaIwezlow[pozycja].ustawDrugiBlok(-1);
			tablicaIwezlow[pozycja].ustawIndeksowyBlok(-1);
			tablicaIwezlow[pozycja].ustawRozmiarPliku(1);
		}

#pragma endregion potrzeba
		tablicaDysk[tablicaIwezlow[pozycja].pobierzPierwszyBlok() * 32] = '0';
		wektorBitowy[tablicaIwezlow[pozycja].pobierzPierwszyBlok()] = true;
		tablicaIwezlow[pozycja].czysc();
		tablicaWpisow[pozycja].czysc();
		tablicaKatalogow[pozycjaFolderu].usunNumerIwezla(pozycja);
		++wolneBloki;
	}

}

void Dysk::zmienNazwePliku(std::string nazwa, std::string rozszerzenie, std::string nowaNazwa, std::string nazwaFolderu)
{
	short pozycja = znajdzPlik(nazwa, rozszerzenie);
	
	if(pozycja!=-1)
	{
		tablicaWpisow[pozycja].ustawNazwe(nowaNazwa);
	}
	else { throw blednaNazwaPliku(); }
}

void Dysk::utworzFolder(std::string nazwa, std::string nazwaNadrzednego)
{
	short pozycja = znajdzFolder(nazwaNadrzednego);
	short pozycjaNowego = znajdzFolder(nazwa);
	
	

	if (nazwa == nazwaNadrzednego)
	{
		throw blednaNazwaFolderu();
	}

	if (pozycja != -1 && pozycjaNowego==-1)
	{
		if (tablicaKatalogow[pozycja].ilePodfolderow() == 31)
		{
			throw brakWolnychSynow();
		}
		katalog katalog(nazwa, pozycja);
		tablicaKatalogow[ileFolderow] = katalog;
		++ileFolderow;
	}
	else { throw blednaNazwaFolderu(); }
	tablicaKatalogow[znajdzFolder(nazwa)].ustawOjca(znajdzFolder(nazwaNadrzednego));
	tablicaKatalogow[znajdzFolder(nazwaNadrzednego)].ustawSyna(znajdzFolder(nazwa));
	tablicaKatalogow[znajdzFolder(nazwaNadrzednego)].ustawPodfoldery(tablicaKatalogow[znajdzFolder(nazwaNadrzednego)].ilePodfolderow() + 1);
	
}

void Dysk::dodajPlikDoKatalogu(std::string nazwaDolcelowego, std::string nazwaPliku, std::string rozszerzenie, std::string nazwaFolderuZPlikiem)
{
	short pozycjaKataloguZPlikiem = znajdzFolder(nazwaFolderuZPlikiem);
	short pozycjaKataloguDocelowego = znajdzFolder(nazwaDolcelowego);
	short pozycjaPliku = znajdzPlik(nazwaPliku, rozszerzenie);
	if(pozycjaKataloguZPlikiem==-1 || pozycjaKataloguDocelowego == -1)
	{
		throw blednaNazwaFolderu();
	}
	if(pozycjaPliku==-1)
	{
		throw blednaNazwaPliku();
	}
	if(pozycjaKataloguZPlikiem !=-1 && pozycjaPliku!=-1)
	{
		tablicaKatalogow[pozycjaKataloguDocelowego].dodajNumerIwezela(znajdzPlik(nazwaPliku, rozszerzenie));
		tablicaKatalogow[pozycjaKataloguZPlikiem].usunNumerIwezla(znajdzPlik(nazwaPliku, rozszerzenie));
	}
}

void Dysk::usunFolder(int pozycja)
{
	if (znajdzFolder(this->pobierzNazweFolder(pozycja)) != -1) {
		this->tablicaKatalogow[tablicaKatalogow[pozycja].pobierzOjca()].usunSyna(pozycja);
		std::array<short, 32> podfoldery = tablicaKatalogow[pozycja].pobierzSynow();
		std::vector<short> iWezly = tablicaKatalogow[pozycja].pobierzNumery();
		int pozycjaOjca = tablicaKatalogow[pozycja].pobierzOjca();
		for (int i = 0; i < iWezly.size(); i++)
		{
			usunPlik(tablicaWpisow[iWezly[i]].pobierzNazwe(), tablicaWpisow[iWezly[i]].pobierzRozszerzenie(), pobierzNazweFolder(pozycja));
		}
		katalog kat;
		tablicaKatalogow[pozycja] = kat;
		for (int i = 0; i < podfoldery.size(); i++)
		{
			if (podfoldery[i] != -1) {
				usunFolder(podfoldery[i]);
			}
		}
		--ileFolderow;
		tablicaKatalogow[pozycjaOjca].ustawPodfoldery(tablicaKatalogow[pozycjaOjca].ilePodfolderow() - 1);
	}
	else { throw blednaNazwaFolderu(); }
}

void Dysk::wypiszDrzewo()
{
	for(int i=0; i<ileFolderow; i++)
	{
		std::cout << "Nazwa: " << tablicaKatalogow[i].pobierzNazwe() <<"\n";

		std::cout << "Zawarte pliki: ";
		std::vector<short> tab = tablicaKatalogow[i].pobierzNumery();
		if(tab.size()==0)
		{
			std::cout << "Brak";
		}
		for(int i=0; i<tab.size(); i++)
		{
			std::cout << tablicaWpisow[tab[i]].pobierzNazwe() << "." << tablicaWpisow[tab[i]].pobierzRozszerzenie() << " ";
		}
		std::cout << "\n";
		int k = tablicaKatalogow[i].pobierzOjca();
		std::cout << "Folder nadrzedny: "; if (tablicaKatalogow[i].pobierzOjca() == -1) { std::cout << "Brak\n"; }
		else { std::cout<<tablicaKatalogow[tablicaKatalogow[i].pobierzOjca()].pobierzNazwe()<<"\n"; };
		std::cout << "Podfoldery: ";
		bool podfoldery = true;
		std::array<short, 32> tabl = tablicaKatalogow[i].pobierzSynow();
		for (int i = 0; i<tabl.size(); i++)
		{
			if (tabl[i] != -1) {
				std::cout << tablicaKatalogow[tabl[i]].pobierzNazwe() << " ";
				podfoldery = false;
			}
		}
		if(podfoldery)
		{
			std::cout << "Brak";
		}
		std::cout << "\n\n";
	}
}

void Dysk::wypiszDysk()
{
	std::vector<short> blokiIndeksowe;

	for(int i=0; i<tablicaIwezlow.size(); i++)
	{
		if(tablicaIwezlow[i].pobierzIndeksowyBlok()!=-1)
		{
			blokiIndeksowe.push_back(tablicaIwezlow[i].pobierzIndeksowyBlok());
		}
	}

	for(int i=0; i<32; i++)
	{
		std::cout << i << " ";
		
		for (int k = 0; k<32; k++)
		{
			if(std::find(blokiIndeksowe.begin(), blokiIndeksowe.end(), i)!= blokiIndeksowe.end())
			{
				std::cout << static_cast<short>(tablicaDysk[(i*wielkoscBloku) + k]) <<" ";
			}
			else {
				std::cout << tablicaDysk[(i*wielkoscBloku) + k];
			}
		}
		std::cout << "\n";
	}
}

void Dysk::wypiszWektorBitowy()
{
	std::cout << "Wektor Bitowy: (1 blok wolny, 0 zajety)\n";
	for (int i=0; i<wektorBitowy.size(); i++)
	{
		std::cout << i << " " << wektorBitowy[i] << "\n";
	}	
}

void Dysk::wypiszTabliceIwezelow()
{
	std::cout << "Tablica iWezelow:\n";
	for (iWezel tablica_iwezlow : tablicaIwezlow)
	{
		tablica_iwezlow.wypiszIwezel();
		std::cout << "\n";
	}
}

void Dysk::ileWolnych()
{
	std::cout << "\n\nWolnych blokow; " << wolneBloki;
}

void Dysk::wypiszTabliceWpisow()
{
	for (int i=0; i<tablicaWpisow.size(); i++)
	{
		tablicaWpisow[i].wypisz();
	}
}

short Dysk::znajdzPlik(std::string nazwa, std::string rozszerzenie)
{
	short doZwrotu = -1;
	for(short i = 0; i<this->tablicaWpisow.size(); i++)
	{
		if(this->tablicaWpisow[i].pobierzNazwe()==nazwa && this->tablicaWpisow[i].pobierzRozszerzenie()==rozszerzenie)
		{
			doZwrotu = tablicaWpisow[i].pobierzNumer();
			break;
		}
	}
	
	return doZwrotu;
}

short Dysk::znajdzWolnyBlok()
{
	short doZwrotu= -1;
	for(int i=0; i<this->wektorBitowy.size(); i++)
	{
		if(this->wektorBitowy[i]==true)
		{
			doZwrotu = i;
			break;
		}
	}
	if (doZwrotu == -1)
	{
		throw brakMiejsca();
	}
	return doZwrotu;
}

short Dysk::znajdzIwezel()
{
	short doZwrotu = -1;
	for (short i = 0; i < this->tablicaIwezlow.size(); i++)
	{
		if (this->tablicaIwezlow[i].pobierzPierwszyBlok() == -1)
		{
			doZwrotu = i;
			break;
		}
	}
	return doZwrotu;
}

std::string Dysk::pobierzNazweFolder(short poz)
{
	return tablicaKatalogow[poz].pobierzNazwe();
}

short Dysk::znajdzFolder(std::string nazwa)
{
	short doZwrotu = -1;
	for(int i=0; i<tablicaKatalogow.size(); i++)
	{
		if(tablicaKatalogow[i].pobierzNazwe()==nazwa)
		{
			doZwrotu = i;
			break;
		}
	}
	return doZwrotu;
}
