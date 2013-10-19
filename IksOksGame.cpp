#include "IksOksGame.hpp"

IksOksGame::IksOksGame()
: _game_state(GameWindow::GS_StartState)
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        throw std::string("init problem");
    }

    if( TTF_Init() == -1 )
    {
        throw std::string( "font init problem" );
    }

    SDL_WM_SetCaption( "Iks Oks", NULL );

    _start_window = new StartWindow();
    _choose_window = new ChooseWindow();
    _play_window = new PlayWindow();
}

IksOksGame::~IksOksGame()
{
    delete _start_window;
    _start_window = NULL;
    delete _choose_window;
    _choose_window = NULL;
    delete _play_window;
    _play_window = NULL;
    TTF_Quit();
    SDL_Quit();
}

void IksOksGame::handle_events(bool *quit)
{
    if( _game_state == GameWindow::GS_StartState )
    {
        _start_window->handle_events(quit, &_game_state);
    }
    else if( _game_state == GameWindow::GS_ChooseState )
    {
        _choose_window->handle_events(quit, &_game_state);
    }
    else if( (_game_state == GameWindow::GS_PlayStatePvP) || (_game_state == GameWindow::GS_PlayStatePvA) )
    {
        _play_window->handle_events(quit, &_game_state);
    }
}

void IksOksGame::handle_logic()
{
    if( _game_state == GameWindow::GS_StartState )
    {
        _start_window->handle_logic();
    }
    else if( _game_state == GameWindow::GS_ChooseState )
    {
        _choose_window->handle_logic();
    }
    else if( (_game_state == GameWindow::GS_PlayStatePvP) || (_game_state == GameWindow::GS_PlayStatePvA) )
    {
        if(_play_window->GetState() == GameWindow::GS_PlayState)
        {
            _play_window->SetState(_game_state);
        }
        _play_window->handle_logic();
    }
    else if( _game_state == GameWindow::GS_EndGame )
    {
        delete _play_window;
        _play_window = new PlayWindow();
        _game_state = GameWindow::GS_ChooseState;
    }
}

void IksOksGame::handle_rendering()
{
    if( _game_state == GameWindow::GS_StartState )
    {
        _start_window->handle_rendering();
    }
    else if( _game_state == GameWindow::GS_ChooseState )
    {
        _choose_window->handle_rendering();
    }
    else if( (_game_state == GameWindow::GS_PlayStatePvP) || (_game_state == GameWindow::GS_PlayStatePvA) )
    {
        _play_window->handle_rendering();
    }
}
