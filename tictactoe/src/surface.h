/*
 * surface.h
 *
 *  Created on: 02.10.2012
 *      Author: Administrator
 */

#ifndef SURFACE_H
#define SURFACE_H

#include<math.h>
#include <SDL/SDL.h>
#include "point.h"

class Surface
{
public:
  Surface(int width, int height, char* windowTitle); //constructor
  void clearScreen();
  void setBackgroundColor(int r, int g, int b);
  void setColor(int r, int g, int b);
  void lineTo( int x, int y);
  void lineTo(Point p);
  void moveTo( int x, int y);
  void moveTo(Point p);
  void moveRel( int dx, int dy);
  Point getCP();
  //for turtle graphics
  void turnTo( float angle );
  void turn( float angle );
  void forward ( int dist, int isVisible );
  SDL_Surface *getSurface();
  void updateSurface();
private:
  Point  CP;         	//current position
  float CD;		//current direction, for turtle graphics
  SDL_Surface *surf;
  Uint16 color;
} ;

#endif

