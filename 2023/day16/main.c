#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID(x, y) (grid[(y)*WIDTH + (x)])
#define ENERGIZED(x, y) (energized[(y)*WIDTH + (x)])

typedef enum
{
    up,
    down,
    left,
    right
} direction;

typedef struct
{
    int x, y;
    direction d;
} light;

int WIDTH, HEIGHT;
char *grid;
int *energized;

light *lights;
int lightslen = 0;

light *cache;
int cachelen = 0;

void addcache(light l)
{
    cache[cachelen] = l;
    cachelen++;
}

char checkincache(light *l)
{
    for (int i = 0; i < cachelen; i++)
    {
        if (cache[i].x == l->x && cache[i].y == l->y && cache[i].d == l->d)
            return 1;
    }
    return 0;
}

void addlight(int x, int y, direction d)
{
    if (checkincache(&(light){x, y, d}))
        return;
    lights[lightslen].x = x;
    lights[lightslen].y = y;
    lights[lightslen].d = d;
    addcache(lights[lightslen]);
    lightslen++;
}

void removelight(int i)
{
    memmove(&lights[i], &lights[i + 1], (lightslen - i - 1) * sizeof(light));
    lightslen--;
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

void stepup(light *l, int i)
{
    switch (GRID(l->x, l->y))
    {
    case '.':
        if (l->y == 0)
            removelight(i);
        else
            l->y--;
        break;
    case '|':
        if (l->y == 0)
            removelight(i);
        else
            l->y--;
        break;
    case '-':
        if (l->x == 0)
        {
            l->x++;
            l->d = right;
        }
        else if (l->x == WIDTH - 1)
        {
            l->x--;
            l->d = left;
        }
        else
        {
            l->x--;
            l->d = left;
            addlight(l->x + 1, l->y, right);
        }
        break;
    case '/':
        if (l->x == WIDTH - 1)
            removelight(i);
        else
        {
            l->x++;
            l->d = right;
        }
        break;
    case '\\':
        if (l->x == 0)
            removelight(i);
        else
        {
            l->x--;
            l->d = left;
        }
    }
}

void stepdown(light *l, int i)
{
    switch (GRID(l->x, l->y))
    {
    case '.':
        if (l->y == HEIGHT - 1)
            removelight(i);
        else
            l->y++;
        break;
    case '|':
        if (l->y == HEIGHT - 1)
            removelight(i);
        else
            l->y++;
        break;
    case '-':
        if (l->x == 0)
        {
            l->x++;
            l->d = right;
        }
        else if (l->x == WIDTH - 1)
        {
            l->x--;
            l->d = left;
        }
        else
        {
            l->x--;
            l->d = left;
            addlight(l->x + 1, l->y, right);
        }
        break;
    case '/':
        if (l->x == 0)
            removelight(i);
        else
        {
            l->x--;
            l->d = left;
        }
        break;
    case '\\':
        if (l->x == WIDTH - 1)
            removelight(i);
        else
        {
            l->x++;
            l->d = right;
        }
    }
}

void stepleft(light *l, int i)
{
    switch (GRID(l->x, l->y))
    {
    case '.':
        if (l->x == 0)
            removelight(i);
        else
            l->x--;
        break;
    case '|':
        if (l->y == 0)
        {
            l->y++;
            l->d = down;
        }
        else if (l->y == HEIGHT - 1)
        {
            l->y--;
            l->d = up;
        }
        else
        {
            l->y--;
            l->d = up;
            addlight(l->x, l->y + 1, down);
        }
        break;
    case '-':
        if (l->x == 0)
            removelight(i);
        else
            l->x--;
        break;
    case '/':
        if (l->y == HEIGHT - 1)
            removelight(i);
        else
        {
            l->y++;
            l->d = down;
        }
        break;
    case '\\':
        if (l->y == 0)
            removelight(i);
        else
        {
            l->y--;
            l->d = up;
        }
    }
}

void stepright(light *l, int i)
{
    switch (GRID(l->x, l->y))
    {
    case '.':
        if (l->x == WIDTH - 1)
            removelight(i);
        else
            l->x++;
        break;
    case '|':
        if (l->y == 0)
        {
            l->y++;
            l->d = down;
        }
        else if (l->y == HEIGHT - 1)
        {
            l->y--;
            l->d = up;
        }
        else
        {
            l->y--;
            l->d = up;
            addlight(l->x, l->y + 1, down);
        }
        break;
    case '-':
        if (l->x == WIDTH - 1)
            removelight(i);
        else
            l->x++;
        break;
    case '/':
        if (l->y == 0)
            removelight(i);
        else
        {
            l->y--;
            l->d = up;
        }
        break;
    case '\\':
        if (l->y == HEIGHT - 1)
            removelight(i);
        else
        {
            l->y++;
            l->d = down;
        }
    }
}

void steplights()
{
    for (int i = lightslen - 1; i >= 0; i--)
    {
        light *l = &lights[i];
        ENERGIZED(l->x, l->y)++;
        switch (l->d)
        {
        case up:
            stepup(l, i);
            break;
        case down:
            stepdown(l, i);
            break;
        case left:
            stepleft(l, i);
            break;
        case right:
            stepright(l, i);
            break;
        }
    }
}

long countenergized()
{
    long sum = 0;
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        if (energized[i])
            sum++;
    return sum;
}

void readgrid(FILE *file)
{
    char c;
    int i = 0;
    do
    {
        c = fgetc(file);
        if (c != '\n')
        {
            grid[i] = c;
            i++;
        }
        if (feof(file))
            break;
    } while (1);
}

long run()
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        energized[i] = 0;
    
    while (lightslen > 0)
        steplights();

    return countenergized();
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

    WIDTH = countwidth(filepath);
    HEIGHT = countlines(filepath);

    grid = malloc(WIDTH * HEIGHT * sizeof(char));
    readgrid(file);
    lights = malloc(WIDTH * HEIGHT * sizeof(light));
    energized = malloc(WIDTH * HEIGHT * sizeof(int));
    cache = malloc(WIDTH * HEIGHT * sizeof(light));

    for (int i = 0; i < WIDTH * HEIGHT; i++)
        energized[i] = 0;
    
    cachelen = 0;
    addlight(0, 0, right);
    printf("Sum of energized part 1: %ld\n", run());
    
    long sum = 0;
    for (int i = 0; i < WIDTH; i++)
    {
        cachelen = 0;
        addlight(i, 0, down);
        long res = run();
        if (res > sum)
            sum = res;
        lightslen = 0;
    }
    for (int i = 0; i < WIDTH; i++)
    {
        cachelen = 0;
        addlight(i, HEIGHT - 1, up);
        long res = run();
        if (res > sum)
            sum = res;
        lightslen = 0;
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        cachelen = 0;
        addlight(0, i, right);
        long res = run();
        if (res > sum)
            sum = res;
        lightslen = 0;
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        cachelen = 0;
        addlight(WIDTH - 1, i, left);
        long res = run();
        if (res > sum)
            sum = res;
        lightslen = 0;
    }

    printf("Sum energized: %ld\n", sum);

    fclose(file);
    free(grid);
    free(lights);
    free(energized);
    free(cache);
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
