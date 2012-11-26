//============================================================================
// Name        : tictactoe.cpp
// Author      : MARIN
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <deque>
#include "surface.h"
#include "iothreads.h"
#include "sound.h"

using namespace std;
SDL_mutex *key_mutex;		//mutex for accessing key queues
SDL_mutex *mouse_mutex;		//mutex for accessing mouse queues

extern deque<Point>mouseq;  //queue to save mouse coordinates
extern bool quit;		    //defined in io_threads.cpp to signal all threads to quit

int main(int argc,char *argv[])
{
    SDL_Surface *screen;

    const int VWIDTH = 640;
    const int VHEIGHT = 480;

    Surface surf( VWIDTH, VHEIGHT, "Tic-tac-toe" );
    SDL_SetEventFilter(FilterEvents);

    //create mutexes for accessing key queues and mouse queue
    key_mutex = SDL_CreateMutex();
    mouse_mutex = SDL_CreateMutex();

    if( key_mutex == NULL )
    	printf( "Failed to create key_mutex!\n");
    if( mouse_mutex == NULL )
    	printf( "Failed to create mouse_mutex!\n" );

    SDL_Thread *kthread, *mthread, *gthread, *sthread;
    kthread = SDL_CreateThread( key_thread, NULL);
    mthread = SDL_CreateThread( mouse_thread, NULL);
    gthread = SDL_CreateThread( game_thread, &surf );
    sthread = SDL_CreateThread( sound_thread, &surf );

    while( !quit ) //keep updating screen until some thread wants to quit
    {
      SDL_PumpEvents();
      if( SDL_PeepEvents ( NULL, 0, SDL_PEEKEVENT, SDL_QUITMASK) )
      {
    	  break;
      }
      //Update the screen
      surf.updateSurface();
      SDL_Delay(20);		//give up some CPU time
   }

    //don't exit until all threads are done
    SDL_WaitThread( mthread, NULL );
    SDL_WaitThread( kthread, NULL );
    SDL_WaitThread( gthread, NULL );
    SDL_WaitThread( sthread, NULL );

    //release the resources
    SDL_DestroyMutex( mouse_mutex );
    SDL_DestroyMutex( key_mutex );
    return 0;
}
