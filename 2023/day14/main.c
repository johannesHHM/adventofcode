#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHEBUFF 256
#define GRID(x, y) (grid[(y)*WIDTH + (x)])

#define CYCLES 1000000000

int WIDTH, HEIGHT;
char *grid;

#define CACHE(i) (cache[(i)*WIDTH * HEIGHT])
int cachelen = 0;
int cachebuff = 256;
char *cache;

void cachegrid()
{
    if (cachelen == cachebuff)
    {
        cachebuff *= 2;
        cache = realloc(cache, cachebuff * WIDTH * HEIGHT * sizeof(char));
    }
    memcpy(&CACHE(cachelen), grid, WIDTH * HEIGHT * sizeof(char));
    cachelen++;
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

void sinknorth()
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            if (GRID(x, y) == 'O')
            {
                int ty = y;
                while (ty - 1 >= 0 && GRID(x, ty - 1) == '.')
                    ty--;
                GRID(x, y) = '.';
                GRID(x, ty) = 'O';
            }
        }
}

void sinksouth()
{
    for (int y = HEIGHT - 1; y >= 0; y--)
        for (int x = 0; x < WIDTH; x++)
        {
            if (GRID(x, y) == 'O')
            {
                int ty = y;
                while (ty + 1 < HEIGHT && GRID(x, ty + 1) == '.')
                    ty++;
                GRID(x, y) = '.';
                GRID(x, ty) = 'O';
            }
        }
}

void sinkwest()
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            if (GRID(x, y) == 'O')
            {
                int tx = x;
                while (tx - 1 >= 0 && GRID(tx - 1, y) == '.')
                    tx--;
                GRID(x, y) = '.';
                GRID(tx, y) = 'O';
            }
        }
}

void sinkeast()
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = WIDTH - 1; x >= 0; x--)
        {
            if (GRID(x, y) == 'O')
            {
                int tx = x;
                while (tx + 1 < WIDTH && GRID(tx + 1, y) == '.')
                    tx++;
                GRID(x, y) = '.';
                GRID(tx, y) = 'O';
            }
        }
}

long countload()
{
    long sum = 0;
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            if (GRID(x, y) == 'O')
                sum += HEIGHT - y;
        }
    return sum;
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
    cache = malloc(CACHEBUFF * WIDTH * HEIGHT * sizeof(char));

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

    for (int i = 0; i < CYCLES; i++)
    {
        sinknorth();

        if (i == 0)
            printf("Sum of load part 1: %ld\n", countload());

        sinkwest();
        sinksouth();
        sinkeast();

        for (int j = 0; j < cachelen; j++)
            if (memcmp(grid, &CACHE(j), WIDTH * HEIGHT) == 0)
                i = CYCLES - ((CYCLES - i) % (i - j));

        cachegrid();
    }
    
    printf("Sum of load part 2: %ld\n", countload());
    
    fclose(file);
    free(grid);
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
