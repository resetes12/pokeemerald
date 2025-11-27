EverGrandeCity_SidneysRoom_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, EverGrandeCity_SidneysRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, EverGrandeCity_SidneysRoom_OnWarp
	map_script MAP_SCRIPT_ON_TRANSITION, EverGrandeCity_SidneysRoom_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, EverGrandeCity_SidneysRoom_OnFrame
	.byte 0

EverGrandeCity_SidneysRoom_OnTransition:
	setflag FLAG_MET_SCOTT_IN_EVERGRANDE
	setflag FLAG_HIDE_EVER_GRANDE_POKEMON_CENTER_1F_SCOTT
	end

EverGrandeCity_SidneysRoom_OnLoad:
	call_if_set FLAG_DEFEATED_ELITE_4_SIDNEY, EverGrandeCity_SidneysRoom_EventScript_ResetAdvanceToNextRoom
	call_if_eq VAR_ELITE_4_STATE, 1, EverGrandeCity_SidneysRoom_EventScript_CloseDoor
	end

EverGrandeCity_SidneysRoom_EventScript_ResetAdvanceToNextRoom::
	call PokemonLeague_EliteFour_EventScript_ResetAdvanceToNextRoom
	return

EverGrandeCity_SidneysRoom_EventScript_CloseDoor::
	call PokemonLeague_EliteFour_EventScript_CloseDoor
	return

EverGrandeCity_SidneysRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_SidneysRoom_EventScript_PlayerTurnNorth
	.2byte 0

EverGrandeCity_SidneysRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

EverGrandeCity_SidneysRoom_OnFrame:
	map_script_2 VAR_ELITE_4_STATE, 0, EverGrandeCity_SidneysRoom_EventScript_WalkInCloseDoor
	.2byte 0

EverGrandeCity_SidneysRoom_EventScript_WalkInCloseDoor::
	lockall
	call PokemonLeague_EliteFour_EventScript_WalkInCloseDoor
	setvar VAR_ELITE_4_STATE, 1
	releaseall
	end

EverGrandeCity_SidneysRoom_EventScript_Sidney::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_ELITE_4_SIDNEY, EverGrandeCity_SidneysRoom_EventScript_PostBattleSpeech
	goto_if_set FLAG_SIDNEY_REMATCH, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch
	playbgm MUS_ENCOUNTER_ELITE_FOUR, FALSE
	msgbox EverGrandeCity_SidneysRoom_Text_IntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SIDNEY, EverGrandeCity_SidneysRoom_Text_Defeat
	goto EverGrandeCity_SidneysRoom_EventScript_Defeated
	end

EverGrandeCity_SidneysRoom_EventScript_SidneyRematch::
	playbgm MUS_ENCOUNTER_ELITE_FOUR, FALSE
	msgbox EverGrandeCity_SidneysRoom_Text_DoublesOrSingles
	dynmultichoice 20, 8, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, EverGrandeCity_SidneysRoom_Text_Doubles, EverGrandeCity_SidneysRoom_Text_Singles
	switch VAR_RESULT
	case 0, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_Doubles
	case 1, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_Singles
	case MULTI_B_PRESSED, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_NotReady
	end

EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_Singles::
	msgbox EverGrandeCity_SidneysRoom_Text_IntroSpeech2, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SIDNEY_2_SINGLE, EverGrandeCity_SidneysRoom_Text_Defeat2
	goto EverGrandeCity_SidneysRoom_EventScript_Defeated
	end

EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_Doubles::
	getpartysize
	goto_if_eq VAR_RESULT, 1, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_Doubles_CantBattle
	msgbox EverGrandeCity_SidneysRoom_Text_IntroSpeech2, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SIDNEY_2_DOUBLE, EverGrandeCity_SidneysRoom_Text_Defeat2
	goto EverGrandeCity_SidneysRoom_EventScript_Defeated
	end

EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_NotReady::
	msgbox EverGrandeCity_SidneysRoom_Text_NotReady, MSGBOX_DEFAULT
	playbgm MUS_VICTORY_ROAD, FALSE
	closemessage
	release
	end

EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_Doubles_CantBattle::
	msgbox EverGrandeCity_SidneysRoom_Text_NotEnoughPokemon, MSGBOX_DEFAULT
	playbgm MUS_VICTORY_ROAD, FALSE
	closemessage
	release
	end

EverGrandeCity_SidneysRoom_EventScript_PostBattleSpeech::
	msgbox EverGrandeCity_SidneysRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	release
	end

EverGrandeCity_SidneysRoom_EventScript_Defeated::
	setflag FLAG_DEFEATED_ELITE_4_SIDNEY
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
	msgbox EverGrandeCity_SidneysRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	release
	end

EverGrandeCity_SidneysRoom_Text_Doubles::
	.string "DOUBLE BATTLE$"

EverGrandeCity_SidneysRoom_Text_Singles::
	.string "SINGLE BATTLE$"

EverGrandeCity_SidneysRoom_Text_IntroSpeech:
	.string "Welcome, challenger!\n"
	.string "I'm SIDNEY of the ELITE FOUR.\p"
	.string "I like that look you're giving me.\n"
	.string "I guess you'll give me a good match.\l"
	.string "That's good! Looking real good!\p"
	.string "All right! You and me, let's enjoy\n"
	.string "a battle that can only be staged\l"
	.string "here in the POKéMON LEAGUE!$"

EverGrandeCity_SidneysRoom_Text_DoublesOrSingles::
	.string "Welcome back, challenger!\n"
	.string "I'm SIDNEY of the ELITE FOUR.\p"
	.string "But you already know me!\n"
	.string "I guess you'll give me a good match\l"
	.string "again. Looking real good!\p"
	.string "All right! You and me, let's enjoy\n"
	.string "a battle that can only be staged\l"
	.string "here in the POKéMON LEAGUE!\p"
	.string "You may choose whether to have a\n"
	.string "DOUBLE BATTLE or a SINGLE BATTLE.$"

EverGrandeCity_SidneysRoom_Text_IntroSpeech2::
	.string "This time, I won't lose.$"

EverGrandeCity_SidneysRoom_Text_NotEnoughPokemon::
	.string "You only have one POKéMON. You need\n"
	.string "two or more POKéMON to battle me.$"

EverGrandeCity_SidneysRoom_Text_NotReady::
	.string "Let me know when you're ready.$"

EverGrandeCity_SidneysRoom_Text_Defeat:
	.string "Well, how do you like that? I lost!\n"
	.string "Eh, it was fun, so it doesn't matter.$"

EverGrandeCity_SidneysRoom_Text_Defeat2:
	.string "Well, how do you like that? I lost!\n"
	.string "Again! It doesn't matter. It was fun.$"

EverGrandeCity_SidneysRoom_Text_PostBattleSpeech:
	.string "Well, listen to what this loser has\n"
	.string "to say.\p"
	.string "You've got what it takes to go far.\n"
	.string "Now, go on to the next room and enjoy\l"
	.string "your next battle!$"
