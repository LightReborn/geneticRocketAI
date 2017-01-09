#pragma once

#include <vector>

#define MOVE_MAGNITUDE_FLOOR 1.0
#define MOVE_MAGNITUDE_CEILING 5.0
#define ANGLE_MAGNITUDE_FLOOR -5.0
#define ANGLE_MAGNITUDE_CEILING 5.0
#define DNA_LENGTH 100

enum GeneMessage {
	MOVE_FORWARD,
	ADJUST_ANGLE
};

struct Gene {
	GeneMessage commandType;
	float magnitude;
};

typedef std::vector<Gene> GeneticCode;