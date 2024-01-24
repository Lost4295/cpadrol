#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_GameController *findController() {
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            return SDL_GameControllerOpen(i);
        }
    }

    return NULL;
}

SDL_JoystickID getControllerInstanceID(SDL_GameController *controller) {
    return SDL_JoystickInstanceID(
            SDL_GameControllerGetJoystick(controller));
}


void renderCross1(SDL_Surface *screenSurface, int x, int y, int w, Uint32 color) {
    SDL_Rect rect = {x, y - w / 2, 1, w};
    SDL_FillRect(screenSurface, &rect, color);

    rect.x= x - w / 2;
    rect.y = y;
    rect.w = w;
    rect.h=  1;
    SDL_FillRect(screenSurface, &rect, color);
}


void renderCross(SDL_Surface *screenSurface , int x, int y, float x1, float x2) {
    renderCross1(screenSurface, x, y, 20, SDL_MapRGB(screenSurface->format, 0x99, 0x99, 0x99));
    renderCross1(screenSurface, x + x1 * SCREEN_WIDTH * 0.2f, y + x2 * SCREEN_HEIGHT * 0.2f,
            10, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xAA));
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
            "hello_sdl2",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("SDL could not initialize! SDL Error: ", SDL_GetError());
        return 1;
    }

    SDL_bool quit = SDL_FALSE;
    SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

    // Note that this code only allows a single active controller at the time, but it would
    // be easy to modify to keep track of multiple controllers.

    // SDL_CONTROLLERDEVICEADDED is not triggered when starting up, so check for controllers now.
    SDL_GameController *controller = findController();

    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent( &event ) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                quit = SDL_TRUE
                ;
                break;
            case SDL_CONTROLLERDEVICEADDED:
                if (!controller) {
                    controller = SDL_GameControllerOpen(event.cdevice.which);
                }
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                if (controller && event.cdevice.which == getControllerInstanceID(controller)) {
                    SDL_GameControllerClose(controller);
                    controller = findController();
                }
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                if (controller && event.cdevice.which == getControllerInstanceID(controller)) {
                    switch (event.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_X:
                        printf("X pressed!");
                        break;
                    }
                }
                break;
            }
        }

        SDL_bool pressed = controller && SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X);

        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x33, 0x33, pressed ? 0xFF : 0x33));

        if (controller) {
            float x = (float) SDL_GameControllerGetAxis(controller,
                SDL_CONTROLLER_AXIS_LEFTX) / (float) INT16_MAX;
            float y = (float) SDL_GameControllerGetAxis(controller,
                SDL_CONTROLLER_AXIS_LEFTY) / (float) INT16_MAX;
            renderCross(screenSurface, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, x, y);
            printf("%f %f            ", x, y);

            x = (float) SDL_GameControllerGetAxis(controller,
                    SDL_CONTROLLER_AXIS_RIGHTX) / (float) INT16_MAX;
            y = (float) SDL_GameControllerGetAxis(controller,
                    SDL_CONTROLLER_AXIS_RIGHTY) / (float) INT16_MAX;
            renderCross(screenSurface, 3 * SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, x, y);
            printf("%f %f\n", x, y);

            if (x < -0.5f) {
                printf("left\n");
                SDL_PushEvent(&(SDL_Event){.type = SDL_KEYDOWN, .key = {.keysym = {.sym = SDLK_LEFT}}});
            } else if (x > 0.5f) {
                printf("right\n");
                SDL_PushEvent(&(SDL_Event){.type = SDL_KEYDOWN, .key = {.keysym = {.sym = SDLK_RIGHT}}});
            } else if (y < -0.5f) {
                printf("up\n");
                SDL_PushEvent(&(SDL_Event){.type = SDL_KEYDOWN, .key = {.keysym = {.sym = SDLK_UP}}});
            } else if (y > 0.5f) {
                printf("down\n");
                SDL_PushEvent(&(SDL_Event){.type = SDL_KEYDOWN, .key = {.keysym = {.sym = SDLK_DOWN}}});
            }
        }

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
