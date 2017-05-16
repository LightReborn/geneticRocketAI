#pragma once

#include <iterator>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>

enum GeneMessage {
	MOVE_FORWARD,
	ADJUST_ANGLE
};

struct Gene {
	GeneMessage commandType;
	float magnitude;
};

typedef std::vector<Gene> GeneticCode;

#define ROCKET_SPEED (5)
#define DNA_LENGTH (100)
#define MOVE_MAGNITUDE_FLOOR (1.0)
#define MOVE_MAGNITUDE_CEILING (5.0)
#define ANGLE_MAGNITUDE_FLOOR (-5.0)
#define ANGLE_MAGNITUDE_CEILING (5.0)
//in percentage
#define RATE_OF_MUTATION (90)

class Rocket {

private:
	//data members
	
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
	GeneticCode DNA;
	Rocket();
	Rocket(float x, float y);
	Rocket(GeneticCode& inboundDNA);
	Rocket(float x, float y, GeneticCode& inboundDNA);
	void generateRandomDNA();
	void copyGenesInto(GeneticCode& outboundDNA);
	void copyGenesFrom(GeneticCode& inboundDna);
	void breedWith(Rocket& mateRocket, GeneticCode& newDNA);
	void mutate();
	void executeGene(Gene commandSet);
	void stepDNA();
	void executeDNA();
	void swapDNAWithExecutedDNA();
	void move(float magnitude);
	void turn(float degrees);
	float getX();
	float getY();
	bool DNAIsExecuted();
	void resetPosition(float x, float y);
};