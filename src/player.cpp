#include "../include/player.h"

Player::Player(const char *file, int w, int h)
: velocity(2)
{
    player_texture = Texture::loadIMG(file);

    FPS = 9;

    UP = 0; LEFT = FPS * 3; DOWN = FPS * 6; RIGHT = FPS * 9;

    pos = npos = {384, 320, w, h};
    is_in_action = moving = false;

    moveX = moveY = next_frame = 0;
    current_frame = DOWN;

    sqmX = pos.x;
    sqmY = pos.y;

    camera = {0, 0, 800, 608};

    key_lock = false;

    level = 1;
    strength = 10;

    hp = {100, 100};
    mp = {100, 100};
    exp = {100, 0};

    dungeon = {false, 0};

    regen_tick = SDL_GetTicks();
}

Player::~Player()
{
    SDL_DestroyTexture(player_texture);
}

SDL_Rect Player::getpos()
{
    return pos;
}

void Player::setpos(int new_x, int new_y)
{
    sqmX = pos.x = new_x;
    sqmY = pos.y = new_y;
}

bool Player::isInAction()
{
    return is_in_action;
}

SDL_Rect Player::getcam()
{
    return camera;
}

void Player::anime_sprite(int direction)
{
    if (next_frame >= FPS * 3 - 1)
        next_frame = 0;
    else if (moveX != 0 || moveY != 0)
        ++next_frame;

    current_frame = direction + next_frame;
}

void Player::render()
{
    //   x , y  , w  , h
    SDL_Rect clip[12] = {
        {1,  1 , 32, 32}, {34,  1 , 32, 32}, {67,  1 , 32, 32},
        {1, 34 , 32, 32}, {34, 34 , 32, 32}, {67, 34 , 32, 32},
        {1, 67 , 32, 32}, {34, 67 , 32, 32}, {67, 67 , 32, 32},
        {1, 100, 32, 32}, {34, 100, 32, 32}, {67, 100, 32, 32},
    };

    SDL_RenderCopy(Game::renderer, player_texture, &clip[current_frame / FPS], &npos);
}

void Player::events(const Uint8 *state)
{
    if (!key_lock) {
        if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
            moveY -= velocity;
        else if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
            moveX -= velocity;
        else if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
            moveY += velocity;
        else if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
            moveX += velocity;
    }
}

void Player::move(std::vector<std::pair<int,int>> colliders)
{
    enum { UP, LEFT, DOWN, RIGHT };
    int next_sqm, prev_sqm = 0;
    moving = false;

    auto movement = [this, &next_sqm, &prev_sqm, &colliders] (int &axis, int &direction, int &sqm, int size, int type) {
        next_sqm = sqm + 32;
        prev_sqm = sqm - 32;

        for (auto& tile : colliders) {
            switch (type) {
                case UP:
                    if (prev_sqm == tile.second && pos.x == tile.first) direction = 0;
                    break;
                case LEFT:
                    if (prev_sqm == tile.first && pos.y == tile.second) direction = 0;
                    break;
                case DOWN:
                    if (next_sqm == tile.second && pos.x == tile.first) direction = 0;
                    break;
                case RIGHT:
                    if (next_sqm == tile.first && pos.y == tile.second) direction = 0;
                    break;
                default: // nothing
                    break;
            }
        }

        axis += direction;

        if (axis + size > next_sqm || axis < sqm) {
            axis += direction;
            key_lock = true;
        } 

        if (axis == next_sqm || axis == prev_sqm) {
            moving = true;
            sqm += (axis == next_sqm) ? 32 : -32;
            direction = 0;
            key_lock = false;
        }
    };
    
    if (moveY < 0) {
        movement(pos.y, moveY, sqmY, pos.h, UP);
        anime_sprite(this->UP);
    } else if (moveX < 0) {
        movement(pos.x, moveX, sqmX, pos.w, LEFT);
        anime_sprite(this->LEFT);
    } else if (moveY > 0) {
        movement(pos.y, moveY, sqmY, pos.h, DOWN);
        anime_sprite(this->DOWN);
    } else if (moveX > 0) {
        movement(pos.x, moveX, sqmX, pos.w, RIGHT);
        anime_sprite(this->RIGHT);
    } else {
        if (current_frame < this->LEFT)
            current_frame = this->UP;
        else if (current_frame > this->LEFT && current_frame < this->DOWN)
            current_frame = this->LEFT;
        else if (current_frame > this->DOWN && current_frame < this->RIGHT)
            current_frame = this->DOWN;
        else if (current_frame > this->RIGHT)
            current_frame = this->RIGHT;
    }

    camera.x = pos.x - 384;
    camera.y = pos.y - 320;
}

bool Player::moved()
{
    return moving;
}

std::pair<int,int> Player::gethp()
{
    return hp;
}

void Player::heal(int healing)
{
    hp.second += (hp.second + healing > hp.first) ? hp.first - hp.second : healing;
}

void Player::dmg(int new_hp)
{
    hp.second -= new_hp;
    if (hp.second < 0) hp.second = 0;
}

void Player::regen()
{
    int hp_regen = 2;
    int mp_regen = 1;
    
    hp.second += (hp.second + hp_regen > hp.first) ? hp.first - hp.second : hp_regen;
    mp.second += (mp.second + mp_regen > mp.first) ? mp.first - mp.second : mp_regen;

    regen_tick = SDL_GetTicks();
}

std::pair<int,int> Player::getmp()
{
    return mp;
}

void Player::mpused(int new_mp)
{
    mp.second -= new_mp;
    if (mp.second < 0) mp.second = 0;
}

int Player::getlvl()
{
    return level;
}

int Player::getstr()
{
    return strength;
}

std::pair<int, int> Player::getexp()
{
    return exp;
}

bool Player::setexp(int new_xp)
{
    exp.second += new_xp;

    if (exp.second >= exp.first) {
        exp.second = exp.second - exp.first;
        exp.first *= 2;

        strength++;
        level++;

        hp.second = hp.first += 100;
        mp.second = mp.first += 50;

        return true;
    }

    return false;
}

void Player::setDungeon(bool enter, int rank = 0)
{
    dungeon = {enter, rank};
}

bool Player::isInDungeon()
{
    return dungeon.first;
}

int Player::getDungeonRank()
{
    return dungeon.second;
}

void Player::kill()
{
    setpos(384, 320);
    hp.second = hp.first;
    mp.second = mp.first;
}

// time events
Uint32 Player::regen_time()
{
    return regen_tick;
}
