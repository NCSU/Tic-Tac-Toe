/*
 * iothreads.cpp
 *
 *  Created on: 02.10.2012
 *      Author: Administrator
 */

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_thread.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <deque>
#include "point.h"

using namespace std;

deque<char>keyq(0);	//a queue to save values of keys pressed
deque<Point>mouseq;	//queue to save mouse coordinates
deque<char>numq;	//queue to save digits
bool quit = false;	//global variable

extern SDL_mutex *key_mutex;	//lock to protect accessing key variables
extern SDL_mutex *mouse_mutex;	//lock to protect accessing mouse variables

//Filter SDL events, set by SDL_setEventFilter ( FilterEvents ) in tictactoe.cpp
int FilterEvents(const SDL_Event *event)
{
  static int reallyquit = 0;
  switch (event->type) {

    case SDL_ACTIVEEVENT:

      // See what's happenning
      printf("App %s ", event->active.gain ? "gained" : "lost");
      if( event->active.state & SDL_APPACTIVE )
  	printf("active ");

      if( event->active.state & SDL_APPMOUSEFOCUS )
  	printf("mouse ");

      if( event->active.state & SDL_APPINPUTFOCUS )
  	printf("input ");

      printf("focus\n");

      // See if we are iconified or restored
      if ( event->active.state & SDL_APPACTIVE ) {
  	printf("App has been %s\n",event->active.gain ? "restored" : "iconified");
      }
      return ( 0 );	//drop all these events

    // Queue it if we want to quit.
    case SDL_QUIT:
      printf("Quit demanded\n");
      return ( 1 );

    // Mouse and keyboard events go to threads, queue them
    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      return ( 1 );

    // Drop all other events
    default:
      return ( 0 );
  }
}

//thread to handle mouse events
int mouse_thread( void *unused )
{
  SDL_Event events[10];
  int i, found;
  Uint32 mask;

  // Handle mouse events here
  mask = ( SDL_MOUSEBUTTONDOWNMASK | SDL_MOUSEMOTIONMASK | SDL_QUITMASK );

  while ( !quit ) {	//keep checking mouse events until quit is set

    //handle up to 10 events, events removed from queue
    found = SDL_PeepEvents(events, 10, SDL_GETEVENT, mask);
    for ( i = 0; i < found; ++i ) {
      Point p;
      switch( events[i].type ) {
        case SDL_MOUSEBUTTONDOWN:
          p.x = events[i].button.x; p.y =  events[i].button.y;
    	  SDL_mutexP( mouse_mutex );	//lock before accessing mouse queue
          mouseq.push_back( p );	//save point in mouse queue
    	  SDL_mutexV( mouse_mutex );	//release lock
          break;
	case SDL_QUIT:
	  quit = true;
	  found = 0;			//exit for loop too
	  SDL_mutexV( mouse_mutex );	//wake up any sleeping thread before quit
	  SDL_mutexV( key_mutex );
	  break;
      }
    }
    // Give up some CPU to allow events to arrive
    SDL_Delay( 100 );
  }
  return(0);
}

//thread to handle key events
int key_thread( void *unused )
{
  SDL_Event events[10];
  int i, found;
  Uint32 mask;

  /* Handle keyboard events here */
  mask = ( SDL_KEYDOWNMASK | SDL_KEYUPMASK );
  while ( quit == false ) {
    found = SDL_PeepEvents(events, 10, SDL_GETEVENT, mask);
    SDL_mutexP(key_mutex);	//lock before accessing shared key variables
    for ( i=0; i<found; ++i ) {
      switch(events[i].type) {
  	case SDL_KEYDOWN:
	  char c =  events[i].key.keysym.sym;
      	  keyq.push_back( c );		//put entered key value in queue

    	  /* Allow hitting <ESC> to quit the app */

    	  if ( c == SDLK_ESCAPE )    //quit
      	    quit = true;
    	  else if ( c >= '0' && c <= '8' )
	    numq.push_back( c );   //a digit
	  break;
      } //switch
    } //for
    SDL_mutexV(key_mutex);	//release lock
    /* Give up some CPU to allow events to arrive */
    SDL_Delay( 100 );
  } //while
  cout << "return from keyboard routine\n";
  return ( 0 );
}


