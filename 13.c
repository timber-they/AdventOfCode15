#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERSON_COUNT 8
#define NAME_LENGTH 7

int part1(FILE *in);
int part2(FILE *in);
int identify(char *name);
int *createHappinessMatrix(FILE *in);
int maxHappiness(int *matrix, int *seats, int seated);
int contains(int *haystack, int needle, int n);
int getHappiness(int *matrix, int a, int b);

int main()
{
    FILE *in = fopen("in13", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int *happinessMatrix = createHappinessMatrix(in);
    int seats[PERSON_COUNT] = {0};
    int res = maxHappiness(happinessMatrix, seats, 0);

    free(happinessMatrix);
    return res;
}

int part2(FILE *in)
{
    in = NULL;
    return -1;
}

int maxHappiness(int *matrix, int *seats, int seated)
{
    if (seated == 0)
    {
        seats[seated] = 1;
        return maxHappiness(matrix, seats, seated+1);
    }
    if (seated == PERSON_COUNT - 1)
    {
        int i;
        for (i = 1; i <= PERSON_COUNT; i++)
            if (!contains(seats, i, seated))
                break;
        seats[seated] = i;
        return getHappiness(matrix, seats[seated], seats[seated-1]) +
               getHappiness(matrix, seats[seated], seats[0]);
    }
    int max = 0;
    for (int i = 0; i < PERSON_COUNT; i++)
    {
        if (contains(seats, i+1, seated))
            continue;
        seats[seated] = i+1;
        int happiness = getHappiness(matrix, seats[seated], seats[seated-1]);
        happiness += maxHappiness(matrix, seats, seated+1);
        if (happiness > max)
            max = happiness;
    }
    return max;
}

int getHappiness(int *matrix, int a, int b)
{
    return matrix[(a-1) * PERSON_COUNT + (b-1)] +
           matrix[(b-1) * PERSON_COUNT + (a-1)];
}

int contains(int *haystack, int needle, int n)
{
    for (int i = 0; i < n; i++)
        if (haystack[i] == needle)
            return i+1;
    return 0;
}

int *createHappinessMatrix(FILE *in)
{
    char *line = NULL;
    size_t n = 0;
    int *matrix = calloc(PERSON_COUNT * PERSON_COUNT, sizeof(*matrix));
    if (matrix == NULL)
    {
        fprintf(stderr, "Couldn't alloc\n");
        exit(-2);
    }
    while (getline(&line, &n, in) > 0 && line != NULL && *line != '\n')
    {
        char a[NAME_LENGTH+2], b[NAME_LENGTH+2];
        int happiness = -1;
        if (sscanf(line, "%s would gain %d happiness units by sitting next to %[^.].\n",
                    a, &happiness, b) != 3)
        {
            if (sscanf(line, "%s would lose %d happiness units by sitting next to %[^.].\n",
                        a, &happiness, b) != 3)
            {
                fprintf(stderr, "Couldn't parse line %s", line);
                exit(2);
            }
            happiness = -happiness;
        }
        matrix[identify(a) * PERSON_COUNT + identify(b)] = happiness;
    }

    free(line);
    return matrix;
}

int identify(char *name)
{
    if (name == NULL)
    {
        fprintf(stderr, "NULL unidentifiable\n");
        exit(-1);
    }
    if (!strcmp(name, "Alice"))
        return 0;
    if (!strcmp(name, "Bob"))
        return 1;
    if (!strcmp(name, "Carol"))
        return 2;
    if (!strcmp(name, "David"))
        return 3;
    if (!strcmp(name, "Eric"))
        return 4;
    if (!strcmp(name, "Frank"))
        return 5;
    if (!strcmp(name, "George"))
        return 6;
    if (!strcmp(name, "Mallory"))
        return 7;
    fprintf(stderr, "Unknown name: '%s'\n", name);
    exit(1);
}

