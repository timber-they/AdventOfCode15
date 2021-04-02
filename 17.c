#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CONTAINER_COUNT 20
#define MAX_LINE_LENGTH 2
#define EGGNOG 150

int part1(FILE *in);
int part2(FILE *in);
int *getContainers(FILE *file, int *buff);
int countCombinations(int *containers, int count, int leftEggnog);
int getMinimumCombination(int *containers, int count, int leftEggnog);
int min(int a, int b);
int countCombinationsInContainers(int *containers, int count, int leftEggnog, int containerCount);

int main()
{
    FILE *in = fopen("in17", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int containers[CONTAINER_COUNT];
    getContainers(in, containers);
    return countCombinations(containers, CONTAINER_COUNT, EGGNOG);
}

int part2(FILE *in)
{
    int containers[CONTAINER_COUNT];
    getContainers(in, containers);
    int minimumCombination = getMinimumCombination(containers, CONTAINER_COUNT, EGGNOG);
    return countCombinationsInContainers(containers, CONTAINER_COUNT, EGGNOG, minimumCombination);
}

int *getContainers(FILE *file, int *buff)
{
    char line[MAX_LINE_LENGTH + 2];
    for (int i = 0; i < CONTAINER_COUNT; i++)
    {
        fgets(line, MAX_LINE_LENGTH + 2, file);
        buff[i] = atoi(line);
    }
    return buff;
}

int countCombinations(int *containers, int count, int leftEggnog)
{
    if (leftEggnog == 0)
        return 1;
    if (leftEggnog < 0)
        return 0;
    if (count <= 0)
        return 0;
    return countCombinations(containers+1, count-1, leftEggnog)
        + countCombinations(containers+1, count-1, leftEggnog - containers[0]);
}

int getMinimumCombination(int *containers, int count, int leftEggnog)
{
    if (leftEggnog == 0)
        return 0;
    if (leftEggnog < 0)
        return -100000;
    if (count <= 0)
        return -100000;
    return min(getMinimumCombination(containers+1, count-1, leftEggnog),
            getMinimumCombination(containers+1, count-1, leftEggnog - containers[0]) + 1);
}

int min(int a, int b)
{
    if (a < 0)
        return b;
    if (b < 0)
        return a;
    return a < b ? a : b;
}

int countCombinationsInContainers(int *containers, int count, int leftEggnog, int containerCount)
{
    if (containerCount < 0)
        return 0;
    if (leftEggnog == 0)
        return 1;
    if (leftEggnog < 0)
        return 0;
    if (count <= 0)
        return 0;
    if (containerCount == 0)
        return 0;
    return countCombinationsInContainers(containers+1, count-1, leftEggnog, containerCount)
        + countCombinationsInContainers(containers+1, count-1, leftEggnog - containers[0], containerCount-1);
}

