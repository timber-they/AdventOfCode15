#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int part1(FILE *in);
int part2(FILE *in);
int paper(char *line);
int ribbon(char *line);

int main()
{
    FILE *in = fopen("in2", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    size_t n = 0;
    char *line = NULL;
    int totalPaper = 0;
    while (getline(&line, &n, in) > 0 && *line != '\n')
        totalPaper += paper(line);
    free(line);
    return totalPaper;
}

int part2(FILE *in)
{
    size_t n = 0;
    char *line = NULL;
    int totalRibbon = 0;
    while (getline(&line, &n, in) > 0 && *line != '\n')
        totalRibbon += ribbon(line);
    free(line);
    return totalRibbon;
}

int paper(char *line)
{
    char *times = strchr(line, 'x');
    *times = '\0';
    int l = atoi(line);
    line = times+1;
    times = strchr(line, 'x');
    *times = '\0';
    int w = atoi(line);
    line = times+1;
    int h = atoi(line);

    int s1 = l*w;
    int s2 = w*h;
    int s3 = h*l;
    int min = s1;
    if (s2 < min)
        min = s2;
    if (s3 < min)
        min = s3;
    return 2*s1 + 2*s2 + 2*s3 + min;
}

int ribbon(char *line)
{
    char *times = strchr(line, 'x');
    *times = '\0';
    int l = atoi(line);
    line = times+1;
    times = strchr(line, 'x');
    *times = '\0';
    int w = atoi(line);
    line = times+1;
    int h = atoi(line);

    int min1 = l < w
        ? l < h
            // l < w,h
            ? l
            // h < l < w
            : h
        : w < h
            // w < l,h
            ? w
            // h < w < l
            : h;
    int min2 = l < w
        ? l < h
            // l < w,h
            ? w < h
                // l < w < h
                ? w
                // l < h < w
                : h
            // h < l < w
            : l
        : w < h
            // w < l,h
            ? l < h
                // w < l < h
                ? l
                // w < h < l
                : h
            // h < w < l
            : w;
    return 2*min1 + 2*min2 + l*w*h;
}

