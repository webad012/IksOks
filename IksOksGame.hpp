#ifndef IKSOKSGAME_HPP
#define IKSOKSGAME_HPP

#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_ttf.h>

#include "StartWindow.hpp"
#include "ChooseWindow.hpp"
#include "PlayWindow.hpp"

class IksOksGame
{
public:
    IksOksGame();
    ~IksOksGame();

    void handle_events(bool*);
    void handle_logic();
    void handle_rendering();

private:
    GameWindow::GameState _game_state;

    StartWindow *_start_window;
    ChooseWindow *_choose_window;
    PlayWindow *_play_window;
};

#endif
