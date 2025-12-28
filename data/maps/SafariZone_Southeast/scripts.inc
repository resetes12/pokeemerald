.set LOCALID_MANTYKEMAN, 7

SafariZone_Southeast_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	map_script MAP_SCRIPT_ON_TRANSITION, EncounterTable
	.byte 0

SafariZone_Southeast_GliscorEvo::
	msgbox SafariZone_Southeast_GliscorEvo_Text, MSGBOX_NPC
	end

SafariZone_Southeast_GliscorEvo_Text:
	.string "I love Sinnoh Pokémon!\p"
	.string "I need to catch a Gligar, so then\n"
	.string "I can get a Gliscor!\p"
	.string "They evolve only at night,\n"
	.string "just like Sneasel and Murkrow.$"

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
	.string "Have you seen that?\p"
	.string "That Mantyke was trying to evolve,\n"
	.string "so it was using Water Pulse!\l"
	.string "Nature is beautiful!$"

@ Event scripts for SafariZone_Southeast are in SafariZone_South/scripts.inc
