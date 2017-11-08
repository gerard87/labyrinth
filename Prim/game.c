#include <stdio.h>
#include <GL/glut.h>

#include "maze.h"


#define WIDTH 1200
#define HEIGHT 600

void display();

int** maze = NULL;
int maze_rows;
int maze_cols;

int main(int argc, char * argv[]){


    if ( argc < 3 )
	{
		fprintf( stderr, "%s: please specify maze dimensions!\n", argv[0] );
		exit( 1 );
	}
	//Read maze dimensions from command line arguments
	if ( sscanf( argv[1], "%d", &maze_rows ) + sscanf( argv[2], "%d", &maze_cols ) < 2 )
	{
		fprintf( stderr, "%s: invalid maze size value!\n", argv[0] );
		exit( 1 );
	}
	//Allow only odd dimensions
	if ( !( maze_rows % 2 ) || !( maze_cols % 2 ) )
	{
		fprintf( stderr, "%s: dimensions must be odd!\n", argv[0] );
		exit( 1 );
	}
	//Do not allow negative dimensions
	if ( maze_rows <= 0 || maze_cols <= 0 )
	{
		fprintf( stderr, "%s: dimensions must be greater than 0!\n", argv[0] );
		exit( 1 );
    }


    maze = generateMaze(maze_rows, maze_cols);
    
    printMaze(maze, maze_rows, maze_cols);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("MAZE");
    
    glutDisplayFunc(display);
    // glutKeyboardFunc(keyboard);
    
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);
    
    glutMainLoop();
}

void display() {
    int row,col;

    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for(row = 0; row < maze_rows; row++)
    for(col = 0; col < maze_cols; col++) {

        switch (maze[row][col]) {
            case 0:
                glColor3f(0.8, 0.8, 0.8);
                break;        
            case 1:
                glColor3f(0.0 ,0.0, 0.8);
                break;
            case 2: 
                glColor3f(0.0 ,0.8, 0.0);
                break;
            case 3: 
                glColor3f(0.8 ,0.0, 0.1);
                break;            
        }

        glBegin(GL_QUADS);

        glVertex2i(col *  WIDTH/maze_cols, HEIGHT - row * HEIGHT/maze_rows); 
        glVertex2i(col*WIDTH/maze_cols, HEIGHT - (row+1)*HEIGHT/maze_rows); 
        glVertex2i((col+1)*WIDTH/maze_cols, HEIGHT - (row+1)*HEIGHT/maze_rows); 
        glVertex2i((col+1)*WIDTH/maze_cols, HEIGHT - row*HEIGHT/maze_rows);
        
        glEnd();
    }

    glutSwapBuffers();
}