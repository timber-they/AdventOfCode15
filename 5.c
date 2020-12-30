#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

int part1(FILE *in);
int part2(FILE *in);
int isNice(char *line);
int isNicer(char *line);
int appearsTwice(char *line, char a, char b);

int main()
{
    FILE *in = fopen("in5", "r");

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
    int count = 0;
    while (getline(&line, &n, in) > 0 && *line != '\n')
        count += isNice(line);
    return count;
}

int part2(FILE *in)
{
    char *line = NULL;
    size_t n = 0;
    int count = 0;
    while (getline(&line, &n, in) > 0 && *line != '\n')
        count += isNicer(line);
    return count;
}

int isNice(char *line)
{
    int vowelCount = 0;
    for (int i = 0; i < strlen(line); i++)
        if (line[i] == 'a' || 
                line[i] == 'e' || 
                line[i] == 'i' ||
                line[i] == 'o' ||
                line[i] == 'u')
            vowelCount++;
    if (vowelCount < 3)
        return 0;
    int i;
    for (i = 1; i < strlen(line); i++)
        if (line[i] == line[i-1])
            break;
    if (i == strlen(line))
        return 0;
    if (strstr(line, "ab") != NULL ||
            strstr(line, "cd") != NULL ||
            strstr(line, "pq") != NULL ||
            strstr(line, "xy") != NULL)
        return 0;
    return 1;
}

int isNicer(char *line)
{
    int i;
    for (i = 1; i < strlen(line); i++)
        if (appearsTwice(line, line[i-1], line[i]))
            break;
    if (i == strlen(line))
        return 0;
    for (i = 2; i < strlen(line); i++)
        if (line[i-2] == line[i])
            break;
    if (i == strlen(line))
        return 0;
    return 1;
}

int appearsTwice(char *line, char a, char b)
{
    int count = 0;
    for (int i = 1; i < strlen(line); i++)
        if (line[i-1] == a && line[i] == b)
        {
            count++;
            // No overlap
            i++;
        }
    return count >= 2;
}

