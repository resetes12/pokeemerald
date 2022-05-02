#ifndef GUARD_ROAMER_H
#define GUARD_ROAMER_H

/* If set to TRUE, at the start of a new game or when InitRoamer() is called,
/* the following roamers are created to showcase the branch's features:
/* A basic Latias roamer that respawns weekly
/* A terrestrial Pikachu roamer that respawns daily
/* A terrestrial Pikachu that does not flee from battle
/* A scaling Weedle stalker that does not flee from battle and respawns instantly
/* All these species are also added to the Pokedex so you can track them*/
#define MULTIPLE_ROAMERS_EXAMPLE TRUE
/* Shows stalkers on the PokeDex. A bit pointless as
/* stalkers are always either at your location or nowhere */
#define SHOW_STALKERS_ON_POKEDEX FALSE

void StopAllRoamers(void);
void InitRoamer(void);
void UpdateLocationHistoryForRoamer(void);
void RoamerMoveToOtherLocationSet(u8 id);
void RoamerMove(u8 index);
bool8 IsRoamerAt(u8 id, u8 mapGroup, u8 mapNum);
void CreateRoamerMonInstance(u8 id);
u8 TryStartRoamerEncounter(bool8 isWaterEncounter);
void UpdateRoamerHPStatus(struct Pokemon *mon);
void StopRoamer(u8 index);
void GetRoamerLocation(u8 index, u8 *mapGroup, u8 *mapNum);
void MoveAllRoamersToOtherLocationSets(void);
void MoveAllRoamers(void);
bool8 DoesRoamerFlee(void);
bool8 CanRoamerRespawn(u8 index);
void HandleRoamerRespawnTimer(void);
void UpdateRoamerRespawns(u16 days);
bool8 TryAddRoamer(u16 species, u8 level, bool8 doesNotFlee, u16 respawnMode);
bool8 TryAddTerrestrialRoamer(u16 species, u8 level, bool8 doesNotFlee, u16 respawnMode);
bool8 TryAddStalker(u16 species, u8 level, bool8 doesNotFlee, bool8 isTerrestrial, u16 respawnMode);

extern u8 gEncounteredRoamerIndex;

enum {
    AMPHIBIOUS,
    TERRESTRIAL,
};

enum {
    FLEES,
    DOES_NOT_FLEE,
};

enum {
    NOT_STALKER,
    STALKER,
};

enum {
    NO_RESPAWN,
    DAILY_RESPAWN,
    WEEKLY_RESPAWN,
    INSTANT_RESPAWN,
};

#endif // GUARD_ROAMER_H
