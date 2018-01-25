#include "agentutils.h"
#include "utils.h"
#include "point.h"

float Agentutils::evaluationFunction(Maze maze, int agent) {
    float total_score = 0;
    Point pos = maze.getCurrentPosition(1);

    Point base = agent == 0 ? maze.getEnemyBase() : maze.getPlayerBase();

    int manDistance = Utils::manhattanDistance(base, pos);

    if (manDistance == 0) total_score += 100;
    else total_score += 1.0/(manDistance + manDistance);
    
    return total_score;
}

Maze Agentutils::result(Maze maze, int agent, Directions::Direction direction) {
    if (!isWin(maze) || !isLose(maze)) maze.move(agent, direction);
    return maze;
}

float Agentutils::utility(Maze maze, int agent) {
    return evaluationFunction(maze, agent);
}

bool Agentutils::terminalTest(Maze maze, int depth) {
    return depth == 0 || isWin(maze) || isLose(maze);
}

bool Agentutils::isWin(Maze maze) {
    return maze.getPlayerBase().getCol() == maze.getCurrentPosition(1).getCol() &&  
        maze.getPlayerBase().getRow() == maze.getCurrentPosition(1).getRow();
}

bool Agentutils::isLose(Maze maze) {
    return maze.getEnemyBase().getCol() == maze.getCurrentPosition(0).getCol() &&  
        maze.getEnemyBase().getRow() == maze.getCurrentPosition(0).getRow();
}
