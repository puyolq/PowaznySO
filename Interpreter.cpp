#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

#include"Interpreter.h"
#include <algorithm>
#include <cctype>

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
	std::string pom2 = "";
	program = program + ".txt";
	std::fstream plik;
	plik.open(program, std::ios::in);
	if (plik.good() == false) {
		std::cout << "Nie ma takiego programu" << std::endl;
	}
	else {
		while (getline(plik, pom)) {
			pom2 = pom2 + pom + '\00';
		}

	}
	ram.addToMem(pcb, pom2);
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

//obsluga bledow fo komend na dysku
void Interpreter::obsluzBledy(short blad)
{
	//  1 : wszystko ok;
	// -1 : B³edna nazwa folderu
	//- 2 : Nie odnaleziono pliku.
	//- 3 : Semafor zablokowany, brak dostepu.
	//- 4 : Brak miejsca na dysku.
	//- 5 : W folderze nie mozna utworzyc nowych podfolderow.
	//- 6 : Nazwa nie jest jednoznaczna.
	if (blad == 1) {
		//...dobrze
	}
	else if (blad == -1) {
		std::clog << "Blad programu: Bledna nazwa folderu" << std::endl;
	}
	else if (blad == -2) {
		std::clog << "Blad programu: Plik nie istnieje" << std::endl;
	}
	else if (blad == -3) {
		//...PCB
	}
	else if (blad == -4) {
		std::clog << "Blad programu: Brak miejsca na dysku" << std::endl;
	}
	else if (blad == -5) {
		std::clog << "Blad programu: W folderze nie mozna utworzyc nowych podfolderow" << std::endl;
	}
	else if (blad == -6) {
		std::clog << "Blad programu: Plik juz istnieje w folderze" << std::endl;
	}
	else
	{
		std::clog << "Blad programu: Blad nie jest rozpatrywany" << std::endl;
	}
}

//sprawdzanie czy dany string to liczba
bool Interpreter::CzyStringLiczba(std::string& s)
{
	return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
	return true;
}


//Wykonywanie programu
void Interpreter::WykonywanieProgramu() {
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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							A += stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							A += 0;
						}
					}


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
					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							B += stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							B += 0;
						}
					}

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
					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							C += stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							C += 0;
						}
					}

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
					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							D += stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							D += 0;
						}
					}

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
					if (liczba[0] == '~') {
						if (liczba[1] == 'A') { ram.saveToRam(kolejkaGotowych.glowa->proces->dajRamLokalizacja(), Indeks, std::to_string(A)); }
						if (liczba[1] == 'B') { ram.saveToRam(kolejkaGotowych.glowa->proces->dajRamLokalizacja(), Indeks, std::to_string(B)); }
						if (liczba[1] == 'C') { ram.saveToRam(kolejkaGotowych.glowa->proces->dajRamLokalizacja(), Indeks, std::to_string(C)); }
						if (liczba[1] == 'D') { ram.saveToRam(kolejkaGotowych.glowa->proces->dajRamLokalizacja(), Indeks, std::to_string(D)); }
					}
					else {
						ram.saveToRam(kolejkaGotowych.glowa->proces->dajRamLokalizacja(), Indeks, liczba);
					}
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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							A -= stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							A -= 0;
						}
					}

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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							B -= stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							B -= 0;
						}
					}

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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							C -= stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							C -= 0;
						}
					}

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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							D -= stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							D -= 0;
						}
					}

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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							A *= stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							A = A;
						}
					}

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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							B *= stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							B = B;
						}
					}

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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							C *= stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							C = C;
						}
					}

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

					if (Pom[0] == '[') {
						std::string indeks;
						int Indeks;
						int i = 1;
						while (Pom[i] != ']') {
							indeks += Pom[i];
							i++;
						}
						Indeks = stoi(indeks);
						if (CzyStringLiczba(ram.showProcess(Indeks))) {
							D *= stoi(ram.showProcess(Indeks));
						}
						else {
							//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
							D = D;
						}
					}

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
				if (dysk.utworzPlik(nazwa, rozszerzenie) != 1) {
					obsluzBledy(dysk.utworzPlik(nazwa, rozszerzenie));
					ram.deleteFromMem(kolejkaGotowych.glowa->proces);
				};
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
						if (dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(A), kolejkaGotowych.glowa->proces) != 1) {
							obsluzBledy(dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(A), kolejkaGotowych.glowa->proces) != 1);
							ram.deleteFromMem(kolejkaGotowych.glowa->proces);
						};
					}
					if (dane[1] == 'B') {
						if (dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(B), kolejkaGotowych.glowa->proces) != 1) {
							obsluzBledy(dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(A), kolejkaGotowych.glowa->proces) != 1);
							ram.deleteFromMem(kolejkaGotowych.glowa->proces);
						};
					}
					if (dane[1] == 'C') {
						if (dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(C), kolejkaGotowych.glowa->proces) != 1) {
							obsluzBledy(dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(A), kolejkaGotowych.glowa->proces) != 1);
							ram.deleteFromMem(kolejkaGotowych.glowa->proces);
						};
					}
					if (dane[1] == 'D') {
						if (dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(D), kolejkaGotowych.glowa->proces) != 1) {
							obsluzBledy(dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(A), kolejkaGotowych.glowa->proces) != 1);
							ram.deleteFromMem(kolejkaGotowych.glowa->proces);
						};
					}
				}
				else {
					if (dysk.zapiszDoPliku(nazwa, rozszerzenie, dane, kolejkaGotowych.glowa->proces) != 1) {
						obsluzBledy(dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(A), kolejkaGotowych.glowa->proces) != 1);
						ram.deleteFromMem(kolejkaGotowych.glowa->proces);
					};
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
				if (dysk.utworzFolder(folder1, folder2) != 1) {
					obsluzBledy(dysk.utworzFolder(folder1, folder2) != 1);
					ram.deleteFromMem(kolejkaGotowych.glowa->proces);
				};
			}

			//Rozkazy do procesow

			//wysylanie wiadomosci
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
				std::string wys = Dane.substr(a + 1, Dane.size() - a - 1);
				if (komunikacja.rozkazWyslaniaKomunikatu(kolejkaGotowych.glowa->proces->dajId(), odbiorca, wys) == false)
				{
					return;
				}
			}

			//wysylanie wiadomosci pobranej z pliku na dysku
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
				pobieDane wyjscie = dysk.pobierzDane(plik, rozszerzenie, procesTymczasowy);
				int stan = wyjscie.blad;
				if (stan == 1) {
					std::string komunikat = wyjscie.dane;
					if (komunikacja.rozkazWyslaniaKomunikatu(kolejkaGotowych.glowa->proces->dajId(), odbiorca, komunikat) == false)
					{
						return;
					}
				}
				else {
					obsluzBledy(stan);
					ram.deleteFromMem(kolejkaGotowych.glowa->proces);
				}
			}

			//czytanie wiadomosci 
			else if (Symbol == "RM") {
				PCB* procesTymczasowy = kolejkaGotowych.glowa->proces;
				Wiadomosc odebranaWiadomosc = komunikacja.rozkazOdebraniaKomunikatu(kolejkaGotowych.glowa->proces->dajId());
				if (odebranaWiadomosc.pobierzKomunikat() == "") {
					LicznikRozkazow -= 3;
					procesTymczasowy->ustawLicznikRozkazow(LicznikRozkazow);
					return;
				}
				std::cout << "Komunikat od procesu o ID: " << odebranaWiadomosc.pobierzNumerPIDNadawcy() << " o tresci: " << odebranaWiadomosc.pobierzKomunikat() << std::endl;
			}

			//DLACZEGO USUWA PROCES W RZ Z USUNIECIEM PROCESU SM
			//czytanie wiadomosci i zapis na dysku do pliku 
			else if (Symbol == "RP") {
				PCB* procesTymczasowy = kolejkaGotowych.glowa->proces;
				Wiadomosc odebranaWiadomosc = komunikacja.rozkazOdebraniaKomunikatu(kolejkaGotowych.glowa->proces->dajId());
				int a = 0;
				std::string plik;
				std::string rozszerzenie;
				while (Dane[a] != '.') {
					plik += Dane[a];
					a++;
				}
				a++;
				rozszerzenie += Dane.substr(a, Dane.size() - a);
				if (odebranaWiadomosc.pobierzKomunikat() == "") {
					LicznikRozkazow -= (plik.size() + rozszerzenie.size() + 5);
					procesTymczasowy->ustawLicznikRozkazow(LicznikRozkazow);
					return;
				}
				if (dysk.zapiszDoPliku(plik, rozszerzenie, odebranaWiadomosc.pobierzKomunikat(), kolejkaGotowych.glowa->proces) != 1) {
					obsluzBledy(dysk.zapiszDoPliku(plik, rozszerzenie, odebranaWiadomosc.pobierzKomunikat(), kolejkaGotowych.glowa->proces) != 1);
					ram.deleteFromMem(kolejkaGotowych.glowa->proces);
				}
			}

			else {
				std::clog << "Nie ma takiej komendy." << std::endl;
				kolejkaGotowych.glowa->proces->ustawStatus(4);
				return;
			}

			UstawRejestr();
		}
		else {
			//UstawRejestr();
			kolejkaGotowych.uruchomEkspedytor(true);
			//PobierzRejestry();
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

