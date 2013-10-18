#include "StatusBar.hpp"

StatusBar::StatusBar(SDL_Surface* screen, int height)
{
    statusRect.x = 0;
    statusRect.y = 0;
    statusRect.w = screen->w;
    statusRect.h = height;

    statusFont = TTF_OpenFont( "fonts-japanese-gothic.ttf", 20 );
    if( statusFont == NULL )
    {
        throw std::string("statusFont problem - " + std::string(TTF_GetError()));
    }

    textColorBlack = {0, 0, 0, 0};

    currentPlayer = 0;

    messageSS << "Current player: " << currentPlayer+1;

    messageSurface = TTF_RenderText_Blended(statusFont, messageSS.str().c_str(), textColorBlack);
    messageSurfaceOffset.x = (statusRect.w - messageSurface->w)/2;
    messageSurfaceOffset.y = (statusRect.h - messageSurface->h)/2;
}

StatusBar::~StatusBar()
{
    TTF_CloseFont(statusFont);
    SDL_FreeSurface(messageSurface);
}

void StatusBar::handle_logic(int curPlayer)
{
    if(curPlayer != currentPlayer)
    {
        messageSS.clear();
        messageSS << "Current player: " << currentPlayer+1;

        SDL_FreeSurface(messageSurface);
        messageSurface = TTF_RenderText_Blended(statusFont, messageSS.str().c_str(), textColorBlack);
    }
}

void StatusBar::handle_rendering(SDL_Surface *screen)
{
    SDL_FillRect(screen, &statusRect, 0x888888);

    SDL_BlitSurface(messageSurface, NULL, screen, &messageSurfaceOffset);

}
