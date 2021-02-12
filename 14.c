#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define REINDEER_CNT 9
#define TIME 2503

typedef struct Reindeer{
    int speed;
    int endurance;
    int rest;
} Reindeer;

int part1(FILE *in);
int part2(FILE *in);
Reindeer *getReindeers(FILE *in);
void printReindeer(Reindeer reindeer);
int travel(Reindeer reindeer, int time);

int main()
{
    FILE *in = fopen("in14", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Reindeer *reindeers = getReindeers(in);
    for (int i = 0; i < REINDEER_CNT; i++)
        printReindeer(reindeers[i]);
    
    int max = 0;
    for (int i = 0; i < REINDEER_CNT; i++)
    {
        int dist = travel(reindeers[i], TIME);
        if (dist > max)
            max = dist;
    }

    free(reindeers);
    return max;
}

int part2(FILE *in)
{
    in = NULL;
    return -1;
}

int travel(Reindeer reindeer, int time)
{
    int res = 0;
    int whole = time / (reindeer.endurance + reindeer.rest);
    res += whole * reindeer.speed * reindeer.endurance;
    int rest = time - whole * (reindeer.endurance + reindeer.rest);
    res += (rest > reindeer.endurance)
        ? reindeer.endurance * reindeer.speed
        : rest * reindeer.speed;
    return res;
}

void printReindeer(Reindeer reindeer)
{
    printf("Speed: %d m/s; Endurance: %d s; Rest: %d s\n", reindeer.speed, reindeer.endurance, reindeer.rest);
}

Reindeer *getReindeers(FILE *in)
{
    Reindeer *res = malloc(REINDEER_CNT * sizeof(*res));
    for (int i = 0; i < REINDEER_CNT; i++)
    {
        errno = 0;
        int read;
        if ((read = fscanf(in, "%*s can fly %d km/s for %d seconds, but then must rest for %d seconds.",
                   &(res[i].speed), &(res[i].endurance), &(res[i].rest))) != 3)
        {
            fprintf(stderr, "PANIC - %d: %d (%d)\n", i, errno, read);
            exit(1);
        }
    }

    return res;
}

