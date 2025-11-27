EverGrandeCity_DrakesRoom_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, EverGrandeCity_DrakesRoom_OnFrame
	map_script MAP_SCRIPT_ON_LOAD, EverGrandeCity_DrakesRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, EverGrandeCity_SidneysRoom_OnWarp
	.byte 0

@ Unused, Drake uses Sidneys identical OnWarp for some reason
EverGrandeCity_DrakesRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_DrakesRoom_EventScript_PlayerTurnNorth
	.2byte 0

EverGrandeCity_DrakesRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

EverGrandeCity_DrakesRoom_OnFrame:
	map_script_2 VAR_ELITE_4_STATE, 3, EverGrandeCity_DrakesRoom_EventScript_WalkInCloseDoor
	.2byte 0

EverGrandeCity_DrakesRoom_EventScript_WalkInCloseDoor::
	lockall
	call PokemonLeague_EliteFour_EventScript_WalkInCloseDoor
	setvar VAR_ELITE_4_STATE, 4
	releaseall
	end

EverGrandeCity_DrakesRoom_OnLoad:
	call_if_set FLAG_DEFEATED_ELITE_4_DRAKE, EverGrandeCity_DrakesRoom_EventScript_ResetAdvanceToNextRoom
	call_if_eq VAR_ELITE_4_STATE, 4, EverGrandeCity_DrakesRoom_EventScript_CloseDoor
	end

EverGrandeCity_DrakesRoom_EventScript_ResetAdvanceToNextRoom::
	call PokemonLeague_EliteFour_EventScript_ResetAdvanceToNextRoom
	return

EverGrandeCity_DrakesRoom_EventScript_CloseDoor::
	call PokemonLeague_EliteFour_EventScript_CloseDoor
	return

EverGrandeCity_DrakesRoom_EventScript_Drake::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_ELITE_4_DRAKE, EverGrandeCity_DrakesRoom_EventScript_PostBattleSpeech
	goto_if_set FLAG_DRAKE_REMATCH, EverGrandeCity_DrakesRoom_EventScript_DrakeRematch
	playbgm MUS_ENCOUNTER_ELITE_FOUR, FALSE
	msgbox EverGrandeCity_DrakesRoom_Text_IntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_DRAKE, EverGrandeCity_DrakesRoom_Text_Defeat
	goto EverGrandeCity_DrakesRoom_EventScript_Defeated
	end

EverGrandeCity_DrakesRoom_EventScript_DrakeRematch::
	playbgm MUS_ENCOUNTER_ELITE_FOUR, FALSE
	msgbox EverGrandeCity_DrakesRoom_Text_DoublesOrSingles
	dynmultichoice 20, 8, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, EverGrandeCity_SidneysRoom_Text_Doubles, EverGrandeCity_SidneysRoom_Text_Singles
	switch VAR_RESULT
	case 0, EverGrandeCity_DrakesRoom_EventScript_DrakeRematch_Doubles
	case 1, EverGrandeCity_DrakesRoom_EventScript_DrakeRematch_Singles
	case MULTI_B_PRESSED, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_NotReady
	end

EverGrandeCity_DrakesRoom_EventScript_DrakeRematch_Singles::
	msgbox EverGrandeCity_DrakesRoom_Text_IntroSpeech2, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_DRAKE_2_SINGLE, EverGrandeCity_DrakesRoom_Text_Defeat2
	goto EverGrandeCity_DrakesRoom_EventScript_Defeated
	end

EverGrandeCity_DrakesRoom_EventScript_DrakeRematch_Doubles::
	getpartysize
	goto_if_eq VAR_RESULT, 1, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_Doubles_CantBattle
	msgbox EverGrandeCity_DrakesRoom_Text_IntroSpeech2, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_DRAKE_2_DOUBLE, EverGrandeCity_DrakesRoom_Text_Defeat2
	goto EverGrandeCity_DrakesRoom_EventScript_Defeated
	end

EverGrandeCity_DrakesRoom_EventScript_PostBattleSpeech::
	msgbox EverGrandeCity_DrakesRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	release
	end

EverGrandeCity_DrakesRoom_EventScript_Defeated::
	setvar VAR_0x8004, FANCOUNTER_DEFEATED_DRAKE
	special Script_TryGainNewFanFromCounter
	setflag FLAG_DEFEATED_ELITE_4_DRAKE
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
	msgbox EverGrandeCity_DrakesRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	release
	end

EverGrandeCity_DrakesRoom_Text_IntroSpeech:
	.string "I am the last of the POKéMON LEAGUE\n"
	.string "ELITE FOUR, DRAKE the DRAGON master!\p"
	.string "In their natural state, POKéMON are\n"
	.string "wild living things. They are free.\p"
	.string "At times, they hinder us.\n"
	.string "At times, they help us.\p"
	.string "For us to battle with POKéMON as\n"
	.string "partners, do you know what it takes?\p"
	.string "Do you know what is needed?\p"
	.string "If you don't, then you will never\n"
	.string "prevail over me!$"

EverGrandeCity_DrakesRoom_Text_DoublesOrSingles:
	.string "I am the last of the POKéMON LEAGUE\n"
	.string "ELITE FOUR, DRAKE the DRAGON master!\p"
	.string "How are your POKéMON partners?\n"
	.string "Did they help you get stronger?\p"
	.string "I'm sure they did! Ready?\p"
	.string "I'll let you choose. Will you face\n"
	.string "me in a DOUBLE or a SINGLE BATTLE?$"

EverGrandeCity_DrakesRoom_Text_IntroSpeech2:
	.string "You seem already fired up. Did the\n"
	.string "other ELITE FOUR members took a\l"
	.string "toll on you? Don't falter me now!\p"
	.string "Prevail over me! Let's dance!$"

EverGrandeCity_DrakesRoom_Text_Defeat:
	.string "Superb, it should be said.$"

EverGrandeCity_DrakesRoom_Text_Defeat2:
	.string "Ha, ha, ha!\n"
	.string "I can't help but laugh!\p"
	.string "Amazing. Superb.\n"
	.string "You really are the best.$"

EverGrandeCity_DrakesRoom_Text_PostBattleSpeech:
	.string "You deserve every credit for coming\n"
	.string "this far as a TRAINER of POKéMON.\p"
	.string "You do seem to know what is needed.\p"
	.string "Yes, what a TRAINER needs is a\n"
	.string "virtuous heart.\p"
	.string "POKéMON touch the good hearts of\n"
	.string "TRAINERS and learn good from wrong.\p"
	.string "They touch the good hearts of\n"
	.string "TRAINERS and grow strong.\p"
	.string "Go! Go onwards!\n"
	.string "The CHAMPION is waiting!$"

