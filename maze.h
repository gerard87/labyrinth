#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Maze {

    public:
        typedef struct {
            int row;
            int col;
        } Point;

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
    
    private:

        int rows;
        int columns;
        int** maze;
        int makeHoles;
        int makeHolesPercent = 18;

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

}; 