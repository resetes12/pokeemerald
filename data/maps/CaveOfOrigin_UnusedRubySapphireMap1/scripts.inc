CaveOfOrigin_UnusedRubySapphireMap1_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, CaveOfOrigin_UnusedRubySapphireMap1_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, CaveOfOrigin_UnusedRubySapphireMap1_OnTransition
	.byte 0

CaveOfOrigin_UnusedRubySapphireMap1_OnLoad:
	call_if_unset FLAG_REGI_DOORS_OPENED, CaveOfOrigin_UnusedRubySapphireMap1_CloseRegiEntrance
	end

CaveOfOrigin_UnusedRubySapphireMap1_OnTransition:
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
	.string "I have seen these kind of rock\n"
	.string "structure in other places...\p"
	.string "For example, in ROUTE 105!\p"
	.string "What is the meaning of these\n"
	.string "rock formations?\p"
	.string "I'm totally sure that they are\n"
	.string "man-made. Do you have any clue?$"

CaveOfOrigin_UnusedRubySapphireMap1_Scientist2_Text:
	.string "Every time there's a thunderstorm,\n"
	.string "some lighting strikes end up\l"
	.string "hitting this small island.\p"
	.string "Isn't that suspicious?\p"
	.string "We think there's a POKéMON living\n"
	.string "here whose only nourishment are\l"
	.string "lightning strikes.$"

CaveOfOrigin_UnusedRubySapphireMap1_Scientist3_Text:
	.string "We are researching a strange\n"
	.string "phenomenon that has been\l"
	.string "happening for some months.\p"
	.string "This island is unhabited by\n"
	.string "humans, so something happening\l"
	.string "here is... extremely myterious.\p"
	.string "KIRI is also looking for clues.\n"
	.string "We need to unravel this mystery.$"

CaveOfOrigin_UnusedRubySapphireMap1_Kirlia_Text:
	.string "KIRI: Kir!$"
