#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define AUNT_COUNT 500

typedef struct Aunt
{
    int children;
    int cats;
    int samoyeds;
    int pomeranians;
    int akitas;
    int vizslas;
    int goldfish;
    int trees;
    int cars;
    int perfumes;
} Aunt;

int part1(FILE *in);
int part2(FILE *in);
// -1 means unknown
Aunt parse(char *line);
int mistakes(Aunt toMatch, Aunt matching);
int mistakes2(Aunt toMatch, Aunt matching);
void printAunt(Aunt aunt);

int main()
{
    FILE *in = fopen("in16", "r");

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

    Aunt goal = 
    {
        .children = 3,
        .cats = 7,
        .samoyeds = 2,
        .pomeranians = 3,
        .akitas = 0,
        .vizslas = 0,
        .goldfish = 5,
        .trees = 3,
        .cars = 2,
        .perfumes = 1
    };

    int i = 0;
    while (getline(&line, &n, in) > 1)
    {
        i++;
        Aunt parsed = parse(line);
        int misses = mistakes(goal, parsed);
        if (!misses)
            return i;
    }
    free(line);
    return -1;
}

int part2(FILE *in)
{
    char *line = NULL;
    size_t n = 0;

    Aunt goal = 
    {
        .children = 3,
        .cats = 7,
        .samoyeds = 2,
        .pomeranians = 3,
        .akitas = 0,
        .vizslas = 0,
        .goldfish = 5,
        .trees = 3,
        .cars = 2,
        .perfumes = 1
    };

    int i = 0;
    while (getline(&line, &n, in) > 1)
    {
        i++;
        Aunt parsed = parse(line);
        int misses = mistakes2(goal, parsed);
        if (!misses)
            return i;
    }
    free(line);
    return -1;
}

void printAunt(Aunt aunt)
{
    printf("children: %d, cats: %d, samoyeds: %d, pomeranians: %d, akitas: %d, vizslas: %d, goldfish: %d, trees: %d, cars: %d, perfumes: %d\n",
            aunt.children,
            aunt.cats,
            aunt.samoyeds,
            aunt.pomeranians,
            aunt.akitas,
            aunt.vizslas,
            aunt.goldfish,
            aunt.trees,
            aunt.cars,
            aunt.perfumes);
}

int mistakes(Aunt toMatch, Aunt matching)
{
    int res = 0;
    res += toMatch.children != matching.children && matching.children != -1;
    res += toMatch.cats != matching.cats && matching.cats != -1;
    res += toMatch.samoyeds != matching.samoyeds && matching.samoyeds != -1;
    res += toMatch.pomeranians != matching.pomeranians && matching.pomeranians != -1;
    res += toMatch.akitas != matching.akitas && matching.akitas != -1;
    res += toMatch.vizslas != matching.vizslas && matching.vizslas != -1;
    res += toMatch.goldfish != matching.goldfish && matching.goldfish != -1;
    res += toMatch.trees != matching.trees && matching.trees != -1;
    res += toMatch.cars != matching.cars && matching.cars != -1;
    res += toMatch.perfumes != matching.perfumes && matching.perfumes != -1;
    return res;
}

int mistakes2(Aunt toMatch, Aunt matching)
{
    int res = 0;
    res += toMatch.children != matching.children && matching.children != -1;
    res += toMatch.cats >= matching.cats && matching.cats != -1;
    res += toMatch.samoyeds != matching.samoyeds && matching.samoyeds != -1;
    res += toMatch.pomeranians <= matching.pomeranians && matching.pomeranians != -1;
    res += toMatch.akitas != matching.akitas && matching.akitas != -1;
    res += toMatch.vizslas != matching.vizslas && matching.vizslas != -1;
    res += toMatch.goldfish <= matching.goldfish && matching.goldfish != -1;
    res += toMatch.trees >= matching.trees && matching.trees != -1;
    res += toMatch.cars != matching.cars && matching.cars != -1;
    res += toMatch.perfumes != matching.perfumes && matching.perfumes != -1;
    return res;
}

Aunt parse(char *line)
{
    Aunt res = 
    {
        .children = -1,
        .cats = -1,
        .samoyeds = -1,
        .pomeranians = -1,
        .akitas = -1,
        .vizslas = -1,
        .goldfish = -1,
        .trees = -1,
        .cars = -1,
        .perfumes = -1
    };
    char *ptr = strstr(line, "children: ");
    if (ptr != NULL)
    {
        ptr += strlen("children: ");
        res.children = atoi(ptr);
    }
    ptr = strstr(line, "cats: ");
    if (ptr != NULL)
    {
        ptr += strlen("cats: ");
        res.cats = atoi(ptr);
    }
    ptr = strstr(line, "samoyeds: ");
    if (ptr != NULL)
    {
        ptr += strlen("samoyeds: ");
        res.samoyeds = atoi(ptr);
    }
    ptr = strstr(line, "pomeranians: ");
    if (ptr != NULL)
    {
        ptr += strlen("pomeranians: ");
        res.pomeranians = atoi(ptr);
    }
    ptr = strstr(line, "akitas: ");
    if (ptr != NULL)
    {
        ptr += strlen("akitas: ");
        res.akitas = atoi(ptr);
    }
    ptr = strstr(line, "vizslas: ");
    if (ptr != NULL)
    {
        ptr += strlen("vizslas: ");
        res.vizslas = atoi(ptr);
    }
    ptr = strstr(line, "goldfish: ");
    if (ptr != NULL)
    {
        ptr += strlen("goldfish: ");
        res.goldfish = atoi(ptr);
    }
    ptr = strstr(line, "trees: ");
    if (ptr != NULL)
    {
        ptr += strlen("trees: ");
        res.trees = atoi(ptr);
    }
    ptr = strstr(line, "cars: ");
    if (ptr != NULL)
    {
        ptr += strlen("cars: ");
        res.cars = atoi(ptr);
    }
    ptr = strstr(line, "perfumes: ");
    if (ptr != NULL)
    {
        ptr += strlen("perfumes: ");
        res.perfumes = atoi(ptr);
    }
    return res;
}

