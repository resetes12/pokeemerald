MirageTower_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MirageTower_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	.byte 0

MirageTower_1F_OnTransition:
	callnative ChangeEncounterTable
	setflag FLAG_ENTERED_MIRAGE_TOWER
	setflag FLAG_FORCE_MIRAGE_TOWER_VISIBLE
	setflag FLAG_LANDMARK_MIRAGE_TOWER
	end

