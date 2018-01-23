#include "minimax.h"
#include "utils.h"
#include <algorithm>
#include <climits>

Minimax::Minimax() {
    this->depth = 2;
}

Minimax::Minimax(int depth) {
    this->depth = depth;
}

float Minimax::evaluationFunction(Maze maze, int agent) {
    float total_score = 0;
    Point pos = maze.getCurrentPosition(1);

    Point base = agent == 0 ? maze.getEnemyBase() : maze.getPlayerBase();

    int manDistance = Utils::manhattanDistance(base, pos);

    if (manDistance == 0) total_score += 100;
    else total_score += 1.0/(manDistance + manDistance);
    
    return total_score;
}

Maze Minimax::result(Maze maze, int agent, Directions::Direction direction) {
    if (!this->isWin(maze) || !this->isLose(maze)) maze.move(agent, direction);
    return maze;
}

float Minimax::utility(Maze maze, int agent) {
    return this->evaluationFunction(maze, agent);
}

bool Minimax::terminalTest(Maze maze, int depth) {
    return depth == 0 || this->isWin(maze) || this->isLose(maze);
}


float Minimax::maxValue(Maze maze, int agent, int depth) {

    if(this->terminalTest(maze, depth)) return this->utility(maze, agent);

    float v = -INT_MAX;

    for(auto const &d : maze.getAvailableMoves(agent)){
        v = std::max(v, this->minValue(this->result(maze, agent, d), 0, depth));
    }

    return v;

}

float Minimax::minValue(Maze maze, int agent, int depth) {

    if(this->terminalTest(maze, depth)) return this->utility(maze, agent);

    float v = INT_MAX;

    for(auto const &d : maze.getAvailableMoves(agent)){

        /*if(agent == maze.getAgentsNum()-1) {
            v = std::min(v, this->maxValue(this->result(maze, agent, d), 0, depth-1));
        } else {
            v = std::min(v, this->minValue(this->result(maze, agent, d), agent+1, depth));
        }*/
        v = std::min(v, this->maxValue(this->result(maze, agent, d), 1, depth-1));
    }

    return v;
}


Directions::Direction Minimax::getAction(Maze maze) {
    float v = -INT_MAX;
    float u;
    std::vector<Directions::Direction> actions;
    for(auto const &d : maze.getAvailableMoves(1)){
        u = this->minValue(this->result(maze, 1, d), 0, this->depth);
        if(u == v) {
            actions.push_back(d);
        } else if(u >= v) {
            v = u;
            actions.clear();
            actions.push_back(d);
        }
  
    }

    return actions[rand() % actions.size()];

}


bool Minimax::isWin(Maze maze) {
    return maze.getPlayerBase().getCol() == maze.getCurrentPosition(1).getCol() &&  
        maze.getPlayerBase().getRow() == maze.getCurrentPosition(1).getRow();
}

bool Minimax::isLose(Maze maze) {
    return maze.getEnemyBase().getCol() == maze.getCurrentPosition(0).getCol() &&  
        maze.getEnemyBase().getRow() == maze.getCurrentPosition(0).getRow();
}

