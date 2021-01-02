#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCATION_CNT 8
#define MAX_LOCATION_LENGTH 14

int part1(FILE *in);
int part2(FILE *in);
int *getDistances(FILE *in);
int getId(char *location);
int getMinDistance(int start, int *visitted, int *distances);
int getMaxDistance(int start, int *visitted, int *distances);

int main()
{
    FILE *in = fopen("in9", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int *distances = getDistances(in);
    int visitted[LOCATION_CNT] = {0};
    int minDistance = 0;
    for (int i = 0; i < LOCATION_CNT; i++)
    {
        int distance = getMinDistance(i, visitted, distances);
        if (distance < minDistance || minDistance == 0)
            minDistance = distance;
    }

    free(distances);
    return minDistance;
}

int part2(FILE *in)
{
    int *distances = getDistances(in);
    int visitted[LOCATION_CNT] = {0};
    int maxDistance = 0;
    for (int i = 0; i < LOCATION_CNT; i++)
    {
        int distance = getMaxDistance(i, visitted, distances);
        if (distance > maxDistance || maxDistance == 0)
            maxDistance = distance;
    }

    free(distances);
    return maxDistance;
}

int getMinDistance(int start, int *visitted, int *distances)
{
    visitted[start] = 1;
    int minDistance = 0;
    for (int i = 0; i < LOCATION_CNT; i++)
    {
        if (visitted[i])
            continue;
        int distance = distances[start * LOCATION_CNT + i];
        distance += getMinDistance(i, visitted, distances);
        if (distance < minDistance || minDistance == 0)
            minDistance = distance;
    }
    visitted[start] = 0;
    return minDistance;
}

int getMaxDistance(int start, int *visitted, int *distances)
{
    visitted[start] = 1;
    int maxDistance = 0;
    for (int i = 0; i < LOCATION_CNT; i++)
    {
        if (visitted[i])
            continue;
        int distance = distances[start * LOCATION_CNT + i];
        distance += getMaxDistance(i, visitted, distances);
        if (distance > maxDistance || maxDistance == 0)
            maxDistance = distance;
    }
    visitted[start] = 0;
    return maxDistance;
}

int *getDistances(FILE *in)
{
    size_t n = 0;
    char *line = NULL;
    int *res = malloc(LOCATION_CNT * LOCATION_CNT * sizeof(*res));
    while (getline(&line, &n, in) > 0 && *line != '\n')
    {
        char from[MAX_LOCATION_LENGTH],
             to[MAX_LOCATION_LENGTH];
        int distance = -1;
        if (sscanf(line, "%s to %s = %d\n", from, to, &distance) != 3)
        {
            fprintf(stderr, "Invalid line: '%s'\n", line);
            exit(2);
        }

        res[getId(from) * LOCATION_CNT + getId(to)] = distance;
        res[getId(to) * LOCATION_CNT + getId(from)] = distance;
    }

    free(line);
    return res;
}

int getId(char *location)
{
    if (!strcmp(location, "AlphaCentauri"))
        return 0;
    if (!strcmp(location, "Snowdin"))
        return 1;
    if (!strcmp(location, "Tambi"))
        return 2;
    if (!strcmp(location, "Faerun"))
        return 3;
    if (!strcmp(location, "Norrath"))
        return 4;
    if (!strcmp(location, "Straylight"))
        return 5;
    if (!strcmp(location, "Tristram"))
        return 6;
    if (!strcmp(location, "Arbre"))
        return 7;
    fprintf(stderr, "Unknown location: '%s'\n", location);
    exit(1);
}

