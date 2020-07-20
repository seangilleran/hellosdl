#include <iostream>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

int main()
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        logSDLError(std::cerr, "SDL_Init");
        return 1;
    }

    // Create window.
    SDL_Window *window = SDL_CreateWindow(
        "HelloSDL",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        logSDLError(std::cerr, "SDL_CreateWindow");
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
        logSDLError(std::cerr, "SDL_CreateRenderer");
        SDL_Quit();
        return 1;
    }

    // Load bitmap.
    std::string imagePath = "..\\content\\hello.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr)
    {
        cleanup(renderer, window);
        logSDLError(std::cerr, "SDL_LoadBMP");
        SDL_Quit();
        return 1;
    }

    // Upload bitmap to renderer.
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (texture == nullptr)
    {
        cleanup(renderer, window);
        logSDLError(std::cerr, "SDL_LoadTextureFromSurface");
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