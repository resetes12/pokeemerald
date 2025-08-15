.set LOCALID_LUGIA, 1

NavelRock_Bottom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, NavelRock_Bottom_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, NavelRock_Bottom_OnResume
	.byte 0

NavelRock_Bottom_OnTransition:
	call_if_set FLAG_CAUGHT_LUGIA, NavelRock_Bottom_EventScript_HideLugia
	call_if_unset FLAG_CAUGHT_LUGIA, NavelRock_Bottom_EventScript_TryShowLugia
	end

NavelRock_Bottom_EventScript_HideLugia::
	setflag FLAG_HIDE_LUGIA
	return

NavelRock_Bottom_EventScript_TryShowLugia::
	goto_if_set FLAG_DEFEATED_LUGIA, Common_EventScript_NopReturn
	clearflag FLAG_HIDE_LUGIA
	return

NavelRock_Bottom_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, NavelRock_Bottom_EventScript_TryRemoveLugia
	end

NavelRock_Bottom_EventScript_TryRemoveLugia::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_LUGIA
	return

NavelRock_Bottom_EventScript_Lugia::
	lock
	faceplayer
	waitse
	playbgm MUS_HG_LUGIA_APPEARS, FALSE
	delay 20
	playse SE_THUNDERSTORM_STOP
	setvar VAR_0x8004, 0  @ vertical pan
	setvar VAR_0x8005, 3  @ horizontal pan
	setvar VAR_0x8006, 4  @ num shakes
	setvar VAR_0x8007, 2  @ shake delay
	special ShakeCamera
	delay 30
	playse SE_THUNDERSTORM_STOP
	setvar VAR_0x8004, 0  @ vertical pan
	setvar VAR_0x8005, 3  @ horizontal pan
	setvar VAR_0x8006, 4  @ num shakes
	setvar VAR_0x8007, 2  @ shake delay
	special ShakeCamera
	delay 30
	delay 50
	waitse
	setweather WEATHER_DOWNPOUR
	doweather
	playmoncry SPECIES_LUGIA, CRY_MODE_ENCOUNTER
	msgbox Lugia_Text, MSGBOX_NPC
	closemessage
	waitmoncry
	delay 20
	seteventmon SPECIES_LUGIA, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, NavelRock_Bottom_EventScript_DefeatedLugia
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, NavelRock_Bottom_EventScript_RanFromLugia
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, NavelRock_Bottom_EventScript_RanFromLugia
	setflag FLAG_CAUGHT_LUGIA
	setweather WEATHER_SHADE
	doweather
	call_if_set FLAG_EXTRA_LEGENDARIES, NavelRock_Bottom_Enable_Johto_Legendaries
	release
	end

NavelRock_Bottom_Enable_Johto_Legendaries::
	setflag FLAG_UNLOCK_DOGS
	playmoncry SPECIES_SUICUNE, CRY_MODE_NORMAL
	waitmoncry
	playmoncry SPECIES_ENTEI, CRY_MODE_NORMAL
	waitmoncry
	playmoncry SPECIES_RAIKOU, CRY_MODE_NORMAL
	waitmoncry
	msgbox Lugia_Legendaries_Text, MSGBOX_NPC
	closemessage
	return

NavelRock_Bottom_EventScript_DefeatedLugia::
	setweather WEATHER_SHADE
	doweather
	setflag FLAG_DEFEATED_LUGIA
	call_if_set FLAG_EXTRA_LEGENDARIES, NavelRock_Bottom_Enable_Johto_Legendaries
	setvar VAR_0x8004, SPECIES_LUGIA
	goto Common_EventScript_LegendaryFlewAway
	end

NavelRock_Bottom_EventScript_RanFromLugia::
	setweather WEATHER_SHADE
	doweather
	setvar VAR_0x8004, SPECIES_LUGIA
	goto Common_EventScript_LegendaryFlewAway
	end

Lugia_Text:
	.string "Grooaah!$"

Lugia_Legendaries_Text:
	.string "Three unknown roars echoed\n"
	.string "through NAVEL ROCK.$"
