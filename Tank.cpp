#include "Tank.h"
#include "Image.h"

//TANK -- Constructor
Tank::Tank(SDL_Rect rect, int s)
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
}

//TANK -- Constructor
Tank::Tank(int x, int y, int w, int h, int s)
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
}

//~TANK -- Destructor
Tank::~Tank()
{
    
}

//GIVEINFO -- Give information
void Tank::GiveInfo(int x1, int x2, int y1, int y2)
{
    x_min = x1;
    x_max = x2;
    y_min = y1;
    y_max = y2;
}

//DRAW -- Draw the tank
void Tank::Draw()
{
    Image_DrawImage(vec[i], r);          
}

//ERASE -- Erase the tank
void Tank::Erase()
{
    Image_EraseImage(r);           
}

//MOVEUP -- Move up
bool Tank::MoveUp()
{
    Vx = 0;
    Vy = -speed;
    d = TANK_UP;
    if(r.y - speed >= y_min)
    {
        return true;
    }
    else
    {
        return false;    
    }
}

//MOVEDOWN -- Move down
bool Tank::MoveDown()
{
    Vx = 0;
    Vy = speed;
    d = TANK_DOWN;
    if(r.y + r.h + speed <= y_max)
    {
        return true;
    }
    else
    {
        return false;    
    }
}

//MOVELEFT -- Move left
bool Tank::MoveLeft()
{
    Vx = -speed;
    Vy = 0;
    d = TANK_LEFT;
    if(r.x - speed >= x_min)
    {
        return true;
    }
    else
    {
        return false;    
    } 
}

//MOVERIGHT -- Move right
bool Tank::MoveRight()
{
    Vx = speed;
    Vy = 0;
    d = TANK_RIGHT;
    if(r.x + r.w + speed <= x_max)
    {
        return true;
    }
    else
    {
        return false;    
    }     
}

//FIRE -- Fire a shot
void Tank::Fire()
{
     
}

//RETURNR -- Return a reference to r
SDL_Rect* Tank::ReturnR()
{
    return &r;
}

//GETR -- Getter for r
SDL_Rect Tank::GetR()
{
    return r;      
}

//SETR -- Setter for r
void Tank::SetR(SDL_Rect rect)
{
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h; 
}

//CHANGERX -- Change the r.x
void Tank::ChangeRX(int x)
{
    r.x += x;
}

//CHANGERY -- Change the r.y
void Tank::ChangeRY(int y)
{
    r.y += y;
}

//SETRX - Setter for r.x
void Tank::SetRX(int x)
{
    r.x = x;
}

//SETRY -- Setter for r.y
void Tank::SetRY(int y)
{
    r.y = y;     
}

//RETURNVEC -- Getter for vec
vector <SDL_Surface*>* Tank::ReturnVec()
{
    return &vec;       
}

//GETVEC -- Getter for vec
SDL_Surface* Tank::GetVec(int i)
{
    return vec[i];             
}

//ADDTOVEC -- Add a SDL_Surface to the vec
void Tank::AddToVec(SDL_Surface *s)
{
    vec.push_back(s);     
}

//GETI -- Getter for i
int Tank::GetI()
{
    return i;    
}

//SETI -- Setter for i
void Tank::SetI(int n)
{
    i = n;     
}

//SHIFTI -- Shift i
void Tank::ShiftI()
{
    i++;
    if(i == (int) vec.size()) i = 0;     
}

//GETVX -- Getter for Vx
int Tank::GetVx()
{
    return Vx;    
}

//SETVX -- Setter for Vx
void Tank::SetVx(int n)
{
    Vx = n;     
}

//GETVY -- Getter for Vy
int Tank::GetVy()
{
    return Vy;    
}

//SETVY -- Setter for Vy
void Tank::SetVy(int n)
{
    Vy = n;     
}

//GETD -- Getter for d
Global_tank_dir Tank::GetD()
{
    return d;                
}

//SETD -- Setter for d
void Tank::SetD(Global_tank_dir dir)
{
    d = dir;     
}

//ISALIVE -- Check whether the tank is alive or not
bool Tank::IsAlive()
{
    return alive;     
}

//CHANGEALIVE -- Change the state of being alive
void Tank::ChangeAlive()
{
    alive = !alive;     
}

//GETSPEED -- Getter for speed
int Tank::GetSpeed()
{
    return speed;    
}

//SETSPEED -- Setter for speed
void Tank::SetSpeed(int n)
{
    speed = n;     
}

//GETTOTALH -- Getter for totalh
int Tank::GetTotalH()
{
    return totalh;    
}

//SETTOTALH -- Setter for totalh
void Tank::SetTotalH(int n)
{
    totalh = n;     
}

//CHANGETOTALH -- Change totalh
void Tank::ChangeTotalH(int n)
{
    totalh +=n;
}

//GETCURRH -- Getter for currh
int Tank::GetCurrH()
{
    return currh;    
}

//SETCURRH -- Setter for currh
void Tank::SetCurrH(int n)
{
    currh = n;     
}

//CHANGECURRH -- Change currh
bool Tank::ChangeCurrH(int n)
{
    if(currh + n < 0)
    {
        currh = 0;
        return false;         
    }
    else if(currh + n > totalh)
    {
        currh = totalh;
        return true;    
    }
    else
    {
        currh += n;
        return true;
    }
}

//GETLIVES -- Getter for lives
int Tank::GetLives()
{
    return lives;    
}

//SETLIVES -- Setter for lives
void Tank::SetLives(int n)
{
    lives = n;     
}

//ADDLIFE -- Add one life
void Tank::AddLife()
{
    lives++;     
}

//SUBLIFE -- Subtract a life
bool Tank::SubLife()
{
    if(lives - 1 < 0)
    {
        return false;         
    }
    else
    {
        lives--;
        return true;    
    }
}

//GETSCORE -- Getter for score
int Tank::GetScore()
{
    return score;    
}

//SETSCORE -- Setter for score
void Tank::SetScore(int n)
{
    score = n;     
}

//ADDSCORE -- Add n to the score
void Tank::AddScore(int n)
{
    score += n;     
}

//GETMISSILE -- Getter for missile
Missile* Tank::GetMissile(int i)
{
    return &(mv[i]);         
}

//GETMV -- Getter for mv
vector <Missile>* Tank::GetMV()
{
    return &mv;       
}

//ERASEMV -- Erase an element in mv
void Tank::EraseMV(int i)
{
    mv.erase(mv.begin() + i);
}

//GETTT -- Getter for tt
Global_tank_type Tank::GetTT()
{
    return tt;
}

//SETTT -- Setter for tt
void Tank::SetTT(Global_tank_type type)
{
    tt = type;     
}
