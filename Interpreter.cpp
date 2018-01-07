#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

#include"Interpreter.h"


Interpreter interpreter;

//Konstruktor
Interpreter::Interpreter()
{
	A = 0;
	B = 0;
	C = 0;
	D = 0;
	NumerRozkazu = 0;
	LicznikRozkazow = 0;
	StanProcesu = 0;
	LokalizacjaProgramu = -1;
	Rozkaz = "";
}

//Destruktor
Interpreter::~Interpreter() {};

//Wpisywanie programu z TXT do RAM          
void Interpreter::WpiszDoRam(PCB *pcb, std::string program) {
	std::string pom;
	program = program + ".txt";
	std::fstream plik;
	plik.open(program, std::ios::in);
	if (plik.good() == true) {
		getline(plik, pom);
	}
	else {

	}
	ram.addToMem(pcb, pom);
}

//Wypisywanie stanow rejestrow aktualnie zapisanych w interpreterze 
void Interpreter::StanRejestrow() {
	std::cout << "A: " << this->A << std::endl;
	std::cout << "B: " << this->B << std::endl;
	std::cout << "C: " << this->C << std::endl;
	std::cout << "D: " << this->D << std::endl;
	std::cout << "Licz: " << this->LicznikRozkazow << std::endl;
	std::cout << "Stan: " << this->StanProcesu << std::endl;
}

//metoda do pobierania pojedynczego rozkazu z ramu
void Interpreter::PobierzRozkaz(PCB *pcb) {
	if (pcb->ramLokalizacja != -1) {
		Rozkaz.clear();
		LicznikRozkazow = kolejkaGotowych.glowa->proces->dajLicznikRozkazow();
		LokalizacjaProgramu = kolejkaGotowych.glowa->proces->dajRamLokalizacja();
		int lokalizacja = pcb->ramLokalizacja + LicznikRozkazow;
		LokalizacjaProgramu = pcb->ramLokalizacja;
		// metoda mariana ktora odsyla std::stringa 
		Rozkaz = ram.showProcess(lokalizacja);
		LicznikRozkazow += Rozkaz.size();
		LicznikRozkazow += 1;
		pcb->ustawLicznikRozkazow(LicznikRozkazow);
	}
}

//Pobieranie Rejestrow od pcb i ustawianie w interpreterze
void Interpreter::PobierzRejestry() {
	A = kolejkaGotowych.glowa->proces->dajRej1();
	B = kolejkaGotowych.glowa->proces->dajRej2();
	C = kolejkaGotowych.glowa->proces->dajRej3();
	D = kolejkaGotowych.glowa->proces->dajRej4();
	LicznikRozkazow = kolejkaGotowych.glowa->proces->dajLicznikRozkazow();
	LokalizacjaProgramu = kolejkaGotowych.glowa->proces->dajRamLokalizacja();
}

//Kopiowanie rejestrow z interpretera do PCB
void Interpreter::UstawRejestr() {
	kolejkaGotowych.glowa->proces->ustawRej1(A);
	kolejkaGotowych.glowa->proces->ustawRej2(B);
	kolejkaGotowych.glowa->proces->ustawRej3(C);
	kolejkaGotowych.glowa->proces->ustawRej4(D);
	kolejkaGotowych.glowa->proces->ustawLicznikRozkazow(LicznikRozkazow);
	kolejkaGotowych.glowa->proces->ustawRamLokalizacja(LokalizacjaProgramu);
}

void Interpreter::WpiszDoKomorki(int lokalizacja, std::string dane)
{

}

//Wykonywanie programu
void Interpreter::WykonywanieProgramu() {
	//dodac pobieranie rejestrow od bartosza i sprawdzanie liczby wykonanych rozkazow
	//...
	//...
	//...
	//...
	PobierzRejestry();
	if (LokalizacjaProgramu != -1) {
		if (Rozkaz != "EX") {
			std::string Symbol, Dane;
			if (Rozkaz.size() == 2) {
				Symbol = Rozkaz;
				Dane = "";
			}
			else {
				Symbol = Rozkaz.substr(0, 2); // Rozkaz memoniczny
				Dane = Rozkaz.substr(3, Rozkaz.length() - 3); //Dane od danego rozkazu
			}
														  //Rozkazy Asemblerowe

														  //Dodawanie
			if (Symbol == "AD") {
				std::string rejestr = Dane.substr(0, 1);
				std::string Pom = Dane.substr(2, Dane.size() - 2);
				//Sumowanie Rejestr Rejestr
				if (rejestr == "A") {
					if (Pom == "B") {
						A += B;
					};
					if (Pom == "C") {
						A += C;
					};
					if (Pom == "D")
					{
						A += D;
					};
					//Sumowanie Rejestr RAM
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}
					*/

					//Sumowanie Rejestr Liczba
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						A += Liczba;
					}
				};
				//Sumowanie Rejestr Rejestr
				if (rejestr == "B") {
					if (Pom == "A") {
						B += A;
					};
					if (Pom == "C") {
						B += C;
					};
					if (Pom == "D")
					{
						B += D;
					};
					//Sumowanie Rejestr RAM
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}*/

					//Sumowanie Rejestr Liczba
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						B += Liczba;
					}
				};
				//Sumowanie Rejestr Rejestr
				if (rejestr == "C") {
					if (Pom == "B") {
						C += B;
					};
					if (Pom == "A") {
						C += A;
					};
					if (Pom == "D")
					{
						C += D;
					};
					//Sumowanie Rejestr RAM
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}*/

					//Sumowanie Rejestr Liczba
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						C += Liczba;
					}
				};
				//Sumowanie Rejestr Rejestr
				if (rejestr == "D")
				{
					if (Pom == "B") {
						D += B;
					};
					if (Pom == "C") {
						D += C;
					};
					if (Pom == "A")
					{
						D += A;
					};
					//Sumowanie Rejestr RAM
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}
					*/

					//Sumowanie Rejestr Liczba
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						D += Liczba;
					}

				};
			}

			//Wpysiwanie
			else if (Symbol == "MV") {
				if (Dane[0] == 'A' || Dane[0] == 'B' || Dane[0] == 'C' || Dane[0] == 'D') {
					std::string rejestr = Dane.substr(0, 1);
					std::string Pom = Dane.substr(2, Dane.size() - 2);
					//Kopiowanie jednego rejestru do drugiego
					if (Pom[0] == 'A' || Pom[0] == 'B' || Pom[0] == 'C' || Pom[0] == 'D') {
						if (rejestr == "A") {
							if (Pom[0] == 'B') {
								A = B;
							}
							if (Pom[0] == 'C') {
								A = C;
							}
							if (Pom[0] == 'D') {
								A = D;
							}
						}
						if (rejestr == "B") {
							if (Pom[0] == 'A') {
								B = A;
							}
							if (Pom[0] == 'C') {
								B = C;
							}
							if (Pom[0] == 'D') {
								B = D;
							}
						}
						if (rejestr == "C") {
							if (Pom[0] == 'B') {
								C = B;
							}
							if (Pom[0] == 'A') {
								C = A;
							}
							if (Pom[0] == 'D') {
								C = D;
							}
						}
						if (rejestr == "D") {
							if (Pom[0] == 'B') {
								D = B;
							}
							if (Pom[0] == 'C') {
								D = C;
							}
							if (Pom[0] == 'A') {
								D = A;
							}
						}
					}
					else {
						//Wpisywanie liczby do danego rejestru, dzia³a dla ujemnych 
						std::string liczba;
						liczba.clear();
						int Liczba;
						int i = 0;
						while (Pom[i] != '\0') {
							liczba += Pom[i];
							i++;
						}
						Liczba = stoi(liczba);
						if (rejestr == "A") {
							A = Liczba;
						}
						if (rejestr == "B") {
							B = Liczba;
						}
						if (rejestr == "C") {
							C = Liczba;
						}
						if (rejestr == "D") {
							D = Liczba;
						}
					}
				}
				//Wpisywanie liczb do RAMU, NIE DZIALA JESZCZE
				////////////////////////////////////////////////////////////////////////////////////////////////WPISYWANIE
				else {
					int i = 1;
					std::string indeks;
					indeks.clear();
					std::string liczba; // CO WPISUJE 
					liczba.clear();
					int Indeks; // POD JAKI INDEKS
					int pom = 0;
					for (i = 1; Dane.size() > i; i++) {
						if (Dane[i] == ']') { pom = 1; i += 2; }
						if (pom == 0) indeks += Dane[i];
						if (pom == 1) liczba += Dane[i];
					}
					Indeks = stoi(indeks);
					//TU MUSZE CIE WYWOLAC Z 
					// Dodac funkce wpisujaca do ramo dowolna warotcs 
					//ram.saveToRam(33,Indeks, liczba);
				}

			}

			//Zwiekszanie o 1
			else if (Symbol == "IN") {
				if (Dane == "A") {
					A += 1;
				}
				if (Dane == "B") {
					B += 1;
				}
				if (Dane == "C") {
					C += 1;
				}
				if (Dane == "D") {
					D += 1;
				}
			}

			//Zmniejszanie o 1
			else if (Symbol == "DC") {
				if (Dane == "A") {
					A -= 1;
				}
				if (Dane == "B") {
					B -= 1;
				}
				if (Dane == "C") {
					C -= 1;
				}
				if (Dane == "D") {
					D -= 1;
				}
			}

			//Skok warunkowy dopuki Rejestr D<0 albo 1 ..... 
			else if (Symbol == "JP") {
				int skok = stoi(Dane);
				if (D > 0) {
					LicznikRozkazow -= skok;
					D -= 1;
				}
			}

			//Odejmowanie
			else if (Symbol == "SB") {
				std::string rejestr = Dane.substr(0, 1);
				std::string Pom = Dane.substr(2, Dane.size() - 2);
				//Odejmowanie Rejestr Rejestr
				if (rejestr == "A") {
					if (Pom == "B") {
						A -= B;
					};
					if (Pom == "C") {
						A -= C;
					};
					if (Pom == "D")
					{
						A -= D;
					};
					//Odejmowanie z rejestru watosci z ramu
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}
					*/

					//Odejmowanie od rejestru danej liczby 
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						A -= Liczba;
					}
				};
				//Odejmowanie Rejestr Rejestr
				if (rejestr == "B") {
					if (Pom == "A") {
						B -= A;;
					};
					if (Pom == "C") {
						B -= C;
					};
					if (Pom == "D")
					{
						B -= D;
					};
					//Odejmowanie z rejestru watosci z ramu
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}
					*/

					//Odejmowanie od rejestru danej liczby 
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						B = B - Liczba;
					}

				};
				//Odejmowanie Rejestr Rejestr
				if (rejestr == "C") {
					if (Pom == "B") {
						C -= B;
					};
					if (Pom == "A") {
						C -= A;
					};
					if (Pom == "D")
					{
						C -= D;
					};

					//Odejmowanie z rejestru watosci z ramu
					/*if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}*/

					//Odejmowanie od rejestru danej liczby 
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						C -= Liczba;
					}

				};
				//Odejmowanie Rejestr Rejestr
				if (rejestr == "D")
				{
					if (Pom == "B") {
						D -= B;
					};
					if (Pom == "C") {
						D -= C;
					};
					if (Pom == "A")
					{
						D -= A;
					};
					//Odejmowanie z rejestru watosci z ramu
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}*/

					//Odejmowanie od rejestru danej liczby 
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						D -= Liczba;
					}

				};
			}

			//Mno¿enie
			else if (Symbol == "ML") {
				std::string rejestr = Dane.substr(0, 1);
				std::string Pom = Dane.substr(2, Dane.size() - 2);
				//Mnozenie Rejestr Rejestr
				if (rejestr == "A") {
					if (Pom == "B") {
						A *= B;
					};
					if (Pom == "C") {
						A *= C;
					};
					if (Pom == "D")
					{
						A *= D;
					};
					//mnozenie rejetr ram
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}*/

					//Mnozenie rejestr liczba 
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						A *= Liczba;
					}
				};
				//Mnozenie Rejestr Rejestr
				if (rejestr == "B") {
					if (Pom == "A") {
						B *= A;
					};
					if (Pom == "C") {
						B *= C;
					};
					if (Pom == "D")
					{
						B *= D;
					};
					//Mnozenie rejestr ram
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}*/

					//Mnozenie rejestr liczba 
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						B *= Liczba;
					}
				};
				//Mnozenie Rejestr Rejestr
				if (rejestr == "C") {
					if (Pom == "B") {
						C *= B;
					};
					if (Pom == "A") {
						C *= A;
					};
					if (Pom == "D")
					{
						C *= D;
					};
					//Mnozenie rejestr ram
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}*/

					//Mnozenie rejestr liczba 
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						C *= Liczba;
					}

				};
				//Mnozenie Rejestr Rejestr
				if (rejestr == "D")
				{
					if (Pom == "B") {
						D *= B;
					};
					if (Pom == "C") {
						D *= C;
					};
					if (Pom == "A")
					{
						D *= A;
					};
					//Mnozenie rejestr ram
					/*
					if (Pom == "[") {
					std::string indeks;
					int Indeks;
					int i = 4;
					while (Dane[i] != ']') {
					indeks += Dane[i];
					i++;
					}
					Indeks = stoi(indeks);
					//A += RAM[indeks];  wartosc zwracana z dnej komurki
					}*/

					//Mnozenie rejestr liczba 
					if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
						std::string liczba = Dane.substr(2, Dane.size() - 2);
						int Liczba = stoi(liczba);
						D *= Liczba;
					}

				};
			}

			//Rozkazy do tworzenia plików

			//Tworzenie pliku
			else if (Symbol == "CF") {
				std::string nazwa = "";
				std::string rozszerzenie = "";
				int pom = 0;
				for (int i = 0; Dane.size() > i; i++) {
					if (Dane[i] == '.') {
						pom = 1; i++;
					}
					if (pom == 0) {
						nazwa += Dane[i];
					}
					else {
						rozszerzenie += Dane[i];
					}
				}
				dysk.utworzPlik(nazwa, rozszerzenie);
			}

			//Wpisywanie danych do pliku - dzia³a 
			else if (Symbol == "WF") {
				std::string nazwa = "";
				std::string rozszerzenie = "";
				std::string dane = "";
				int i = 0;
				int pom = 0;
				for (i = 0; Dane.size() > i; i++) {
					if (Dane[i] == '.') {
						pom = 1; i++;
					}
					if (Dane[i] == ' ') {
						pom = 2; i++;
					}
					if (pom == 0) {
						nazwa += Dane[i];
					}
					if (pom == 1) {
						rozszerzenie += Dane[i];
					}
					if (pom == 2) {
						dane += Dane[i];
					}
				}
				if (dane[0] == '~') {
					if (dane[1] == 'A') {
						dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(A), kolejkaGotowych.glowa->proces);
					}
					if (dane[1] == 'B') {
						dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(B), kolejkaGotowych.glowa->proces);
					}
					if (dane[1] == 'C') {
						dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(C), kolejkaGotowych.glowa->proces);
					}
					if (dane[1] == 'D') {
						dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(D), kolejkaGotowych.glowa->proces);
					}
				}
				else {
					dysk.zapiszDoPliku(nazwa, rozszerzenie, dane, kolejkaGotowych.glowa->proces);
				}
				dysk.zamknijPlik(nazwa, rozszerzenie, kolejkaGotowych.glowa->proces);
			}

			// Tworzenie folderów - dzia³aa 
			else if (Symbol == "CK") {
				std::string folder1 = "";
				std::string folder2 = "Dysk";
				for (int i = 0; Dane.size() > i; i++) {
					if (Dane[i] != ' ') {
						folder1 += Dane[i];
					}
					else {
						folder2 = Dane.substr(i + 1, Dane.size() - i - 1);
						break;
					}
				}
				dysk.utworzFolder(folder1, folder2);
			}

			//Rozkazy do procesow

			else if (Symbol == "SM") {
				PCB* procesTymczasowy = kolejkaGotowych.glowa->proces;
				std::string odbiorca = "";
				int a = 0;
				for (int i = 0; i < Dane.size(); i++)
				{
					if (Dane[i] != ' ') {
						odbiorca += Dane[i];
						a++;
					}
					else
						break;
				}
				if (komunikacja.rozkazWyslaniaKomunikatu(kolejkaGotowych.glowa->proces->dajId(), odbiorca, Dane.substr(a, Dane.size() - a)) == false)
				{
					return;
				}
			}

			else if (Symbol == "SP") {
				PCB* procesTymczasowy = kolejkaGotowych.glowa->proces;
				std::string odbiorca = "";
				int a = 0;
				for (int i = 0; i < Dane.size(); i++)
				{
					if (Dane[i] != ' ') {
						odbiorca += Dane[i];
						a++;
					}
					else
						break;
				}
				a++;
				std::string plik;
				std::string rozszerzenie;
				while (Dane[a] != '.') {
					plik += Dane[a];
					a++;
				}
				a++;
				rozszerzenie += Dane.substr(a, Dane.size() - a);
				std::string komunikat = dysk.pobierzDane(plik, rozszerzenie, procesTymczasowy);
				if (komunikacja.rozkazWyslaniaKomunikatu(kolejkaGotowych.glowa->proces->dajId(), odbiorca, komunikat) == false)
				{
					return;
				}
			}

			else if (Symbol == "RM") {
				PCB* procesTymczasowy = kolejkaGotowych.glowa->proces;
				if (komunikacja.rozkazOdebraniaKomunikatu(kolejkaGotowych.glowa->proces->dajId()) == false) {
					LicznikRozkazow -= 4;
					procesTymczasowy->ustawLicznikRozkazow(LicznikRozkazow);
					return;
				}
			}

			else if (Symbol == "") {

			}

			else {
				std::clog << "Nie ma takiej komendy." << std::endl;
				kolejkaGotowych.glowa->proces->ustawStatus(4);
				return;
			}

			UstawRejestr();
		}
		else {
			UstawRejestr();
			kolejkaGotowych.uruchomEkspedytor(true);
			NumerRozkazu = 0; //dodano - test
			PobierzRejestry();
			return;
		}
		NumerRozkazu++;
		if (NumerRozkazu == 5) {
			kolejkaGotowych.uruchomEkspedytor(false);
		}
		PobierzRejestry();
		std::clog << std::endl;
	}
else {
	zarzadzanieProcesami.usunProces(kolejkaGotowych.glowa->proces->dajId());
	//kolejkaGotowych.glowa->proces.usunProces(kolejkaGotowych.glowa->proces);
}
}

