#include <stdio.h>
#include <stdlib.h>

char *grid;
int WIDTH = 0, HEIGHT = 0;
#define GRID(x, y) (grid[(y) * WIDTH + (x)])

void readgrid(FILE *file)
{
    char c;
    int i = 0;
    int gridcap = 64;
    int linecount = 0;
    grid = (char *)malloc(gridcap);
    while (1)
    {
        c = fgetc(file);
        if (c != '\n')
        {
            grid[i] = c;
            i++;
        }
        else
        {
            HEIGHT++;
            linecount++;
            if (!WIDTH)
                WIDTH = i;
        }
        if (feof(file))
            break;
        if (gridcap <= i)
        {
            gridcap *= 2;
            grid = (char *)realloc(grid, gridcap);
        }
    }
}

char checkchar(int x, int y, char c)
{
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        return 0;
    if (GRID(x, y) == c)
        return 1;
    return 0;
}

#define XMAS "XMAS"
#define XMASLEN (int)sizeof(XMAS) - 1
#define waycount 8
int ways[waycount][2] = {{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},};

int checkxmas(int x, int y)
{
    int count = 0;
    int i, j;
    int hold[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (i = 0; i < XMASLEN; i++)
    {
        for (j = 0; j < waycount; j++)
            if (checkchar(x + (ways[j][0] * i), y + (ways[j][1] * i), XMAS[i]))
                hold[j]++;
        for (j = 0; j < waycount; j++)
            if (hold[j] == XMASLEN)
                count++;
    }
    return count;
}

int checkcrossmas(int x, int y)
{
    // im dreaming of matrix rotation
    if (
        checkchar(x, y, 'A') &&
        ((

            checkchar(x+1, y+1, 'S') &&
            checkchar(x-1, y+1, 'M') &&
            checkchar(x-1, y-1, 'M') &&
            checkchar(x+1, y-1, 'S')
        ) || (
            checkchar(x+1, y+1, 'M') &&
            checkchar(x-1, y+1, 'M') &&
            checkchar(x-1, y-1, 'S') &&
            checkchar(x+1, y-1, 'S')
        ) || (
            checkchar(x+1, y+1, 'S') &&
            checkchar(x-1, y+1, 'S') &&
            checkchar(x-1, y-1, 'M') &&
            checkchar(x+1, y-1, 'M')
        ) || (
            checkchar(x+1, y+1, 'M') &&
            checkchar(x-1, y+1, 'S') &&
            checkchar(x-1, y-1, 'S') &&
            checkchar(x+1, y-1, 'M')
        ))
    ) return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give input file as an argument.");
        exit(0);
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", argv[1]);
        exit(1);
    }
    readgrid(file);
    int res1 = 0, res2 = 0;
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            res1 += checkxmas(x, y);
            res2 += checkcrossmas(x, y);
        }
    printf("Result part 1: %d\nResult part 2: %d\n", res1, res2);
    free(grid);

    exit(0);
}
