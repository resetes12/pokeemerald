.set LOCALID_ZAPDOS, 2

ScorchedSlab_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, ScorchedSlab_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, ScorchedSlab_OnTransition
	.byte 0

ScorchedSlab_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, ScorchedSlab_EventScript_TryRemoveZapdos
	end

ScorchedSlab_EventScript_TryRemoveZapdos::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

ScorchedSlab_OnTransition:
	setflag FLAG_LANDMARK_SCORCHED_SLAB
	call_if_unset FLAG_UNLOCK_BIRDS, ScorchedSlab_EventScript_HideZapdos
	call_if_unset FLAG_DEFEATED_ZAPDOS, ScorchedSlab_EventScript_ShowZapdos
	end

ScorchedSlab_EventScript_ShowZapdos::
	clearflag FLAG_HIDE_ZAPDOS
	end

ScorchedSlab_EventScript_HideZapdos::
	setflag FLAG_HIDE_ZAPDOS
	removeobject LOCALID_ZAPDOS
	end

ScorchedSlab_EventScript_Zapdos::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_ZAPDOS, 50
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ScorchedSlab_EventScript_DefeatedZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, ScorchedSlab_EventScript_RanFromZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, ScorchedSlab_EventScript_RanFromZapdos
	setflag FLAG_DEFEATED_ZAPDOS
	release
	end

ScorchedSlab_EventScript_DefeatedZapdos::
	setflag FLAG_DEFEATED_ZAPDOS
	goto Common_EventScript_RemoveStaticPokemon
	end

ScorchedSlab_EventScript_RanFromZapdos::
	setvar VAR_0x8004, SPECIES_ZAPDOS
	goto Common_EventScript_LegendaryFlewAway
	end
