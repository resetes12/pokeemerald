#include "global.h"
#include "save_migration.h"
#include "save.h"
#include "constants/global.h"

// Recognized save versions. Add new entries here as releases ship.
static const u8 sRecognizedVersions[] = {
    ME_SAVE_VERSION_NONE,    // Pre-tracking (3.5 and earlier)
    ME_SAVE_VERSION_3_6,     // 3.6 (first tracked version)
    ME_SAVE_VERSION_3_7,     // 3.7 (Soul Link run metadata)
    ME_SAVE_VERSION_3_8,     // 3.8 (persistent failed Soul Link encounters)
};

u8 GetSaveVersion(void)
{
    return gSaveBlock2Ptr->saveVersion;
}

bool8 SaveNeedsMigration(void)
{
    return gSaveBlock2Ptr->saveVersion < ME_SAVE_VERSION_CURRENT;
}

bool8 SaveVersionIsRecognized(u8 version)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sRecognizedVersions); i++)
    {
        if (sRecognizedVersions[i] == version)
            return TRUE;
    }
    return FALSE;
}

void StampCurrentSaveVersion(void)
{
    gSaveBlock2Ptr->saveVersion = ME_SAVE_VERSION_CURRENT;
}

// ============================================================
// Pre-checksum sector data migration
// ============================================================
// This is the low-level migration that runs BEFORE checksum validation.
// It operates on raw sector bytes and remaps fields when struct layouts
// change between versions.
//
// HOW TO ADD A NEW MIGRATION:
// 1. Define the new ME_SAVE_VERSION_X_Y in constants/global.h
// 2. Add a static migration function below: MigrateSector_VX_to_VY(data, size)
// 3. Add it to the migration chain in TryMigrateSectorData
// 4. Update ME_SAVE_VERSION_CURRENT
//
// Each migration function takes raw sector data and remaps bytes from
// the old layout to the new layout. Migrations are applied sequentially
// (old → intermediate → ... → current).

// The offset of the version byte within sector 0's raw data.
// This MUST match offsetof(struct SaveBlock2, saveVersion) at all times.
// !! THIS OFFSET IS FROZEN FOREVER !!
// If struct SaveBlock2 changes layout above saveVersion, old saves WILL break
// with no recovery path. The pre-checksum migration cannot find the version
// byte if it moves.
#define SAVE_VERSION_SECTOR0_OFFSET offsetof(struct SaveBlock2, saveVersion)

// Compile-time guarantee that the version byte hasn't moved.
// If you need to move it, you CANNOT — this is the one field that is frozen.
STATIC_ASSERT(offsetof(struct SaveBlock2, saveVersion) < SECTOR_DATA_SIZE, SaveVersionFitsInSector);

// Old SaveBlock2 size used by all ME versions from 2.4 through 3.5.
// These versions had identical struct layouts (fields only appended after this point).
#define SAVEBLOCK2_SIZE_V24_TO_V35 0xF2C
#define SAVEBLOCK2_SIZE_V36 offsetof(struct SaveBlock2, soulLink)
#define SAVEBLOCK2_SIZE_V37 (offsetof(struct SaveBlock2, soulLink) + 16)

u16 GetHistoricalSaveBlock2Size(const u8 *data)
{
    switch (data[SAVE_VERSION_SECTOR0_OFFSET])
    {
    case ME_SAVE_VERSION_NONE:
        return SAVEBLOCK2_SIZE_V24_TO_V35;
    case ME_SAVE_VERSION_3_6:
        return SAVEBLOCK2_SIZE_V36;
    case ME_SAVE_VERSION_3_7:
        return SAVEBLOCK2_SIZE_V37;
    default:
        return 0;
    }
}

bool8 TryMigrateSectorData(u8 sectorId, u8 *data, u16 size)
{
    u16 oldSize;

    // Only sector 0 (SaveBlock2) needs old-size fallback for now
    if (sectorId != SECTOR_ID_SAVEBLOCK2)
        return FALSE;

    oldSize = GetHistoricalSaveBlock2Size(data);
    if (oldSize == 0 || oldSize >= size)
        return FALSE;

    // The caller has verified the historical-size checksum. Zero fields that
    // did not exist in that version before accepting it as the current layout.
    {
        u16 j;
        for (j = oldSize; j < size; j++)
            data[j] = 0;
    }

    return TRUE;
}
