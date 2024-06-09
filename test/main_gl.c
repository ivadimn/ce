#include <GL/gl.h>

void Draw( void ) {
   glClear( GL_COLOR_BUFFER_BIT ); 
   glColor3f( 0.0f, 0.0f, 1.0f );
   glLineWidth( 1 ); 
   glBegin( GL_LINES ); 
   glVertex2f( 0, 0.5f ); 
   glVertex2f( 0, -0.5f ); 
   glEnd();
   glFlush();
}

int main( int argc, char *argv[] ) { 
   glInit( &argc, argv );
   glInitWindowSize( 400, 300 );
   glInitWindowPosition( 100, 100 );
   glCreateWindow( "GL Demo" );
   glDisplayFunc( Draw );
   glClearColor( 0, 0, 0, 0 );
   glMainLoop();
   return 0;
}