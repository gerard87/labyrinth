#ifndef UTILS_H
#define UTIL_H
#include "point.h"
#include "directions.h"

class Utils {
    public:
        static int manhattanDistance(Point a, Point b);
        static int col_to_x(int col, int offset, int width, int maze_columns);
        static int row_to_y(int row, int offset, int height, int maze_rows);
        static float calc_angle(Directions::Direction direction, Directions::Direction orientation);
};
#endif