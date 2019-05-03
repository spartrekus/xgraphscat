
//////////////////////////////////////////
//////////////////////////////////////////
#include <stdio.h>
#define PATH_MAX 2500
#if defined(__linux__) //linux
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__) 
#define MYOS 4  // freebsd
#define PATH_MAX 2500
#else
#define MYOS 0
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>

#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
#include <time.h>

/* include the X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



Display *dis;
int screen;
Window win;
GC gc;

void redraw();

XEvent event;		/* the XEvent declaration !!! */
KeySym key;		/* a dealie-bob to handle KeyPress Events */	

void initscr() 
{
        /* get the colors black and white (see section for details) */        
	unsigned long black,white;

	dis=XOpenDisplay((char *)0);
   	screen=DefaultScreen(dis);
	black=BlackPixel(dis, screen),
	white=WhitePixel(dis, screen);
        // xserver android
   	//win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0,	1260, 690, 5, black, white);
   	win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0,	640, 480, 5, black, white);
	XSetStandardProperties(dis,win,"X11 Window","Hi",None,NULL,0,NULL);
	XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
        gc=XCreateGC(dis, win, 0,0);        
	XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);
};

void endwin() {
	XFreeGC(dis, gc);
	XDestroyWindow(dis,win);
	XCloseDisplay(dis);	
	exit(1);				
};


int getch()
{
     char text[255];
     int valuert = 0;
     if (event.type == KeyPress && XLookupString(&event.xkey, text, 255,&key,0)==1) 
       valuert = text[ 0 ];
     else 
       valuert = 0;

     return valuert;  
}

void erase() {
	XClearWindow(dis, win);
};

void redraw() {
	XClearWindow(dis, win);
};

void mvprintw( int yy , int xx , char *mystring) 
{
      int x = xx; int y= yy;  char text[255];			
      strcpy(text, mystring );
      XDrawString(dis,win,gc,x,y, text, strlen(text));  
};

void mvlinew( int y0, int x0, int y1, int x1)
{
    XPoint points[] = {
      {y0, x0},
      {y1, x1}
    };
    int npoints = sizeof(points)/sizeof(XPoint);
    XDrawLines( dis , win, gc, points, npoints, CoordModeOrigin);
}





void mvpointw( int y0, int x0 )
{
    XDrawPoint(dis, win, gc, x0 , y0 );
}

void mvrectanglew( int y0, int x0, int y1, int x1)
{
   XDrawRectangle(dis, win, gc, y0, x0, y1-y0, x1-x0);
}



void draw_axis( ) 
{
                  double x , y;
                  for( y= -10; y<= 10 ; y++) 
                   for( x= -10; x<= 10 ; x++) 
                    XFillArc( dis, win, gc, 30*x + 150 , 30 * y + 150, 4, 4, 0, 360*64);

                   mvlinew(150,0,150,300) ;  mvlinew( 0,150,300,150);
}


///////////////////////////////////////////
void readfileline( char *filesource )
{
   FILE *source; 
   char lline[PATH_MAX];
   int fileeof = 0;  int pcc = 0; int c  = 0;   int co = 0; 

   float x , y; 

   int i,j, k, separator ; 
   char ptr[PATH_MAX];
   char qtr[PATH_MAX];
   char str[PATH_MAX];
   int rdp ; separator = ';'; 
   float value    = 0; 
   float value1   = 0; 
   float value2   = 0; 
   
   source = fopen( filesource , "r");
   while( fileeof == 0 )
   {
       c = fgetc( source ); 
       if ( c == EOF ) 
         fileeof = 1;

       else if (c == '\r')  
       {                 }

       else if (c == 0x0C )    // ^L char
       {                 }

       else if (c == 0x95 )   //vim <95>
       {                 }

       else
       {
          if ( c != '\n' )
               lline[pcc++]=c;
          else 
          {
           if ( fileeof == 0 )
           if ( c == '\n' ) 
           {
              printf( "%s\n", lline );

                    j = 0;  rdp = 0;  k = 0;
                    strncpy( str, lline, PATH_MAX );
                    for(i=0; str[i]!='\0'; i++)
                    {
                      if ( str[i] == ';' ) 
                         rdp++;

                      if ( str[i] != '\0' ) 
                        if ( rdp == 0 ) 
                            ptr[j++]=str[i];

                      if ( str[i] != '\0' ) 
                        if ( rdp == 1 ) 
                          if ( str[ i ] != separator )
                            qtr[k++]=str[i];

                    } 
                    ptr[j]='\0';
                    qtr[k]='\0';

                    value1 =  atof( ptr );
                    value2 =  atof( qtr );

              x = value1 ; y = value2 ; 
              XFillArc( dis, win, gc, 30*x + 150 , 150 - 30*y , 12, 12, 0, 360*64); //12 size
              lline[pcc++]='\0';
           }
           lline[0]='\0';
           pcc = 0;
          }
       }
       co = c; 
   }
   fclose( source );

                  
           /*
           double x, y;
           for( x= -10; x<= 10 ; x+=0.1 )
           {
                    y = sin( x );
                    XFillArc( dis, win, gc, 30*x + 150 , 150 - 30*y , 4, 4, 0, 360*64);
           }
           */
}





int main()
{	
        int ch ; int gameover = 0;
        char text[255];
	initscr();

	while( gameover == 0 ) 
        {		
		XNextEvent(dis, &event);
		//if (event.type==Expose && event.xexpose.count==0) {
		//	redraw();
		//}
                //mvlinew( 150, 150, 300, 300 ); 
                //mvlinew( 20, 30, 100, 120 ); 
                //mvpointw( 95, 90 );
                //XDrawArc( dis, win, gc, 100, 100, 150, 150, 0, 360);
                //XFillArc( dis, win, gc, 300, 300, 10, 10, 0, 360*64);
                //mvrectanglew( 10, 10, 200, 50); 

                  
                draw_axis();
                readfileline( "scatter.csv" );

                ch = getch(); 

                if      ( ch =='L') {    mvprintw( 250, 250, "line" );   } 
                else if ( ch =='l') {    mvlinew( 20, 30, 100, 120 ); }
                else if ( ch =='r') {    mvrectanglew( 10, 20, 100, 200); }
                else if ( ch =='q') {  gameover = 1 ; }
                else if ( ch =='p') {  mvpointw( 75, 50 ); }

		if (event.type==ButtonPress) 
                {
			int x=event.xbutton.x, y=event.xbutton.y;
                        XSetForeground(dis, gc, 0 );
			mvprintw( y, x , "Point");
			printf( "Y=%d X=%d\n", y, x );
		}

	}
   	endwin();
	return 0;
}

