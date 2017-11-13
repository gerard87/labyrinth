#ifndef DIRECTIONS_H
#define DIRECTIONS_H

class Directions {
    
    public:
        struct Direction {
            bool operator==(const Direction& d) const {
                return x == d.x && y == d.y;
            }
            Direction() {}
            Direction(int xx, int yy) :x(xx), y(yy) {}

            int x; int y;  
        };

        static Direction UP;
        static Direction DOWN;
        static Direction LEFT;
        static Direction RIGHT;
        static Direction STOP;
}; 
#endif