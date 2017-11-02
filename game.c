#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

#define WIDTH 800
#define HEIGHT 800

void display();

struct Node *nodes; 
int columns, rows;

int main (int argc, char **argv) {

    //Check argument count
	if ( argc < 3 )
	{
		fprintf( stderr, "%s: please specify maze dimensions!\n", argv[0] );
		exit( 1 );
	}
	//Read maze dimensions from command line arguments
	if ( sscanf( argv[1], "%d", &columns ) + sscanf( argv[2], "%d", &rows ) < 2 )
	{
		fprintf( stderr, "%s: invalid maze size value!\n", argv[0] );
		exit( 1 );
	}
	//Allow only odd dimensions
	if ( !( columns % 2 ) || !( rows % 2 ) )
	{
		fprintf( stderr, "%s: dimensions must be odd!\n", argv[0] );
		exit( 1 );
	}
	//Do not allow negative dimensions
	if ( columns <= 0 || rows <= 0 )
	{
		fprintf( stderr, "%s: dimensions must be greater than 0!\n", argv[0] );
		exit( 1 );
    }

	
    
    generateMaze(columns, rows);
    nodes = getMaze();
    //draw(nodes, COLUMNS, ROWS);


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Maze");
  
    glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
  
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);
  
    glutMainLoop();
    return 0;

}

void display()
{
    int i,j;

    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
  
    for(i=0;i<rows;i++) {
        for(j=0;j<columns;j++) {

            if (getI(nodes, i, j, columns)) {

                glColor3f(0.5,0.5,0.4);
                glBegin(GL_QUADS);

                glVertex2i(j*WIDTH/columns,HEIGHT - i*HEIGHT/rows); 
                glVertex2i((j+1)*WIDTH/columns,HEIGHT - i*HEIGHT/rows); 
                glVertex2i((j+1)*WIDTH/columns,HEIGHT - (i+1)*HEIGHT/rows); 
                glVertex2i(j*WIDTH/columns,HEIGHT - (i+1)*HEIGHT/rows);
          
                glEnd();
            }

        }
    }
  
    glutSwapBuffers();
}