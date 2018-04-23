#include "TDraw.h"

TDraw::TDraw(){}
TDraw::~TDraw(){}

void TDraw::init() {
    setlocale(LC_ALL, "en_US.UTF-8");
	initscr();
	start_color();
	cbreak();
    nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);

	for( short i = 0; i < 8; ++i ) {
		short v = ((float)i/7)*1000;
		init_color(i, v, v, v);
		init_pair(i, i, 0);
	}
	
	status1 = (char*)malloc(64*sizeof(char));
	status2 = (char*)malloc(64*sizeof(char));
    
    width = COLS;
    height = LINES;
    screen_width = width*SYM_WIDTH;
    screen_height = height*SYM_HEIGHT;
    
    screen = (wchar_t*)malloc(height*width*sizeof(wchar_t));
    color_data = (char*)malloc(height*width*sizeof(char));
    
    for(int i = 0; i < height*width; ++i) {
        screen[i] = DOT_BASE;
        color_data[i] = 4;
    }
}

void TDraw::deinit() {
    free(screen);
    free(color_data);
    free(status1);
	free(status2);
	
	endwin();
}

void TDraw::draw() {    
	wchar_t block[2] = {0, '\0'};
	for(int i = 0; i < height*width; ++i) {
		block[0] = screen[i];
		attron(COLOR_PAIR(color_data[i]));
		mvaddwstr(i/width, i%width, block);
		attroff(COLOR_PAIR(color_data[i]));
	}

    attron(COLOR_PAIR(7));
    mvaddstr(height-2, (width-strlen(status1))/2, status1);
    mvaddstr(height-1, (width-strlen(status2))/2, status2);
    attroff(COLOR_PAIR(7));
	
    free(screen);
    free(color_data);
    refresh();
    
    width = COLS;
    height = LINES;

    screen = (wchar_t*)malloc(height*width*sizeof(wchar_t));
    color_data = (char*)malloc(height*width*sizeof(char));
    
    for(int i = 0; i < height*width; ++i) {
        screen[i] = DOT_BASE;
        color_data[i] = 4;
    }
    
    screen_width = width*SYM_WIDTH;
    screen_height = height*SYM_HEIGHT;
}

void TDraw::drawPolygon ( int* x, int* y, int sides, char color ) {
	if( sides <= 0 ) return;
	else if( sides == 1 ) { drawPixel( *x, *y, color); return; }
	else {
		for( int i = 0; i < sides; ++i ) {
			drawLine( x[i], y[i], x[(i+1)%sides], y[(i+1)%sides], color);
		}
	}
}

void TDraw::drawRegPoly ( int x, int y, int rad, int sides, double rot, char color ) {
	int* polyX = new int[sides];
    int* polyY = new int[sides];
    
	for( int i = 0; i < sides; ++i ) {
		polyX[i] = x + rad*cos( ((float)i/sides)*(2*M_PI) + 3*M_PI/2 + rot);
		polyY[i] = y + rad*sin( ((float)i/sides)*(2*M_PI) + 3*M_PI/2 + rot);
	}

	drawPolygon(polyX, polyY, sides, color);
    
    delete[] polyX;
    delete[] polyY;
}

void TDraw::drawRect ( int x, int y, int w, int h, char color ) {
	int rectX[4], rectY[4];
	rectX[0] = x; rectY[0] = y;
	rectX[1] = x; rectY[1] = y + h;
	rectX[2] = x + w; rectY[2] = y + h;
	rectX[3] = x + w; rectY[3] = y;
	drawPolygon( rectX, rectY, 4, color ); 
}

void TDraw::drawLine ( int x1, int y1, int x2, int y2, char color, char color2 ) {
	int w = x2-x1, h = y2-y1;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if( w < 0 ) dx1 = -1; else if( w > 0 ) dx1 = 1;
	if( w < 0 ) dx2 = -1; else if( w > 0 ) dx2 = 1;
	if( h < 0 ) dy1 = -1; else if( h > 0 ) dy1 = 1;
	
	int longest = abs(w);
	int shortest = abs(h);

	if( longest <= shortest ) {
		std::swap(longest, shortest);
		if( h < 0 ) dy2 = -1; else if( h > 0 ) dy2 = 1;
		dx2 = 0;
	}

	int num = longest/2;

	if(color2 < 0) color2 = color;
	int gradient = color;

	for( int i = 0; i <= longest; ++i ) {
		gradient = mapTo( i, 0, longest, color, color2 );
		drawPixel( x1, y1, gradient);
		num += shortest;
		if( num >= longest ) {
			num -= longest;
			x1 += dx1;
			y1 += dy1;
		} else {
			x1 += dx2;
			y1 += dy2;
		}
	}
}

void TDraw::drawCircle ( int x, int y, int r, double precision, char color ) {
	for( float a = 0; a < 2*M_PI; a+=precision ) {
		drawPixel( x + r*cos(a), y + r*sin(a), color );
	}
}

void TDraw::drawPixel ( int x, int y, char color ) {
	if( x < 0 || x > width*SYM_WIDTH-1 || y < 0 || y > height*SYM_HEIGHT-1) return;
	int id = x/SYM_WIDTH + (y/SYM_HEIGHT)*width;

#ifdef ASCII_MODE
    #ifdef ABSOLUTE_MODE
        screen[id] = '@';
    #else	
        if( y%2==0 ) {
            screen[id] = ( screen[id]=='.' ? '@' : '^' );
        } else {
            screen[id] = ( screen[id]=='^' ? '@' : '.' );
        }
    #endif
#elif defined ANSI_MODE    
    #ifdef ABSOLUTE_MODE
        screen[id] = 0x2588;
    #else	
        if( y%2==0 ) {
            screen[id] = ( screen[id]==0x2584 ? 0x2588 : 0x2580 );
        } else {
            screen[id] = ( screen[id]==0x2580 ? 0x2588 : 0x2584 );
        }
    #endif
#else
	screen[id] |= PIXEL_MATRIX[y%SYM_HEIGHT][x%SYM_WIDTH];
#endif
	color_data[id] = color;
}

int TDraw::GetScreenHeight() const {
	return screen_height;
}

int TDraw::GetScreenWidth() const {
	return screen_width;
}

void TDraw::Status1( char str[50] ) {
    free(status1);
    status1 = (char*)malloc(50*sizeof(char));
    strcat(status1, str);
}

void TDraw::Status2( char str[50] ) {
    free(status2);
    status2 = (char*)malloc(50*sizeof(char));
    strcat(status2, str);
}
