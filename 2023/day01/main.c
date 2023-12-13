#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *digitwords[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int combinedigits(char left, char right)
{
    return (10 * (left - '0')) + right - '0';
}

int digitword(char *line, char *digit)
{
    while (*line != '\0')
    {
        if (*line != *digit)
            return 0;

        line++;
        digit++;

        if (*digit == '\0')
            return 1;
    }
    return 0;
}

char getdigitword(char *line)
{
    for (int i = 0; i < 9; i++)
        if (digitword(line, digitwords[i]))
            return i + '0' + 1;
    return 'n';
}

char findfirstdigit(char *line, int direction)
{
    char digit;
    while (*line != '\0')
    {
        if (isdigit(*line))
            digit = *line;
        else
            digit = getdigitword(line);

        if (digit != 'n')
            return digit;

        line += direction;
    }
    return 'n';
}

int parseline(char *line)
{
    char left, right;
    left = findfirstdigit(line, 1);
    right = findfirstdigit(line + strlen(line) - 2, -1);

    return combinedigits(left, right);
}

int parsefile(char *filepath)
{
    FILE *file;
    char *line;
    size_t length = 0;

    int sum = 0;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'", filepath);
        return -1;
    }

    while (getline(&line, &length, file) != -1)
        sum += parseline(line);

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
