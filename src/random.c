#include "global.h"
#include "random.h"
#include "new_game.h" //tx_difficulty_challenges
#include "tx_difficulty_challenges.h"

EWRAM_DATA static u8 sUnknown = 0;
EWRAM_DATA static u32 sRandCount = 0;

// IWRAM common
u32 gRngValue;
u32 gRng2Value;

u16 Random(void)
{
    gRngValue = ISO_RANDOMIZE1(gRngValue);
    sRandCount++;
    return gRngValue >> 16;
}

void SeedRng(u16 seed)
{
    gRngValue = seed;
    sUnknown = 0;
}

void SeedRng2(u16 seed)
{
    gRng2Value = seed;
}

u16 Random2(void)
{
    gRng2Value = ISO_RANDOMIZE1(gRng2Value);
    return gRng2Value >> 16;
}

//tx_difficulty_challenges
u16 RandomSeeded(u16 value, u8 seeded)
{
    u16 otId, result;

    if (TX_RANDOM_CHAOS_MODE && !seeded)
    {
        result = Random();
    }
    else
    {
        otId = GetTrainerId(gSaveBlock2Ptr->playerTrainerId);
        result = ISO_RANDOMIZE1(otId + value) >> 16;
    }
    return result;
}

