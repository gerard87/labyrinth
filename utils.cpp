#include<cmath>
#include "utils.h"
#include "directions.h"

int Utils::manhattanDistance(Point a, Point b) {
    return std::abs(a.getCol() - b.getCol()) + std::abs(a.getRow() - b.getRow());
}

int Utils::col_to_x(int col, int offset, int width, int maze_columns) {
    return (int)(((col+offset)*(width)/maze_columns) + ((col+1+offset)*(width)/maze_columns))/2;
}

int Utils::row_to_y(int row, int offset, int height, int maze_rows) {
    return (int)((row+offset)*(height)/maze_rows) + ((row+1+offset)*(height)/maze_rows)/2 - (row+offset)*(height)/maze_rows/2;
}

float Utils::calc_angle(Directions::Direction direction, Directions::Direction orientation) {
    if(direction == Directions::LEFT) {
        if(orientation == Directions::UP) return 90.0f;
        else if (orientation == Directions::DOWN) return -90.0f;
        else if(orientation == Directions::RIGHT) return 180.0f;
        else return 0;
    } else if(direction == Directions::RIGHT) {
        if(orientation == Directions::UP) return -90.0f;
        else if (orientation == Directions::DOWN) return 90.0f;
        else if(orientation == Directions::LEFT) return 180.0f;
        else return 0;
    } else if(direction == Directions::UP) {
        if(orientation == Directions::DOWN) return 180.0f;
        else if(orientation == Directions::LEFT) return -90.0f;
        else if(orientation == Directions::RIGHT) return 90.0f;
        else return 0;
    } else if(direction == Directions::DOWN) {
        if(orientation == Directions::UP) return 180.0f;
        else if(orientation == Directions::LEFT) return 90.0f;
        else if(orientation == Directions::RIGHT) return -90.0f;
        else return 0;
    }
}