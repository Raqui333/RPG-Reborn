#include "../include/texture.h"

SDL_Texture *Texture::loadIMG(const char *file)
{
    SDL_Surface *tmp_surface = IMG_Load(file);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
    return texture;
}

void Texture::drawText(const char *text, SDL_Color color, int size, int x, int y)
{
    TTF_Font *tmp_font = TTF_OpenFont("assets/Pixeled.ttf", size * 8);
    SDL_Surface *tmp_surface = TTF_RenderText_Solid(tmp_font, text, color);

    SDL_Rect text_size = {x, y, (int)(strlen(text) * size), size * 2};
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(Game::renderer, tmp_surface);

    SDL_FreeSurface(tmp_surface);
    TTF_CloseFont(tmp_font);

    SDL_RenderCopy(Game::renderer, text_texture, NULL, &text_size);

    SDL_DestroyTexture(text_texture);
}
