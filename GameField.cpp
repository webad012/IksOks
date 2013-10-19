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
    for(std::vector<Field*>::iterator it = fields.begin(); it != fields.end(); ++it)
    {
        delete *it;
        *it = NULL;
    }

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

void GameField::AIMakeMove(GameWindow::AIDifficulty aiDifficulty)
{
    bool moveMade = false;

    if(aiDifficulty >= GameWindow::AI_Easy)
    {
        AI_complete_row(&moveMade);
        AI_block_row(&moveMade);
    }

    if(aiDifficulty >= GameWindow::AI_Normal)
    {
        AI_set_trap(&moveMade);
    }

    if(aiDifficulty == GameWindow::AI_Hard)
    {
//        AI_block_trap();
    }

    AI_rand_move(&moveMade);
}

void GameField::AI_complete_row(bool *moveMade)
{
    if( *moveMade == false )
    {
        Uint32 delay = rand() % 500;
        SDL_Delay(delay);
        int space = -1;

        // top row
        if( (fields[0]->GetType() == Field::FT_NONE ) && (fields[1]->GetType() == Field::FT_OKS) && (fields[2]->GetType() == Field::FT_OKS) )
        {
            space = 0;
        }
        else if( (fields[0]->GetType() == Field::FT_OKS ) && (fields[1]->GetType() == Field::FT_NONE) && (fields[2]->GetType() == Field::FT_OKS) )
        {
            space = 1;
        }
        else if( (fields[0]->GetType() == Field::FT_OKS) && (fields[1]->GetType() == Field::FT_OKS) && (fields[2]->GetType() == Field::FT_NONE) )
        {
            space = 2;
        }

        // middle row
        else if( (fields[3]->GetType() == Field::FT_NONE) && (fields[4]->GetType() == Field::FT_OKS) && (fields[5]->GetType() == Field::FT_OKS) )
        {
            space = 3;
        }
        else if( (fields[3]->GetType() == Field::FT_OKS) && (fields[4]->GetType() == Field::FT_NONE) && (fields[5]->GetType() == Field::FT_OKS) )
        {
            space = 4;
        }
        else if( (fields[3]->GetType() == Field::FT_OKS) && (fields[4]->GetType() == Field::FT_OKS) && (fields[5]->GetType() == Field::FT_NONE) )
        {
            space = 5;
        }

        // bottom row
        else if( (fields[6]->GetType() == Field::FT_NONE) && (fields[7]->GetType() == Field::FT_OKS) && (fields[8]->GetType() == Field::FT_OKS) )
        {
            space = 6;
        }
        else if( (fields[6]->GetType() == Field::FT_OKS) && (fields[7]->GetType() == Field::FT_NONE) && (fields[8]->GetType() == Field::FT_OKS) )
        {
            space = 7;
        }
        else if( (fields[6]->GetType() == Field::FT_OKS) && (fields[7]->GetType() == Field::FT_OKS) && (fields[8]->GetType() == Field::FT_NONE) )
        {
            space = 8;
        }

        // left column
        else if( (fields[0]->GetType() == Field::FT_NONE) && (fields[3]->GetType() == Field::FT_OKS) && (fields[6]->GetType() == Field::FT_OKS) )
        {
            space = 0;
        }
        else if( (fields[0]->GetType() == Field::FT_OKS) && (fields[3]->GetType() == Field::FT_NONE) && (fields[6]->GetType() == Field::FT_OKS) )
        {
            space = 3;
        }
        else if( (fields[0]->GetType() == Field::FT_OKS) && (fields[3]->GetType() == Field::FT_OKS) && (fields[6]->GetType() == Field::FT_NONE) )
        {
            space = 6;
        }

        // middle column
        else if( (fields[1]->GetType() == Field::FT_NONE) && (fields[4]->GetType() == Field::FT_OKS) && (fields[7]->GetType() == Field::FT_OKS) )
        {
            space = 1;
        }
        else if( (fields[1]->GetType() == Field::FT_OKS) && (fields[4]->GetType() == Field::FT_NONE) && (fields[7]->GetType() == Field::FT_OKS) )
        {
            space = 4;
        }
        else if( (fields[1]->GetType() == Field::FT_OKS) && (fields[4]->GetType() == Field::FT_OKS) && (fields[7]->GetType() == Field::FT_NONE) )
        {
            space = 7;
        }

        //right column
        else if( (fields[2]->GetType() == Field::FT_NONE) && (fields[5]->GetType() == Field::FT_OKS) && (fields[8]->GetType() == Field::FT_OKS) )
        {
            space = 2;
        }
        else if( (fields[2]->GetType() == Field::FT_OKS) && (fields[5]->GetType() == Field::FT_NONE) && (fields[8]->GetType() == Field::FT_OKS) )
        {
            space = 5;
        }
        else if( (fields[2]->GetType() == Field::FT_OKS) && (fields[5]->GetType() == Field::FT_OKS) && (fields[8]->GetType() == Field::FT_NONE) )
        {
            space = 8;
        }

        //topleft-downright diagonal
        else if( (fields[0]->GetType() == Field::FT_NONE) && (fields[4]->GetType() == Field::FT_OKS) && (fields[8]->GetType() == Field::FT_OKS) )
        {
            space = 0;
        }
        else if( (fields[0]->GetType() == Field::FT_OKS) && (fields[4]->GetType() == Field::FT_NONE) && (fields[8]->GetType() == Field::FT_OKS) )
        {
            space = 4;
        }
        else if( (fields[0]->GetType() == Field::FT_OKS) && (fields[4]->GetType() == Field::FT_OKS) && (fields[8]->GetType() == Field::FT_NONE) )
        {
            space = 8;
        }

        //topright-downleft diagonal
        else if( (fields[2]->GetType() == Field::FT_NONE) && (fields[4]->GetType() == Field::FT_OKS) && (fields[6]->GetType() == Field::FT_OKS) )
        {
            space = 2;
        }
        else if( (fields[2]->GetType() == Field::FT_OKS) && (fields[4]->GetType() == Field::FT_NONE) && (fields[6]->GetType() == Field::FT_OKS) )
        {
            space = 4;
        }
        else if( (fields[2]->GetType() == Field::FT_OKS) && (fields[4]->GetType() == Field::FT_OKS) && (fields[6]->GetType() == Field::FT_NONE) )
        {
            space = 6;
        }

        if(space != -1)
        {
            fields[space]->SetType(2);
            *moveMade = true;
        }
    }

}
void GameField::AI_block_row(bool *moveMade)
{
    if( *moveMade == false )
    {
        Uint32 delay = rand() % 500;
        SDL_Delay(delay);
        int space = -1;

        // top row
        if( (fields[0]->GetType() == Field::FT_NONE ) && (fields[1]->GetType() == Field::FT_IKS) && (fields[2]->GetType() == Field::FT_IKS) )
        {
            space = 0;
        }
        else if( (fields[0]->GetType() == Field::FT_IKS ) && (fields[1]->GetType() == Field::FT_NONE) && (fields[2]->GetType() == Field::FT_IKS) )
        {
            space = 1;
        }
        else if( (fields[0]->GetType() == Field::FT_IKS) && (fields[1]->GetType() == Field::FT_IKS) && (fields[2]->GetType() == Field::FT_NONE) )
        {
            space = 2;
        }

        // middle row
        else if( (fields[3]->GetType() == Field::FT_NONE) && (fields[4]->GetType() == Field::FT_IKS) && (fields[5]->GetType() == Field::FT_IKS) )
        {
            space = 3;
        }
        else if( (fields[3]->GetType() == Field::FT_IKS) && (fields[4]->GetType() == Field::FT_NONE) && (fields[5]->GetType() == Field::FT_IKS) )
        {
            space = 4;
        }
        else if( (fields[3]->GetType() == Field::FT_IKS) && (fields[4]->GetType() == Field::FT_IKS) && (fields[5]->GetType() == Field::FT_NONE) )
        {
            space = 5;
        }

        // bottom row
        else if( (fields[6]->GetType() == Field::FT_NONE) && (fields[7]->GetType() == Field::FT_IKS) && (fields[8]->GetType() == Field::FT_IKS) )
        {
            space = 6;
        }
        else if( (fields[6]->GetType() == Field::FT_IKS) && (fields[7]->GetType() == Field::FT_NONE) && (fields[8]->GetType() == Field::FT_IKS) )
        {
            space = 7;
        }
        else if( (fields[6]->GetType() == Field::FT_IKS) && (fields[7]->GetType() == Field::FT_IKS) && (fields[8]->GetType() == Field::FT_NONE) )
        {
            space = 8;
        }

        // left column
        else if( (fields[0]->GetType() == Field::FT_NONE) && (fields[3]->GetType() == Field::FT_IKS) && (fields[6]->GetType() == Field::FT_IKS) )
        {
            space = 0;
        }
        else if( (fields[0]->GetType() == Field::FT_IKS) && (fields[3]->GetType() == Field::FT_NONE) && (fields[6]->GetType() == Field::FT_IKS) )
        {
            space = 3;
        }
        else if( (fields[0]->GetType() == Field::FT_IKS) && (fields[3]->GetType() == Field::FT_IKS) && (fields[6]->GetType() == Field::FT_NONE) )
        {
            space = 6;
        }

        // middle column
        else if( (fields[1]->GetType() == Field::FT_NONE) && (fields[4]->GetType() == Field::FT_IKS) && (fields[7]->GetType() == Field::FT_IKS) )
        {
            space = 1;
        }
        else if( (fields[1]->GetType() == Field::FT_IKS) && (fields[4]->GetType() == Field::FT_NONE) && (fields[7]->GetType() == Field::FT_IKS) )
        {
            space = 4;
        }
        else if( (fields[1]->GetType() == Field::FT_IKS) && (fields[4]->GetType() == Field::FT_IKS) && (fields[7]->GetType() == Field::FT_NONE) )
        {
            space = 7;
        }

        //right column
        else if( (fields[2]->GetType() == Field::FT_NONE) && (fields[5]->GetType() == Field::FT_IKS) && (fields[8]->GetType() == Field::FT_IKS) )
        {
            space = 2;
        }
        else if( (fields[2]->GetType() == Field::FT_IKS) && (fields[5]->GetType() == Field::FT_NONE) && (fields[8]->GetType() == Field::FT_IKS) )
        {
            space = 5;
        }
        else if( (fields[2]->GetType() == Field::FT_IKS) && (fields[5]->GetType() == Field::FT_IKS) && (fields[8]->GetType() == Field::FT_NONE) )
        {
            space = 8;
        }

        //topleft-downright diagonal
        else if( (fields[0]->GetType() == Field::FT_NONE) && (fields[4]->GetType() == Field::FT_IKS) && (fields[8]->GetType() == Field::FT_IKS) )
        {
            space = 0;
        }
        else if( (fields[0]->GetType() == Field::FT_IKS) && (fields[4]->GetType() == Field::FT_NONE) && (fields[8]->GetType() == Field::FT_IKS) )
        {
            space = 4;
        }
        else if( (fields[0]->GetType() == Field::FT_IKS) && (fields[4]->GetType() == Field::FT_IKS) && (fields[8]->GetType() == Field::FT_NONE) )
        {
            space = 8;
        }

        //topright-downleft diagonal
        else if( (fields[2]->GetType() == Field::FT_NONE) && (fields[4]->GetType() == Field::FT_IKS) && (fields[6]->GetType() == Field::FT_IKS) )
        {
            space = 2;
        }
        else if( (fields[2]->GetType() == Field::FT_IKS) && (fields[4]->GetType() == Field::FT_NONE) && (fields[6]->GetType() == Field::FT_IKS) )
        {
            space = 4;
        }
        else if( (fields[2]->GetType() == Field::FT_IKS) && (fields[4]->GetType() == Field::FT_IKS) && (fields[6]->GetType() == Field::FT_NONE) )
        {
            space = 6;
        }

        if(space != -1)
        {
            fields[space]->SetType(2);
            *moveMade = true;
        }
    }
}

void GameField::AI_set_trap(bool *moveMade)
{
    if( *moveMade == false )
    {
        if( ( fields[ 6 ]->GetType() == Field::FT_OKS ) && ( fields[ 4 ]->GetType() == Field::FT_OKS )
            && ( fields[ 0 ]->GetType() == Field::FT_NONE ) && ( fields[ 7 ]->GetType() == Field::FT_NONE ) && ( fields[ 8 ]->GetType() == Field::FT_NONE ) )
        {
            fields[ 8 ]->SetType(2);
            *moveMade = true;
        }
        else if( ( fields[ 0 ]->GetType() == Field::FT_OKS ) && ( fields[ 4 ]->GetType() == Field::FT_OKS )
                && ( fields[ 2 ]->GetType() == Field::FT_NONE ) && ( fields[ 3 ]->GetType() == Field::FT_NONE ) && ( fields[ 4 ]->GetType() == Field::FT_NONE ) )
        {
            fields[ 6 ]->SetType(2);
            *moveMade = true;
        }
        else if( ( fields[ 2 ]->GetType() == Field::FT_OKS ) && ( fields[ 4 ]->GetType() == Field::FT_OKS )
                && ( fields[ 6 ]->GetType() == Field::FT_NONE ) && ( fields[ 3 ]->GetType() == Field::FT_NONE ) && ( fields[ 0 ]->GetType() == Field::FT_NONE ) )
        {
            fields[ 0 ]->SetType(2);
            *moveMade = true;
        }
        else if( ( fields[ 8 ]->GetType() == Field::FT_OKS ) && ( fields[ 4 ]->GetType() == Field::FT_OKS )
                && ( fields[ 0 ]->GetType() == Field::FT_NONE ) && ( fields[ 2 ]->GetType() == Field::FT_NONE ) && ( fields[ 5 ]->GetType() == Field::FT_NONE ) )
        {
            fields[ 2 ]->SetType(2);
            *moveMade = true;
        }


        else if( fields[ 4 ]->GetType() == Field::FT_OKS )
        {
            if( ( fields[ 1 ]->GetType() == Field::FT_OKS )
                && ( fields[ 0 ]->GetType() == Field::FT_NONE ) && ( fields[ 2 ]->GetType() == Field::FT_NONE ) )
            {
                if( fields[ 6 ]->GetType() == Field::FT_NONE )
                {
                    fields[ 2 ]->SetType(2);
                    *moveMade = true;
                }
                else if( fields[ 8 ]->GetType() == Field::FT_NONE )
                {
                    fields[ 0 ]->SetType(2);
                    *moveMade = true;
                }


            }
            else if( ( fields[ 3 ]->GetType() == Field::FT_OKS )
                && ( fields[ 0 ]->GetType() == Field::FT_NONE ) && ( fields[ 6 ]->GetType() == Field::FT_NONE ) )
            {
                if( fields[ 2 ]->GetType() == Field::FT_NONE )
                {
                    fields[ 6 ]->SetType(2);
                    *moveMade = true;
                }
                else if( fields[ 8 ]->GetType() == Field::FT_NONE )
                {
                    fields[ 0 ]->SetType(2);
                    *moveMade = true;
                }
            }
            else if( ( fields[ 5 ]->GetType() == Field::FT_OKS )
                && ( fields[ 2 ]->GetType() == Field::FT_NONE ) && ( fields[ 8 ]->GetType() == Field::FT_NONE ) )
            {
                if( fields[ 0 ]->GetType() == Field::FT_NONE )
                {
                    fields[ 0 ]->SetType(2);
                    *moveMade = true;
                }
                else if( fields[ 6 ]->GetType() == Field::FT_NONE )
                {
                    fields[ 2 ]->SetType(2);
                    *moveMade = true;
                }
            }
            else if( ( fields[ 7 ]->GetType() == Field::FT_OKS )
                && ( fields[ 6 ]->GetType() == Field::FT_NONE ) && ( fields[ 8 ]->GetType() == Field::FT_NONE ) )
            {
                if( fields[ 0 ]->GetType() == Field::FT_NONE )
                {
                    fields[ 8 ]->SetType(2);
                    *moveMade = true;
                }
                else if( fields[ 2 ]->GetType() == Field::FT_NONE )
                {
                    fields[ 6 ]->SetType(2);
                    *moveMade = true;
                }
            }
        }
    }
}

void GameField::AI_rand_move(bool *moveMade)
{
    while(*moveMade == false)
    {
        Uint32 delay = rand() % 500;
        SDL_Delay(delay);
        int space = rand()%9;

        if( fields[space]->GetType() == Field::FT_NONE )
        {
            fields[space]->SetType(2);
            *moveMade = true;
        }
    }
}



