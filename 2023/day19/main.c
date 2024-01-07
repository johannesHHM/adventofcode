#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARTLEN 4
#define IDBUF 8
#define RULESBUF 8
#define LINEBUF 256
#define PARTSBUF 512
#define WORKFLOWSBUF 1024

// clang-format off
static int cattable[] = {
    ['x'] = 0,
    ['m'] = 1,
    ['a'] = 2,
    ['s'] = 3
};
// clang-format on

typedef struct
{
    char type;
    char category;
    char dest[IDBUF];
    int limit;
} rule;

typedef struct
{
    char id[IDBUF];
    rule rules[RULESBUF];
    int ruleslen;
} work;

typedef struct
{
    int start;
    int end;
} range;

typedef struct
{
    range ratings[PARTLEN];
} partrange;

int parts[PARTSBUF][PARTLEN];
work workflows[WORKFLOWSBUF];

int partslen = 0;
int workflowslen = 0;

void splitrange(range *old, range *new, rule *r)
{
    if (r->type == '>')
    {
        if (old->end <= r->limit)
        {
            new->start = old->start;
            new->end = old->end;
            old->start = -1;
            old->end = -1;
        }
        else if (old->start > r->limit)
        {
            new->start = -1;
            new->end = -1;
        }
        else
        {
            new->start = old->start;
            new->end = r->limit;
            old->start = r->limit + 1;
        }
    }
    else if (r->type == '<')
    {
        if (old->start >= r->limit)
        {
            new->start = old->start;
            new->end = old->end;
            old->start = -1;
            old->end = -1;
        }
        else if (old->end < r->limit)
        {
            new->start = -1;
            new->end = -1;
        }
        else
        {
            new->start = r->limit;
            new->end = old->end;
            old->end = r->limit - 1;
        }
    }
}

long sumpartrange(partrange *pr)
{
    long sum = 1;
    for (int i = 0; i < PARTLEN; i++)
        sum *= pr->ratings[i].end - pr->ratings[i].start + 1;
    return sum;
}

work *getwork(char *id)
{
    for (int i = 0; i < workflowslen; i++)
        if (strcmp(workflows[i].id, id) == 0)
            return &workflows[i];
    return NULL;
}

void addrule(work *w, rule *r)
{
    w->rules[w->ruleslen].type = r->type;
    w->rules[w->ruleslen].category = r->category;
    w->rules[w->ruleslen].limit = r->limit;
    strcpy(w->rules[w->ruleslen].dest, r->dest);
    w->ruleslen++;
}

void parserules(work *w, char *s)
{
    rule r;
    int read;
    while (sscanf(s, "%c%c%d:%[^,],%n", &r.category, &r.type, &r.limit, r.dest, &read) == 4)
    {
        addrule(w, &r);
        s += read;
    }
    rule end;
    end.type = 'd';
    strcpy(end.dest, s);
    addrule(w, &end);
}

uint64_t calccomb(partrange start, char *workid, int i)
{
    if (workid[0] == 'A')
        return sumpartrange(&start);

    if (workid[0] == 'R')
        return 0;

    work *w = getwork(workid);
    range new;
    uint64_t sum = 0;
    rule *r = &w->rules[i];

    if (r->type == 'd')
        sum += calccomb(start, r->dest, 0);
    else
    {
        int category = cattable[(int)r->category];
        splitrange(&start.ratings[category], &new, r);

        if (start.ratings[category].start != -1)
            sum += calccomb(start, r->dest, 0);

        if (new.start != -1)
        {
            start.ratings[category] = new;
            sum += calccomb(start, workid, i + 1);
        }
    }
    return sum;
}

long runone()
{
    long sum = 0;
    for (int i = 0; i < partslen; i++)
    {
        int *p = parts[i];
        work *w;
        char id[IDBUF] = "in";
        int category;
    next:
        if (id[0] == 'A')
        {
            sum += p[0] + p[1] + p[2] + p[3];
            continue;
        }
        if (id[0] == 'R')
            continue;
        while (1)
        {
            w = getwork(id);
            for (int y = 0; y < w->ruleslen; y++)
            {
                category = cattable[(int)w->rules[y].category];
                if (w->rules[y].type == 'd')
                {
                    strcpy(id, w->rules[y].dest);
                    goto next;
                }
                if (w->rules[y].type == '<' && p[category] < w->rules[y].limit)
                {
                    strcpy(id, w->rules[y].dest);
                    goto next;
                }
                if (w->rules[y].type == '>' && p[category] > w->rules[y].limit)
                {
                    strcpy(id, w->rules[y].dest);
                    goto next;
                }
            }
        }
    }
    return sum;
}


uint64_t runtwo()
{
    partrange start;
    for (int i = 0; i < PARTLEN; i++)
        start.ratings[i] = (range){1, 4000};
    return calccomb(start, "in", 0);
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

    char workid[IDBUF];
    char workrules[LINEBUF];

    while (fscanf(file, "%[^{]{%[^}]}\n", workid, workrules) == 2)
    {
        work w;
        w.ruleslen = 0;
        strcpy(w.id, workid);
        parserules(&w, workrules);
        workflows[workflowslen] = w;
        workflowslen++;
    }

    while (fscanf(file, "{x=%d,m=%d,a=%d,s=%d}\n", &parts[partslen][0], &parts[partslen][1], &parts[partslen][2],
                  &parts[partslen][3]) == 4)
        partslen++;

    printf("Sum part one: %ld \n", runone());
    printf("Sum part two: %" PRId64 "\n", runtwo());

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
