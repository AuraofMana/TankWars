#ifndef SOUND_H__
#define SOUND_H__

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>

#include "ProjXGlobal.h"

using std::string;

bool Sound_Initialize(string *file_list, string *music_list); //Initialize and ready the SDL_Mixer's
void Sound_Shutdown(); //Clean up and end gracefully

void Sound_PlaySound(Global_sound s); //Play a certain sound

void Sound_PlayMusic(Global_music m); //Play a certain music
void Sound_PlayMusicRand(); //Randomly play a music
void Sound_MusicFinished(); //Finish playing music

#endif
