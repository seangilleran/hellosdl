#include <iostream>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

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
 * Tile texture to fit screen.
 */
void tileTexture(SDL_Texture *texture, SDL_Renderer *renderer)
{
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    renderTexture(texture, renderer, 0, 0);
    renderTexture(texture, renderer, width, 0);
    renderTexture(texture, renderer, 0, height);
    renderTexture(texture, renderer, width, height);
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
    SDL_Texture *background = loadTexture("hello2.bmp", renderer);
    SDL_Texture *texture = loadTexture("hello.bmp", renderer);
    if (background == nullptr || texture == nullptr)
    {
        cleanup(renderer, window);
        SDL_Quit();
        return 1;
    }

    // Draw texture to window.
    for (auto i = 0; i < 3; ++i)
    {
        SDL_RenderClear(renderer);
        tileTexture(background, renderer);
        renderTexture(texture, renderer, 170, 90);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    // Clean up.
    cleanup(background, texture, renderer, window);
    SDL_Quit();
    return 0;
}