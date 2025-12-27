SafariZone_Southwest_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	map_script MAP_SCRIPT_ON_TRANSITION, EncounterTable
	.byte 0

SafariZone_Southwest_EventScript_Woman::
	msgbox SafariZone_Southwest_Text_Woman, MSGBOX_NPC
	end

SafariZone_Southwest_EventScript_RestHouseSign::
	msgbox SafariZone_Southwest_Text_RestHouseSign, MSGBOX_SIGN
	end

SafariZone_Southwest_EventScript_Happiny::
	msgbox SafariZone_Southwest_EventScript_Happiny_Text, MSGBOX_NPC
	end

SafariZone_Southwest_EventScript_Happiny_Text:
	.string "Chansey evolves by Friendship to\n"
	.string "Blissey. What about Happinny?\l"
	.string "How does it evolve?\p"
	.string "Well, turns out that they have to hold\n"
	.string "a Lucky Egg while leveling up!$"
