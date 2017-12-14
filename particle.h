#ifndef PARTICLE_H
#define PARTICLE_H
#include <GL/glut.h>

#include "point.h"
#include "directions.h"
#include "bullet.h"

class Particle {

    private:
        // bool operator==(const Particle& p) const {
        //     return row == p.row && col == p.col;
        // }
        // int row;
        // int col;

        Point position;

        float x, y;
        int nextx = -1, nexty = -1;
        float vx, vy;
        int state;
        long time_remaining;

        Directions::Direction orientation;
        float angle;
        float v_angle;
        int rot_state;
        long rot_time_remaining;

        bool reset;

        GLUquadric* myReusableQuadric = 0;
        Bullet bullet;

        void drawGluSlantCylinder(double height, double radiusBase, double radiusTop, int slices, int stacks);
        void drawGluCylinder(double height, double radius, int slices, int stacks);
        
    public:
        static const int QUIET = 1;
        static const int MOVE = 2;

        Particle();
        Particle(int row, int col);
        void set_position(int x,int y);
        void init_movement(int destination_x,int destination_y,int duration);

        void integrate(long t);
        void draw();
        int getState();
        Point getPosition();
        void setPoint(Point p);
        float getX();
        float getY();
        void draw(float square_width, float square_height, int width, int height);
        void setOrientation(Directions::Direction direction);
        Directions::Direction getOrientation();
        void setAngle(float angle);
        float getAngle();
        int getRotState();
        void init_rotate(float angle, Directions::Direction direction, int duration);
        Bullet* getBullet();
        void shoot(int cols, int rows, int width, int height, int direction_x, int direction_y, bool kill, Particle* player, Point base);
        bool getReset();
        void setReset(bool b);
        void resetPlayer(Point p, int cols, int rows, int width, int height);

};
#endif