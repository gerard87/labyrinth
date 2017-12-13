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
#include "jpeglib.h"

#define HEIGHT 1000
#define PI 3.1416
#define DIM 64
using namespace std;

void display();
void moveEnemy();
void setScreenSize();
void specialKeys(int key, int x, int y);
void idle();
void timer(int extra);
void timer2(int extra);
void PositionObserver(float alpha,float beta,int radi);
void keyboard(unsigned char c,int x,int y);
void printCube(int row, int col);
void printSquare(int row, int col);
void printFloor(int row, int col);
void moveAgent(int agentIndex, Directions::Direction direction);
unsigned char* LoadTexture(char *filename,int dim);
void ReadJPEG(char *filename,unsigned char **image,int *width, int *height);
void setTexture (unsigned char* texture);
void setMaterialForTexture();
float * getNormalFromSquare(int *p1, int *p2, int *p3);


Maze maze;
int WIDTH;

long last_t = 0;

float timeleft = 140;

int anglealpha = 75;
int anglebeta = 35;

unsigned char *grass, *wall, *top;

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

        int colX = Utils::col_to_x(pos.getCol(), 0, WIDTH, maze.getColumns());
        int rowY = Utils::row_to_y(pos.getRow(), 0, HEIGHT, maze.getRows());
        agent->set_position(colX, rowY);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Labyrinth Race");
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(1000, timer2, 0);

    glBindTexture(GL_TEXTURE_2D,0);
    grass = LoadTexture((char*)"img/grass.jpg",DIM);
    wall = LoadTexture((char*)"img/wall.jpg",DIM);
    top = LoadTexture((char*)"img/top.jpg",DIM);
    
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

    GLint position[4];
    GLfloat color[4];
    GLfloat material[4];

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

    glDisable(GL_TEXTURE_2D);


    //-- Ambient light
    position[0]=0; position[1]=0; position[2]=0; position[3]=1; 
    glLightiv(GL_LIGHT0,GL_POSITION,position);
    
    color[0]=0.1; color[1]=0.1; color[2]=0.1; color[3]=1;
    glLightfv(GL_LIGHT0,GL_AMBIENT,color);

    color[0]=0.0; color[1]=0.0; color[2]=0.0; color[3]=1;
    glLightfv(GL_LIGHT0,GL_DIFFUSE,color);
    glLightfv(GL_LIGHT0,GL_SPECULAR,color);

    glEnable(GL_LIGHT0);
    color[0]=0.0; color[1]=0.0; color[2]=0.0; color[3]=1;
    glLightfv(GL_LIGHT0,GL_AMBIENT,color);
    glLightfv(GL_LIGHT0,GL_SPECULAR,color);
    //-- End ambient


    material[0]=0.7; material[1]=0.8; material[2]=0.7; material[3]=1.0; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
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
                    printFloor(row,col);
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


    GLfloat *dir;
    GLfloat up[] = {0,1,-1};
    GLfloat down[] = {0,1,1};
    GLfloat left[] = {-1,1,0};
    GLfloat right[] = {1,1,0};
    GLfloat light;
        
    for(int i = 0; i < maze.getAgentsNum(); i++) {
        if (i == 0) {material[0]=0.8; material[1]=0.5; material[2]=0.0; material[3]=1.0; }
        else { material[0]=0.6; material[1]=0.1; material[2]=0.6; material[3]=1.0; }
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);

        Particle *agent = maze.getAgent(i);        

        agent->draw((WIDTH/maze.getColumns())/2, (HEIGHT/maze.getRows())/2, WIDTH, HEIGHT);

        if (i == 0) {
            light = GL_LIGHT1;
        } else if (i == 1) {
            light = GL_LIGHT2;
        }

        if(agent->getOrientation() == Directions::LEFT) {
            dir = left;
        } else if(agent->getOrientation() == Directions::RIGHT) {
            dir = right;
        } else if(agent->getOrientation() == Directions::UP) {
            dir = up;
        } else if(agent->getOrientation() == Directions::DOWN) {
            dir = down;
        }

        //-- Direccional
        position[0]=agent->getX()-WIDTH/2; position[1]=15; position[2]=agent->getY()-HEIGHT/2; position[3]=1; 
        glLightiv(light,GL_POSITION,position);
        
        glLightfv (light,GL_SPOT_DIRECTION, dir);

        color[0]=0.8; color[1]=0.8; color[2]=0.8; color[3]=1;
        glLightfv(light,GL_DIFFUSE,color);
        
        glLightf(light,GL_CONSTANT_ATTENUATION,1);
        glLightf(light,GL_LINEAR_ATTENUATION,0.0);
        glLightf(light,GL_QUADRATIC_ATTENUATION,0.0);

        glLightf(light,GL_SPOT_CUTOFF,60.0);
        glLightf(light,GL_SPOT_EXPONENT, 2.0);
        
        glEnable(light);
        //--End
    }

    drawStrokeText("Time left to play: " + std::to_string((int)(timeleft)) +"s", 25, HEIGHT - 50, 0);

    glutSwapBuffers();
}

void setMaterialForTexture() {
    GLfloat texture_material[] = {1.0,1.0,1.0,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, texture_material);
}

void printFloor(int row, int col) {
    int w = WIDTH;
    int h = HEIGHT;

    setMaterialForTexture();
    setTexture(grass);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0,1.0); glVertex3i((col*w/maze.getColumns())-(w/2), 0, (row*h/maze.getRows())-(h/2));
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(1.0,1.0); glVertex3i((col*w/maze.getColumns())-(w/2), 0, ((row+1)*h/maze.getRows())-(h/2)); 
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(1.0,0.0); glVertex3i(((col+1)*w/maze.getColumns())-(w/2),0, ((row+1)*h/maze.getRows())-(h/2)); 
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0,0.0); glVertex3i(((col+1)*w/maze.getColumns())-(w/2), 0, (row*h/maze.getRows())-(h/2));
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void printSquare(int row, int col) {
    int w = WIDTH;
    int h = HEIGHT;

    glBegin(GL_QUADS);
    glVertex3i((col*w/maze.getColumns())-(w/2), 0, (row*h/maze.getRows())-(h/2));
    glVertex3i((col*w/maze.getColumns())-(w/2), 0, ((row+1)*h/maze.getRows())-(h/2)); 
    glVertex3i(((col+1)*w/maze.getColumns())-(w/2),0, ((row+1)*h/maze.getRows())-(h/2)); 
    glVertex3i(((col+1)*w/maze.getColumns())-(w/2), 0, (row*h/maze.getRows())-(h/2));
    glEnd();
}

void printCube(int row, int col) {   
    int squareL = WIDTH/maze.getColumns();
    int z = (((col+1)*WIDTH/maze.getColumns()) - (col*WIDTH/maze.getColumns()))/2; 

    int p1[3];
    int p2[3];
    int p3[3];
    int p4[3];

    setMaterialForTexture();
    setTexture(top);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);

    p1[0] = (col*WIDTH/maze.getColumns())-(WIDTH/2);
    p1[1] = z;
    p1[2] = (row*HEIGHT/maze.getRows())-(HEIGHT/2);

    p2[0] = (col*WIDTH/maze.getColumns())-(WIDTH/2);
    p2[1] = z;
    p2[2] = ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2);

    p3[0] = ((col+1)*WIDTH/maze.getColumns())-(WIDTH/2);
    p3[1] = z;
    p3[2] = ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2);

    p4[0] = ((col+1)*WIDTH/maze.getColumns())-(WIDTH/2);
    p4[1] = z;
    p4[2] = (row*HEIGHT/maze.getRows())-(HEIGHT/2);

    float * normal = getNormalFromSquare(p1, p2, p3);

    glBegin(GL_QUADS);
    glNormal3f(normal[0], normal[1], normal[2]);
    glTexCoord2f(0.0,1.0); glVertex3i(p1[0], p1[1], p1[2]);
    glNormal3f(normal[0], normal[1], normal[2]);
    glTexCoord2f(1.0,1.0); glVertex3i(p2[0], p2[1], p2[2]);
    glNormal3f(normal[0], normal[1], normal[2]);
    glTexCoord2f(1.0,0.0); glVertex3i(p3[0], p3[1], p3[2]);
    glNormal3f(normal[0], normal[1], normal[2]);
    glTexCoord2f(0.0,0.0); glVertex3i(p4[0], p4[1], p4[2]);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0,1.0); glVertex3i((col*squareL)-(WIDTH/2), 0, ((row*squareL)) - (HEIGHT/2));
    glTexCoord2f(1.0,1.0); glVertex3i(((col+1)*squareL)-(WIDTH/2), 0, ((row*squareL))- (HEIGHT/2));
    glTexCoord2f(1.0,0.0); glVertex3i(((col+1)*squareL)-(WIDTH/2),0, (((row+1)*squareL))- (HEIGHT/2));
    glTexCoord2f(0.0,0.0); glVertex3i((col*squareL)-(WIDTH/2), 0, (((row+1)*squareL))- (HEIGHT/2));
    glEnd();

    setTexture(wall);

    // N
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,1.0); glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glTexCoord2f(1.0,1.0); glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glTexCoord2f(1.0,0.0); glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)),0, (row*HEIGHT/maze.getRows())-(HEIGHT/2)); 
    glTexCoord2f(0.0,0.0); glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), 0, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glEnd();

    // W
    glBegin(GL_QUADS);
    //glNormal3f(1.0, 0.0, 0.0);
    glTexCoord2f(0.0,1.0); glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), z, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glTexCoord2f(1.0,1.0); glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glTexCoord2f(1.0,0.0); glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)),0, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glTexCoord2f(0.0,0.0); glVertex3i(((col*WIDTH/maze.getColumns())-(WIDTH/2)), 0, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glEnd();

    // E
    glBegin(GL_QUADS);
    //glNormal3f(-1.0, 0.0, 0.0);
    glTexCoord2f(1.0,1.0); glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), z, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glTexCoord2f(1.0,0.0); glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), 0, ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2));
    glTexCoord2f(0.0,0.0); glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)),0, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glTexCoord2f(0.0,1.0); glVertex3i((((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)), z, (row*HEIGHT/maze.getRows())-(HEIGHT/2));
    glEnd();

    // S
    p1[0] = ((col*WIDTH/maze.getColumns())-(WIDTH/2)); p1[1] = z; p1[2] = ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2);
    p2[0] = ((col*WIDTH/maze.getColumns())-(WIDTH/2)); p2[1] = 0; p2[2] = ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2);
    p3[0] = (((col+1)*WIDTH/maze.getColumns())-(WIDTH/2));p3[1] = 0; p3[2] = ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2);
    p4[0] = (((col+1)*WIDTH/maze.getColumns())-(WIDTH/2)); p4[1] = z; p4[2] = ((row+1)*HEIGHT/maze.getRows())-(HEIGHT/2);

    normal = getNormalFromSquare(p1, p2, p3);

    glBegin(GL_QUADS);
    glNormal3f(normal[0], normal[1], normal[2]);
    glTexCoord2f(1.0,1.0); glVertex3i(p1[0], p1[1], p1[2]);
    glNormal3f(normal[0], normal[1], normal[2]);
    glTexCoord2f(1.0,0.0); glVertex3i(p2[0], p2[1], p2[2]);
    glNormal3f(normal[0], normal[1], normal[2]);
    glTexCoord2f(0.0,0.0); glVertex3i(p3[0], p3[1], p3[2]);
    glNormal3f(normal[0], normal[1], normal[2]);
    glTexCoord2f(0.0,1.0); glVertex3i(p4[0], p4[1], p4[2]);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

float * getNormalFromSquare(int *p1, int *p2, int *p3) {
    float a[3], b[3], normal[3];
    a[0] = p3[0] - p2[0];
    a[1] = p3[1] - p2[1];
    a[2] = p3[2] - p2[2];

    b[0] = p1[0] - p2[0];
    b[1] = p1[1] - p2[1];
    b[2] = p1[2] - p2[2];

    // a x b;
    normal[0] = (a[1] * b[2] - a[2] * b[1]);
    normal[1] = (a[2] * b[0] - a[0] * b[2]);
    normal[2] = (a[0] * b[1] - a[1] * b[0]);

    float modulus = (float)sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

    if (modulus == 0.0f) {
        modulus = 1.0f;
    }

    normal[0] /= modulus;
    normal[1] /= modulus;
    normal[2] /= modulus;

    float * w = (float *)malloc(sizeof(float) * 3);
    w[0] = normal[0];
    w[1] = normal[1];
    w[2] = normal[2];

    return w;
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
    float angle = Utils::calc_angle(direction, agent->getOrientation());

    if(agentIndex == 0) {

        agent->init_rotate(angle, direction, 200);

        if (maze.move(agentIndex, direction)) {
            agent->init_movement(Utils::col_to_x(pos.getCol(), direction.x, WIDTH, maze.getColumns()),
                                Utils::row_to_y(pos.getRow(), direction.y, HEIGHT, maze.getRows()), 150);
        }
    } else {
        if (maze.move(agentIndex, direction)) {

            agent->init_rotate(angle, direction, 200);
            agent->init_movement(Utils::col_to_x(pos.getCol(), direction.x, WIDTH, maze.getColumns()),
                                Utils::row_to_y(pos.getRow(), direction.y, HEIGHT, maze.getRows()), 150);
        }
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


unsigned char* LoadTexture(char *filename,int dim) {
    unsigned char *buffer, *buffer2;
    int width,height;
    long i,j;
    long k,h;

    ReadJPEG(filename,&buffer,&width,&height);

    buffer2=(unsigned char*)malloc(dim*dim*3);

    for(i=0;i<dim;i++)
        for(j=0;j<dim;j++) {
            k=i*height/dim;
            h=j*width/dim;
            
            buffer2[3*(i*dim+j)]=buffer[3*(k*width +h)];
            buffer2[3*(i*dim+j)+1]=buffer[3*(k*width +h)+1];
            buffer2[3*(i*dim+j)+2]=buffer[3*(k*width +h)+2];
        }
    

    free(buffer);

    return buffer2;
}

void ReadJPEG(char *filename,unsigned char **image,int *width, int *height) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * infile;
    unsigned char **buffer;
    int i,j;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);


    if ((infile = fopen(filename, "rb")) == NULL) {
        printf("Unable to open file %s\n",filename);
        exit(1);
    }

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_calc_output_dimensions(&cinfo);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height  = cinfo.output_height;

    *image=(unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.output_components);

    buffer=(unsigned char **)malloc(1*sizeof(unsigned char **));
    buffer[0]=(unsigned char *)malloc(cinfo.output_width*cinfo.output_components);

    i=0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        for(j=0;j<cinfo.output_width*cinfo.output_components;j++) {
            (*image)[i]=buffer[0][j];
            i++;
        }   

    }

    free(buffer);
    jpeg_finish_decompress(&cinfo);
}

void setTexture (unsigned char* texture) {
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,DIM,DIM,0,GL_RGB,GL_UNSIGNED_BYTE, texture);
}
