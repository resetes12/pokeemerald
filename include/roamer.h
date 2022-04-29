#ifndef GUARD_ROAMER_H
#define GUARD_ROAMER_H

/* Creates Latias and Latios roamers on new game,
/* makes them seen on the Pokedex and sets the Pokedex flag on. */
#define MULTIPLE_ROAMERS_EXAMPLE TRUE

void DeactivateAllRoamers(void);
void ClearAllRoamerLocationData(void);
void ClearRoamerLocationData(u8 index);
void InitRoamer(void);
void UpdateLocationHistoryForRoamer(void);
void RoamerMoveToOtherLocationSet(u8 id);
void RoamerMove(u8 index);
bool8 IsRoamerAt(u8 id, u8 mapGroup, u8 mapNum);
void CreateRoamerMonInstance(u8 id);
u8 TryStartRoamerEncounter(void);
void UpdateRoamerHPStatus(struct Pokemon *mon);
void SetRoamerInactive(u8 index);
void GetRoamerLocation(u8 index, u8 *mapGroup, u8 *mapNum);
bool8 TryAddRoamer(u16 species, u8 level);

extern u8 gEncounteredRoamerIndex;

#endif // GUARD_ROAMER_H
