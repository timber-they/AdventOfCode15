#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Just a guess
#define MAX_LENGTH 1000000
#define ITERATIONS 40

int part1(FILE *in);
int part2(FILE *in);
int iterate(int *data);

int main()
{
    FILE *in = fopen("in10", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char line[MAX_LENGTH] = {0};
    int res[MAX_LENGTH] = {0};
    int resCount = 0;
    fgets(line, MAX_LENGTH, in);
    *strchr(line, '\n') = '\0';
    for (int i = 0; i < strlen(line); i++)
        res[i] = line[i] - '0';
    for (int i = 0; i < ITERATIONS; i++)
        resCount = iterate(res);
    return resCount;
}

int part2(FILE *in)
{
    in = NULL;
    return -1;
}

int iterate(int *data)
{
    int *buff = calloc(MAX_LENGTH, sizeof(*buff));
    int current = 0;
    int currentCount = 0;
    int buffI = 0;
    for (int i = 0; data[i] != 0; i++)
    {
        if (current != data[i])
        {
            if (currentCount != 0)
            {
                buff[buffI++] = currentCount;
                buff[buffI++] = current;
            }

            current = data[i];
            currentCount = 1;
        }
        else
            currentCount++;
    }
    if (currentCount != 0)
    {
        buff[buffI++] = currentCount;
        buff[buffI++] = current;
        //printf("-> %dx%d (%d)\n", currentCount, current, buffI);
    }
    memcpy(data, buff, MAX_LENGTH * sizeof(*buff));
    free(buff);
    return buffI;
}

