CaveOfOrigin_UnusedRubySapphireMap1_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, CaveOfOrigin_UnusedRubySapphireMap1_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, EncounterTable
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	.byte 0

CaveOfOrigin_UnusedRubySapphireMap1_OnLoad:
	call_if_unset FLAG_REGI_DOORS_OPENED, CaveOfOrigin_UnusedRubySapphireMap1_CloseRegiEntrance
	end

CaveOfOrigin_UnusedRubySapphireMap1_CloseRegiEntrance::
	setmetatile 14, 14, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 14, 15, METATILE_General_RockWall_SandBase, TRUE
	return

CaveOfOrigin_UnusedRubySapphireMap1_Scientist1::
	msgbox CaveOfOrigin_UnusedRubySapphireMap1_Scientist1_Text, MSGBOX_NPC
	release
	end

CaveOfOrigin_UnusedRubySapphireMap1_Scientist2::
	msgbox CaveOfOrigin_UnusedRubySapphireMap1_Scientist2_Text, MSGBOX_NPC
	release
	end

CaveOfOrigin_UnusedRubySapphireMap1_Scientist3::
	msgbox CaveOfOrigin_UnusedRubySapphireMap1_Scientist3_Text, MSGBOX_NPC
	release
	end

CaveOfOrigin_UnusedRubySapphireMap1_Kirlia::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_KIRLIA, CRY_MODE_NORMAL
	msgbox CaveOfOrigin_UnusedRubySapphireMap1_Kirlia_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end

CaveOfOrigin_UnusedRubySapphireMap1_Scientist1_Text:
	.string "I have seen this kind of rock structure\n"
	.string "in other places, like in ROUTE 132.\p"
	.string "What is the meaning of these rock\n"
	.string "formations?\p"
	.string "I'm totally sure that they are man-made.\n"
	.string "Do you have any clue?$"

CaveOfOrigin_UnusedRubySapphireMap1_Scientist2_Text:
	.string "Every time there's a thunderstorm, some\n"
	.string "lighting strikes end up hitting this\l"
	.string "small island. Isn't that suspicious?\p"
	.string "We think there's a POKéMON living\n"
	.string "here whose only nourishment are\l"
	.string "lightning strikes.$"

CaveOfOrigin_UnusedRubySapphireMap1_Scientist3_Text:
	.string "We are researching a strange\n"
	.string "phenomenon that has been happening\l"
	.string "for a few months. This island is\l"
	.string "unhabited by humans, so something\l"
	.string "happening here is extremely myterious,\l"
	.string "to say the least.\p"
	.string "KIRI is also looking for clues.\n"
	.string "We need to unravel this mystery.$"

CaveOfOrigin_UnusedRubySapphireMap1_Kirlia_Text:
	.string "KIRI: Kir!$"
