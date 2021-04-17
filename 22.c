#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>

#define HITPOINTS_STRING "Hit Points: "
#define DAMAGE_STRING "Damage: "

typedef enum Effect
{
    MagicMissile,
    Drain,
    Shield,
    Poison,
    Recharge
} Effect;

typedef struct State
{
    int hard;
    int enemyHitpoints;
    int enemyDamage;
    int playerHitpoints;
    int playerArmor; // Is either 7 or 0
    int mana;
    int effectActivations[Recharge+1]; // Negative means never got activated
} State;

int Cost[] =
{
    53,     // Magic Missile
    73,     // Drain
    113,    // Shield
    173,    // Poison
    229     // Recharge
};

int Durations[] =
{
    1,      // Magic Missile
    1,      // Drain
    6,      // Shield
    6,      // Poison
    5       // Recharge
};

int part1(FILE *in);
int part2(FILE *in);
State getState(FILE *in, int hard);
int getMinimumSpentManaForWin(State state, int round, int leftToSpend);
void applyEffects(State *state, int round);

int main()
{
    FILE *in = fopen("in22", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    State state = getState(in, 0);
    return getMinimumSpentManaForWin(state, 0, INT_MAX);
}

int part2(FILE *in)
{
    State state = getState(in, 1);
    return getMinimumSpentManaForWin(state, 0, INT_MAX);
}

State getState(FILE *in, int hard)
{
    State res = {0};
    res.hard = hard;

    size_t n = 0;
    char *line = NULL;
    int read = getline(&line, &n, in);
    if (read <= strlen(HITPOINTS_STRING))
    {
        fprintf(stderr, "Invalid input\n");
        free(line);
        return res;
    }
    res.enemyHitpoints = atoi(line + strlen(HITPOINTS_STRING));
    read = getline(&line, &n, in);
    if (read <= strlen(DAMAGE_STRING))
    {
        fprintf(stderr, "Invalid input\n");
        free(line);
        return res;
    }
    res.enemyDamage = atoi(line + strlen(DAMAGE_STRING));
    free(line);

    res.playerHitpoints = 50;
    res.mana = 500;
    for (Effect effect = MagicMissile; effect <= Recharge; effect++)
        res.effectActivations[effect] = INT_MIN / 2;

    return res;
}

int getMinimumSpentManaForWin(State state, int round, int leftToSpend)
{
    int minimum = INT_MAX / 2;
    applyEffects(&state, round);

    if (state.playerHitpoints <= 0)
        return minimum; // Player killed by hardness
    if (state.enemyHitpoints <= 0)
        return 0; // Enemy killed by past effects

    if (round % 2 != 0)
    {
        // Enemys turn
        state.playerHitpoints -= state.enemyDamage - state.playerArmor;
        return state.playerHitpoints <= 0
            ? minimum
            : getMinimumSpentManaForWin(state, round+1, leftToSpend);
    }

    // Players turn
    for (Effect effect = MagicMissile; effect <= Recharge; effect++)
    {
        if (state.effectActivations[effect] > round - Durations[effect])
            continue;
        if (Cost[effect] >= minimum || Cost[effect] >= leftToSpend || Cost[effect] > state.mana)
            continue;

        int prevEffectActivation = state.effectActivations[effect];
        state.effectActivations[effect] = round;
        state.mana -= Cost[effect];
        int newLeftToSpend = leftToSpend < minimum
            ? leftToSpend - Cost[effect]
            : minimum - Cost[effect];

        int curr = Cost[effect] + getMinimumSpentManaForWin(state, round+1, newLeftToSpend);

        state.effectActivations[effect] = prevEffectActivation;
        state.mana += Cost[effect];

        if (curr < minimum)
            minimum = curr;
    }
    return minimum;
}

void applyEffects(State *state, int round)
{
    if (state->hard && round % 2 == 0)
        state->playerHitpoints--;

    if (state->playerHitpoints <= 0)
        // Player has already died
        return;

    if (state->effectActivations[MagicMissile] >= round - Durations[MagicMissile])
        state->enemyHitpoints -= 4;

    if (state->effectActivations[Drain] >= round - Durations[Drain])
    {
        state->enemyHitpoints -= 2;
        state->playerHitpoints += 2;
    }

    if (state->effectActivations[Shield] >= round - Durations[Shield])
        state->playerArmor = 7;
    else
        state->playerArmor = 0;

    if (state->effectActivations[Poison] >= round - Durations[Poison])
        state->enemyHitpoints -= 3;

    if (state->effectActivations[Recharge] >= round - Durations[Recharge])
        state->mana += 101;
}

