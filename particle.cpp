#include "particle.h"
#include "directions.h"

GLUquadric* myReusableQuadric = 0;

Particle::Particle() {
    this->state = QUIET;
    this->rot_state = QUIET;
    this->orientation = Directions::DOWN;
    this->angle = 0;
}

Particle::Particle(int row, int col) {
    this->position = Point(row, col);
    this->state = QUIET;
    this->rot_state = QUIET;
    this->orientation = Directions::DOWN;
    this->angle = 0;
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

void Particle::setOrientation(Directions::Direction direction) {
    this->orientation = direction;
}

Directions::Direction Particle::getOrientation() {
    return this->orientation;
}

void Particle::setAngle(float angle) {
    this->angle = angle;
}


float Particle::getAngle() {
    return this->angle;
}

int Particle::getRotState() {
    return this->rot_state;
}

void Particle::init_movement(int destination_x, int destination_y, int duration) {
    this->vx = (destination_x - this->x)/duration;
    this->vy = (destination_y - this->y)/duration;

    this->state = MOVE;
    this->time_remaining = duration;
}

void Particle::integrate(long t) {
    if(this->rot_state==MOVE && t<this->rot_time_remaining) {
        this->angle += this->v_angle*t;
        this->rot_time_remaining-=t;
    } else if(this->rot_state==MOVE && t>=this->rot_time_remaining) {
        this->angle += this->v_angle*this->rot_time_remaining;
        this->rot_state=QUIET;
        if (this->nextx > -1 && this->nexty > -1) {
            this->init_movement((int)nextx, (int)nexty, 100);
        }

    } else if(this->state==MOVE && t<this->time_remaining) {
        this->x += this->vx*t;
        this->y += this->vy*t;
        this->time_remaining-=t;
    } else if(this->state==MOVE && t>=this->time_remaining) {
        this->x += this->vx*this->time_remaining;
        this->y += this->vy*this->time_remaining;
        this->state=QUIET;
    }
}

void Particle::init_rotate(float angle, Directions::Direction direction, int duration) {
    if(this->rot_state == QUIET && this->state == QUIET && angle != 0) {
        this->v_angle = angle/duration;
        this->rot_state = MOVE;
        this->rot_time_remaining = duration;
        this->orientation = direction;
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

    glColor3f(0, 0, 0);
    gluCylinder(myReusableQuadric, radiusBase, radiusTop, height, slices, stacks);
    
    glColor3f(0.3, 0.3, 0.3);
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
    float radius = square_width/4;
    float diameter = radius;
    int z = (square_height/4)*3;
    int ax = (-square_width)/2;
    int bx = square_width/2;
    int ay = (-square_width)/2;
    int by = square_width/2;
    int cannon_base_height = (z/5)*3;
    int border_size = radius/2;


    glPushMatrix();
    glTranslatef((x-square_height/2)-(width/2) +10, radius, (y-square_width/2)-(height/2)+10);

    glRotatef( this->angle, 0.0, 1.0, 0.0 );

    /* Tank base */

    glBegin(GL_QUADS);
    glVertex3i(ax, z, ay);
    glVertex3i(ax, z, by);
    glVertex3i(bx, z, by);
    glVertex3i(bx, z, ay);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(ax, diameter, ay);
    glVertex3i(bx, diameter, ay);
    glVertex3i(bx, diameter, by);
    glVertex3i(ax, diameter, by);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(ax, z, ay);
    glVertex3i(bx, z, ay);
    glVertex3i(bx, diameter, ay);
    glVertex3i(ax, diameter, ay);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(ax, z, by);
    glVertex3i(ax, z, ay);
    glVertex3i(ax, diameter, ay);
    glVertex3i(ax, diameter, by);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(bx, z, by);
    glVertex3i(bx, diameter, by);
    glVertex3i(bx, diameter, ay);
    glVertex3i(bx, z, ay);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(ax, z, by);
    glVertex3i(ax, diameter, by);
    glVertex3i(bx, diameter, by);
    glVertex3i(bx, z, by);
    glEnd();


    /* Cannon base */
    GLfloat material[] = {0.3, 0.3, 0.3,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);

    glBegin(GL_QUADS);
    glVertex3i(ax + border_size, z+cannon_base_height, ay + border_size);
    glVertex3i(ax + border_size, z+cannon_base_height, by - border_size);
    glVertex3i(bx - border_size, z+cannon_base_height, by - border_size);
    glVertex3i(bx - border_size, z+cannon_base_height, ay + border_size);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(ax + border_size, z, ay + border_size);
    glVertex3i(bx - border_size, z, ay + border_size);
    glVertex3i(bx - border_size, z, by - border_size);
    glVertex3i(ax + border_size, z, by - border_size);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(ax + border_size, z+cannon_base_height, ay + border_size);
    glVertex3i(bx - border_size, z+cannon_base_height, ay + border_size);
    glVertex3i(bx - border_size, z, ay + border_size);
    glVertex3i(ax + border_size, z, ay + border_size);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(ax + border_size, z+cannon_base_height, by - border_size);
    glVertex3i(ax + border_size, z+cannon_base_height, ay + border_size);
    glVertex3i(ax + border_size, z, ay + border_size);
    glVertex3i(ax + border_size, z, by - border_size);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(bx - border_size, z+cannon_base_height, by - border_size);
    glVertex3i(bx - border_size, z, by - border_size);
    glVertex3i(bx - border_size, z, ay + border_size);
    glVertex3i(bx - border_size, z+cannon_base_height, ay + border_size);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3i(ax + border_size, z+cannon_base_height, by - border_size);
    glVertex3i(ax + border_size, z, by - border_size);
    glVertex3i(bx - border_size, z, by - border_size);
    glVertex3i(bx - border_size, z+cannon_base_height, by - border_size);
    glEnd();

    glDisable( GL_CULL_FACE );
    glPushMatrix();
    glTranslatef( 1.5, 0.0, 0.0 );
    glRotatef( -180.0, 1.0, 0.0, 1.0 );
    
    /* Wheels */
    material[0] = 0.0; material[1] = 0.0; material[2] = 0.0; material[3] = 1.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
    // Parameters: height, radius, slices, stacks
    glTranslatef(-square_width/2, 0, -square_width/2);
    drawGluCylinder(square_height, radius, 90, 1 );

    glTranslatef(square_width/2, 0, -square_width);
    drawGluCylinder(square_height, radius, 90, 1 );

    glTranslatef(square_height/2, 0, -square_width);
    drawGluCylinder(square_height, radius, 90, 1 );


    /* Cannon */
    glRotatef( -180.0, 1.0, 0.0, 1.0 );
    glTranslatef((-square_height)/2, z+(cannon_base_height/2), (-square_width+10)/2);
    drawGluCylinder(square_height, radius/1.5, 90, 1 );

    glPopMatrix();
    
}