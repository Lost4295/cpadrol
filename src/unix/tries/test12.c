#include <SDL2/SDL.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#if defined(__GNUC__) || defined(__clang__)
static void panic_and_abort(const char *title, const char *text) __attribute__((noreturn));
#endif

static void panic_and_abort(const char *title, const char *text)
{
    fprintf(stderr, "PANIC: %s ... %s\n", title, text);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, text, window);
    SDL_Quit();
    exit(1);
}

static float volume_slider_value = 1.0f;
static float volume_slider_value2 = 1.0f;
static float volume_slider_value3 = 1.0f;

int main(int argc, char **argv)
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
    {
        panic_and_abort("SDL_Init failed", SDL_GetError());
    }

    window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if (!window)
    {
        panic_and_abort("SDL_CreateWindow failed", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        panic_and_abort("SDL_CreateRenderer failed", SDL_GetError());
    }

    SDL_bool paused = SDL_TRUE;
    const SDL_Rect volume_rect = {(640 - 500) / 2, 400, 500, 20};
    const SDL_Rect volume_rect2 = {(640 - 500) / 2, 300, 500, 20};
    const SDL_Rect volume_rect3 = {(640 - 500) / 2, 200, 500, 20};

    SDL_Rect volume_knob;
    volume_knob.y = volume_rect.y;
    volume_knob.h = volume_rect.h;
    volume_knob.w = 20;
    volume_knob.x = (volume_rect.x + volume_rect.w) - volume_knob.w;

    SDL_Rect volume_knob2;
    volume_knob2.y = volume_rect2.y;
    volume_knob2.h = volume_rect2.h;
    volume_knob2.w = 20;
    volume_knob2.x = (volume_rect2.x + volume_rect2.w) - volume_knob2.w;

    SDL_Rect volume_knob3;
    volume_knob3.y = volume_rect3.y;
    volume_knob3.h = volume_rect3.h;
    volume_knob3.w = 20;
    volume_knob3.x = (volume_rect3.x + volume_rect3.w) - volume_knob3.w;

    int green = 0;
    int red = 0;
    int blue = 0;
    SDL_bool keep_going = SDL_TRUE;
    while (keep_going)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                keep_going = SDL_FALSE;
                break;

            case SDL_MOUSEBUTTONDOWN:
            {
                //                    const SDL_Point pt = { e.button.x, e.button.y };
                //                    if (SDL_PointInRect(&pt, &rewind_rect)) {  // pressed the "rewind" button
                //                        SDL_ClearQueuedAudio(audio_device);
                //                        SDL_AudioStreamClear(stream);
                //                        if (SDL_AudioStreamPut(stream, wavbuf, wavlen) == -1) {  // !!! FIXME: graceful handling.
                //                            panic_and_abort("Audio stream put failed", SDL_GetError());
                //                        }
                //                        SDL_AudioStreamFlush(stream);  // !!! FIXME: error handling
                //                    } else if (SDL_PointInRect(&pt, &pause_rect)) {  // pressed the "pause" button
                //                        paused = paused ? SDL_FALSE : SDL_TRUE;
                //                        SDL_PauseAudioDevice(audio_device, paused);
                //                    }
                break;
            }

            case SDL_MOUSEMOTION:
            {
                const SDL_Point pt = {e.motion.x, e.motion.y};
                if (SDL_PointInRect(&pt, &volume_rect) && (e.motion.state & SDL_BUTTON_LMASK))
                { // mouse is pressed inside the "volume" "slider"?
                    const float fx = (float)(pt.x - volume_rect.x);
                    volume_slider_value = (fx / ((float)volume_rect.w)); // a value between 0.0f and 1.0f
                    printf("SLIDING 1! At %dx%d (%d percent)\n", pt.x, pt.y, (int)SDL_round(volume_slider_value * 100.0f));
                    volume_knob.x = pt.x - (volume_knob.w / 2);
                    volume_knob.x = SDL_max(volume_knob.x, volume_rect.x);
                    volume_knob.x = SDL_min(volume_knob.x, (volume_rect.x + volume_rect.w) - volume_knob.w);
                }
                else if (SDL_PointInRect(&pt, &volume_rect2) && (e.motion.state & SDL_BUTTON_LMASK))
                { // mouse is pressed inside the "volume" "slider"?
                    const float fx = (float)(pt.x - volume_rect2.x);
                    volume_slider_value2 = (fx / ((float)volume_rect2.w)); // a value between 0.0f and 1.0f
                    printf("SLIDING 2 ! At %dx%d (%d percent)\n", pt.x, pt.y, (int)SDL_round(volume_slider_value2 * 100.0f));
                    volume_knob2.x = pt.x - (volume_knob2.w / 2);
                    volume_knob2.x = SDL_max(volume_knob2.x, volume_rect2.x);
                    volume_knob2.x = SDL_min(volume_knob2.x, (volume_rect2.x + volume_rect2.w) - volume_knob2.w);
                }
                else if (SDL_PointInRect(&pt, &volume_rect3) && (e.motion.state & SDL_BUTTON_LMASK))
                { // mouse is pressed inside the "volume" "slider"?
                    const float fx = (float)(pt.x - volume_rect3.x);
                    volume_slider_value3 = (fx / ((float)volume_rect3.w)); // a value between 0.0f and 1.0f
                    printf("SLIDING 3! At %dx%d (%d percent)\n", pt.x, pt.y, (int)SDL_round(volume_slider_value3 * 100.0f));
                    volume_knob3.x = pt.x - (volume_knob3.w / 2);
                    volume_knob3.x = SDL_max(volume_knob3.x, volume_rect3.x);
                    volume_knob3.x = SDL_min(volume_knob3.x, (volume_rect3.x + volume_rect3.w) - volume_knob3.w);
                }
                break;
            }

            case SDL_DROPFILE: // à reprendre
            {
                //open_new_audio_file(e.drop.file);
                //SDL_free(e.drop.file);
                break;
            }
            }
        }

        SDL_SetRenderDrawColor(renderer, blue, green, red, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderFillRect(renderer, &volume_rect);
        SDL_RenderFillRect(renderer, &volume_rect2);
        SDL_RenderFillRect(renderer, &volume_rect3);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &volume_knob);
        SDL_RenderFillRect(renderer, &volume_knob2);
        SDL_RenderFillRect(renderer, &volume_knob3);

        SDL_RenderPresent(renderer);

        green = convertvalue((int)SDL_round(volume_slider_value * 100.0f));
        red =convertvalue((int)SDL_round(volume_slider_value2 * 100.0f));
        blue =convertvalue((int)SDL_round(volume_slider_value3 * 100.0f));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
int convertvalue(int oldValue) {
    return oldValue * 255 / 100;
}