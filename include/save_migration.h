#ifndef GUARD_SAVE_MIGRATION_H
#define GUARD_SAVE_MIGRATION_H

#include "constants/global.h"

// Reads the save version from the currently loaded SaveBlock2.
// Returns ME_SAVE_VERSION_NONE (0) for pre-tracking saves.
u8 GetSaveVersion(void);

// Returns TRUE if the loaded save needs migration (version < current).
bool8 SaveNeedsMigration(void);

// Returns TRUE if the loaded save is from a recognized version.
// Unrecognized = junk data or very old save where the byte is garbage.
bool8 SaveVersionIsRecognized(u8 version);

// Stamps the current version into SaveBlock2. Call after migration completes
// or on new game init.
void StampCurrentSaveVersion(void);

// Pre-checksum sector migration. Called from save.c during load.
// Takes raw sector 0 data and its size, peeks at the version byte,
// and if the layout changed between versions, remaps bytes in-place.
// Returns TRUE if data was modified (caller should recalculate checksum).
bool8 TryMigrateSectorData(u8 sectorId, u8 *data, u16 size);

// Returns the old SaveBlock2 struct size for fallback checksum validation.
u16 GetOldSaveBlock2Size(void);

#endif // GUARD_SAVE_MIGRATION_H
