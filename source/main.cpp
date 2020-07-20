#include <iostream>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
 * Log SDL_GetError() to specified output stream.
 */
void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
 * Load texture from a BMP file.
 */
SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *renderer)
{
    std::string filename = "..\\content\\" + file;
    SDL_Texture *texture = IMG_LoadTexture(renderer, filename.c_str());
    if (texture == nullptr)
    {
        logSDLError(std::cerr, "IMG_LoadTexture");
        return texture;
    }

    std::cout << "Loaded texture: " << file << std::endl;
    return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving the
* texture's width and height
*/
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
    SDL_RenderCopy(renderer, texture, NULL, &destination);
}


/**
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
    SDL_Texture *texture = loadTexture("hello.bmp", renderer);
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
        renderTexture(texture, renderer, 170, 90);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    // Clean up.
    cleanup(texture, renderer, window);
    SDL_Quit();
    return 0;
}