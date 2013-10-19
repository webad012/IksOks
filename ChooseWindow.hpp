#ifndef CHOOSEWINDOW_HPP
#define CHOOSEWINDOW_HPP

#include <iostream>

#include "GameWindow.hpp"

class ChooseWindow : public GameWindow
{
public:
    ChooseWindow();
    ~ChooseWindow();

    void handle_events(bool*, GameState*);
    void handle_logic();
    void handle_rendering();

    AIDifficulty GetAIDiffictulty();

private:
    SDL_Rect pvpRect, pvaRect, exitRect, difficultyRect;
    SDL_Surface *PvP, *PvA, *exit, *difficultySurface;
    SDL_Color borderColor;
    Uint32 triangleColor;
    AIDifficulty selectedDifficulty;

    int tr1x1, tr1y1, tr1x2, tr1y2, tr1x3, tr1y3;
    int tr2x1, tr2y1, tr2x2, tr2y2, tr2x3, tr2y3;

    SDL_Rect selectedRect;
};

#endif
