#include "IksOksGame.hpp"
#include "Log.hpp"

using namespace std;

int main()
{
    try
    {
        bool quit=false;
        IksOksGame *game = NULL;
        game = new IksOksGame();

        if(game == NULL)
        {
            throw std::string("creating game problem");
        }

        while(quit == false)
        {
            /// events
            game->handle_events(&quit);

            /// logic
            game->handle_logic();

            /// rendering
            game->handle_rendering();
        }

        delete game;

        return 0;
    }
    catch(std::string s)
    {
        Log::Instance()->log( "STRING EXCEPTION - main - " + s );
    }
    catch(std::exception &e)
    {
        Log::Instance()->log( "UNHANDLED EXCEPTION - main - " + std::string(e.what()) );
    }

    return 1;
}
