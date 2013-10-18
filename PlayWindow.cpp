#include "PlayWindow.hpp"

PlayWindow::PlayWindow()
: statusBarHeight(30)
{
    playState = GameWindow::GS_PlayState;
    currentPlayer = 0;
    _status_bar = new StatusBar(screen, statusBarHeight);
    _game_field = new GameField(screen, statusBarHeight);
    gameDone = false;
}

PlayWindow::~PlayWindow()
{

}

void PlayWindow::handle_events(bool* quit, GameState* gamestate)
{
    while( SDL_PollEvent( &event ) )
    {
        if(gameDone == false)
        {
            _game_field->handle_events(event);
        }

        if( event.type == SDL_QUIT )
        {
            *quit = true;
        }
    }
}

void PlayWindow::handle_logic()
{
    _status_bar->handle_logic(currentPlayer);
    _game_field->handle_logic(&gameDone);
}

void PlayWindow::handle_rendering()
{
    SDL_FillRect(screen, NULL, 0x000000);
    _status_bar->handle_rendering(screen);
    _game_field->handle_rendering(screen);

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }
}

void PlayWindow::SetState(GameState state)
{
    playState = state;
}

GameWindow::GameState PlayWindow::GetState()
{
    return playState;
}
