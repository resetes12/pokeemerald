Route117_PokemonDayCare_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route117_PokemonDayCare_OnTransition
	.byte 0

Route117_PokemonDayCare_OnTransition:
	setflag FLAG_LANDMARK_POKEMON_DAYCARE
	end

Route117_PokemonDayCare_Incense::
	msgbox Route117_PokemonDayCare_Incense_Text, MSGBOX_NPC
	end

Route117_PokemonDayCare_Incense_Text:
	.string "Some POKéMON can only be obtained\n"
	.string "by breeding while holding an item\l"
	.string "called INCENSE. There are 2 kinds.\p"
	.string "How many can you get this way?\n"
	.string "Right now, 9 different POKéMON\l"
	.string "can be bred using INCENSE.$"
