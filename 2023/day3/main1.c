#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int LINELENGTH;

char searchline(char *line, int from, int too)
{
    for (int i = from; i < too && i < LINELENGTH - 1; i++)
    {
        if (i < 0)
            continue;
        if (!isdigit(*(line + i)) && *(line + i) != '.')
            return 1;
    }
    return 0;
}

char scanforpart(char *last, char *line, char *next, int start, int numblen)
{
    char haspart = 0;
    if (last)
        haspart += searchline(last, start - 1, start + numblen + 1);

    haspart += searchline(line, start - 1, start + numblen + 1);

    if (next)
        haspart += searchline(next, start - 1, start + numblen + 1);

    return haspart;
}

int parseline(char *last, char *line, char *next)
{
    int localsum = 0;
    int offset = 0;
    int bytesread;
    int number = 0;
    char haspart = 0;

    while (offset < LINELENGTH)
    {
        if (isdigit(*(line + offset)))
        {
            sscanf(line + offset, "%d%n", &number, &bytesread);
            haspart = scanforpart(last, line, next, offset, bytesread);
            offset += bytesread - 1;
            if (haspart)
                localsum += number;
        }
        haspart = 0;
        offset++;
    }
    return localsum;
}

void parsefile(char *filepath)
{
    FILE *file;
    size_t length = 0;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        exit(0);
    }

    char *last, *line, *next;
    int sum = 0;

    last = NULL;
    getline(&line, &length, file);
    getline(&next, &length, file);

    LINELENGTH = strlen(line);

    do
    {
        sum += parseline(last, line, next);
        if (!next)
            break;
        if (!last)
            last = malloc(LINELENGTH * sizeof(char));
        last = strcpy(last, line);
        line = strcpy(line, next);
        if (getline(&next, &length, file) == -1)
        {
            free(next);
            next = NULL;
        }
    } while (1);

    printf("Sum of part numbers: %d\n", sum);

    fclose(file);
    free(last);
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
