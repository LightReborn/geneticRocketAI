#pragma once

#include <iterator>
#include "settings.h"
#include "Genetics.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

class Rocket {

private:
	//data members
	GeneticCode DNA;
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
	void move(float magnitude);
	void turn(float degrees);
	float getX();
	float getY();
};