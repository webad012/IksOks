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

private:
    SDL_Rect pvpRect, pvaRect, exitRect;
    SDL_Surface *PvP, *PvA, *exit;
    SDL_Color borderColor;

    SDL_Rect selectedRect;
};

#endif
