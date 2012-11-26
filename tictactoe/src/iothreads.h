/*
 * iothreads.h
 *
 *  Created on: 02.10.2012
 *      Author: Administrator
 */

#ifndef IOTHREADS_H
#define IOTHREADS_H

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "point.h"

using namespace std;
int FilterEvents(const SDL_Event *event);
int mouse_thread( void *unused );
int key_thread( void *unused );
int game_thread( void *unused );

#endif
