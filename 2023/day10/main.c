#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    north,
    south,
    east,
    west,
    none
} direction;

typedef struct
{
    char **map;
    int width;
    int height;
} grid;

void printgrid(grid *g)
{
    for (int y = 0; y < g->height; y++)
    {
        for (int x = 0; x < g->width; x++)
        {
            printf("%c ", g->map[x][y]);
        }
        printf("\n");
    }
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
        for (int x = 0; x < g->height; x++)
        {
            ch = fgetc(file);
            g->map[x][y] = ch;
        }
        fgetc(file);
    }
}

void findstart(grid *g, int *sx, int *sy)
{
    for (int y = 0; y < g->height; y++)
    {
        for (int x = 0; x < g->height; x++)
            if (g->map[x][y] == 'S')
            {
                *sx = x;
                *sy = y;
                return;
            }
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

void walkmaze(grid *g)
{
    int x = -1;
    int y = -1;
    findstart(g, &x, &y);
    printf("Start at (%d, %d)\n", x, y);

    int steps = 0;
    direction too = none;
    if (y > 0 && checkconnects(north, g->map[x][y - 1]))
        too = north;
    if (y < g->height - 2 && checkconnects(south, g->map[x][y + 1]))
        too = south;
    if (x > 0 && checkconnects(west, g->map[x - 1][y]))
        too = west;
    if (x < g->width - 2 && checkconnects(east, g->map[x + 1][y]))
        too = east;
    // too = east;

    do
    {
        walkdirection(too, &x, &y);
        // printf("on: (%d, %d)\n", x, y);
        too = nextpipe(opposite(too), g->map[x][y]);
        steps++;
        // sleep(2);
    } while (g->map[x][y] != 'S');

    printf("steps: %d\n", steps);
    printf("furthest: %d\n", steps / 2);
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
        ny = y * 3;
        for (int x = 0; x < g->height; x++)
        {
            nx = x * 3;
            switch (g->map[x][y])
            {
            case '.':
                newmap[nx][ny] = '.';
                newmap[nx + 1][ny] = '.';
                newmap[nx + 2][ny] = '.';
                newmap[nx][ny + 1] = '.';
                newmap[nx + 1][ny + 1] = '.';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx][ny + 2] = '.';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case '|':
                newmap[nx][ny] = '.';
                newmap[nx + 1][ny] = '|';
                newmap[nx + 2][ny] = '.';
                newmap[nx][ny + 1] = '.';
                newmap[nx + 1][ny + 1] = '|';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx][ny + 2] = '.';
                newmap[nx + 1][ny + 2] = '|';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case '-':
                newmap[nx][ny] = '.';
                newmap[nx + 1][ny] = '.';
                newmap[nx + 2][ny] = '.';
                newmap[nx][ny + 1] = '-';
                newmap[nx + 1][ny + 1] = '-';
                newmap[nx + 2][ny + 1] = '-';
                newmap[nx][ny + 2] = '.';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case 'L':
                newmap[nx][ny] = '.';
                newmap[nx + 1][ny] = '|';
                newmap[nx + 2][ny] = '.';
                newmap[nx][ny + 1] = '.';
                newmap[nx + 1][ny + 1] = 'L';
                newmap[nx + 2][ny + 1] = '-';
                newmap[nx][ny + 2] = '.';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case 'J':
                newmap[nx][ny] = '.';
                newmap[nx + 1][ny] = '|';
                newmap[nx + 2][ny] = '.';
                newmap[nx][ny + 1] = '-';
                newmap[nx + 1][ny + 1] = 'J';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx][ny + 2] = '.';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case '7':
                newmap[nx][ny] = '.';
                newmap[nx + 1][ny] = '.';
                newmap[nx + 2][ny] = '.';
                newmap[nx][ny + 1] = '-';
                newmap[nx + 1][ny + 1] = '7';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx][ny + 2] = '.';
                newmap[nx + 1][ny + 2] = '|';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case 'F':
                newmap[nx][ny] = '.';
                newmap[nx + 1][ny] = '.';
                newmap[nx + 2][ny] = '.';
                newmap[nx][ny + 1] = '.';
                newmap[nx + 1][ny + 1] = 'F';
                newmap[nx + 2][ny + 1] = '-';
                newmap[nx][ny + 2] = '.';
                newmap[nx + 1][ny + 2] = '|';
                newmap[nx + 2][ny + 2] = '.';
                break;
            case 'S':
                newmap[nx][ny] = 'S';
                newmap[nx + 1][ny] = 'S';
                newmap[nx + 2][ny] = 'S';
                newmap[nx][ny + 1] = 'S';
                newmap[nx + 1][ny + 1] = 'S';
                newmap[nx + 2][ny + 1] = 'S';
                newmap[nx][ny + 2] = 'S';
                newmap[nx + 1][ny + 2] = 'S';
                newmap[nx + 2][ny + 2] = 'S';
                break;
            default:
                newmap[nx][ny] = '.';
                newmap[nx + 1][ny] = '.';
                newmap[nx + 2][ny] = '.';
                newmap[nx][ny + 1] = '.';
                newmap[nx + 1][ny + 1] = '.';
                newmap[nx + 2][ny + 1] = '.';
                newmap[nx][ny + 2] = '.';
                newmap[nx + 1][ny + 2] = '.';
                newmap[nx + 2][ny + 2] = '.';
                break;
            }
        }
    }
    g->map = newmap;
    g->width *= 3;
    g->height *= 3;
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
    printf("lines: %d, width: %d\n", height, width);

    char **map;
    map = malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++)
    {
        map[i] = malloc(width * sizeof(char));
    }

    grid grid;
    grid.map = map;
    grid.width = width;
    grid.height = height;

    readfile(file, &grid);
    printgrid(&grid);

    walkmaze(&grid);

    // scalemaze(&grid);
    // printgrid(&grid);

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
