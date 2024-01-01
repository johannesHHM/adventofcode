#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define EDGESBUF 1024
#define LINEBUF 256

// 0 for part 1, 1 for part 2
#define PART 1

typedef struct
{
    long x, y;
} point;

point edges[EDGESBUF];
int edgeslen = 0;

// clang-format off
static int movetable[][2] = {
    ['U'] = {0, -1},
    ['D'] = {0, 1},
    ['L'] = {-1, 0},
    ['R'] = {1, 0}
};

static char dirtable[] = {
    [0] = 'R',
    [1] = 'D',
    [2] = 'L',
    [3] = 'U'
};
// clang-format on

void movepos(int *x, int *y, char dir, int len)
{
    *x += movetable[(int)dir][0] * len;
    *y += movetable[(int)dir][1] * len;
}

int64_t gaussarea(int64_t vertlen)
{
    int64_t area = 0;
    for (int i = 0; i < edgeslen - 1; i++)
        area += (edges[i].x * edges[i + 1].y - edges[i + 1].x * edges[i].y);

    area += (edges[edgeslen - 1].x * edges[0].y - edges[0].x * edges[edgeslen - 1].y);

    if (area < 0)
        area *= -1;

    area += vertlen;
    return area * 0.5;
}

void runinstruction(int *x, int *y, char dir, int len, char *hex, int64_t *vertlen, char parttwo)
{
    if (parttwo)
    {
        dir = dirtable[hex[5] - 48];
        hex[5] = '\0';
        len = strtol(hex, NULL, 16);
    }
    *vertlen += len;
    movepos(x, y, dir, len);
    edges[edgeslen] = (point){*x, *y};
    edgeslen++;
}

void parsefile(char *filepath)
{
    FILE *file;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%7s'\n", filepath);
        exit(0);
    }

    int x = 0;
    int y = 0;
    int64_t vertlen = 2; // IDK why 2 is needed
    char direction;
    int length;
    char hexstring[7];

    while (fscanf(file, "%c %d (#%6s)\n", &direction, &length, hexstring) == 3)
        runinstruction(&x, &y, direction, length, hexstring, &vertlen, PART);

    printf("Lava pool size: %ld m²\n", gaussarea(vertlen));
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERROR: no input file");
        exit(0);
    }
    parsefile(argv[1]);
    exit(0);
}
