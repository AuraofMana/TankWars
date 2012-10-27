#ifndef IMAGE_H__
#define IMAGE_H__

#include <SDL/SDL.h>
#include <string>

#include "ProjXGlobal.h"

using std::string;

bool Image_Initialize(string *file_list); //Initialize and ready the SDL_Surface's
void Image_Shutdown(); //Shuts down image

void Image_GiveInfo(int win_width, int win_height, int win_x_start, int win_y_start); //Give information
void Image_SetTransparency(int index, int fR, int fG, int fB); //Set a certain SDL_Surface transparent
void Image_SetTransparency(int index); //Set a certain SDL_Surface transparent
void Image_SetTransparency(int low, int high); //Set the range of SDL_Surface's transparent

void Image_SetScreen(SDL_Surface *g); //Set the actual screen
void Image_UpdateScreen(); //Update the screen
void Image_WipeScreen(); //Erase the entire screen

void Image_DrawImage(Global_image i, SDL_Rect r); //Blit image
void Image_DrawImage(SDL_Surface *s, SDL_Rect r); //Blit surface
void Image_EraseImage(SDL_Rect r); //Erase image at area

#endif
