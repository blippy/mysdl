#include <cassert>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>

using std::cout;
using std::endl;

int
main()
{
	SDL_Window *w;
	SDL_Init(SDL_INIT_VIDEO);

	//constexpr auto winflags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS;
	//constexpr auto winflags = SDL_WINDOW_BORDERLESS;
	constexpr auto winflags = 0;
	w = SDL_CreateWindow("mySDL", SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 640, 480, winflags);
	if(w == nullptr) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	puts("Type 'q' to quit");

	Mix_Music *beep;
	beep = Mix_LoadMUS("beep-07.wav");
	if(!beep) {
		printf("Mix_LoadMUS:beep-07.wav:%s\n", Mix_GetError());
		return 1;
	}


	SDL_Event event;
	bool quit = false;
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {			
				case SDL_KEYDOWN: 
					{
						auto k = event.key.keysym.scancode;
						//cout << k << endl;
						if(k == SDL_SCANCODE_Q) quit = true;
					}
					break;
				default:
					break;
			}
		}
	}

	//SDL_Delay(3000);
	SDL_DestroyWindow(w);
	SDL_Quit();

	return 0;
}
