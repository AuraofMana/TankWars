#ifndef TTFTEXT_H__
#define TTFTEXT_H__

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>

#include "ProjXGlobal.h"

using std::string;

bool TTF_Initialize(string *file_list); //Initialize font
void TTF_Shutdown(); //Clean up gracefully

void TTF_GiveInfo(string pn, int fs, Uint8 fgR, Uint8 fgG, Uint8 fgB, Uint8 fgA, Uint8 bgR, Uint8 bgG, Uint8 bgB, Uint8 bgA); //Give information regarding path name of the font

void TTF_DisplayText(Global_text t, int total_space, SDL_Rect r); //Display text
void TTF_DisplayText(string str, int total_space, SDL_Rect r); //Display text
void TTF_EraseText(Global_text t, SDL_Rect r); //Erase text over area
void TTF_EraseText(string str, SDL_Rect r); //Erase text over area

#endif
