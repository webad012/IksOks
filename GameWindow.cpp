#include "GameWindow.hpp"

GameWindow::GameWindow()
{
    screen = SDL_SetVideoMode(600, 630, 32, SDL_SWSURFACE );
    if( screen == NULL )
    {
        throw std::string("screen init problem");;
    }

    font50 = TTF_OpenFont( "fonts-japanese-gothic.ttf", 50 );
    if( font50 == NULL )
    {
        throw std::string("font50 problem - " + std::string(TTF_GetError()));
    }

    font20 = TTF_OpenFont( "fonts-japanese-gothic.ttf", 20 );
    if( font20 == NULL )
    {
        throw std::string("font20 problem - " + std::string(TTF_GetError()));
    }

    font10 = TTF_OpenFont( "fonts-japanese-gothic.ttf", 10 );
    if( font10 == NULL )
    {
        throw std::string("font10 problem - " + std::string(TTF_GetError()));
    }

    textColorWhite =  {255, 255, 255, 255};
}

GameWindow::~GameWindow()
{
    TTF_CloseFont(font10);
    TTF_CloseFont(font20);
    TTF_CloseFont(font50);
}

void GameWindow::ApplySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
    SDL_Rect offset;
    offset.x=x;
    offset.y=y;

    SDL_BlitSurface(source, clip, destination, &offset);
}

void GameWindow::DrawRectangle(SDL_Rect rect, SDL_Surface *destination, int bodersize, SDL_Color bordercolor)
{
    int bordX1 = rect.x - 5;
    int bordY1 = rect.y - 5;
    int bordX2 = rect.x + rect.w + 5;
    int bordY2 = rect.y + rect.h + 5;

    for(int i=0; i < bodersize; i++)
    {
        rectangleRGBA(destination, bordX1-i, bordY1-i, bordX2+i, bordY2+i, bordercolor.r, bordercolor.g, bordercolor.b, bordercolor.unused);
    }
}

bool GameWindow::CompareRect(const SDL_Rect r1, const SDL_Rect r2)
{
    if((r1.x==r2.x) && (r1.y==r2.y) && (r1.w==r2.w) && (r1.h==r2.h))
    {
        return true;
    }

    return false;
}

bool GameWindow::IfMouseOverRect(SDL_Event e, SDL_Rect r)
{
    int evtx = e.button.x;
    int evty = e.button.y;

    if( ( evtx > r.x ) && ( evtx< r.x + r.w ) && ( evty > r.y ) && ( evty < r.y + r.h ) )
    {
        return true;
    }

    return false;
}


