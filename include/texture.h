#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "game.h"

namespace Texture {
    SDL_Texture *loadIMG(const char*);
    void drawText(const char*, SDL_Color, int, int, int);
}

#endif // TEXTURE_H_
