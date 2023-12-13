#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *nums;
    int length;
} row;

long nthtriangle(int num)
{
    if (num == 0)
        return 0;
    return num + nthtriangle(num - 1);
}

int solvepyramid(int *pyramid, int rowlen, int depth, char way)
{
    int lasts[depth], results[depth], start;
    if (way)
        start = -1;
    else
        start = -rowlen - 1;

    for (int i = 0; i < depth; i++)
    {
        if (way)
            start += rowlen - i;
        else
            start += rowlen - i + 1;
        lasts[i] = pyramid[start];
    }
    
    results[depth - 1] = 0;
    if (way)
        for (int i = depth - 2; i >= 0; i--)
            results[i] = lasts[i] + results[i+1];
    else
        for (int i = depth - 2; i >= 0; i--)
            results[i] = lasts[i] - results[i+1];
    
    return results[0];
}

int makepyramid(row *row)
{
    row->nums = realloc(row->nums, nthtriangle(row->length) * sizeof(int));
    int depth = 1; 
    int previous = 0;
    for (int rc = 1; rc < row->length; rc++)
    {
        depth++;
        int start = previous + row->length - rc + 1;
        char allnil = 1;
        for (int i = 0; i < row->length - rc; i++)
        {
            int curr = start + i;
            int left = previous + i;
            int right = previous + i + 1;
            row->nums[curr] = row->nums[right] - row->nums[left];
            if (row->nums[curr] != 0)
                allnil = 0;
        }
        if (allnil)
            return depth;
        previous = start;
    }
    return row->length;
}

row *parseline(char *line)
{
    int numcount = 1;
    for (int o = 0; *(line + o) != '\n'; o++)
         if (*(line + o) == ' ')
            numcount++;
    
    row *res;
    res = malloc(sizeof(row));
    res->nums = malloc(numcount * sizeof(int));
    int off = 0;
    int read = 0;
    int count = 0;
    while (sscanf(line + off, "%d %n", &res->nums[count], &read) == 1)
    {
        off += read;
        count++;
    }

    res->length = numcount;
    
    return res;
}

void parsefile(char *filepath)
{
    FILE *file;
    char *line;
    size_t l = 0;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        exit(0);
    }

    long sumf = 0;
    long sums = 0;
    while (getline(&line, &l, file) != -1) 
    {
        row *r = parseline(line);
        int depth;
        depth = makepyramid(r);
        sumf += solvepyramid(r->nums, r->length, depth, 1);
        sums += solvepyramid(r->nums, r->length, depth, 0);
        free(r->nums);
        free(r);
    }

    printf("sum first: %ld\n", sumf);
    printf("sum second: %ld\n", sums);

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
