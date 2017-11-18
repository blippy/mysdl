#include <cassert>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

using std::cout;
using std::endl;

void play_beep(Mix_Music* music, int times = 1)
{
	//cout << "times=" << times << "\n";
	if(Mix_PlayMusic(music, times) == -1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
		exit(1);
	}
}

static bool keep_beeping = true;

void beeper(Mix_Music *music)
{
	while(keep_beeping) {
		play_beep(music);
		std::this_thread::sleep_for(5s);
	}
}

int
main()
{
	SDL_Window *w;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		return 1;
	}


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

	clock_t begin = std::clock();

	Mix_Music *beep;
	//beep = Mix_LoadMUS("beep-07.wav");
	beep = Mix_LoadMUS("pluck.wav");
	if(!beep) {
		printf("Mix_LoadMUS:beep-07.wav:%s\n", Mix_GetError());
		return 1;
	}

	std::thread t1(beeper, beep);

	SDL_Event event;
	bool quit = false;
	while(!quit) {
		clock_t now = clock();
		constexpr auto mins = 30;
		double elapsed_mins = double(now-begin) / CLOCKS_PER_SEC / 60;
		if(elapsed_mins>mins) quit = true;

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
	keep_beeping = false;
	t1.join();
	play_beep(beep, 10);
	while(Mix_PlayingMusic()) std::this_thread::sleep_for(1s);

	Mix_FreeMusic(beep);
	Mix_CloseAudio();
	SDL_DestroyWindow(w);
	SDL_Quit();

	return 0;
}
