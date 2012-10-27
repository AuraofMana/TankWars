#include <iostream>
#include <sstream>
#include <fstream>
#include <SDL/SDL_gfxPrimitives.h>

#include "Image.h"
#include "Sound.h"
#include "TTFText.h"
#include "Tile.h"
#include "PlayerTank.h"
#include "NormalTank.h"
#include "Explosion.h"

using std::cout;
using std::endl;
using std::stringstream;
using std::ofstream;
using std::ifstream;

//GLOBAL
//Image Files
string image_list_file[] = 
{
    "Image/Tile/tile_base1.bmp", "Image/Tile/tile_base2.bmp", "Image/Tile/tile_base3.bmp", "Image/Tile/tile_base4.bmp", 
    "Image/Tile/tile_spawn1.bmp", "Image/Tile/tile_spawn2.bmp", "Image/Tile/tile_spawn3.bmp", "Image/Tile/tile_spawn4.bmp", 
    "Image/Tile/tile_normal.bmp", "Image/Tile/tile_road.bmp", "Image/Tile/tile_rock.bmp", "Image/Tile/tile_forest1.bmp", 
    "Image/Tile/tile_forest2.bmp", "Image/Tile/tile_wall.bmp", "Image/Tile/tile_steelwall.bmp", "Image/Tile/tile_water1.bmp", 
    "Image/Tile/tile_water2.bmp","Image/Tile/tile_water3.bmp", 
    
    "Image/PT/tank_up1.bmp", "Image/PT/tank_up2.bmp", "Image/PT/tank_up3.bmp", "Image/PT/tank_down1.bmp", 
    "Image/PT/tank_down2.bmp", "Image/PT/tank_down3.bmp", "Image/PT/tank_left1.bmp", "Image/PT/tank_left2.bmp", 
    "Image/PT/tank_left3.bmp", "Image/PT/tank_right1.bmp", "Image/PT/tank_right2.bmp", "Image/PT/tank_right3.bmp", 
    
    "Image/NT/normal_up1.bmp", "Image/NT/normal_up2.bmp", "Image/NT/normal_up3.bmp", "Image/NT/normal_down1.bmp", 
    "Image/NT/normal_down2.bmp", "Image/NT/normal_down3.bmp", "Image/NT/normal_left1.bmp", "Image/NT/normal_left2.bmp", 
    "Image/NT/normal_left3.bmp", "Image/NT/normal_right1.bmp", "Image/NT/normal_right2.bmp", "Image/NT/normal_right3.bmp", 
    
    "Image/Missile/missile.bmp", 
    
    "Image/Explosion/explosion.bmp", 
    
    "***"
};
//Image X Size
int image_size_x[] =
{
     50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
     
     40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 
     
     40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 
     
     10, 
     
     50
};
//Image Y Size
int image_size_y[] =
{
     50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
     
     40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 
     
     40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 
     
     10, 
     
     50
};
//Sound Files
string sound_list_file[] =
{
    "Sound/LevelStart.wav", "Sound/Victory.wav", "Sound/Defeat.wav", "Sound/Score.wav", 
    "Sound/TankFire1.wav", "Sound/TankDeath1.wav", "Sound/PTDeath.wav", "Sound/BaseDeath.wav", 
    "Sound/MissileExplosion.wav", "Sound/WallExplosion.wav", 
    
    
    "***"                  
};
//Music Files
string music_list_file[] =
{
    "Music/Music1.ogg", "Music/Music2.ogg", "Music/Music3.ogg", "Music/Music4.ogg", 
    "Music/Music5.ogg", "Music/Music6.ogg", "Music/Music7.ogg", "Music/Music8.ogg", 
    "Music/Music9.ogg", "Music/Music10.ogg", "Music/Music11.ogg", "Music/Music12.ogg", 
    "Music/Music13.ogg", "Music/Music14.ogg", "Music/Music15.ogg", "Music/Music16.ogg", 
    "Music/Music17.ogg", 
    
    "***"       
};
//Texts
string text_list_file[] =
{
    "RUNNING", "PAUSED", "VICTORY", "DEFEATED", "SCORE: ", "ENEMY REMAINING: ", "***"       
};

const string font_path_name = "TTF/FreeMono.ttf"; //Font
SDL_Surface *g_screen = NULL; //Main screen
//Entire Window Information
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
SDL_Rect g_rect; //SDL_Rect for the entire window

//Actual Playing Area Locations
const int PLAYING_FIELD_X_START = 137; //Max 15 Tiles
const int PLAYING_FIELD_X_END = WINDOW_WIDTH - PLAYING_FIELD_X_START;
const int PLAYING_FIELD_Y_START = 134; //Max 10 Tiles
const int PLAYING_FIELD_Y_END = WINDOW_HEIGHT - PLAYING_FIELD_Y_START;
const int PLAYING_FIELD_WIDTH = PLAYING_FIELD_X_END - PLAYING_FIELD_X_START;
const int PLAYING_FIELD_HEIGHT = PLAYING_FIELD_Y_END - PLAYING_FIELD_Y_START;

const string CAPTION = "TANK WARS";

//FPS and Time
int frame = 0;
const int FPS_WANTED = 50;
int delay = 15;
Uint32 curr_time = 0;
Uint32 prev_time = 0;

//Text
const int PHASE_X_START = (PLAYING_FIELD_X_START + (PLAYING_FIELD_WIDTH / 2)) - 30;
const int PHASE_X_END = PHASE_X_START + 60;
const int PHASE_Y_START = PLAYING_FIELD_Y_START - 20;
const int PHASE_Y_END = PLAYING_FIELD_Y_START;

const int SCORE_X_START = PLAYING_FIELD_X_START;
const int SCORE_X_END = PLAYING_FIELD_X_START + 80;
const int SCORE_Y_START = PLAYING_FIELD_Y_END;
const int SCORE_Y_END = PLAYING_FIELD_Y_END + 20;

const int ENEMY_REMAINING_X_START = 0;
const int ENEMY_REMAINING_X_END = 50;
const int ENEMY_REMAINING_Y_START = 0;
const int ENEMY_REMAINING_Y_END = 20;

//Maps
int map_size_x = 0;
int map_size_y = 0;
Tile* **Tile_Arr = NULL;
extern SDL_Surface* *Image_arr;
const int TILE_AN = 30; //Tile animation timer
const int PIXEL_DIFF = 0; //For collision realism
const int RELOC_DIFF = 1; //Relocation offset
const int MISSILE_DIFF = 0; //Missile collision realism

vector <Tile*> Spawn_Vec; //Vector of the spawn points
vector <Tile*> Base_Vec; //Vector of base(s)

//Terrain Effects
const int ROAD_BONUS_SPEED = 2;
const int ROCK_BONUS_SPEED = -2;

const int X_TERRAIN_DELAY = 0;
const int Y_TERRAIN_DELAY = 0;

//Explosions
vector <Explosion*> Ex_Vec;

//Tanks
vector <Tank*> Tank_Vec;

//Player Tank
const int PT_LVL1_SPEED = 3;
const int PT_LVL2_SPEED = 4;
const int PT_LVL3_SPEED = 5;
const int PT_LVL4_SPEED = 6;

const int PT_LVL1_ROF = 725;
const int PT_LVL2_ROF = 600;
const int PT_LVL3_ROF = 475;
const int PT_LVL4_ROF = 350;

const int PT_LVL1_DMG = 1;
const int PT_LVL2_DMG = 2;
const int PT_LVL3_DMG = 3;
const int PT_LVL4_DMG = 4;

PlayerTank *P1 = new PlayerTank(392, 589, image_size_x[I_TANK_UP1], image_size_y[I_TANK_UP1], PT_LVL1_SPEED, PT_LVL1_ROF);

//Missile
int Lvl1_Missile_Speed = 6;
int Lvl2_Missile_Speed = 8;
int Lvl3_Missile_Speed = 10;
int Lvl4_Missile_Speed = 12;

//AI Stats
int Spawn_Current = 0; //Current number of enemies on the map
int Spawn_Amount = 3; //Number of enemies on the map at once
int Spawn_Total = 0; //Total amount of enemies for the map
int Spawn_Normal_Total = 0;
int Spawn_Fast_Total = 0;
int Spawn_Heavy_Total = 0;
int Spawn_Suicidal_Total = 0;
int Spawn_Special_Total = 0;
int AI_Chance_To_Shoot = 5;
Global_difficulty difficulty = EASY;

//NORMAL TANK
int NT_Speed = PT_LVL1_SPEED;
int NT_ROF = PT_LVL1_ROF;
int NT_Health = 1;
int NT_Missile_Speed = Lvl1_Missile_Speed;
int NT_Damage = PT_LVL1_DMG;
int NT_Drop_Chance = 20;

//FAST TANK
int FT_Speed = PT_LVL3_SPEED;
int FT_ROF = PT_LVL1_ROF;
int FT_Health = 1;
int FT_Missile_Speed = Lvl3_Missile_Speed;
int FT_Damage = PT_LVL1_DMG;
int FT_Drop_Chance = 25;

//HEAVY TANK
int HT_Speed = PT_LVL1_SPEED;
int HT_ROF = PT_LVL1_ROF;
int HT_Health = 4;
int HT_Missile_Speed = Lvl1_Missile_Speed;
int HT_Damage = PT_LVL3_DMG;
int HT_Drop_Chance = 35;

//SUICIDAL TANK
int ST_Speed = PT_LVL2_SPEED;
int ST_ROF = -1;
int ST_Health = 2;
int ST_Missile_Speed = -1;
int ST_Damage = PT_LVL4_DMG;
int ST_Drop_Chance = 25;

//SPECIAL TANK
int SPT_Speed = PT_LVL4_SPEED;
int SPT_ROF = PT_LVL4_ROF;
int SPT_Health = 4;
int SPT_Missile_Speed = Lvl4_Missile_Speed;
int SPT_Damage = PT_LVL4_DMG;
int SPT_Drop_Chance = 50;


//PROTOTYPES
void Initialize();
void Shutdown();

void GiveInfo();
void UpdateGame();
void EraseWindow();

void DisplayPhase(Global_text t);
void DisplayScore();
string ItoS(int n);

void PlaySound(Global_sound s);

void GrabMap(string pn);
void CreateMap(int *arr, int len);
void DisplayMap();
void DisplayForest();
void DrawMapBorder();

void Tank_Add(Tank *p);
void GivePTInfo();
void GiveNTInfo(int x, int y);
void GiveFTInfo(int x, int y);
void GiveHTInfo(int x, int y);
void GiveSTInfo(int x, int y);
void GiveSPTInfo(int x, int y);

void HandleUp(Tank *p);
void HandleDown(Tank *p);
void HandleLeft(Tank *p);
void HandleRight(Tank *p);
SDL_Rect* CheckTile(int t_x, int t_y, int n);
SDL_Rect* CheckTank(Tank *p, int t_x, int t_y, int n);
int TerrainEffect(int t_x, int t_y);

void MoveMissile(Tank *p);
void MissileVSTileCollision();
void MissileVSOtherMissile();
void PT_MissileVSTank();
void Enemy_MissileVSTank();
void Enemy_MissileVSBase();

void ActivateAI(Tank *p);
void SpawnEnemy();
void SpawnTank(Global_tank_type t);
void MoveAI(Tank *p);
void FiringAI(Tank *p);

void AddExplosion(SDL_Rect rect);
void DisplayExplosion();

//INITIALIZE -- Initialize the game
void Initialize()
{
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        Shutdown();
        exit(1);
    }
    //Start the game screen
    g_screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(!g_screen)
    {
        printf("Couldn't set %d x %d video mode: %s\n", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
        Shutdown();
        exit(1);
    }
    //Initialize Image
    if(!Image_Initialize(image_list_file))
    {
        cout << "Could not initialize Image.h" << endl;
        Shutdown();
        exit(1);                       
    }
    Image_SetScreen(g_screen); //Set game screen
    //Set Transparency
    Image_SetTransparency(I_BASE1, I_EXPLOSION);
    
    //Initialize Sound
    if(!Sound_Initialize(sound_list_file, music_list_file))
    {
        cout << "Could not initialize Sound.h" << endl;
        Shutdown();
        exit(1);                       
    }
    //Initialize TTF
    GiveInfo();
    if(!TTF_Initialize(text_list_file))
    {
        cout << "Could not initialize TTFText.h" << endl;
        Shutdown();
        exit(1);                       
    }
    SDL_WM_SetCaption(CAPTION.c_str(), NULL); //Set caption           
}

//SHUTDOWN -- Shut down the game gracefully
void Shutdown()
{
    //Delete tile array
    if(Tile_Arr)
    {
        for(int i = 0; i < map_size_y; i++)
        {
            delete []Tile_Arr[i];        
        }
        delete []Tile_Arr;
        Tile_Arr = NULL;
    }
    //Delete tank array
    for(int i = 0; i < Tank_Vec.size(); i++)
    {
        delete Tank_Vec[i];        
    }
    Image_Shutdown();
    Sound_Shutdown();
    TTF_Shutdown();
    g_screen = NULL;
    SDL_Quit();     
}

//GIVEINFO -- Give information to the headers
void GiveInfo()
{
    Image_GiveInfo(PLAYING_FIELD_X_END, PLAYING_FIELD_Y_END, PLAYING_FIELD_X_START, PLAYING_FIELD_Y_START);
    TTF_GiveInfo(font_path_name, 14, 0, 100, 255, 0, 0, 0, 0, 0);   
}

//UPDATEGAME -- Update the game by flipping the screen
void UpdateGame()
{
    Image_UpdateScreen();
    frame++;  
}

//ERASEWINDOW -- Erase the entire window
void EraseWindow()
{
    SDL_FillRect(g_screen, &g_rect, SDL_MapRGB(g_screen->format, 0, 0, 0));     
}

//DISPLAYPHASE -- Display current game phase
void DisplayPhase(Global_text t)
{
    SDL_Rect r;
    r.x = PHASE_X_START;
    r.y = PHASE_Y_START;
    r.w = PHASE_X_END - PHASE_X_START;
    r.h = PHASE_Y_END - PHASE_Y_START;
    TTF_DisplayText(t, 8, r);
}

//DISPLAYSCORE -- Display current score for P1
void DisplayScore()
{
    SDL_Rect r;
    r.x = SCORE_X_START;
    r.y = SCORE_Y_START;
    r.w = SCORE_X_END - SCORE_X_START;
    r.h = SCORE_Y_END - SCORE_Y_START;
    TTF_DisplayText(text_list_file[TEXT_SCORE] + ItoS(P1->GetScore()), 12, r);
}

//ITOS -- Convert an integer to a string
string ItoS(int n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

//PLAYSOUND -- Play a sound
void PlaySound(Global_sound s)
{
    Sound_PlaySound(s);
}

//GRABMAP -- Grab the map information
void GrabMap(string pn)
{
    ifstream mapfile; //File
    mapfile.open(pn.c_str()); //Open
    if(mapfile.is_open()) //Check if it is open
    {
        int count = 0;
        int *temp_arr;
        while(mapfile.good()) //While it doesn't end
        {
            switch(count)
            {
                case 0:
                {
                    mapfile >> map_size_x; //Get vertical size
                    break;
                }
                case 1:
                {
                    mapfile >> map_size_y; //Get horizontal size
                    Tile_Arr = new Tile**[map_size_y]; //Set Tile_Arr i's
                    for(int i = 0; i < map_size_y; i++)
                    {
                        *(Tile_Arr + i) = new Tile*[map_size_x]; //Set Tile_Arr j's
                    }
                    break;
                }
                case 2: //Grab the rest of the tile
                {
                    int total_size = map_size_x * map_size_y;
                    temp_arr = new int[total_size]; //Int array to do some tile saving
                    for(int i = 0; i < total_size; i++)
                    {
                        mapfile >> temp_arr[i]; //Grab ints
                    }
                    break;
                }
                case 3:
                {
                    mapfile >> Spawn_Total;
                    break;          
                }
                case 4:
                {
                    mapfile >> Spawn_Normal_Total;
                    break;     
                }
                case 5:
                {
                    mapfile >> Spawn_Fast_Total;
                    break;     
                }
                case 6:
                {
                    mapfile >> Spawn_Heavy_Total;
                    break;     
                }
                case 7:
                {
                    mapfile >> Spawn_Suicidal_Total;
                    break;     
                }
                case 8:
                {
                    mapfile >> Spawn_Special_Total;
                    break;     
                }
            }
            count++;                    
        }
        Spawn_Total = Spawn_Normal_Total + Spawn_Fast_Total + Spawn_Heavy_Total 
        + Spawn_Suicidal_Total + Spawn_Special_Total;
        CreateMap(temp_arr, map_size_x * map_size_y); //Create the map
        delete []temp_arr;
        temp_arr = NULL;
    }
    else //Error while opening
    {
        cout << "Map file cannot be opened." << endl;
        exit(1);
    }
}

//CREATEMAP -- Create the map
/*
base = 0
spawn = 1
normal = 2
road = 3
rock = 4
forest = 5
wall = 6
steelwall = 7
water = 8
*/
void CreateMap(int *arr, int len)
{
    //int x_extra = (15 - map_size_x) / (image_size_x[I_NORMAL] / 2);
    //int y_extra = (15 - map_size_y) / (image_size_y[I_NORMAL] / 2);
    Tile_Arr = new Tile**[map_size_y];
    for(int i = 0; i < map_size_y; i++)
    {
        Tile_Arr[i] = new Tile*[map_size_x];        
    }
    int k = 0;
    for(int i = 0; i < map_size_y; i++)
    {
        for(int j = 0; j < map_size_x; j++)
        {
            SDL_Rect r;
            r.x = (j * image_size_x[I_NORMAL]) + PLAYING_FIELD_X_START; //+ x_extra;
            r.y = (i * image_size_y[I_NORMAL]) + PLAYING_FIELD_Y_START; //+ y_extra;
            r.w = image_size_x[I_NORMAL];
            r.h = image_size_y[I_NORMAL];
            Tile_Arr[i][j] = new Tile(r, true, false);
            switch(arr[k])
            {
                case TILE_BASE:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_BASE1]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_BASE2]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_BASE3]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_BASE4]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_BASE3]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_BASE2]);
                    Tile_Arr[i][j]->SetT(TILE_BASE);
                    Tile_Arr[i][j]->SetP(false);
                    Tile_Arr[i][j]->SetTimer(5);
                    Base_Vec.push_back(Tile_Arr[i][j]);
                    break;   
                }
                case TILE_SPAWN:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_SPAWN1]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_SPAWN2]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_SPAWN3]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_SPAWN4]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_SPAWN3]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_SPAWN2]);
                    Tile_Arr[i][j]->SetT(TILE_SPAWN);
                    Tile_Arr[i][j]->SetTimer(5);
                    Spawn_Vec.push_back(Tile_Arr[i][j]);
                    break;   
                }
                case TILE_NORMAL:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_NORMAL]);
                    Tile_Arr[i][j]->SetT(TILE_NORMAL);
                    break;   
                }
                case TILE_ROAD:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_ROAD]);
                    Tile_Arr[i][j]->SetT(TILE_ROAD);
                    Tile_Arr[i][j]->SetBonus(true);
                    break;   
                }
                case TILE_ROCK:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_ROCK]);
                    Tile_Arr[i][j]->SetT(TILE_ROCK);
                    Tile_Arr[i][j]->SetBonus(true);
                    break;   
                }
                case TILE_FOREST:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_FOREST1]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_FOREST2]);
                    Tile_Arr[i][j]->SetT(TILE_FOREST);
                    Tile_Arr[i][j]->SetTimer(TILE_AN);
                    break;   
                }
                case TILE_WALL:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_WALL]);
                    Tile_Arr[i][j]->SetT(TILE_WALL);
                    Tile_Arr[i][j]->SetP(false);
                    
                    int x_half_size = (image_size_x[I_NORMAL] / 2);
                    int y_half_size = (image_size_y[I_NORMAL] / 2); 
                    Tile_Arr[i][j]->SetTL(true);
                    Tile_Arr[i][j]->SetTR(true);
                    Tile_Arr[i][j]->SetBL(true);
                    Tile_Arr[i][j]->SetBR(true);
                    Tile_Arr[i][j]->SetTL_R(r.x, r.y, x_half_size, y_half_size);
                    Tile_Arr[i][j]->SetTR_R(r.x + x_half_size, r.y, x_half_size, y_half_size);
                    Tile_Arr[i][j]->SetBL_R(r.x, r.y + y_half_size, x_half_size, y_half_size);
                    Tile_Arr[i][j]->SetBR_R(r.x + x_half_size, r.y + y_half_size, x_half_size, y_half_size);
                    
                    break;   
                }
                case TILE_STEELWALL:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_STEELWALL]);
                    Tile_Arr[i][j]->SetT(TILE_STEELWALL);
                    Tile_Arr[i][j]->SetP(false);
                    
                    int x_half_size = (image_size_x[I_NORMAL] / 2);
                    int y_half_size = (image_size_y[I_NORMAL] / 2); 
                    Tile_Arr[i][j]->SetTL(true);
                    Tile_Arr[i][j]->SetTR(true);
                    Tile_Arr[i][j]->SetBL(true);
                    Tile_Arr[i][j]->SetBR(true);
                    Tile_Arr[i][j]->SetTL_R(r.x, r.y, x_half_size, y_half_size);
                    Tile_Arr[i][j]->SetTR_R(r.x + x_half_size, r.y, x_half_size, y_half_size);
                    Tile_Arr[i][j]->SetBL_R(r.x, r.y + y_half_size, x_half_size, y_half_size);
                    Tile_Arr[i][j]->SetBR_R(r.x + x_half_size, r.y + y_half_size, x_half_size, y_half_size);
                    break;   
                }
                case TILE_WATER:
                {
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_WATER1]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_WATER2]);
                    Tile_Arr[i][j]->AddToVec(Image_arr[I_WATER3]);
                    Tile_Arr[i][j]->SetT(TILE_WATER);
                    Tile_Arr[i][j]->SetTimer(TILE_AN);
                    Tile_Arr[i][j]->SetP(false);
                    break;   
                }
            }
            k++;   
        }        
    }
}

//DISPLAYMAP -- Display the map on the screen
void DisplayMap()
{
    for(int i = 0; i < map_size_y; i++)
    {
        for(int j = 0; j < map_size_x; j++)
        {
            if(Tile_Arr[i][j]->GetT() != TILE_FOREST)
            {
                if(Tile_Arr[i][j]->GetTimer() == 0 && Tile_Arr[i][j]->GetTimer() != -1)
                {
                    Tile_Arr[i][j]->ShiftI();
                    Tile_Arr[i][j]->SetTimer(TILE_AN);
                }
                else
                {
                    Tile_Arr[i][j]->MinusTimer();    
                }
                Image_DrawImage(Tile_Arr[i][j]->GetVec(Tile_Arr[i][j]->GetI()), Tile_Arr[i][j]->GetR());
                if(Tile_Arr[i][j]->GetT() == TILE_WALL || Tile_Arr[i][j]->GetT() == TILE_STEELWALL)
                {
                    if(!Tile_Arr[i][j]->GetTL())
                    {
                        Image_EraseImage(Tile_Arr[i][j]->GetTL_R());                           
                    }
                    if(!Tile_Arr[i][j]->GetTR())
                    {
                        Image_EraseImage(Tile_Arr[i][j]->GetTR_R());                           
                    }
                    if(!Tile_Arr[i][j]->GetBL())
                    {
                        Image_EraseImage(Tile_Arr[i][j]->GetBL_R());                           
                    }
                    if(!Tile_Arr[i][j]->GetBR())
                    {
                        Image_EraseImage(Tile_Arr[i][j]->GetBR_R());                           
                    }
                }                                             
            }        
        }        
    }
}

//DISPLAYFOREST -- Display the forest on the map
void DisplayForest()
{
    for(int i = 0; i < map_size_y; i++)
    {
        for(int j = 0; j < map_size_x; j++)
        {
            if(Tile_Arr[i][j]->GetT() == TILE_FOREST)
            {
                if(Tile_Arr[i][j]->GetTimer() == 0 && Tile_Arr[i][j]->GetTimer() != -1)
                {
                    Tile_Arr[i][j]->ShiftI();
                    Tile_Arr[i][j]->SetTimer(TILE_AN);
                }
                else
                {
                    Tile_Arr[i][j]->MinusTimer();    
                }
                Image_DrawImage(Tile_Arr[i][j]->GetVec(Tile_Arr[i][j]->GetI()), Tile_Arr[i][j]->GetR());                    
            }        
        }        
    }     
}

//DRAWMAPBORDER -- Draw the map border
void DrawMapBorder()
{
    Sint16 off_by = 1;
    Sint16 x1 = PLAYING_FIELD_X_START - off_by;
    Sint16 y1 = PLAYING_FIELD_Y_START - off_by;
    Sint16 x2 = PLAYING_FIELD_X_END + off_by;
    Sint16 y2 = PLAYING_FIELD_Y_END + off_by;
    
    Uint8 rc = 0;
    Uint8 gc = 255;
    Uint8 bc = 0;
    Uint8 ac = 255;
    
    rectangleRGBA(g_screen, x1, y1, x2, y2, rc, gc, bc, ac);
}

//TANK_ADD -- Add one to the tank vector
void Tank_Add(Tank *p)
{
    Tank_Vec.push_back(p);
}

//GivePTInfo -- Give information for PT
void GivePTInfo()
{
    P1->GiveInfo(PLAYING_FIELD_X_START, PLAYING_FIELD_X_END, PLAYING_FIELD_Y_START, PLAYING_FIELD_Y_END);
    P1->AddToVec(Image_arr[I_TANK_UP1]);
    P1->AddToVec(Image_arr[I_TANK_UP2]);
    P1->AddToVec(Image_arr[I_TANK_UP3]);
    P1->AddToVec(Image_arr[I_TANK_DOWN1]);
    P1->AddToVec(Image_arr[I_TANK_DOWN2]);
    P1->AddToVec(Image_arr[I_TANK_DOWN3]);
    P1->AddToVec(Image_arr[I_TANK_LEFT1]);
    P1->AddToVec(Image_arr[I_TANK_LEFT2]);
    P1->AddToVec(Image_arr[I_TANK_LEFT3]);
    P1->AddToVec(Image_arr[I_TANK_RIGHT1]);
    P1->AddToVec(Image_arr[I_TANK_RIGHT2]);
    P1->AddToVec(Image_arr[I_TANK_RIGHT3]);
    Tank_Add(P1);
}

//GIVENTINFO -- Give information for NT
void GiveNTInfo(int x, int y)
{
    NormalTank *p = new NormalTank(x, y, image_size_x[I_NORMAL_UP1], image_size_y[I_NORMAL_UP1], NT_Speed, NT_ROF, NT_Health);
    p->GiveInfo(PLAYING_FIELD_X_START, PLAYING_FIELD_X_END, PLAYING_FIELD_Y_START, PLAYING_FIELD_Y_END);
    p->AddToVec(Image_arr[I_NORMAL_UP1]);
    p->AddToVec(Image_arr[I_NORMAL_UP2]);
    p->AddToVec(Image_arr[I_NORMAL_UP3]);
    p->AddToVec(Image_arr[I_NORMAL_DOWN1]);
    p->AddToVec(Image_arr[I_NORMAL_DOWN2]);
    p->AddToVec(Image_arr[I_NORMAL_DOWN3]);
    p->AddToVec(Image_arr[I_NORMAL_LEFT1]);
    p->AddToVec(Image_arr[I_NORMAL_LEFT2]);
    p->AddToVec(Image_arr[I_NORMAL_LEFT3]);
    p->AddToVec(Image_arr[I_NORMAL_RIGHT1]);
    p->AddToVec(Image_arr[I_NORMAL_RIGHT2]);
    p->AddToVec(Image_arr[I_NORMAL_RIGHT3]);
    Tank_Add(p);
}

//GIVEFTINFO -- Give information for FT
void GiveFTInfo(int x, int y)
{
     
}

//GIVEHTINFO -- Give information for HT
void GiveHTInfo(int x, int y)
{
     
}

//GIVESTINFO -- Give information for ST
void GiveSTInfo(int x, int y)
{
     
}

//GIVESPTINFO -- Give information for SPT
void GiveSPTInfo(int x, int y)
{
     
}

//HANDLEUP -- Handle up action
void HandleUp(Tank *p)
{
    if(p->MoveUp())
    {
        //Handle none boundary
        int x = (p->GetR()).x;
        int y = (p->GetR()).y;
        int change = TerrainEffect(x + (image_size_x[I_TANK_UP1] / 2), 
        y + (image_size_y[I_TANK_UP1] / 2));
        int move = -(p->GetSpeed());
        SDL_Rect *r;
        if(r = CheckTile(x, y + move, 0))
        {
            p->SetRY(r->y + r->h + RELOC_DIFF);
        }
        else if(r = CheckTank(p, x, y + move, 0))
        {
            p->SetRY(r->y + r->h + RELOC_DIFF);    
        }
        else
        {
            p->ChangeRY(move);
            p->ChangeRY(-(change));
        }       
    }
    else
    {
        p->SetRY(PLAYING_FIELD_Y_START);
        //Handle boundary hit
    }
    if(p->GetD() != TANK_UP)
    {
        p->SetD(TANK_UP);
        p->SetI(0);              
    }
    else
    {
        if(p->GetI() == 0)
        {
            p->SetI(1);              
        }
        else if(p->GetI() == 1)
        {
            p->SetI(2);    
        }
        else
        {
            p->SetI(0);
        }
    }
}

//HANDLEDOWN -- Handle down action
void HandleDown(Tank *p)
{
    if(p->MoveDown())
    {
        //Handle none boundary
        int x = (p->GetR()).x;
        int y = (p->GetR()).y;
        int change = TerrainEffect(x + (image_size_x[I_TANK_UP1] / 2), 
        y + (image_size_y[I_TANK_UP1] / 2));
        int move = p->GetSpeed();       
        SDL_Rect *r;
        if(r = CheckTile(x, y + move, 0))
        {
            p->SetRY(r->y - image_size_y[I_TANK_UP1] - RELOC_DIFF);
        }
        else if(r = CheckTank(p, x, y + move, 0))
        {
            p->SetRY(r->y - image_size_y[I_TANK_UP1] - RELOC_DIFF);    
        }
        else
        {
            p->ChangeRY(move);
            p->ChangeRY(change);
        }
    }
    else
    {
        //Handle boundary hit
        p->SetRY(PLAYING_FIELD_Y_END - image_size_y[I_TANK_UP1]);
    } 
    if(p->GetD() != TANK_DOWN)
    {
        p->SetD(TANK_DOWN);
        p->SetI(3);              
    }
    else
    {
        if(p->GetI() == 3)
        {
            p->SetI(4);              
        }
        else if(p->GetI() == 4)
        {
            p->SetI(5);    
        }
        else
        {
            p->SetI(3);
        }
    }
}

//HANDLELEFT -- Handle left action
void HandleLeft(Tank *p)
{
    if(p->MoveLeft())
    {   
        //Handle none boundary
        int x = (p->GetR()).x;
        int y = (p->GetR()).y;
        int change = TerrainEffect(x + (image_size_x[I_TANK_UP1] / 2), 
        y + (image_size_y[I_TANK_UP1] / 2));
        int move = -(p->GetSpeed());
        SDL_Rect *r;
        if(r = CheckTile(x + move, y, 1))
        {
            p->SetRX(r->x + r->w + RELOC_DIFF);
        }
        else if(r = CheckTank(p, x + move, y, 1))
        {
            p->SetRX(r->x + r->w + RELOC_DIFF);    
        }
        else
        {
            p->ChangeRX(move);
            p->ChangeRX(-(change));
        }
    }
    else
    {
        //Handle boundary hit
        p->SetRX(PLAYING_FIELD_X_START);
    }
    if(p->GetD() != TANK_LEFT)
    {
        p->SetD(TANK_LEFT);
        p->SetI(6);              
    }
    else
    {
        if(p->GetI() == 6)
        {
            p->SetI(7);              
        }
        else if(p->GetI() == 7)
        {
            p->SetI(8);    
        }
        else
        {
            p->SetI(6);
        }
    }
}

//HANDLERIGHT -- Handle right action
void HandleRight(Tank *p)
{
    if(p->MoveRight())
    {
        //Handle none boundary
        int x = (p->GetR()).x;
        int y = (p->GetR()).y;
        int change = TerrainEffect(x + (image_size_x[I_TANK_UP1] / 2), 
        y + (image_size_y[I_TANK_UP1] / 2));
        int move = p->GetSpeed();       
        SDL_Rect *r;
        if(r = CheckTile(x + move, y, 1))
        {
            p->SetRX(r->x - image_size_x[I_TANK_UP1] - RELOC_DIFF);
        }
        else if(r = CheckTank(p, x + move, y, 1))
        {
            p->SetRX(r->x - image_size_x[I_TANK_UP1] - RELOC_DIFF);    
        }
        else
        {
            p->ChangeRX(move);
            p->ChangeRX(change);
        }
    }
    else
    {
        //Handle boundary hit
        p->SetRX(PLAYING_FIELD_X_END - image_size_x[I_TANK_UP1]);
    }
    if(p->GetD() != TANK_RIGHT)
    {
        p->SetD(TANK_RIGHT);
        p->SetI(9);              
    }
    else
    {
        if(p->GetI() == 9)
        {
            p->SetI(10);              
        }
        else if(p->GetI() == 10)
        {
            p->SetI(11);    
        }
        else
        {
            p->SetI(9);
        }
    }
}

//CHECKTILE -- Check tile to make sure of movement blocking
SDL_Rect* CheckTile(int t_x, int t_y, int n)
{
    int x_diff = 0;
    int y_diff = 0;
    if(n == 0)
    {
        x_diff = PIXEL_DIFF;
    }
    else
    {
        y_diff = PIXEL_DIFF;
    }
    for(int i = 0; i < map_size_y; i++)
    {
        for(int j = 0; j < map_size_x; j++)
        {
            if(!Tile_Arr[i][j]->GetP())
            {
                if((Tile_Arr[i][j]->GetT() != TILE_WALL && Tile_Arr[i][j]->GetT() != TILE_STEELWALL) || 
                Tile_Arr[i][j]->CheckFull())
                {
                    int tile_x = (Tile_Arr[i][j]->GetR()).x;
                    int tile_y = (Tile_Arr[i][j]->GetR()).y;
                    int tile_x_max = tile_x + (Tile_Arr[i][j]->GetR()).w;
                    int tile_y_max = tile_y + (Tile_Arr[i][j]->GetR()).h;
                    int t_x_m = t_x + image_size_x[I_TANK_UP1];
                    int t_y_m = t_y + image_size_y[I_TANK_UP1];
                
                    if((tile_x <= t_x_m - x_diff) && (t_x + x_diff <= tile_x_max) 
                    && (tile_y <= t_y_m - y_diff) && (t_y + y_diff <= tile_y_max))
                    {
                        return Tile_Arr[i][j]->ReturnR();
                    }
                }
                else
                {
                    int t_x_m = t_x + image_size_x[I_TANK_UP1];
                    int t_y_m = t_y + image_size_y[I_TANK_UP1];
                    
                    int tile_x = (Tile_Arr[i][j]->GetTL_R()).x;
                    int tile_y = (Tile_Arr[i][j]->GetTL_R()).y;
                    int tile_x_max = tile_x + (Tile_Arr[i][j]->GetTL_R()).w;
                    int tile_y_max = tile_y + (Tile_Arr[i][j]->GetTL_R()).h;
                    
                    if(Tile_Arr[i][j]->GetTL() && (tile_x <= t_x_m - x_diff) && (t_x + x_diff <= tile_x_max) 
                    && (tile_y <= t_y_m - y_diff) && (t_y + y_diff <= tile_y_max))
                    {
                        return Tile_Arr[i][j]->ReturnTL_R();       
                    }
                    
                    tile_x = (Tile_Arr[i][j]->GetTR_R()).x;
                    tile_y = (Tile_Arr[i][j]->GetTR_R()).y;
                    tile_x_max = tile_x + (Tile_Arr[i][j]->GetTR_R()).w;
                    tile_y_max = tile_y + (Tile_Arr[i][j]->GetTR_R()).h;
                    
                    if(Tile_Arr[i][j]->GetTR() && (tile_x <= t_x_m - x_diff) && (t_x + x_diff <= tile_x_max) 
                    && (tile_y <= t_y_m - y_diff) && (t_y + y_diff <= tile_y_max))
                    {
                        return Tile_Arr[i][j]->ReturnTR_R();       
                    }
                    
                    tile_x = (Tile_Arr[i][j]->GetBL_R()).x;
                    tile_y = (Tile_Arr[i][j]->GetBL_R()).y;
                    tile_x_max = tile_x + (Tile_Arr[i][j]->GetBL_R()).w;
                    tile_y_max = tile_y + (Tile_Arr[i][j]->GetBL_R()).h;
                    
                    if(Tile_Arr[i][j]->GetBL() && (tile_x <= t_x_m - x_diff) && (t_x + x_diff <= tile_x_max) 
                    && (tile_y <= t_y_m - y_diff) && (t_y + y_diff <= tile_y_max))
                    {
                        return Tile_Arr[i][j]->ReturnBL_R();       
                    }
                    
                    tile_x = (Tile_Arr[i][j]->GetBR_R()).x;
                    tile_y = (Tile_Arr[i][j]->GetBR_R()).y;
                    tile_x_max = tile_x + (Tile_Arr[i][j]->GetBR_R()).w;
                    tile_y_max = tile_y + (Tile_Arr[i][j]->GetBR_R()).h;
                    
                    if(Tile_Arr[i][j]->GetBR() && (tile_x <= t_x_m - x_diff) && (t_x + x_diff <= tile_x_max) 
                    && (tile_y <= t_y_m - y_diff) && (t_y + y_diff <= tile_y_max))
                    {
                        return Tile_Arr[i][j]->ReturnBR_R();       
                    }
                }   
            }
        }  
    }
    return NULL;
}

//CHECKTANK -- Collision detection between tanks
SDL_Rect* CheckTank(Tank *p, int t_x, int t_y, int n)
{
    int x_diff = 0;
    int y_diff = 0;
    if(n == 0)
    {
        x_diff = PIXEL_DIFF;
    }
    else
    {
        y_diff = PIXEL_DIFF;
    }
    
    for(int i = 0; i < Tank_Vec.size(); i++)
    {
        if(Tank_Vec[i] == p)
        {
            continue;               
        }
        else
        {
            int x1 = (Tank_Vec[i]->GetR()).x;
            int y1 = (Tank_Vec[i]->GetR()).y;
            int x_m1 = x1 + (Tank_Vec[i]->GetR()).w;
            int y_m1 = y1 + (Tank_Vec[i]->GetR()).h;
            
            int x2 = t_x;
            int y2 = t_y;
            int x_m2 = x2 + (p->GetR()).w;
            int y_m2 = y2 + (p->GetR()).h;
            
            if((x1 <= x_m2 - PIXEL_DIFF) && (x2 + PIXEL_DIFF <= x_m1) 
            && (y1 <= y_m2 - PIXEL_DIFF) && (y2 + PIXEL_DIFF <= y_m1))
            {
                return Tank_Vec[i]->ReturnR();       
            }  
        }
    }
    return NULL;
}

//TERRAINEFFECT -- Add effects according to the terrain
int TerrainEffect(int t_x, int t_y)
{
    for(int i = 0; i < map_size_y; i++)
    {
        for(int j = 0; j < map_size_x; j++)
        {
            if(Tile_Arr[i][j]->GetBonus())
            {
                int tile_x = (Tile_Arr[i][j]->GetR()).x;
                int tile_y = (Tile_Arr[i][j]->GetR()).y;
                int tile_x_max = tile_x + (Tile_Arr[i][j]->GetR()).w;
                int tile_y_max = tile_y + (Tile_Arr[i][j]->GetR()).h;
                int t_x_m = t_x + (image_size_x[I_TANK_UP1] / 2);
                int t_y_m = t_y + (image_size_y[I_TANK_UP1] / 2);
                
                if((tile_x <= t_x_m - X_TERRAIN_DELAY) && (t_x_m + X_TERRAIN_DELAY <= tile_x_max) 
                && (tile_y <= t_y_m - Y_TERRAIN_DELAY) && (t_y_m + Y_TERRAIN_DELAY <= tile_y_max))
                {
                    if(Tile_Arr[i][j]->GetT() == TILE_ROAD)
                    {
                        return ROAD_BONUS_SPEED;                          
                    }
                    else if(Tile_Arr[i][j]->GetT() == TILE_ROCK)
                    {
                        return ROCK_BONUS_SPEED; 
                    }
                }  
            }
        }  
    }
    return 0;
}

//MOVEMISSILE -- Move and display missiles
void MoveMissile(Tank *p)
{
    for(int j = 0; j < (p->GetMV())->size(); j++)
    {
        bool b;
        SDL_Rect old_rect = (p->GetMissile(j))->GetR();
        b = (p->GetMissile(j))->Move();
        if(b)
        {
            (p->GetMissile(j))->Display();
        }
        else
        {
            (p->GetMissile(j))->Erase();
            p->EraseMV(j);
        }
    }
}

//MISSILEVSTILECOLLISION -- Do the collision effects between missile and collision
void MissileVSTileCollision()
{
    for(int i = 0; i < map_size_y; i++)
    {
        for(int j = 0; j < map_size_x; j++)
        {   
            for(int k = 0; k < Tank_Vec.size(); k++)
            {
                for(int l = 0; l < (Tank_Vec[k]->GetMV())->size(); l++)
                {
                    if((Tile_Arr[i][j]->GetT() == TILE_WALL || Tile_Arr[i][j]->GetT() == TILE_STEELWALL))
                    {
                        int tile_x = (Tile_Arr[i][j]->GetR()).x;
                        int tile_y = (Tile_Arr[i][j]->GetR()).y;
                        int tile_x_max = tile_x + (Tile_Arr[i][j]->GetR()).w;
                        int tile_y_max = tile_y + (Tile_Arr[i][j]->GetR()).h;
                
                        int m_x = ((Tank_Vec[k]->GetMissile(l))->GetR()).x;
                        int m_y = ((Tank_Vec[k]->GetMissile(l))->GetR()).y;
                        int m_x_max = m_x + ((Tank_Vec[k]->GetMissile(l))->GetR()).w;
                        int m_y_max = m_y + ((Tank_Vec[k]->GetMissile(l))->GetR()).h;
                
                        if((tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                        && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                        {
                            int b = false;
                               
                            tile_x = (Tile_Arr[i][j]->GetTL_R()).x;
                            tile_y = (Tile_Arr[i][j]->GetTL_R()).y;
                            tile_x_max = tile_x + (Tile_Arr[i][j]->GetTL_R()).w;
                            tile_y_max = tile_y + (Tile_Arr[i][j]->GetTL_R()).h;
                    
                            if(Tile_Arr[i][j]->GetTL() && (tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                            && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                            {
                                Tile_Arr[i][j]->ChangeTL();
                            
                                b = true;
                            }
                        
                            tile_x = (Tile_Arr[i][j]->GetTR_R()).x;
                            tile_y = (Tile_Arr[i][j]->GetTR_R()).y;
                            tile_x_max = tile_x + (Tile_Arr[i][j]->GetTR_R()).w;
                            tile_y_max = tile_y + (Tile_Arr[i][j]->GetTR_R()).h;
                    
                            if(Tile_Arr[i][j]->GetTR() && (tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                            && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                            {
                                Tile_Arr[i][j]->ChangeTR();
                            
                                if(j + 1 < map_size_x && (Tile_Arr[i][j + 1]->GetT() == TILE_WALL || Tile_Arr[i][j + 1]->GetT() == TILE_STEELWALL) 
                                && Tile_Arr[i][j + 1]->GetTL())
                                {
                                    tile_x = (Tile_Arr[i][j + 1]->GetTL_R()).x;
                                    tile_y = (Tile_Arr[i][j + 1]->GetTL_R()).y;
                                    tile_x_max = tile_x + (Tile_Arr[i][j + 1]->GetTL_R()).w;
                                    tile_y_max = tile_y + (Tile_Arr[i][j + 1]->GetTL_R()).h;
                                
                                    if((tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                                    && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                                    {
                                        Tile_Arr[i][j + 1]->ChangeTL();
                                    }
                                }
                            
                                b = true;
                            }
                        
                            tile_x = (Tile_Arr[i][j]->GetBL_R()).x;
                            tile_y = (Tile_Arr[i][j]->GetBL_R()).y;
                            tile_x_max = tile_x + (Tile_Arr[i][j]->GetBL_R()).w;
                            tile_y_max = tile_y + (Tile_Arr[i][j]->GetBL_R()).h;
                    
                            if(Tile_Arr[i][j]->GetBL() && (tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                            && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                            {
                                Tile_Arr[i][j]->ChangeBL();
                            
                                if(i + 1 < map_size_y && (Tile_Arr[i + 1][j]->GetT() == TILE_WALL || Tile_Arr[i + 1][j]->GetT() == TILE_STEELWALL) 
                                && Tile_Arr[i + 1][j]->GetTL())
                                {
                                    tile_x = (Tile_Arr[i + 1][j]->GetTL_R()).x;
                                    tile_y = (Tile_Arr[i + 1][j]->GetTL_R()).y;
                                    tile_x_max = tile_x + (Tile_Arr[i + 1][j]->GetTL_R()).w;
                                    tile_y_max = tile_y + (Tile_Arr[i + 1][j]->GetTL_R()).h;
                                
                                    if((tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                                    && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                                    {
                                        Tile_Arr[i + 1][j]->ChangeTL();
                                    }
                                }
                            
                                b = true;
                            }
                        
                            tile_x = (Tile_Arr[i][j]->GetBR_R()).x;
                            tile_y = (Tile_Arr[i][j]->GetBR_R()).y;
                            tile_x_max = tile_x + (Tile_Arr[i][j]->GetBR_R()).w;
                            tile_y_max = tile_y + (Tile_Arr[i][j]->GetBR_R()).h;
                    
                            if(Tile_Arr[i][j]->GetBR() && (tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                            && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                            {
                                Tile_Arr[i][j]->ChangeBR();
                            
                                if(j + 1 < map_size_x && (Tile_Arr[i][j + 1]->GetT() == TILE_WALL || Tile_Arr[i][j + 1]->GetT() == TILE_STEELWALL) 
                                && Tile_Arr[i][j + 1]->GetBL())
                                {
                                    tile_x = (Tile_Arr[i][j + 1]->GetBL_R()).x;
                                    tile_y = (Tile_Arr[i][j + 1]->GetBL_R()).y;
                                    tile_x_max = tile_x + (Tile_Arr[i][j + 1]->GetBL_R()).w;
                                    tile_y_max = tile_y + (Tile_Arr[i][j + 1]->GetBL_R()).h;
                                
                                    if((tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                                    && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                                    {
                                        Tile_Arr[i][j + 1]->ChangeBL();
                                    }
                                }
                            
                                if(i + 1 < map_size_y && (Tile_Arr[i + 1][j]->GetT() == TILE_WALL || Tile_Arr[i + 1][j]->GetT() == TILE_STEELWALL) 
                                && Tile_Arr[i + 1][j]->GetTR())
                                {
                                    tile_x = (Tile_Arr[i + 1][j]->GetTR_R()).x;
                                    tile_y = (Tile_Arr[i + 1][j]->GetTR_R()).y;
                                    tile_x_max = tile_x + (Tile_Arr[i + 1][j]->GetTR_R()).w;
                                    tile_y_max = tile_y + (Tile_Arr[i + 1][j]->GetTR_R()).h;
                                
                                    if((tile_x <= m_x_max - MISSILE_DIFF) && (m_x + MISSILE_DIFF <= tile_x_max) 
                                    && (tile_y <= m_y_max - MISSILE_DIFF) && (m_y + MISSILE_DIFF <= tile_y_max))
                                    {
                                        Tile_Arr[i + 1][j]->ChangeTR();
                                    }
                                }
                            
                                b = true;
                            }
                        
                            if(b)
                            {
                                //Image_EraseImage((Tank_Vec[k]->GetMissile(l))->GetR());
                                Tank_Vec[k]->EraseMV(l);
                                //PlaySound(SOUND_WALLEXPLOSION);
                                //Play sound here
                                //Explosion here
                            }     
                        }
                    }
                }
            } 
        }
    }
}

//MISSILEVSOTHERMISSILE -- Missile vs. other missiles collision
void MissileVSOtherMissile()
{
   for(int i = 0; i < (P1->GetMV())->size(); i++)
   {
       for(int j = 1; j < Tank_Vec.size(); j++)
       {
           for(int k = 0; k < (Tank_Vec[j]->GetMV())->size(); k++)
           {
               int x1 = ((P1->GetMissile(i))->GetR()).x;
               int y1 = ((P1->GetMissile(i))->GetR()).y;
               int x_m1 = x1 + ((P1->GetMissile(i))->GetR()).w;
               int y_m1 = y1 + ((P1->GetMissile(i))->GetR()).h;
           
               int x2 = ((Tank_Vec[j]->GetMissile(k))->GetR()).x;
               int y2 = ((Tank_Vec[j]->GetMissile(k))->GetR()).y;
               int x_m2 = x2 + ((Tank_Vec[j]->GetMissile(k))->GetR()).w;
               int y_m2 = y2 + ((Tank_Vec[j]->GetMissile(k))->GetR()).h;
                
                if((x1 <= x_m2 - MISSILE_DIFF) && (x2 + MISSILE_DIFF <= x_m1) 
                && (y1 <= y_m2 - MISSILE_DIFF) && (y2 + MISSILE_DIFF <= y_m1))
                {
                    //Image_EraseImage((P1->GetMissile(i))->GetR());
                    P1->EraseMV(i);
                    //Image_EraseImage((Tank_Vec[j]->GetMissile(k))->GetR());
                    Tank_Vec[j]->EraseMV(j);
                    PlaySound(SOUND_MISSILEEXPLOSION);
                    //Play sound here
                    //Explosion here 
                }
           }
       }  
   }
}

//PT_MISSILEVSTANK -- PT Missile vs. Tank collision
void PT_MissileVSTank()
{
    for(int i = 0; i < (P1->GetMV())->size(); i++)
    {
        for(int j = 1; j < Tank_Vec.size(); j++)
        {
            int x1 = ((P1->GetMissile(i))->GetR()).x;
            int y1 = ((P1->GetMissile(i))->GetR()).y;
            int x_m1 = x1 + ((P1->GetMissile(i))->GetR()).w;
            int y_m1 = y1 + ((P1->GetMissile(i))->GetR()).h;
            
            int x2 = (Tank_Vec[j]->GetR()).x;
            int y2 = (Tank_Vec[j]->GetR()).y;
            int x_m2 = x2 + (Tank_Vec[j]->GetR()).w;
            int y_m2 = y2 + (Tank_Vec[j]->GetR()).h;
            
            if((x1 <= x_m2 - MISSILE_DIFF) && (x2 + MISSILE_DIFF <= x_m1) 
            && (y1 <= y_m2 - MISSILE_DIFF) && (y2 + MISSILE_DIFF <= y_m1))
            {
                //Image_EraseImage((P1->GetMissile(i))->GetR());
                P1->EraseMV(i);
                Tank_Vec[j]->ChangeCurrH(-1);
                if(!Tank_Vec[j]->ChangeCurrH(-1))
                {
                    //Play hit sound here
                    PlaySound(SOUND_TANKDEATH1);
                    //Play graphic here
                    Tank_Vec[j]->ChangeAlive();
                    //Image_EraseImage(Tank_Vec[j]->GetR());
                    AddExplosion(Tank_Vec[j]->GetR());
                    PlaySound(SOUND_SCORE);
                    switch(Tank_Vec[j]->GetTT())
                    {
                        case NORMALTANK:
                        {
                            P1->AddScore(10);
                            break;     
                        }
                        case FASTTANK:
                        {
                        
                            break;     
                        }
                        case HEAVYTANK:
                        {
                        
                            break;     
                        }
                        case SUICIDALTANK:
                        {
                        
                            break;     
                        }
                        case SPECIALTANK:
                        {
                        
                            break;     
                        }
                        default: break;               
                    }
                    Tank_Vec.erase(Tank_Vec.begin() + j);
                    Spawn_Current--;
                    //Play death sound here
                    //Add score here
                }       
            }
        }
    }
}

//ENEMY_MISSILEVSTANK -- Enemy Missiles vs. PT collision
void Enemy_MissileVSTank()
{
    for(int i = 1; i < Tank_Vec.size(); i++)
    {
        for(int j = 0; j < (Tank_Vec[i]->GetMV())->size(); j++)
        {
            int x1 = ((Tank_Vec[i]->GetMissile(j))->GetR()).x;
            int y1 = ((Tank_Vec[i]->GetMissile(j))->GetR()).y;
            int x_m1 = x1 + ((Tank_Vec[i]->GetMissile(j))->GetR()).w;
            int y_m1 = y1 + ((Tank_Vec[i]->GetMissile(j))->GetR()).h;
           
            int x2 = (P1->GetR()).x;
            int y2 = (P1->GetR()).y;
            int x_m2 = x2 + (P1->GetR()).w;
            int y_m2 = y2 + (P1->GetR()).h;
            if((x1 <= x_m2 - MISSILE_DIFF) && (x2 + MISSILE_DIFF <= x_m1) 
            && (y1 <= y_m2 - MISSILE_DIFF) && (y2 + MISSILE_DIFF <= y_m1))
            {
                //Image_EraseImage((Tank_Vec[i]->GetMissile(i))->GetR());
                Tank_Vec[i]->EraseMV(j);
                P1->ChangeAlive();
                AddExplosion(P1->GetR());
                PlaySound(SOUND_PTDEATH);
                /*
                if(!P1->ChangeCurrH(-((Tank_Vec[i]->GetMissile(j))->GetDMG())))
                {
                    //Play hit sound here
                    //Play graphic here
                    P1->ChangeAlive();
                    //Image_EraseImage(P1->GetR());
                    //Play death sound here
                    //Add score here
                }
                */
            }
        }     
    }
}

//ENEMY_MISSILEVSBASE -- Enemy Missiles vs. Base collision
void Enemy_MissileVSBase()
{
    for(int i = 1; i < Tank_Vec.size(); i++)
    {
        for(int j = 0; j < (Tank_Vec[i]->GetMV())->size(); j++)
        {
            int x1 = ((Tank_Vec[i]->GetMissile(j))->GetR()).x;
            int y1 = ((Tank_Vec[i]->GetMissile(j))->GetR()).y;
            int x_m1 = x1 + ((Tank_Vec[i]->GetMissile(j))->GetR()).w;
            int y_m1 = y1 + ((Tank_Vec[i]->GetMissile(j))->GetR()).h;
           
            int x2 = (Base_Vec[0]->GetR()).x;
            int y2 = (Base_Vec[0]->GetR()).y;
            int x_m2 = x2 + (Base_Vec[0]->GetR()).w;
            int y_m2 = y2 + (Base_Vec[0]->GetR()).h;
            if((x1 <= x_m2 - MISSILE_DIFF) && (x2 + MISSILE_DIFF <= x_m1) 
            && (y1 <= y_m2 - MISSILE_DIFF) && (y2 + MISSILE_DIFF <= y_m1))
            {
                //Image_EraseImage((Tank_Vec[i]->GetMissile(i))->GetR());
                Tank_Vec[i]->EraseMV(j);
                P1->ChangeAlive();
                AddExplosion(Base_Vec[0]->GetR());
                PlaySound(SOUND_BASEDEATH);
                /*
                if(!P1->ChangeCurrH(-((Tank_Vec[i]->GetMissile(j))->GetDMG())))
                {
                    //Play hit sound here
                    //Play graphic here
                    P1->ChangeAlive();
                    //Image_EraseImage(P1->GetR());
                    //Play death sound here
                    //Add score here
                }
                */
            }
        }     
    }
}

//ACTIVATEAI -- Activate AI on Enemy tanks
void ActivateAI(Tank *p)
{
    if(p->GetTT() != PLAYERTANK)
    {
        MoveAI(p);
        FiringAI(p);
    }  
}

//SPAWNENEMY -- Spawn enemies
void SpawnEnemy()
{
    if(Spawn_Total > 0 && Spawn_Current < Spawn_Amount)
    {
        bool b = false;
        while(!b)
        {
            int n = rand() % 5 + 1;
            switch(n)
            {
                case 1:
                {
                    if(Spawn_Normal_Total != 0)
                    {
                        SpawnTank(NORMALTANK);
                        b = true;                  
                    }
                    break;           
                }
                case 2:
                {
                    if(Spawn_Fast_Total != 0)
                    {
                        SpawnTank(FASTTANK);
                        b = true;                  
                    }
                    break;           
                }
                case 3:
                {
                    if(Spawn_Heavy_Total != 0)
                    {
                        SpawnTank(HEAVYTANK);
                        b = true;                  
                    }
                    break;           
                }
                case 4:
                {
                    if(Spawn_Suicidal_Total != 0)
                    {
                        SpawnTank(SUICIDALTANK);
                        b = true;                  
                    }
                    break;           
                }
                case 5:
                {
                    if(Spawn_Special_Total != 0)
                    {
                        SpawnTank(SPECIALTANK);
                        b = true;                  
                    }
                    break;           
                }         
            }        
        }
        Spawn_Total--;               
    }
}

//SPAWNTANK -- Spawn a tank
void SpawnTank(Global_tank_type t)
{
    int s = rand() % Spawn_Vec.size();
    SDL_Rect sr = Spawn_Vec[s]->GetR();
    switch(t)
    {
        case NORMALTANK:
        {
            GiveNTInfo(sr.x + 5, sr.y + 5);
            break;                
        }
        case FASTTANK:
        {
            GiveFTInfo(sr.x + 5, sr.y + 5);
            break;                
        }
        case HEAVYTANK:
        {
            GiveHTInfo(sr.x + 5, sr.y + 5);
            break;                
        }
        case SUICIDALTANK:
        {
            GiveSTInfo(sr.x + 5, sr.y + 5);
            break;                
        }
        case SPECIALTANK:
        {
            GiveSPTInfo(sr.x + 5, sr.y + 5);
            break;                
        }
    }
    Spawn_Current++;          
}

//MOVEAI -- AI for moving
void MoveAI(Tank *p)
{
    Global_tank_dir dir = p->GetD();
    int x1 = (p->GetR()).x;
    int y1 = (p->GetR()).y;
    
    //bool b = false;
    //int n = rand() % 5 + 1;
    
    switch(dir)
    {
        case TANK_UP:
        {
            HandleUp(p);
            break;     
        }
        case TANK_DOWN:
        {
            HandleDown(p);
            break;     
        }
        case TANK_LEFT:
        {
            HandleLeft(p);
            break;     
        }
        case TANK_RIGHT:
        {
            HandleRight(p);
            break;     
        }           
    }
    
    /*
    switch(dir)
    {
        case TANK_UP:
        {
            if(n > 1)
            {
                HandleUp(p);
            }
            else
            {
                b = true; 
            }
            break;     
        }
        case TANK_DOWN:
        {
            if(n > 1)
            {
                HandleDown(p);
            }
            else
            {
                b = true; 
            }
            break;     
        }
        case TANK_LEFT:
        {
            if(n > 1)
            {
                HandleLeft(p);
            }
            else
            {
                b = true; 
            }
            break;     
        }
        case TANK_RIGHT:
        {
            if(n > 1)
            {
                HandleRight(p);
            }
            else
            {
                b = true; 
            }
            break;     
        }           
    }
    
    if(b)
    {
        Global_tank_dir newdir = dir;
        int o = rand() % 4 + 1;
        while(newdir == dir)
        {
            switch(o)
            {
                case 1:
                {
                    newdir = TANK_UP;
                    HandleUp(p);
                    break;     
                }
                case 2:
                {
                    newdir = TANK_DOWN;
                    HandleDown(p);
                    break;     
                }
                case 3:
                {
                    newdir = TANK_LEFT;
                    HandleLeft(p);
                    break;     
                }
                case 4:
                {
                    newdir = TANK_RIGHT;
                    HandleRight(p);
                    break;     
                }         
            }
            p->SetD(newdir);            
        }   
    }
    */
    
    int x2 = (p->GetR()).x;
    int y2 = (p->GetR()).y;
    Global_tank_dir newdir = dir;
    if(x1 == x2 && y1 == y2)
    {
        while(newdir == dir)
        {
            int n = rand() % 4 + 1;
            switch(n)
            {
                case 1:
                {
                    newdir = TANK_UP;
                    break;     
                }
                case 2:
                {
                    newdir = TANK_DOWN;
                    break;     
                }
                case 3:
                {
                    newdir = TANK_LEFT;
                    break;     
                }
                case 4:
                {
                    newdir = TANK_RIGHT;
                    break;     
                }         
            }
        }
        p->SetD(newdir);
    } 
}

//FIRINGAI -- AI for firing
void FiringAI(Tank *p)
{
    int n = rand() % 100 + 1;
    if(n > (100 - AI_Chance_To_Shoot))
    {
        p->Fire();
    }
}

//ADDEXPLOSION -- Add an explosion to Ex_Vec
void AddExplosion(SDL_Rect rect)
{
    Explosion *ex = new Explosion(rect);
    ex->AddToVec(Image_arr[I_EXPLOSION]);
    Ex_Vec.push_back(ex);
}

//DISPLAYEXPLOSION -- Display the explosions
void DisplayExplosion()
{
    for(int i = 0; i < Ex_Vec.size(); i++)
    {
        if(Ex_Vec[i]->GetTimer() != 0)
        {
            Ex_Vec[i]->Draw();
            Ex_Vec[i]->MinusTimer();                        
        }
        else
        {
           Ex_Vec.erase(Ex_Vec.begin() + i);
        }        
    }     
}

//MAIN -- It's the best function out there!
int main(int argc, char **argv)
{
    g_rect.x = 0;
    g_rect.y = 0;
    g_rect.w = WINDOW_WIDTH;
    g_rect.h = WINDOW_HEIGHT;
    srand((unsigned) (time(0))); //Random
    
    Initialize(); //Initialization Stage
    
    SDL_ShowCursor(0); //Hide cursor
    
    string pn = "Map/mapfile.txt";
    GrabMap(pn); //Load Map
    
    //Initialize Player Tank
    GivePTInfo();
    
    //Game Controlling Booleans
    bool game_running = true;
    bool game_paused = false;
    bool game_menu = false;
    
    //Play Starting sound
    PlaySound(SOUND_LEVELSTART);
    
    while(game_running) //Game loop
    {
        //INPUT
        SDL_Event event;
        while(SDL_PollEvent(&event)) //Poll event
        {
            switch(event.type) //Find out the event value
            {
                case SDL_QUIT: //Exit game due to the 'X' button
                {
                    game_running = false;
                    break;                  
                }
                case SDL_KEYDOWN: //Someone pressed a key
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                        {
                            game_running = false;
                            break;     
                        }
                        case SDLK_LEFT:
                        {
                            HandleLeft(P1);
                            break;     
                        }
                        case SDLK_RIGHT:
                        {
                            HandleRight(P1);
                            break;     
                        }
                        case SDLK_UP:
                        {
                            HandleUp(P1);
                            break;     
                        }
                        case SDLK_DOWN:
                        {
                            HandleDown(P1);
                            break;     
                        }
                        case SDLK_SPACE:
                        {
                            P1->Fire();
                            break;                
                        }
                        case SDLK_p:
                        {
                            if(game_paused) //Already paused
                            {
                                game_paused = false;
                            }
                            else //Not paused yet
                            {
                                game_paused = true;
                            }
                            break;
                        }
                        default: //No input
                        {
                            break;
                        }                            
                    }
                    break;
                }                   
            }                             
        }
        
        //Check Key States
        if(!game_paused && !game_menu)
        {
            Uint8 *keystate = SDL_GetKeyState(NULL);
            if(keystate[SDLK_LEFT]) //Left
            {
                HandleLeft(P1);                      
            }
            else if(keystate[SDLK_RIGHT]) //Right
            {
                HandleRight(P1);      
            }
            else if(keystate[SDLK_UP]) //Up
            {
                HandleUp(P1);  
            }
            else if(keystate[SDLK_DOWN]) //Down
            {
                HandleDown(P1);       
            }
            
            if(keystate[SDLK_SPACE]) //Space
            {
                P1->Fire();                        
            }
        }
        //END INPUT
        
        //UPDATE
        if(!P1->IsAlive())
        {
            DisplayExplosion();
            DisplayPhase(TEXT_DEFEATED);
            SDL_Delay(2000);
            PlaySound(SOUND_DEFEAT);
            game_menu = true;
            SDL_Delay(2000);
            game_running = false;       
        }
        
        if(Spawn_Total == 0 && Spawn_Current == 0)
        {
            //You won!
            DisplayPhase(TEXT_VICTORY);
            PlaySound(SOUND_VICTORY);
            game_menu = true;
            SDL_Delay(9000);  
            game_running = false;              
        }
        
        //END UPDATE
        
        //DISPLAY
        if(!game_paused && !game_menu)
        {
            Sound_PlayMusicRand();
            DisplayPhase(TEXT_RUNNING);
            DisplayScore();
            DrawMapBorder(); //Draw map border
            DisplayMap(); //Map without the forests
            SpawnEnemy(); //Spawn some enemies if possible
            for(int i = 0; i < Tank_Vec.size(); i++)
            {
                Tank_Vec[i]->Draw(); //Display the Tanks
                MoveMissile(Tank_Vec[i]); //Move and draw missiles
                ActivateAI(Tank_Vec[i]); //AI actions
            }
            MissileVSTileCollision();
            MissileVSOtherMissile();
            PT_MissileVSTank();
            Enemy_MissileVSTank();
            Enemy_MissileVSBase();        
            DisplayExplosion();
            DisplayForest(); //Map the forests
            
            curr_time = SDL_GetTicks();
            if(curr_time - prev_time >= 1000)
            {
                //Delay
                if(frame > FPS_WANTED) //Check whether FPS is near the rate wanted
                {
                    delay++; //Increase it to slow down the game         
                }
                else if(frame < FPS_WANTED)
                {     
                    delay--; //Decrease it to speed up the game
                }
                if(delay == 0)
                {
                    delay = 1;          
                }
                frame = 0; //Reset FPS count
                prev_time = curr_time;
            }
        }
        else if(game_paused && !game_menu)
        {
            DisplayPhase(TEXT_PAUSED);
            DisplayScore();     
        }
        else
        {
            DisplayScore();    
        }
        //END DISPLAY
        
        UpdateGame(); //Update game by flipping g_screen
        
        SDL_Delay(delay); //Delay the game for constant fps       
        
        //Erase the entire window
        if(!game_paused && !game_menu)
        {
            EraseWindow();
        }     
    }
    
    Shutdown();
    return 0;    
}
