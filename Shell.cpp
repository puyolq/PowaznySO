#include "Shell.h"

using namespace std;




Shell::Shell()
{
	clog << "Shell initialized" << endl;

}

void Shell::CP(std::string nazwa, std::string rodzic)
{
	zarzadzanieProcesami->dodajProces(nazwa, rodzic);
}

void Shell::DP(std::string nazwa)
{
	zarzadzanieProcesami->usunProces(nazwa);
}


void Shell::BC(std::string nazwa)
{
	zarzadzanieProcesami->wyswietlProces(nazwa);
}

void Shell::GO()
{
	interpreter->WykonywanieProgramu();
}

void Shell::PP()
{
	zarzadzanieProcesami->wyswietlWszystkieProcesy();
}

void Shell::MC(int adres, int rozmiar)
{
	ram->showRange(adres, rozmiar);
}

void Shell::MS()
{
	ram->showRam();
}



void Shell::MF(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu)
{
	dysk->utworzPlik(nazwa, rozszerzenie, nazwaFolderu);
}

void Shell::DF(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu)
{
	dysk->usunPlik(nazwa, rozszerzenie, nazwaFolderu);
}

void Shell::RF(std::string nazwa, std::string rozszerzenie, std::string nowaNazwa, std::string nazwaFolderu)
{
	dysk->zmienNazwePliku(nazwa, rozszerzenie, nowaNazwa, nazwaFolderu);
}

void Shell::SF(std::string nazwa, std::string rozszerzenie, std::string dane, PCB * proces, std::string nazwaFolderu)
{
	dysk->zapiszDoPliku(nazwa, rozszerzenie, dane, proces, nazwaFolderu);
	dysk->zamknijPlik(nazwa, rozszerzenie, "", zarzadzanieProcesami->init, nazwaFolderu);
}

void Shell::PF(std::string nazwa, std::string rozszerzenie, PCB * proces, std::string nazwaFolderu)
{
	string wyjscie = dysk->pobierzDane(nazwa, rozszerzenie, proces, nazwaFolderu);
	cout << wyjscie << endl;
	dysk->zamknijPlik(nazwa, rozszerzenie, "", zarzadzanieProcesami->init, nazwaFolderu);
}

void Shell::XF(std::string nazwa, std::string rozszerzenie, PCB * proces, std::string nazwaFolderu)
{
	dysk->otworzStratnie(nazwa, rozszerzenie, zarzadzanieProcesami->init, nazwaFolderu);
	dysk->zamknijPlik(nazwa, rozszerzenie, "", zarzadzanieProcesami->init, nazwaFolderu);
}

void Shell::CF(std::string nazwa, std::string rozszerzenie, std::string dane, PCB * proces, std::string nazwaFolderu)
{
	dysk->zamknijPlik(nazwa, rozszerzenie, dane, zarzadzanieProcesami->init, nazwaFolderu);
}




void Shell::MD(std::string nazwa, std::string nazwaNadrzednego)
{
	dysk->utworzFolder(nazwa, nazwaNadrzednego);
}

void Shell::AD(std::string nazwaDocelowego, std::string nazwaPliku, std::string rozszerzenie, std::string nazwaFolderuZPlikiem)
{
	dysk->dodajPlikDoKatalogu(nazwaDocelowego, nazwaPliku, rozszerzenie, nazwaFolderuZPlikiem);
}

void Shell::DD(std::string nazwa)
{
	int pozycja = dysk->znajdzFolder(nazwa);
	dysk->usunFolder(pozycja);
}

void Shell::FD(std::string nazwa)
{
	dysk->znajdzFolder(nazwa);
}

void Shell::PT()
{
	dysk->wypiszDrzewo();
}

void Shell::PD()
{
	dysk->wypiszDysk();
}

void Shell::PV()
{
	dysk->wypiszWektorBitowy();
}

void Shell::PN()
{
	dysk->wypiszTabliceIwezelow();
}

void Shell::PX()
{
	dysk->wypiszTabliceWpisow();
}


void Shell::exit() {
	cout << "wylaczanka";
	wlaczone = false;
}


void Shell::czytajWejscie(std::string wejscie)
{
	istringstream buffer(wejscie);
	istream_iterator<string> beg(buffer), end;
	vector<string>args(beg, end);

	string komenda = args[0];
	//cout << args[0] << endl << args[1] << endl;
	if (komenda == "CP") {
		if (args.size()!=3)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			CP(args[1], args[2]);
	}
	else if (komenda == "DP") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			DP(args[1]);
	}

	else if (komenda == "BC") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			BC(args[1]);
	}
	else if (komenda == "PP") {
		PP();
	}
	else if (komenda == "MF") {
		if (args.size() <3)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size()!=4)
				args.push_back("Dysk");
			MF(args[1], args[2], args[3]);
		}
	}
	else if (komenda == "DF") {
		if (args.size() <3)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			DF(args[1], args[2], args[3]);
		}
	}
	else if (komenda == "RF") {
		if (args.size() < 4)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			RF(args[1], args[2], args[3], args[4]);
		}
	}

	else if (komenda == "MD") {
		if (args.size() <2)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 3)
				args.push_back("Dysk");
			MD(args[1], args[2]);
		}
	}
	else if (komenda == "AD") {
		if (args.size() <4)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			AD(args[1], args[2], args[3], args[4]);
		}
	}
	else if (komenda == "DD") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			DD(args[1]);
	}
	else if (komenda == "FD") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			FD(args[1]);
	}
	else if (komenda == "PT") {
		PT();
	}
	else if (komenda == "PD") {
		PD();
	}
	else if (komenda == "PV") {
		PV();
	}
	else if (komenda == "PN") {
		PN();
	}
	else if (komenda == "PX") {
		PX();
	}
	else if (komenda == "MS") {
		MS();
	}
	else if (komenda == "MC") {
		if (args.size() != 3)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			MC(stoi(args[1]), stoi(args[2]));
	}

	else if (komenda == "open") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			ladujSkrypt(args[1]);
	}
	else if (komenda == "exit") {
		exit();
	}
	else if (komenda == "SF") {
		if (args.size() < 4)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			SF(args[1], args[2], args[3], zarzadzanieProcesami->init, args[4]);

		}
	}
	else if (komenda == "PF") {
		if (args.size() < 3)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			PF(args[1], args[2], zarzadzanieProcesami->init, args[3]);

		}
	}
	else if (komenda == "XF") {
		if (args.size() < 3)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			XF(args[1], args[2], zarzadzanieProcesami->init, args[3]);

		}
	}
	else if (komenda == "CF") {
		if (args.size() < 4)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			CF(args[1], args[2], args[3], zarzadzanieProcesami->init, args[4]);
		}
	}
	args.clear();

}

void Shell::ladujSkrypt(string nazwa)
{
	string komenda = "";
	skrypt.open(nazwa + ".txt");
	if (!skrypt.good()) {
		cout << "Brak pliku" << endl;
	}
	string linia;
	vector<vector<string>>polecenia;
	vector<string>polecenie;

	while (getline(skrypt, linia)) {
		istringstream buffer(linia);
		istream_iterator<string> beg(buffer), end;
		vector<string>polecenie(beg, end);
		polecenia.push_back(polecenie);
		polecenie.clear();

	}


	for (int i = 0; i < polecenia.size(); i++) {
		for (int j = 0; j < polecenia[i].size(); j++) {

			//cout << polecenia[i][j];
			if (j != polecenia[i].size())
				komenda = komenda + polecenia[i][j] + " ";
			else
				komenda = komenda + polecenia[i][j];
		}
		czytajWejscie(komenda);
		komenda = "";
		//cout << endl;
	}

	polecenia.clear();
	skrypt.close();
}

