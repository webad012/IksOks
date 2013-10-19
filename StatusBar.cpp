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

    textColorBlack.r = 0;
    textColorBlack.g = 0;
    textColorBlack.b = 0;
    textColorBlack.unused = 0;

    currentPlayer = 0;

    messageSS << "Na potezu igrač: " << currentPlayer;
    messageSurface = TTF_RenderUTF8_Blended(statusFont, messageSS.str().c_str(), textColorBlack);
    messageSurfaceOffset.x = (statusRect.w - messageSurface->w)/2;
    messageSurfaceOffset.y = (statusRect.h - messageSurface->h)/2;

    previous_gamedone_status = true;
}

StatusBar::~StatusBar()
{
    TTF_CloseFont(statusFont);
    SDL_FreeSurface(messageSurface);
}

void StatusBar::handle_logic(bool gameDone, int playerNum)
{
    if((gameDone == false) && (playerNum != currentPlayer))
    {
        currentPlayer = playerNum;
        messageSS.str("");
        messageSS.clear();
        messageSS << "Na potezu igrač: " << currentPlayer;

        SDL_FreeSurface(messageSurface);
        messageSurface = TTF_RenderUTF8_Solid(statusFont, messageSS.str().c_str(), textColorBlack);
        previous_gamedone_status = false;
    }
    else if((gameDone == true) && (previous_gamedone_status == false))
    {
        messageSS.str("");
        messageSS.clear();

        if(playerNum == 0)
        {
            messageSS << "Nema pobednika.";
        }
        else if(playerNum == 1 || playerNum == 2)
        {
            messageSS << "Pobednik: " << playerNum << ". igrač.";
        }
        else
        {
            throw std::string("StatusBar::handle_logic - wrong playerNum value: " + playerNum);
        }

        SDL_FreeSurface(messageSurface);
        messageSurface = TTF_RenderUTF8_Solid(statusFont, messageSS.str().c_str(), textColorBlack);
        previous_gamedone_status = true;
    }
}

void StatusBar::handle_rendering(SDL_Surface *screen)
{
    SDL_FillRect(screen, &statusRect, 0x888888);

    SDL_BlitSurface(messageSurface, NULL, screen, &messageSurfaceOffset);

}
