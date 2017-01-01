#include <iostream>
#include "Rocket.h"

void main() {
	Rocket myFirstRocket(1, 1);
	GeneticCode container;

	std::cout << "Generating new genetic code.\n";
	myFirstRocket.generateRandomDNA();

	myFirstRocket.copyGenesInto(container);

	for (auto v : container) {
		std::cout << "(" << v.commandType << ", " << v.magnitude << ")\n";
	}
	system("PAUSE");
}