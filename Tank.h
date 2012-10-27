#ifndef TANK_H__
#define TANK_H__

#include <SDL/SDL.h>
#include <vector>

#include "ProjXGlobal.h"
#include "Missile.h"
#include "Sound.h"

using std::vector;

class Tank
{
    public:
        //Functions
        Tank(SDL_Rect rect, int s);
        Tank(int x, int y, int w, int h, int s);
        ~Tank();
        void GiveInfo(int x1, int x2, int y1, int y2);
        void Draw();
        void Erase();
        bool MoveUp();
        bool MoveDown();
        bool MoveLeft();
        bool MoveRight();
        virtual void Fire();
        
        //Getters and Setters
        SDL_Rect* ReturnR();
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
        int GetVy();
        void SetVy(int n);
        Global_tank_dir GetD();
        void SetD(Global_tank_dir dir);
        bool IsAlive();
        void ChangeAlive();
        int GetSpeed();
        void SetSpeed(int n);
        int GetTotalH();
        void SetTotalH(int n);
        void ChangeTotalH(int n);
        int GetCurrH();
        void SetCurrH(int n);
        bool ChangeCurrH(int n);
        int GetLives();
        void SetLives(int n);
        void AddLife();
        bool SubLife();
        int GetScore();
        void SetScore(int n);
        void AddScore(int n);
        Missile* GetMissile(int i);
        vector <Missile>* GetMV();
        void EraseMV(int i);
        Global_tank_type GetTT();
        void SetTT(Global_tank_type type);
           
    protected:
        SDL_Rect r; //SDL_Rect to store location and size
        vector <SDL_Surface*> vec; //Vector of SDL_Surface pointers for animation
        int i; //Current iterator for the vec
        int Vx; //Velocity in the x direction
        int Vy; //Velocity in the y direction
        Global_tank_dir d; //Current direction
        bool alive; //Whether or not the tank is still alive
        int speed; //Current speed
        int lives; //Number of lives left
        int totalh; //Total health
        int currh; //Current health
        int score; //Score
        vector <Missile> mv; //Vectors of missiles
        
        //Map boundaries
        int x_min;
        int x_max;
        int y_min;
        int y_max;
        
        //Tank Type
        Global_tank_type tt;
};

#endif
