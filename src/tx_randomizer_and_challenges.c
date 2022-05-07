#include "global.h"
#include "event_data.h"
#include "item.h"
#include "tx_randomizer_and_challenges.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "constants/items.h"
#include "constants/party_menu.h"
#include "constants/region_map_sections.h"

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

bool8 IsDifficultyOptionsActivated(void)
{
    if (gSaveBlock1Ptr->tx_Challenges_PartyLimit
        || gSaveBlock1Ptr->tx_Challenges_LevelCap
        || gSaveBlock1Ptr->tx_Challenges_ExpMultiplier
        || gSaveBlock1Ptr->tx_Challenges_NoItemPlayer
        || gSaveBlock1Ptr->tx_Challenges_NoItemTrainer
        || gSaveBlock1Ptr->tx_Challenges_PkmnCenter)
        return TRUE;

    return FALSE;
}

bool8 IsChallengesActivated(void)
{
    if (gSaveBlock1Ptr->tx_Challenges_EvoLimit
        || gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge != TX_CHALLENGE_TYPE_OFF
        || gSaveBlock1Ptr->tx_Challenges_BaseStatEqualizer
        || gSaveBlock1Ptr->tx_Challenges_Mirror
        || gSaveBlock1Ptr->tx_Challenges_Mirror_Thief)
        return TRUE;

    return FALSE;
}

bool8 IsNuzlockeActive(void)
{
    if (!FlagGet(FLAG_SYS_POKEMON_GET))     //Nuzlocke has not started
        return FALSE;
    if (!FlagGet(FLAG_ADVENTURE_STARTED))   //Nuzlocke has not started
        return FALSE;
    if (FlagGet(FLAG_IS_CHAMPION))          //Player is champion and Nuzlocke stopped
        return FALSE;

    return gSaveBlock1Ptr->tx_Challenges_Nuzlocke;
}

bool8 IsNuzlockeNicknamingActive(void)
{
    if (!gSaveBlock1Ptr->tx_Challenges_Nuzlocke)
        return FALSE;
    if (FlagGet(FLAG_IS_CHAMPION))
        return FALSE;

    return gSaveBlock1Ptr->tx_Nuzlocke_Nicknaming;
}

bool8 IsPokecenterChallengeActivated(void)
{
    return gSaveBlock1Ptr->tx_Challenges_PkmnCenter;
}

bool8 HMsOverwriteOptionActive(void)
{
    return (gSaveBlock1Ptr->tx_Challenges_Nuzlocke 
            || gSaveBlock1Ptr->tx_Challenges_Mirror 
            || gSaveBlock1Ptr->tx_Random_Moves
            || gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge);
}

// Nuzlocke code
const u8 NuzlockeLUT[] =
{
    //0
    [MAPSEC_ROUTE_101] = 0x0,
    [MAPSEC_ROUTE_102] = 0x1,
    [MAPSEC_ROUTE_103] = 0x2,
    [MAPSEC_ROUTE_104] = 0x3,
    [MAPSEC_ROUTE_105] = 0x4,
    [MAPSEC_ROUTE_106] = 0x5,
    [MAPSEC_ROUTE_107] = 0x6,
    [MAPSEC_ROUTE_108] = 0x7,
    //1
    [MAPSEC_ROUTE_109] = 0x8,
    [MAPSEC_ROUTE_110] = 0x9,
    [MAPSEC_ROUTE_111] = 0xA,
    [MAPSEC_ROUTE_112] = 0xB,
    [MAPSEC_ROUTE_113] = 0xC,
    [MAPSEC_ROUTE_114] = 0xD,
    [MAPSEC_ROUTE_115] = 0xE,
    [MAPSEC_ROUTE_116] = 0xF,
    //2
    [MAPSEC_ROUTE_117] = 0x10,
    [MAPSEC_ROUTE_118] = 0x11,
    [MAPSEC_ROUTE_119] = 0x12,
    [MAPSEC_ROUTE_120] = 0x13,
    [MAPSEC_ROUTE_121] = 0x14,
    [MAPSEC_ROUTE_122] = 0x15,
    [MAPSEC_ROUTE_123] = 0x16,
    [MAPSEC_ROUTE_124] = 0x17,
    //3
    [MAPSEC_ROUTE_125] = 0x18,
    [MAPSEC_ROUTE_126] = 0x19,
    [MAPSEC_ROUTE_127] = 0x1A,
    [MAPSEC_ROUTE_128] = 0x1B,
    [MAPSEC_ROUTE_129] = 0x1C,
    [MAPSEC_ROUTE_130] = 0x1D,
    [MAPSEC_ROUTE_131] = 0x1E,
    [MAPSEC_ROUTE_132] = 0x1F,
    //4
    [MAPSEC_ROUTE_133] = 0x20,
    [MAPSEC_ROUTE_134] = 0x21,
    [MAPSEC_PETALBURG_CITY] = 0x22,
    [MAPSEC_DEWFORD_TOWN] = 0x23,
    [MAPSEC_SLATEPORT_CITY] = 0x24,
    [MAPSEC_LILYCOVE_CITY] = 0x25,
    [MAPSEC_MOSSDEEP_CITY] = 0x26,
    [MAPSEC_PACIFIDLOG_TOWN] = 0x27,
    //5
    [MAPSEC_SOOTOPOLIS_CITY] = 0x28,
    [MAPSEC_EVER_GRANDE_CITY] = 0x29,
    [MAPSEC_PETALBURG_WOODS] = 0x2A,
    [MAPSEC_RUSTURF_TUNNEL] = 0x2B,
    [MAPSEC_GRANITE_CAVE] = 0x2C,
    [MAPSEC_FIERY_PATH] = 0x2D,
    [MAPSEC_METEOR_FALLS] = 0x2E,
    [MAPSEC_JAGGED_PASS] = 0x2F,
    //6
    [MAPSEC_MIRAGE_TOWER] = 0x30,
    [MAPSEC_ABANDONED_SHIP] = 0x31,
    [MAPSEC_NEW_MAUVILLE] = 0x32,
    [MAPSEC_SAFARI_ZONE_AREA1] = 0x33,
    [MAPSEC_SAFARI_ZONE_AREA2] = 0x34,
    [MAPSEC_SAFARI_ZONE_AREA3] = 0x35,
    [MAPSEC_SAFARI_ZONE_AREA4] = 0x36,
    [MAPSEC_MT_PYRE] = 0x37,
    //7
    [MAPSEC_SHOAL_CAVE] = 0x38,
    [MAPSEC_AQUA_HIDEOUT] = 0x39,
    [MAPSEC_MAGMA_HIDEOUT] = 0x3A,
    [MAPSEC_SEAFLOOR_CAVERN] = 0x3B,
    [MAPSEC_CAVE_OF_ORIGIN] = 0x3C,
    [MAPSEC_SKY_PILLAR] = 0x3D,
    [MAPSEC_VICTORY_ROAD] = 0x3E,
    [MAPSEC_UNDERWATER_124] = 0x3F,
    //8
    [MAPSEC_UNDERWATER_126] = 0x3F,
    [MAPSEC_ARTISAN_CAVE] = 0x40,
    [MAPSEC_DESERT_UNDERPASS] = 0x41,
    [MAPSEC_ALTERING_CAVE_FRLG] = 0x42,
    [MAPSEC_SAFARI_ZONE_AREA5] = 0x43,
    [MAPSEC_SAFARI_ZONE_AREA6] = 0x44
};

//tx_randomizer_and_challenges
u8 NuzlockeFlagSet(u16 mapsec) // @Kurausukun
{
    u8 id = NuzlockeLUT[mapsec];
    u8 * ptr = &gSaveBlock1Ptr->NuzlockeEncounterFlags[id / 8];
    u8 i;
    if (ptr)
        * ptr |= 1 << (id & 7);

    #ifdef GBA_PRINTF
    mgba_printf(MGBA_LOG_DEBUG, "NuzlockeFlagSet Id=%d", id);
    for (i=0; i<9; i++)
        mgba_printf(MGBA_LOG_DEBUG, "gSaveBlock1Ptr->NuzlockeEncounterFlags[%d] = %d" , i, gSaveBlock1Ptr->NuzlockeEncounterFlags[i]);
    #endif

    return 0;
}
u8 NuzlockeFlagClear(u16 mapsec) // @Kurausukun
{
    u8 id = NuzlockeLUT[mapsec];
    u8 * ptr = &gSaveBlock1Ptr->NuzlockeEncounterFlags[id / 8];
    if (ptr)
        * ptr &= ~(1 << (id & 7));
    return 0;
}
u8 NuzlockeFlagGet(u16 mapsec) // @Kurausukun
{
    u8 id = NuzlockeLUT[mapsec];
    u8 * ptr = &gSaveBlock1Ptr->NuzlockeEncounterFlags[id / 8];

    #ifdef GBA_PRINTF
    mgba_printf(MGBA_LOG_DEBUG, "NuzlockeFlagGet Id=%d", id);
    #endif

    if (!ptr)
        return 0;

    if (!(((*ptr) >> (id & 7)) & 1))
        return 0;
    return 1;
}

void NuzlockeDeletePartyMon(u8 position)
{
    if (!gSaveBlock1Ptr->tx_Nuzlocke_Deletion)
    {
        struct Pokemon *pokemon = &gPlayerParty[position];
        u8 val[1] = {TRUE};
        
        SetMonData(pokemon, MON_DATA_NUZLOCKE_RIBBON, val);
        SendMonToPC(&gPlayerParty[position]);
    }
    PurgeMonOrBoxMon(TOTAL_BOXES_COUNT, position);
}
void NuzlockeDeleteFaintedPartyPokemon(void) // @Kurausukun
{
    u8 i;
    struct Pokemon *pokemon;
    u32 monItem;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        pokemon = &gPlayerParty[i];
        if (GetMonData(pokemon, MON_DATA_SANITY_HAS_SPECIES, NULL) && !GetMonData(pokemon, MON_DATA_IS_EGG, NULL))
        {
            if (GetMonAilment(pokemon) == AILMENT_FNT)
            {
                monItem = GetMonData(pokemon, MON_DATA_HELD_ITEM, NULL);

                if (monItem != ITEM_NONE)
                    AddBagItem(monItem, 1);
                NuzlockeDeletePartyMon(i);
            }
        }
    }
    CompactPartySlots();
}

// Difficulty
u8 GetMaxPartySize(void)
{
    return (6 - gSaveBlock1Ptr->tx_Challenges_PartyLimit);
}

u8 GetCurrentBadgeCount(void)
{
    u16 i, badgeCount = 0;
    for (i = FLAG_BADGE01_GET; i < FLAG_BADGE01_GET + NUM_BADGES; i++) //count badges
    {
        if (FlagGet(i))
            badgeCount++;
    }
    return badgeCount;
}

enum LevelCap {
    LEVEL_CAP_NO_BADGES,
    LEVEL_CAP_BADGE_1,
    LEVEL_CAP_BADGE_2,
    LEVEL_CAP_BADGE_3,
    LEVEL_CAP_BADGE_4,
    LEVEL_CAP_BADGE_5,
    LEVEL_CAP_BADGE_6,
    LEVEL_CAP_BADGE_7,
    LEVEL_CAP_BADGE_8
};
static const u8 sLevelCapTable_Normal[] = 
{
    [LEVEL_CAP_NO_BADGES]   = 15,
    [LEVEL_CAP_BADGE_1]     = 19,
    [LEVEL_CAP_BADGE_2]     = 24,
    [LEVEL_CAP_BADGE_3]     = 29,
    [LEVEL_CAP_BADGE_4]     = 31,
    [LEVEL_CAP_BADGE_5]     = 33,
    [LEVEL_CAP_BADGE_6]     = 42,
    [LEVEL_CAP_BADGE_7]     = 46,
    [LEVEL_CAP_BADGE_8]     = 58,
};
static const u8 sLevelCapTable_Hard[] = 
{
    [LEVEL_CAP_NO_BADGES]   = 12,
    [LEVEL_CAP_BADGE_1]     = 16,
    [LEVEL_CAP_BADGE_2]     = 20,
    [LEVEL_CAP_BADGE_3]     = 24,
    [LEVEL_CAP_BADGE_4]     = 27,
    [LEVEL_CAP_BADGE_5]     = 29,
    [LEVEL_CAP_BADGE_6]     = 41,
    [LEVEL_CAP_BADGE_7]     = 41,
    [LEVEL_CAP_BADGE_8]     = 55,
};
#define TX_CHALLENGE_LEVEL_CAP_DEBUG 0
u8 GetCurrentPartyLevelCap(void)
{
    u8 badgeCount = GetCurrentBadgeCount();

    if (TX_CHALLENGE_LEVEL_CAP_DEBUG != 0) //debug allways overwrites the rest
        return TX_CHALLENGE_LEVEL_CAP_DEBUG;

    if (FlagGet(FLAG_IS_CHAMPION)) //after beating the E4 remove the cap
        return MAX_LEVEL;

    if (gSaveBlock1Ptr->tx_Challenges_LevelCap == 1) //normal level cap
        return sLevelCapTable_Normal[badgeCount];

    if (gSaveBlock1Ptr->tx_Challenges_LevelCap == 2) //hard level cap
        return sLevelCapTable_Hard[badgeCount];

    return MAX_LEVEL;
}

// Scaling IVs and EVs
static const u8 sIV_Table[] = 
{
    [LEVEL_CAP_NO_BADGES]   = 7,
    [LEVEL_CAP_BADGE_1]     = 10,
    [LEVEL_CAP_BADGE_2]     = 13,
    [LEVEL_CAP_BADGE_3]     = 16,
    [LEVEL_CAP_BADGE_4]     = 19,
    [LEVEL_CAP_BADGE_5]     = 22,
    [LEVEL_CAP_BADGE_6]     = 25,
    [LEVEL_CAP_BADGE_7]     = 28,
    [LEVEL_CAP_BADGE_8]     = 31,
};
static const u8 sEV_Table[] = 
{
    [LEVEL_CAP_NO_BADGES]   = 12,
    [LEVEL_CAP_BADGE_1]     = 24,
    [LEVEL_CAP_BADGE_2]     = 36,
    [LEVEL_CAP_BADGE_3]     = 48,
    [LEVEL_CAP_BADGE_4]     = 60,
    [LEVEL_CAP_BADGE_5]     = 72,
    [LEVEL_CAP_BADGE_6]     = 80,
    [LEVEL_CAP_BADGE_7]     = 100,
    [LEVEL_CAP_BADGE_8]     = 128,
};
u8 GetCurrentTrainerIVs(void)
{
    u8 badgeCount = GetCurrentBadgeCount();

    switch (gSaveBlock1Ptr->tx_Challenges_TrainerScalingIVs)
    {
    case 1:     return sIV_Table[badgeCount];
    default:    return MAX_PER_STAT_IVS;
    }
}
u8 GetCurrentTrainerEVs(void)
{
    u8 badgeCount = GetCurrentBadgeCount();

    switch (gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs)
    {
    case 1:     return sEV_Table[badgeCount];
    case 2:     return 128;
    case 3:     return 252;
    default:    return 0;
    }
}

// Challenges


// DEBUG
void PrintTXSaveData(void)
{
    #ifdef GBA_PRINTF
    
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_WildPokemon"          , gSaveBlock1Ptr->tx_Random_WildPokemon);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_Trainer"              , gSaveBlock1Ptr->tx_Random_Trainer);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_Similar"              , gSaveBlock1Ptr->tx_Random_Similar);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_MapBased"             , gSaveBlock1Ptr->tx_Random_MapBased);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_IncludeLegendaries"   , gSaveBlock1Ptr->tx_Random_IncludeLegendaries);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_Type"                 , gSaveBlock1Ptr->tx_Random_Type);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_Moves"                , gSaveBlock1Ptr->tx_Random_Moves);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_Abilities"            , gSaveBlock1Ptr->tx_Random_Abilities);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_Evolutions"           , gSaveBlock1Ptr->tx_Random_Evolutions);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_EvolutionMethods"     , gSaveBlock1Ptr->tx_Random_EvolutionMethods);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_TypeEffectiveness"    , gSaveBlock1Ptr->tx_Random_TypeEffectiveness);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_Items"                , gSaveBlock1Ptr->tx_Random_Items);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_Chaos"                , gSaveBlock1Ptr->tx_Random_Chaos);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Random_OneForOne"            , gSaveBlock1Ptr->tx_Random_OneForOne);

    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_Nuzlocke"         , gSaveBlock1Ptr->tx_Challenges_Nuzlocke);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_NuzlockeHardcore" , gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Nuzlocke_SpeciesClause"      , gSaveBlock1Ptr->tx_Nuzlocke_SpeciesClause);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Nuzlocke_ShinyClause"        , gSaveBlock1Ptr->tx_Nuzlocke_ShinyClause);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Nuzlocke_Nicknaming"         , gSaveBlock1Ptr->tx_Nuzlocke_Nicknaming);

    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_PartyLimit"       , gSaveBlock1Ptr->tx_Challenges_PartyLimit);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_LevelCap"         , gSaveBlock1Ptr->tx_Challenges_LevelCap);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_ExpMultiplier"    , gSaveBlock1Ptr->tx_Challenges_ExpMultiplier);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_NoItemPlayer"     , gSaveBlock1Ptr->tx_Challenges_NoItemPlayer);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_NoItemTrainer"    , gSaveBlock1Ptr->tx_Challenges_NoItemTrainer);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_PkmnCenter"       , gSaveBlock1Ptr->tx_Challenges_PkmnCenter);

    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_EvoLimit"         , gSaveBlock1Ptr->tx_Challenges_EvoLimit);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_OneTypeChallenge" , gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_BaseStatEqualizer", gSaveBlock1Ptr->tx_Challenges_BaseStatEqualizer);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_Mirror"           , gSaveBlock1Ptr->tx_Challenges_Mirror);
    mgba_printf(MGBA_LOG_DEBUG, "%d tx_Challenges_Mirror_Thief"     , gSaveBlock1Ptr->tx_Challenges_Mirror_Thief);
    #endif
}

void TestRandomizerValues(u8 type)
{
    #ifdef GBA_PRINTF
    u16 i, j;
    u8 real_j;
    u16 tmp;
    u16 array[10];
    u8 save_values[30];

    //save saveblock values
    save_values[0]  = gSaveBlock1Ptr->tx_Random_Chaos;
    save_values[1]  = gSaveBlock1Ptr->tx_Random_WildPokemon;
    save_values[2]  = gSaveBlock1Ptr->tx_Random_Similar;
    save_values[3]  = gSaveBlock1Ptr->tx_Random_MapBased;
    save_values[4]  = gSaveBlock1Ptr->tx_Random_IncludeLegendaries;
    save_values[5]  = gSaveBlock1Ptr->tx_Random_Type;
    save_values[6]  = gSaveBlock1Ptr->tx_Random_TypeEffectiveness;
    save_values[7]  = gSaveBlock1Ptr->tx_Random_Abilities;
    save_values[8]  = gSaveBlock1Ptr->tx_Random_Moves;
    save_values[9]  = gSaveBlock1Ptr->tx_Random_Trainer;
    save_values[10] = gSaveBlock1Ptr->tx_Random_Evolutions;
    save_values[11] = gSaveBlock1Ptr->tx_Random_EvolutionMethods;
    save_values[12] = gSaveBlock1Ptr->tx_Challenges_EvoLimit;
    save_values[13] = gSaveBlock1Ptr->tx_Challenges_Nuzlocke;
    save_values[14] = gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore;
    save_values[15] = gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge;
    save_values[16] = gSaveBlock1Ptr->tx_Challenges_PartyLimit;
    save_values[17] = gSaveBlock1Ptr->tx_Challenges_NoItemPlayer;
    save_values[18] = gSaveBlock1Ptr->tx_Challenges_NoItemTrainer;
    save_values[19] = gSaveBlock1Ptr->tx_Challenges_PkmnCenter;
    save_values[20] = gSaveBlock1Ptr->tx_Random_OneForOne;
    save_values[21] = gSaveBlock1Ptr->tx_Challenges_BaseStatEqualizer;
    save_values[22] = gSaveBlock1Ptr->tx_Challenges_LevelCap;
    save_values[23] = gSaveBlock1Ptr->tx_Random_Items;
    save_values[24] = gSaveBlock1Ptr->tx_Nuzlocke_SpeciesClause;
    save_values[25] = gSaveBlock1Ptr->tx_Nuzlocke_ShinyClause;
    save_values[26] = gSaveBlock1Ptr->tx_Nuzlocke_Nicknaming;

    gSaveBlock1Ptr->tx_Random_WildPokemon           = TRUE;
    gSaveBlock1Ptr->tx_Random_Similar               = FALSE;
    gSaveBlock1Ptr->tx_Random_MapBased              = FALSE;
    gSaveBlock1Ptr->tx_Random_IncludeLegendaries    = FALSE;


    for (i=0; i<10; i++)
    {
        for (j=0; j<NUM_SPECIES; j++)
        {
        //tmp = RandomSeededModulo(j, NUM_SPECIES);
        tmp = GetSpeciesRandomSeeded(j, type);
        
        real_j = j % 10;
        array[real_j] = tmp;
        if (real_j == 9)
            mgba_printf(MGBA_LOG_DEBUG, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,", array[0],array[1],array[2],array[3],array[4],array[5],array[6],array[7],array[8],array[9]);
        }
    }




    //restore saveblock values
    gSaveBlock1Ptr->tx_Random_Chaos                 =   save_values[0];
    gSaveBlock1Ptr->tx_Random_WildPokemon           =   save_values[1];
    gSaveBlock1Ptr->tx_Random_Similar               =   save_values[2];
    gSaveBlock1Ptr->tx_Random_MapBased              =   save_values[3];
    gSaveBlock1Ptr->tx_Random_IncludeLegendaries    =   save_values[4];
    gSaveBlock1Ptr->tx_Random_Type                  =   save_values[5];
    gSaveBlock1Ptr->tx_Random_TypeEffectiveness     =   save_values[6];
    gSaveBlock1Ptr->tx_Random_Abilities             =   save_values[7];
    gSaveBlock1Ptr->tx_Random_Moves                 =   save_values[8];
    gSaveBlock1Ptr->tx_Random_Trainer               =   save_values[9];
    gSaveBlock1Ptr->tx_Random_Evolutions            =   save_values[10];
    gSaveBlock1Ptr->tx_Random_EvolutionMethods      =   save_values[11];
    gSaveBlock1Ptr->tx_Challenges_EvoLimit          =   save_values[12];
    gSaveBlock1Ptr->tx_Challenges_Nuzlocke          =   save_values[13];
    gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore  =   save_values[14];
    gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge  =   save_values[15];
    gSaveBlock1Ptr->tx_Challenges_PartyLimit        =   save_values[16];
    gSaveBlock1Ptr->tx_Challenges_NoItemPlayer      =   save_values[17];
    gSaveBlock1Ptr->tx_Challenges_NoItemTrainer     =   save_values[18];
    gSaveBlock1Ptr->tx_Challenges_PkmnCenter        =   save_values[19];
    gSaveBlock1Ptr->tx_Random_OneForOne             =   save_values[20];
    gSaveBlock1Ptr->tx_Challenges_BaseStatEqualizer =   save_values[21];
    gSaveBlock1Ptr->tx_Challenges_LevelCap          =   save_values[22];
    gSaveBlock1Ptr->tx_Random_Items                 =   save_values[23];
    gSaveBlock1Ptr->tx_Nuzlocke_SpeciesClause       =   save_values[24];
    gSaveBlock1Ptr->tx_Nuzlocke_ShinyClause         =   save_values[25];
    gSaveBlock1Ptr->tx_Nuzlocke_Nicknaming          =   save_values[26];
    #endif
}
