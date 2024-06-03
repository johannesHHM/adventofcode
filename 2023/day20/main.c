#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART 2 // 2 for part 2

// lazy solution for part two, hardcoded in loops, found by running
// 'cat input | grep rx', then ran 'cat input | grep (module that points to rx)'
// all modules that point to that second module are listed here, by their id
// to calculate the final score calculate the least common multiple (LCM)
// of the printed numbers.
static int LOOPJUNCS[] = {26, 29, 31, 53};
static char FOUNDLOOP[] = {0, 0, 0, 0};

#define IDBUF 16
#define DESTNUMBUF 8
#define FROMNUMBUF 32
#define MODULESBUF 128

typedef enum
{
    LOW = 0,
    HIGH = 1,
} pulsestate;

typedef struct
{
    char type;
    char strid[IDBUF];
    int id;

    char strdests[DESTNUMBUF][IDBUF];
    int dests[DESTNUMBUF];
    int destslen;

    /* conjunction state */
    int fromids[FROMNUMBUF];
    pulsestate fromstate[FROMNUMBUF];
    int fromslen;

    /* flip-flop state */
    char flipstate;
} module;

typedef struct
{
    pulsestate strength;
    int from;
    int too;
} pulse;

module modules[MODULESBUF];
int moduleslen = 0;

static int pulsecounts[] = {0, 0};

pulse *pulselist = NULL;
int pulselistcap = 32;
int pulselistlen = 0;

void addpulse(pulse p)
{
    if (!pulselist)
        pulselist = (pulse *)malloc(pulselistcap * sizeof(pulse));
    if (pulselistlen >= pulselistcap)
    {
        pulselistcap *= 2;
        pulselist = (pulse *)realloc(pulselist, pulselistcap * sizeof(pulse));
    }
    pulselist[pulselistlen].strength = p.strength;
    pulselist[pulselistlen].from = p.from;
    pulselist[pulselistlen].too = p.too;
    pulsecounts[(int)p.strength]++;
    pulselistlen++;
}

void poppulse()
{
    if (pulselistlen <= 0)
        return;
    memmove(&pulselist[0], &pulselist[1], pulselistlen * sizeof(pulse));
    pulselistlen--;
}

void pressbutton()
{
    for (int i = 0; i < moduleslen; i++)
        if (modules[i].type == 'p')
            addpulse((pulse){LOW, modules[i].id, modules[i].dests[0]});

    static int loopsfound = 0;
    static int count = 1;
    while (pulselistlen > 0)
    {
        pulse *pul = pulselist;
        module *from = &modules[pul->from];
        module *too = &modules[pul->too];
        
        if (PART == 2)
            for (unsigned int i = 0; i < sizeof(LOOPJUNCS) / sizeof(int); i++)
                if (pul->from == LOOPJUNCS[i] && pul->strength == HIGH && !FOUNDLOOP[i])
                {
                    printf("Loop for (%s) at %d\n", modules[LOOPJUNCS[i]].strid, count);
                    FOUNDLOOP[i] = 1;
                    loopsfound++;
                    if (loopsfound >= 4)
                        exit(0);
                }

        switch (too->type)
        {
        case '%':
            if (pul->strength == HIGH)
                break;
            too->flipstate = too->flipstate == HIGH ? LOW : HIGH;

            for (int i = 0; i < too->destslen; i++)
                addpulse((pulse){too->flipstate, too->id, too->dests[i]});
            break;
        case '&':
            for (int i = 0; i < too->fromslen; i++)
                if (too->fromids[i] == from->id)
                    too->fromstate[i] = pul->strength;

            pulsestate tosend = LOW;
            for (int i = 0; i < too->fromslen; i++)
                if (too->fromstate[i] == LOW)
                {
                    tosend = HIGH;
                    break;
                }

            for (int i = 0; i < too->destslen; i++)
                addpulse((pulse){tosend, too->id, too->dests[i]});
            break;
        case 'b':
            for (int i = 0; i < too->destslen; i++)
                addpulse((pulse){pul->strength, too->id, too->dests[i]});
            break;
        }
        poppulse();
    }
    count++;
}

void printmodule(module *m)
{
    printf("%c:%s(%d) ->", m->type, m->strid, m->id);
    for (int i = 0; i < m->destslen; i++)
        printf(" %s(%d),", m->strdests[i], m->dests[i]);
    if (m->type == '&')
    {
        printf("\n  fromids: ");
        for (int i = 0; i < m->fromslen; i++)
            printf("%d ", m->fromids[i]);
    }
    printf("\n");
}

void printmodules()
{
    for (int i = 0; i < moduleslen; i++)
        printmodule(&modules[i]);
}

void addmodule(module m)
{
    m.id = moduleslen;
    modules[moduleslen] = m;
    moduleslen++;
    if (moduleslen > MODULESBUF)
    {
        printf("ERROR: too many modules!\n");
        exit(1);
    }
}

void initmodules()
{
    module b;
    b.type = 'p';
    strcpy(b.strid, "button");
    for (int i = 0; i < moduleslen; i++)
        if (modules[i].type == 'b')
            strcpy(b.strdests[0], modules[i].strid);
    b.destslen = 1;
    addmodule(b);

    for (int i = 0; i < moduleslen; i++)
    {
        module *m = &modules[i];
        m->flipstate = LOW;
        for (int y = 0; y < m->destslen; y++)
        {
            char found = 0;
            for (int j = 0; j < moduleslen; j++)
            {
                if (strcmp(modules[j].strid, m->strdests[y]) == 0)
                {
                    m->dests[y] = modules[j].id;
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                module out;
                out.type = 'o';
                strcpy(out.strid, out.strdests[y]);
                out.destslen = 0;
                addmodule(out);

                m->dests[y] = moduleslen - 1;
                continue;
            }

            module *cm = &modules[m->dests[y]];
            if (cm->type != '&')
                continue;
            cm->fromids[cm->fromslen] = m->id;
            cm->fromstate[cm->fromslen] = LOW;
            cm->fromslen++;
        }
    }
};

void parseline(char *line)
{
    module m;
    int read;
    sscanf(line, "%c%s ->%n", &m.type, m.strid, &read);
    if (m.type == 'b')
        strcpy(m.strid, "broadcast");
    m.destslen = 0;
    m.fromslen = 0;
    line += read + 1;

    int i = 0;
    while (*line != '\n')
    {
        if (*line == ',')
        {
            m.strdests[m.destslen][i] = '\0';
            m.destslen++;
            line += 2;
            i = 0;
            continue;
        }
        m.strdests[m.destslen][i++] = *line;
        line++;
    }
    m.strdests[m.destslen][i] = '\0';
    m.destslen++;
    addmodule(m);
}

void parsefile(char *filepath)
{
    FILE *file;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        exit(1);
    }

    char *line;
    size_t len;

    while (getline(&line, &len, file) != -1)
        parseline(line);

    initmodules();
    // printmodules();

    if (PART == 1)
    {
        for (int i = 0; i < 1000; i++)
            pressbutton();
        printf("Result: %d\n", pulsecounts[0] * pulsecounts[1]);
    }
    else if (PART == 2)
        while (1)
            pressbutton();

    fclose(file);
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
