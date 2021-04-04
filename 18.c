#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SIZE 100
#define STEPS 100
#define COORD(x,y) ((y)*(SIZE)+(x))
#define IS_ON(state) ((state) == On || (state) == TurningOff)
#define GET(yard,x,y) ((x) >= 0 && (y) >= 0 && (x) < (SIZE) && (y) < (SIZE) && IS_ON((yard)[COORD((x),(y))]))

typedef enum State
{
    On,
    Off,
    TurningOn,
    TurningOff
} State;

int part1(FILE *in);
int part2(FILE *in);
State *read(FILE *in, State *buff);
void print(State *yard);
void iterate(State *yard);
int getActiveNeighbors(State *yard, int x, int y);
int countActive(State *yard);
void iterate2(State *yard);
void keepCornersOn(State *yard);

int main()
{
    FILE *in = fopen("in18", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    State yard[SIZE*SIZE];
    read(in, yard);
    for (int i = 0; i < STEPS; i++)
        iterate(yard);
    return countActive(yard);
}

int part2(FILE *in)
{
    State yard[SIZE*SIZE];
    read(in, yard);
    keepCornersOn(yard);
    for (int i = 0; i < STEPS; i++)
        iterate2(yard);
    return countActive(yard);
}

void keepCornersOn(State *yard)
{
    yard[COORD(0,0)] = On;
    yard[COORD(SIZE-1,0)] = On;
    yard[COORD(0,SIZE-1)] = On;
    yard[COORD(SIZE-1,SIZE-1)] = On;
}

void iterate2(State *yard)
{
    iterate(yard);
    keepCornersOn(yard);
}

int countActive(State *yard)
{
    int res = 0;
    for (int i = 0; i < SIZE * SIZE; i++)
        res += yard[i] == On;
    return res;
}

void iterate(State *yard)
{
    for (int i = 0; i < SIZE * SIZE; i++)
    {
        int activeNeighbors = getActiveNeighbors(yard, i % SIZE, i / SIZE);
        if (yard[i] == On)
            yard[i] = activeNeighbors == 2 || activeNeighbors == 3 ? On : TurningOff;
        else
            yard[i] = activeNeighbors == 3 ? TurningOn : Off;
    }
    for (int i = 0; i < SIZE * SIZE; i++)
        yard[i] = yard[i] == On || yard[i] == TurningOn ? On : Off;
}

int getActiveNeighbors(State *yard, int x, int y)
{
    return GET(yard, x-1, y-1) +
        GET(yard, x, y-1) +
        GET(yard, x+1, y-1) +
        GET(yard, x-1, y) +
        GET(yard, x+1, y) +
        GET(yard, x-1, y+1) +
        GET(yard, x, y+1) +
        GET(yard, x+1, y+1);
}

void print(State *yard)
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
            printf("%c", yard[COORD(x,y)] == On ? '#' : '.');
        printf("\n");
    }
    printf("\n");
}

State *read(FILE *in, State *buff)
{
    int i = 0;
    for (int c = fgetc(in); c != EOF; c = fgetc(in))
        if (c == '#')
            buff[i++] = On;
        else if (c == '.')
            buff[i++] = Off;
    return buff;
}

