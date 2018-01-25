#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "point.h"
#include "directions.h"
#include "maze.h"

class Alphabeta {
    private:

        int depth;

        float maxValue(Maze maze, int agent, int depth, float alpha, float beta);
        float minValue(Maze maze, int agent, int depth, float alpha, float beta);

    public:

        Alphabeta();
        Alphabeta(int depth);

        Directions::Direction getAction(Maze maze);

};
#endif