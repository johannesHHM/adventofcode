#include <stdio.h>
#include <stdlib.h>

void calculatewrapping(char *filepath)
{
    FILE *file;
    size_t length = 0;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'", filepath);
        exit(1);
    }

    int sides[3];
    int slack;
    int ribbon;
    int sumWrapping = 0;
    int sumRibbon = 0;

    while (fscanf(file, "%dx%dx%d\n", &sides[0], &sides[1], &sides[2]) == 3)
    {
        slack = sides[0] * sides[1];
        ribbon = 2 * sides[0] + 2 * sides[1];

        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
            {
                if (x != y && slack > sides[x] * sides[y])
                    slack = sides[x] * sides[y];
                if (x != y && ribbon > 2 * sides[x] + 2 * sides[y])
                    ribbon = 2 * sides[x] + 2 * sides[y];
            }

        sumWrapping += 2 * sides[0] * sides[1] + 2 * sides[1] * sides[2] + 2 * sides[2] * sides[0] + slack;
        sumRibbon += sides[0] * sides[1] * sides[2] + ribbon;
    }

    fclose(file);

    printf("Square feet wrapping paper: %d\n", sumWrapping);
    printf("Feet or ribbon: %d\n", sumRibbon);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give input file as an argument.");
        exit(0);
    }
    calculatewrapping(argv[1]);
}
