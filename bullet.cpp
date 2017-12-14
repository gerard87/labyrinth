#include "bullet.h"
#include "point.h"
#include "particle.h"

Bullet::Bullet() {
}

Bullet::Bullet(int row, int col) {
    this->state = QUIET;
    this->angle = 0;
    this->opponentKilled = false;
}

void Bullet::set_position(int x, int y) {
    this->x = x;
    this->y = y;
}

int Bullet::getState() {
    return this->state;
}

void Bullet::setOpponentKilled(bool b) {
    this->opponentKilled = b;
}

void Bullet::setOpponent(Particle* player, Point base, int cols, int rows, int width, int height) {
    this->opponent = player;
    this->base = base;
    this->cols = cols;
    this->rows = rows;
    this->width = width;
    this->height = height;
}

void Bullet::init_movement(int destination_x, int destination_y, int duration) {
    this->vx = (destination_x - this->x)/duration;
    this->vy = (destination_y - this->y)/duration;

    this->state = MOVE;
    this->time_remaining = duration;
}

void Bullet::integrate(long t) {
    if(this->state==MOVE && t<this->time_remaining) {
        this->x += this->vx*t;
        this->y += this->vy*t;
        this->time_remaining-=t;
    } else if(this->state==MOVE && t>=this->time_remaining) {
        this->x += this->vx*this->time_remaining;
        this->y += this->vy*this->time_remaining;
        this->state=QUIET;
        if(this->opponentKilled) {
            this->opponent->resetPlayer(this->base, this->cols, this->rows, this->width, this->height);
            this->opponentKilled = false;
        }
        
    }
}


void Bullet::drawGluSlantCylinder(double height, double radiusBase, double radiusTop, int slices, int stacks) {
	if (!myReusableQuadric) {
		myReusableQuadric = gluNewQuadric();  
		gluQuadricNormals(myReusableQuadric, GL_TRUE);
	}

    GLfloat material[] = {1.0, 0.8, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
    glDisable(GL_LIGHTING);
    glColor3d(1.0, 0.8, 0.0);
    gluCylinder(myReusableQuadric, radiusBase, radiusTop, height, slices, stacks);

    gluDisk(myReusableQuadric, 0.0, radiusBase, slices, stacks);
    glTranslatef(0, 0, height);
    gluDisk(myReusableQuadric, 0.0, radiusBase, slices, stacks);
    glEnable(GL_LIGHTING);
}


void Bullet::drawGluCylinder(double height, double radius, int slices, int stacks) {
	drawGluSlantCylinder( height, radius, radius, slices, stacks );
}

void Bullet::draw(float square_width, float square_height, int width, int height, int angle) {

    float radius = square_width/4;
    int z = (square_height/4)*3;
    int cannon_base_height = (z/5)*3;


    glPushMatrix();
    glTranslatef((this->x-square_height/2)-(width/2) +10, radius, (this->y-square_width/2)-(height/2)+10);

    glRotatef( angle, 0.0, 1.0, 0.0 );

    glTranslatef(0, z+(cannon_base_height/2), 0);
    drawGluCylinder(square_height/2, radius/2.5, 90, 1 );

    glPopMatrix();
}

