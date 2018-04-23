#ifndef TDRAW_H_
#define TDRAW_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <ncursesw/curses.h>
#include <wctype.h>
#include <wchar.h>
#include <locale.h>
#include "Vector.h"

// Symbol pixel dimensions

// #define ASCII_MODE
#define ANSI_MODE

// #define ABSOLUTE_MODE

#ifdef ABSOLUTE_MODE
    #define SYM_WIDTH 1
    #define SYM_HEIGHT 1
#elif defined ASCII_MODE
    #define SYM_WIDTH 1
    #define SYM_HEIGHT 2
#elif defined ANSI_MODE
    #define SYM_WIDTH 1
    #define SYM_HEIGHT 2
#else
    #define SYM_WIDTH 2
    #define SYM_HEIGHT 4
#endif

// Variables

static const wchar_t DOT_BASE = 0x2800;

static const unsigned char PIXEL_MATRIX[4][2] = {
	{0x01, 0x08},
	{0x02, 0x10},
	{0x04, 0x20},
	{0x40, 0x80}
};

class TDraw {

private:

	// Screen pixel array
	wchar_t* screen;
	// Screen pixel color data
	char* color_data;

	// Status char arrays
	char* status1;
	char* status2;

	// Dynamic screen parameters
	int screen_width;
	int screen_height;
	int width;
	int height;


public: 

	TDraw();
	virtual ~TDraw();

	/** Initializing function */
	virtual void init();
	/** Deinitializing function */
	virtual void deinit();

	/** Drawing function */
	virtual void draw ();

	/** Object drawing function */
	virtual void drawPixel ( int x, int y, char color );
	virtual void drawLine ( int x1, int y1, int x2, int y2, char color, char color2 = -1 );
	virtual void drawCircle ( int x, int y, int r, double precision , char color );
	virtual void drawRect ( int x, int y, int w, int h, char color );
	virtual void drawPolygon ( int* x, int* y, int sides, char color );
	virtual void drawRegPoly ( int x, int y, int rad, int sides, double rot , char color );

	int GetScreenHeight() const;
	int GetScreenWidth() const;
    
    void Status1( char str[50] );
    void Status2( char str[50] );
};

#endif /** TDRAW_H_ */
