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

    void handle_events(SDL_Event, int*);
    void handle_logic(bool*, int*);
    void handle_rendering(bool, SDL_Surface*);

    int GetWinnerNum();

private:
    SDL_Rect fieldRect;

    std::vector<Field*> fields;

    void drawField(SDL_Surface*);
    Field::FieldType checkForWinner();

    int winnerNum;
    int previousFieldsLeft;

    SDL_Surface *doneCoveringSurface;
    SDL_Surface *restartFontSurface;
    SDL_Rect restartFontRect;
    TTF_Font *restartFont;
    SDL_Color textColorWhite, textColorBlack;
    Uint32 last_time;
    Uint32 blinkTime;
    bool drawMe;
};

#endif


