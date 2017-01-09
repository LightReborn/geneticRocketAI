#pragma once

#include <iterator>
#include "Genetics.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ROCKET_SPEED (5)

class Rocket {

private:
	//data members
	GeneticCode DNA;
	GeneticCode executedDNA;
	float xPosition, yPosition;
	float angle;

	//Private Method members
	GeneMessage generateRandomCommandType();
	float generateRandomMoveMagnitude();
	float generateRandomAngleMagnitude();
	Gene generateRandomGene();

public:
	//Public Method members
	Rocket();
	Rocket(float x, float y);
	Rocket(GeneticCode inboundDNA);
	Rocket(float x, float y, GeneticCode inboundDNA);
	void generateRandomDNA();
	void copyGenesInto(GeneticCode& outboundDNA);
	void copyGenesFrom(GeneticCode inboundDna);
	GeneticCode breedWith(Rocket mateRocket);
	void executeGene(Gene commandSet);
	void stepDNA();
	void executeDNA();
	void move(float magnitude);
	void turn(float degrees);
	float getX();
	float getY();
	bool DNAIsExecuted();
};