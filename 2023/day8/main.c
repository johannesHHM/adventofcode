#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 3

typedef struct node node;
typedef struct graph graph;

struct node
{
    char data[NAME_LENGTH];
    node *left;
    node *right;
};

struct graph
{
    node **nodes;
    char *instructions;
    int numstart;
    int length;
};

long gcd(long a, long b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

long lcm(long a, long b)
{
    return (a / gcd(a, b)) * b;
}

long lcmlist(int *list, int listlength)
{
    long last = list[0];
    for (int i = 1; i < listlength; i++)
        last = lcm(last, list[i]);
    return last;
}

node *createnode(const char *data)
{
    node *res = malloc(sizeof(node));
    if (res)
    {
        memcpy(res->data, data, NAME_LENGTH * sizeof(char));
        res->left = NULL;
        res->right = NULL;
    }
    return res;
}

struct node *findnode(graph *graph, const char *key)
{
    for (int i = 0; i < graph->length; i++)
        if (strncmp(graph->nodes[i]->data, key, NAME_LENGTH) == 0)
            return graph->nodes[i];
    return NULL;
}

void addedge(graph *graph, const char *sourcedata, const char *leftdata, const char *rightdata)
{
    node *source = findnode(graph, sourcedata);
    node *left = findnode(graph, leftdata);
    node *right = findnode(graph, rightdata);
    if (source != NULL && left != NULL && right != NULL)
    {
        source->left = left;
        source->right = right;
    }
    else
        printf("[ERROR]: One or more nodes not found\n");
}

graph *creategraph(FILE *fp)
{
    int bufsize = 256;
    graph *g = malloc(sizeof(graph));
    g->nodes = malloc(bufsize * sizeof(node *));
    g->length = 0;
    g->numstart = 0;

    char *line;
    size_t length = 0;
    getline(&g->instructions, &length, fp);
    getline(&line, &length, fp);
    while (getline(&line, &length, fp) != -1)
    {
        if (bufsize <= g->length)
        {
            bufsize *= 2;
            g->nodes = realloc(g->nodes, bufsize * sizeof(node *));
        }
        g->nodes[g->length] = createnode(line);
        if (g->nodes[g->length]->data[2] == 'A')
            g->numstart++;
        g->length++;
    }
    return g;
}

void fillgraph(FILE *fp, graph *g)
{
    char *line;
    size_t length = 0;
    getline(&line, &length, fp);
    getline(&line, &length, fp);
    char source[4], left[4], right[4];
    while (fscanf(fp, "%3s = (%3s, %3s)\n", source, left, right) == 3)
        addedge(g, source, left, right);
}

long walkgraphfirst(graph *g, char *start, char *stop)
{
    long count = 0;
    node *currentnode = findnode(g, start);
    if (!currentnode)
    {
        printf("[ERROR] could not find start node '%3s'\n", start);
        return -1;
    }
    char *instruction = g->instructions;
    while (strncmp(currentnode->data, stop, NAME_LENGTH) != 0)
    {
        if (*instruction == 'L')
            currentnode = currentnode->left;
        else if (*instruction == 'R')
            currentnode = currentnode->right;
        
        instruction++;
        if (*instruction == '\n')
            instruction = g->instructions;
        count++;
    }
    return count;
}

long walkgraphsecond(graph *g, char start, char stop)
{
    node *currentnode;
    int *firstfound = malloc(g->numstart * sizeof(int));
    int startcount = 0;
    for (int i = 0; i < g->length; i++)
    {
        if (g->nodes[i]->data[NAME_LENGTH - 1] == start)
        {
            currentnode = g->nodes[i];
            char *instruction = g->instructions;

            int count = 0;
            while (1)
            {
                if (currentnode->data[NAME_LENGTH - 1] == stop)
                {
                    firstfound[startcount] = count;
                    break;
                }
                if (*instruction == 'L')
                    currentnode = currentnode->left;
                else if (*instruction == 'R')
                    currentnode = currentnode->right;

                instruction++;
                if (*instruction == '\n')
                    instruction = g->instructions;
                count++;
            }
            startcount++;
        }
        if (startcount == g->numstart)
            break;
    }

    return lcmlist(firstfound, g->numstart); 
}

void parsefile(char *filepath)
{
    FILE *file1, *file2;

    file1 = fopen(filepath, "r");
    file2 = fopen(filepath, "r");
    if (!file1)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        exit(0);
    }

    graph *graph = creategraph(file1);
    fillgraph(file2, graph);

    long sum = walkgraphfirst(graph, "AAA", "ZZZ");
    printf("Sum first part: %ld\n", sum);
    sum = walkgraphsecond(graph, 'A', 'Z');
    printf("Sum second part: %ld\n", sum);

    fclose(file1);
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
