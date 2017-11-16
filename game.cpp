#include <iostream>
#include <string>
#include <cstdlib>
#include <climits>
#include <GL/glut.h>

#include "maze.h"
#include "directions.h"
#include "utils.h"

#define HEIGHT 600

using namespace std;

void display();
void moveEnemy();
void setScreenSize();
void specialKeys(int key, int x, int y);
void idle();
int col_to_x(int col, int offset);
int row_to_y(int row, int offset);
void timer (int extra);

Maze maze;
int WIDTH;

long last_t = 0;

void drawStrokeText(char*string,int x,int y,int z)
{
	  char *c;
	  glPushMatrix();
	  glTranslatef(x, y+8,z);
	  glScalef(0.15f, 0.15f,z);
  
	  for (c=string; *c != '\0'; c++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
	  }
	  glPopMatrix();
}

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

    srand(time(NULL));

    setScreenSize();

    for(int i = 0; i < maze.getAgentsNum(); i++) {
        Particle* agent = maze.getAgent(i);
        Point pos = agent->getPosition();

        int colX = col_to_x(pos.getCol(), 0);
        int rowY = row_to_y(pos.getRow(), 0);
        agent->set_position(colX, rowY);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Labyrinth Race");
    
    glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);
    glutTimerFunc(0, timer, 0);
    
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
            Point pos = Point(row, col);
            
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

            for(int i = 0; i < maze.getAgentsNum(); i++) {
                if (i == 0) glColor3f(0.8, 0.5, 0.0);
                else glColor3f(0.6, 0.1, 0.6);
                maze.getAgent(i)->draw((WIDTH/maze.getColumns())/2, (HEIGHT/maze.getRows())/2);
            }
        }

        glColor3f(0,0,0);
	    drawStrokeText("Time left to play: 43s", 25, HEIGHT - 50,0);
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {
    Directions::Direction direction;
    Particle* agent = maze.getAgent(0);
    Point pos = agent->getPosition();
    switch (key) {
        case GLUT_KEY_LEFT:
            direction = Directions::LEFT;
            break;
        case GLUT_KEY_RIGHT:
            direction = Directions::RIGHT;
            break;
        case GLUT_KEY_UP:
            direction = Directions::UP;
            break;
        case GLUT_KEY_DOWN:
            direction = Directions::DOWN;
            break;
        default:
            direction = Directions::STOP;
            break;
    }
    if (maze.move(0, direction)) {
        agent->init_movement(col_to_x(pos.getCol(), direction.x), row_to_y(pos.getRow(), direction.y), 100);
    }
}

void idle() {
    long t;
    
    t = glutGet(GLUT_ELAPSED_TIME); 

    if(last_t != 0) {
        for(int i = 0; i < maze.getAgentsNum(); i++) maze.getAgent(i)->integrate(t-last_t);
    } 

    last_t=t;
    
    glutPostRedisplay();
}

void timer (int extra) {
    moveEnemy();
    glutTimerFunc(100, timer, 0);
}

void moveEnemy() {
    Directions::Direction direction;

    for(int i = 1; i < maze.getAgentsNum(); i++) {
        Particle* agent = maze.getAgent(i);
        Point pos = agent->getPosition();

        std::vector<Directions::Direction> minMoves;
        int minDistance = INT_MAX;
        for(auto const &d : maze.getAvailableMoves(i)){
            Point newPos = Point(pos.getRow() + d.y, pos.getCol() + d.x);
            int manDistance = Utils::manhattanDistance(maze.getPlayerBase(), newPos);
            if (manDistance <= minDistance) {
                minDistance = manDistance;
                if (manDistance < minDistance) minMoves.clear();
                minMoves.push_back(d);
            }
        }

        direction = minMoves[rand() % minMoves.size()];

        if (maze.move(i, direction)) {
            agent->init_movement(col_to_x(pos.getCol(), direction.x), row_to_y(pos.getRow(), direction.y), 100);
        }
    }
}

int col_to_x(int col, int offset) {
    return (int)(((col+offset)*WIDTH/maze.getColumns()) + ((col+1+offset)*WIDTH/maze.getColumns()))/2;
}

int row_to_y(int row, int offset) {
    return (int)((HEIGHT - (row+offset)*HEIGHT/maze.getRows()) + (HEIGHT - (row+1+offset)*HEIGHT/maze.getRows()))/2;
}