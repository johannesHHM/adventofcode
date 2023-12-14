#include <stdio.h>
#include <stdlib.h>

#define GRIDSIZE 1000

int lights[GRIDSIZE][GRIDSIZE] = {0};

typedef enum
{
    turnoff = 0,
    turnon = 1,
    toggle
} command;

void updatelights(command c, int x0, int y0, int x1, int y1)
{
    for (int x = x0; x <= x1; x++)
        for (int y = y0; y <= y1; y++)
        {
            if (c == toggle)
                lights[x][y] = !lights[x][y];
            else
                lights[x][y] = (int)c;
        }
}

void runinstruction(char *line)
{
    int x0, y0, x1, y1;
    if (sscanf(line, "turn on %d,%d through %d,%d\n", &x0, &y0, &x1, &y1) == 4)
        updatelights(turnon, x0, y0, x1, y1);
    else if (sscanf(line, "turn off %d,%d through %d,%d\n", &x0, &y0, &x1, &y1) == 4)
        updatelights(turnoff, x0, y0, x1, y1);
    else if (sscanf(line, "toggle %d,%d through %d,%d\n", &x0, &y0, &x1, &y1) == 4)
        updatelights(toggle, x0, y0, x1, y1);
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

    char *line;
    size_t len;

    while (getline(&line, &len, file) != -1)
        runinstruction(line);

    long sum = 0;

    for (int x = 0; x < GRIDSIZE; x++)
        for (int y = 0; y < GRIDSIZE; y++)
            sum += lights[x][y];

    printf("Sum part 1: %ld\n", sum);

    fclose(file);
    free(line);
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
