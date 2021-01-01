#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int part1(FILE *in);
int part2(FILE *in);
int memorySize(char *line);
int encodedSize(char *line);

int main()
{
    FILE *in = fopen("in8", "r");

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
    int res = 0;
    while (getline(&line, &n, in) > 0 && *line != '\n')
    {
        int characterCount = strlen(line);
        int memSize = memorySize(line);
        res += characterCount - memSize;
    }
    free(line);
    return res;
}

int part2(FILE *in)
{
    size_t n = 0;
    char *line = NULL;
    int res = 0;
    while (getline(&line, &n, in) > 0 && *line != '\n')
    {
        int characterCount = strlen(line);
        int memSize = encodedSize(line);
        res += memSize - characterCount;
    }
    free(line);
    return res;
}

int memorySize(char *line)
{
    int res = strlen(line);
    res -= 2;
    line -= 2;
    while ((line = strchr(line+2, '\\')) != NULL)
    {
        switch(line[1])
        {
            case '\\':
                res--;
                break;
            case '"':
                res--;
                break;
            case 'x':
                res -= 3;
                break;
            default:
                fprintf(stderr, "Unexpected escape: %c (for %s)\n", line[1], line);
                exit(1);
        }
    }
    return res;
}

int encodedSize(char *line)
{
    int res = strlen(line);
    res += 2;
    char *ptr = line-1;
    while ((ptr = strchr(ptr+1, '"')) != NULL)
        res++;
    ptr = line-1;
    while ((ptr = strchr(ptr+1, '\\')) != NULL)
        res++;
    return res;
}

