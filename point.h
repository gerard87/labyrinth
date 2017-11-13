#ifndef POINT_H
#define POINT_H

class Point {
    private:
        int row;
        int col;

    public:

        bool operator==(const Point& p) const {
            return row == p.row && col == p.col;
        }

        Point();
        Point(int row, int col);

        // Getters
        int getRow();
        int getCol();

        // Setters
        void setRow(int row);
        void setCol(int col);
    };
    #endif