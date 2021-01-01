#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONNECTION_CNT 339

typedef enum Input{
    Value,
    Cable,
    Or,
    And,
    Rshift,
    Lshift,
    Not
} Input;

typedef struct Connection{
    Input type;
    unsigned short value;
    int in1;
    int in2;
    int out;
} Connection;

int part1(FILE *in);
int part2(FILE *in);
int identify(char *cable);
unsigned short getValue(int id, Connection *all);
Connection get(int id, Connection *all);
Connection *getConnections(FILE *in);
Connection parse(char *line);
int isdigit(int c);

unsigned short knownValues[(26+1) * (26+1)];
int known[(26+1) * (26+1)] = {0};

int main()
{
    FILE *in = fopen("in7", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Connection *connections = getConnections(in);
    unsigned short res = getValue(identify("a"), connections);
    free(connections);
    return res;
}

int part2(FILE *in)
{
    Connection *connections = getConnections(in);
    unsigned short res = getValue(identify("a"), connections);
    memset(known, 0, (26+1) * (26+1) * sizeof(*known));
    for (int i = 0; i < CONNECTION_CNT; i++)
        if (connections[i].out == identify("b"))
        {
            connections[i].type = Value;
            connections[i].value = res;
        }
    res = getValue(identify("a"), connections);
    free(connections);
    return res;
}

Connection *getConnections(FILE *in)
{
    size_t n = 0;
    char *line = NULL;
    Connection *connections = calloc(CONNECTION_CNT, sizeof(*connections));
    int i = 0;
    while (getline(&line, &n, in) > 0 && *line != '\n')
        connections[i++] = parse(line);
    free(line);
    return connections;
}

Connection parse(char *line)
{
    int d;
    Connection res = {0};
    char a[10], b[10], c[10];
    if (strstr(line, "OR") != NULL)
        res.type = Or;
    else if(strstr(line, "AND") != NULL)
        res.type = And;
    else if(strstr(line, "NOT") != NULL)
        res.type = Not;
    else if(strstr(line, "RSHIFT") != NULL)
        res.type = Rshift;
    else if(strstr(line, "LSHIFT") != NULL)
        res.type = Lshift;
    else if(isdigit(line[0]))
        res.type = Value;
    else
        res.type = Cable;
    switch(res.type)
    {
        case Or:
            if ((d = sscanf(line, "%s OR %s -> %s\n", a, b, c)) != 3)
            {
                fprintf(stderr, "Couldn't parse line %s (%d)\n", line, d);
                exit(3);
            }
            break;
        case And:
            if ((d = sscanf(line, "%s AND %s -> %s\n", a, b, c)) != 3)
            {
                fprintf(stderr, "Couldn't parse line %s (%d)\n", line, d);
                exit(3);
            }
            break;
        case Not:
            if ((d = sscanf(line, "NOT %s -> %s\n", a, c)) != 2)
            {
                fprintf(stderr, "Couldn't parse line %s (%d)\n", line, d);
                exit(3);
            }
            break;
        case Rshift:
            if ((d = sscanf(line, "%s RSHIFT %s -> %s\n", a, b, c)) != 3)
            {
                fprintf(stderr, "Couldn't parse line %s (%d)\n", line, d);
                exit(3);
            }
            break;
        case Lshift:
            if ((d = sscanf(line, "%s LSHIFT %s -> %s\n", a, b, c)) != 3)
            {
                fprintf(stderr, "Couldn't parse line %s (%d)\n", line, d);
                exit(3);
            }
            break;
        case Value:
            if ((d = sscanf(line, "%s -> %s\n", a, c)) != 2)
            {
                fprintf(stderr, "Couldn't parse line %s (%d)\n", line, d);
                exit(3);
            }
            break;
        case Cable:
            if ((d = sscanf(line, "%s -> %s\n", a, c)) != 2)
            {
                fprintf(stderr, "Couldn't parse line %s (%d)\n", line, d);
                exit(3);
            }
            break;
    }
    if (isdigit(*a))
        res.value = atoi(a);
    else
        res.in1 = identify(a);
    if (isdigit(*b))
        res.value = atoi(b);
    else
        res.in2 = identify(b);
    res.out = identify(c);

    //printf("Parsed %s to %d - (%d,%d;%d) -> %d\n", line, res.type, res.in1, res.in2, res.value, res.out);
    return res;
}

int identify(char *cable)
{
    if (cable == NULL || strlen(cable) == 0)
        return 0;
    else if (strlen(cable) == 1)
        return cable[0]-'a'+1;
    else
        return (cable[0]-'a'+1) * 26 + cable[1]-'a'+1;
}

unsigned short getValue(int id, Connection *all)
{
    if (known[id])
        return knownValues[id];
    unsigned short res;
    Connection conn = get(id, all);
    switch(conn.type)
    {
        case Value:
            res = conn.value;
            break;
        case Cable:
            res = getValue(conn.in1, all);
            break;
        case Or:
            if (conn.value != 0)
                if (conn.in1 != 0)
                    res = conn.value | getValue(conn.in1, all);
                else
                    res = conn.value | getValue(conn.in2, all);
            else
                res = getValue(conn.in1, all) | getValue(conn.in2, all);
            break;
        case And:
            if (conn.value != 0)
                if (conn.in1 != 0)
                    res = conn.value & getValue(conn.in1, all);
                else
                    res = conn.value & getValue(conn.in2, all);
            else
                res = getValue(conn.in1, all) & getValue(conn.in2, all);
            break;
        case Rshift:
            res = getValue(conn.in1, all) >> conn.value;
            break;
        case Lshift:
            res = getValue(conn.in1, all) << conn.value;
            break;
        case Not:
            res = ~getValue(conn.in1, all);
            break;
        default:
            fprintf(stderr, "Invalid type: %d\n", conn.type);
            exit(1);
    }
    knownValues[id] = res;
    known[id] = 1;
    //printf("Getting value of %d with type %d -> %d\n", id, conn.type, res);
    return res;
}

Connection get(int id, Connection *all)
{
    for (int i = 0; i < CONNECTION_CNT; i++)
        if (all[i].out == id)
            return all[i];
    fprintf(stderr, "Couldn't identify %d\n", id);
    exit(2);
}

int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

