#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    int *nums;
    int length;
} row;

typedef struct
{
    row **rows;
    int buffsize;
    int len;
} rowlist;

long nthtriangle(int num)
{
    if (num == 0)
        return 0;
    return num + nthtriangle(num - 1);
}

int solverow(row row)
{
    row.nums = realloc(row.nums, nthtriangle(row.length * sizeof(int)));
    
    int previous = 0;
    for (int rc = 1; rc < row.length; rc++)
    {
        int start = previous + row.length - rc + 1;
        printf("start: %d\n", start);
        for (int i = 0; i < row.length - rc; i++)
        {
            int curr = start + i;
            int left = previous + i;
            int right = previous + i + 1;
            printf("%d = %d - %d\n", curr, right, left);
            row.nums[curr] = row.nums[right] - row.nums[left];
            printf("%d\n", row.nums[curr]);
        }
        previous = start;
        printf("\n");
    }
    return 0;
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
        // printf("found: %d, off: %d\n", res->nums[count], off);
        off += read;
        count++;
    }

    res->length = numcount;
    
    return res;
}

rowlist *createrowlist(int buffsize)
{
    rowlist *res;
    res = malloc(sizeof(rowlist));
    res->rows = malloc(buffsize * sizeof(row *));
    res->buffsize = buffsize;
    res->len = 0;
    return res;
}

void addrow(rowlist *list, row *element)
{
    if (list->len >= list->buffsize)
    {
        list->buffsize *= 2;
        list->rows = realloc(list->rows, list->buffsize * sizeof(row *));
    }
    list->rows[list->len] = element;
    list->len++;
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

    rowlist *list = createrowlist(256);

    while (getline(&line, &l, file) != -1) 
    {
        row *r = parseline(line);
        solverow(*r);
        addrow(list, r);
    }

    printf("%ld\n", nthtriangle(10));

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
