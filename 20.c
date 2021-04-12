#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define INPUT_SIZE 9

int part1(FILE *in);
int part2(FILE *in);
int getPresents(int i);
int getNewPresents(int i);

int main()
{
    FILE *in = fopen("in20", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char line[INPUT_SIZE+1];
    int dest = atoi(fgets(line, INPUT_SIZE, in)) / 10;
    for (int i = 2; i < dest; i++)
    {
        int presents = getPresents(i);
        if (presents > dest)
            return i;
    }
    return -1;
}

int part2(FILE *in)
{
    char line[INPUT_SIZE+1];
    int dest = atoi(fgets(line, INPUT_SIZE, in));
    for (int i = 2; i < dest; i++)
    {
        int presents = getNewPresents(i);
        if (presents > dest)
            return i;
    }
    return -1;
}

int getPresents(int i)
{
    int res = 1 + i;
    for (int j = 2; j <= sqrt(i); j++)
        if (i % j == 0)
            res += (j == i / j) ? j : j + i / j;
    return res;
}

int getNewPresents(int i)
{
    int res = 0;
    for (int j = 1; j <= sqrt(i); j++)
        if (i % j == 0)
        {
            if (i / j <= 50)
                res += 11 * j;
            if (i / j != j && j <= 50)
                res += 11 * i / j;
        }
    return res;
}

