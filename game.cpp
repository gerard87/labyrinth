#include <iostream>
#include <string>
#include <cstdlib>
#include <climits>
#include <GL/glut.h>
#include <math.h>
#include <string.h>

#include "maze.h"
#include "directions.h"
#include "utils.h"

#define HEIGHT 1000

#define PI 3.1416

using namespace std;

void display();
void moveEnemy();
void setScreenSize();
void specialKeys(int key, int x, int y);
void idle();
int col_to_x(int col, int offset);
int row_to_y(int row, int offset);
void timer(int extra);
void timer2(int extra);
void PositionObserver(float alpha,float beta,int radi);
void keyboard(unsigned char c,int x,int y);
void printCube(int row, int col);
void printSquare(int row, int col);
float calc_angle(Directions::Direction direction, Directions::Direction orientation);
void moveAgent(int agentIndex, Directions::Direction direction);

Maze maze;
int WIDTH;

long last_t = 0;

float timeleft = 140;

int anglealpha = 75;
int anglebeta = 35;

void drawStrokeText(std::string text,int x,int y,int z) {
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, WIDTH - 1, 0.0, HEIGHT - 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(50, HEIGHT - 50);
    void * font = GLUT_BITMAP_HELVETICA_18;
    for (string::iterator i = text.begin(); i != text.end(); ++i) {
        char c = *i;
        glColor3d(1.0, 0.0, 0.0);
        glutBitmapCharacter(font, c);
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

int main(int argc, char* argv[]) {

    int maze_cols, maze_rows;
    if (argc == 1) {
		maze = Maze(25);
	} else if (argc == 2) {        
        sscanf(argv[1], "%d", &maze_rows);
        maze = Maze(maze_rows);
    } else {
        int debug = 0;
        if (argc == 3 && strcmp(argv[2],"-d") == 0 || argc == 4 && strcmp(argv[3],"-d") == 0) {
            debug = 1;
        }  
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
        if (debug) {
            maze = Maze(debug, maze_rows, maze_cols);
        } else {
            maze = Maze(maze_rows, maze_cols);
        }
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Labyrinth Race");
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(1000, timer2, 0);
    
    
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

void PositionObserver(float alpha,float beta,int radi)
{
  float x,y,z;
  float upx,upy,upz;
  float modul;

  x = (float)radi*cos(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);
  y = (float)radi*sin(beta*2*PI/360.0);
  z = (float)radi*sin(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);

  if (beta>0)
    {
      upx=-x;
      upz=-z;
      upy=(x*x+z*z)/y;
    }
  else if(beta==0)
    {
      upx=0;
      upy=1;
      upz=0;
    }
  else
    {
      upx=x;
      upz=z;
      upy=-(x*x+z*z)/y;
    }


  modul=sqrt(upx*upx+upy*upy+upz*upz);

  upx=upx/modul;
  upy=upy/modul;
  upz=upz/modul;

  gluLookAt(x,y,z,    0.0, 0.0, 0.0,     upx,upy,upz);
}

void display() {
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    PositionObserver(anglealpha,anglebeta,1000);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH*0.6,WIDTH*0.6,-HEIGHT*0.6,HEIGHT*0.6,10,2000);
  
    glMatrixMode(GL_MODELVIEW);
  
    glPolygonMode(GL_FRONT,GL_FILL);
    //glPolygonMode(GL_BACK,GL_LINE);
    glPolygonMode(GL_BACK,GL_FILL);

    glColor3f(0.7, 0.8, 0.7);
    glBegin(GL_QUADS);
    glVertex3i(-1 * (3 * WIDTH), -1, -1 * 3 * HEIGHT);
    glVertex3i(-1 * (3 * WIDTH), -1, 3 * HEIGHT);
    glVertex3i(3 * WIDTH, -1, 3 * HEIGHT); 
    glVertex3i(3 * WIDTH, -1, -1 * 3 * HEIGHT); 
    glEnd();

    for(int row = 0; row < maze.getRows(); row++)
        for(int col = 0; col < maze.getColumns(); col++) {
            Point pos = Point(row, col);
            
            switch (maze.getValue(row,col)) {
                case 0:
                    glColor3f(0.8, 0.8, 0.8);
                    printSquare(row,col);
                    break;        
                case 1:
                    glColor3f(0.0, 0.0, 1.0);
                    printCube(row, col);
                    break;
                case 2: 
                    glColor3f(0.0, 0.8, 0.0);
                    printSquare(row, col);
                    break;
                case 3: 
                    glColor3f(0.8, 0.0, 0.1);
                    printSquare(row, col);
                    break;            
            } 
      
                 
        }

    for(int i = 0; i < maze.getAgentsNum(); i++) {
        if (i == 0) glColor3f(0.8, 0.5, 0.0);
        else glColor3f(0.6, 0.1, 0.6);
        maze.getAgent(i)->draw((WIDTH/maze.getColumns())/2, (HEIGHT/maze.getRows())/2, WIDTH, HEIGHT);
    }

    drawStrokeText("Time left to play: " + std::to_string((int)(timeleft)) +"s", 25, HEIGHT - 50, 0);

    glutSwapBuffers();
}

void printSquare(int row, int col) {
    int w = WIDTH;
    int h = HEIGHT;
    glBegin(GL_QUADS);
    glVertex3i((col*w/maze.getColumns())-(w/2), 0, (row*h/maze.getRows())-(h/2));
    glVertex3i(((col+1)*w/maze.getColumns())-(w/2), 0, (row*h/maze.getRows())-(h/2));
    glVertex3i(((col+1)*w/maze.getColumns())-(w/2),0, ((row+1)*h/maze.getRows())-(h/2)); 
    glVertex3i((col*w/maze.getColumns())-(w/2), 0, ((row+1)*h/maze.getRows())-(h/2)); 
    glEnd();
}

void printCube(int row, int col) {   
    int squareL = WIDTH/maze.getColumns();
    int z = (((col+1)*WIDTH/maze.getColumns()) - (col*WIDTH/maze.getColumns()))/2; 
    glBegin(GL_QUADS);
    glVertex3i((col*WIDTH/maze.getColumns())-(WIDTH/2), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i(((col+1)*WIDTH/maze.getColumns())-(WIDTH/2), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i(((col+1)*WIDTH/maze.getColumns())-(WIDTH/2),z, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i((col*WIDTH/maze.getColumns())-(WIDTH/2), z, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i((col*squareL)-(WIDTH/2), 0, ((row*squareL)) - (HEIGHT/2));
    glVertex3i((col*squareL)-(WIDTH/2), 0, (((row+1)*squareL))- (HEIGHT/2));
    glVertex3i(((col+1)*squareL)-(WIDTH/2),0, (((row+1)*squareL))- (HEIGHT/2));
    glVertex3i(((col+1)*squareL)-(WIDTH/2), 0, ((row*squareL))- (HEIGHT/2));
    glEnd();

    glColor3f(0.0 ,0.5, 0.8);

    glBegin(GL_QUADS);
    glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), 0, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)),0, (row*HEIGHT/maze.getRows())-(HEIGHT/2)); 
    glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), z, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), 0, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)),0, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glEnd();


    glBegin(GL_QUADS);
    glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), z, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)),0, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), 0, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), z, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), z, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)),0, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), 0, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glEnd();

}

void specialKeys(int key, int x, int y) {
    Directions::Direction direction;
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
    moveAgent(0, direction);
}

void moveAgent(int agentIndex, Directions::Direction direction) {
    Particle* agent = maze.getAgent(agentIndex);
    Point pos = agent->getPosition();
    float angle = calc_angle(direction, agent->getOrientation());
    if(angle != 0) agent->init_rotate(angle, 100);
    agent->setOrientation(direction);
    if (maze.move(agentIndex, direction)) {
        agent->init_movement(col_to_x(pos.getCol(), direction.x), row_to_y(pos.getRow(), direction.y), 150);
    }
}

void keyboard(unsigned char c,int x,int y) {
  int i,j;

  if (c=='i' && anglebeta<=(90-4))
    anglebeta=(anglebeta+3);
  else if (c=='k' && anglebeta>=(-90+4))
    anglebeta=anglebeta-3;
  else if (c=='j')
    anglealpha=(anglealpha+3)%360;
  else if (c=='l')
    anglealpha=(anglealpha-3+360)%360;

  glutPostRedisplay();
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

void timer2(int extra) {
    timeleft -= 1.0;
    glutPostRedisplay();
    glutTimerFunc(1000, timer2, 0);
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

        moveAgent(i, direction);
    }
}

int col_to_x(int col, int offset) {
    return (int)(((col+offset)*(WIDTH)/maze.getColumns()) + ((col+1+offset)*(WIDTH)/maze.getColumns()))/2;
}

int row_to_y(int row, int offset) {
    return (int)((row+offset)*(HEIGHT)/maze.getRows()) + ((row+1+offset)*(HEIGHT)/maze.getRows())/2 - (row+offset)*(HEIGHT)/maze.getRows()/2;
}

float calc_angle(Directions::Direction direction, Directions::Direction orientation) {
    if(direction == Directions::LEFT) {
        if(orientation == Directions::UP) return 90.0f;
        else if (orientation == Directions::DOWN) return -90.0f;
        else if(orientation == Directions::RIGHT) return 180.0f;
        else return 0;
    } else if(direction == Directions::RIGHT) {
        if(orientation == Directions::UP) return -90.0f;
        else if (orientation == Directions::DOWN) return 90.0f;
        else if(orientation == Directions::LEFT) return 180.0f;
        else return 0;
    } else if(direction == Directions::UP) {
        if(orientation == Directions::DOWN) return 180.0f;
        else if(orientation == Directions::LEFT) return -90.0f;
        else if(orientation == Directions::RIGHT) return 90.0f;
        else return 0;
    } else if(direction == Directions::DOWN) {
        if(orientation == Directions::UP) return 180.0f;
        else if(orientation == Directions::LEFT) return 90.0f;
        else if(orientation == Directions::RIGHT) return -90.0f;
        else return 0;
    }
}