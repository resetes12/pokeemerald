#include "global.h"
#include "tx_randomizer_and_challenges.h"
#include "pokemon.h"

// Generic functions
bool8 IsRandomizerActivated(void)
{
    if (gSaveBlock1Ptr->tx_Random_Chaos
        || gSaveBlock1Ptr->tx_Random_WildPokemon
        || gSaveBlock1Ptr->tx_Random_Similar
        || gSaveBlock1Ptr->tx_Random_MapBased
        || gSaveBlock1Ptr->tx_Random_IncludeLegendaries
        || gSaveBlock1Ptr->tx_Random_Type
        || gSaveBlock1Ptr->tx_Random_TypeEffectiveness
        || gSaveBlock1Ptr->tx_Random_Abilities
        || gSaveBlock1Ptr->tx_Random_Moves
        || gSaveBlock1Ptr->tx_Random_Trainer
        || gSaveBlock1Ptr->tx_Random_Evolutions
        || gSaveBlock1Ptr->tx_Random_EvolutionMethods
        || gSaveBlock1Ptr->tx_Random_OneForOne
        || gSaveBlock1Ptr->tx_Random_Items)
        return TRUE;

    return FALSE;
}

bool8 IsRandomItemsActivated(void)
{
    return gSaveBlock1Ptr->tx_Random_Items;
}

bool8 IsChallengesActivated(void)
{
    if (gSaveBlock1Ptr->tx_Challenges_EvoLimit
        || gSaveBlock1Ptr->tx_Challenges_Nuzlocke
        || gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore
        || gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge != TX_CHALLENGE_TYPE_OFF
        || gSaveBlock1Ptr->tx_Challenges_PartyLimit
        || gSaveBlock1Ptr->tx_Challenges_NoItemPlayer
        || gSaveBlock1Ptr->tx_Challenges_NoItemTrainer
        || gSaveBlock1Ptr->tx_Challenges_PkmnCenter
        || gSaveBlock1Ptr->tx_Challenges_BaseStatEqualizer
        || gSaveBlock1Ptr->tx_Challenges_LevelCap)
        return TRUE;

    return FALSE;
}

bool8 IsNuzlockeActivated(void)
{
    return gSaveBlock1Ptr->tx_Challenges_Nuzlocke;
}

bool8 IsPokecenterChallengeActivated(void)
{
    return gSaveBlock1Ptr->tx_Challenges_PkmnCenter;
}

