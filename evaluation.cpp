#include "evaluation.h"
#include "utils.h"
#include "point.h"

float Evaluation::evaluationFunction(Maze maze, int agent) {
    float total_score = 0;
    Point pos = maze.getCurrentPosition(1);

    Point base = agent == 0 ? maze.getEnemyBase() : maze.getPlayerBase();

    int manDistance = Utils::manhattanDistance(base, pos);

    if (manDistance == 0) total_score += 100;
    else total_score += 1.0/(manDistance + manDistance);
    
    return total_score;
}