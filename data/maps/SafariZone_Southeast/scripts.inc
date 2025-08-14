.set LOCALID_MANTYKEMAN, 7

SafariZone_Southeast_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	map_script MAP_SCRIPT_ON_TRANSITION, EncounterTable
	.byte 0

SafariZone_Southeast_GliscorEvo::
	msgbox SafariZone_Southeast_GliscorEvo_Text, MSGBOX_NPC
	end

SafariZone_Southeast_GliscorEvo_Text:
	.string "I love SINNOH POKéMON!\p"
	.string "I need to catch a GLIGAR, so then\n"
	.string "I can get a GLISCOR!\p"
	.string "They evolve only at night,\n"
	.string "just like SNEASEL and MURKROW.$"

SafariZone_Southeast_MantykeEvo::
	msgbox SafariZone_Southeast_MantykeEvo_Text, MSGBOX_SIGN
	waitmessage
	msgbox SafariZone_Southeast_MantykeEvo_Text2, MSGBOX_NPC
	applymovement LOCALID_MANTYKEMAN, SafariZone_Southeast_MantykeEvoMoves
	waitmovement 0
	end

SafariZone_Southeast_MantykeEvoMoves:
	face_right
	step_end

SafariZone_Southeast_MantykeEvo_Text:
	.string "OH!$"

SafariZone_Southeast_MantykeEvo_Text2:
	.string "HAVE YOU SEEN THAT?\p"
	.string "That MANTYKE was trying to evolve,\n"
	.string "so it was using WATER PULSE!\l"
	.string "Nature is beautiful!$"

@ Event scripts for SafariZone_Southeast are in SafariZone_South/scripts.inc
