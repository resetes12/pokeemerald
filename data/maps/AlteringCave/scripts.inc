.set LOCALID_MEWTWO, 5

AlteringCave_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, AlteringCave_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, AlteringCave_OnTransition
	.byte 0

AlteringCave_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, AlteringCave_EventScript_TryRemoveMewtwo
	end

AlteringCave_EventScript_TryRemoveMewtwo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

AlteringCave_OnTransition:
	setflag FLAG_LANDMARK_ALTERING_CAVE
	call_if_unset FLAG_UNLOCK_MEWTWO, AlteringCave_EventScript_HideMewtwo
	call_if_unset FLAG_DEFEATED_MEWTWO, AlteringCave_EventScript_ShowMewtwo
	end

AlteringCave_EventScript_ShowMewtwo::
	clearflag FLAG_HIDE_MEWTWO
	end

AlteringCave_EventScript_HideMewtwo::
	setflag FLAG_HIDE_MEWTWO
	removeobject LOCALID_MEWTWO
	end

AlteringCave_EventScript_Mewtwo::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_MEWTWO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_MEWTWO, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, AlteringCave_EventScript_DefeatedMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, AlteringCave_EventScript_RanFromMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, AlteringCave_EventScript_RanFromMewtwo
	setflag FLAG_DEFEATED_MEWTWO
	release
	end

AlteringCave_EventScript_DefeatedMewtwo::
	setflag FLAG_DEFEATED_MEWTWO
	goto Common_EventScript_RemoveStaticPokemon
	end

AlteringCave_EventScript_RanFromMewtwo::
	setvar VAR_0x8004, SPECIES_MEWTWO
	goto Common_EventScript_LegendaryFlewAway
	end
