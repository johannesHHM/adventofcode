#include <stdio.h>
#include <stdlib.h>

int WIDTH = -1;
int HEIGHT = -1;

#define MAXWAY 10
#define GRIDSIZE 256
#define NODESBUFFER 10000

#define GRID(x, y) (costgrid[(y)*WIDTH + (x)])

typedef enum
{
    up = 0,
    down = 1,
    left = 2,
    right = 3
} direction;

typedef struct
{
    int cost;
    int waycount;
    int x, y;
    direction dir;
} node;

int *costgrid;
int visitedmatrix[GRIDSIZE][GRIDSIZE][4][MAXWAY] = {0};

node nodes[NODESBUFFER];
int nodeslen = 0;

direction opposite[4] = {down, up, right, left};
int moves[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

void reset()
{
    for(int x = 0; x < GRIDSIZE; x++)
        for(int y = 0; y < GRIDSIZE; y++)
            for(int d = 0; d < 4; d++)
                for(int w = 0; w < MAXWAY; w++)
                    visitedmatrix[x][y][d][w] = 0;
    nodeslen = 0;
}

void addneighbours(node source, int minway, int maxway)
{
    for (int dir = 0; dir < 4; dir++)
    {
        if ((direction)dir == opposite[source.dir])
            continue;

        node n;
        n.dir = dir;
        n.cost = source.cost;
        n.x = source.x;
        n.y = source.y;
        n.waycount = (source.dir == (direction)dir && source.waycount != 0) ? source.waycount : 0;
        int stepcount = (source.dir == (direction)dir && source.waycount != 0) ? 1 : minway;
        for (int i = 0; i < stepcount; i++)
        {
            n.x += moves[dir][0];
            n.y += moves[dir][1];
            n.waycount += 1;
            if (n.x < 0 || n.y < 0 || n.x >= WIDTH || n.y >= HEIGHT || n.waycount > maxway ||
                visitedmatrix[n.x][n.y][n.dir][n.waycount - 1])
                goto skip;
            n.cost += GRID(n.x, n.y);
        }

        visitedmatrix[n.x][n.y][n.dir][n.waycount - 1] = 1;
        nodes[nodeslen] = n;
        nodeslen++;

    skip:;
    }
}

int run(int minway, int maxway)
{
    nodes[0] = (node){0, 0, 0, 0, right};
    nodeslen++;
    int currentnodeindex = 0;

    while (nodeslen)
    {
        node *currentnode = &nodes[currentnodeindex];

        if (currentnode->x == WIDTH - 1 && currentnode->y == HEIGHT - 1)
        {
            reset();
            return currentnode->cost;
        }
        addneighbours(*currentnode, minway, maxway);

        nodes[currentnodeindex] = nodes[nodeslen - 1];
        nodeslen--;

        currentnodeindex = 0;
        for (int i = 1; i < nodeslen; i++)
            if (nodes[currentnodeindex].cost > nodes[i].cost)
                currentnodeindex = i;
    }
    reset();
    printf("Could not find end node.\n");
    return -1;
}

void setgridsize(char *filepath)
{
    FILE *file = fopen(filepath, "r");
    int height = 0;
    int width = -1;
    char ch;
    while (!feof(file))
    {
        ch = fgetc(file);
        width++;
        if (ch == '\n')
        {
            WIDTH = width;
            height++;
            width = -1;
        }
    }
    HEIGHT = height;
}

void readgrid(FILE *file)
{
    char c;
    int i = 0;
    while (1)
    {
        c = fgetc(file);
        if (c != '\n')
        {
            costgrid[i] = (int)c - 48;
            i++;
        }
        if (feof(file))
            break;
    }
}

void parsefile(char *filepath)
{
    FILE *file;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        exit(0);
    }

    setgridsize(filepath);
    costgrid = malloc(WIDTH * HEIGHT * sizeof(node));

    readgrid(file);

    printf("Result part 1: %d\n", run(1, 3));
    printf("Result part 2: %d\n", run(4, 10));

    fclose(file);
    free(costgrid);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERROR: no input file");
        exit(0);
    }
    parsefile(argv[1]);
    exit(0);
}
