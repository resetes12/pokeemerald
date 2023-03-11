AquaHideout_UnusedRubyMap3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AquaHideout_UnusedRubyMap3_OnTransition
	.byte 0

AquaHideout_UnusedRubyMap3_OnTransition::
	end

AquaHideout_UnusedRubyMap3_BonslyMunchEvo::
	msgbox AquaHideout_UnusedRubyMap3_BonslyMunchEvo_Text, MSGBOX_NPC
	end

AquaHideout_UnusedRubyMap3_BonslyMunchEvo_Text:
	.string "Have you see those strange\n"
	.string "moving trees? They are a\l"
	.string "POKéMON named SUDOWOODO!\p"
	.string "It's a POKéMON that disguises\n"
	.string "itself as a tree.\p"
	.string "And there's also BONSLY, which\n"
	.string "does the same... but doesn't\l"
	.string "look as realistic.\p"
	.string "Speaking of which, I've seen\n"
	.string "BONSLY evolve while trying\l"
	.string "to imitate other SUDOWOODO.\p"
	.string "Is that how they evolve?\p"
	.string "If I'm not mistaken, MIME JR.\n"
	.string "also evolves that way.\p"
	.string "Very different from my\n"
	.string "MUNCHLAX, who evolved by\l"
	.string "FRIENDSHIP, right?$"

BFNewZone_EventScript_Sudo2::
	lock
	faceplayer
	msgbox BFNewZone_EventScript_SudoText, MSGBOX_DEFAULT
	closemessage
	setwildbattle SPECIES_SUDOWOODO, 40
	waitse
	playmoncry SPECIES_SUDOWOODO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_WILD
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BFNewZone_EventScript_Sudo2_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BFNewZone_EventScript_Sudo2_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BFNewZone_EventScript_Sudo2_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BFNewZone_EventScript_Sudo2_Ran
	release
	end

BFNewZone_EventScript_SudoText:
	.string "Wait, I've see a tree\n"
	.string "like this before...\p"
	.string "It's not a tree! It attacked!$"

BFNewZone_EventScript_Sudo2_Won::
	setflag FLAG_UNUSED_0x923
	goto Common_EventScript_RemoveStaticPokemon
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

BFNewZone_EventScript_Sudo2_Ran::
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

BFNewZone_EventScript_Sudo3::
	lock
	faceplayer
	setwildbattle SPECIES_SUDOWOODO, 40
	waitse
	playmoncry SPECIES_SUDOWOODO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_WILD
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BFNewZone_EventScript_Sudo3_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BFNewZone_EventScript_Sudo3_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BFNewZone_EventScript_Sudo3_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BFNewZone_EventScript_Sudo3_Ran
	release
	end

BFNewZone_EventScript_Sudo3_Won::
	setflag FLAG_UNUSED_0x924
	goto Common_EventScript_RemoveStaticPokemon
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

BFNewZone_EventScript_Sudo3_Ran::
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end
