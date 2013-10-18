#include "StartWindow.hpp"

StartWindow::StartWindow()
{
    last_time = SDL_GetTicks();
    drawMe = true;
    blinkTime = 800;

    gameName = TTF_RenderText_Solid(font50, "IKS OKS", textColorWhite);
    blinkingText = TTF_RenderText_Solid(font20, "Press Enter to start.", textColorWhite);
    bySignature = TTF_RenderText_Solid(font10, "By: Milos Jankovic", textColorWhite);
}

StartWindow::~StartWindow()
{
    SDL_FreeSurface(gameName);
    SDL_FreeSurface(blinkingText);
    SDL_FreeSurface(bySignature);
}

void StartWindow::handle_events(bool* quit, GameState* gamestate)
{
    while( SDL_PollEvent( &event ) )
    {
        if( event.type == SDL_KEYDOWN )
        {
            if(event.key.keysym.sym == SDLK_RETURN )
            {
                *gamestate = GameWindow::GS_ChooseState;
            }
        }

        if( event.type == SDL_QUIT )
        {
            *quit = true;
        }
    }
}

void StartWindow::handle_logic()
{
}

void StartWindow::handle_rendering()
{
    SDL_FillRect(screen, &screen->clip_rect, 0x000000);

    ApplySurface( (screen->w - gameName->w)/2, screen->h/4, gameName, screen );

    ApplySurface( (screen->w - bySignature->w)/2, screen->h - screen->h/4, bySignature, screen );

    /// blinking text
    Uint32 this_time = SDL_GetTicks();
    if(this_time - last_time >= blinkTime)
    {
        last_time = this_time;
        drawMe = !drawMe;
    }
    if(drawMe)
    {
        ApplySurface( (screen->w - blinkingText->w)/2, screen->h - screen->h/2, blinkingText, screen );
    }

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }
}




