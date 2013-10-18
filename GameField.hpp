#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <iostream>
#include <vector>

#include "Field.hpp"
#include "GameWindow.hpp"

class GameField
{
public:
    GameField(SDL_Surface*, int statusBarHeight);
    ~GameField();

    void handle_events(SDL_Event);
    void handle_logic(bool*);
    void handle_rendering(SDL_Surface*);

private:
    SDL_Rect fieldRect;

    std::vector<Field*> fields;

    void drawField(SDL_Surface*);
    Field::FieldType checkForWinner();

    int winnerNum;
};

#endif


