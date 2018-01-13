#include <iostream>
#include <string>
#include <Windows.h>
#include "Interpreter.h"
#include "Kolejka procesow.hpp"
#include "RAM.hpp"
#include "ZarzadzanieProcesami.h"
#include "dysk.h"
#include "Semafory.h"
#include "Shell.h"

int main()
{

	Shell shell;
	shell.wlaczone = true;
	std::string wejscie;
	while (shell.wlaczone) {
		std::getline(std::cin, wejscie);
		shell.czytajWejscie(wejscie);
	}
	return 0;
}