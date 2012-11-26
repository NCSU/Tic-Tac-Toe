/*
 * point.h
 *
 *  Created on: 02.10.2012
 *      Author: Administrator
 */

#ifndef POINT_H
#define POINT_H
using namespace std;

class Point {
  public:
    int x;
    int y;
    Point () { x = y = 0; }
    Point ( int x1, int y1 ) { x = x1; y = y1; }
    void set ( int x1, int y1 ) { x = x1; y = y1; }
};

#endif
