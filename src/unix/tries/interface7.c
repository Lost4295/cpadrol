#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

bool init();
void kill();
bool loop();

// Pointers to our window, renderer, texture, music, and sound
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
Mix_Music* music;
Mix_Chunk* sound;

int main(int argc, char** args) {

	if ( !init() ) {
		system("pause");
		return 1;
	}

	while ( loop() ) {
		// wait before processing the next frame
		SDL_Delay(10); 
	}

	kill();
	return 0;
}

bool loop() {


	SDL_Event e;
	SDL_Rect dest;

	// Clear the window to white
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );

	// Event loop
	while ( SDL_PollEvent( &e ) != 0 ) {
		switch (e.type) {
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
				if ( e.key.keysym.sym == SDLK_SPACE ) {
					if ( Mix_PausedMusic() == 1 ) {
						Mix_ResumeMusic();
					} else {
						Mix_PauseMusic();
					}
				} else if ( e.key.keysym.sym == SDLK_ESCAPE ) {
					Mix_HaltMusic();
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				// Play sound once on the first available channel
				Mix_PlayChannel( -1, sound, 0 );
				break;
		}
	}

	// Render texture
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	// Update window
	SDL_RenderPresent( renderer );

	return true;
}

bool init() {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return false;
	} 

	// Initialize SDL_image with PNG loading subsystem
	if ( IMG_Init(IMG_INIT_PNG) < 0 ) {
		printf("Error initializing SDL_image: %s\n", IMG_GetError());
		system("pause");
		return false;
	}

	// Initialize SDL_mixer with our audio format
	if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) < 0 ) {
		printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
		return false;
	}

	window = SDL_CreateWindow( "Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
	if ( !window ) {
		printf("Error creating window: %s\n", SDL_GetError() );
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		printf("Error creating renderer: %s\n", SDL_GetError());
		return false;
	}

	// Load image (PNG) into surface
	SDL_Surface* buffer = IMG_Load("test.png");
	if ( !buffer ) {
		printf("Error loading image test.png: %s\n", SDL_GetError());
		return false;
	}

	// Create texture
	texture = SDL_CreateTextureFromSurface( renderer, buffer );
	// Free surface as it's no longer needed
	SDL_FreeSurface( buffer );
	buffer = NULL;
	if ( !texture ) {
		printf("Error creating texture: %s\n", SDL_GetError());
		return false;
	}

	// Load music
	music = Mix_LoadMUS("music.wav");
	if ( !music ) {
		printf("Error loading music: %s\n", Mix_GetError());
		return false;
	}

	// Load sound
	sound = Mix_LoadWAV("scratch.wav");
	if ( !sound ) {
		printf("Error loading sound: %s\n", Mix_GetError());
		return false;
	}

	// Play music forever
	Mix_PlayMusic( music, -1 );

	return true;
}

void kill() {
	SDL_DestroyTexture( texture );
	Mix_FreeMusic( music );
	Mix_FreeChunk( sound );
	texture = NULL;
	music = NULL;
	sound = NULL;

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}