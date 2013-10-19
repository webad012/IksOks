#include "PlayWindow.hpp"

PlayWindow::PlayWindow()
: statusBarHeight(30)
{
    playState = GameWindow::GS_PlayState;
    currentPlayer = 1;
    _status_bar = new StatusBar(screen, statusBarHeight);
    _game_field = new GameField(screen, statusBarHeight);
    gameDone = false;
    winnerNum = -1;
}

PlayWindow::~PlayWindow()
{
    delete _status_bar;
    delete _game_field;
}

void PlayWindow::handle_events(bool* quit, GameState* gamestate)
{
    while( SDL_PollEvent( &event ) )
    {
        if(gameDone == false)
        {
            _game_field->handle_events(event, &currentPlayer);
        }
        else if(gameDone == true)
        {
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_RETURN) )
            {
                *gamestate = GameWindow::GS_EndGame;
            }
        }

        if( event.type == SDL_QUIT )
        {
            *quit = true;
        }
    }
}

void PlayWindow::handle_logic()
{
    if(gameDone == true)
    {
        if(winnerNum == -1)
        {
            winnerNum = _game_field->GetWinnerNum();
        }

         _status_bar->handle_logic(true, winnerNum);
    }
    else
    {
        _status_bar->handle_logic(false, currentPlayer);
        _game_field->handle_logic(&gameDone, &currentPlayer);
    }
}

void PlayWindow::handle_rendering()
{
    SDL_FillRect(screen, NULL, 0x000000);
    _status_bar->handle_rendering(screen);
    _game_field->handle_rendering(gameDone, screen);

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }

    SDL_Delay(1);
}

void PlayWindow::SetState(GameState state)
{
    playState = state;
}

GameWindow::GameState PlayWindow::GetState()
{
    return playState;
}
