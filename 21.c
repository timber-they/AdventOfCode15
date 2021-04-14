#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define HITPOINTS_STRING "Hit Points: "
#define DAMAGE_STRING "Damage: "
#define ARMOR_STRING "Armor: "

typedef struct Item
{
    int cost;
    int damage;
    int armor;
} Item;

typedef struct Equipment
{
    Item weapon;
    Item armor;
    Item ring1;
    Item ring2;
} Equipment;

typedef struct Player
{
    int hitPoints;
    int damage;
    int armor;
} Player;

int part1(FILE *in);
int part2(FILE *in);
Player getEnemy(FILE *in);
Player constructPlayer(Player naked, Equipment equipment);
int playerWins(Player player, Player enemy);
int getLeastWinningGold(Player nakedPlayer, Player enemy);
int getMostLosingGold(Player nakedPlayer, Player enemy);

Item Weapons[] =
{
    (Item) {.cost = 8, .damage = 4, .armor = 0},
    (Item) {.cost = 10, .damage = 5, .armor = 0},
    (Item) {.cost = 25, .damage = 6, .armor = 0},
    (Item) {.cost = 40, .damage = 7, .armor = 0},
    (Item) {.cost = 74, .damage = 8, .armor = 0}
};

Item Armor[] =
{
    (Item) {.cost = 13, .damage = 0, .armor = 1},
    (Item) {.cost = 31, .damage = 0, .armor = 2},
    (Item) {.cost = 53, .damage = 0, .armor = 3},
    (Item) {.cost = 75, .damage = 0, .armor = 4},
    (Item) {.cost = 102, .damage = 0, .armor = 5},
    (Item) {.cost = 0, .damage = 0, .armor = 0} // No armor
};

Item Rings[] =
{
    (Item) {.cost = 25, .damage = 1, .armor = 0},
    (Item) {.cost = 50, .damage = 2, .armor = 0},
    (Item) {.cost = 100, .damage = 3, .armor = 0},
    (Item) {.cost = 20, .damage = 0, .armor = 1},
    (Item) {.cost = 40, .damage = 0, .armor = 2},
    (Item) {.cost = 80, .damage = 0, .armor = 3},
    (Item) {.cost = 0, .damage = 0, .armor = 0}, // No first ring
    (Item) {.cost = 0, .damage = 0, .armor = 0}, // No second ring
};

int main()
{
    FILE *in = fopen("in21", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Player enemy = getEnemy(in);
    Player player = (Player) {.hitPoints = 100, .armor = 0, .damage = 0};
    return getLeastWinningGold(player, enemy);
}

int part2(FILE *in)
{
    Player enemy = getEnemy(in);
    Player player = (Player) {.hitPoints = 100, .armor = 0, .damage = 0};
    return getMostLosingGold(player, enemy);
}

Player getEnemy(FILE *in)
{
    Player enemy = {0};
    size_t n = 0;
    char *line = NULL;
    int read = getline(&line, &n, in);
    if (read <= strlen(HITPOINTS_STRING))
    {
        fprintf(stderr, "Invalid input\n");
        return enemy;
    }
    enemy.hitPoints = atoi(line + strlen(HITPOINTS_STRING));
    read = getline(&line, &n, in);
    if (read <= strlen(DAMAGE_STRING))
    {
        fprintf(stderr, "Invalid input\n");
        return enemy;
    }
    enemy.damage = atoi(line + strlen(DAMAGE_STRING));
    read = getline(&line, &n, in);
    if (read <= strlen(ARMOR_STRING))
    {
        fprintf(stderr, "Invalid input\n");
        return enemy;
    }
    enemy.armor = atoi(line + strlen(ARMOR_STRING));
    free(line);

    return enemy;
}

Player constructPlayer(Player naked, Equipment equipment)
{
    return (Player)
    {
        .hitPoints = naked.hitPoints,
        .damage = naked.damage +
            equipment.weapon.damage +
            equipment.armor.damage +
            equipment.ring1.damage +
            equipment.ring2.damage,
        .armor = naked.armor +
            equipment.weapon.armor +
            equipment.armor.armor +
            equipment.ring1.armor +
            equipment.ring2.armor
    };
}

int playerWins(Player player, Player enemy)
{
    for (int turn = 0; player.hitPoints > 0 && enemy.hitPoints > 0; turn++)
        if (turn % 2 == 0)
            enemy.hitPoints -= player.damage - enemy.armor > 0
                ? player.damage - enemy.armor
                : 1;
        else
            player.hitPoints -= enemy.damage - player.armor > 0
                ? enemy.damage - player.armor
                : 1;
    return enemy.hitPoints <= 0;
}

int getLeastWinningGold(Player nakedPlayer, Player enemy)
{
    int minGold = -1;
    for (int i = 0; i < sizeof(Weapons) / sizeof(*Weapons); i++)
    for (int j = 0; j < sizeof(Armor) / sizeof(*Armor); j++)
    for (int k = 0; k < sizeof(Rings) / sizeof(*Rings); k++)
    for (int l = 0; l < sizeof(Rings) / sizeof(*Rings); l++)
    {
        if (k == l)
            continue;
        Equipment equipment = (Equipment)
        {
            .weapon = Weapons[i],
            .armor = Armor[j],
            .ring1 = Rings[k],
            .ring2 = Rings[l]
        };
        int requiredGold = equipment.weapon.cost +
            equipment.armor.cost +
            equipment.ring1.cost +
            equipment.ring2.cost;
        if (minGold >= 0 && requiredGold >= minGold)
            continue;
        Player player = constructPlayer(nakedPlayer, equipment);
        if (playerWins(player, enemy))
            minGold = requiredGold;
    }
    return minGold;
}

int getMostLosingGold(Player nakedPlayer, Player enemy)
{
    int mostGold = -1;
    for (int i = 0; i < sizeof(Weapons) / sizeof(*Weapons); i++)
    for (int j = 0; j < sizeof(Armor) / sizeof(*Armor); j++)
    for (int k = 0; k < sizeof(Rings) / sizeof(*Rings); k++)
    for (int l = 0; l < sizeof(Rings) / sizeof(*Rings); l++)
    {
        if (k == l)
            continue;
        Equipment equipment = (Equipment)
        {
            .weapon = Weapons[i],
            .armor = Armor[j],
            .ring1 = Rings[k],
            .ring2 = Rings[l]
        };
        int requiredGold = equipment.weapon.cost +
            equipment.armor.cost +
            equipment.ring1.cost +
            equipment.ring2.cost;
        if (requiredGold <= mostGold)
            continue;
        Player player = constructPlayer(nakedPlayer, equipment);
        if (!playerWins(player, enemy))
            mostGold = requiredGold;
    }
    return mostGold;
}

