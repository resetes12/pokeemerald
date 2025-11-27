EverGrandeCity_GlaciasRoom_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, EverGrandeCity_GlaciasRoom_OnFrame
	map_script MAP_SCRIPT_ON_LOAD, EverGrandeCity_GlaciasRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, EverGrandeCity_GlaciasRoom_OnWarp
	.byte 0

EverGrandeCity_GlaciasRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_GlaciasRoom_EventScript_PlayerTurnNorth
	.2byte 0

EverGrandeCity_GlaciasRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

EverGrandeCity_GlaciasRoom_OnFrame:
	map_script_2 VAR_ELITE_4_STATE, 2, EverGrandeCity_GlaciasRoom_EventScript_WalkInCloseDoor
	.2byte 0

EverGrandeCity_GlaciasRoom_EventScript_WalkInCloseDoor::
	lockall
	call PokemonLeague_EliteFour_EventScript_WalkInCloseDoor
	setvar VAR_ELITE_4_STATE, 3
	releaseall
	end

EverGrandeCity_GlaciasRoom_OnLoad:
	call_if_set FLAG_DEFEATED_ELITE_4_GLACIA, EverGrandeCity_GlaciasRoom_EventScript_ResetAdvanceToNextRoom
	call_if_eq VAR_ELITE_4_STATE, 3, EverGrandeCity_GlaciasRoom_EventScript_CloseDoor
	end

EverGrandeCity_GlaciasRoom_EventScript_ResetAdvanceToNextRoom::
	call PokemonLeague_EliteFour_EventScript_ResetAdvanceToNextRoom
	return

EverGrandeCity_GlaciasRoom_EventScript_CloseDoor::
	call PokemonLeague_EliteFour_EventScript_CloseDoor
	return

EverGrandeCity_GlaciasRoom_EventScript_Glacia::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_ELITE_4_GLACIA, EverGrandeCity_GlaciasRoom_EventScript_PostBattleSpeech
	goto_if_set FLAG_GLACIA_REMATCH, EverGrandeCity_GlaciasRoom_EventScript_GlaciaRematch
	playbgm MUS_ENCOUNTER_ELITE_FOUR, FALSE
	msgbox EverGrandeCity_GlaciasRoom_Text_IntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GLACIA, EverGrandeCity_GlaciasRoom_Text_Defeat
	goto EverGrandeCity_GlaciasRoom_EventScript_Defeated
	end

EverGrandeCity_GlaciasRoom_EventScript_GlaciaRematch::
	playbgm MUS_ENCOUNTER_ELITE_FOUR, FALSE
	msgbox EverGrandeCity_GlaciasRoom_Text_DoublesOrSingles
	dynmultichoice 20, 8, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, EverGrandeCity_SidneysRoom_Text_Doubles, EverGrandeCity_SidneysRoom_Text_Singles
	switch VAR_RESULT
	case 0, EverGrandeCity_GlaciasRoom_EventScript_GlaciaRematch_Doubles
	case 1, EverGrandeCity_GlaciasRoom_EventScript_GlaciaRematch_Singles
	case MULTI_B_PRESSED, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_NotReady
	end

EverGrandeCity_GlaciasRoom_EventScript_GlaciaRematch_Singles::
	msgbox EverGrandeCity_GlaciasRoom_Text_IntroSpeech2, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GLACIA_2_SINGLE, EverGrandeCity_GlaciasRoom_Text_Defeat2
	goto EverGrandeCity_GlaciasRoom_EventScript_Defeated
	end

EverGrandeCity_GlaciasRoom_EventScript_GlaciaRematch_Doubles::
	getpartysize
	goto_if_eq VAR_RESULT, 1, EverGrandeCity_SidneysRoom_EventScript_SidneyRematch_Doubles_CantBattle
	msgbox EverGrandeCity_GlaciasRoom_Text_IntroSpeech2, MSGBOX_DEFAULT
	trainerbattle_no_intro  TRAINER_GLACIA_2_DOUBLE, EverGrandeCity_GlaciasRoom_Text_Defeat2
	goto EverGrandeCity_GlaciasRoom_EventScript_Defeated
	end

EverGrandeCity_GlaciasRoom_EventScript_PostBattleSpeech::
	msgbox EverGrandeCity_GlaciasRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	release
	end

EverGrandeCity_GlaciasRoom_EventScript_Defeated::
	setflag FLAG_DEFEATED_ELITE_4_GLACIA
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
	msgbox EverGrandeCity_GlaciasRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	release
	end

EverGrandeCity_GlaciasRoom_Text_IntroSpeech:
	.string "Welcome, my name is GLACIA\n"
	.string "of the ELITE FOUR.\p"
	.string "I've traveled from afar to HOENN\n"
	.string "so that I may hone my ice skills.\p"
	.string "But all I have seen are challenges by\n"
	.string "weak TRAINERS and their POKéMON.\p"
	.string "What about you?\p"
	.string "It would please me to no end if I could\n"
	.string "go all out against you!$"

EverGrandeCity_GlaciasRoom_Text_DoublesOrSingles:
	.string "Welcome again, I'm GLACIA. I won't\n"
	.string "introduce myself again, you know me.\p"
	.string "I haven't had any challenge since I\n"
	.string "last battled you and your POKéMON.\p"
	.string "What about you?\n"
	.string "Have your skills honed?\p"
	.string "Before we start, I'll give you the\n"
	.string "chance to decide whether to fight me\l"
	.string "in a DOUBLE or a SINGLE BATTLE.$"

EverGrandeCity_GlaciasRoom_Text_IntroSpeech2:
	.string "It would please me to no end if I could\n"
	.string "go all out against you!\p"
	.string "This time… I will win!$"

EverGrandeCity_GlaciasRoom_Text_Defeat:
	.string "You and your POKéMON…\n"
	.string "How hot your spirits burn!\p"
	.string "The all-consuming heat overwhelms.\p"
	.string "It's no surprise that my icy skills\n"
	.string "failed to harm you.$"

EverGrandeCity_GlaciasRoom_Text_Defeat2:
	.string "Your spirit is still burning! But\p"
	.string "I got left in the cold again…$"

EverGrandeCity_GlaciasRoom_Text_PostBattleSpeech:
	.string "Advance to the next room.\p"
	.string "And there, confirm the truly fearsome\n"
	.string "side of the POKéMON LEAGUE.$"
