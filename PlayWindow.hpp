#ifndef PLAYWINDOW_HPP
#define PLAYWINDOW_HPP

#include <iostream>

#include "GameWindow.hpp"
#include "StatusBar.hpp"
#include "GameField.hpp"

class PlayWindow : public GameWindow
{
public:
    PlayWindow();
    ~PlayWindow();

    void handle_events(bool*, GameState*);
    void handle_logic();
    void handle_rendering();

    GameState GetState();
    void SetState(GameState);

private:
    GameState playState;

    int currentPlayer;
    int statusBarHeight;

    bool gameDone;

    StatusBar *_status_bar;
    GameField *_game_field;
};

#endif

