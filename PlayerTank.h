#ifndef PLAYERTANK_H__
#define PLAYERTANK_H__

#include "Tank.h"

class PlayerTank : public Tank
{
    public:
        PlayerTank(SDL_Rect rect, int s, int f);
        PlayerTank(int x, int y, int w, int h, int s, int f);
        ~PlayerTank();
        
        void Fire();
        
        //Getters and Setters
        Uint32 GetLastFired();
        void SetLastFired(Uint32 t);
        Uint32 GetCurrFired();
        void SetCurrFired(Uint32 t);
        Uint32 GetFiringRate();
        void SetFiringRate(Uint32 t);
               
    private:
        Uint32 lastfired;
        Uint32 currfired;
        Uint32 firingrate;    
};

#endif
