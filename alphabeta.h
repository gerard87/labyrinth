#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "point.h"
#include "directions.h"
#include "maze.h"

class Alphabeta {
    private:

        int depth;

        Maze result(Maze maze, int agent, Directions::Direction direction);
        float utility(Maze maze, int agent);
        bool terminalTest(Maze maze, int depth);

        float maxValue(Maze maze, int agent, int depth, float alpha, float beta);
        float minValue(Maze maze, int agent, int depth, float alpha, float beta);

        bool isWin(Maze maze);
        bool isLose(Maze maze);

    public:

        Alphabeta();
        Alphabeta(int depth);

        Directions::Direction getAction(Maze maze);

};
#endif