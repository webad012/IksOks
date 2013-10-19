#ifndef FIELD_HPP
#define FIELD_HPP

#include <SDL/SDL.h>
#include <iostream>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>

class Field
{
public:
    Field(int, SDL_Surface*, int);
    ~Field();

    enum FieldType {FT_NONE, FT_IKS, FT_OKS};

    void handle_events();
    void handle_logic();
    void handle_rendering(SDL_Surface*);

    void SetType(int);

    SDL_Rect GetRect();
    FieldType GetType();

private:
    SDL_Rect fieldRect;
    FieldType fieldType;

    SDL_Surface *iksSurface, *oksSurface;
    SDL_Rect imageRect;

    SDL_Surface* load_image(std::string);
};

#endif
