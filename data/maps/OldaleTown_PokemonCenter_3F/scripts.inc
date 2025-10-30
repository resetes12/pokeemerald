OldaleTown_PokemonCenter_3F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, WonderTrade_OnFrame
	.byte 0

WonderTrade_OnFrame:
	map_script_2 VAR_WONDERTRADE, 0, EventScript_FirstTimeWonder_Unlimited  @0 is unlimited
	map_script_2 VAR_WONDERTRADE, 1, EventScript_FirstTimeWonder_Limited    @1 is limited
	.2byte 0															    @2 doesn't trigger first time text

SetWonderTradeModeLimited:
	setvar VAR_WONDERTRADE, 1
	return

EventScript_FirstTimeWonder_Unlimited: @0 is unlimited
	lockall
	msgbox EventScript_FirstTimeWonder_Text_Welcome, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Wondertrade_Movement_PlayerApproachCounter
	waitmovement 0
	delay 30
	msgbox EventScript_FirstTimeWonder_Text_Unlimited, MSGBOX_DEFAULT
	closemessage
	setvar VAR_WONDERTRADE, 2 @2 doesn't trigger first time text
	releaseall
	end

EventScript_FirstTimeWonder_Limited: @1 is limited
	lockall
	msgbox EventScript_FirstTimeWonder_Text_Welcome, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Wondertrade_Movement_PlayerApproachCounter
	waitmovement 0
	delay 30
	msgbox EventScript_FirstTimeWonder_Text, MSGBOX_DEFAULT
	closemessage
	setvar VAR_WONDERTRADE, 2 @2 doesn't trigger first time text
	releaseall
	end

Wondertrade_Movement_PlayerApproachCounter:
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	step_end

EventScript_DoWonderTrade::
	goto_if_set FLAG_UNLIMITIED_WONDERTRADE, EventScript_DoWonderTradeStartNoLimit
	special CheckIfRandomizerIsActive
	goto_if_unset FLAG_WT_ENABLED_RANDOMIZER, EventScript_DoWonderTradeStart
	special CheckIfChallengesAreActive
	goto_if_set FLAG_WONDERTRADE3, EventScript_NoMoreForToday
	goto_if_set FLAG_NO_WT_BECAUSE_CHALLENGE, EventScript_WTChallenges
	goto_if_unset FLAG_WONDERTRADE1, EventScript_DoWonderTradeFirstTrade	
	goto_if_unset FLAG_WONDERTRADE2, EventScript_DoWonderTradeSecondTrade
	goto_if_unset FLAG_WONDERTRADE3, EventScript_DoWonderTradeThirdTrade
	end

EventScript_DoWonderTradeStart:
	goto_if_unset FLAG_BADGE05_GET, EventScript_WTNotActiveYet
	msgbox EventScript_DoWonderTrade_Text_Welcome, MSGBOX_DEFAULT
	closemessage
	special CheckIfChallengesAreActive
	goto_if_set FLAG_WONDERTRADE3, EventScript_NoMoreForToday
	goto_if_set FLAG_NO_WT_BECAUSE_CHALLENGE, EventScript_WTChallenges
	goto_if_unset FLAG_WONDERTRADE1, EventScript_DoWonderTradeFirstTrade	
	goto_if_unset FLAG_WONDERTRADE2, EventScript_DoWonderTradeSecondTrade
	goto_if_unset FLAG_WONDERTRADE3, EventScript_DoWonderTradeThirdTrade
	end

EventScript_WTChallenges::
	goto_if_unset FLAG_SYS_GAME_CLEAR, EventScript_WTChallengesCancel
	goto_if_unset FLAG_WONDERTRADE1, EventScript_DoWonderTradeFirstTrade	
	goto_if_unset FLAG_WONDERTRADE2, EventScript_DoWonderTradeSecondTrade
	goto_if_unset FLAG_WONDERTRADE3, EventScript_DoWonderTradeThirdTrade
	end

EventScript_WTChallengesCancel::
	msgbox EventScript_DoWonderTrade_Text_Challenges, MSGBOX_DEFAULT
	closemessage
	end

EventScript_WTNotActiveYet::
	msgbox EventScript_DoWonderTrade_Text_FiveBadges, MSGBOX_DEFAULT
	closemessage
	end

EventScript_DoWonderTradeFirstTrade::
	setflag FLAG_WONDERTRADE1
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, EventScript_DoWonderTradeCancelSaveFirstTrade
	special ChoosePartyMon
	waitstate
	goto EventScript_DoWonderTradeTrade

EventScript_DoWonderTradeCancelSaveFirstTrade::
	clearflag FLAG_WONDERTRADE1
	msgbox EventScript_DoWonderTrade_Text_Done, MSGBOX_DEFAULT
	closemessage
	end

EventScript_DoWonderTradeSecondTrade::
	setflag FLAG_WONDERTRADE2
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, EventScript_DoWonderTradeCancelSaveSecondTrade
	special ChoosePartyMon
	waitstate
	goto EventScript_DoWonderTradeTrade

EventScript_DoWonderTradeCancelSaveSecondTrade::
	clearflag FLAG_WONDERTRADE2
	msgbox EventScript_DoWonderTrade_Text_Done, MSGBOX_DEFAULT
	closemessage
	end

EventScript_DoWonderTradeThirdTrade::
	setflag FLAG_WONDERTRADE3
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, EventScript_DoWonderTradeCancelSaveThirdTrade
	special ChoosePartyMon
	waitstate
	goto EventScript_DoWonderTradeTrade

EventScript_DoWonderTradeCancelSaveThirdTrade::
	clearflag FLAG_WONDERTRADE3
	msgbox EventScript_DoWonderTrade_Text_Done, MSGBOX_DEFAULT
	closemessage
	end

EventScript_DoWonderTradeStartNoLimit::
	goto_if_unset FLAG_BADGE05_GET, EventScript_WTNotActiveYet
	msgbox EventScript_DoWonderTrade_Text_Welcome, MSGBOX_DEFAULT
	closemessage
	special CheckIfChallengesAreActive
	goto_if_set FLAG_NO_WT_BECAUSE_CHALLENGE, EventScript_WTChallengesUnlimited
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, EventScript_DoWonderTradeCancelSaveThirdTrade
	special ChoosePartyMon
	waitstate
	goto EventScript_DoWonderTradeTrade

EventScript_WTChallengesUnlimited::
	goto_if_unset FLAG_SYS_GAME_CLEAR, EventScript_WTChallengesCancel
	return

EventScript_DoWonderTradeTrade::
	compare VAR_0x8004, PARTY_SIZE
	goto_if_ge EventScript_End
	copyvar VAR_0x8005, VAR_0x8004
	special CreateWonderTradePokemon
	special DoInGameTradeScene
	waitstate
	special ForceSaveGame
	waitstate
	msgbox EventScript_DoWonderTrade_Text_Done, MSGBOX_DEFAULT
	closemessage
	end

EventScript_NoMoreForToday::
	msgbox EventScript_NoMoreForToday_Text, MSGBOX_DEFAULT
	closemessage
	end

EventScript_End::
	end

EventScript_FirstTimeWonder_Text_Welcome:
	.string "Hello!\p"
	.string "This must be your first time\n"
	.string "down here.\p"
	.string "I'll show you how the WONDERTRADE\n"
	.string "STATION of the POKéCENTER works.\p"
	.string "First, I need to show you this\n"
	.string "floor of our POKéMON CENTER.\p"
	.string "Right this way, please.$"

EventScript_FirstTimeWonder_Text:
	.string "Let me explain how the WONDERTRADE\n"
	.string "STATION works.\p"
	.string "WONDERTRADE is a new technology\n"
	.string "developed by DEVON CORP, and it\l"
	.string "lets you trade anonymously.\p"
	.string "Send a POKéMON and receive one\n"
	.string "from someone else in return!\p"
	.string "You won’t know what you’ll get,\n"
	.string "and neither will they.\l"
	.string "That’s the fun!\p"
	.string "To avoid server overload, only 3\n"
	.string "WONDERTRADES can be done per day.\p"
	.string "Enjoy your WONDERTRADE!$"

EventScript_FirstTimeWonder_Text_Unlimited:
	.string "Let me explain how the WONDERTRADE\n"
	.string "STATION works.\p"
	.string "WONDERTRADE is a new technology\n"
	.string "developed by DEVON CORP, and it\l"
	.string "lets you trade anonymously.\p"
	.string "Send a POKéMON and receive one\n"
	.string "from someone else in return!\p"
	.string "You won’t know what you’ll get,\n"
	.string "and neither will they.\l"
	.string "That’s the fun!\p"
	.string "Enjoy your WONDERTRADE!$"

EventScript_DoWonderTrade_Text_Welcome:
	.string "Welcome to the WONDERTRADE STATION\n"
	.string "of the POKéCENTER!\p"
	.string "Before selecting which POKéMON is\n"
	.string "going to get WONDERTRADED, I need\l"
	.string "save your game.$"

EventScript_NoMoreForToday_Text:
	.string "Oh! Seems that you have used all\n"
	.string "3 DAILY TRADES.\p"
	.string "Come back tomorrow!$"

EventScript_DoWonderTrade_Text_Done:
	.string "We hope you had a good\n"
	.string "WONDERTRADE!$"

EventScript_DoWonderTrade_Text_Challenges:
	.string "Looks like you have challenges\n"
	.string "activated. Please come back\l"
	.string "again after the POKéMON LEAGUE.$"

EventScript_DoWonderTrade_Text_FiveBadges:
	.string "Sadly, the WONDERTRADE SYSTEM is\n"
	.string "not active yet. Please do come\l"
	.string "back later.$"
