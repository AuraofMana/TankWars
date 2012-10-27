#include "Sound.h"

//GLOBAL
Mix_Chunk* *Sound_arr = NULL;
int Sound_len = 0; //Length of the Sound_arr

Mix_Music* *Music_arr = NULL;
int Music_len = 0; //Length of the Music_arr

bool music_playing = false;

int Sound_channel = 0; //Sound_channel for audio playing

//SOUND_INITIALIZE -- Initialize and ready the SDL_Mixer's
bool Sound_Initialize(string *file_list, string *music_list)
{
    if(!file_list)
    {
       printf("Sound Initialization failed.\n");
       return false;
    }
    
    if(!music_list)
    {
       printf("Sound Initialization failed.\n");
       return false;
    }
    
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_Sound_channels = 2;
    int audio_buffers = 4096;
    if(Mix_OpenAudio(audio_rate, audio_format, audio_Sound_channels, audio_buffers) < 0)
    {
       printf("Unable to initialize audio: %s\n", Mix_GetError());
       return false;
    }
    
    int len = 0;
    while(file_list[len] != "***") //Find length
    {
        len++;                   
    }
    Sound_len = len; //Save it
    Sound_arr = new Mix_Chunk*[len]; //Allocate based on length found
    for(int i = 0; i < len; i++)
    {
        Sound_arr[i] = Mix_LoadWAV(file_list[i].c_str());
        if(!Sound_arr[i]) //If it is still NULL
        {
            printf("Could not load sound from %s: %s\n", file_list[i].c_str(), Mix_GetError());
            return false;
        }      
    }
    
    int len2 = 0;
    while(music_list[len2] != "***") //Find length
    {
        len2++;                   
    }
    Music_len = len2;
    Music_arr = new Mix_Music*[len2];
    for(int i = 0; i < len2; i++)
    {
        Music_arr[i] = Mix_LoadMUS(music_list[i].c_str());
        if(!Music_arr[i])
        {
            printf("Could not load music from %s: %s\n", music_list[i].c_str(), Mix_GetError());
            return false;
        }        
    }

    return true; //Success     
}

//SOUND_SHUTDOWN -- Clean up and end gracefully
void Sound_Shutdown()
{
    //while(Mix_Playing(Sound_channel) != 0); //Wait for all Sound_channel to finish
    Mix_HaltChannel(-1); //Stop all sound in all channels
    if(Sound_arr) //If Sound_arr is not NULL
    {
        for(int i = 0; i < Sound_len; i++) //Free Sounds
        {
            Mix_FreeChunk(Sound_arr[i]);
        }
        delete[] Sound_arr; //Deallocate it
        Sound_arr = NULL;
    }
    
    Mix_HaltMusic(); //Halt all music playing
    if(Music_arr) //If Music_arr is not NULL
    {
        for(int i = 0; i < Music_len; i++) //Free Musics
        {
            Mix_FreeMusic(Music_arr[i]);
        }
        delete[] Music_arr; //Deallocate it
        Music_arr = NULL;           
    }
    
    Mix_CloseAudio();     
}

//SOUND_PLAYSOUND -- Play a certain sound
void Sound_PlaySound(Global_sound s)
{
    Sound_channel = Mix_PlayChannel(-1, Sound_arr[s], 0);
    if(Sound_channel == -1)
    {
        printf("Unable to play %s: %s\n", Sound_arr[s], Mix_GetError());
    } 
}

//SOUND_PLAYMUSIC -- Play a certain music
void Sound_PlayMusic(Global_music m)
{
    if(!music_playing) //If no current music is playing
    {
        if(Mix_PlayMusic(Music_arr[m], 0) == -1) 
	    {
		    printf("Unable to play music file \"%s\": %s\n", Music_arr[m], Mix_GetError());
        }
        music_playing = true;
        Mix_HookMusicFinished(Sound_MusicFinished);
    }
}

//SOUND_PLAYMUSICRAND -- Randomly play a music
void Sound_PlayMusicRand()
{
    if(!music_playing) //If no current music is playing
    {
        int m = rand() % Music_len;
        if(Mix_PlayMusic(Music_arr[m], 0) == -1) 
	    {
		    printf("Unable to play music file \"%s\": %s\n", Music_arr[m], Mix_GetError());
        }
        music_playing = true;
        Mix_HookMusicFinished(Sound_MusicFinished);                  
    }
}

//SOUND_MUSICFINISHED -- Finished playing music
void Sound_MusicFinished()
{
    music_playing = false; //Set to false
}
