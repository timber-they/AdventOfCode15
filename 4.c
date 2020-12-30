#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#define LINE_LENGTH 8
#define MINE_LENGTH 11

int part1(FILE *in);
int part2(FILE *in);
char *md5(char *string);
int isValid(char *line, int number);

int main()
{
    FILE *in = fopen("in4", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char line[LINE_LENGTH+MINE_LENGTH+2];
    if (fgets(line, LINE_LENGTH+2, in) == NULL)
        return -1;
    *strchr(line, '\n') = '\0';
    for (int i = 0;; i++)
        if (isValid(line, i))
            return i;
}

int isValid(char *line, int number)
{
    char *clone = malloc((MINE_LENGTH + LINE_LENGTH + 1) * sizeof(*clone));
    strcpy(clone, line);
    char str[MINE_LENGTH+1];
    snprintf(str, MINE_LENGTH, "%d", number);
    strcat(clone, str);
    char *res = md5(clone);
    int i;
    for (i = 0; i < 5; i++)
        if (res[i] != '0')
            break;

    free(clone);
    free(res);
    return i == 5;
}

int part2(FILE *in)
{
    in = NULL;
    return -1;
}

char *md5(char *string)
{
    char *result = calloc(2 * MD5_DIGEST_LENGTH + 1, sizeof(*result));
    unsigned char hash[MD5_DIGEST_LENGTH];
    char buff[3];
    MD5((unsigned char *) string, strlen(string), hash);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        snprintf(buff, 3, "%02x", hash[i]);
        strcat(result, buff);
    }
    return result;
}

