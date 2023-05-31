
ShoalCave_LowTideIceRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ShoalCave_LowTideIceRoom_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

ShoalCave_LowTideIceRoom_OnTransition::
	callnative SetTimeBasedEncounters
	goto_if_set FLAG_SUICUNE, ShoalCave_LowTideIceRoom_Show
	goto_if_unset FLAG_UNLOCK_DOGS, ShoalCave_LowTideIceRoom_HideSuicune
	end

ShoalCave_LowTideIceRoom_HideSuicune::
	@playmoncry SPECIES_SUICUNE, CRY_MODE_ENCOUNTER
	setflag FLAG_DEFEAT_SUICUNE
	setflag FLAG_SUICUNE
	end

ShoalCave_LowTideIceRoom_Show::
	end

ShoalCave_LowTideIceRoom_Suicune::
	lock
	faceplayer
	setwildbattle SPECIES_SUICUNE, 50
	waitse
	playmoncry SPECIES_SUICUNE, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_MEWTWO
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ShoalCave_LowTideIceRoom_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, ShoalCave_LowTideIceRoom_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, ShoalCave_LowTideIceRoom_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, ShoalCave_LowTideIceRoom_Ran
	release
	end

ShoalCave_LowTideIceRoom_Won::
	setflag FLAG_DEFEAT_SUICUNE
	goto Common_EventScript_RemoveStaticPokemon
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

ShoalCave_LowTideIceRoom_Ran::
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end
