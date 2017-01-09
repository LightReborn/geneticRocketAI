#include "stdio.h"
#include "Rocket.h"

#include "SDL.h"
#include "SDL_image.h"

#define WINDOW_HEIGHT (640)
#define WINDOW_WIDTH (480)

//speed in pixels per second
#define NUMBER_OF_ROCKETS (25)

bool allDNAExecuted(std::vector<Rocket> rockets) {

	for (int i = 0; i < NUMBER_OF_ROCKETS; i++) {
		if (!rockets[i].DNAIsExecuted()) {
			return false;
		}
	}
	return true;
}

//todo
int getHighestFitnessIndex(std::vector<float> fitnessVector) {
	return 0;
}

//todo
std::vector<float> determineFitness(std::vector<Rocket> rockets) {
	std::vector<float> fitnessVector;
	return fitnessVector;
}

//todo
std::vector<Rocket> breed(std::vector<Rocket> rockets) {
	/*	brainstorm:
	*	pick the top 10-20% most fit objects.
	*	place all the picked one into a new rocket vector
	*	The best breeds with all other rockets. (if there is enough size.)
	*	then randomly breed index 1 through n until the vector fills up (NUMBER_OF_ROCKETS)
	*/
	std::vector<float> fitnessVector = determineFitness(rockets);
	return rockets;
}

#undef main
int main(void) {
	bool loopFlag = true;
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

	printf("Entering display loop\n");
	//render the rockets at their starting positions.
	SDL_RenderClear(rend);

	for (auto currentRect : rectVector) {
		SDL_RenderCopy(rend, tex, NULL, &currentRect);
	}
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
		for (int i = 0; i < rocketVector.size(); i++) {
			rocketVector[i].stepDNA();
			printf("Main: (%f, %f)\n", rocketVector[i].getX(), rocketVector[i].getY());
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
		//SDL_RenderCopy(rend, tex, NULL, NULL);
		SDL_RenderPresent(rend);
		numbOfLoops++;

		if (allDNAExecuted(rocketVector)) {
			rocketVector = breed(rocketVector);
		}
	}

	//clean up resources
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(workingWindow);
	SDL_Quit();
	
	for (auto rocket : rocketVector) {
		printf("(%f, %f)\n", rocket.getX(), rocket.getY());
	}
	printf("Loop executions: %d\n", numbOfLoops);
	system("pause");

	return 0;
}