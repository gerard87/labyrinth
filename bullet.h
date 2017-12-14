#ifndef BULLET_H
#define BULLET_H
#include <GL/glut.h>

#include "point.h"
#include "directions.h"
class Particle;

class Bullet {

    private:

        float x, y;
        float vx, vy;
        int state;
        long time_remaining;
        float angle;
        int rows, cols, width, height;
        Point base;

        Particle* opponent;

        GLUquadric* myReusableQuadric = 0;

        bool opponentKilled;

        void drawGluSlantCylinder(double height, double radiusBase, double radiusTop, int slices, int stacks);
        void drawGluCylinder(double height, double radius, int slices, int stacks);


    public:

        static const int QUIET = 1;
        static const int MOVE = 2;

        Bullet();
        Bullet(int row, int col);
        void set_position(int x, int y);
        int getState();
        void draw(float square_width, float square_height, int width, int height, int angle);
        void init_movement(int destination_x,int destination_y,int duration);
        void integrate(long t);
        void setOpponent(Particle* player, Point base, int cols, int rows, int width, int height);
        void setOpponentKilled(bool b);

};
#endif