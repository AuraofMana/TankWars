#include "TTFText.h"
#include "Image.h"

//GLOBAL
string TTF_path_name; //Path name for the font
int TTF_font_size = 0; //Font size
Uint8 TTF_fR = 0;
Uint8 TTF_fG = 0;
Uint8 TTF_fB = 0;
Uint8 TTF_fA = 0;
Uint8 TTF_bR = 0;
Uint8 TTF_bG = 0;
Uint8 TTF_bB = 0;
Uint8 TTF_bA = 0;

TTF_Font *font = NULL; //The font
string *TTF_arr = NULL; //Length of the TTF_arr
int TTF_len = 0;

//TTF_Initialize -- Initialize font
bool TTF_Initialize(string *file_list)
{
    if(!file_list)
    {
       printf("Sound Initialization failed.\n");
       return false;
    }
    int len = 0;
    while(file_list[len] != "***") //Find length
    {
        len++;                   
    }
    TTF_len = len;
    TTF_arr = new string[len]; //Allocate based on length found
    for(int i = 0; i < len; i++) //Put it all inside
    {
        TTF_arr[i] = file_list[i];        
    }
    
    if(!TTF_WasInit() && TTF_Init() == -1) //Initialize TTF
    {
       printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
       return false;
    }
    font = TTF_OpenFont(TTF_path_name.c_str(), TTF_font_size); //Attempt to load the font
    if(!font)
    {
        printf("Unable to load font: %s %s \n", TTF_path_name.c_str(), TTF_GetError());
        return false;
    }
    return true; //Success
}

//TTF_SHUTDOWN-- Clean up gracefully
void TTF_Shutdown()
{
    delete[] TTF_arr; //Deallocate it
    TTF_arr = NULL;
    if(font)
    {
        TTF_CloseFont(font);
        font = NULL;        
    }
    if(TTF_WasInit())
    {
        TTF_Quit();
    }     
}

//TTF_GIVEINFO -- Give information regarding path name of the font
void TTF_GiveInfo(string pn, int fs, Uint8 fgR, Uint8 fgG, Uint8 fgB, Uint8 fgA, Uint8 bgR, Uint8 bgG, Uint8 bgB, Uint8 bgA)
{
    TTF_path_name = pn;
    TTF_font_size = fs;
    TTF_fR = fgR;
    TTF_fG = fgG;
    TTF_fB = fgB;
    TTF_fA = fgA;
    TTF_bR = bgR;
    TTF_bG = bgG;
    TTF_bB = bgB;
    TTF_bA = bgA;
}

//TTF_DISPLAYTEXT -- Display text (Global_text)
void TTF_DisplayText(Global_text t, int total_space, SDL_Rect r)
{
    SDL_Color fc = {TTF_fR, TTF_fG, TTF_fB, TTF_fA}; //Font
    SDL_Color bg = {TTF_bR, TTF_bG, TTF_bB, TTF_bA}; //Background
    string str = TTF_arr[t];
    for(int i = 0; i < total_space - TTF_arr[t].length(); i++) //Add spaces to equal it out
    {
        str += " ";        
    }
    SDL_Surface *text = TTF_RenderText_Shaded(font, str.c_str(), fc, bg);
    Image_DrawImage(text, r); //Draw the image
    SDL_FreeSurface(text);
}

//TTF_DISPLAYTEXT -- Display text (string)
void TTF_DisplayText(string str, int total_space, SDL_Rect r)
{
    SDL_Color fc = {TTF_fR, TTF_fG, TTF_fB, TTF_fA}; //Font
    SDL_Color bg = {TTF_bR, TTF_bG, TTF_bB, TTF_bA}; //Background
    int str_len = str.length();
    for(int i = 0; i < total_space - str_len; i++) //Add spaces to equal it out
    {
        str += " ";        
    }
    SDL_Surface *text = TTF_RenderText_Shaded(font, str.c_str(), fc, bg);
    Image_DrawImage(text, r); //Draw the image
    SDL_FreeSurface(text);     
}

//TTF_ERASETEXT -- Erase text over area
void TTF_EraseText(Global_text t, SDL_Rect r)
{
    SDL_Color fc = {TTF_bR, TTF_bG, TTF_bB, TTF_bA}; //Font
    SDL_Color bg = {TTF_bR, TTF_bG, TTF_bB, TTF_bA}; //Background
    string erase_text;
    for(int i = 0; i < TTF_arr[t].length(); i++) //Add spaces to equal it out
    {
        erase_text += " ";        
    }
    SDL_Surface *text = TTF_RenderText_Shaded(font, erase_text.c_str(), fc, bg);
    Image_DrawImage(text, r); //Draw the image
    SDL_FreeSurface(text);  
}

//TTF_ERASETEXT -- Erase text over area
void TTF_EraseText(string str, SDL_Rect r)
{
    SDL_Color fc = {TTF_bR, TTF_bG, TTF_bB, TTF_bA}; //Font
    SDL_Color bg = {TTF_bR, TTF_bG, TTF_bB, TTF_bA}; //Background
    string erase_text;
    for(int i = 0; i < str.length(); i++) //Add spaces to equal it out
    {
        erase_text += " ";        
    }
    SDL_Surface *text = TTF_RenderText_Shaded(font, erase_text.c_str(), fc, bg);
    Image_DrawImage(text, r); //Draw the image
    SDL_FreeSurface(text);  
}
