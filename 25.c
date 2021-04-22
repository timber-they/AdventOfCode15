#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define INIITAL 20151125

int part1(FILE *in);
int part2(FILE *in);
int getIndex(FILE *in);
int getValue(int index);

int main()
{
    FILE *in = fopen("in25", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int index = getIndex(in);
    return getValue(index);
}

int part2(FILE *in)
{
    return -1;
}

int getIndex(FILE *in)
{
    size_t n = 0;
    char *line = NULL;

    getline(&line, &n, in);

    char *ptr = strstr(line, "row ");
    if (ptr == NULL)
    {
        free(line);
        return -1;
    }
    int row = atoi(ptr+strlen("row "));

    ptr = strstr(line, "column ");
    if (ptr == NULL)
    {
        free(line);
        return -1;
    }
    int col = atoi(ptr+strlen("column "));
    free(line);

    int i = 1;
    for (int j = 1; j < row; j++)
        i += j;
    for (int j = row+1; j < col+row; j++)
        i += j;

    return i;
}

int getValue(int index)
{
    long value = INIITAL;
    for (int i = 1; i < index; i++)
        value = (value * 252533) % 33554393;
    return (int) value;
}

