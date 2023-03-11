SafariZone_Southwest_MapScripts::
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
	.string "CHANSEY evolves by FRIENDSHIP\n"
	.string "to BLISSEY. What about\l"
	.string "HAPPINNY? How does it evolve?\p"
	.string "Well, turns out that they have\n"
	.string "to hold a LUCKY EGG while\l"
	.string "leveling up!$"
