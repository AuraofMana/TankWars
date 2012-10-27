#ifndef NORMALTANK_H__
#define NORMALTANK_H__

#include "Tank.h"

class NormalTank : public Tank
{
    public:
        NormalTank(SDL_Rect rect, int s, int f, int th);
        NormalTank(int x, int y, int w, int h, int s, int f, int th);
        ~NormalTank();
        
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
