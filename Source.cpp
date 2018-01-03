#include "Kolejka procesow.hpp"
#include "ZarzadzanieProcesami.h"
#include "Shell.h"
#include <iostream>

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
