#ifndef STARTWINDOW_HPP
#define STARTWINDOW_HPP

#include <iostream>

#include "GameWindow.hpp"

class StartWindow : public GameWindow
{
public:
    StartWindow();
    ~StartWindow();

    void handle_events(bool*, GameState*);
    void handle_logic();
    void handle_rendering();

private:
    Uint32 last_time;
    bool drawMe;
    Uint32 blinkTime;

    SDL_Surface *gameName, *blinkingText, *bySignature;
};

#endif
