#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

typedef enum
{
    north = 1,
    south = 2,
    east = 3,
    west = 4,
    none = 9
} direction;

typedef struct
{
    char **map;
    int width;
    int height;
} grid;

void freemap(grid *g)
{
    for (int y = 0; y < g->height; y++)
    {
        free(g->map[y]);
    }
    free(g->map);
}

void scalemaze(grid *g)
{
    char **newmap;
    newmap = malloc(g->height * 3 * sizeof(char *));
    for (int i = 0; i < g->height * 3; i++)
    {
        newmap[i] = malloc(g->width * 3 * sizeof(char));
    }
    int nx, ny;
    for (int y = 0; y < g->height; y++)
    {
        nx = y * 3;
        for (int x = 0; x < g->width; x++)
        {
            ny = x * 3;
            switch (g->map[y][x])
            {
            case '.':
                newmap[nx + 0][ny + 0] = '.';
                newmap[nx + 0][ny + 1] = '.';
                newmap[nx + 0][ny + 2] = '.';
                newmap[nx + 1][ny + 0] = '.';
                newmap[nx + 1][ny + 1] = '.';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 0] = '.';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case '|':
                newmap[nx + 0][ny + 0] = '.';
                newmap[nx + 0][ny + 1] = '|';
                newmap[nx + 0][ny + 2] = '.';
                newmap[nx + 1][ny + 0] = '.';
                newmap[nx + 1][ny + 1] = '|';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 0] = '.';
                newmap[nx + 2][ny + 1] = '|';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case '-':
                newmap[nx + 0][ny + 0] = '.';
                newmap[nx + 0][ny + 1] = '.';
                newmap[nx + 0][ny + 2] = '.';
                newmap[nx + 1][ny + 0] = '-';
                newmap[nx + 1][ny + 1] = '-';
                newmap[nx + 1][ny + 2] = '-';
                newmap[nx + 2][ny + 0] = '.';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;

            case 'L':
                newmap[nx + 0][ny + 0] = '.';
                newmap[nx + 0][ny + 1] = '|';
                newmap[nx + 0][ny + 2] = '.';
                newmap[nx + 1][ny + 0] = '.';
                newmap[nx + 1][ny + 1] = 'L';
                newmap[nx + 1][ny + 2] = '-';
                newmap[nx + 2][ny + 0] = '.';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case 'J':
                newmap[nx + 0][ny + 0] = '.';
                newmap[nx + 0][ny + 1] = '|';
                newmap[nx + 0][ny + 2] = '.';
                newmap[nx + 1][ny + 0] = '-';
                newmap[nx + 1][ny + 1] = 'J';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 0] = '.';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case '7':
                newmap[nx + 0][ny + 0] = '.';
                newmap[nx + 0][ny + 1] = '.';
                newmap[nx + 0][ny + 2] = '.';
                newmap[nx + 1][ny + 0] = '-';
                newmap[nx + 1][ny + 1] = '7';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 0] = '.';
                newmap[nx + 2][ny + 1] = '|';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case 'F':
                newmap[nx + 0][ny + 0] = '.';
                newmap[nx + 0][ny + 1] = '.';
                newmap[nx + 0][ny + 2] = '.';
                newmap[nx + 1][ny + 0] = '.';
                newmap[nx + 1][ny + 1] = 'F';
                newmap[nx + 1][ny + 2] = '-';
                newmap[nx + 2][ny + 0] = '.';
                newmap[nx + 2][ny + 1] = '|';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case 'S':
                newmap[nx + 0][ny + 0] = '.';
                newmap[nx + 0][ny + 1] = '|';
                newmap[nx + 0][ny + 2] = '.';
                newmap[nx + 1][ny + 0] = '-';
                newmap[nx + 1][ny + 1] = 'S';
                newmap[nx + 1][ny + 2] = '-';
                newmap[nx + 2][ny + 0] = '.';
                newmap[nx + 2][ny + 1] = '|';
                newmap[nx + 2][ny + 2] = '.';
                break;
            default:
                newmap[nx + 0][ny + 0] = ',';
                newmap[nx + 0][ny + 1] = ',';
                newmap[nx + 0][ny + 2] = ',';
                newmap[nx + 1][ny + 0] = ',';
                newmap[nx + 1][ny + 1] = ',';
                newmap[nx + 1][ny + 2] = ',';
                newmap[nx + 2][ny + 0] = ',';
                newmap[nx + 2][ny + 1] = ',';
                newmap[nx + 2][ny + 2] = ',';
                break;
            }
        }
    }
    freemap(g);
    g->map = newmap;
    g->width *= 3;
    g->height *= 3;
}

int countlines(char *filepath)
{
    FILE *file = fopen(filepath, "r");
    int lines = 0;
    char ch;
    while (!feof(file))
    {
        ch = fgetc(file);
        if (ch == '\n')
            lines++;
    }
    fclose(file);
    return lines;
}

int countwidth(char *filepath)
{
    FILE *file = fopen(filepath, "r");
    char ch;
    int count = -1;
    do
    {
        ch = fgetc(file);
        count++;
    } while (ch != '\n');
    fclose(file);
    return count;
}

void readfile(FILE *file, grid *g)
{
    char ch;
    for (int y = 0; y < g->height; y++)
    {
        for (int x = 0; x < g->width; x++)
        {
            ch = fgetc(file);
            g->map[y][x] = ch;
        }
        fgetc(file);
    }
}

void findstart(grid *g, int *sx, int *sy)
{
    for (int y = 0; y < g->height; y++)
        for (int x = 0; x < g->width; x++)
            if (g->map[y][x] == 'S')
            {
                *sx = x;
                *sy = y;
                return;
            }
}

direction nextpipe(direction from, char nextpipe)
{
    direction nextdirection = none;
    switch (nextpipe)
    {
    case '|':
        if (from == south)
            nextdirection = north;
        else if (from == north)
            nextdirection = south;
        break;
    case '-':
        if (from == west)
            nextdirection = east;
        else if (from == east)
            nextdirection = west;
        break;
    case 'L':
        if (from == north)
            nextdirection = east;
        else if (from == east)
            nextdirection = north;
        break;
    case 'J':
        if (from == north)
            nextdirection = west;
        else if (from == west)
            nextdirection = north;
        break;
    case '7':
        if (from == south)
            nextdirection = west;
        else if (from == west)
            nextdirection = south;
        break;
    case 'F':
        if (from == south)
            nextdirection = east;
        else if (from == east)
            nextdirection = south;
        break;
    }
    return nextdirection;
}

void walkdirection(direction too, int *x, int *y)
{
    switch (too)
    {
    case north:
        *y -= 1;
        return;
    case south:
        *y += 1;
        return;
    case east:
        *x += 1;
        return;
    case west:
        *x -= 1;
        return;
    case none:
        return;
    }
}

direction opposite(direction dir)
{
    switch (dir)
    {
    case north:
        return south;
    case south:
        return north;
    case east:
        return west;
    case west:
        return east;
    case none:
        return none;
    }
    return none;
}

char checkconnects(direction from, char pipe)
{
    if (nextpipe(opposite(from), pipe) == none)
        return 0;
    else
        return 1;
}

int walkmaze(grid *g, char replace, int scale)
{
    int x = -1;
    int y = -1;
    findstart(g, &x, &y);

    int steps = 0;
    direction too = none;
    if (y > scale - 1 && checkconnects(north, g->map[y - scale][x]))
        too = north;
    if (y < g->height - 2 && checkconnects(south, g->map[y + scale][x]))
        too = south;
    if (x > scale - 1 && checkconnects(west, g->map[y][x - scale]))
        too = west;
    if (x < g->width - 2 && checkconnects(east, g->map[y][x + scale]))
        too = east;
    int px, py;
    do
    {
        px = x;
        py = y;
        walkdirection(too, &x, &y);

        too = nextpipe(opposite(too), g->map[y][x]);
        if (replace && g->map[py][px] != 'S')
            g->map[py][px] = '#';
        steps++;
    } while (g->map[y][x] != 'S');
    return steps;
}

void flood(grid *g, char fill, int x, int y, char *inside)
{
    if (x < 0 || y < 0 || x >= g->width || y >= g->height)
    {
        *inside = 0;
        return;
    }
    if (g->map[y][x] != '#' && g->map[y][x] != fill && g->map[y][x] != 'S')
    {
        g->map[y][x] = fill;
        flood(g, fill, x + 1, y, inside);
        flood(g, fill, x - 1, y, inside);
        flood(g, fill, x, y + 1, inside);
        flood(g, fill, x, y - 1, inside);
    }
}

void findinside(grid *g, int *x, int *y)
{
    int sx = -1;
    int sy = -1;
    findstart(g, &sx, &sy);
    char fill = 'a';
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
        {
            fill++;
            if (i == 0 || j == 0)
                continue;
            char found = 1;
            flood(g, fill, sx + i, sy + j, &found);
            if (found)
            {
                *x = sx + i;
                *y = sy + j;
                return;
            }
        }
}

long countinside(grid *g)
{
    long count = 0;
    for (int y = 1; y < g->height; y += 3)
        for (int x = 1; x < g->width; x += 3)
            if (g->map[y][x] == 'I')
                count++;
    return count;
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
    int height, width;
    height = countlines(filepath);
    width = countwidth(filepath);

    char **map;
    map = malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++)
        map[i] = malloc(width * sizeof(char));

    grid grid;
    grid.map = map;
    grid.width = width;
    grid.height = height;

    readfile(file, &grid);
    
    int steps;
    steps = walkmaze(&grid, 0, 1);

    printf("Furthest away: %d\n", steps / 2);

    scalemaze(&grid);

    walkmaze(&grid, 1, 2);
    
    int sx, sy;
    char _;
    findinside(&grid, &sx, &sy);
    flood(&grid, 'I', sx, sy, &_);
    
    long c = countinside(&grid);
    
    printf("Inside spots: %ld\n", c);

    freemap(&grid);
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give input file as an argument.");
        exit(0);
    }
    parsefile(argv[1]);
    exit(0);
}
