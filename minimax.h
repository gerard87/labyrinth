#ifndef MINIMAX_H
#define MINIMAX_H

#include "point.h"
#include "directions.h"
#include "maze.h"

class Minimax {
    private:

        int depth;

        float maxValue(Maze maze, int agent, int depth);
        float minValue(Maze maze, int agent, int depth);

    public:

        Minimax();
        Minimax(int depth);

        Directions::Direction getAction(Maze maze);

};
#endif