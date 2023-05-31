MagmaHideout_3F_1R_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MagmaHideout_3F_1R_OnTransition
	.byte 0

MagmaHideout_3F_1R_OnTransition:
	goto_if_set FLAG_ENTEI, MagmaHideout_3F_1R_EventScript_ShowEntei
	goto_if_unset FLAG_UNLOCK_DOGS, MagmaHideout_3F_1R_EventScript_NoShowEntei
	end

MagmaHideout_3F_1R_EventScript_NoShowEntei::
	@playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
	setflag FLAG_DEFEAT_ENTEI
	setflag FLAG_ENTEI
	end

MagmaHideout_3F_1R_EventScript_ShowEntei::
	end

MagmaHideout_3F_1R_EventScript_Entei::
	lock
	faceplayer
	setwildbattle SPECIES_ENTEI, 50
	waitse
	playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_LEGEND
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MagmaHideout_3F_1R_EventScript_EnteiWon
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MagmaHideout_3F_1R_EventScript_EnteiWon
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MagmaHideout_3F_1R_EventScript_EnteiRan
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MagmaHideout_3F_1R_EventScript_EnteiRan
	release
	end

MagmaHideout_3F_1R_EventScript_EnteiWon::
	setflag FLAG_DEFEAT_ENTEI
	goto Common_EventScript_RemoveStaticPokemon
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

MagmaHideout_3F_1R_EventScript_EnteiRan::
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

MagmaHideout_3F_1R_EventScript_Grunt9::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_9, MagmaHideout_3F_1R_Text_Grunt9Intro, MagmaHideout_3F_1R_Text_Grunt9Defeat
	msgbox MagmaHideout_3F_1R_Text_Grunt9PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_1R_EventScript_Grunt16::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_16, MagmaHideout_3F_1R_Text_Grunt16Intro, MagmaHideout_3F_1R_Text_Grunt16Defeat
	msgbox MagmaHideout_3F_1R_Text_Grunt16PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_1R_Text_Grunt9Intro:
	.string "What did I do to deserve this guard\n"
	.string "posting?\p"
	.string "My left ear is burning up!$"

MagmaHideout_3F_1R_Text_Grunt9Defeat:
	.string "I'm getting heat exhaustion…$"

MagmaHideout_3F_1R_Text_Grunt9PostBattle:
	.string "Do you think it's odd that we're wearing\n"
	.string "hoods in this magma-filled volcano?$"

MagmaHideout_3F_1R_Text_Grunt16Intro:
	.string "We joined so we can help our leader\n"
	.string "achieve his fantastic vision.\p"
	.string "I don't care if you're with TEAM AQUA\n"
	.string "or if you're just some kid passing by.\p"
	.string "No one interferes with us and gets\n"
	.string "away with it!$"

MagmaHideout_3F_1R_Text_Grunt16Defeat:
	.string "Oh, no!\n"
	.string "You're not to be trusted at all!$"

MagmaHideout_3F_1R_Text_Grunt16PostBattle:
	.string "Listen to me.\n"
	.string "TEAM MAGMA is right!\p"
	.string "Don't listen to TEAM AQUA.\n"
	.string "Don't believe their lies!$"

