#ifndef MISSILE_H__
#define MISSILE_H__

#include <SDL/SDL.h>
#include <vector>
#include "Sound.h"
#include "Image.h"
#include "ProjXGlobal.h"

using std::vector;

class Missile
{
    public:
        //Constructors and Destructors
        Missile();
        Missile(int V_x, int V_y, int x, int y, int w, int h); //Make a missile with that width and height
        Missile(int V_x, int V_y, SDL_Rect rect);
        ~Missile();
        void GiveInfo(int x1, int x2, int y1, int y2);
           
        //Public Functions
        bool Move();
        void Display();
        void Erase();
           
        //Private Member Functions
        SDL_Rect GetR();
        void SetR(SDL_Rect rect);
        void SetR(int x, int y, int w, int h);
        void ChangeRX(int x);
        void ChangeRY(int y);
        void SetRX(int x);
        void SetRY(int y);
        vector <SDL_Surface*>* ReturnVec();
        SDL_Surface* GetVec(int i);
        void AddToVec(SDL_Surface *s);
        int GetI();
        void SetI(int n);
        void ShiftI();
        int GetVx();
        void SetVx(int n);
        void ChangeVx(int n);
        int GetVy();
        void SetVy(int n);
        void ChangeVy(int n);
        int GetDMG();
        void SetDMG(int n);
        void ChangeDMG(int n);
        
    protected:
        SDL_Rect r; //SDL_Rect to store location and size
        vector <SDL_Surface*> vec; //Vector of SDL_Surface pointers for animation
        int i; //Current iterator for the vec
        int Vx; //Velocity in the x direction
        int Vy; //Velocity in the y direction
        int dmg; //Damage
        
        //Map Boundaries
        int x_min;
        int x_max;
        int y_min;
        int y_max;
};

#endif
