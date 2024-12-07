#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
} way;

typedef struct
{
    int x;
    int y;
    way direction;
} guard_t;

int front[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
way turnright[4] = {RIGHT, LEFT, UP, DOWN};

char *grid;
int WIDTH = 0, HEIGHT = 0;
#define GRID(x, y) (grid[(y) * WIDTH + (x)])

char *walked;
guard_t start;
guard_t guard;

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
            if (c == '^')
            {
                if (!WIDTH)
                    start.x = 1;
                else
                    start.x = i % WIDTH;
                start.y = HEIGHT;
                start.direction = UP;
            }
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

void simulateguard()
{
    guard.x = start.x;
    guard.y = start.y;
    guard.direction = start.direction;
    while (1)
    {
        if (GRID(guard.x + front[guard.direction][0], guard.y + front[guard.direction][1]) == '#')
        {
            guard.direction = turnright[guard.direction];
            walked[(guard.y) * WIDTH + (guard.x)] = 'X';
            continue;
        }
        walked[(guard.y) * WIDTH + (guard.x)] = 'X';

        guard.x += front[guard.direction][0];
        guard.y += front[guard.direction][1];

        if (guard.x >= WIDTH || guard.x < 0 || guard.y >= HEIGHT || guard.y < 0)
            return;
    }
}

// Part 2: hacky, ugly, slow. But it's 4am and i need sleep
int brute()
{
    guard.x = start.x;
    guard.y = start.y;
    guard.direction = start.direction;
    long count = 0;
    while (1)
    {
        count++;
        if (count > 10000)
            return 1;

        if (GRID(guard.x + front[guard.direction][0], guard.y + front[guard.direction][1]) == '#')
        {
            guard.direction = turnright[guard.direction];
            continue;
        }

        guard.x += front[guard.direction][0];
        guard.y += front[guard.direction][1];

        if (guard.x >= WIDTH || guard.x < 0 || guard.y >= HEIGHT || guard.y < 0)
            return 0;
    }
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

    walked = (char *)malloc(WIDTH * HEIGHT);
    memcpy(walked, grid, WIDTH * HEIGHT);

    simulateguard();

    long res1 = 0;
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        if (walked[i] == 'X')
            res1++;
    
    long res2 = 0;
    for (int y = 0; y < WIDTH; y++)
    {
        for (int x = 0; x < HEIGHT; x++)
        {
            if (GRID(x, y) == '.' && walked[y * WIDTH + x] == 'X')
            {
                GRID(x, y) = '#';
                if (brute())
                    res2++;
                GRID(x, y) = '.';

            }
        }
    }

    printf("Result part 1: %ld\n", res1);
    printf("Result part 2: %ld\n", res2);

    free(grid);
    free(walked);

    exit(0);
}
