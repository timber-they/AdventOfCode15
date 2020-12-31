#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1000

int part1(FILE *in);
int part2(FILE *in);
void apply(char *line, int *field);

int main()
{
    FILE *in = fopen("in6", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int field[SIZE * SIZE] = {0};
    size_t n = 0;
    char *line = NULL;
    while (getline(&line, &n, in) > 0 && *line != '\n')
        apply(line, field);
    int count = 0;
    for (int i = 0; i < SIZE * SIZE; i++)
        count += field[i];
    free(line);
    return count;
}

void apply(char *line, int *field)
{
    int x1, y1, x2, y2;
    if (strstr(line, "toggle") != NULL)
    {
        if (!(sscanf(line, "toggle %d,%d through %d,%d\n", &x1, &y1, &x2, &y2)))
            exit(1);
        for (int y = y1; y <= y2; y++)
            for (int x = x1; x <= x2; x++)
                field[y * SIZE + x] = !field[y * SIZE + x];
    }
    else if (strstr(line, "on") != NULL)
    {
        if (!(sscanf(line, "turn on %d,%d through %d,%d\n", &x1, &y1, &x2, &y2)))
            exit(1);
        for (int y = y1; y <= y2; y++)
            for (int x = x1; x <= x2; x++)
                field[y * SIZE + x] = 1;
    }
    else if (strstr(line, "off") != NULL)
    {
        if (!(sscanf(line, "turn off %d,%d through %d,%d\n", &x1, &y1, &x2, &y2)))
            exit(1);
        for (int y = y1; y <= y2; y++)
            for (int x = x1; x <= x2; x++)
                field[y * SIZE + x] = 0;
    }
}

int part2(FILE *in)
{
    in = NULL;
    return -1;
}

