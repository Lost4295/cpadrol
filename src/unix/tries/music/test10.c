#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>

bool init();
void kill();
bool loop();
void printe(int i, SDL_Rect dest);

// Pointers to our window, renderer, texture, music, and sound
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
Mix_Music *music;
Mix_Chunk *sound;
TTF_Font *font;
SDL_Rect dest = {100, 440, 0, 0};

int main(int argc, char **args)
{

	if (!init())
	{
		system("pause");
		return 1;
	}

	while (loop())
	{
		// wait before processing the next frame
		SDL_Delay(10);
	}

	kill();
	return 0;
}

int i = 1;
bool loop()
{

	SDL_Event e;
	SDL_Rect dest;

	// Clear the window to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Event loop
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_SPACE)
			{
				if (Mix_PausedMusic() == 1)
				{
					Mix_ResumeMusic();
				}
				else
				{
					Mix_PauseMusic();
				}
			}
			else if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				Mix_HaltMusic();
			}
			else if (e.key.keysym.sym == SDLK_UP)
			{
				(i == 1) ? i = 6 : i--;
				printf("%d\n", i);
				printe(i, dest);
				launchmusic(i);
			}
			else if (e.key.keysym.sym == SDLK_DOWN)
			{
				(i == 6) ? i = 1 : i++;
				printf("%d\n", i);
				printe(i, dest);
				launchmusic(i);
			}
			else if (e.key.keysym.sym == SDLK_RETURN)
			{
				Mix_PlayMusic(music, -1);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			// Play sound once on the first available channel
			Mix_PlayChannel(-1, sound, 0);
			break;
		}
	}

	// Render texture
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	printe(i, dest);

	// Update window
	SDL_RenderPresent(renderer);

	return true;
}

void launchmusic(int i)
{
	char musique[12];
	char num[2];
	num[0] = i + '0';
	num[1] = '\0';
	strcpy(musique, "music");
	strcat(musique, num);
	strcat(musique, ".mp3");
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	music = Mix_LoadMUS(musique);
	Mix_PlayMusic(music, -1);
}
void printe(int i, SDL_Rect dest)
{
	SDL_Surface *text;
	char texte[100];
	char num[2];
	num[0] = i + '0';
	num[1] = '\0';
	strcpy(texte, "Now playing: music ");
	strcat(texte, num);
	SDL_Color col = {0, 0, 0};
	text = TTF_RenderText_Solid(font, texte, col);
	if (!text)
	{
		printf("Failed to render text: %s\n", TTF_GetError());
	}
	SDL_Texture *text_texture;
	SDL_Rect destn = {0, 0, text->w, text->h};
	destn.x = dest.x;
	destn.y = dest.y;
	text_texture = SDL_CreateTextureFromSurface(renderer, text);
	SDL_RenderCopy(renderer, text_texture, NULL, &destn);
}

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}

	// Initialize SDL_image with PNG loading subsystem
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		printf("Error initializing SDL_image: %s\n", IMG_GetError());
		system("pause");
		return false;
	}

	// Initialize SDL_mixer with our audio format
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
	{
		printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
		return false;
	}

	window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("Error creating window: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("Error creating renderer: %s\n", SDL_GetError());
		return false;
	}
	if (TTF_Init() < 0)
	{
		fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
		return -1;
	}
	font = TTF_OpenFont("../Roboto.ttf", 32);
	if (!font)
	{
		printf("Error loading font: %s\n", TTF_GetError());
		return -1;
	}
	// Load image (PNG) into surface
	SDL_Surface *buffer = IMG_Load("test.png");
	if (!buffer)
	{
		printf("Error loading image test.png: %s\n", SDL_GetError());
		return false;
	}

	// Create texture
	texture = SDL_CreateTextureFromSurface(renderer, buffer);
	// Free surface as it's no longer needed
	SDL_FreeSurface(buffer);
	buffer = NULL;
	if (!texture)
	{
		printf("Error creating texture: %s\n", SDL_GetError());
		return false;
	}

	// Load music
	music = Mix_LoadMUS("music1.mp3");
	if (!music)
	{
		printf("Error loading music: %s\n", Mix_GetError());
		return false;
	}

	// Load sound
	sound = Mix_LoadWAV("scratch.wav");
	if (!sound)
	{
		printf("Error loading sound: %s\n", Mix_GetError());
		return false;
	}

	// Play music forever
	Mix_PlayMusic(music, -1);

	return true;
}

void kill()
{
	SDL_DestroyTexture(texture);
	Mix_FreeMusic(music);
	Mix_FreeChunk(sound);
	texture = NULL;
	music = NULL;
	sound = NULL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}