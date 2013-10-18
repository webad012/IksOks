#ifndef FIELD_HPP
#define FIELD_HPP

#include <SDL/SDL.h>
#include <iostream>
#include <SDL/SDL_gfxPrimitives.h>

class Field
{
public:
    Field(int, SDL_Surface*, int);
    ~Field();

    enum FieldType {FT_NONE, FT_IKS, FT_OKS};

    void handle_events();
    void handle_logic();
    void handle_rendering(SDL_Surface*);

    void SetClicked(bool);

    SDL_Rect GetRect();
    FieldType GetType();

private:
    SDL_Rect fieldRect;
    bool clicked;
    FieldType fieldType;
};

#endif
