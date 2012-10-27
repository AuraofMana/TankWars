#ifndef TILE_H__
#define TILE_H__

#include <SDL/SDL.h>
#include <vector>

#include "ProjXGlobal.h"

using std::vector;

class Tile
{
    public:
        //Functions
        Tile(SDL_Rect rect, bool b, bool b2);
        Tile(int x, int y, int w, int h, bool b, bool b2);
        ~Tile();
        void Draw();
        void Erase();
        
        //Getters and Setters
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
        bool GetP();
        void SetP(bool b);
        Global_tile GetT();
        void SetT(Global_tile g);
        int GetTimer();
        void SetTimer(int n);
        void ChangeTimer(int n);
        void MinusTimer();
        bool GetBonus();
        void SetBonus(bool b);
        
        bool GetTL();
        void SetTL(bool b);
        void ChangeTL();
        bool GetTR();
        void SetTR(bool b);
        void ChangeTR();
        bool GetBL();
        void SetBL(bool b);
        void ChangeBL();
        bool GetBR();
        void SetBR(bool b);
        void ChangeBR();
        SDL_Rect GetTL_R();
        SDL_Rect* ReturnTL_R();
        void SetTL_R(SDL_Rect rect);
        void SetTL_R(int x, int y, int w, int h);
        SDL_Rect GetTR_R();
        SDL_Rect* ReturnTR_R();
        void SetTR_R(SDL_Rect rect);
        void SetTR_R(int x, int y, int w, int h);
        SDL_Rect GetBL_R();
        SDL_Rect* ReturnBL_R();
        void SetBL_R(SDL_Rect rect);
        void SetBL_R(int x, int y, int w, int h);
        SDL_Rect GetBR_R();
        SDL_Rect* ReturnBR_R();
        void SetBR_R(SDL_Rect rect);
        void SetBR_R(int x, int y, int w, int h);
        
        bool CheckFull();
        
    private:
        SDL_Rect r; //SDL_Rect to save information
        vector <SDL_Surface*> vec; //Vector of SDL_Surface's
        int i; //Current iterator for the vec
        bool p; //boolean to check if the tile is passable or not
        Global_tile t; //Enum to represent what the tile is
        int timer; //Timer to animation
        bool bonus; //Bonus
        
        //Tile Destruction Variables
        bool tl;
        bool tr;
        bool bl;
        bool br;
        SDL_Rect tl_r;
        SDL_Rect tr_r;
        SDL_Rect bl_r;
        SDL_Rect br_r;
};

#endif
