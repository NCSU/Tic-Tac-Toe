/*
 * sound.cpp
 *
 *  Created on: 02.10.2012
 *      Author: Administrator
 */

#include <iostream>
#include <stdlib.h>
#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_thread.h"
#include "surface.h"

using namespace std;

extern bool quit;		//global variable
extern SDL_mutex *key_mutex;
extern bool game_over;
bool musicPlaying = true;
Mix_Chunk *sfx;

void channelFinished( int channel )
{
  Mix_FreeChunk(sfx);
}

//Play the audio chunk
void play_sfx( char *name )
{
  int channel;

  //load wav file
  sfx = Mix_LoadWAV(name);
  //play once
  channel = Mix_PlayChannel(-1, sfx, 0);
  Mix_ChannelFinished( channelFinished);
}

void musicFinished()
{
   //Music is done!
   musicPlaying = false;
}

int sound_thread ( void *surface )
{
  Surface *surf = ( Surface *) surface;
  SDL_Surface *screen = ( SDL_Surface *) surf->getSurface();

  Mix_Music *music;                       //Pointer to our music, in memory
  int audio_rate = 22050;                 //Frequency of audio playback
  Uint16 audio_format = AUDIO_S16SYS;     //Format of the audio we're playing
  int audio_channels = 2;                 //2 channels = stereo
  int audio_buffers = 4096;               //Size of the audio buffers in memory

  //Initialize SDL_mixer with our chosen audio settings
  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
  {
    printf("Unable to initialize audio: %s\n", Mix_GetError());
    return 1;
  }
  //Load our OGG file from disk
  music = Mix_LoadMUS("music.ogg");
  if(music == NULL) {
    printf("Unable to load OGG file: %s\n", Mix_GetError());
    return 1;
  }
  //Play that funky music!
  //  -1 in argument means play music again and again, if 0, means play once
  if(Mix_PlayMusic(music, -1) == -1) {
    printf("Unable to play OGG file: %s\n", Mix_GetError());
    return 1;
  }

  //Make sure that the musicFinished() function is called when the music stops playing
  Mix_HookMusicFinished(musicFinished);

  while ( !quit ) {
    if ( game_over ) {
	Mix_PauseMusic();		//pause music
	musicPlaying = false;
    } else {
      if ( !musicPlaying ) {
	Mix_ResumeMusic();		//resume playing music
	musicPlaying = true;
      }
    }
    SDL_Delay( 100 );			//give up some cpu time for others
  } //while ( !quit )			//quit may be set to true in key_thread
  cout << "quiting sound thread" << endl;
  //Release the memory allocated to our music
  Mix_FadeOutMusic( 1000 );		//fade out music in 1 sec
  Mix_HaltMusic();

  Mix_FreeMusic(music);

  //Need to make sure that SDL_mixer and SDL have a chance to clean up
  Mix_CloseAudio();

  return 0;
}

