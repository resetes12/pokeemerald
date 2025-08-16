.set LOCALID_IT, 1
.set LOCALID_K, 2
.set LOCALID_I, 3
.set LOCALID_N, 4
.set LOCALID_G, 5
.set LOCALID_EM, 6
.set LOCALID_ARC, 7
.set LOCALID_G2, 8
.set LOCALID_O, 9
.set LOCALID_D, 10

NavelRock_Unown_Room_1_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, NavelRock_Unown_Room_1_RemoveFollower
	map_script MAP_SCRIPT_ON_LOAD, NavelRock_Unown_Room_1_RemoveFollower
	.byte 0

NavelRock_Unown_Room_1_RemoveFollower::
	setflag FLAG_TEMP_HIDE_FOLLOWER
	release
	end

NavelRock_Unown_Room_1_U_Guide::
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox NavelRock_Unown_Room_1_U_Guide_Text, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, Debug_Script_1_Closemessage
	case MULTI_B_PRESSED, Debug_Script_1_Closemessage
	closemessage
	warp MAP_NAVEL_ROCK_UNOWN_CHAMBER, 12, 12
	end

NavelRock_Unown_Room_1_U_Guide_Text:
	.string "You feel as though this UNOWN\n"
	.string "is urging you to follow it.\p"
	.string "Accept its offering and go\n"
	.string "back to the UNOWN CHAMBER?$"

@GOD

NavelRock_Unown_Room_1_First_Trigger::
	lock
	goto_if_gt VAR_TEST_DARK, 0, Debug_Script_1_Closemessage
	setvar VAR_TEST_DARK, 1
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_G2, LOCALID_G2_Movement
	applymovement LOCALID_O, LOCALID_O_Movement
	applymovement LOCALID_D, LOCALID_D_Movement
	waitmovement 0
	removeobject LOCALID_G2, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	removeobject LOCALID_O, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	removeobject LOCALID_D, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	setflag FLAG_TEST_HIDE_1
	release
	end

LOCALID_G2_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

LOCALID_O_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LOCALID_D_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

@BATTLE 1

NavelRock_Unown_Room_1_Second_Trigger::
	lock
	goto_if_gt VAR_TEST_DARK, 1, Debug_Script_1_Closemessage
	playmoncry SPECIES_ARCEUS, CRY_MODE_NORMAL
	waitmoncry
	msgbox NavelRock_Unown_Room_1_Second_Trigger_Text, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, NavelRock_Unown_Room_1_Second_Trigger_No
	case MULTI_B_PRESSED, NavelRock_Unown_Room_1_Second_Trigger_No
	closemessage
	playbgm MUS_DP_AZURE_FLUTE, FALSE
	setflag FLAG_SYS_NO_BAG_USE
	applymovement LOCALID_ARC, LOCALID_ARC_Movement
	waitmovement 0
	playmoncry SPECIES_ARCEUS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_ARCEUS, 80
	setvar VAR_0x8000, 999 @HP
	setvar VAR_0x8001, 250 @ATK
	setvar VAR_0x8002, 50 @DEF
	setvar VAR_0x8003, 200 @SPD
	setvar VAR_0x8005, 250 @SP. ATK
	setvar VAR_0x8006, 50 @SP. DEF
	special SetHpStat
	special SetAtkStat
	special SetDefStat
	special SetSpAtkStat
	special SetSpDefStat
	special SetSpdStat
	special BattleSetup_StartLegendaryBattle
	waitstate
	calculatemonstats
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, NavelRock_Unown_Room_1_Second_Trigger_Battle_Finished
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, NavelRock_Unown_Room_1_Second_Trigger_Battle_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, NavelRock_Unown_Room_1_Second_Trigger_Battle_Ran
	release
	end

NavelRock_Unown_Room_1_Second_Trigger_No::
	clearflag FLAG_SYS_NO_BAG_USE
	applymovement OBJ_EVENT_ID_PLAYER, Player_Movement_1
	waitmovement 0
	closemessage
	end

NavelRock_Unown_Room_1_Second_Trigger_Battle_Ran::
	clearflag FLAG_SYS_NO_BAG_USE
	applymovement LOCALID_ARC, LOCALID_ARC_Movement_No
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Player_Movement_1
	waitmovement 0
	closemessage
	end

NavelRock_Unown_Room_1_Second_Trigger_Battle_Finished::
	clearflag FLAG_SYS_NO_BAG_USE
	setvar VAR_TEST_DARK, 2
	applymovement LOCALID_ARC, LOCALID_ARC_Movement_No
	waitmovement 0
	setvar VAR_0x8000, 0
	setvar VAR_0x8001, 0
	setvar VAR_0x8002, 0
	setvar VAR_0x8003, 0
	setvar VAR_0x8005, 0
	setvar VAR_0x8006, 0
	end

Player_Movement_1:
	walk_down
	step_end

LOCALID_ARC_Movement_No:
	walk_up
	walk_right
	face_down
	delay_16
	delay_16
	delay_16
	step_end

LOCALID_ARC_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_left
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	face_down
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	step_end

NavelRock_Unown_Room_1_Second_Trigger_Text:
	.string "A giant, godly creature towers above\n"
	.string "you, sending shivers down your spine.\p"
	.string "It radiates immense power, and\n"
	.string "it will not let you pass any further.\p"
	.string "Are you ready to face it?$"

NavelRock_Unown_Room_1_Talk_Arceus::
	msgbox NavelRock_Unown_Room_1_Talk_Arceus_Text1, MSGBOX_DEFAULT
	closemessage
	setvar 0x8002, SPECIES_ARCEUS
	special CheckPkm
	goto_if_eq VAR_RESULT, FALSE, Debug_Script_1_Closemessage
	playmoncry SPECIES_ARCEUS, CRY_MODE_NORMAL
	waitmoncry
	faceplayer
	msgbox NavelRock_Unown_Room_1_Talk_Arceus_Text2,  MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ARC, LOCALID_ARC_Movement_Talk
	waitmovement 0
	end

LOCALID_ARC_Movement_Talk:
	face_down
	step_end

NavelRock_Unown_Room_1_Talk_Arceus_Text1:
	.string "ARCEUS is letting me pass.$"

NavelRock_Unown_Room_1_Talk_Arceus_Text2:
	.string "The wild ARCEUS reacted to the\n"
	.string "ARCEUS in my party!\p"
	.string "Somehow, I understood that it was\n"
	.string "strangely surprised to find another\l"
	.string "ARCEUS in this same dimension.\p"
	.string "Where did the EGG that STEVEN has\n"
	.string "given me come from?$"


@KING!

NavelRock_Unown_Room_1_Third_Trigger::
	lock
	goto_if_gt VAR_TEST_DARK, 2, Debug_Script_1_Closemessage
	setvar VAR_TEST_DARK, 3
	playbgm MUS_DP_HALL_OF_ORIGIN, TRUE
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_K, LOCALID_K_Movement
	applymovement LOCALID_I, LOCALID_I_Movement
	applymovement LOCALID_N, LOCALID_N_Movement
	applymovement LOCALID_G, LOCALID_G_Movement
	applymovement LOCALID_EM, LOCALID_EM_Movement
	waitmovement 0
	removeobject LOCALID_K, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	removeobject LOCALID_I, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	removeobject LOCALID_N, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	removeobject LOCALID_G, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	removeobject LOCALID_EM, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	setflag FLAG_TEST_HIDE_2
	release
	end

LOCALID_K_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

LOCALID_I_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

LOCALID_N_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LOCALID_G_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end


LOCALID_EM_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

@BATTLE 2

NavelRock_Unown_Room_1_Final_Trigger::
	lock
	goto_if_gt VAR_TEST_DARK, 3, Debug_Script_1_Closemessage
	playmoncry SPECIES_UNOWN, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1   @ vertical pan
	setvar VAR_0x8005, 1   @ horizontal pan
	setvar VAR_0x8006, 32  @ num shakes
	setvar VAR_0x8007, 2   @ shake delay
	special ShakeCamera
	waitstate
	msgbox NavelRock_Unown_Room_1_Final_Trigger_Text, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, NavelRock_Unown_Room_1_Final_Trigger_No
	case MULTI_B_PRESSED, NavelRock_Unown_Room_1_Final_Trigger_No
	closemessage
	playbgm MUS_DP_LEGEND_APPEARS, FALSE
	applymovement LOCALID_IT, LOCALID_IT_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_TEST, 80
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_0x8000, 999 @HP
	setvar VAR_0x8002, 200 @DEF
	setvar VAR_0x8003, 999 @SPD
	setvar VAR_0x8005, 200 @SP. ATK
	setvar VAR_0x8006, 150 @SP. DEF
	special SetHpStat
	special SetAtkStat
	special SetDefStat
	special SetSpAtkStat
	special SetSpDefStat
	special SetSpdStat
	special BattleSetup_StartLegendaryBattle
	waitstate
	calculatemonstats
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, NavelRock_Unown_Room_1_Final_Trigger_Battle_Caught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, NavelRock_Unown_Room_1_Final_Trigger_Battle_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, NavelRock_Unown_Room_1_Final_Trigger_Battle_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, NavelRock_Unown_Room_1_Final_Trigger_Battle_Ran
	release
	end

LOCALID_IT_Movement:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	face_down
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	step_end

LOCALID_IT_Movement_No:
	walk_up
	face_up
	step_end

NavelRock_Unown_Room_1_Final_Trigger_No::
	applymovement OBJ_EVENT_ID_PLAYER, Player_Movement_1
	waitmovement 0
	closemessage
	end

NavelRock_Unown_Room_1_Final_Trigger_Battle_Ran::
	applymovement LOCALID_IT, LOCALID_IT_Movement_No
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Player_Movement_1
	waitmovement 0
	closemessage
	end

NavelRock_Unown_Room_1_Final_Trigger_Battle_Caught::
	setvar VAR_TEST_DARK, 4
	playbgm MUS_DUMMY, TRUE
	setvar VAR_0x8000, 0
	setvar VAR_0x8001, 0
	setvar VAR_0x8002, 0
	setvar VAR_0x8003, 0
	setvar VAR_0x8005, 0
	setvar VAR_0x8006, 0
	setflag FLAG_TEST_CAPTURED
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_IT, MAP_NAVEL_ROCK_UNOWN_ROOM_1
	fadescreen FADE_FROM_BLACK
	playbgm MUS_HG_CREDITS, TRUE
	end

NavelRock_Unown_Room_1_Final_Trigger_Text:
	.string "The ground trembles beneath your\n"
	.string "feet, and the very air seems to\l"
	.string "twist and shiver.\p"
	.string "It exists.\p"
	.string "Legends once dismissed as stories\n"
	.string "take form before your eyes…\p"
	.string "Are you ready to face the myth\n"
	.string "in your final challenge?$"

NavelRock_Unown_Room_1_Ending_Trigger::
	goto_if_gt VAR_TEST_DARK, 4, Debug_Script_1_Closemessage
	setvar VAR_TEST_DARK, 5
	msgbox NavelRock_Unown_Room_1_Ending_Trigger_Text, MSGBOX_DEFAULT
	closemessage
	delay 40
	delay 40
	warp MAP_NAVEL_ROCK_UNOWN_CHAMBER, 12, 12
	end

NavelRock_Unown_Room_1_Ending_Trigger_Text:
	.string "{COLOR GREEN}CONGRATULATIONS! You beat the\n"
	.string "secret event! If you got here there's\l"
	.string "not much else to do.\p"
	.string "Enjoy your newly discovered POKéMON\n"
	.string "and don't spoil too much this event\l"
	.string "to others! You can leave them hints!\p"
	.string "Thank you for playing POKéMON\n"
	.string "MODERN EMERALD,\p"
	.string "with gratitude,\p"
	.string "its creator,\p"
	.string "Resetes12.$"
