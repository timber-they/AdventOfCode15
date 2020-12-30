#include <stdio.h>
#include <stdlib.h>

int part1(FILE *in);
int part2(FILE *in);

int main()
{
    FILE *in = fopen("in1", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int res = 0;
    char c;
    while ((c = fgetc(in)) != '\n')
        res += c == '(' ? 1 : -1;
    return res;
}

int part2(FILE *in)
{
    int floor = 0;
    char c;
    int i = 0;
    for (c = fgetc(in); floor != -1; c = fgetc(in), i++)
        floor += c == '(' ? 1 : -1;
    return i;
}

