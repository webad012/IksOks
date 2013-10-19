#include "Field.hpp"

Field::Field(int num, SDL_Surface *screen, int statusBarHeight)
{
    if(num == 1 || num == 4 || num == 7)
    {
        fieldRect.x = 1;
    }
    else if(num==2 || num == 5 || num==8)
    {
        fieldRect.x = screen->w/3 + 1;
    }
    else
    {
        fieldRect.x = 2*screen->w/3 + 1;
    }

    if(num == 1 || num == 2 || num == 3)
    {
        fieldRect.y = statusBarHeight + 1;
    }
    else if(num == 4 || num==5 || num == 6)
    {
        fieldRect.y = statusBarHeight + screen->w/3 + 1;
    }
    else
    {
        fieldRect.y = statusBarHeight + 2*screen->w/3 + 1;
    }

    fieldRect.w = screen->w/3-1;
    fieldRect.h = screen->w/3-1;

    fieldType = FT_NONE;

    oksSurface = load_image( "oks.png" );
    if(oksSurface == NULL)
    {
        throw std::string("oks.png load image fail");
    }

    iksSurface = load_image( "iks.png" );
    if(iksSurface == NULL)
    {
        throw std::string("iks.png load image fail");
    }

    imageRect.x = fieldRect.x + ((fieldRect.w-oksSurface->w)/2);
    imageRect.y = fieldRect.y + ((fieldRect.h-oksSurface->h)/2);

    winningField = false;
}

Field::~Field()
{
    SDL_FreeSurface(oksSurface);
    SDL_FreeSurface(iksSurface);
}

void Field::handle_rendering(SDL_Surface* screen)
{
    SDL_FillRect(screen, &fieldRect, 0xAAAAAA);

    /// borders

    //left
    lineColor(screen, fieldRect.x, fieldRect.y, fieldRect.x, fieldRect.h+fieldRect.y, 0x000000FF);

    //right
    lineColor(screen, fieldRect.x+fieldRect.w-1, fieldRect.y, fieldRect.x+fieldRect.w-1, fieldRect.h+fieldRect.y, 0x000000FF);

    //bottom
    lineColor(screen, fieldRect.x, fieldRect.y+fieldRect.h-1, fieldRect.x+fieldRect.w, fieldRect.h+fieldRect.y-1, 0x000000FF);

    //top
    lineColor(screen, fieldRect.x, fieldRect.y, fieldRect.x+fieldRect.w, fieldRect.y, 0x000000FF);

    if(fieldType == FT_IKS)
    {
        SDL_BlitSurface(iksSurface, NULL, screen, &imageRect);
    }
    else if(fieldType == FT_OKS)
    {
        SDL_BlitSurface(oksSurface, NULL, screen, &imageRect);
    }

    if(winningField == true)
    {
        //left
        lineColor(screen, fieldRect.x+1, fieldRect.y+1, fieldRect.x+1, fieldRect.h+fieldRect.y-2, 0x00AA00FF);
        lineColor(screen, fieldRect.x+2, fieldRect.y+1, fieldRect.x+2, fieldRect.h+fieldRect.y-2, 0x00AA00FF);
        lineColor(screen, fieldRect.x+3, fieldRect.y+1, fieldRect.x+3, fieldRect.h+fieldRect.y-2, 0x00AA00FF);

        //right
        lineColor(screen, fieldRect.x+fieldRect.w-2, fieldRect.y+1, fieldRect.x+fieldRect.w-2, fieldRect.h+fieldRect.y-2, 0x00AA00FF);
        lineColor(screen, fieldRect.x+fieldRect.w-3, fieldRect.y+1, fieldRect.x+fieldRect.w-3, fieldRect.h+fieldRect.y-2, 0x00AA00FF);
        lineColor(screen, fieldRect.x+fieldRect.w-4, fieldRect.y+1, fieldRect.x+fieldRect.w-4, fieldRect.h+fieldRect.y-2, 0x00AA00FF);

        //bottom
        lineColor(screen, fieldRect.x+1, fieldRect.y+fieldRect.h-2, fieldRect.x+fieldRect.w-2, fieldRect.h+fieldRect.y-2, 0x00AA00FF);
        lineColor(screen, fieldRect.x+1, fieldRect.y+fieldRect.h-3, fieldRect.x+fieldRect.w-2, fieldRect.h+fieldRect.y-3, 0x00AA00FF);
        lineColor(screen, fieldRect.x+1, fieldRect.y+fieldRect.h-4, fieldRect.x+fieldRect.w-2, fieldRect.h+fieldRect.y-4, 0x00AA00FF);

        //top
        lineColor(screen, fieldRect.x+1, fieldRect.y+1, fieldRect.x+fieldRect.w-2, fieldRect.y+1, 0x00AA00FF);
        lineColor(screen, fieldRect.x+1, fieldRect.y+2, fieldRect.x+fieldRect.w-2, fieldRect.y+2, 0x00AA00FF);
        lineColor(screen, fieldRect.x+1, fieldRect.y+3, fieldRect.x+fieldRect.w-2, fieldRect.y+3, 0x00AA00FF);
    }
}

SDL_Rect Field::GetRect()
{
    return fieldRect;
}

Field::FieldType Field::GetType()
{
    return fieldType;
}

void Field::SetType(int playerNum)
{
    if(playerNum == 1)
    {
        fieldType = FT_IKS;
    }
    else if(playerNum == 2)
    {
        fieldType = FT_OKS;
    }
    else
    {
        throw std::string("unknown playerNum value: " + playerNum);
    }
}

SDL_Surface* Field::load_image(std::string filename)
{
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xAA, 0xAA, 0xAA );

            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }

    return optimizedImage;
}

void Field::SetWinningField()
{
    winningField = true;
}
