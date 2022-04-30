#ifndef GUARD_ROAMER_H
#define GUARD_ROAMER_H

/* Create Latias and Latios roamers as well as 2 terrestrial
/* Pikachu roamers on new game, flag them all as seen on the 
/* Pokedex and give the Pokedex to the player.
/* One of the Pikachu does not flee from battle! */
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
bool8 TryAddRoamer(u16 species, u8 level, bool8 doesNotFlee);
bool8 TryAddTerrestrialRoamer(u16 species, u8 level, bool8 doesNotFlee);
void MoveAllRoamersToOtherLocationSets(void);
void MoveAllRoamers(void);
bool8 DoesRoamerFlee(void);

extern u8 gEncounteredRoamerIndex;

enum {
	AMPHIBIOUS,
	TERRESTRIAL,
};

enum {
	FLEES,
	DOES_NOT_FLEE,
};

#endif // GUARD_ROAMER_H
