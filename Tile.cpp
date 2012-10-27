#include "Tile.h"
#include "Image.h"

//TILE -- Constructor
Tile::Tile(SDL_Rect rect, bool b, bool b2)
{
    r.x = rect.x;
    r.y = rect.y;            
    r.w = rect.w;
    r.h = rect.h;
    p = b;
    i = 0;
    timer = -1;
    bonus = b2;
}

//TILE -- Constructor
Tile::Tile(int x, int y, int w, int h, bool b, bool b2)
{
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;     
    p = b;
    i = 0;
    timer = -1;
    bonus = b2;
}

//~TILE -- Destructor
Tile::~Tile()
{
      
}

//DRAW-- Draw the tile
void Tile::Draw()
{
    Image_DrawImage(vec[i], r);          
}

//ERASE -- Erase the tile
void Tile::Erase()
{
    Image_EraseImage(r);           
}
                   
//GETR -- Getter for r
SDL_Rect Tile::GetR()
{
    return r;
}

//RETURNR -- Return r
SDL_Rect* Tile::ReturnR()
{
    return &r;
}

//SETR -- Setter for r
void Tile::SetR(SDL_Rect rect)
{
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h; 
}

//SETR -- Setter for r
void Tile::SetR(int x, int y, int w, int h)
{
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;     
}

//RETURNVEC -- Getter for vec
vector <SDL_Surface*>* Tile::ReturnVec()
{
    return &vec;       
}

//GETVEC -- Getter for vec
SDL_Surface* Tile::GetVec(int i)
{
    return vec[i];             
}

//ADDTOVEC -- Add a SDL_Surface to the vec
void Tile::AddToVec(SDL_Surface *s)
{
    vec.push_back(s);     
}

//GETI -- Getter for i
int Tile::GetI()
{
    return i;    
}

//SETI -- Setter for i
void Tile::SetI(int n)
{
    i = n;     
}

//SHIFTI -- Shift i
void Tile::ShiftI()
{
    i++;
    if(i == vec.size())
    {
        i = 0;
    }     
}

//GETP -- Getter for p
bool Tile::GetP()
{
    return p;     
}

//SETP -- Setter for p
void Tile::SetP(bool b)
{
    p = b;     
}

//GETT -- Getter for t
Global_tile Tile::GetT()
{
    return t;            
}

//SETT -- Setter for t
void Tile::SetT(Global_tile g)
{
    t = g;     
}

//GETTIMER -- Getter for timer
int Tile::GetTimer()
{
    return timer;
}

//SETTIMER -- Setter for timer
void Tile::SetTimer(int n)
{
    timer = n; 
}

//CHANGETIMER -- Change the timer
void Tile::ChangeTimer(int n)
{
    timer += n;    
}

//MINUSTIMER -- Subtract timer by 1
void Tile::MinusTimer()
{
    timer--;     
}

//GETBONUS -- Getter for bonus
bool Tile::GetBonus()
{
    return bonus;
}

//SETBONUS -- Setter for bonus
void Tile::SetBonus(bool b)
{
    bonus = b;
}

//GETTL -- Getter for tl
bool Tile::GetTL()
{
    return tl;     
}

//SETTL -- Setter for tl
void Tile::SetTL(bool b)
{
    tl = b;     
}

//CHANGETL -- Change tl
void Tile::ChangeTL()
{
    tl = !tl;
}

//GETTR -- Getter for tr
bool Tile::GetTR()
{
    return tr;     
}

//SETTR -- Setter for tr
void Tile::SetTR(bool b)
{
    tr = b;     
}

//CHANGETR -- Change tr
void Tile::ChangeTR()
{
    tr = !tr;
}

//GETBL -- Getter for bl
bool Tile::GetBL()
{
    return bl;     
}

//SETBL -- Setter for bl
void Tile::SetBL(bool b)
{
    bl = b;     
}

//CHANGEBL -- Change bl
void Tile::ChangeBL()
{
    bl = !bl;     
}

//GETBR -- Getter for br
bool Tile::GetBR()
{
    return br;     
}

//SETBR -- Setter for br
void Tile::SetBR(bool b)
{
    br = b;     
}

//CHANGEBR -- Change br
void Tile::ChangeBR()
{
    br = !br;     
}

//GETTL_R -- Getter for tl_r
SDL_Rect Tile::GetTL_R()
{
    return tl_r;         
}

//RETURNTL_R -- Return tl_r
SDL_Rect* Tile::ReturnTL_R()
{
    return &tl_r;
}

//SETTL_R -- Setter for tl_r
void Tile::SetTL_R(SDL_Rect rect)
{
    tl_r.x = rect.x;
    tl_r.y = rect.y;
    tl_r.w = rect.w;
    tl_r.h = rect.h;    
}

//SETTL_R -- Setter for tl_r
void Tile::SetTL_R(int x, int y, int w, int h)
{
    tl_r.x = x;
    tl_r.y = y;
    tl_r.w = w;
    tl_r.h = h;     
}

//GETTR_R -- Getter for tr_r
SDL_Rect Tile::GetTR_R()
{
    return tr_r;         
}

//RETURNTR_R -- Return tr_r
SDL_Rect* Tile::ReturnTR_R()
{
    return &tr_r;
}

//SETTR_R -- Setter for tr_r
void Tile::SetTR_R(SDL_Rect rect)
{
    tr_r.x = rect.x;
    tr_r.y = rect.y;
    tr_r.w = rect.w;
    tr_r.h = rect.h;    
}

//SETTR_R -- Setter for tr_r
void Tile::SetTR_R(int x, int y, int w, int h)
{
    tr_r.x = x;
    tr_r.y = y;
    tr_r.w = w;
    tr_r.h = h;     
}

//GETBL_R -- Getter for bl_r
SDL_Rect Tile::GetBL_R()
{
    return bl_r;         
}

//RETURNBL_R -- Return bl_r
SDL_Rect* Tile::ReturnBL_R()
{
    return &bl_r;
}

//SETBL_R -- Setter for bl_r
void Tile::SetBL_R(SDL_Rect rect)
{
    bl_r.x = rect.x;
    bl_r.y = rect.y;
    bl_r.w = rect.w;
    bl_r.h = rect.h;    
}

//SETBL_R -- Setter for bl_r
void Tile::SetBL_R(int x, int y, int w, int h)
{
    bl_r.x = x;
    bl_r.y = y;
    bl_r.w = w;
    bl_r.h = h;     
}

//GETBR_R -- Getter for br_r
SDL_Rect Tile::GetBR_R()
{
    return br_r;         
}

//RETURNBR_R -- Return br_r
SDL_Rect* Tile::ReturnBR_R()
{
    return &br_r;
}

//SETbR_R -- Setter for br_r
void Tile::SetBR_R(SDL_Rect rect)
{
    br_r.x = rect.x;
    br_r.y = rect.y;
    br_r.w = rect.w;
    br_r.h = rect.h;    
}

//SETBR_R -- Setter for br_r
void Tile::SetBR_R(int x, int y, int w, int h)
{
    br_r.x = x;
    br_r.y = y;
    br_r.w = w;
    br_r.h = h;     
}

//CHECKFULL -- Check if all 4 bools are true or not
bool Tile::CheckFull()
{
    if(tl && tr && bl && br)
    {
        return true;
    }
    else
    {
        return false;    
    }
}
