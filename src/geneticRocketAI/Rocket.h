#ifndef RocketAI
#define RocketAI

#include <list>

class Rocket {

private:
	//highly specialized data types used exclusively by Rocket.
	enum GeneMessage {
		MOVE_FORWARD,
		ADJUST_ANGLE
	};

	struct Gene	{
		GeneMessage commandType;
		float magnitude;
	};

	typedef std::list<Gene> geneticCode;

	//data members
	geneticCode DNA;

public:
	//method members
	Rocket();
	Rocket(geneticCode inboundDNA);

	void copyGenesInto(geneticCode outboundDNA);
	void copyGenesFrom(geneticCode inboundDNA);
};

#endif