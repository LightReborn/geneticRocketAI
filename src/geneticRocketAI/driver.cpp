#include "stdio.h"
#include "Rocket.h"

#include "SDL.h"
#include "SDL_image.h"

#define WINDOW_HEIGHT (640)
#define WINDOW_WIDTH (480)

//speed in pixels per second
#define NUMBER_OF_ROCKETS (25)

#define TARGET_FITNESS (100)

struct Target {
	float x;
	float y;
	Target(int inX, int inY) {
		x = inX;
		y = inY;
	}
}rocketTarget(WINDOW_HEIGHT/2, 10);

bool allDNAExecuted(std::vector<Rocket>& rockets) {

	for (int i = 0; i < NUMBER_OF_ROCKETS; i++) {
		if (!rockets[i].DNAIsExecuted()) {
			return false;
		}
	}
	return true;
}

float getDistance(float x1, float y1, float x2, float y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;

	return sqrt((dx * dx) + (dy * dy));
}

int determineFitness(std::vector<Rocket>& rockets, std::vector<float>& fitnessVector) {
	float fitness = 0.0;
	for (auto rocket : rockets) {
		fitness = 1 / getDistance(rocket.getX(), rocket.getY(), rocketTarget.x, rocketTarget.y);
		if (fitness >= TARGET_FITNESS) {//let us know if we've hit our target.
			printf("Target of %d fitness reached. Actual: %f", TARGET_FITNESS, fitness);
			return 1;
		}
		fitnessVector.push_back(fitness);
	}

	return 0;
}

void getBestRockets(std::vector<Rocket>& rockets, std::vector<float>& fitnessVector, std::vector<Rocket>& newRocketVector) {
	//get the top performing 20% of rockets.
	unsigned int howManyRocketsToKeep = NUMBER_OF_ROCKETS / 5;

	while (newRocketVector.size() <= howManyRocketsToKeep - 1) {
		int indexOfBestRocket = 0;
		for (unsigned int i = 1; i < rockets.size(); i++) {
			if (fitnessVector[i] > fitnessVector[indexOfBestRocket]) {
				indexOfBestRocket = i;
			}
		}
		newRocketVector.push_back(rockets[indexOfBestRocket]);
		fitnessVector[indexOfBestRocket] = 0.0; //the loop will see the best value as the worst one instead now.
	}
}

void generateNewRockets(std::vector<Rocket>& rockets) {
	//fill the vector with new rockets by using the Rocket.breedWith() method until the vector reach NUMBER_OF_ROCKETS
	int originalSize = rockets.size() - 1;
	GeneticCode newDNA;

	//printf("rockets: %d\n", originalSize);
	for (int i = 1; (i < originalSize) && (rockets.size() < NUMBER_OF_ROCKETS - 1); i++) {
		rockets[0].breedWith(rockets[i], newDNA);
	}
	//printf("out of for loop\n");
	while (rockets.size() < NUMBER_OF_ROCKETS) {
		//breed the rest randomly
		int firstRocketIndex = rand() % rockets.size();
		int secondRocketIndex = rand() % rockets.size();

		GeneticCode newDNA;
		rockets[firstRocketIndex].breedWith(rockets[secondRocketIndex], newDNA);
		rockets.push_back(Rocket(newDNA));
	}
	//printf("Finished generating new rockets.\n");
	
}

//todo
void mutateAll(std::vector<Rocket>& rockets) {
	for (auto rocket : rockets) {
		rocket.mutate();
	}
}

//todo
int breed(std::vector<Rocket>& rockets) {
	std::vector<float> fitnessVector;
	std::vector<Rocket> newRocketVector;

	if (determineFitness(rockets, fitnessVector) == 1) {
		return 1;
	}
	getBestRockets(rockets, fitnessVector, newRocketVector);

	rockets.swap(newRocketVector);
	//free up space.
	newRocketVector.clear(); 
	//printf("Length of rockets: %d", rockets.size());

	generateNewRockets(rockets);
	mutateAll(rockets);

	return 0;
}

void undoAllExecutions(std::vector<Rocket>& rockets) {
	for (auto &rocket : rockets) {
		rocket.swapDNAWithExecutedDNA();
	}
}

#undef main
int main(void) {
	bool loopFlag = true;
	bool successfulGeneration = false;
	float standardX = WINDOW_WIDTH / 2;
	float standardY = WINDOW_HEIGHT / 2;
	srand(time(NULL));
	
	//initialize
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr,
			"Unable to initialize SDL: %s\n",
			SDL_GetError()
		);
		return 1;
	}

	//Create the window
	SDL_Window* workingWindow = SDL_CreateWindow(
		"Genetic Rockets",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_HEIGHT, WINDOW_WIDTH, 0
	);

	if (!workingWindow) {
		printf("Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//load image
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* rend = SDL_CreateRenderer(workingWindow, -1, render_flags);
	if (!rend) {
		printf("Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(workingWindow);
		SDL_Quit();
		return 1;
	}

	SDL_Surface* rocketIMG = IMG_Load("AVA2.jpg");
	if (!rocketIMG) {
		printf("Error loading rocket image: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(workingWindow);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, rocketIMG);
	SDL_FreeSurface(rocketIMG);
	if (!tex) {
		printf("Error creating texture: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(workingWindow);
		SDL_Quit();
		return 1;
	}

	int numbOfLoops = 0;

	//actual work begins
	printf("Generating %d new rockets.\n", NUMBER_OF_ROCKETS);
	std::vector<Rocket> rocketVector;
	//generate list of rockets
	for (int i = 0; i < NUMBER_OF_ROCKETS; i++) {
		Rocket newRocket(standardX, standardY);
		newRocket.generateRandomDNA();
		rocketVector.push_back(newRocket);
	}
	printf("Number of rockets: %d\n", rocketVector.size());

	printf("Generating %d new rects.\n", NUMBER_OF_ROCKETS);
	std::vector<SDL_Rect> rectVector;
	int i = 0;
	//generate list of rects.
	for (auto rocket : rocketVector) {
		SDL_Rect dest;

		SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
		dest.w = 5;
		dest.h = 25;

		dest.x = rocket.getX();
		dest.y = rocket.getY();

		rectVector.push_back(dest);
	}
	printf("Number of rects: %d\n", rectVector.size());
	//The rect representing the target destination.
	SDL_Rect targetRect;
	SDL_QueryTexture(tex, NULL, NULL, &targetRect.w, &targetRect.h);
	targetRect.w = 25;
	targetRect.h = 25;
	targetRect.x = rocketTarget.x;
	targetRect.y = rocketTarget.y;

	printf("Entering display loop\n");
	//render the rockets at their starting positions.
	SDL_RenderClear(rend);

	for (auto currentRect : rectVector) {
		SDL_RenderCopy(rend, tex, NULL, &currentRect);
	}
	SDL_RenderCopy(rend, tex, NULL, &targetRect);

	while (loopFlag) {
		SDL_RenderClear(rend);

		//printf("Handling events.\n");
		SDL_Event eventToProcess;
		while (SDL_PollEvent(&eventToProcess)) {
			if (eventToProcess.type == SDL_QUIT) { //if they clicked X to close the window.
				loopFlag = false;
			}
		}

		//printf("Updating rockets.\n");
		for (unsigned int i = 0; i < rocketVector.size(); i++) {
			rocketVector[i].stepDNA();
			//printf("Main: (%f, %f)\n", rocketVector[i].getX(), rocketVector[i].getY());
		}

		//printf("Updating rects.\n");
		for (int i = 0; i < NUMBER_OF_ROCKETS; i++) {
			rectVector[i].x = rocketVector[i].getX();
			rectVector[i].y = rocketVector[i].getY();
		}

		//printf("Rendering rockets.\n");
		for (auto currentRect : rectVector) {
			SDL_RenderCopy(rend, tex, NULL, &currentRect);
			//SDL_RenderPresent(rend);
		}
		SDL_RenderCopy(rend, tex, NULL, &targetRect);
		SDL_RenderPresent(rend);


		if (allDNAExecuted(rocketVector)) {
			numbOfLoops++;
			undoAllExecutions(rocketVector);
			if (breed(rocketVector) == 1){
				loopFlag = false;
				successfulGeneration = true;
			}
			for (unsigned int i = 0; i < rocketVector.size(); i++) {
				rocketVector[i].resetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
			}
		}
	}
	printf("\n\n\n");
	if (successfulGeneration) {
		printf("Success after %d generations!\n\n\n", numbOfLoops);
	}
	//clean up resources
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(workingWindow);
	SDL_Quit();
	
	system("pause");

	return 0;
}