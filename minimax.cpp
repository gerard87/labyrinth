#include "minimax.h"
#include "utils.h"
#include "agentutils.h"
#include <algorithm>
#include <climits>

Minimax::Minimax() {
    this->depth = 2;
}

Minimax::Minimax(int depth) {
    this->depth = depth;
}

float Minimax::maxValue(Maze maze, int agent, int depth) {

    if(Agentutils::terminalTest(maze, depth)) return Agentutils::utility(maze, agent);

    float v = -INT_MAX;

    for(auto const &d : maze.getAvailableMoves(agent)){
        v = std::max(v, this->minValue(Agentutils::result(maze, agent, d), 0, depth));
    }

    return v;

}

float Minimax::minValue(Maze maze, int agent, int depth) {

    if(Agentutils::terminalTest(maze, depth)) return Agentutils::utility(maze, agent);

    float v = INT_MAX;

    for(auto const &d : maze.getAvailableMoves(agent)){

        /*if(agent == maze.getAgentsNum()-1) {
            v = std::min(v, this->maxValue(this->result(maze, agent, d), 0, depth-1));
        } else {
            v = std::min(v, this->minValue(this->result(maze, agent, d), agent+1, depth));
        }*/
        v = std::min(v, this->maxValue(Agentutils::result(maze, agent, d), 1, depth-1));
    }

    return v;
}


Directions::Direction Minimax::getAction(Maze maze) {
    float v = -INT_MAX;
    float u;
    std::vector<Directions::Direction> actions;
    for(auto const &d : maze.getAvailableMoves(1)){
        u = this->minValue(Agentutils::result(maze, 1, d), 0, this->depth);
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
