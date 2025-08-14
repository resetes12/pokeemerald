AbandonedShip_Rooms2_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EncounterTable
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	.byte 0

AbandonedShip_Rooms2_B1F_EventScript_Camper::
	msgbox AbandonedShip_Rooms2_B1F_Text_PerfectPlaceToGoExploring, MSGBOX_NPC
	end

AbandonedShip_Rooms2_B1F_Text_PerfectPlaceToGoExploring:
	.string "This is a perfect place to go exploring!\n"
	.string "It's exciting here!\p"
	.string "I bet there're amazing treasures on\n"
	.string "board.$"

