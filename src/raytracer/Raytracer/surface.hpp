#pragma once
#include <functional>
#include <memory>
#include "SDL.h"

// https://codereview.stackexchange.com/a/202032
class Surface
{
public:

    Surface(SDL_Surface* sdlSurface, bool freeOnDestruction);

    Surface(Surface&&) = default;
    Surface& operator=(Surface&&) = default;

    Surface(Surface const&) = delete;
    Surface& operator=(Surface const&) = delete;

    SDL_Surface* GetSDLSurface() const; // Blit / Fill etc. should use this to get the SDL_Surface.

private:

    using SDLSurfacePtrDeleterT = std::function<void(SDL_Surface*)>;
    using SDLSurfacePtrT = std::unique_ptr<SDL_Surface, SDLSurfacePtrDeleterT>;
    SDLSurfacePtrT m_sdlSurfacePtr;
};

