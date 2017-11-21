#include "particle.h"

GLUquadric* myReusableQuadric = 0;
float Azimuth = 20.0;	
float RotateAngle = 0.0f;
float AngleStepSize = 3.0f;		// Step three degrees at a time
const float AngleStepMax = 10.0f;
const float AngleStepMin = 0.1f;

Particle::Particle() {
    this->state = QUIET;
}

Particle::Particle(int row, int col) {
    this->position = Point(row, col);
    this->state = QUIET;
}

float Particle::getX() {
    return this->x;
}

float Particle::getY() {
    return this->y;
}

int Particle::getState() {
    return this->state;
}

void Particle::init_movement(int destination_x, int destination_y, int duration) {
    this->vx = (destination_x - this->x)/duration;
    this->vy = (destination_y - this->y)/duration;

    this->state = MOVE;
    this->time_remaining = duration;
}

void Particle::integrate(long t) {
    if(this->state==MOVE && t<this->time_remaining) {
        this->x += this->vx*t;
        this->y += this->vy*t;
        this->time_remaining-=t;
    } else if(this->state==MOVE && t>=this->time_remaining) {
        this->x += this->vx*this->time_remaining;
        this->y += this->vy*this->time_remaining;
        this->state=QUIET;
    }
}

void Particle::set_position(int x, int y) {
    this->x = x;
    this->y = y;
}

Point Particle::getPosition() {
    return this->position;
}

void Particle::setPoint(Point p) {
    this->position = p;
}

void drawGluSlantCylinder( double height, double radiusBase, double radiusTop, int slices, int stacks )
{
	if (!myReusableQuadric) {
		myReusableQuadric = gluNewQuadric();  
		gluQuadricNormals(myReusableQuadric, GL_TRUE);
	}

    //glPolygonMode(GL_FRONT,GL_LINE);
    glPolygonMode(GL_BACK,GL_LINE);
    gluCylinder(myReusableQuadric, radiusBase, radiusTop, height, slices, stacks);
    
    gluDisk(myReusableQuadric, 0.0, radiusBase, slices, stacks);
    glTranslatef(0, 0, height);
    gluDisk(myReusableQuadric, 0.0, radiusBase, slices, stacks);
}


void drawGluCylinder( double height, double radius, int slices, int stacks ) {
	drawGluSlantCylinder( height, radius, radius, slices, stacks );
}

void Particle::draw(float square_width, float square_height, int width, int height) {
    float x = getX();
    float y = getY();
    int z = (x+square_height/2) - (x-square_height/2); 

    /*glBegin(GL_QUADS);
    glVertex3i((x-square_height/2)-(width/2), z, (y-square_width/2)-(height/2));
    glVertex3i((x-square_height/2)-(width/2), z, (y+square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), z, (y+square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), z, (y-square_width/2)-(height/2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i((x-square_height/2)-(width/2), 0, (y-square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), 0, (y-square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), 0, (y+square_width/2)-(height/2));
    glVertex3i((x-square_height/2)-(width/2), 0, (y+square_width/2)-(height/2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i((x-square_height/2)-(width/2), z, (y-square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), z, (y-square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), 0, (y-square_width/2)-(height/2));
    glVertex3i((x-square_height/2)-(width/2), 0, (y-square_width/2)-(height/2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i((x-square_height/2)-(width/2), z, (y+square_width/2)-(height/2));
    glVertex3i((x-square_height/2)-(width/2), z, (y-square_width/2)-(height/2));
    glVertex3i((x-square_height/2)-(width/2), 0, (y-square_width/2)-(height/2));
    glVertex3i((x-square_height/2)-(width/2), 0, (y+square_width/2)-(height/2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i((x+square_height/2)-(width/2), z, (y+square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), 0, (y+square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), 0, (y-square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), z, (y-square_width/2)-(height/2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i((x-square_height/2)-(width/2), z, (y+square_width/2)-(height/2));
    glVertex3i((x-square_height/2)-(width/2), 0, (y+square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), 0, (y+square_width/2)-(height/2));
    glVertex3i((x+square_height/2)-(width/2), z, (y+square_width/2)-(height/2));
    glEnd();*/

    glPushMatrix();
    glTranslatef( (x-square_height/2)-(width/2), 5.0, (y-square_width/2)-(height/2) );
    //glRotatef( RotateAngle, 0.0, 1.0, 0.0 );		// Rotate around y-axis
    //glRotatef( Azimuth, 1.0, 0.0, 0.0 );			// Set Azimuth angle

    glDisable( GL_CULL_FACE );
    glPushMatrix();
    glTranslatef( 1.5, 0.0, 0.0 );
    glRotatef( -180.0, 1.0, 0.0, 1.0 );
    // Parameters: height, radius, slices, stacks
    drawGluCylinder(square_height, 5.0, 90, 1 );
    glTranslatef(square_height, 0, -square_width);
    drawGluCylinder(square_height, 5.0, 90, 1 );

    glPopMatrix();
    
    

}