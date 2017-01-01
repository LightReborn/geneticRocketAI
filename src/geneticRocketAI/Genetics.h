#pragma once

#include <list>

enum GeneMessage {
	MOVE_FORWARD,
	ADJUST_ANGLE
};

struct Gene {
	GeneMessage commandType;
	float magnitude;
};

typedef std::list<Gene> GeneticCode;