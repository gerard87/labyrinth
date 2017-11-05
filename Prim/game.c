#include <stdio.h>
#include <GL/glut.h>

#include "maze.h"


#define WIDTH 1200
#define HEIGHT 600

#define MAZE_ROWS 25
#define MAZE_COLS 51

void display();

int** maze = NULL;

int main(int argc, char * argv[]){
   
    maze = generateMaze(MAZE_ROWS, MAZE_COLS);
    
    printMaze(maze, MAZE_ROWS, MAZE_COLS);

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

    for(row = 0; row < MAZE_ROWS; row++)
    for(col = 0; col < MAZE_COLS; col++) {

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

        glVertex2i(col *  WIDTH/MAZE_COLS, HEIGHT - row * HEIGHT/MAZE_ROWS); 
        glVertex2i(col*WIDTH/MAZE_COLS, HEIGHT - (row+1)*HEIGHT/MAZE_ROWS); 
        glVertex2i((col+1)*WIDTH/MAZE_COLS, HEIGHT - (row+1)*HEIGHT/MAZE_ROWS); 
        glVertex2i((col+1)*WIDTH/MAZE_COLS, HEIGHT - row*HEIGHT/MAZE_ROWS);
        
        glEnd();
    }

    glutSwapBuffers();
}