#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>

using std::cout, std::endl;

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Quit();
    return 0;
}