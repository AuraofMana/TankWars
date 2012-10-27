#include "Image.h"

//GLOBAL
int Image_window_width = 0;
int Image_window_height = 0;
int Image_window_x_start = 0;
int Image_window_y_start = 0;
int Image_screen_split = 0;

SDL_Surface *Image_g_s = NULL;
SDL_Surface* *Image_arr = NULL;
int Image_len = 0; //Length of Image_arr

//IMAGE_INITIALIZE -- Initialize and ready the SDL_Surface's
bool Image_Initialize(string *file_list)
{
    if(!file_list)
    {
       printf("Image Initialization failed.\n");
       return false;
    }
    int len = 0;
    while(file_list[len] != "***") //Find length
    {
        len++;                   
    }
    Image_len = len; //Save it
    Image_arr = new SDL_Surface*[len]; //Allocate based on length found
    for(int i = 0; i < len; i++)
    {
        Image_arr[i] = SDL_LoadBMP(file_list[i].c_str());
        if(!Image_arr[i]) //If it is still NULL
        {
            printf("Could not load image from %s.\n", file_list[i].c_str());
            return false;
        }      
    }
    return true; //Success
}

//IMAGE_SHUTDOWN -- Shuts down image
void Image_Shutdown()
{
    Image_g_s = NULL;
    if(Image_arr) //If Image_arr is not NULL
    {
        for(int i = 0; i < Image_len; i++) //Free SDL_Surface's
        {
            SDL_FreeSurface(Image_arr[i]);
        }
        delete[] Image_arr; //Deallocate it
        Image_arr = NULL;
    }
}

//IMAGE_GIVEINFO -- Give information
void Image_GiveInfo(int win_width, int win_height, int win_x_start, int win_y_start)
{
    Image_window_width = win_width;
    Image_window_height = win_height;
    Image_window_x_start = win_x_start;
    Image_window_y_start = win_y_start;     
}

//IMAGE_SETTRANSPARENCY -- Set a certain SDL_Surface transparent
void Image_SetTransparency(int index, int fR, int fG, int fB)
{
    SDL_SetColorKey(Image_arr[index], SDL_SRCCOLORKEY, SDL_MapRGB(Image_arr[index]->format, fR, fG, fB));     
}

//IMAGE_SETTRANSPARENCY -- Set a certain SDL_Surface transparent
void Image_SetTransparency(int index)
{
    SDL_SetColorKey(Image_arr[index], SDL_SRCCOLORKEY, SDL_MapRGB(Image_arr[index]->format, 255, 0, 255));  
}

//Set the range of SDL_Surface's transparent
void Image_SetTransparency(int low, int high)
{
    for(int i = low; i < high + 1; i++)
    {
        SDL_SetColorKey(Image_arr[i], SDL_SRCCOLORKEY, SDL_MapRGB(Image_arr[i]->format, 255, 0, 255));        
    }
}

//IMAGE_SETSCREEN -- Set the actual screen
void Image_SetScreen(SDL_Surface *g)
{
    Image_g_s = g;     
}

//IMAGE_UPDATESCREEN -- Update the screen
void Image_UpdateScreen()
{
    if(Image_g_s)
    {
        SDL_Flip(Image_g_s); //Flip the buffer            
    }     
}

//IMAGE_WIPESCREEN -- Erase the entire screen
void Image_WipeScreen()
{
    SDL_Rect r;
    r.x = Image_window_x_start;
    r.y = Image_window_y_start;
    r.w = Image_window_width - r.x;
    r.h = Image_window_height - r.y;
    SDL_FillRect(Image_g_s, &r, SDL_MapRGB(Image_g_s->format, 0, 0, 0));
}

//IMAGE_DRAWIMAGE -- Blit image
void Image_DrawImage(Global_image i, SDL_Rect r)
{
    SDL_BlitSurface(Image_arr[i], NULL, Image_g_s, &r);     
}

//IMAGE_DRAWIMAGE -- Blit surface
void Image_DrawImage(SDL_Surface *s, SDL_Rect r)
{
    SDL_BlitSurface(s, NULL, Image_g_s, &r); 
}

//IMAGE_ERASEIMAGE -- Erase image at area
void Image_EraseImage(SDL_Rect r)
{
    SDL_FillRect(Image_g_s, &r, SDL_MapRGB(Image_g_s->format, 0, 0, 0));
}
