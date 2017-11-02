typedef struct Node Node;
int generateMaze(int width, int height);
void draw(struct Node *nodes, int width, int height);
struct Node *getMaze();
int getI(struct Node *nodes, int i, int j, int width);
