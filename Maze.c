// Portarlo a C++, así podemos utilizar listas etc.
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MAZE_ROWS 25
#define MAZE_COLS 50

typedef struct {
    int row;
    int col;
} Point;

int contains(Point* list,int length, Point aPoint);
int containsAlt(Point* list, int length, int row, int col);
int** allocMaze(int rows, int cols);
void randPrim(int** maze, int rows, int cols);
void printMaze(int** maze, int rows, int cols);
void removeIndex(Point* list, int length, int index);
void removePoint(Point* list, int length, Point aPoint);
int addNearbyWalls(int** maze, int rows, int cols, Point* list, int length, Point aPoint);
void encloseMaze(int** maze, int rows, int cols);

int main(){

   srand(time(NULL));
   int** maze = allocMaze(MAZE_ROWS,MAZE_COLS); 
   
   randPrim(maze,MAZE_ROWS,MAZE_COLS);
   encloseMaze(maze, MAZE_ROWS,MAZE_COLS);
 
   printMaze(maze, MAZE_ROWS, MAZE_COLS);
}


//Print the maze, x's for walls
void printMaze(int** maze, int rows, int cols){
   for(int i = 0; i < rows; i++){  
        for(int k = 0; k < cols; k++){  
            switch(maze[i][k]){
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

//return 1 if the posn list contains the posn, else 0
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

//Remove the posn at the specified index and shift late elements left
void removeIndex(Point* list, int length, int index){
    for(int k = index+1; k < length; k++){
        list[k-1] = list[k];
    }
}

void removePoint(Point* list, int length, Point aPoint){
    //Find the index of the posn then use other function
    for(int k = 0; k < length; k++){
        if(list[k].row == aPoint.row && list[k].col == aPoint.col)
            return removeIndex(list, length, k);
    }
    printf("\n\n -- No such Point %d %d Unvisited  \n\n", aPoint.row, aPoint.col);
}

//Make all the borders walls
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
            //printMaze(maze,rows,cols);
        }

        if(unvisitedIndex == 0)
            break;

        int nextIndex = rand() % unvisitedIndex;
        Point nextCell = unvisited[nextIndex];

        removeIndex(unvisited, unvisitedIndex, nextIndex);
        unvisitedIndex -= 1;
        
        //add those walls to nthe list
        wallListLength += addNearbyWalls(maze,rows,cols,wallList,wallListLength,nextCell);
    }

}