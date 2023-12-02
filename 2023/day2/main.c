#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int colortoid(char c)
{
    switch (c)
    {
    case 'r':
        return 0;
    case 'g':
        return 1;
    case 'b':
        return 2;
    default:
        return -1;
    }
}

int colortolength(char c)
{
    switch (c)
    {
    case 'r':
        return 3;
    case 'g':
        return 5;
    case 'b':
        return 4;
    default:
        return -1;
    }
}

int parseline(char *line, int *limits, char powers)
{
    int linelength;
    int offset = 0;
    int bytesread;

    char color;
    int gameid, number, power;

    int minimum[3] = {0};
    int sums[3] = {0};

    linelength = strlen(line);

    if (sscanf(line, "Game %d:%n", &gameid, &bytesread) != 1)
        exit(0);

    offset += bytesread;

    while (linelength > offset)
    {
        sums[0] = 0;
        sums[1] = 0;
        sums[2] = 0;
        do
        {
            if (sscanf(line + offset, " %d %c%n", &number, &color, &bytesread) != 2)
                exit(0);

            offset += bytesread;
            sums[colortoid(color)] += number;
            offset += colortolength(color);

        } while (*(line + offset - 1) != ';' && *(line + offset - 1) != '\n');

        for (int i = 0; i < 3; i++)
            if (minimum[i] < sums[i])
                minimum[i] = sums[i];

        if ((sums[0] > limits[0] || sums[1] > limits[1] || sums[2] > limits[2]) && !powers)
            return 0;
    }
    power = minimum[0] * minimum[1] * minimum[2];

    if (!powers)
        return gameid;
    else
        return power;
}

int parsefile(char *filepath, char powers)
{
    FILE *file;
    char *line;
    size_t length = 0;

    int sum = 0;
    int limits[3] = {12, 13, 14};

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        return -1;
    }

    while (getline(&line, &length, file) != -1)
        sum += parseline(line, limits, powers);

    fclose(file);
    if (line)
        free(line);

    return sum;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give input file as an argument.");
        exit(0);
    }
    int result;

    result = parsefile(argv[1], 0);
    printf("Sum of IDs: %d\n", result);

    result = parsefile(argv[1], 1);
    printf("Sum of powers: %d\n", result);

    exit(0);
}
