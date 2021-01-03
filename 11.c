#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD_LENGTH 8

char *part1(FILE *in);
char *part2(FILE *in);
char *getNextPassword(char *current);
int isInvalid(char *password);
char *inc(char *current);

int main()
{
    FILE *in = fopen("in11", "r");

    char *p1 = part1(in);
    printf("Part1: %s\n", p1);
    rewind(in);
    char *p2 = part2(in);
    printf("Part2: %s\n", p2);

    free(p1);
    free(p2);
    fclose(in);
    return 0;
}

char *part1(FILE *in)
{
    char *current = calloc(PASSWORD_LENGTH+1, sizeof(*current));
    fgets(current, PASSWORD_LENGTH+1, in);
    if (current == NULL)
    {
        fprintf(stderr, "Couldn't read\n");
        exit(2);
    }
    return getNextPassword(current);
}

char *part2(FILE *in)
{
    char *current = calloc(PASSWORD_LENGTH+1, sizeof(*current));
    fgets(current, PASSWORD_LENGTH+1, in);
    if (current == NULL)
    {
        fprintf(stderr, "Couldn't read\n");
        exit(2);
    }
    return getNextPassword(getNextPassword(current));
}

char *getNextPassword(char *current)
{
    for (inc(current); isInvalid(current); inc(current));
    return current;
}

int isInvalid(char *password)
{
    int i;
    // Length
    if (strlen(password) != PASSWORD_LENGTH)
        return 1;
    // Lowercase
    for (i = 0; i < PASSWORD_LENGTH; i++)
        if (password[i] < 'a' || password[i] > 'z')
            return 2;
    // Straigt
    for (i = 2; i < PASSWORD_LENGTH; i++)
        if (password[i-2] == password[i]-2 && password[i-1] == password[i]-1)
            break;
    if (i == PASSWORD_LENGTH)
        return 3;
    // Not mistakable
    if (strchr(password, 'i') != NULL || 
            strchr(password, 'o') != NULL ||
            strchr(password, 'l') != NULL)
        return 4;
    // Pairs
    int found = 0;
    for (i = 1; i < PASSWORD_LENGTH; i++)
        if (password[i-1] == password[i])
        {
            if (found)
                break;
            found++;
            i++;
        }
    if (i >= PASSWORD_LENGTH)
        return 5;
    return 0;
}

char *inc(char *current)
{
    for (int r = PASSWORD_LENGTH-1; r >= 0; r--)
    {
        current[r]++;
        if (current[r] <= 'z')
            return current;
        current[r] = 'a';
    }
    fprintf(stderr, "Couldn't inc anymore\n");
    exit(1);
}

