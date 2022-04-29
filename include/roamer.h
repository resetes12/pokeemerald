#ifndef GUARD_ROAMER_H
#define GUARD_ROAMER_H

/* Create Latias and Latios roamers as well as 2 terrestrial
/* Pikachu roamers on new game, flag them all as seen on the 
/* Pokedex and give the Pokedex to the player. */
#define MULTIPLE_ROAMERS_EXAMPLE TRUE

void DeactivateAllRoamers(void);
void InitRoamer(void);
void UpdateLocationHistoryForRoamer(void);
void RoamerMoveToOtherLocationSet(u8 id);
void RoamerMove(u8 index);
bool8 IsRoamerAt(u8 id, u8 mapGroup, u8 mapNum);
void CreateRoamerMonInstance(u8 id);
u8 TryStartRoamerEncounter(bool8 isWaterEncounter);
void UpdateRoamerHPStatus(struct Pokemon *mon);
void SetRoamerInactive(u8 index);
void GetRoamerLocation(u8 index, u8 *mapGroup, u8 *mapNum);
bool8 TryAddRoamer(u16 species, u8 level);
bool8 TryAddTerrestrialRoamer(u16 species, u8 level);
void MoveAllRoamersToOtherLocationSets(void);
void MoveAllRoamers(void);

extern u8 gEncounteredRoamerIndex;

#endif // GUARD_ROAMER_H
