#include <iostream>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/*
 * Log SDL_GetError() to specified output stream.
 */
void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

/*
 * Load texture from a BMP file.
 * @param file BMP file to load.
 * @param renderer Renderer to load texture onto.
 * @return Loaded texture, or nullptr if something went wrong.
 */
SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *renderer)
{
    SDL_Texture *texture = nullptr;

    std::string filename = "..\\content\\" + file;
    SDL_Surface *bmp = SDL_LoadBMP(filename.c_str());
    if (bmp == nullptr)
    {
        logSDLError(std::cerr, "SDL_LoadBMP");
        return texture;
    }

    texture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (texture == nullptr)
    {
        logSDLError(std::cerr, "SDL_CreateTextureFromRenderer");
        return texture;
    }

    std::cout << "Loaded texture: " << file << std::endl;
    return texture;
}

/*
 * Application entry point.
 */
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
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        cleanup(window);
        logSDLError(std::cerr, "SDL_CreateRenderer");
        SDL_Quit();
        return 1;
    }

    // Load bitmap.
    std::string bmpFile = "hello.bmp";
    SDL_Texture *texture = loadTexture(bmpFile, renderer);
    if (texture == nullptr)
    {
        cleanup(renderer, window);
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