#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int LINELENGTH;

void searchline(char *line, int from, int too, int *numbers)
{
    for (int i = from; i < too + 1 && i < LINELENGTH - 1; i++)
    {
        if (i < 0)
            continue;

        if (isdigit(*(line + i)))
        {
            int number, numblength;
            int offset = i;

            while (offset - 1 >= 0 && isdigit(*(line + offset - 1)))
                offset--;

            sscanf(line + offset, "%d%n", &number, &numblength);

            for (int n = 0; n < 3; n++)
            {
                if (numbers[n] == 0)
                {
                    numbers[n] = number;
                    break;
                }
            }
            i = offset + numblength - 1;
        }
    }
}

int scanfornumbers(char *last, char *line, char *next, int start)
{
    int numbers[3] = {0};

    if (last)
        searchline(last, start - 1, start + 1, numbers);

    searchline(line, start - 1, start + 1, numbers);

    if (next)
        searchline(next, start - 1, start + 1, numbers);

    if (numbers[2] == 0)
        return numbers[0] * numbers[1];
    else
        return 0;
}

int parseline(char *last, char *line, char *next)
{
    int localsum = 0;
    int offset = 0;

    while (offset < LINELENGTH)
    {
        if (*(line + offset) == '*')
            localsum += scanfornumbers(last, line, next, offset);
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

    printf("Sum of gear ratios: %d\n", sum);

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
