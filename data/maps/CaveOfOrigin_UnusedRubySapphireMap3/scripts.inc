CaveOfOrigin_UnusedRubySapphireMap3_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, CaveOfOrigin_UnusedRubySapphireMap3_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, CaveOfOrigin_UnusedRubySapphireMap3_OnTransition
	.byte 0

CaveOfOrigin_UnusedRubySapphireMap3_OnLoad:
	call_if_unset FLAG_REGI_DOORS_OPENED, CaveOfOrigin_UnusedRubySapphireMap3_CloseRegiEntrance
	end

CaveOfOrigin_UnusedRubySapphireMap3_OnTransition:
	end

CaveOfOrigin_UnusedRubySapphireMap3_CloseRegiEntrance::
	setmetatile 9, 6, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 9, 7, METATILE_General_RockWall_SandBase, TRUE
	return

CaveOfOrigin_UnusedRubySapphireMap3_Hiker::
	msgbox CaveOfOrigin_UnusedRubySapphireMap3_Hiker_Text, MSGBOX_NPC
	release
	end

CaveOfOrigin_UnusedRubySapphireMap3_Hiker_Text:
	.string "My brother has found this exact same\n"
	.string "ruins in the northern part of the\l"
	.string "region. What do they represent?\p"
	.string "Are they from an ancient civilization?$"
