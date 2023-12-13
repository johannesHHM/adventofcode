#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEBUFF 256
#define HINTSBUFF 64
#define HINTSCBUFF 64

#define UNFOLDDUP 5

typedef struct
{
    char *line;
    int linelen;
    int *hints;
    int hintslen;
} record;

static long cache[LINEBUFF][HINTSBUFF][HINTSCBUFF];

void resetcache()
{
    for (int x = 0; x < LINEBUFF; x++)
        for (int y = 0; y < HINTSBUFF; y++)
            for (int z = 0; z < HINTSCBUFF; z++)
                cache[x][y][z] = -1;
}

// Could not figure this one out myself, lots of inspiration from:
// https://github.com/fuglede/adventofcode/blob/master/2023/day12/solutions.py
long solve(char *line, int linelen, int *hints, int hintslen, int li, int hi, int hc)
{
    if (cache[li][hi][hc] != -1)
        return cache[li][hi][hc];

    if (li == linelen)
        return (long)hi == hintslen && hc == 0;

    long solved = 0;

    if (line[li] != '.')
        solved += solve(line, linelen, hints, hintslen, li + 1, hi, hc + 1);
    if (line[li] != '#')
    {
        if (hc)
        {
            if (hints[hi] == hc)
                solved += solve(line, linelen, hints, hintslen, li + 1, hi + 1, 0);
        }
        else
            solved += solve(line, linelen, hints, hintslen, li + 1, hi, 0);
    }
    cache[li][hi][hc] = solved;
    return solved;
}

record parseline(char *line)
{
    record res;
    res.line = malloc(LINEBUFF * sizeof(char));
    res.hints = malloc(HINTSBUFF * sizeof(int));
    res.linelen = 0;
    res.hintslen = 0;

    int bytesread;
    int origlinelen;
    int orighintslen;

    while (*line != ' ')
    {
        res.line[res.linelen] = *line;
        line++;
        res.linelen++;
    }
    origlinelen = res.linelen;
    while (sscanf(line, "%d%n", &res.hints[res.hintslen], &bytesread) == 1)
    {
        line += bytesread + 1;
        res.hintslen++;
    }
    orighintslen = res.hintslen;
    for (int i = 0; i < UNFOLDDUP - 1; i++)
    {
        res.line[res.linelen] = '?';
        res.linelen++;
        mempcpy(&res.line[res.linelen], res.line, origlinelen);
        res.linelen += origlinelen;

        mempcpy(&res.hints[res.hintslen], res.hints, orighintslen * sizeof(int));
        res.hintslen += orighintslen;
    }
    res.line[res.linelen] = '.';
    res.linelen++;
    return res;
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

    uint64_t sum = 0;
    while (getline(&line, &len, file) != -1)
    {
        resetcache();
        record r = parseline(line);
        uint64_t permutations = solve(r.line, r.linelen, r.hints, r.hintslen, 0, 0, 0);
        sum += permutations;
    }

    printf("Sum permutations: %" PRIu64 "\n", sum);

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
