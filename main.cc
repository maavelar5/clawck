#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ctime>

#include "clock.xpm"

SDL_Rect get_sprite (char c)
{
    int index = c - 48;

    return { index * 8, 0, 8, 8 };
}

int main (int argc, char **argv)
{
    SDL_Init (SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow (
        "clawck", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 176, 24,
        SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE
            | SDL_WINDOW_ALWAYS_ON_TOP);

    SDL_Renderer *renderer
        = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetLogicalSize (renderer, 176, 24);

    bool      run = true;
    SDL_Event event;

    SDL_SetWindowOpacity (window, .8f);

    SDL_Surface *surface = IMG_ReadXPMFromArray (clock_xpm);
    SDL_Texture *texture = SDL_CreateTextureFromSurface (renderer, surface);

    time_t     rawtime;
    struct tm *timeinfo;
    char       buffer[80];

    unsigned int current = 0;

    while (run)
    {
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT)
            {
                run = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_q: run = false; break;
                }
            }
        }

        if (SDL_GetTicks () - current >= 1000 || current == 0)
        {
            SDL_SetRenderDrawColor (renderer, 0, 0, 0, 255);
            SDL_RenderClear (renderer);

            SDL_Rect d = { 0, 0, 24, 24 };
            SDL_Rect r;

            time (&rawtime);
            timeinfo = localtime (&rawtime);

            strftime (buffer, 80, "%T", timeinfo);

            for (int x = 0; x < strlen (buffer); x++)
            {
                if (buffer[x] == ':')
                {
                    r = { 0, 8, 8, 8 };
                }
                else
                {
                    r = get_sprite (buffer[x]);
                }

                SDL_RenderCopy (renderer, texture, &r, &d);

                d.x += 20;
            }

            current = SDL_GetTicks ();
        }

        SDL_RenderPresent (renderer);
    }

    return 0;
}
