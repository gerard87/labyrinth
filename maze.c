 
//Code by Jacek Wieczorek

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct
{
	int x, y; //Node position - little waste of memory, but it allows faster generation
	void *parent; //Pointer to parent node
	char c; //Character to be displayed
	char dirs; //Directions that stil haven't been explored
	int i;
} Node;

Node *nodes; //Nodes array


int init(int width, int height)
{
	int i, j;
	Node *n;
	
	//Allocate memory for maze
	nodes = calloc( width * height, sizeof( Node ) );
	if ( nodes == NULL ) return 1;
		
	//Setup crucial nodes
	for ( i = 0; i < width; i++ )
	{
		for ( j = 0; j < height; j++ )
		{
			n = nodes + i + j * width;
			if ( i * j % 2 ) 
			{
				n->x = i;
				n->y = j;
				n->dirs = 15; //Assume that all directions can be explored (4 youngest bits set)
				n->c = ' ';
				n->i = 0; 
			}
			else {
				n->c = '#'; //Add walls between nodes
				n->i = 1; 
			}
		}
	}
	return 0;
}

Node *link(Node *n, int width, int height)
{
	//Connects node to random neighbor (if possible) and returns
	//address of next node that should be visited

	int x, y;
	char dir;
	Node *dest;
	
	//Nothing can be done if null pointer is given - return
	if ( n == NULL ) return NULL;
	
	//While there are directions still unexplored
	while ( n->dirs )
	{
		//Randomly pick one direction
		dir = ( 1 << ( rand( ) % 4 ) );
		
		//If it has already been explored - try again
		if ( ~n->dirs & dir ) continue;
		
		//Mark direction as explored
		n->dirs &= ~dir;
		
		//Depending on chosen direction
		switch ( dir )
		{
			//Check if it's possible to go right
			case 1:
				if ( n->x + 2 < width )
				{
					x = n->x + 2;
					y = n->y;
				}
				else continue;
				break;
			
			//Check if it's possible to go down
			case 2:
				if ( n->y + 2 < height )
				{
					x = n->x;
					y = n->y + 2;
				}
				else continue;
				break;
			
			//Check if it's possible to go left	
			case 4:
				if ( n->x - 2 >= 0 )
				{
					x = n->x - 2;
					y = n->y;
				}
				else continue;
				break;
			
			//Check if it's possible to go up
			case 8:
				if ( n->y - 2 >= 0 )
				{
					x = n->x;
					y = n->y - 2;
				}
				else continue;
				break;
		}
		
		//Get destination node into pointer (makes things a tiny bit faster)
		dest = nodes + x + y * width;
		
		//Make sure that destination node is not a wall
		if ( !dest->i )
		{
			//If destination is a linked node already - abort
			if ( dest->parent != NULL ) continue;
			
			//Otherwise, adopt node
			dest->parent = n;
			
			//Remove wall between nodes
			nodes[n->x + ( x - n->x ) / 2 + ( n->y + ( y - n->y ) / 2 ) * width].c = ' ';
			nodes[n->x + ( x - n->x ) / 2 + ( n->y + ( y - n->y ) / 2 ) * width].i = 0;
			
			//Return address of the child node
			return dest;
		}
	}
	
	//If nothing more can be done here - return parent's address
	return n->parent;
}

void draw(Node *nodes, int width, int height)
{
	int i, j;

	//Outputs maze to terminal - nothing special
	for ( i = 0; i < height; i++ )
	{
		for ( j = 0; j < width; j++ )
		{
			printf( "%c", nodes[j + i * width].c );
		}
		printf( "\n" );
	}
}

int getI(Node *nodes, int i, int j, int width) {
	return nodes[j + i * width].i;
}

Node *getMaze() {
	return nodes;
}

int generateMaze(int width, int height)
{
	int i, badarg;
	long seed;
	Node *start, *last;

	//Seed random generator
	srand( time( NULL ) );
	
	//Initialize maze
	if ( init(width, height) )
	{
		fprintf( stderr, "Out of memory!\n" );
		exit( 1 );
	}
	
	//Setup start node
	start = nodes + 1 + width;
	start->parent = start;
	last = start;
	
	//Connect nodes until start node is reached and can't be left
	while ( ( last = link(last, width, height) ) != start );
	//draw(width, height);
}
