SafariZone_Northeast_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SetTimeEncounters
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

SafariZone_Northeast_AmbipomEvo::
	msgbox SafariZone_Northeast_AmbipomEvo_Text, MSGBOX_NPC
	end

SafariZone_Northeast_AmbipomEvo_Text:
	.string "I love the POKéMON AIPOM! They are\n"
	.string "so cute! But, did you know that\l"
	.string "they can evolve once?\p"
	.string "If you use a move that makes\n"
	.string "everyone laugh, they will evolve!\p"
	.string "And they're lovely!$"

@ Event scripts for SafariZone_Northeast are in SafariZone_South/scripts.inc
