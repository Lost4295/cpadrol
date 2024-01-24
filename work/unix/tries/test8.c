
#include <SDL2/SDL.h>
#include <stdbool.h>

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);

int width =100;
int height =100;
    SDL_Window *win = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    /* Draw red window */
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend); // Completely red

    SDL_Delay(3000);

    /* Increase window size and draw it green */
    SDL_SetWindowSize(win, 800, 800);
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend); // Partially green

    SDL_Delay(3000);

    /* Make window blue */
    SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend); // Completely blue

    SDL_Delay(3000);
    bool resized = false;

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while (!quit)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            {quit = SDL_TRUE;}
        else if (event.type == SDL_WINDOWEVENT_RESIZED || event.type == SDL_WINDOWEVENT_SIZE_CHANGED || event.type == SDL_WINDOWEVENT_MAXIMIZED || event.type == SDL_WINDOWEVENT_RESTORED
                 || event.type == SDL_WINDOWEVENT_SHOWN || event.type == SDL_WINDOWEVENT_EXPOSED  || event.type == SDL_WINDOWEVENT_ENTER || event.type == SDL_WINDOWEVENT_LEAVE
                 || event.type == SDL_WINDOWEVENT_FOCUS_GAINED || event.type == SDL_WINDOWEVENT_FOCUS_LOST || event.type == SDL_WINDOWEVENT_CLOSE || event.type == SDL_WINDOWEVENT_TAKE_FOCUS)
        {
            resized = true;
            printf("Window %d resized to %dx%d\n", event.window.windowID, event.window.data1, event.window.data2);
        }

        if (resized)
        {
            SDL_Surface *image = SDL_LoadBMP("test.bmp");
            SDL_Surface *surface;
            resized = false;
            SDL_GetWindowSize(win, &width, &height);
            surface = SDL_GetWindowSurface(win);
            SDL_BlitSurface(image, NULL, surface, NULL);
            SDL_UpdateWindowSurface(win);
        }
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}