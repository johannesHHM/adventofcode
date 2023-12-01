#include <stdio.h>
#include <stdlib.h>

void countstairs(char *filepath)
{
    FILE *file;
    char *line;
    size_t length = 0;

    int floor = 0;
    int basementposition = -1;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'", filepath);
        exit(1);
    }

    while (getline(&line, &length, file) != -1)
    {
        char *currentchar;
        currentchar = line;

        while (*currentchar != '\0')
        {
            if (basementposition == -1 && floor == -1)
                basementposition = currentchar - line;

            if (*currentchar == '(')
                floor++;
            else if (*currentchar == ')')
                floor--;

            if (basementposition == -1 && floor == -1)
                basementposition = currentchar - line + 1;

            currentchar++;
        }
    }

    fclose(file);
    if (line)
        free(line);

    printf("Floor count: %d\n", floor);
    printf("Basement position: %d\n", basementposition);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give input file as an argument.");
        exit(0);
    }
    countstairs(argv[1]);
}
