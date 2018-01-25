#ifndef EXPECTIMAX_H
#define EXPECTIMAX_H

#include "point.h"
#include "directions.h"
#include "maze.h"

class Expectimax {
    private:

        int depth;

        float maxValue(Maze maze, int agent, int depth);
        float expectedValue(Maze maze, int agent, int depth);

    public:

        Expectimax();
        Expectimax(int depth);

        Directions::Direction getAction(Maze maze);

};
#endif