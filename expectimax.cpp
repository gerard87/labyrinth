#include "expectimax.h"
#include "utils.h"
#include "evaluation.h"
#include <algorithm>
#include <climits>

Expectimax::Expectimax() {
    this->depth = 2;
}

Expectimax::Expectimax(int depth) {
    this->depth = depth;
}

Maze Expectimax::result(Maze maze, int agent, Directions::Direction direction) {
    if (!this->isWin(maze) || !this->isLose(maze)) maze.move(agent, direction);
    return maze;
}

float Expectimax::utility(Maze maze, int agent) {
    return Evaluation::evaluationFunction(maze, agent);
}

bool Expectimax::terminalTest(Maze maze, int depth) {
    return depth == 0 || this->isWin(maze) || this->isLose(maze);
}


float Expectimax::maxValue(Maze maze, int agent, int depth) {

    if(this->terminalTest(maze, depth)) return this->utility(maze, agent);

    float v = -INT_MAX;

    for(auto const &d : maze.getAvailableMoves(agent)){
        v = std::max(v, this->expectedValue(this->result(maze, agent, d), 0, depth));
    }

    return v;

}

float Expectimax::expectedValue(Maze maze, int agent, int depth) {

    if(this->terminalTest(maze, depth)) return this->utility(maze, agent);

    float total_score = 0;
    std::vector<Directions::Direction> actions = maze.getAvailableMoves(agent);

    for(auto const &d : actions){
        total_score += this->maxValue(this->result(maze, agent, d), 1, depth-1);
    }

    return total_score/actions.size();
}


Directions::Direction Expectimax::getAction(Maze maze) {
    float v = -INT_MAX;
    float u;
    std::vector<Directions::Direction> actions;
    for(auto const &d : maze.getAvailableMoves(1)){
        u = this->expectedValue(this->result(maze, 1, d), 0, this->depth);

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


bool Expectimax::isWin(Maze maze) {
    return maze.getPlayerBase().getCol() == maze.getCurrentPosition(1).getCol() &&  
        maze.getPlayerBase().getRow() == maze.getCurrentPosition(1).getRow();
}

bool Expectimax::isLose(Maze maze) {
    return maze.getEnemyBase().getCol() == maze.getCurrentPosition(0).getCol() &&  
        maze.getEnemyBase().getRow() == maze.getCurrentPosition(0).getRow();
}

