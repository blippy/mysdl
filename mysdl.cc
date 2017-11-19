#include <cassert>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <thread>
#include <chrono>
#include <sys/stat.h>
using namespace std::chrono_literals;

using std::cout;
using std::endl;

bool exists(std::string filename)
{
	bool found = false;
	struct stat sb;
	/*
	if (stat(filename.c_str(), &sb) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}

	exists = (sb.st_mode & S_IFMT) == S_IFREG;
	*/
	found = stat(filename.c_str(), &sb) != -1;
	return found;

}

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
	constexpr bool use_video = false;

	auto init_flags = SDL_INIT_AUDIO;
	if(use_video) init_flags |= SDL_INIT_VIDEO;

	SDL_Window *w = nullptr;
	if(SDL_Init(init_flags) == -1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		return 1;
	}


	if(use_video) {
		constexpr auto winflags = 0;
		w = SDL_CreateWindow("mySDL", SDL_WINDOWPOS_UNDEFINED, 
				SDL_WINDOWPOS_UNDEFINED, 640, 480, winflags);
		if(w == nullptr) {
			printf("Could not create window: %s\n", SDL_GetError());
			return 1;
		}
	}

	//puts("Type 'q' to quit");

	clock_t begin = std::clock();

	std::string pluck_wav = "pluck.wav";
	//cout << "Datadir:" << SHAREDIR << "\n";
	if(! exists(pluck_wav)) {
			pluck_wav = std::string(SHAREDIR) + "/pluck.wav";
	}
	cout << "Wav:" << pluck_wav << "\n";

	Mix_Music* beep = Mix_LoadMUS(pluck_wav.c_str());
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
		//constexpr auto mins = 0.25;
		double elapsed_mins = double(now-begin) / CLOCKS_PER_SEC / 60;
		if(elapsed_mins>mins) quit = true;

		if(use_video) {
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
	}

	keep_beeping = false;
	t1.join();
	play_beep(beep, 10);
	while(Mix_PlayingMusic()) std::this_thread::sleep_for(1s);

	Mix_FreeMusic(beep);
	Mix_CloseAudio();
	if(use_video) SDL_DestroyWindow(w);
	SDL_Quit();

	return 0;
}
