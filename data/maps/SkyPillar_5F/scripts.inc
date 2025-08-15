SkyPillar_5F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SkyPillar_5F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	.byte 0

SkyPillar_5F_OnTransition:
	callnative ChangeEncounterTable
	call_if_lt VAR_SKY_PILLAR_STATE, 2, SkyPillar_5F_EventScript_CleanFloor
	return

SkyPillar_5F_EventScript_CleanFloor::
	setmaplayoutindex LAYOUT_SKY_PILLAR_5F_CLEAN
	return

