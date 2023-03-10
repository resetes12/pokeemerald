CaveOfOrigin_UnusedRubySapphireMap3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CaveOfOrigin_UnusedRubySapphireMap3_OnTransition
	.byte 0

CaveOfOrigin_UnusedRubySapphireMap3_OnTransition:
	call_if_set FLAG_UNUSED_RS_LEGENDARY_BATTLE_DONE, CaveOfOrigin_EventScript_DisableTriggers
	end

CaveOfOrigin_UnusedRubySapphireMap3_Hiker::
	msgbox CaveOfOrigin_UnusedRubySapphireMap3_Hiker_Text, MSGBOX_NPC
	release
	end

CaveOfOrigin_UnusedRubySapphireMap3_Hiker_Text:
	.string "My brother has found this\n"
	.string "exact same ruins in the\l"
	.string "northern part of the region.\p"
	.string "What do they represent?\n"
	.string "Are they from an ancient\l"
	.string "civilization? I don't know...$"
