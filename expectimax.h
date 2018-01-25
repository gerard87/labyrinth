#ifndef EXPECTIMAX_H
#define EXPECTIMAX_H

#include "point.h"
#include "directions.h"
#include "maze.h"

class Expectimax {
    private:

        int depth;

        Maze result(Maze maze, int agent, Directions::Direction direction);
        float utility(Maze maze, int agent);
        bool terminalTest(Maze maze, int depth);

        float maxValue(Maze maze, int agent, int depth);
        float expectedValue(Maze maze, int agent, int depth);

        bool isWin(Maze maze);
        bool isLose(Maze maze);

    public:

        Expectimax();
        Expectimax(int depth);

        Directions::Direction getAction(Maze maze);

};
#endif