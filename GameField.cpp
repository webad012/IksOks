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
    previousFieldsLeft = 9;

    doneCoveringSurface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ANYFORMAT, screen->w, screen->h, screen->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_FillRect(doneCoveringSurface, NULL, SDL_MapRGBA(screen->format, 0xAA, 0xAA, 0xAA, 0xFF));
	SDL_SetAlpha(doneCoveringSurface, SDL_SRCALPHA, 100);

	restartFont = TTF_OpenFont( "fonts-japanese-gothic.ttf", 50 );
    if( restartFont == NULL )
    {
        throw std::string("restartFont problem - " + std::string(TTF_GetError()));
    }
    textColorWhite.r = 255;
    textColorWhite.g = 255;
    textColorWhite.b = 255;
    textColorWhite.unused = 255;

    textColorBlack.r = 0;
    textColorBlack.g = 0;
    textColorBlack.b = 0;
    textColorBlack.unused = 255;

    restartFontSurface = TTF_RenderUTF8_Shaded(restartFont, "Pritisni Enter", textColorWhite, textColorBlack);
    restartFontRect.x = (screen->w - restartFontSurface->w)/2;
    restartFontRect.y = (screen->h - restartFontSurface->h)/2;

    last_time = SDL_GetTicks();
    drawMe = true;
    blinkTime = 800;
}

GameField::~GameField()
{
    SDL_FreeSurface(doneCoveringSurface);
    TTF_CloseFont(restartFont);
    SDL_FreeSurface(restartFontSurface);
}

void GameField::handle_events(SDL_Event event, int *currentPlayer)
{
    if( (event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == SDL_BUTTON_LEFT) )
    {
        for(std::vector<Field*>::iterator it = fields.begin(); it != fields.end(); it++)
        {
            if( (GameWindow::IfMouseOverRect(event, (*it)->GetRect())==true) && ((*it)->GetType() == Field::FT_NONE ) )
            {
                if(*currentPlayer == 1 || *currentPlayer == 2)
                {
                    (*it)->SetType(*currentPlayer);
                }
                else
                {
                    throw std::string("unknown currentPlayer value: " + *currentPlayer);
                }
            }
        }
    }
}

void GameField::handle_logic(bool* gameDone, int *currentPlayer)
{
    if(*gameDone == false)
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
            else if(winner == Field::FT_OKS)
            {
                winnerNum = 2;
            }
        }
        else if(fields_left == 0)
        {
            *gameDone = true;
            winnerNum = 0;
        }

        if(fields_left == previousFieldsLeft-1)
        {
            previousFieldsLeft--;
            *currentPlayer = ((*currentPlayer)%2) + 1;
        }
    }
}

void GameField::handle_rendering(bool gameDone, SDL_Surface *screen)
{
    SDL_FillRect(screen, &fieldRect, SDL_MapRGBA(screen->format, 0x00, 0x00, 0x00, 0xFF));

    for(int i=0; i<9; i++)
    {
        fields[i]->handle_rendering(screen);
    }

    if(gameDone)
    {
        SDL_BlitSurface(doneCoveringSurface, NULL, screen, &fieldRect);

        /// blinking text
        Uint32 this_time = SDL_GetTicks();
        if(this_time - last_time >= blinkTime)
        {
            last_time = this_time;
            drawMe = !drawMe;
        }
        if(drawMe)
        {
            SDL_BlitSurface(restartFontSurface, NULL, screen, &restartFontRect);
        }
//        ApplySurface( (screen->w - blinkingText->w)/2, screen->h - screen->h/2, blinkingText, screen );
    }
}

Field::FieldType GameField::checkForWinner()
{
    Field::FieldType winner = Field::FT_NONE;

    // top row
    if( (fields[0]->GetType() != Field::FT_NONE) && (fields[0]->GetType() == fields[1]->GetType()) && (fields[0]->GetType() == fields[2]->GetType()) )
    {
        fields[0]->SetWinningField();
        fields[1]->SetWinningField();
        fields[2]->SetWinningField();
        winner = fields[0]->GetType();
    }

    // middle row
    if( (fields[3]->GetType() != Field::FT_NONE) && (fields[3]->GetType() == fields[4]->GetType()) && (fields[3]->GetType() == fields[5]->GetType()) )
    {
        fields[3]->SetWinningField();
        fields[4]->SetWinningField();
        fields[5]->SetWinningField();
        winner = fields[3]->GetType();
    }

    // bottom row
    if( (fields[6]->GetType() != Field::FT_NONE) && (fields[6]->GetType() == fields[7]->GetType()) && (fields[7]->GetType() == fields[8]->GetType()) )
    {
        fields[6]->SetWinningField();
        fields[7]->SetWinningField();
        fields[8]->SetWinningField();
        winner = fields[6]->GetType();
    }

    // topleft-downright diagonal
    if( (fields[0]->GetType() != Field::FT_NONE) && (fields[0]->GetType() == fields[4]->GetType()) && (fields[0]->GetType() == fields[8]->GetType()) )
    {
        fields[0]->SetWinningField();
        fields[4]->SetWinningField();
        fields[8]->SetWinningField();
        winner = fields[0]->GetType();
    }

    // topright-downleft diagonal
    if( (fields[2]->GetType() != Field::FT_NONE) && (fields[2]->GetType() == fields[4]->GetType()) && (fields[2]->GetType() == fields[6]->GetType()) )
    {
        fields[2]->SetWinningField();
        fields[4]->SetWinningField();
        fields[6]->SetWinningField();
        winner = fields[2]->GetType();
    }

    // left column
    if( (fields[0]->GetType() != Field::FT_NONE) && (fields[0]->GetType() == fields[3]->GetType()) && (fields[0]->GetType() == fields[6]->GetType()) )
    {
        fields[0]->SetWinningField();
        fields[3]->SetWinningField();
        fields[6]->SetWinningField();
        winner = fields[0]->GetType();
    }

    // middle column
    if( (fields[1]->GetType() != Field::FT_NONE) && (fields[1]->GetType() == fields[4]->GetType()) && (fields[1]->GetType() == fields[7]->GetType()) )
    {
        fields[1]->SetWinningField();
        fields[4]->SetWinningField();
        fields[7]->SetWinningField();
        winner = fields[1]->GetType();
    }

    // right column
    if( (fields[2]->GetType() != Field::FT_NONE) && (fields[2]->GetType() == fields[5]->GetType()) && (fields[2]->GetType() == fields[8]->GetType()) )
    {
        fields[2]->SetWinningField();
        fields[5]->SetWinningField();
        fields[8]->SetWinningField();
        winner = fields[2]->GetType();
    }

    return winner;
}

int GameField::GetWinnerNum()
{
    return winnerNum;
}


