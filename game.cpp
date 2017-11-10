#include <iostream>
#include <string>
#include <GL/glut.h>

#include "maze.h"

#define HEIGHT 600

using namespace std;

void display();
void setScreenSize();
void specialKeys(int key, int x, int y);    


Maze maze;
int WIDTH;

int main(int argc, char* argv[]) {

    int maze_cols, maze_rows;
    if (argc == 1) {
		maze = Maze(25);
	} else if (argc == 2) {
        sscanf(argv[1], "%d", &maze_rows);
        maze = Maze(maze_rows);
    } else {
	//Read maze dimensions from command line arguments
        if (sscanf(argv[1], "%d", &maze_rows ) + sscanf(argv[2], "%d", &maze_cols) < 2) {
            fprintf( stderr, "%s: invalid maze size value!\n", argv[0]);
            exit(1);
        }
        //Allow only odd dimensions
        if (!(maze_rows % 2) || !(maze_cols % 2 )) {
            fprintf( stderr, "%s: dimensions must be odd!\n", argv[0]);
            exit( 1 );
        }
        //Do not allow negative dimensions
        if (maze_rows <= 0 || maze_cols <= 0) {
            fprintf(stderr, "%s: dimensions must be greater than 0!\n", argv[0]);
            exit(1);
        }
        maze = Maze(maze_rows, maze_cols);
    }

    setScreenSize();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Labyrinth Race");
    
    glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);
    
    glutMainLoop();
}

void setScreenSize() {
    if (maze.isSquared()) {
        WIDTH = HEIGHT;
    } else {
        double ratio;
        ratio = maze.getColumns()/(double)maze.getRows();
        WIDTH = (int)(HEIGHT * ratio);
    }
}

void display() {
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for(int row = 0; row < maze.getRows(); row++)
    for(int col = 0; col < maze.getColumns(); col++) {
        Maze::Point pos = Maze::Point(row, col);
            if (pos == maze.getCurrentPosition(0)) {
                glColor3f(0.8, 0.5, 0.0);
            } else {
            switch (maze.getValue(row,col)) {
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
            }

        glBegin(GL_QUADS);

        glVertex2i(col *  WIDTH/maze.getColumns(), HEIGHT - row * HEIGHT/maze.getRows()); 
        glVertex2i(col*WIDTH/maze.getColumns(), HEIGHT - (row+1)*HEIGHT/maze.getRows()); 
        glVertex2i((col+1)*WIDTH/maze.getColumns(), HEIGHT - (row+1)*HEIGHT/maze.getRows()); 
        glVertex2i((col+1)*WIDTH/maze.getColumns(), HEIGHT - row*HEIGHT/maze.getRows());
        
        glEnd();
    }

    glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {
    Maze::Directions direction;
    switch (key) {
        //case 27 :      break;
        case 100:
            direction = Maze::LEFT;
            break;
        case 102:
            direction = Maze::RIGHT;
            break;
        case 101:
            direction = Maze::DOWN;
            break;
        case 103:
            direction = Maze::UP;
            break;
    }
    if (maze.move(0, direction)) glutPostRedisplay();
}