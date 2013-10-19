#include "ChooseWindow.hpp"

ChooseWindow::ChooseWindow()
{
    PvP = TTF_RenderUTF8_Solid(font20, "Igrač protiv Igrača", textColorWhite);
    PvA = TTF_RenderUTF8_Solid(font20, "Igrač protiv Računara", textColorWhite);
    exit = TTF_RenderUTF8_Solid(font20, "Izađi", textColorWhite);

    pvpRect.x = (screen->w - PvP->w)/2;
    pvpRect.y = screen->h/4;
    pvpRect.h = PvP->h;
    pvpRect.w = PvP->w;

    pvaRect.x = (screen->w - PvA->w)/2;
    pvaRect.y = screen->h/2;
    pvaRect.w = PvA->w;
    pvaRect.h = PvA->h;

    exitRect.x = (screen->w - exit->w)/2;
    exitRect.y = screen->h - screen->h/4;
    exitRect.w = exit->w;
    exitRect.h = exit->h;

    borderColor = {50, 50, 150, 255};

    selectedRect = pvpRect;
}

ChooseWindow::~ChooseWindow()
{
    SDL_FreeSurface(PvP);
    SDL_FreeSurface(PvA);
    SDL_FreeSurface(exit);
}

void ChooseWindow::handle_events(bool* quit, GameState* gamestate)
{
    while( SDL_PollEvent( &event ) )
    {
        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_UP)
            {
                if( CompareRect(selectedRect, pvpRect) )
                {
                    selectedRect = exitRect;
                }
                else if( CompareRect(selectedRect, pvaRect) )
                {
                    selectedRect = pvpRect;
                }
                else if( CompareRect(selectedRect, exitRect) )
                {
                    selectedRect = pvaRect;
                }
            }
            else if(event.key.keysym.sym == SDLK_DOWN)
            {
                if( CompareRect(selectedRect, pvpRect) )
                {
                    selectedRect = pvaRect;
                }
                else if( CompareRect(selectedRect, pvaRect) )
                {
                    selectedRect = exitRect;
                }
                else if( CompareRect(selectedRect, exitRect) )
                {
                    selectedRect = pvpRect;
                }
            }
            else if(event.key.keysym.sym == SDLK_RETURN)
            {
                if( CompareRect(selectedRect, pvpRect) )
                {
                    *gamestate = GameWindow::GS_PlayStatePvP;
                }
                else if( CompareRect(selectedRect, pvaRect) )
                {
                    *gamestate = GameWindow::GS_PlayStatePvA;
                }
                else if( CompareRect(selectedRect, exitRect) )
                {
                    *quit = true;
                }
            }
        }

        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                if( IfMouseOverRect(event, pvpRect) )
                {
                    *gamestate = GameWindow::GS_PlayStatePvP;
                }
                else if( IfMouseOverRect(event, pvaRect) )
                {
                    *gamestate = GameWindow::GS_PlayStatePvA;
                }
                else if( IfMouseOverRect(event, exitRect) )
                {
                    *quit = true;
                }
            }
        }

        if( event.type == SDL_QUIT )
        {
            *quit = true;
        }
    }
}

void ChooseWindow::handle_logic()
{

}

void ChooseWindow::handle_rendering()
{
    SDL_FillRect(screen, &screen->clip_rect, 0x000000);

    ApplySurface( pvpRect.x, pvpRect.y, PvP, screen );
    ApplySurface( pvaRect.x, pvaRect.y, PvA, screen );
    ApplySurface( exitRect.x, exitRect.y, exit, screen );

    DrawRectangle(selectedRect, screen, 3, borderColor);

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }

    SDL_Delay(1);
}




