#include <iostream>
#include <signal.h>
#include <sys/stat.h>

#include <SDL.h>
#include <SDL_mixer.h>


bool exists(std::string filename)
{
	bool found = false;
	struct stat sb;
	found = stat(filename.c_str(), &sb) != -1;
	return found;

}

void quit(int s)
{
	exit(0);
	//cout << "Type 'q' to quite
}

int main(int argc, char* argv[])
{
	// Initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	std::cout << "Type Ctl-C to quit\n";
	struct sigaction hnd;
	hnd.sa_handler = quit;
	sigemptyset(&hnd.sa_mask);
	hnd.sa_flags = 0;
	sigaction(SIGINT, &hnd, NULL);

	int    audioRate = 22050;
	Uint16 audioFormat = AUDIO_S16SYS;
	int    audioChannels = 2;
	int    audioBuffers = 4096;

	// Open audio device.
	int res = Mix_OpenAudio( audioRate, audioFormat, audioChannels, 4096 );

	if( res == -1 )
	{
		const char* errorMsg =  Mix_GetError();

		std::cout << "Mix_OpenAudio(): failed - " << errorMsg << std::endl;
		return 0;
	}

	std::cout << "Mix_OpenAudio(): success." << std::endl;

	// Load a sound.
	std::string wav_file = "pink.wav";
	if(!exists(wav_file))  {
		wav_file = std::string(SHAREDIR) + wav_file;
	}
	//Mix_Chunk *sound = NULL;
	//sound = Mix_LoadWAV( "c:\\users\\public\\sound.wav" );
	Mix_Chunk *sound = Mix_LoadWAV( wav_file.c_str() );

	if(sound == NULL) 
	{
		std::cout << "Mix_LoadWAV(): failed" << std::endl;
		return 0;
	}

	std::cout << "Mix_LoadWAV(): success" << std::endl;

	//std::thread th(quit);

	// Attempt to play the sound.
	int channel;
	channel = Mix_PlayChannel( -1, sound, -1 );

	if(channel == -1) 
		std::cout << "Mix_PlayChannel(): failed" << std::endl;
	else
		std::cout << "Mix_PlayChannel(): success. channel: " << channel << std::endl;

	while(Mix_Playing(channel) != 0)
	{
	//	std::cout << "Mix_PlayChannel(): Playing" << std::endl;
	}

	Mix_FreeChunk(sound);

	std::cout << "Mix_FreeSound(): Done" << std::endl;

	while( 1);

	Mix_CloseAudio();

	return 0;
}
