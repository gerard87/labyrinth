#include <math.h>
#include <time.h>
#include <random>

#include "maze.h"
#include "particle.h"
#include "point.h"


Maze::Maze() {}

Maze::Maze(int rows, int columns) {
    this->rows = rows;
    this->columns = columns;
    this->generateMaze();
}

Maze::Maze(int squared) {
    this->rows = squared;
    this->columns = squared;
    this->generateMaze();
}

Maze::Maze(int debug, int rows, int columns) {
    this->debug = debug;
    this->rows = rows;
    this->columns = columns;
    this->generateMaze();
}

// Getters
int Maze::getRows() {
    return this->rows;
}

int Maze::getColumns() {
    return this->columns;
}

bool Maze::isSquared() {
    return this->rows == this->columns;
}

int** Maze::getMaze() {
    return this->maze;
}

int Maze::getValue(int row, int column) {
    return this->maze[row][column];
}

bool Maze::isWall(Point p) {
    return isWall(p.getRow(),p.getCol());
}

bool Maze::isWall(int row, int col) {
    return this->maze[row][col] == 1;
}

bool Maze::agentInPosition(int agentIndex, Point pos) {
    switch (agentIndex) {
        case 0:
            if (pos == this->enemyParticle.getPosition()) {
                return true;
            } else {
                return false;
            }
        case 1: 
            if (pos == this->playerParticle.getPosition()) {
                return true;
            } else {
                return false;
            }
        default:
            return false;
    }
}

bool Maze::checkValidMove(int agentIndex, Point to) {
    return (!isWall(to) && !agentInPosition(agentIndex, to));
}

Point Maze::getPlayerBase() {
    return this->playerBase;
}

Point Maze::getEnemyBase() {
    return this->enemyBase;
}

Point Maze::getCurrentPosition(int agentIndex) {
    return getAgent(agentIndex)->getPosition();
}

Particle* Maze::getAgent(int agentIndex) {
    if (agentIndex == 0) {
        return &this->playerParticle;
    } else {
        return &this->enemyParticle;
    }
}

std::vector<Directions::Direction> Maze::getAvailableMoves(int agentIndex) {
    std::vector<Directions::Direction> availableMoves;

    Point pos = getCurrentPosition(agentIndex);
    Point newPos;

    newPos = Point(pos.getRow(), pos.getCol() + 1);
    if (checkValidMove(agentIndex, newPos)) {
        availableMoves.push_back(Directions::RIGHT);
    }

    newPos = Point(pos.getRow(), pos.getCol() - 1);
    if (checkValidMove(agentIndex, newPos)) {
        availableMoves.push_back(Directions::LEFT);
    }

    newPos = Point(pos.getRow() + 1, pos.getCol());
    if (checkValidMove(agentIndex, newPos)) {
        availableMoves.push_back(Directions::DOWN);

    }

    newPos = Point(pos.getRow() - 1, pos.getCol());
    if (checkValidMove(agentIndex, newPos)) {
        availableMoves.push_back(Directions::UP);
    }

    if (availableMoves.size() == 0) {
        availableMoves.push_back(Directions::STOP);
    }

    return availableMoves;
}

int Maze::getAgentsNum() {
    return this->agentsNum;
}

// Maze game helpers.
bool Maze::move(int agentIndex, Directions::Direction direction) {
    Particle* agent = getAgent(agentIndex);
    Point p = agent->getPosition();
    int row = p.getRow();
    int col = p.getCol();
    
    Point newPos = Point(row + direction.y, col + direction.x);
    if (checkValidMove(agentIndex, newPos) && agent->getState() == Particle::QUIET) {
        agent->setPoint(Point(row + direction.y, col + direction.x));
        if (debug) if (agentIndex == 0) printMaze();
            
        return true;
    } else return false;
}


// Maze generation helper methods.

void Maze::generateMaze(){
    srand(time(NULL));    
    allocMaze(); 
    randPrim();
    encloseMaze();
    generateStartingPoints(); 
    setHolesQuantity();
    createHoles(); 
    if (debug) printMaze();

}

void Maze::setHolesQuantity() {
    for (int row = 1; row < this->rows - 1; row++) {
        for (int col = 1; col < this->columns - 2; col++) {
            if (this->isWall(row, col)) {
                this->walls.push_back(Point(row, col));
            }
        }  
    }
    this->makeHoles = (int)(this->walls.size() * this->makeHolesPercent /(double)100);
}

void Maze::removePoint(std::vector<Point> & points, int row, int col) {
    points.erase(
        std::remove_if(points.begin(), points.end(), [&](Point /* const */ & point) {
            return point.getRow() == row && point.getCol() == col;
        }),
        points.end());
}

void Maze::createHoles() {   

    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> choose(0, this->walls.size() - 1);
    
    Point points[this->makeHoles];
    int generated_points = 0;
    while (generated_points < this->makeHoles) {
        Point p;
        int is_wall;

        do {
            is_wall = 0;
            p = this->walls[choose(engine)];
            if (isWall(p) &&
                        ((p.getRow() - 1) > 0 && isWall(p.getRow() - 1, p.getCol())) && 
                        ((p.getRow() + 1) < (this->rows - 1) && isWall(p.getRow() + 1, p.getCol())) &&
                        ((p.getCol() - 1) > 0 && isWall(p.getRow(), p.getCol() - 1)) &&
                        ((p.getCol() + 1) < (this->columns - 1) && isWall(p.getRow(), p.getCol() + 1))
                    ) 
            {
                continue;
            } else {
                is_wall = 1;
            }
        } while (!is_wall);
        removePoint(this->walls, p.getRow(), p.getCol());
        this->maze[p.getRow()][p.getCol()] = 0;
        generated_points++;
    }
}

int Maze::random_int(int min, int max){
   return min + rand() % (max+1 - min);
}

void Maze::generateStartingPoints() {
    for (int col = 1; col < this->columns - 1; col++) {
        if (this->maze[1][col] == 0) {
            this->maze[1][col] = 2;
            this->playerParticle.setPoint(Point(1, col));
            this->playerBase = Point(1, col);
            break;
        }
    }

    for (int col = this->columns - 1; col> 1; col--) {
        if (this->maze[this->rows-2][col] == 0) {
            this->maze[this->rows-2][col] = 3;
            this->enemyParticle.setPoint(Point(this->rows-2, col));
            this->enemyBase = Point(this->rows-2, col);
            break;
        }
    }
}

void Maze::encloseMaze(){
    for(int i = 0; i < this->columns; i++){
        maze[0][i] = 1;
        maze[this->rows-1][i] = 1;
    }

    for(int i = 0; i < this->rows; i++){
        maze[i][0] = 1;
        maze[i][this->columns-1] = 1;
    }
}

void Maze::allocMaze(){ 
     // First, allocate the array of pointers to the columns
     int** array = (int**) (calloc(this->rows, sizeof(int*)));
 
     for (int x = 0; x < this->rows; x++) {
         // Allocate each column array.
         array[x] = (int*) (calloc(this->columns, sizeof(int)));
 
         for(int y = 0; y < this->columns; y++){
             if(y%2 != 0 && x%2 != 0) array[x][y] = 0;
             else array[x][y] = 1;
         }
     }
     this->maze = array;
}

void Maze::printMaze() {
    for(int i = 0; i < this->rows; i++){  
        for(int k = 0; k < this->columns; k++){  
            Point pos = Point(i, k);
            if (pos == getCurrentPosition(0)) {
                printf("ñ");
            } else {
            switch(this->maze[i][k]){
                case 3:
                    printf("C");
                    break;  
                case 2:
                    printf("P");
                    break;                
                case 1:
                    printf("0");
                    break;
                case 0:
                    printf(" ");
                    break;
             }
         }
        }
        printf("\n");
    }
    printf("\n\n");
}

void Maze::removeIndex(Point* list, int length, int index){
    for(int k = index+1; k < length; k++){
        list[k-1] = list[k];
    }
}

void Maze::removePoint(Point* list, int length, Point aPoint){
    for(int k = 0; k < length; k++){
        if(list[k] == aPoint) return removeIndex(list, length, k);
    }
    printf("\n\n -- No such Point %d %d Unvisited  \n\n", aPoint.getRow(), aPoint.getCol());
}

int Maze::contains(Point* list,int length, Point aPoint){
    for(int k = 0; k < length; k++){
        if(list[k] ==aPoint) return 1;
    }
    return 0;
}

int Maze::containsAlt(Point* list, int length, int row, int col){
    return contains(list, length, Point(row, col));
}

int Maze::addNearbyWalls(Point* list, int length, Point aPoint){
    int originalLength = length;
    if(aPoint.getRow() -1 >= 0){
        Point thisWall = Point(aPoint.getRow() - 1, aPoint.getCol());
        //If it is a wall, and is not already in the list, add it to the list 
        if(this->maze[thisWall.getRow()][thisWall.getCol()] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if(aPoint.getRow() + 1 < this->rows){
        Point thisWall = Point(aPoint.getRow() + 1, aPoint.getCol());

        if(this->maze[thisWall.getRow()][thisWall.getCol()] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if (aPoint.getCol() -1 >= 0) {
        Point thisWall = Point(aPoint.getRow(), aPoint.getCol() - 1);

        if(this->maze[thisWall.getRow()][thisWall.getCol()] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if (aPoint.getCol() + 1 < this->columns) {
        Point thisWall = Point(aPoint.getRow(), aPoint.getCol() + 1);
        
        if (this->maze[thisWall.getRow()][thisWall.getCol()] && !contains(list, length, thisWall)) {
            list[length] = thisWall;
            length++;
        }
    }

    //return the number of elements added 
    return length - originalLength;
}

void Maze::randPrim() {
    Point* wallList = (Point*)malloc(sizeof(Point) * this->rows * this->columns);
    int wallListLength = 0;

    Point* unvisited = (Point*)malloc(sizeof(Point)* this->rows * this->columns);

    int unvisitedIndex = 0; // index of the unvisited array;
    
    //Add all the unvisted cells to the list
    for(int i = 0; i < this->rows; i++){
        for(int k = 0; k < this->columns; k++){
            if(this->maze[i][k] == 0){
                unvisited[unvisitedIndex] = Point(i, k);
                unvisitedIndex++;
            }
        }
    }

    while(unvisitedIndex > 0){
        while(wallListLength > 0){
            int nextWallIndex = rand() % wallListLength;
            Point nextWall = wallList[nextWallIndex];
            removeIndex(wallList, wallListLength, nextWallIndex);
            wallListLength--;
            
            int numNearUnvisited = 0;
            Point nextMove; 

            //If the next position is valid and unvisited
            if(nextWall.getRow() - 1 >= 0 && containsAlt(unvisited, unvisitedIndex, nextWall.getRow()-1, nextWall.getCol())){
                //set it to the next move, increment numNearUnvisited
                nextMove = Point(nextWall.getRow() - 1, nextWall.getCol());
                numNearUnvisited++;
            }

            if(nextWall.getRow() + 1 < this->rows && containsAlt(unvisited, unvisitedIndex, nextWall.getRow()+1, nextWall.getCol())){
                nextMove = Point(nextWall.getRow() + 1, nextWall.getCol());
                numNearUnvisited++;
            }

            if(nextWall.getCol() - 1 >= 0 && containsAlt(unvisited, unvisitedIndex, nextWall.getRow(), nextWall.getCol()-1)){
                nextMove = Point(nextWall.getRow(), nextWall.getCol() - 1);
                numNearUnvisited++;
            }

            if(nextWall.getCol() + 1 < this->columns && containsAlt(unvisited, unvisitedIndex, nextWall.getRow(), nextWall.getCol()+1)){
                nextMove = Point(nextWall.getRow(), nextWall.getCol() + 1);
                numNearUnvisited++;
            }
            
            //If there is only one unvisited cell by the wall, make a passage
            //Then add the neighboring walls of that cell to the wall list
            if(numNearUnvisited == 1){
                this->maze[nextWall.getRow()][nextWall.getCol()] = 0;
                removePoint(unvisited, unvisitedIndex, nextMove);
                unvisitedIndex -= 1;
                wallListLength += addNearbyWalls(wallList, wallListLength, nextMove);
            }
        }

        if(unvisitedIndex == 0) break;

        int nextIndex = rand() % unvisitedIndex;
        Point nextCell = unvisited[nextIndex];

        removeIndex(unvisited, unvisitedIndex, nextIndex);
        unvisitedIndex -= 1;
        
        wallListLength += addNearbyWalls(wallList,wallListLength,nextCell);
    }

}