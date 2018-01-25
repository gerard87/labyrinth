#include "expectimax.h"
#include "utils.h"
#include "agentutils.h"
#include <algorithm>
#include <climits>

Expectimax::Expectimax() {
    this->depth = 2;
}

Expectimax::Expectimax(int depth) {
    this->depth = depth;
}

float Expectimax::maxValue(Maze maze, int agent, int depth) {

    if(Agentutils::terminalTest(maze, depth)) return Agentutils::utility(maze, agent);

    float v = -INT_MAX;

    for(auto const &d : maze.getAvailableMoves(agent)){
        v = std::max(v, this->expectedValue(Agentutils::result(maze, agent, d), 0, depth));
    }

    return v;

}

float Expectimax::expectedValue(Maze maze, int agent, int depth) {

    if(Agentutils::terminalTest(maze, depth)) return Agentutils::utility(maze, agent);

    float total_score = 0;
    std::vector<Directions::Direction> actions = maze.getAvailableMoves(agent);

    for(auto const &d : actions){
        total_score += this->maxValue(Agentutils::result(maze, agent, d), 1, depth-1);
    }

    return total_score/actions.size();
}


Directions::Direction Expectimax::getAction(Maze maze) {
    float v = -INT_MAX;
    float u;
    std::vector<Directions::Direction> actions;
    for(auto const &d : maze.getAvailableMoves(1)){
        u = this->expectedValue(Agentutils::result(maze, 1, d), 0, this->depth);

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
