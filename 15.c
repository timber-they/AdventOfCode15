#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INGREDIENTS 4
#define PROPERTIES 5

int part1(FILE *in);
int part2(FILE *in);
int **getProperties(FILE *in);
int maximize(int **properties, int n, int s, int r, int *weights);
int getScore(int* weights, int **properties);
int getCalories(int *weights, int **properties);
int maximizeLean(int **properties, int n, int s, int r, int *weights);

int main()
{
    FILE *in = fopen("in15", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int **properties = getProperties(in);
    int *weights = malloc(INGREDIENTS * sizeof(*weights));
    int res = maximize(properties, INGREDIENTS, 0, 100, weights);
    
    for (int i = 0; i < INGREDIENTS; i++)
        free(properties[i]);
    free(properties);
    free(weights);
    return res;
}

int part2(FILE *in)
{
    int **properties = getProperties(in);
    int *weights = malloc(INGREDIENTS * sizeof(*weights));
    int res = maximizeLean(properties, INGREDIENTS, 0, 100, weights);
    
    for (int i = 0; i < INGREDIENTS; i++)
        free(properties[i]);
    free(properties);
    free(weights);
    return res;
}

int maximize(int **properties, int n, int s, int r, int *weights)
{
    if (s == n)
        return getScore(weights, properties);
    else if (s == n-1)
    {
        weights[s] = r;
        return maximize(properties, n, s+1, 0, weights);
    }
    int max = 0;
    for (int i = 0; i < r; i++)
    {
        weights[s] = i;
        int score = maximize(properties, n, s+1, r-i, weights);
        if (score > max)
            max = score;
    }
    return max;
}

int maximizeLean(int **properties, int n, int s, int r, int *weights)
{
    if (s == n)
        return getScore(weights, properties);
    else if (s == n-1)
    {
        weights[s] = r;
        if (getCalories(weights, properties) != 500)
            return -1;
        return maximizeLean(properties, n, s+1, 0, weights);
    }
    int max = 0;
    for (int i = 0; i < r; i++)
    {
        weights[s] = i;
        int score = maximizeLean(properties, n, s+1, r-i, weights);
        if (score > max)
            max = score;
    }
    return max;
}

int getCalories(int *weights, int **properties)
{
    int res = 0;
    for (int i = 0; i < INGREDIENTS; i++)
        res += weights[i] * properties[i][PROPERTIES-1];
    return res;
}

int getScore(int* weights, int **properties)
{
    int res = 1;
    for (int i = 0; i < PROPERTIES-1; i++)
    {
        int sum = 0;
        for (int j = 0; j < INGREDIENTS; j++)
            sum += properties[j][i] * weights[j];
        if (sum < 0)
            sum = 0;
        res *= sum;
    }
    return res;
}

int **getProperties(FILE *in)
{
    int **res = malloc(INGREDIENTS * sizeof(*res));
    for (int i = 0; i < INGREDIENTS; i++)
    {
        res[i]= malloc(PROPERTIES * sizeof(*res[i]));
        if (fscanf(in, "%*s capacity %d, durability %d, flavor %d, texture %d, calories %d",
                    res[i], res[i]+1, res[i]+2, res[i]+3, res[i]+4) != 5)
        {
            fprintf(stderr, "PANIC\n");
            exit(1);
        }
    }

    return res;
}

