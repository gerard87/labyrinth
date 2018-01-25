#include "alphabeta.h"
#include "utils.h"
#include "agentutils.h"
#include <algorithm>
#include <climits>

Alphabeta::Alphabeta() {
    this->depth = 2;
}

Alphabeta::Alphabeta(int depth) {
    this->depth = depth;
}

float Alphabeta::maxValue(Maze maze, int agent, int depth, float alpha, float beta) {

    if(Agentutils::terminalTest(maze, depth)) return Agentutils::utility(maze, agent);

    float v = -INT_MAX;

    for(auto const &d : maze.getAvailableMoves(agent)){
        v = std::max(v, this->minValue(Agentutils::result(maze, agent, d), 0, depth, alpha, beta));
        if(v >= beta) return v;
        alpha = std::max(alpha, v);
    }

    return v;

}

float Alphabeta::minValue(Maze maze, int agent, int depth, float alpha, float beta) {

    if(Agentutils::terminalTest(maze, depth)) return Agentutils::utility(maze, agent);

    float v = INT_MAX;

    for(auto const &d : maze.getAvailableMoves(agent)){
        v = std::min(v, this->maxValue(Agentutils::result(maze, agent, d), 1, depth-1, alpha, beta));
        if(v <= alpha) return v;
        beta = std::min(beta, v);
    }

    return v;
}


Directions::Direction Alphabeta::getAction(Maze maze) {
    float v = -INT_MAX;
    float u;
    float alpha = -INT_MAX;
    float beta = INT_MAX;
    std::vector<Directions::Direction> actions;
    for(auto const &d : maze.getAvailableMoves(1)){
        u = this->minValue(Agentutils::result(maze, 1, d), 0, this->depth, alpha, beta);

        if(u >= beta) return d;
        alpha = std::max(alpha, u);

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
