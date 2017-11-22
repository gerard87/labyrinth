#include "particle.h"
#include "directions.h"

GLUquadric* myReusableQuadric = 0;

Particle::Particle() {
    this->state = QUIET;
    this->orientation = Directions::DOWN;
    this->angle = 0;
}

Particle::Particle(int row, int col) {
    this->position = Point(row, col);
    this->state = QUIET;
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

void Particle::init_rotate(float angle, int duration) {
    this->v_angle = angle/duration;
    this->rot_state = MOVE;
    this->rot_time_remaining = duration;
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
    float radius = 5.0;
    float diameter = radius * 2;
    int z = ((x+square_height/2) - (x-square_height/2));
    int cannon_base_height = 10;
    int ax = (x-square_height/2)-(width/2);
    int bx = (x+square_height/2)-(width/2);
    int ay = (y-square_width/2)-(height/2);
    int by = (y+square_width/2)-(height/2);
    int border_size = 3;

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

    glColor3f(0.3, 0.3, 0.3);

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



    // ----------------------

    glPushMatrix();
    glTranslatef((x-square_height/2)-(width/2), radius, (y-square_width/2)-(height/2));

    glRotatef( this->angle, 0.0, 1.0, 0.0 );

    float x_offset=0, z_offset=0;
    if(this->orientation == Directions::UP) x_offset = z_offset = -square_width;
    else if (this->orientation == Directions::LEFT) z_offset = -square_width;
    else if (this->orientation == Directions::RIGHT) x_offset = -square_width;
    glTranslatef(x_offset, 0, z_offset);


    glDisable( GL_CULL_FACE );
    glPushMatrix();
    glTranslatef( 1.5, 0.0, 0.0 );
    glRotatef( -180.0, 1.0, 0.0, 1.0 );
    

    /* Wheels */
    
    // Parameters: height, radius, slices, stacks
    drawGluCylinder(square_height, radius, 90, 1 );
    glTranslatef(square_height, 0, -square_width);
    drawGluCylinder(square_height, radius, 90, 1 );


    /* Cannon */

    glRotatef( -180.0, 1.0, 0.0, 1.0 );
    glTranslatef((-square_height)/2, z, (-square_width+10)/2);
    drawGluCylinder(square_height, 4, 90, 1 );

    glPopMatrix();
    
}