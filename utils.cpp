#include <math.h>
#include "utils.h"

int Utils::manhattanDistance(Point a, Point b) {
    return abs(a.getCol() - b.getCol()) + abs(a.getRow() - b.getRow());
}