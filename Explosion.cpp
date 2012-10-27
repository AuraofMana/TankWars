#include "Explosion.h"

//Explosion -- Constructor
Explosion::Explosion(SDL_Rect rect)
{
    r.x = rect.x;
    r.y = rect.y;            
    r.w = rect.w;
    r.h = rect.h;
    timer = 35;
    i = 0;
}

//Explosion -- Constructor
Explosion::Explosion(int x, int y, int w, int h)
{
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;     
    timer = 35;
    i = 0;
}

//~Explosion -- Destructor
Explosion::~Explosion()
{
      
}

//DRAW-- Draw the Explosion
void Explosion::Draw()
{
    Image_DrawImage(vec[i], r);          
}

//ERASE -- Erase the Explosion
void Explosion::Erase()
{
    Image_EraseImage(r);           
}

//GETR -- Getter for r
SDL_Rect Explosion::GetR()
{
    return r;
}

//RETURNR -- Return r
SDL_Rect* Explosion::ReturnR()
{
    return &r;
}

//SETR -- Setter for r
void Explosion::SetR(SDL_Rect rect)
{
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h; 
}

//SETR -- Setter for r
void Explosion::SetR(int x, int y, int w, int h)
{
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;     
}

//RETURNVEC -- Getter for vec
vector <SDL_Surface*>* Explosion::ReturnVec()
{
    return &vec;       
}

//GETVEC -- Getter for vec
SDL_Surface* Explosion::GetVec(int i)
{
    return vec[i];             
}

//ADDTOVEC -- Add a SDL_Surface to the vec
void Explosion::AddToVec(SDL_Surface *s)
{
    vec.push_back(s);     
}

//GETI -- Getter for i
int Explosion::GetI()
{
    return i;    
}

//SETI -- Setter for i
void Explosion::SetI(int n)
{
    i = n;     
}

//SHIFTI -- Shift i
void Explosion::ShiftI()
{
    i++;
    if(i == vec.size())
    {
        i = 0;
    }     
}

//GETTIMER -- Getter for timer
int Explosion::GetTimer()
{
    return timer;
}

//SETTIMER -- Setter for timer
void Explosion::SetTimer(int n)
{
    timer = n; 
}

//CHANGETIMER -- Change the timer
void Explosion::ChangeTimer(int n)
{
    timer += n;    
}

//MINUSTIMER -- Subtract timer by 1
void Explosion::MinusTimer()
{
    timer--;     
}
