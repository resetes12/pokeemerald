ArtisanCave_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ArtisanCave_B1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	.byte 0

ArtisanCave_B1F_OnTransition:
	callnative ChangeEncounterTable
	setflag FLAG_LANDMARK_ARTISAN_CAVE
	end

