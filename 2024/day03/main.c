#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DO   "do()"
#define DONT "don't()"
#define MUL  "mul(%d,%d%c"
#define len(s) sizeof(s) - 1

#define CHECK(s, i) if (strncmp(input, s, len(s)) == 0) { \
            enabled = i; \
            n = len(s); \
            goto END; \
        } \

static char part2 = 0;

int parse(char *input)
{
    int res = 0;
    int a, b, n;
    char end;
    char enabled = 1;
    while (1)
    {
        end = '\0';
        a = b = -1;
        if (part2)
        {
            CHECK(DO, 1)
            CHECK(DONT, 0)
        }
        n = sscanf(input, MUL, &a, &b, &end);
        if (a != -1 && b != -1 && end == ')' && enabled)
            res += a*b;
    END:
        if (n < 1)
            n = 1;
        input += n;
        if (*input == '\0')
            break;
    }
    return res;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give input file as an argument.");
        exit(0);
    }

    char * buffer = 0;
    long length;
    FILE * f = fopen (argv[1], "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, length, f);
        }
        fclose (f);
    }

    if (buffer)
    {
        int res1 = parse(buffer);
        part2 = 1;
        int res2 = parse(buffer);
        printf("Result part 1: %d\nResult part 2: %d\n", res1, res2);
    }
    else 
    {
        printf("Error reading input file '%s'", argv[1]);
        exit(1);
    }

    exit(0);
}
