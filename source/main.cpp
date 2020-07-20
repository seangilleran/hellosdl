#include <iostream>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "cleanup.h"

using std::cerr, std::endl, std::string;

int main()
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Create window.
    SDL_Window *window = SDL_CreateWindow(
        "HelloSDL",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer.
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (renderer == nullptr)
    {
        cleanup(window);
        cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    // Load bitmap.
    string imagePath = "..\\content\\hello.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr)
    {
        cleanup(renderer, window);
        cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    // Upload bitmap to renderer.
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (texture == nullptr)
    {
        cleanup(renderer, window);
        cerr << "SDL_CreateTextureFromSurfaceError: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    // Draw texture to window.
    for (auto i = 0; i < 3; ++i)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    // Clean up.
    cleanup(texture, renderer, window);
    SDL_Quit();
    return 0;
}