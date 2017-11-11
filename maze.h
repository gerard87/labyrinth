#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#define MOVE 1
#define QUIET 2

using namespace std;

class Maze {

    public:

        enum Directions {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        struct Point {
            bool operator==(const Point& p) const {
                return row == p.row && col == p.col; // or another approach as above
            }
            int row;
            int col;

            float x, y;
            float vx, vy;
            int state;
            long time_remaining;


            Point() {
                this->state = QUIET;
            }

            Point(int row, int col) {
                this->row = row;
                this->col = col;
                this->state = QUIET;
            }
        };

        Maze();
        Maze(int rows, int columns);
        Maze(int squared);
        
        int getRows();
        int getColumns();
        int** getMaze();

        void printMaze();
        bool isWall(Point p);
        bool isWall(int row, int col);

        bool isSquared();

        int getValue(int row, int column);
    

        bool move(int agentIndex, Directions direction);
        Point getCurrentPosition(int agentIndex);

        void set_position(int agentIndex, int x, int y);
        void init_movement(int agentIndex, int destination_x, int destination_y, int duration);
        void integrate(int agentIndex, long t);

    private:

        int rows;
        int columns;
        int** maze;
        int makeHoles;
        int makeHolesPercent = 18;

        Point playerPosition;
        Point enemyPosition;

        Point playerBase;
        Point enemyBase;

        std::vector<Maze::Point> walls;

        void generateMaze();
        void allocMaze();
        void randPrim();
        void removeIndex(Point* list, int length, int index);
        void removePoint(Point* list, int length, Point aPoint);
        int contains(Point* list,int length, Point aPoint);
        int containsAlt(Point* list, int length, int row, int col);
        int addNearbyWalls(Point* list, int length, Point aPoint);
        void createHoles();
        int random_int(int min, int max);
        void generateStartingPoints();
        void encloseMaze();
        void setHolesQuantity();
        void removePoint(std::vector<Point> & points, int row, int col);


        // Game Helpers
        bool agentInPosition(int agentIndex, Point pos);
        bool checkValidMove(int agentIndex, Point to);



}; 