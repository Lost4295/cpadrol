#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef struct
{
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
} LTexture;

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

LTexture gPromptTextTexture;
LTexture gInputTextTexture;

void LTexture_free(LTexture* texture)
{
	if (texture->mTexture != NULL)
	{
		SDL_DestroyTexture(texture->mTexture);
		texture->mTexture = NULL;
		texture->mWidth = 0;
		texture->mHeight = 0;
	}
}

bool LTexture_loadFromFile(LTexture* texture, const char* path)
{
	LTexture_free(texture);

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		}
		else
		{
			texture->mWidth = loadedSurface->w;
			texture->mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}

	texture->mTexture = newTexture;
	return texture->mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture_loadFromRenderedText(LTexture* texture, const char* textureText, SDL_Color textColor)
{
	LTexture_free(texture);

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);
	if (textSurface != NULL)
	{
		texture->mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (texture->mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			texture->mWidth = textSurface->w;
			texture->mHeight = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	return texture->mTexture != NULL;
}
#endif

int LTexture_getWidth(LTexture* texture)
{
	return texture->mWidth;
}

int LTexture_getHeight(LTexture* texture)
{
	return texture->mHeight;
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	gFont = TTF_OpenFont("32_text_input_and_clipboard_handling/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0, 0xFF };
		if (!LTexture_loadFromRenderedText(&gPromptTextTexture, "Enter Text:", textColor))
		{
			printf("Failed to render prompt text!\n");
			success = false;
		}
	}

	return success;
}

void close()
{
	LTexture_free(&gPromptTextTexture);
	LTexture_free(&gInputTextTexture);

	TTF_CloseFont(gFont);
	gFont = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			SDL_Color textColor = { 0, 0, 0, 0xFF };
			char inputText[100] = "Some Text";
			LTexture_loadFromRenderedText(&gInputTextTexture, inputText, textColor);
			SDL_StartTextInput();

			while (!quit)
			{
				bool renderText = false;

				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0)
						{
							inputText[strlen(inputText) - 1] = '\0';
							renderText = true;
						}
						else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
						{
							SDL_SetClipboardText(inputText);
						}
						else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
						{
							char* tempText = SDL_GetClipboardText();
							strcpy(inputText, tempText);
							SDL_free(tempText);
							renderText = true;
						}
					}
					else if (e.type == SDL_TEXTINPUT)
					{
						if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
						{
							strcat(inputText, e.text.text);
							renderText = true;
						}
					}
				}

				if (renderText)
				{
					if (strlen(inputText) > 0)
					{
						LTexture_loadFromRenderedText(&gInputTextTexture, inputText, textColor);
					}
					else
					{
						LTexture_loadFromRenderedText(&gInputTextTexture, " ", textColor);
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				SDL_Rect promptRect = { (SCREEN_WIDTH - LTexture_getWidth(&gPromptTextTexture)) / 2, 0, LTexture_getWidth(&gPromptTextTexture), LTexture_getHeight(&gPromptTextTexture) };
				SDL_Rect inputRect = { (SCREEN_WIDTH - LTexture_getWidth(&gInputTextTexture)) / 2, LTexture_getHeight(&gPromptTextTexture), LTexture_getWidth(&gInputTextTexture), LTexture_getHeight(&gInputTextTexture) };

				SDL_RenderCopy(gRenderer, gPromptTextTexture.mTexture, NULL, &promptRect);
				SDL_RenderCopy(gRenderer, gInputTextTexture.mTexture, NULL, &inputRect);

				SDL_RenderPresent(gRenderer);
			}

			SDL_StopTextInput();
		}
	}

	close();
	return 0;
}


