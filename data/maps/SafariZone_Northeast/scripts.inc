SafariZone_Northeast_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EncounterTable
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	.byte 0

SafariZone_Northeast_AmbipomEvo::
	msgbox SafariZone_Northeast_AmbipomEvo_Text, MSGBOX_NPC
	end

SafariZone_Northeast_AmbipomEvo_Text:
	.string "I love the POKéMON AIPOM! They\n"
	.string "are really cute!\p"
	.string "Did you know they can evolve?\p"
	.string "Use a move that makes everyone\n"
	.string "laugh, and they will evolve!\p"
	.string "Aipom are just lovely!$"

@ Event scripts for SafariZone_Northeast are in SafariZone_South/scripts.inc
