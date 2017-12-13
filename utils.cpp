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

float * Utils::getNormalFromSquare(int *p1, int *p2, int *p3) {
    float a[3], b[3], normal[3];
    a[0] = p3[0] - p2[0];
    a[1] = p3[1] - p2[1];
    a[2] = p3[2] - p2[2];

    b[0] = p1[0] - p2[0];
    b[1] = p1[1] - p2[1];
    b[2] = p1[2] - p2[2];

    // a x b;
    normal[0] = (a[1] * b[2] - a[2] * b[1]);
    normal[1] = (a[2] * b[0] - a[0] * b[2]);
    normal[2] = (a[0] * b[1] - a[1] * b[0]);

    float modulus = (float)sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

    if (modulus == 0.0f) {
        modulus = 1.0f;
    }

    normal[0] /= modulus;
    normal[1] /= modulus;
    normal[2] /= modulus;

    float * w = (float *)malloc(sizeof(float) * 3);
    w[0] = normal[0];
    w[1] = normal[1];
    w[2] = normal[2];

    return w;
}