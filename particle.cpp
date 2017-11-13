#include "particle.h"

Particle::Particle() {
    this->state = QUIET;
}

Particle::Particle(int row, int col) {
    this->position = Point(row, col);
    this->state = QUIET;
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

Point::Point Particle::getPosition() {
    return this->position;
}

void Particle::setPoint(Point p) {
    this->position = p;
}