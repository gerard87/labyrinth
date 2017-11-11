#include <iostream>
#include <string>
#include <GL/glut.h>

#include "maze.h"

#define HEIGHT 600

using namespace std;

void display();
void setScreenSize();
void specialKeys(int key, int x, int y);
void idle();
float col_to_x(int col, int offset);
float row_to_y(int row, int offset);


Maze maze;
int WIDTH;

long last_t = 0;

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

    Maze::Point pos = maze.getCurrentPosition(0);
    maze.set_position(0, col_to_x(pos.col, 0), row_to_y(pos.row, 0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Labyrinth Race");
    
    glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);
    
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);
    
    glutMainLoop();

    return 0;
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
      
            glBegin(GL_QUADS);
            
            glVertex2i(col*WIDTH/maze.getColumns(), HEIGHT - row*HEIGHT/maze.getRows()); 
            glVertex2i(col*WIDTH/maze.getColumns(), HEIGHT - (row+1)*HEIGHT/maze.getRows()); 
            glVertex2i((col+1)*WIDTH/maze.getColumns(), HEIGHT - (row+1)*HEIGHT/maze.getRows()); 
            glVertex2i((col+1)*WIDTH/maze.getColumns(), HEIGHT - row*HEIGHT/maze.getRows());
            
            glEnd();

            if (pos == maze.getCurrentPosition(0)) {

                Maze::Point player = maze.getCurrentPosition(0);
                float square_widht = (WIDTH/maze.getColumns())/2;
                float square_height = (HEIGHT/maze.getRows())/2;

                glColor3f(0.8, 0.5, 0.0);

                glBegin(GL_QUADS);
                glVertex2i(player.x-square_height, player.y-square_widht); 
                glVertex2i(player.x+square_height, player.y-square_widht); 
                glVertex2i(player.x+square_height, player.y+square_widht); 
                glVertex2i(player.x-square_height, player.y+square_widht);
                glEnd();

                glColor3f(0.8, 0.8, 0.8);

            }

        }
        
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {
    Maze::Directions direction;
    Maze::Point pos = maze.getCurrentPosition(0);
    int col_offset;
    int row_offset;
    switch (key) {
        //case 27 :      break;
        case 100:
            direction = Maze::LEFT;
            col_offset = -1;
            row_offset = 0;
            break;
        case 102:
            direction = Maze::RIGHT;
            col_offset = 1;
            row_offset = 0;
            break;
        case 101:
            direction = Maze::DOWN;
            col_offset = 0;
            row_offset = -1;
            break;
        case 103:
            direction = Maze::UP;
            col_offset = 0;
            row_offset = 1;
            break;
    }
    if (maze.move(0, direction)) {
        maze.init_movement(0, col_to_x(pos.col, col_offset), row_to_y(pos.row, row_offset), 100);
        glutPostRedisplay();
    }
}

void idle() {
    long t;
    
    t=glutGet(GLUT_ELAPSED_TIME); 

    if(last_t!=0)
        maze.integrate(0, t-last_t);

    last_t=t;
    
    glutPostRedisplay();
}

float col_to_x(int col, int offset) {
    return (((col+offset)*WIDTH/maze.getColumns()) + ((col+1+offset)*WIDTH/maze.getColumns()))/2;
}

float row_to_y(int row, int offset) {
    return ((HEIGHT - (row+offset)*HEIGHT/maze.getRows()) + (HEIGHT - (row+1+offset)*HEIGHT/maze.getRows()))/2;
}