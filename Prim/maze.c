// Portarlo a C++, así podemos utilizar listas etc.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_HOLES 60


int** allocMaze(int rows, int cols);
int** generateMaze(int rows, int cols);    
void createHoles(int** maze);
int random_int(int min, int max);
void generateStartingPoints(int** maze);   
void encloseMaze(int** maze, int rows, int cols);  
void randPrim(int** maze, int rows, int cols);    


int MAZE_ROWS = 0;
int MAZE_COLS = 0;

typedef struct {
    int row;
    int col;
} Point;

int** generateMaze(int rows, int cols){
    MAZE_ROWS = rows;
    MAZE_COLS = cols;    
    srand(time(NULL));    
    int** the_maze = allocMaze(MAZE_ROWS,MAZE_COLS); 
    
    randPrim(the_maze,MAZE_ROWS,MAZE_COLS);
    encloseMaze(the_maze, MAZE_ROWS,MAZE_COLS);
    generateStartingPoints(the_maze); 
    createHoles(the_maze);   

    return the_maze;
}

void createHoles(int** maze) {
    int holes = 0;
    
    Point points[MAX_HOLES];
    int generated_points = 0;
    while (generated_points < MAX_HOLES) {
        Point p;
        int is_wall = 0;

        do {
            p.row = random_int(1, MAZE_ROWS - 2);
            p.col = random_int(1, MAZE_COLS - 2);
            if (maze[p.row][p.col] == 1) {
                if (p.row - 1 > 0 && maze[p.row - 1][p.col] == 1) continue;
                if (p.row + 1 < MAZE_ROWS - 1 && maze[p.row + 1][p.col] == 1) continue;
                if (p.col - 1 > 0 && maze[p.row][p.col] - 1 == 1) continue;
                if (p.col + 1 < MAZE_COLS - 1 && maze[p.row][p.col + 1] == 1) continue;
                
                
                is_wall = 1;
            }
        } while (!is_wall);
        points[generated_points++] = p;
    }

    for (int i = 0; i < MAX_HOLES; i++) {
        Point p = points[i];
        printf("Point to be broken: %d, %d\n", p.row, p.col);
        maze[p.row][p.col] = 0;
    }


}

int random_int(int min, int max){
   return min + rand() % (max+1 - min);
}

void generateStartingPoints(int** maze) {
    for (int col = 1; col < MAZE_COLS - 1; col++) {
        if (maze[1][col] == 0) {
            maze[1][col] = 2;
            break;
        }
    }

    for (int col = MAZE_COLS - 1; col> 1; col--) {
        if (maze[MAZE_ROWS-2][col] == 0) {
            maze[MAZE_ROWS-2][col] = 3;
            break;
        }
    }
}

void printMaze(int** maze, int rows, int cols){
   for(int i = 0; i < rows; i++){  
        for(int k = 0; k < cols; k++){  
            switch(maze[i][k]){
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
        printf("\n");
    }
    printf("\n\n");
}

//Allocate a 2d array of ints, with a grid of walls
//works best with odd rows/cols
int** allocMaze(int rows, int cols){
     int x, y; // Column and row coordinates.
 
     // First, allocate the array of pointers to the columns
     int** array = (int**) (calloc(rows, sizeof(int*)));
 
     for (x = 0; x < rows; x++) {
         // Allocate each column array.
         array[x] = (int*) (calloc(cols, sizeof(int)));
 
         for(y = 0; y < cols; y++){
             if(y%2 != 0 && x%2 != 0)
                 array[x][y] = 0;
             else
                 array[x][y] = 1;
         }
     }
     return array;
}

int contains(Point* list,int length, Point aPoint){
    for(int k = 0; k < length; k++){
        if(list[k].row == aPoint.row && list[k].col == aPoint.col)
            return 1;
    }
    return 0;
}
int containsAlt(Point* list, int length, int row, int col){
    Point theCell;
    theCell.row = row;
    theCell.col = col;
    
    return contains(list, length, theCell);
}

void removeIndex(Point* list, int length, int index){
    for(int k = index+1; k < length; k++){
        list[k-1] = list[k];
    }
}

void removePoint(Point* list, int length, Point aPoint){
    for(int k = 0; k < length; k++){
        if(list[k].row == aPoint.row && list[k].col == aPoint.col)
            return removeIndex(list, length, k);
    }
    printf("\n\n -- No such Point %d %d Unvisited  \n\n", aPoint.row, aPoint.col);
}

void encloseMaze(int** maze, int rows, int cols){
    for(int i = 0; i < cols; i++){
        maze[0][i] = 1;
        maze[rows-1][i] = 1;
    }

    for(int k = 0; k < rows; k++){
        maze[k][0] = 1;
        maze[k][cols-1] = 1;
    }
}

//Adds nearby walls to the list of posns Given, from the maze at the specified posn
//Return the num of nearby walls added
int addNearbyWalls(int** maze, int rows, int cols, Point* list, int length, Point aPoint){
    int originalLength = length;
    if(aPoint.row -1  >= 0){
        Point thisWall;
        thisWall.row = aPoint.row -1;
        thisWall.col = aPoint.col;
       //If it is a wall, and is not already in the list, add it to the list 
        if(maze[thisWall.row][thisWall.col] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if(aPoint.row + 1 < rows){
        Point thisWall;
        thisWall.row = aPoint.row +1;
        thisWall.col = aPoint.col;

        if(maze[thisWall.row][thisWall.col] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if (aPoint.col -1 >= 0) {
        Point thisWall;
        thisWall.row = aPoint.row;
        thisWall.col = aPoint.col-1;

        if(maze[thisWall.row][thisWall.col] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if (aPoint.col + 1 < cols) {
        Point thisWall;
        thisWall.row = aPoint.row;
        thisWall.col = aPoint.col+1;
        
        if (maze[thisWall.row][thisWall.col] && !contains(list, length, thisWall)) {
            list[length] = thisWall;
            length++;
        }
    }

    //return the number of elements added 
    return length - originalLength;
}

void randPrim(int** maze, int rows, int cols){
    Point* wallList = (Point*)malloc(sizeof(Point) * rows * cols);
    int wallListLength = 0;

    Point* unvisited = (Point*)malloc(sizeof(Point)* rows * cols);

    int unvisitedIndex = 0; // index of the unvisited array;
    
    //Add all the unvisted cells to the list
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if(maze[i][k] == 0){
                Point nextPoint;
                nextPoint.row = i;
                nextPoint.col = k;
                unvisited[unvisitedIndex] = nextPoint;
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
            if(nextWall.row -1 >= 0 && containsAlt(unvisited, unvisitedIndex, nextWall.row-1, nextWall.col)){
                //set it to the next move, increment numNearUnvisited
                nextMove.row = nextWall.row -1;
                nextMove.col = nextWall.col;
                numNearUnvisited++;
            }

            if(nextWall.row +1 < rows && containsAlt(unvisited, unvisitedIndex, nextWall.row+1, nextWall.col)){
                nextMove.row = nextWall.row+1;
                nextMove.col = nextWall.col;
                numNearUnvisited++;
            }

            if(nextWall.col -1 >= 0 && containsAlt(unvisited, unvisitedIndex, nextWall.row, nextWall.col-1)){
                nextMove.row = nextWall.row;
                nextMove.col = nextWall.col-1;
                numNearUnvisited++;
            }

            if(nextWall.col +1 < cols && containsAlt(unvisited, unvisitedIndex, nextWall.row, nextWall.col+1)){
                nextMove.row = nextWall.row;
                nextMove.col = nextWall.col+1;
                numNearUnvisited++;
            }
            
            //If there is only one unvisited cell by the wall, make a passage
            //Then add the neighboring walls of that cell to the wall list
            if(numNearUnvisited == 1){
                maze[nextWall.row][nextWall.col] = 0;
                removePoint(unvisited, unvisitedIndex, nextMove);
                unvisitedIndex -= 1;
                wallListLength += addNearbyWalls(maze, rows, cols, wallList, wallListLength, nextMove);
            }
        }

        if(unvisitedIndex == 0) break;

        int nextIndex = rand() % unvisitedIndex;
        Point nextCell = unvisited[nextIndex];

        removeIndex(unvisited, unvisitedIndex, nextIndex);
        unvisitedIndex -= 1;
        
        wallListLength += addNearbyWalls(maze,rows,cols,wallList,wallListLength,nextCell);
    }

}