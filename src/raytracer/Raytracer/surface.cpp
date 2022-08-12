#include "surface.hpp"
#include <cassert>
Surface::Surface(SDL_Surface* sdlSurface, bool freeOnDestruction)
{
    assert(sdlSurface != nullptr); // no null surfaces!
    // (means we don't have to check the inner pointer every time we e.g. blit).

    auto deleter = freeOnDestruction ?
        SDLSurfacePtrDeleterT([] (SDL_Surface* surface) { SDL_FreeSurface(surface); }) :
        SDLSurfacePtrDeleterT([] (SDL_Surface*) { });

    m_sdlSurfacePtr = SDLSurfacePtrT(sdlSurface, deleter);
}

SDL_Surface* Surface::GetSDLSurface() const
{
    assert(m_sdlSurfacePtr != nullptr); // check this surface hasn't been moved from
    // (if the m_sdlSurfacePtr is valid, the inner surface should be valid (or at least non-null) due to the assert in the constructor).

    return m_sdlSurfacePtr.get();
}