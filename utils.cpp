#include<cmath>
#include "utils.h"

int Utils::manhattanDistance(Point a, Point b) {
    return std::abs(a.getCol() - b.getCol()) + std::abs(a.getRow() - b.getRow());
}