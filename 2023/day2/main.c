#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static char *colors[] = {"red", "green", "blue"};


int parsefile(char *filepath)
{
    FILE *file;
    char *line;
    size_t length = 0;

    int sum = 0;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        return -1;
    }

    while (getline(&line, &length, file) != -1)
        printf("%s", line);

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
    result = parsefile(argv[1]);
    printf("Result: %d", result);
    exit(0);
}
