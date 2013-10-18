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

    clicked = false;
    fieldType = FT_NONE;

//    std::cout << "field num: " << num << " - x:" << fieldRect.x << " - y:" << fieldRect.y << " - w:" << fieldRect.w << " - h:" << fieldRect.h << std::endl;
}

Field::~Field()
{

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

    if(clicked)
    {
//        lineColor(screen, fieldRect.x, fieldRect.y+1, fieldRect.x+fieldRect.w, fieldRect.y+1, 0x000000FF);
//        lineColor(screen, fieldRect.x, fieldRect.y+2, fieldRect.x+fieldRect.w, fieldRect.y+2, 0x111111FF);
//        lineColor(screen, fieldRect.x, fieldRect.y+3, fieldRect.x+fieldRect.w, fieldRect.y+3, 0x222222FF);
//        lineColor(screen, fieldRect.x+fieldRect.w-2, fieldRect.y, fieldRect.x+fieldRect.w-2, fieldRect.h+fieldRect.y, 0x000000FF);
//        lineColor(screen, fieldRect.x+fieldRect.w-3, fieldRect.y, fieldRect.x+fieldRect.w-3, fieldRect.h+fieldRect.y, 0x111111FF);
//        lineColor(screen, fieldRect.x+fieldRect.w-4, fieldRect.y, fieldRect.x+fieldRect.w-4, fieldRect.h+fieldRect.y, 0x222222FF);
    }

}

SDL_Rect Field::GetRect()
{
    return fieldRect;
}

void Field::SetClicked(bool click)
{
    clicked = click;
}

Field::FieldType Field::GetType()
{
    return fieldType;
}


