#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "directions.h"
#include "particle.h"
#include "point.h"

using namespace std;

//class Directions;

class Maze {

    public:
        
        Maze();
        Maze(int rows, int columns);
        Maze(int squared);
        
        int getRows();
        int getColumns();
        int** getMaze();

        void printMaze();
        bool isWall(Point::Point p);
        bool isWall(int row, int col);

        bool isSquared();

        int getValue(int row, int column);
    
        bool move(int agentIndex, Directions::Direction direction);
        Point getCurrentPosition(int agentIndex);
        Particle getAgent(int agentIndex);

        void set_position(int agentIndex, int x, int y);
        void init_movement(int agentIndex, int destination_x, int destination_y, int duration);
        void integrate(int agentIndex, long t);
        int getAgentsNum();

        Point getPlayerBase();

        std::vector<Directions::Direction> getAvailableMoves(int agentIndex);

        // int manhattanDistance(Point a, Point b);

    private:

        int rows;
        int columns;
        int** maze;
        int makeHoles;
        int makeHolesPercent = 18;
        int agentsNum = 2;

        Point playerPosition;
        Point enemyPosition;

        Particle playerParticle;
        Particle enemyParticle;

        Point playerBase;
        Point enemyBase;

        std::vector<Point::Point> walls;

        void generateMaze();
        void allocMaze();
        void randPrim();
        void removeIndex(Point* list, int length, int index);
        void removePoint(Point* list, int length, Point::Point aPoint);
        int contains(Point* list,int length, Point::Point aPoint);
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