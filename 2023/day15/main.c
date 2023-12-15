#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOXLEN 256
#define LABELBUFF 32
#define LENSLEN 32

typedef struct
{
    char lenses[LENSLEN][LABELBUFF];
    int focals[LENSLEN];
    int lenslen;
} box;

box *boxes;

void initboxes()
{
    boxes = malloc(BOXLEN * sizeof(box));
    for (int i = 0; i < BOXLEN; i++)
    {
        boxes[i].lenslen = 0;
    }
}

void setlens(box *box, char *label, int focal)
{
    for (int i = 0; i < box->lenslen; i++)
        if (strcmp(box->lenses[i], label) == 0)
        {
            box->focals[i] = focal;
            return;
        }
    strcpy(box->lenses[box->lenslen], label);
    box->focals[box->lenslen] = focal;
    box->lenslen++;
}

void removelens(box *box, char *label)
{
    for (int i = 0; i < box->lenslen; i++)
        if (strcmp(box->lenses[i], label) == 0)
        {
            memmove(box->lenses[i], box->lenses[i + 1], (box->lenslen - i - 1) * LABELBUFF * sizeof(char));
            memmove(&box->focals[i], &box->focals[i + 1], (box->lenslen - i - 1) * sizeof(int));
            box->lenslen--;
            return;
        }
}

int hash(char *line)
{
    int value = 0;
    while (*line != '\0')
    {
        if (*line != '\n')
            value = (value + *line) * 17 % 256;
        line++;
    }
    return value;
}

void parsecommand(char *line)
{
    char label[LABELBUFF];
    int i = 0;
    while (line[i] != '=' && line[i] != '-')
    {
        label[i] = line[i];
        i++;
    }
    label[i] = '\0';
    if (line[i] == '=')
        setlens(&boxes[hash(label)], label, atoi(&line[i + 1]));
    else
        removelens(&boxes[hash(label)], label);
}

long sumfocuspower()
{
    long sum = 0;
    for (int i = 0; i < BOXLEN; i++)
        for (int j = 0; j < boxes[i].lenslen; j++)
            sum += (i + 1) * (j + 1) * boxes[i].focals[j];
    return sum;
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

    initboxes();
    char instruction[LABELBUFF];
    long sum = 0;

    while (fscanf(file, "%[^,],", instruction) == 1)
    {
        parsecommand(instruction);
        sum += (long)hash(instruction);
    }

    printf("Sum part 1: %ld\n", sum);
    printf("Sum part 2: %ld", sumfocuspower());

    fclose(file);
    free(boxes);
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
