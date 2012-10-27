#include "Missile.h"

//MISSILE -- Constructor
Missile::Missile()
{
    i = 0;
}

//MISSILE -- Constructor with integers
Missile::Missile(int V_x, int V_y, int x, int y, int w, int h)
{
    r.x = x;
    r.y = y;            
    r.w = w;
    r.h = h;
    Vx = V_x;
    Vy = V_y;
    i = 0;                        
}

//MISSILE -- Constructor with SDL_Rect
Missile::Missile(int V_x, int V_y, SDL_Rect rect)
{
    r.x = rect.x;
    r.y = rect.y;            
    r.w = rect.w;
    r.h = rect.h;
    Vx = V_x;
    Vy = V_y;
    i = 0;
}

//~MISSILE -- Destructor
Missile::~Missile()
{
    
}

//GIVEINFO -- Give map boundary information
void Missile::GiveInfo(int x1, int x2, int y1, int y2)
{
    x_min = x1;
    x_max = x2;
    y_min = y1;
    y_max = y2;
}

//MOVE -- Move the missile
bool Missile::Move()
{
    if(r.x + Vx >= x_min && r.x + r.w + Vx <= x_max)
    {
        r.x += Vx;
    }
    else
    {
        return false;    
    }
    if(r.y + Vy >= y_min && r.y + r.h + Vy <= y_max)
    {
        r.y += Vy;
    }
    else
    {
        return false;    
    }
    return true;
}

//DISPLAY -- Display missile
void Missile::Display()
{
    Image_DrawImage(vec[i], r);
}

//ERASE -- Erase the image
void Missile::Erase()
{
    Image_EraseImage(r);
}

//GETR -- Getter for r
SDL_Rect Missile::GetR()
{
    return r;
}

//SETR -- Setter for r
void Missile::SetR(SDL_Rect rect)
{
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h;     
}

//SETR -- Setter for r
void Missile::SetR(int x, int y, int w, int h)
{
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;     
}

//CHANGERX -- Change r.x
void Missile::ChangeRX(int x)
{
    r.x += x;
}

//CHANGERY -- Change r.y
void Missile::ChangeRY(int y)
{
    r.y += y;
}

//SETRX -- Setter for r.x
void Missile::SetRX(int x)
{
    r.x = x;
}

//SETRY -- Setter for r.y
void Missile::SetRY(int y)
{
    r.y = y; 
}

//RETURNVEC -- Return vec
vector <SDL_Surface*>* Missile::ReturnVec()
{
    return &vec;   
}

//GETVEC -- Getter for vec
SDL_Surface* Missile::GetVec(int i)
{
    return vec[i];         
}

//ADDTOVEC -- Add a SDL_Surface ptr to vec
void Missile::AddToVec(SDL_Surface *s)
{
    vec.push_back(s); 
}

//GETI -- Getter for i
int Missile::GetI()
{
    return i;
}

//SETI -- Setter for i
void Missile::SetI(int n)
{
    i = n;
}

//SHIFTI -- Shift i by 1
void Missile::ShiftI()
{
    i++;
    if(i == (int) vec.size()) i = 0; 
}

//GETVX -- Getter for Vx
int Missile::GetVx()
{
    return Vx;
}

//SETVX -- Setter for Vx
void Missile::SetVx(int n)
{
    Vx = n;
}

//CHANGEVX -- Change Vx
void Missile::ChangeVx(int n)
{
    Vx += n;
}

//GETVY -- Getter for Vy
int Missile::GetVy()
{
    return Vy;
}

//SETVY -- Setter for Vy
void Missile::SetVy(int n)
{
    Vy = n;
}

//CHANGEVY -- Change Vy
void Missile::ChangeVy(int n)
{
    Vy += n;
}

//GETDMG -- Getter for dmg
int Missile::GetDMG()
{
    return dmg;
}

//SETDMG -- Setter for dmg
void Missile::SetDMG(int n)
{
    dmg = n;     
}

//CHANGEDMG -- Change dmg
void Missile::ChangeDMG(int n)
{
    if(dmg + n < 0)
    {
        dmg = 0;       
    }
    else
    {
        dmg += n;
    }   
}
