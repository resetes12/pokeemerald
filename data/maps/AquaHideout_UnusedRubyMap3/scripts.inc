AquaHideout_UnusedRubyMap3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AquaHideout_UnusedRubyMap3_OnTransition
	.byte 0

AquaHideout_UnusedRubyMap3_OnTransition::
	end

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
