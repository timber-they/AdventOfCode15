#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define REPLACEMENT_COUNT 43
#define MAX_IN 2
#define MAX_OUT 10
#define MAX_REPLACEMENTS 100
#define MAX_BLOWUP 100000

typedef struct Replacement
{
    char *in;
    char *out;
} Replacement;

typedef struct NormalRule
{
    int in;
    int *out;
} NormalRule;

typedef enum ReplacableElement
{
    Al,
    B,
    Ca,
    F,
    H,
    Mg,
    N,
    O,
    P,
    Si,
    Th,
    Ti,
    // Not an element but who cares
    e
} ReplacableElement;

typedef enum NonReplacableElements
{
    Rn = (int)e+1,
    Ar = (int)e+2,
    Y = (int)e+3,
    C = (int)e+4
} NonReplacableElements;

char *elementStrings[] =
{
    "Al",
    "B",
    "Ca",
    "F",
    "H",
    "Mg",
    "N",
    "O",
    "P",
    "Si",
    "Th",
    "Ti",
    "e",
    "Rn",
    "Ar",
    "Y",
    "C"
};

int part1(FILE *in);
int part2(FILE *in);
Replacement *getReplacements(FILE *in, Replacement *buff);
char **applyReplacement(Replacement replacement, char *medicine);
char *replace(char *string, char *ptr, char *toReplace, char *replacement);
char **applyAllReplacements(Replacement *replacements, char *medicine);
int addDistinct(char **set, char *toAdd);
int count(char **set);
int *normalizeMedicine(char *s);
NormalRule *normalizeReplacements(Replacement *replacements, NormalRule *buff);
void printNormalRules(NormalRule *rules);
void printMedicine(int *medicine);

int main()
{
    FILE *in = fopen("in19", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Replacement replacements[REPLACEMENT_COUNT];
    getReplacements(in, replacements);
    char *medicine = NULL;
    size_t n = 0;
    if (getline(&medicine, &n, in) == 0)
        fprintf(stderr, "Couldn't read medicine\n");
    // Don't store line break
    medicine[strlen(medicine)-1] = '\0';

    char **replaced = applyAllReplacements(replacements, medicine);
    int res = count(replaced);

    for (int i = 0; replaced[i] != NULL; i++)
        free(replaced[i]);
    free(replaced);
    for (int i = 0; i < REPLACEMENT_COUNT; i++)
    {
        free(replacements[i].in);
        free(replacements[i].out);
    }
    free(medicine);
    return res;
}

int part2(FILE *in)
{
    Replacement replacements[REPLACEMENT_COUNT];
    getReplacements(in, replacements);
    char *medicine = NULL;
    size_t n = 0;
    if (getline(&medicine, &n, in) == 0)
        fprintf(stderr, "Couldn't read medicine\n");
    // Don't store line break
    medicine[strlen(medicine)-1] = '\0';

    // Normalize
    int *normalMedicine = normalizeMedicine(medicine);
    NormalRule normalRules[REPLACEMENT_COUNT];
    normalizeReplacements(replacements, normalRules);

    // Because 16 doesn't appear in the result, all rules used produce two from one
    //  or are of one of the following formats:
    //  x 13 x 14
    //  x 13 x 15 x 14
    //  Hence the count of the second format is simply the count of 15 (15 can't be obtained otherwise)
    //  and the count of the first format is the count of 13 (or 14 - both can't be obtained otherwise)
    //  _minus_ the count of 15.
    //  The first format makes 4 from 1 (so +3) and the second 6 from 1 (so +5). If m is the resulting
    //  elements, cx the count of the element x and n the steps (what we're looking for), the following must hold then:
    //  m = c15 * 5 + (c13 - c15) * 3 + n - c13 + 1
    //  => n = m - (c15 * 5 + (c13 - c15) * 3 - c13 + 1)
    
    // Count 13 and 15
    int c13 = 0, c15 = 0;
    // Count m
    int m;
    for (m = 0; normalMedicine[m] >= 0; m++)
    {
        c13 += normalMedicine[m] == 13;
        c15 += normalMedicine[m] == 15;
    }
    int res = m - (c15 * 5 + (c13 - c15) * 3 - c13 + 1);

    for (int i = 0; i < REPLACEMENT_COUNT; i++)
    {
        free(replacements[i].in);
        free(replacements[i].out);
        free(normalRules[i].out);
    }

    free(normalMedicine);
    free(medicine);
    return res;
}

void printNormalRules(NormalRule *rules)
{
    for (int i = 0; i < REPLACEMENT_COUNT; i++)
    {
        printf("%s => ", elementStrings[rules[i].in]);
        printMedicine(rules[i].out);
    }
}

void printMedicine(int *medicine)
{
    for (int *ptr = medicine; *ptr >= 0; ptr++)
        printf("%s", elementStrings[*ptr]);
    printf("\n");
}

int *normalizeMedicine(char *s)
{
    int *res = malloc((strlen(s)+1) * sizeof(*res));
    for (int i = 0; i <= strlen(s); i++)
        res[i] = -1;
    int r = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        switch(s[i])
        {
            case 'A':
                switch(s[i+1])
                {
                    case 'l':
                        res[r++] = (int) Al;
                        break;
                    case 'r':
                        res[r++] = (int) Ar;
                        break;
                }
                i++;
                break;
            case 'B':
                res[r++] = (int) B;
                break;
            case 'C':
                switch(s[i+1])
                {
                    case 'a':
                        res[r++] = (int) Ca;
                        i++;
                        break;
                    default:
                        res[r++] = (int) C;
                        break;
                }
                break;
            case 'F':
                res[r++] = (int) F;
                break;
            case 'H':
                res[r++] = (int) H;
                break;
            case 'M':
                res[r++] = (int) Mg;
                i++;
                break;
            case 'N':
                res[r++] = (int) N;
                break;
            case 'O':
                res[r++] = (int) O;
                break;
            case 'P':
                res[r++] = (int) P;
                break;
            case 'S':
                res[r++] = (int) Si;
                i++;
                break;
            case 'T':
                switch(s[i+1])
                {
                    case 'h':
                        res[r++] = (int) Th;
                        break;
                    case 'i':
                        res[r++] = (int) Ti;
                        break;
                }
                i++;
                break;
            case 'R':
                res[r++] = (int) Rn;
                break;
            case 'Y':
                res[r++] = (int) Y;
                break;
            case 'e':
                res[r++] = (int) e;
                break;
        }
    }
    return res;
}

NormalRule *normalizeReplacements(Replacement *replacements, NormalRule *buff)
{
    for (int i = 0; i < REPLACEMENT_COUNT; i++)
    {
        int *in = normalizeMedicine(replacements[i].in);
        int *out = normalizeMedicine(replacements[i].out);
        buff[i].in = in[0];
        buff[i].out = out;
        free(in);
    }
    return buff;
}

int count(char **set)
{
    int i;
    for (i = 0; set[i] != NULL; i++);
    return i;
}

int addDistinct(char **set, char *toAdd)
{
    int i;
    for (i = 0; set[i] != NULL; i++)
        if (!strcmp(set[i], toAdd))
            return 0;
    set[i] = toAdd;
    return 1;
}

char **applyAllReplacements(Replacement *replacements, char *medicine)
{
    char **res = calloc(REPLACEMENT_COUNT * MAX_REPLACEMENTS, sizeof(*res));
    for (int i = 0; i < REPLACEMENT_COUNT; i++)
    {
        char **replaced = applyReplacement(replacements[i], medicine);
        for (int j = 0; replaced[j] != NULL; j++)
            if (!addDistinct(res, replaced[j]))
                free(replaced[j]);
        free(replaced);
    }
    return res;
}

char **applyReplacement(Replacement replacement, char *medicine)
{
    char **res = calloc(MAX_REPLACEMENTS, sizeof(*res));
    char *ptr = medicine;
    for (int i = 0; i <= MAX_REPLACEMENTS; i++)
    {
        if (i == MAX_REPLACEMENTS)
        {
            fprintf(stderr, "Replacements overflow\n");
            break;
        }
        ptr = strstr(ptr, replacement.in);
        if (ptr == NULL)
            break;
        res[i] = replace(medicine, ptr, replacement.in, replacement.out);
        ptr++;
    }
    return res;
}

char *replace(char *string, char *ptr, char *toReplace, char *replacement)
{
    char *res = malloc((strlen(string) + strlen(replacement) - strlen(toReplace) + 1) * sizeof(*res));
    size_t pre = strlen(string) - strlen(ptr);
    strncpy(res, string, pre * sizeof(*res));
    res[pre] = '\0';
    strcat(res, replacement);
    ptr += strlen(toReplace);
    strcat(res, ptr);
    return res;
}

Replacement *getReplacements(FILE *file, Replacement *buff)
{
    char *in;
    char *out;

    for (int i = 0; i < REPLACEMENT_COUNT; i++)
    {
        in = malloc((MAX_IN+1) * sizeof(*in));
        out = malloc((MAX_OUT+1) * sizeof(*out));

        if (fscanf(file, "%s => %s ", in, out) != 2)
            fprintf(stderr, "Couldn't read line %d\n", i);

        buff[i] = (Replacement) {.in = in, .out = out};
    }

    return buff;
}

