#include "point.h"

Point::Point() {}

Point::Point(int row, int col) {
    this->row = row;
    this->col = col;
}

int Point::getRow() {
    return this->row;
}

void Point::setRow(int row) {
    this->row = row;
}

int Point::getCol() {
    return this->col;
}

void Point::setCol(int col) {
    this->col = col;
}