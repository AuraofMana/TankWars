#ifndef EXPLOSION_H__
#define EXPLOSION_H__

#include <SDL/SDL.h>
#include <vector>

#include "ProjXGlobal.h"
#include "Image.h"

using std::vector;

class Explosion
{
    public:
        Explosion(SDL_Rect rect);
        Explosion(int x, int y, int w, int h);
        ~Explosion();
        void Draw();
        void Erase();
           
        SDL_Rect GetR();
        SDL_Rect* ReturnR();
        void SetR(SDL_Rect rect);
        void SetR(int x, int y, int w, int h);
        vector <SDL_Surface*>* ReturnVec();
        SDL_Surface* GetVec(int i);
        void AddToVec(SDL_Surface *s);
        int GetI();
        void SetI(int n);
        void ShiftI();
        int GetTimer();
        void SetTimer(int n);
        void ChangeTimer(int n);
        void MinusTimer();
           
    private:
        SDL_Rect r; //SDL_Rect to save information
        vector <SDL_Surface*> vec; //Vector of SDL_Surface's
        int i; //Current iterator for the vec
        int timer; //Timer to animation
};

#endif
