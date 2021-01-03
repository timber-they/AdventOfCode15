#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A guess
#define MAX_DEPTH 1000

int part1(FILE *in);
int part2(FILE *in);
int sumNumbers(char *data);
int isdigit(int c);
void ignoreAreas(char *data);
void ignoreArea(char *data, int start);

int main()
{
    FILE *in = fopen("in12", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char *line = NULL;
    size_t n = 0;
    if (getline(&line, &n, in) < 0)
    {
        fprintf(stderr, "Couldn't read\n");
        exit(1);
    }
    
    int res = sumNumbers(line);
    free(line);
    return res;
}

int part2(FILE *in)
{
    char *line = NULL;
    size_t n = 0;
    if (getline(&line, &n, in) < 0)
    {
        fprintf(stderr, "Couldn't read\n");
        exit(1);
    }
    
    ignoreAreas(line);
    int res = sumNumbers(line);
    free(line);
    return res;
}

int sumNumbers(char *data)
{
    int sum = 0;
    char *numberStart = NULL;
    for (; *data != '\n' && *data != '\0'; data++)
    {
        if (numberStart == NULL && (isdigit(*data) || *data == '-'))
            numberStart = data;
        else if (numberStart != NULL && !isdigit(*data))
        {
            *data = '\0';
            sum += atoi(numberStart);
            numberStart = NULL;
        }
    }
    return sum;
}

int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

void ignoreAreas(char *data)
{
    for (int i = 4; i < strlen(data); i++)
        if (data[i-4] == ':' && 
                data[i-3] == '"' &&
                data[i-2] == 'r' && 
                data[i-1] == 'e' && 
                data[i] == 'd')
            ignoreArea(data, i);
}

void ignoreArea(char *data, int start)
{
    int depth = 0;
    for (int r = start; r >= 0 && depth >= 0; r--)
    {
        if (data[r] == '}')
            depth++;
        if (data[r] == '{')
            depth--;
        data[r] = ' ';
    }
    depth = 0;
    for (int i = start; i < strlen(data) && depth >= 0; i++)
    {
        if (data[i] == '{')
            depth++;
        if (data[i] == '}')
            depth--;
        data[i] = ' ';
    }
}

