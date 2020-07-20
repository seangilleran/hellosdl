#pragma once

#include <utility>
#include <SDL.h>

/*
 * Recurse through a list of SDL objects, cleaning the first in each iteration.
 * See: https://www.willusher.io/sdl2%20tutorials/2014/08/01/postscript-1-easy-cleanup
 */
template <typename T, typename... Args>
void cleanup(T *t, Args &&... args)
{
    cleanup(t);
    cleanup(std::forward<Args>(args)...);
}

/*
 * These specializations serve to free the passed argument and also provide the
 * base cases for the recursive call above, eg. when args is only a single item
 * one of the specializations below will be called by
 * cleanup(std::forward<Args>(args)...), ending the recursion.
 * We also make it safe to pass nullptrs to handle situations where we
 * don't want to bother finding out which values failed to load (and thus are
 * null) but rather just want to clean everything up and let cleanup sort it
 * out.
 */
template <>
inline void cleanup<SDL_Window>(SDL_Window *window)
{
    if (!window)
    {
        return;
    }
    SDL_DestroyWindow(window);
}

template <>
inline void cleanup<SDL_Renderer>(SDL_Renderer *renderer)
{
    if (!renderer)
    {
        return;
    }
    SDL_DestroyRenderer(renderer);
}

template <>
inline void cleanup<SDL_Texture>(SDL_Texture *texture)
{
    if (!texture)
    {
        return;
    }
    SDL_DestroyTexture(texture);
}

template <>
inline void cleanup<SDL_Surface>(SDL_Surface *surface)
{
    if (!surface)
    {
        return;
    }
    SDL_FreeSurface(surface);
}