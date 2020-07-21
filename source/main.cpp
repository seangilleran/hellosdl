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
    std::string path = "..\\content\\" + file;
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr)
    {
        logSDLError(std::cerr, "IMG_LoadTexture");
        return texture;
    }

    std::cout << "IMG_LoadTexture (" << path << ") ok!" << std::endl;
    return texture;
}

/**
* Render SDL_Texture.
*/
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int width, int height)
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = width;
    destination.h = height;

    SDL_RenderCopy(renderer, texture, NULL, &destination);
}

/**
* Render SDL_Texture with original width and height intact.
*/
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    renderTexture(texture, renderer, x, y, width, height);
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
    std::cout << "SDL_Init ok!" << std::endl;

    // Initialize SDL_image.
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        logSDLError(std::cerr, "IMG_Init");
        SDL_Quit();
        return 1;
    }
    std::cout << "IMG_Init ok!" << std::endl;

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
    std::cout << "SDL_CreateWindow ok!" << std::endl;

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
    std::cout << "SDL_CreateRenderer ok!" << std::endl;

    // Load bitmap.
    SDL_Texture *texture = loadTexture("hello.bmp", renderer);
    SDL_Texture *texture2 = loadTexture("hello3.png", renderer);
    if (texture == nullptr || texture2 == nullptr)
    {
        cleanup(renderer, window);
        SDL_Quit();
        return 1;
    }

    // Event loop.
    SDL_Event e;
    bool isRunning = true;
    while (isRunning)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (e.type == SDL_KEYDOWN)
            {
                isRunning = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                isRunning = false;
            }
        }

        SDL_RenderClear(renderer);
        renderTexture(texture, renderer, 170, 90);
        renderTexture(texture2, renderer, 170, 90);
        SDL_RenderPresent(renderer);
    }

    // Clean up.
    cleanup(texture, texture2, renderer, window);
    SDL_Quit();
    return 0;
}