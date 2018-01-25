#ifndef AGENTUTILS_H
#define AGENTUTILS_H

#include "maze.h"

class Agentutils {
    private:
        static float evaluationFunction(Maze maze, int agent);
        static bool isWin(Maze maze);
        static bool isLose(Maze maze);

    public:
        static Maze result(Maze maze, int agent, Directions::Direction direction);
        static float utility(Maze maze, int agent);
        static bool terminalTest(Maze maze, int depth);
};
#endif