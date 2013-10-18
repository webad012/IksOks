#include "GameField.hpp"

GameField::GameField(SDL_Surface *screen, int statusBarHeight)
{
    fieldRect.x = 0;
    fieldRect.y = statusBarHeight;
    fieldRect.w = screen->w;
    fieldRect.h = screen->w;

    fields.push_back( new Field(1, screen, statusBarHeight) );
    fields.push_back( new Field(2, screen, statusBarHeight) );
    fields.push_back( new Field(3, screen, statusBarHeight) );

    fields.push_back( new Field(4, screen, statusBarHeight) );
    fields.push_back( new Field(5, screen, statusBarHeight) );
    fields.push_back( new Field(6, screen, statusBarHeight) );

    fields.push_back( new Field(7, screen, statusBarHeight) );
    fields.push_back( new Field(8, screen, statusBarHeight) );
    fields.push_back( new Field(9, screen, statusBarHeight) );

    winnerNum = -1;
}

GameField::~GameField()
{

}

void GameField::handle_events(SDL_Event event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            for(std::vector<Field*>::iterator it = fields.begin(); it != fields.end(); it++)
            {
                if( GameWindow::IfMouseOverRect(event, (*it)->GetRect() ) )
                {
                    (*it)->SetClicked(true);
                }
            }
        }
    }
    else if(event.type == SDL_MOUSEBUTTONUP)
    {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            for(std::vector<Field*>::iterator it = fields.begin(); it != fields.end(); it++)
            {
                if( GameWindow::IfMouseOverRect(event, (*it)->GetRect() ) )
                {
                    (*it)->SetClicked(false);
                }
            }
        }
    }
}

void GameField::handle_logic(bool* gameDone)
{
    int fields_left=0;
    for(std::vector<Field*>::iterator it = fields.begin(); it != fields.end(); it++)
    {
        if( (*it)->GetType() == Field::FT_NONE )
        {
            fields_left++;
        }
    }

    Field::FieldType winner = checkForWinner();
    if(winner != Field::FT_NONE)
    {
        *gameDone = true;

        if(winner == Field::FT_IKS)
        {
            winnerNum = 1;
        }
        else
        {
            winnerNum = 2;
        }
    }
    else if(fields_left == 0)
    {
        *gameDone = true;
        winnerNum = 0;
    }
}

void GameField::handle_rendering(SDL_Surface *screen)
{
    SDL_FillRect(screen, &fieldRect, 0x000000);

//    drawField(screen);

    for(int i=0; i<9; i++)
    {
        fields[i]->handle_rendering(screen);
    }
}

void GameField::drawField(SDL_Surface *screen)
{
    // vertical 1
    lineColor(screen, fieldRect.w/3, fieldRect.y, fieldRect.w/3, fieldRect.h+fieldRect.y, 0x00000FF);

    // vertical 2
    lineColor(screen, 2*(fieldRect.w/3), fieldRect.y, 2*(fieldRect.w/3), fieldRect.h+fieldRect.y, 0x00000FF);

    // horizontal 1
    lineColor(screen, 0, fieldRect.h/3 +fieldRect.y, fieldRect.w, fieldRect.h/3 +fieldRect.y, 0x00000FF);

//    // horizontal 2
    lineColor(screen, 0, 2*(fieldRect.h/3) +fieldRect.y, fieldRect.w, 2*(fieldRect.h/3) +fieldRect.y, 0x00000FF);
}

Field::FieldType GameField::checkForWinner()
{
    Field::FieldType winner = Field::FT_NONE;
    if( (fields[0]->GetType() != Field::FT_NONE) && (fields[0]->GetType() == fields[1]->GetType()) && (fields[0]->GetType() == fields[2]->GetType()) )
    {
        winner = fields[0]->GetType();
    }
    if( (fields[3]->GetType() != Field::FT_NONE) && (fields[3]->GetType() == fields[4]->GetType()) && (fields[3]->GetType() == fields[5]->GetType()) )
    {
        winner = fields[3]->GetType();
    }
    if( (fields[6]->GetType() != Field::FT_NONE) && (fields[6]->GetType() == fields[7]->GetType()) && (fields[7]->GetType() == fields[8]->GetType()) )
    {
        winner = fields[6]->GetType();
    }
    if( (fields[0]->GetType() != Field::FT_NONE) && (fields[0]->GetType() == fields[4]->GetType()) && (fields[0]->GetType() == fields[8]->GetType()) )
    {
        winner = fields[0]->GetType();
    }
    if( (fields[2]->GetType() != Field::FT_NONE) && (fields[2]->GetType() == fields[4]->GetType()) && (fields[2]->GetType() == fields[6]->GetType()) )
    {
        winner = fields[2]->GetType();
    }

    return winner;
}
