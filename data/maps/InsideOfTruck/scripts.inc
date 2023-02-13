InsideOfTruck_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, InsideOfTruck_OnLoad
	map_script MAP_SCRIPT_ON_RESUME, InsideOfTruck_OnResume
	.byte 0

InsideOfTruck_OnLoad:
	setmetatile 4, 1, METATILE_InsideOfTruck_ExitLight_Top, FALSE
	setmetatile 4, 2, METATILE_InsideOfTruck_ExitLight_Mid, FALSE
	setmetatile 4, 3, METATILE_InsideOfTruck_ExitLight_Bottom, FALSE
	end

InsideOfTruck_OnResume:
	setstepcallback STEP_CB_TRUCK
	end

InsideOfTruck_EventScript_SetIntroFlags::
	lockall
	setflag FLAG_HIDE_MAP_NAME_POPUP
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, InsideOfTruck_EventScript_SetIntroFlagsMale
	goto_if_eq VAR_RESULT, FEMALE, InsideOfTruck_EventScript_SetIntroFlagsFemale
	end

InsideOfTruck_EventScript_SetIntroFlagsMale::
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F
	setvar VAR_LITTLEROOT_INTRO_STATE, 1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 1
	setdynamicwarp MAP_LITTLEROOT_TOWN, 3, 10
	goto InsideOfTruck_EventScript_Challenge
	releaseall
	end

InsideOfTruck_EventScript_SetIntroFlagsFemale::
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_MAYS_HOUSE_2F
	setvar VAR_LITTLEROOT_INTRO_STATE, 2
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 1
	setdynamicwarp MAP_LITTLEROOT_TOWN, 12, 10
	goto InsideOfTruck_EventScript_Challenge
	releaseall
	end

InsideOfTruck_EventScript_MovingBox::
	msgbox InsideOfTruck_Text_BoxPrintedWithMonLogo, MSGBOX_SIGN
	end

InsideOfTruck_Text_BoxPrintedWithMonLogo:
	.string "The box is printed with a POKéMON logo.\p"
	.string "It's a POKéMON brand moving and\n"
	.string "delivery service.$"

InsideOfTruck_EventScript_Challenge::
	message InsideOfTruck_Text_Challenge
	waitmessage
	switch VAR_DIFFICULTY
	case 0, InsideOfTruck_EventScript_DefaultNormal
	case 1, InsideOfTruck_EventScript_DefaultHard
	case 2, InsideOfTruck_EventScript_DefaultEasy

InsideOfTruck_EventScript_DefaultNormal:
	multichoicedefault 21, 6, MULTI_TAG_DIFFICULTY, 1, FALSE
	goto InsideOfTruck_EventScript_ChallengeSelect

InsideOfTruck_EventScript_DefaultHard:
	multichoicedefault 21, 6, MULTI_TAG_DIFFICULTY, 2, FALSE
	goto InsideOfTruck_EventScript_ChallengeSelect

InsideOfTruck_EventScript_DefaultEasy:
	multichoicedefault 21, 6, MULTI_TAG_DIFFICULTY, 0, FALSE
	goto InsideOfTruck_EventScript_ChallengeSelect

InsideOfTruck_EventScript_ChallengeSelect:
	switch VAR_RESULT
	case 0, InsideOfTruck_EventScript_SelectedEasy
	case 1, InsideOfTruck_EventScript_SelectedNormal
	case 2, InsideOfTruck_EventScript_SelectedHard
	case MULTI_B_PRESSED, InsideOfTruck_EventScript_SelectedCancel
	end

InsideOfTruck_EventScript_SelectedNormal:
	msgbox InsideOfTruck_Text_SelectedNormal, MSGBOX_DEFAULT
	setvar VAR_DIFFICULTY, 0
	closemessage
	end

InsideOfTruck_EventScript_SelectedHard:
	msgbox InsideOfTruck_Text_SelectedHard, MSGBOX_DEFAULT
	setvar VAR_DIFFICULTY, 1
	closemessage
	end

InsideOfTruck_EventScript_SelectedEasy:
	msgbox InsideOfTruck_Text_SelectedEasy, MSGBOX_DEFAULT
	setvar VAR_DIFFICULTY, 2
	closemessage
	end

InsideOfTruck_EventScript_SelectedCancel:
	msgbox InsideOfTruck_Text_SelectedCancel, MSGBOX_DEFAULT
	closemessage
	end

InsideOfTruck_Text_Challenge:
	.string "What difficulty would\n"
	.string "you like to play at?$"

InsideOfTruck_Text_SelectedEasy:
	.string "Easy difficulty\n"
	.string "was chosen.$"

InsideOfTruck_Text_SelectedNormal:
	.string "Normal difficulty\n"
	.string "was chosen.$"

InsideOfTruck_Text_SelectedHard:
	.string "Hard difficulty\n"
	.string "was chosen.$"

InsideOfTruck_Text_SelectedCancel:
	.string "Difficulty is\n"
	.string "unchanged.$"
