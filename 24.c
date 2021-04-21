#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>

typedef struct Information
{
    int packages;
    long entanglement;
} Information;

#define PACKAGE_COUNT 29

long part1(FILE *in);
int part2(FILE *in);
int *getPackages(FILE *in, int *buff);
int getGroupSize(int *packages);
Information getMinimumEntanglement(int *packages, int leftPackages,
        int left1, int left2, int left3, int groupSize, Information *mem, int leftPackages1);
Information getMinimum(Information a, Information b);
int getGroupSize2(int *packages);
Information getMinimumEntanglement2(int *packages, int leftPackages,
        int left1, int left2, int left3, int left4, int groupSize, Information *mem, int leftPackages1);

int main()
{
    FILE *in = fopen("in24", "r");

    printf("Part1: %ld\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

long part1(FILE *in)
{
    int packages[PACKAGE_COUNT];
    getPackages(in, packages);
    int groupSize = getGroupSize(packages);
    Information *mem = calloc((groupSize+1) * (groupSize+1) * (groupSize+1), sizeof(*mem));
    long res = getMinimumEntanglement(packages, PACKAGE_COUNT, groupSize,
            groupSize, groupSize, groupSize, mem, PACKAGE_COUNT).entanglement;
    free(mem);
    return res;
}

int part2(FILE *in)
{
    int packages[PACKAGE_COUNT];
    getPackages(in, packages);
    int groupSize = getGroupSize2(packages);
    Information *mem = calloc((groupSize+1) * (groupSize+1) * (groupSize+1), sizeof(*mem));
    long res = getMinimumEntanglement2(packages, PACKAGE_COUNT, groupSize,
            groupSize, groupSize, groupSize, groupSize, mem, PACKAGE_COUNT).entanglement;
    free(mem);
    return res;
}

int *getPackages(FILE *in, int *buff)
{
    size_t n = 0;
    char *line = NULL;
    for (int i = 0; i < PACKAGE_COUNT; i++)
    {
        if (getline(&line, &n, in) == 0)
            fprintf(stderr, "Unexpected empty line\n");
        buff[i] = atoi(line);
    }
    free(line);
    return buff;
}

int getGroupSize(int *packages)
{
    int res = 0;
    for (int i = 0; i < PACKAGE_COUNT; i++)
        res += packages[i];
    return res / 3;
}

int getGroupSize2(int *packages)
{
    int res = 0;
    for (int i = 0; i < PACKAGE_COUNT; i++)
        res += packages[i];
    return res / 4;
}

#define AT(l1,l2,l3,gs) ((l1) + (l2) * ((gs)+1) + (l3) * ((gs)+1) * ((gs)+1))
Information getMinimumEntanglement(int *packages, int leftPackages,
        int left1, int left2, int left3, int groupSize, Information *mem, int leftPackages1)
{
    if (left1 < 0 || left2 < 0 || left3 < 0 || leftPackages1 < 0)
        return (Information) {.entanglement = -1, .packages = -1};

    int index = AT(left1, left2, left3, groupSize);
    if (mem[index].entanglement != 0)
        goto end;
    if (leftPackages == 0)
    {
        mem[index] = left1 != 0 || left2 != 0 || left3 != 0
                ? (Information) {.entanglement = -1, .packages = -1}
                : (Information) {.entanglement = 1, .packages = 0};
        goto end;
    }

    // Let's go
    
    Information in1 = getMinimumEntanglement(packages, leftPackages-1, left1-packages[leftPackages-1],
            left2, left3, groupSize, mem, leftPackages1-1);
    if (in1.packages >= 0)
    {
        in1.entanglement *= packages[leftPackages-1];
        in1.packages++;
    }

    if (in1.packages >= 0 && in1.packages < leftPackages1)
        // New value to undergo
        leftPackages1 = in1.packages;

    Information in2 = getMinimumEntanglement(packages, leftPackages-1, left1,
            left2-packages[leftPackages-1], left3, groupSize, mem, leftPackages1);

    Information minimum = getMinimum(in1, in2);
    if (minimum.packages >= 0 && minimum.packages < leftPackages1)
        leftPackages1 = minimum.packages;

    Information in3 = getMinimumEntanglement(packages, leftPackages-1, left1,
            left2, left3-packages[leftPackages-1], groupSize, mem, leftPackages1);

    mem[index] = getMinimum(minimum, in3);

end:
    if (mem[index].entanglement < -1)
        printf("Entanglement: %ld; Packages: %d\n", mem[index].entanglement, mem[index].packages);
    return mem[index];
}

Information getMinimum(Information a, Information b)
{
    if (a.packages == -1)
        return b;
    if (b.packages == -1)
        return a;
    if (a.packages < b.packages)
        return a;
    else if (a.packages > b.packages)
        return b;
    else if (a.entanglement < b.entanglement)
        return a;
    else
        return b;
}

Information getMinimumEntanglement2(int *packages, int leftPackages,
        int left1, int left2, int left3, int left4, int groupSize, Information *mem, int leftPackages1)
{
    if (left1 < 0 || left2 < 0 || left3 < 0 || left4 < 0 || leftPackages1 < 0)
        return (Information) {.entanglement = -1, .packages = -1};

    int index = AT(left1, left2, left3, groupSize);
    if (mem[index].entanglement != 0)
        goto end;
    if (leftPackages == 0)
    {
        mem[index] = left1 != 0 || left2 != 0 || left3 != 0 || left4 != 0
                ? (Information) {.entanglement = -1, .packages = -1}
                : (Information) {.entanglement = 1, .packages = 0};
        goto end;
    }

    // Let's go
    
    Information in1 = getMinimumEntanglement2(packages, leftPackages-1, left1-packages[leftPackages-1],
            left2, left3, left4, groupSize, mem, leftPackages1-1);
    if (in1.packages >= 0 && in1.entanglement < LONG_MAX / packages[leftPackages-1] + packages[leftPackages-1])
    {
        in1.entanglement *= packages[leftPackages-1];
        in1.packages++;
    }

    if (in1.packages >= 0 && in1.packages < leftPackages1)
        // New value to undergo
        leftPackages1 = in1.packages;

    Information in2 = getMinimumEntanglement2(packages, leftPackages-1, left1,
            left2-packages[leftPackages-1], left3, left4, groupSize, mem, leftPackages1);

    Information minimum = getMinimum(in1, in2);
    if (minimum.packages >= 0 && minimum.packages < leftPackages1)
        leftPackages1 = minimum.packages;

    Information in3 = getMinimumEntanglement2(packages, leftPackages-1, left1,
            left2, left3-packages[leftPackages-1], left4, groupSize, mem, leftPackages1);

    minimum = getMinimum(minimum, in3);
    if (minimum.packages >= 0 && minimum.packages < leftPackages1)
        leftPackages1 = minimum.packages;

    Information in4 = getMinimumEntanglement2(packages, leftPackages-1, left1,
            left2, left3, left4-packages[leftPackages-1], groupSize, mem, leftPackages1);

    mem[index] = getMinimum(minimum, in4);

end:
    if (mem[index].entanglement < -1)
        printf("Entanglement: %ld; Packages: %d\n", mem[index].entanglement, mem[index].packages);
    return mem[index];
}

