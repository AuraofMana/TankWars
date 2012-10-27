#include <SDL/SDL.h>

#include "NormalTank.h"
#include "Sound.h"
#include "Image.h"

extern int image_size_x[];
extern int image_size_y[];
extern int NT_Missile_Speed;
extern SDL_Surface* *Image_arr;

//NORMALTANK -- Constructor with SDL_Rect
NormalTank::NormalTank(SDL_Rect rect, int s, int f, int th) : Tank(rect, s)
{
    r.x = rect.x;
    r.y = rect.y;            
    r.w = rect.w;
    r.h = rect.h;
    speed = s;
    alive = true;
    i = 0;
    d = TANK_UP;
    score = 0;
    lastfired = 0;
    currfired = 0;
    firingrate = f;
    tt = NORMALTANK;
    totalh = th;
    currh = totalh;                         
}

//NORMALTANK -- Constructor with ints
NormalTank::NormalTank(int x, int y, int w, int h, int s, int f, int th) : Tank(x, y, w, h, s)
{
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    speed = s;
    alive = true;
    i = 0;
    d = TANK_UP;
    score = 0;
    lastfired = 0;
    currfired = 0;
    firingrate = f;
    tt = NORMALTANK;
    totalh = th;
    currh = totalh;                
}

//~NORMALTANK -- Destructor
NormalTank::~NormalTank()
{
             
}

//FIRE -- Fire a missile
void NormalTank::Fire()
{
    currfired = SDL_GetTicks();
    if(lastfired == 0 || currfired - lastfired >= firingrate)
    {  
        SDL_Rect rect;
        rect.w = image_size_x[I_MISSILE];
        rect.h = image_size_y[I_MISSILE];
        Missile mi;
        switch(d)
        {
            case TANK_UP:
            {
                rect.x = r.x + (r.w / 2) - (rect.w / 2);
                rect.y = r.y - rect.h;
                mi.SetVx(0);
                mi.SetVy(-(NT_Missile_Speed));
                break;     
            }
            case TANK_DOWN:
            {
                rect.x = r.x + (r.w / 2) - (rect.w / 2);
                rect.y = r.y + r.h + rect.h;
                mi.SetVx(0);
                mi.SetVy(NT_Missile_Speed);
                break;     
            }
            case TANK_LEFT:
            {
                rect.x = r.x - rect.w;
                rect.y = r.y + (r.h / 2) - (rect.h / 2);
                mi.SetVx(-(NT_Missile_Speed));
                mi.SetVy(0);
                break;     
            }
            case TANK_RIGHT:
            {
                rect.x = r.x + r.w + rect.w;
                rect.y = r.y + (r.h / 2) - (rect.h / 2);
                mi.SetVx(NT_Missile_Speed);
                mi.SetVy(0);
                break;     
            }         
        }
        mi.SetR(rect);
        mi.AddToVec(Image_arr[I_MISSILE]);
        mi.GiveInfo(x_min, x_max, y_min, y_max);
        mi.Display();
        mv.push_back(mi);
        //PLAY FIRING SOUND
    
        lastfired = currfired;
    }
}

//GETLASTFIRED -- Getter for lastfired
Uint32 NormalTank::GetLastFired()
{
    return lastfired;
}

//SETLASTFIRED -- Setter for lastfired
void NormalTank::SetLastFired(Uint32 t)
{
    lastfired = t;
}

//GETCURRFIRED -- Getter for currfired
Uint32 NormalTank::GetCurrFired()
{
    return currfired;      
}

//SETCURRFIRED -- Setter for currfired
void NormalTank::SetCurrFired(Uint32 t)
{
    currfired = t;
}

//GETFIRINGRATE -- Getter for firingrate
Uint32 NormalTank::GetFiringRate()
{
    return firingrate;
}

//SETFIRINGRATE -- Setter for firingrate
void NormalTank::SetFiringRate(Uint32 t)
{
    firingrate = t;
}
