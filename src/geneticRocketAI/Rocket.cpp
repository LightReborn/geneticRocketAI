#include "Rocket.h"

GeneMessage Rocket::generateRandomCommandType() {
	int randomMessage = rand() % 2;
	switch (randomMessage) {
	case 0:
		return ADJUST_ANGLE;
	case 1:
		return MOVE_FORWARD;
	default:
		return MOVE_FORWARD;
	}
}

float Rocket::generateRandomMoveMagnitude() {
	return (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / MOVE_MAGNITUDE_CEILING))) + MOVE_MAGNITUDE_FLOOR;
}

float Rocket::generateRandomAngleMagnitude() {
	float totalRange = abs(ANGLE_MAGNITUDE_FLOOR) + abs(ANGLE_MAGNITUDE_CEILING);

	return (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / totalRange))) + ANGLE_MAGNITUDE_FLOOR;
}

Gene Rocket::generateRandomGene() {
	Gene newGene;

	newGene.commandType = generateRandomCommandType();

	if (newGene.commandType == MOVE_FORWARD) {
		newGene.magnitude = generateRandomMoveMagnitude();
	}
	else if (newGene.commandType == ADJUST_ANGLE) {
		newGene.magnitude = generateRandomAngleMagnitude();
	}
	
	return newGene;
}

Rocket::Rocket() {
	xPosition = 0;
	yPosition = 0;
	angle = 0;
	generateRandomDNA();
}

Rocket::Rocket(float x, float y) {
	xPosition = x;
	yPosition = y;
	angle = 0;
	generateRandomDNA();
}

Rocket::Rocket(GeneticCode& inboundDNA) {
	xPosition = 0;
	yPosition = 0;
	angle = 0;
	DNA = inboundDNA;
}

Rocket::Rocket(float x, float y, GeneticCode& inboundDNA) {
	xPosition = x;
	yPosition = y;
	DNA = inboundDNA;
}

void Rocket::generateRandomDNA() {
	//If DNA was already instantiated this will overwrite the genetic code.
	DNA.clear();
	for (int i = 0; i < DNA_LENGTH; i++) {
		DNA.push_back(generateRandomGene());
	}
}

void Rocket::copyGenesInto(GeneticCode& outboundDNA) {
	std::copy(DNA.begin(), DNA.end(), std::back_insert_iterator<GeneticCode>(outboundDNA));
}

void Rocket::copyGenesFrom(GeneticCode& inboundDna) {
	DNA = inboundDna;
}

void Rocket::breedWith(Rocket& mateRocket, GeneticCode& newDNA) { //typedef std::vector<Gene> GeneticCode;
	//pre: this.DNA.length == DNA_LENGTH && mateRocket.DNA.length == 100
	int crossoverIndex = rand() % DNA_LENGTH;
	//printf("\nthis.DNA: %d\tmate.DNA: %d\tCrossover: %d\n", DNA.size(), mateRocket.DNA.size(), crossoverIndex);

	for (int i = 0; i < crossoverIndex; i++) {
		//printf("i: %d", i);
		newDNA.push_back(DNA[i]); 
	}
	//printf("Out of first part of crossover.\n\n");
	for (int i = crossoverIndex; i < DNA_LENGTH; i++) {
		newDNA.push_back(mateRocket.DNA[i]);
	}
}

void Rocket::mutate() {

	for (auto geneToMutate : DNA) {
		int mutationChance = rand() % 100;
		if (mutationChance <= RATE_OF_MUTATION) {
			//gotta change this gene.
			geneToMutate = generateRandomGene();
		}
	}
}

void Rocket::executeGene(Gene commandSet) {
	if (commandSet.commandType == MOVE_FORWARD) {
		move(commandSet.magnitude);
	}
	else if (commandSet.commandType == ADJUST_ANGLE) {
		turn(commandSet.magnitude);
	}
}

void Rocket::stepDNA() {
	//save the gene that is going to be executed.
	if (DNA.size() > 0) {
		executedDNA.push_back(DNA.back());
		//deploy execution of gene.
		executeGene(DNA.back());
		//remove from DNA
		DNA.pop_back();
	}
	//DNA.size = DNA.size - 1 && exectuedDNA = executedDNA.size + 1
	//DAN = #DNA - DNA.back && executedDNA = executedDNA + DNA.back
}

void Rocket::executeDNA() {
	for (auto geneToExecute : DNA) {
		executeGene(geneToExecute);
	}
}

void Rocket::swapDNAWithExecutedDNA(){
	DNA.swap(executedDNA);
}

void Rocket::move(float magnitude) {
	this->xPosition += (sin(this->angle) * (magnitude * ROCKET_SPEED));
	this->yPosition -= (cos(this->angle) * (magnitude * ROCKET_SPEED));
}

void Rocket::turn(float magnitude) {
	angle += magnitude;
}

float Rocket::getX() {
	return xPosition;
}

float Rocket::getY() {
	return yPosition;
}

bool Rocket::DNAIsExecuted() {
	if (DNA.size() > 0) {
		return false;
	}
	return true;
}

void Rocket::resetPosition(float x, float y) {
	xPosition = x;
	yPosition = y;
	angle = 0;
}