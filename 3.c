#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HOUSE_CNT 8192

int part1(FILE *in);
int part2(FILE *in);
int contains(int *houses, int x, int y, int houseCount);

int main()
{
    FILE *in = fopen("in3", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int *houses = malloc(HOUSE_CNT * 2 * sizeof(*houses));
    char c;
    int houseCount = 0;
    int x = 0, y = 0;
    while ((c = fgetc(in)) != '\n')
    {
        if (!contains(houses, x, y, houseCount))
        {
            houses[2*houseCount] = x;
            houses[2*houseCount+1] = y;
            houseCount++;
        }
        switch(c)
        {
            case '^':
                y++;
                break;
            case 'v':
                y--;
                break;
            case '>':
                x++;
                break;
            case '<':
                x--;
                break;
        }
    }
    if (!contains(houses, x, y, houseCount))
    {
        houses[2*houseCount] = x;
        houses[2*houseCount+1] = y;
        houseCount++;
    }
    free(houses);
    return houseCount;
}

int contains(int *houses, int x, int y, int houseCount)
{
    for (int i = 0; i < houseCount; i++)
        if (houses[2*i] == x && houses[2*i+1] == y)
            return i+1;
    return 0;
}

int part2(FILE *in)
{
    int *houses = malloc(HOUSE_CNT * 2 * sizeof(*houses));
    char c;
    int houseCount = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int robot = 0;
    while ((c = fgetc(in)) != '\n')
    {
        if (robot)
        {
            if (!contains(houses, x1, y1, houseCount))
            {
                houses[2*houseCount] = x1;
                houses[2*houseCount+1] = y1;
                houseCount++;
            }
            switch(c)
            {
                case '^':
                    y1++;
                    break;
                case 'v':
                    y1--;
                    break;
                case '>':
                    x1++;
                    break;
                case '<':
                    x1--;
                    break;
            }
        }
        else
        {
            if (!contains(houses, x2, y2, houseCount))
            {
                houses[2*houseCount] = x2;
                houses[2*houseCount+1] = y2;
                houseCount++;
            }
            switch(c)
            {
                case '^':
                    y2++;
                    break;
                case 'v':
                    y2--;
                    break;
                case '>':
                    x2++;
                    break;
                case '<':
                    x2--;
                    break;
            }
        }
        robot = !robot;
    }
    if (robot && !contains(houses, x1, y1, houseCount))
    {
        houses[2*houseCount] = x1;
        houses[2*houseCount+1] = y1;
        houseCount++;
    }
    else if (!robot && !contains(houses, x2, y2, houseCount))
    {
        houses[2*houseCount] = x2;
        houses[2*houseCount+1] = y2;
        houseCount++;
    }
    free(houses);
    return houseCount;
}

