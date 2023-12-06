#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RANGE_BUFF 256

typedef struct
{
    uint64_t start;
    uint64_t length;
    char updated;
} range;

typedef struct
{
    uint64_t destination;
    uint64_t source;
    uint64_t length;
    int groupid;
} transform;

static uint64_t RANGESBUFFSIZE = 128;
static uint64_t RANGESLENGTH = 0;
static range *ranges;

static uint64_t TRANSFORMSBUFFSIZE = 256;
static uint64_t TRANSFORMSLENGTH = 0;
static transform *transforms;

void addrange(range r)
{
    if (RANGESBUFFSIZE <= RANGESLENGTH)
        ranges = realloc(ranges, RANGESBUFFSIZE * 2 * sizeof(range));

    ranges[RANGESLENGTH] = r;
    RANGESLENGTH++;
}

void resetranges()
{
    RANGESLENGTH = 0;
}

void resettransforms()
{
    TRANSFORMSLENGTH = 0;
}

void addtransform(transform t)
{
    if (TRANSFORMSBUFFSIZE <= TRANSFORMSLENGTH)
        transforms = realloc(transforms, TRANSFORMSBUFFSIZE * 2 * sizeof(range));

    transforms[TRANSFORMSLENGTH] = t;
    TRANSFORMSLENGTH++;
}

void updaterange(range *r, const transform *t)
{
    if (r->updated == t->groupid)
        return;

    uint64_t rs = r->start;
    uint64_t re = r->start + r->length - 1;

    uint64_t ts = t->source;
    uint64_t te = t->source + t->length - 1;

    if ((rs < ts && re < ts) || (rs > te))
    {
        return;
    }
    else if (rs >= ts && re <= te)
    {
        r->start = t->destination + rs - ts;
        r->updated = t->groupid;
    }
    else if (rs < ts && re > te)
    {
        addrange((range){rs, ts - rs, 0});
        addrange((range){te + 1, re - te, 0});

        r->start = t->destination;
        r->length = t->length;
        r->updated = t->groupid;
    }
    else if (rs < ts && (re >= ts && re <= te))
    {
        addrange((range){rs, ts - rs, 0});

        r->start = t->destination;
        r->length = re - ts + 1;
        r->updated = t->groupid;
    }
    else if ((rs >= ts && rs <= te) && re > te)
    {
        addrange((range){te + 1, re - te, 0});

        r->start = t->destination + te - rs + 1;
        r->length = te - rs + 1;
        r->updated = t->groupid;
    }
    else
    {
        printf("[ERROR] range is an anomaly!\n");
    }
}

void parsetransforms(FILE *file)
{
    char *line;
    size_t l = 0;
    transform t;

    int groupid = 0;

    while (getline(&line, &l, file) != -1)
    {
        if (isalpha(*line))
            groupid++;
        if (!isdigit(*line))
            continue;
        sscanf(line, "%ld %ld %ld", &t.destination, &t.source, &t.length);
        t.groupid = groupid;

        addtransform(t);
    }
}

void parserangessecond(FILE *file)
{
    fscanf(file, "seeds:");

    range r;
    r.updated = 0;
    while (fscanf(file, " %" SCNu64 " %" SCNu64, &r.start, &r.length) == 2)
        addrange(r);
}

void parserangesfirst(FILE *file)
{
    fscanf(file, "seeds:");

    range r;
    r.updated = 0;
    while (fscanf(file, " %" SCNu64, &r.start) == 1)
    {
        r.length = 1;
        addrange(r);
    }
}

/* main run function */

void parsefile(char *filepath)
{
    FILE *file1, *file2;
    size_t l = 0;

    file1 = fopen(filepath, "r");
    if (!file1)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        exit(0);
    }

    parserangesfirst(file1);
    parsetransforms(file1);

    for (int y = 0; y < TRANSFORMSLENGTH; y++)
        for (int i = 0; i < RANGESLENGTH; i++)
            updaterange(&ranges[i], &transforms[y]);

    uint64_t lowest = UINT64_MAX;
    for (int i = 0; i < RANGESLENGTH; i++)
        if (ranges[i].start < lowest)
            lowest = ranges[i].start;

    printf("Lowest found first: %" SCNu64 "\n", lowest);

    resetranges();
    resettransforms();

    file2 = fopen(filepath, "r");

    parserangessecond(file2);
    parsetransforms(file2);

    for (int y = 0; y < TRANSFORMSLENGTH; y++)
        for (int i = 0; i < RANGESLENGTH; i++)
            updaterange(&ranges[i], &transforms[y]);

    lowest = UINT64_MAX;
    for (int i = 0; i < RANGESLENGTH; i++)
        if (ranges[i].start < lowest)
            lowest = ranges[i].start;

    printf("Lowest found second: %" SCNu64 "\n", lowest);

    fclose(file1);
    fclose(file2);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give input file as an argument.");
        exit(0);
    }

    ranges = malloc(RANGESBUFFSIZE * sizeof(range));
    transforms = malloc(TRANSFORMSBUFFSIZE * sizeof(transform));

    parsefile(argv[1]);

    free(ranges);
    free(transforms);

    exit(0);
}