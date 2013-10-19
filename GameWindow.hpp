#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>

class GameWindow
{
public:
    GameWindow();
    ~GameWindow();

    enum GameState { GS_StartState, GS_ChooseState, GS_PlayState, GS_PlayStatePvP, GS_PlayStatePvA, GS_EndGame };
    enum AIDifficulty { AI_Easy, AI_Normal, AI_Hard };

    virtual void handle_events(bool*, GameState*) = 0;
    virtual void handle_logic() = 0;
    virtual void handle_rendering() = 0;

    void ApplySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip=NULL);
    void DrawRectangle(SDL_Rect rect, SDL_Surface *destination, int bordersize, SDL_Color bordercolor);
    bool CompareRect(const SDL_Rect, const SDL_Rect);
    static bool IfMouseOverRect(SDL_Event, SDL_Rect);
    static bool IfMouseOverTriangle(SDL_Event, int, int, int, int, int, int);

protected:
    TTF_Font *font50, *font20, *font10;
    SDL_Event event;
    SDL_Color textColorWhite;

    SDL_Surface *screen;
};

#endif
