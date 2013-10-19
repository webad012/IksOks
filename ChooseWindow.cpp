#include "ChooseWindow.hpp"

ChooseWindow::ChooseWindow()
{
    PvP = TTF_RenderUTF8_Blended(font20, "Igrač protiv Igrača", textColorWhite);
    PvA = TTF_RenderUTF8_Blended(font20, "Igrač protiv Računara", textColorWhite);
    exit = TTF_RenderUTF8_Blended(font20, "Izađi", textColorWhite);
    difficultySurface = TTF_RenderUTF8_Blended(font20, "Normal", textColorWhite);

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

    difficultyRect.x = pvaRect.x + pvaRect.w + 40;
    difficultyRect.y = pvaRect.y + pvaRect.h/2 - difficultySurface->h/2;
    difficultyRect.w = difficultySurface->w;
    difficultyRect.h = difficultySurface->h;

    borderColor.r = 50;
    borderColor.g = 50;
    borderColor.b = 150;
    borderColor.unused = 255;

    triangleColor = 0x323296FF;

    /// triangle 1 points
    tr1x1 = pvaRect.x+pvaRect.w+20;
    tr1y1 = pvaRect.y + pvaRect.h/2;
    tr1x2 = pvaRect.x+pvaRect.w+30;
    tr1y2 = pvaRect.y + pvaRect.h/2 -10;
    tr1x3 = pvaRect.x+pvaRect.w+30;
    tr1y3 = pvaRect.y + pvaRect.h/2 +10;

    /// triangle 2 points
    tr2x1 = difficultyRect.x+difficultyRect.w+20;
    tr2y1 = difficultyRect.y + difficultyRect.h/2 -10;
    tr2x2 = difficultyRect.x+difficultyRect.w+20;
    tr2y2 = difficultyRect.y + difficultyRect.h/2 +10;
    tr2x3 = difficultyRect.x+difficultyRect.w+30;
    tr2y3 = difficultyRect.y + difficultyRect.h/2;

    selectedRect = pvpRect;
    selectedDifficulty = AI_Easy;
}

ChooseWindow::~ChooseWindow()
{
    SDL_FreeSurface(PvP);
    SDL_FreeSurface(PvA);
    SDL_FreeSurface(exit);
    SDL_FreeSurface(difficultySurface);
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

            else if( (event.key.keysym.sym == SDLK_LEFT) && CompareRect(selectedRect, pvaRect) )
            {
                if( selectedDifficulty == AI_Easy )
                {
                    selectedDifficulty = AI_Hard;
                    SDL_FreeSurface(difficultySurface);
                    difficultySurface = TTF_RenderUTF8_Blended(font20, "Hard", textColorWhite);
                }
                else if( selectedDifficulty == AI_Normal )
                {
                    selectedDifficulty = AI_Easy;
                    SDL_FreeSurface(difficultySurface);
                    difficultySurface = TTF_RenderUTF8_Blended(font20, "Easy", textColorWhite);
                }
                else if( selectedDifficulty == AI_Hard )
                {
                    selectedDifficulty = AI_Normal;
                    SDL_FreeSurface(difficultySurface);
                    difficultySurface = TTF_RenderUTF8_Blended(font20, "Normal", textColorWhite);
                }
            }
            else if( (event.key.keysym.sym == SDLK_RIGHT) && CompareRect(selectedRect, pvaRect) )
            {
                if( selectedDifficulty == AI_Easy )
                {
                    selectedDifficulty = AI_Normal;
                    SDL_FreeSurface(difficultySurface);
                    difficultySurface = TTF_RenderUTF8_Blended(font20, "Normal", textColorWhite);
                }
                else if( selectedDifficulty == AI_Normal )
                {
                    selectedDifficulty = AI_Hard;
                    SDL_FreeSurface(difficultySurface);
                    difficultySurface = TTF_RenderUTF8_Blended(font20, "Hard", textColorWhite);
                }
                else if( selectedDifficulty == AI_Hard )
                {
                    selectedDifficulty = AI_Easy;
                    SDL_FreeSurface(difficultySurface);
                    difficultySurface = TTF_RenderUTF8_Blended(font20, "Easy", textColorWhite);
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

                if( CompareRect(selectedRect, pvaRect) && IfMouseOverTriangle(event, tr1x1, tr1y1, tr1x2, tr1y2, tr1x3, tr1y3) )
                {
                    if( selectedDifficulty == AI_Easy )
                    {
                        selectedDifficulty = AI_Hard;
                        SDL_FreeSurface(difficultySurface);
                        difficultySurface = TTF_RenderUTF8_Blended(font20, "Hard", textColorWhite);
                    }
                    else if( selectedDifficulty == AI_Normal )
                    {
                        selectedDifficulty = AI_Easy;
                        SDL_FreeSurface(difficultySurface);
                        difficultySurface = TTF_RenderUTF8_Blended(font20, "Easy", textColorWhite);
                    }
                    else if( selectedDifficulty == AI_Hard )
                    {
                        selectedDifficulty = AI_Normal;
                        SDL_FreeSurface(difficultySurface);
                        difficultySurface = TTF_RenderUTF8_Blended(font20, "Normal", textColorWhite);
                    }
                }
                else if( CompareRect(selectedRect, pvaRect) && IfMouseOverTriangle(event, tr2x1, tr2y1, tr2x2, tr2y2, tr2x3, tr2y3) )
                {
                    if( selectedDifficulty == AI_Easy )
                    {
                        selectedDifficulty = AI_Normal;
                        SDL_FreeSurface(difficultySurface);
                        difficultySurface = TTF_RenderUTF8_Blended(font20, "Normal", textColorWhite);
                    }
                    else if( selectedDifficulty == AI_Normal )
                    {
                        selectedDifficulty = AI_Hard;
                        SDL_FreeSurface(difficultySurface);
                        difficultySurface = TTF_RenderUTF8_Blended(font20, "Hard", textColorWhite);
                    }
                    else if( selectedDifficulty == AI_Hard )
                    {
                        selectedDifficulty = AI_Easy;
                        SDL_FreeSurface(difficultySurface);
                        difficultySurface = TTF_RenderUTF8_Blended(font20, "Easy", textColorWhite);
                    }
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

    if( CompareRect(selectedRect, pvaRect) == true )
    {
        /// first triangle
        int tr1 = filledTrigonColor(screen, tr1x1, tr1y1, tr1x2, tr1y2, tr1x3, tr1y3, triangleColor);
        if( -1 == tr1 )
        {
            throw std::string("ChooseWindow::handle_rendering - draw triangle1 problem");
        }

        ApplySurface( difficultyRect.x, difficultyRect.y, difficultySurface, screen );

        /// second triangle
        int tr2 = filledTrigonColor(screen, tr2x1, tr2y1, tr2x2, tr2y2, tr2x3, tr2y3, triangleColor);
        if( -1 == tr2 )
        {
            throw std::string("ChooseWindow::handle_rendering - draw triangle2 problem");
        }
    }

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }

    SDL_Delay(1);
}

GameWindow::AIDifficulty ChooseWindow::GetAIDiffictulty()
{
    return selectedDifficulty;
}


