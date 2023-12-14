#include <stdio.h>
#include <stdlib.h>

#define GRID(x, y) (grid[(y)*WIDTH + (x)])

int WIDTH, HEIGHT;
char *grid;

void printgrid()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
            printf("%c", grid[y*WIDTH+x]);
        printf("\n");
    }
    printf("\n");
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
    for (int y = WIDTH - 1; y >= 0; y--)
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
    for (int y = 0; y < WIDTH; y++)
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
    for (int y = 0; y < WIDTH; y++)
        for (int x = WIDTH - 1; x >= 0; x--)
        {
            if (GRID(x, y) == 'O')
            {
                int tx = x;
                while (tx + 1 >= 0 && GRID(tx + 1, y) == '.')
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
    
    char c;
    int i = 0;
    do {
        c = fgetc(file);
        if (c != '\n')
        {
            grid[i] = c;
            i++;
        }
        if (feof(file))
            break;
    }while (1);
    
    for (int i = 0; i < 1000000000; i++)
    {
        sinknorth();
        sinkwest();
        sinksouth();
        sinkeast();
    }
     
    printf("sum: %ld\n", countload());

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
