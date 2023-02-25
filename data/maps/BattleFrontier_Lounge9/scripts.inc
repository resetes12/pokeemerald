BattleFrontier_Lounge9_MapScripts::
	.byte 0

BattleFrontier_Lounge9_Heal::
	msgbox BattleFrontier_Lounge9_Heal_TEXT, MSGBOX_NPC
	waitmessage
	fadescreen 0x1
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	fadescreen 0x0
	msgbox BattleFrontier_Lounge9_Heal_TEXT2, MSGBOX_NPC
	waitmessage
	release
	end

BattleFrontier_Lounge9_Heal_TEXT:
	.string "Hi! Welcome to the HYPER TRAINING\n"
	.string "GYM! I will heal your POKéMON.$"

BattleFrontier_Lounge9_Heal_TEXT2:
	.string "We hope to see you again!$"

BattleFrontier_Lounge9_EV_No:
	msgbox BattleFrontier_Lounge9_EV_No_TEXT, MSGBOX_DEFAULT
	waitmessage
	release
	end

@EV Gym
BattleFrontier_Lounge9_EV_HP::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge9_EV_HP_TEXT, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_HP_Yes
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_No
	end

BattleFrontier_Lounge9_EV_HP_Yes::
	msgbox BattleFrontier_Lounge9_EV_HP_TEXT2, MSGBOX_DEFAULT
	waitmessage
	setflag FLAG_SYS_NO_CATCHING
	setwildbattle SPECIES_LICKITUNG, 1, ITEM_NONE
	dowildbattle
	clearflag FLAG_SYS_NO_CATCHING
	release
	end
	
BattleFrontier_Lounge9_EV_ATK::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge9_EV_ATK_TEXT, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_ATK_Yes
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_No
	end

BattleFrontier_Lounge9_EV_ATK_Yes::
	msgbox BattleFrontier_Lounge9_EV_ATK_TEXT2, MSGBOX_DEFAULT
	waitmessage
	setflag FLAG_SYS_NO_CATCHING
	setwildbattle SPECIES_HITMONLEE, 1, ITEM_NONE
	dowildbattle
	clearflag FLAG_SYS_NO_CATCHING
	release
	end

BattleFrontier_Lounge9_EV_SATK::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge9_EV_SATK_TEXT, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_SATK_Yes
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_No
	end

BattleFrontier_Lounge9_EV_SATK_Yes::
	msgbox BattleFrontier_Lounge9_EV_SATK_TEXT2, MSGBOX_DEFAULT
	waitmessage
	setflag FLAG_SYS_NO_CATCHING
	setwildbattle SPECIES_GENGAR, 1, ITEM_NONE
	dowildbattle
	clearflag FLAG_SYS_NO_CATCHING
	release
	end

BattleFrontier_Lounge9_EV_DEF::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge9_EV_DEF_TEXT, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_DEF_Yes
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_No
	end

BattleFrontier_Lounge9_EV_DEF_Yes::
	msgbox BattleFrontier_Lounge9_EV_DEF_TEXT2, MSGBOX_DEFAULT
	waitmessage
	setflag FLAG_SYS_NO_CATCHING
	setwildbattle SPECIES_STEELIX, 1, ITEM_NONE
	dowildbattle
	clearflag FLAG_SYS_NO_CATCHING
	release
	end

BattleFrontier_Lounge9_EV_SDEF::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge9_EV_SDEF_TEXT, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_SDEF_Yes
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_No
	end

BattleFrontier_Lounge9_EV_SDEF_Yes::
	msgbox BattleFrontier_Lounge9_EV_SDEF_TEXT2, MSGBOX_DEFAULT
	waitmessage
	setflag FLAG_SYS_NO_CATCHING
	setwildbattle SPECIES_HITMONCHAN, 1, ITEM_NONE
	dowildbattle
	clearflag FLAG_SYS_NO_CATCHING
	release
	end

BattleFrontier_Lounge9_EV_SPD::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge9_EV_SPD_TEXT, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_SPD_Yes
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_No
	end

BattleFrontier_Lounge9_EV_SPD_Yes::
	msgbox BattleFrontier_Lounge9_EV_SPD_TEXT2, MSGBOX_DEFAULT
	waitmessage
	setflag FLAG_SYS_NO_CATCHING
	setwildbattle SPECIES_FURRET, 1, ITEM_NONE
	dowildbattle
	clearflag FLAG_SYS_NO_CATCHING
	release
	end

BattleFrontier_Lounge9_EV_Column::
	msgbox BattleFrontier_Lounge9_EV_Column_Text, MSGBOX_SIGN
	end

@EV RESET
BattleFrontier_Lounge9_EV_SetZero::
	lockall
	faceplayer
	msgbox BattleFrontier_Lounge9_EV_SetZeroText, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroMoney
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroNo
	end
	
BattleFrontier_Lounge9_EV_SetZeroMoney::
	msgbox BattleFrontier_Lounge9_EV_SetZeroMoneyText, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroCheckMoney
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroNo
	end

BattleFrontier_Lounge9_EV_SetZeroCheckMoney::
	checkmoney 10000, 0
	compare VAR_RESULT, TRUE
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroYes
	compare VAR_RESULT, FALSE
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroNoMoney
	end

BattleFrontier_Lounge9_EV_SetZeroYes::
	msgbox BattleFrontier_Lounge9_EV_SetZeroText2, MSGBOX_DEFAULT
	waitmessage
	closemessage
	special ChoosePartyMon
	waitstate
	compare VAR_0x8004, PARTY_SIZE
	goto_if_ge BattleFrontier_Lounge9_EV_SetZeroNo
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	compare VAR_RESULT, SPECIES_EGG
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroEgg
	goto BattleFrontier_Lounge9_EV_SetZeroSure
	end

BattleFrontier_Lounge9_EV_SetZeroNo::
	msgbox BattleFrontier_Lounge9_EV_SetZeroTextNo, MSGBOX_DEFAULT
	waitmessage
	releaseall
	end

BattleFrontier_Lounge9_EV_SetZeroNoMoney::
	msgbox BattleFrontier_Lounge9_EV_SetZeroTextNoMoney, MSGBOX_DEFAULT
	waitmessage
	releaseall
	end

BattleFrontier_Lounge9_EV_SetZeroEgg::
	msgbox BattleFrontier_Lounge9_EV_SetZeroTextEgg, MSGBOX_DEFAULT
	waitmessage
	releaseall
	end

BattleFrontier_Lounge9_EV_SetZeroSure::
	faceplayer
	msgbox BattleFrontier_Lounge9_EV_SetZeroTextSure, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroProceed
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroNo
	end

BattleFrontier_Lounge9_EV_SetZeroProceed::
	faceplayer
	fadescreen 0x1
	delay 30
	removemoney 5000, 0
	fadescreen 0x0
	setvar VAR_0x8000, 0
	setvar VAR_0x8001, 0
	setvar VAR_0x8002, 0
	setvar VAR_0x8003, 0
	setvar VAR_0x8005, 0
	setvar VAR_0x8006, 0
	special SetHpEvs
	special SetAtkEvs
	special SetDefEvs
	special SetSpdEvs
	special SetSpAtkEvs
	special SetSpDefEvs
	msgbox BattleFrontier_Lounge9_EV_SetZeroText3, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

@IV setter to 31 (all of them)
BattleFrontier_Lounge9_IV_Man::
	checkflag FLAG_IV_SETTER
	lockall
	faceplayer
	goto_if_unset FLAG_IS_CHAMPION, BattleFrontier_Lounge9_IV_ManNoChampion
	call_if_unset FLAG_IV_SETTER, BattleFrontier_Lounge9_IV_First_Time
	msgbox BattleFrontier_Lounge9_IV_Text1, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_IV_CheckMoney
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_IV_No
	end

BattleFrontier_Lounge9_IV_ManNoChampion::
	msgbox BattleFrontier_Lounge9_IV_TextNoChampion, MSGBOX_NPC
	releaseall
	end

BattleFrontier_Lounge9_IV_TextNoChampion:
	.string "Who are you? You seem... Weak?\n"
	.string "Kinda...\p"
	.string "Meh. Come back if you get stronger.$"

BattleFrontier_Lounge9_IV_First_Time::
	msgbox BattleFrontier_Lounge9_IV_TextFirstTime, MSGBOX_NPC
	setflag FLAG_IV_SETTER
	releaseall
	end

BattleFrontier_Lounge9_IV_No::
	msgbox BattleFrontier_Lounge9_IV_Text_No, MSGBOX_DEFAULT
	waitmessage
	releaseall
	end

BattleFrontier_Lounge9_IV_CheckMoney::
	checkmoney 60000, 0
	compare VAR_RESULT, TRUE
	goto_if_eq BattleFrontier_Lounge9_IV_Continue
	compare VAR_RESULT, FALSE
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroNoMoney
	end

BattleFrontier_Lounge9_IV_Continue::
	faceplayer
	closemessage
	special ChoosePartyMon
	waitstate
	compare VAR_0x8004, PARTY_SIZE
	goto_if_ge BattleFrontier_Lounge9_IV_No
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	compare VAR_RESULT, SPECIES_EGG
	goto_if_eq BattleFrontier_Lounge9_EV_SetZeroEgg
	goto BattleFrontier_Lounge9_IV_Continue2
	end

BattleFrontier_Lounge9_IV_Continue2::
	faceplayer
	msgbox BattleFrontier_Lounge9_IV_Continue_Text, MSGBOX_YESNO
	waitmessage
	compare VAR_RESULT, YES
	goto_if_eq BattleFrontier_Lounge9_IV_SetMax
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_Lounge9_IV_No
	end
	releaseall
	end

BattleFrontier_Lounge9_IV_SetMax::
	faceplayer
	fadescreen 0x1
	delay 100
	removemoney 60000, 0
	fadescreen 0x0
	setvar VAR_0x8007, 31
	setvar VAR_0x8008, 31
	setvar VAR_0x8009, 31
	setvar VAR_0x800A, 31
	setvar VAR_0x800B, 31
	setvar VAR_UNUSED_0x8014, 31
	special SetHpIvs
	special SetAtkIvs
	special SetDefIvs
	special SetSpdIvs
	special SetSpAtkIvs
	special SetSpDefIvs
	msgbox BattleFrontier_Lounge9_IV_Finish, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

BattleFrontier_Lounge9_IV_TextFirstTime:
	.string "Hey! It's you! Nice to\n"
	.string "see you around here!\p"
	.string "Wait... You don't know me?\p"
	.string ". . .\p"
	.string ". . .\p"
	.string "Oh... Of course! I saw you on\n"
	.string "the TV a short while ago!\p"
	.string "You are {PLAYER} and you recently\n"
	.string "won the POKéMON LEAGUE, right?\p"
	.string "Your last combat was sooo amazing...\p"
	.string "I learned so many things from you\n"
	.string "that I decided to join your FAN CLUB!\p"
	.string "Anyway, I decided to become stronger\n"
	.string "so I joined this GYM a a few days ago.\p"
	.string "My dad works here in this city so it's\n"
	.string "easy for me to come here every day.\p"
	.string "I love training in this GYM, since\n"
	.string "it's always full of powerful trainers!\p"
	.string "In fact, here I developed a technique\n"
	.string "to maximize the potential of POKéMON.\p"
	.string "If you want we can train together to\n"
	.string "maximize the potential of your POKéMON.\p"
	.string "Do you want to try it?\p"
	.string "It's a lenghty and difficult process, so\n"
	.string "I need 60.000¥ to prepare everything\p"
	.string "Tell me when you are ready!$"

BattleFrontier_Lounge9_IV_Text1:
	.string "Hi! What's up?\p"
	.string "Ready for the best training\n"
	.string "routine of all HOENN?$"

BattleFrontier_Lounge9_IV_Continue_Text:
	.string "Are you sure that this \n"
	.string "is the correct POKéMON?$"

BattleFrontier_Lounge9_IV_Text_No:
	.string "Oh... Well, see you another time.$"

BattleFrontier_Lounge9_IV_Finish:
	.string "Phew... I'm exhausted...\n"
	.string "What about your POKéMON?\p"
	.string "You should REST a bit\n"
	.string "before going.\p"
	.string "Well, see you soon!$"

BattleFrontier_Lounge9_EV_SetZeroText:
	.string "Hello there, trainer!\p"
	.string "I'm known as EV Man, and I'm\n"
	.string "the owner of this gym.\p"
	.string "If you want to reset your POKéMON\n"
	.string "EVs to zero, just talk to me.\p"
	.string "So, do you want to reset any\n"
	.string "POKéMON EFFORT VALUE?$"

BattleFrontier_Lounge9_EV_SetZeroText2:
	.string "Alright, which POKéMON EVs\n"
	.string "should I reset to zero?$"

BattleFrontier_Lounge9_EV_SetZeroText3:
	.string "Phew! It was hard, but I managed\n"
	.string "it! Its EVs are now zero!$"

BattleFrontier_Lounge9_EV_SetZeroTextEgg:
	.string "Sorry, that won't work.$"

BattleFrontier_Lounge9_EV_SetZeroTextNo:
	.string "Alright, come back if\n"
	.string "you need to!$"

BattleFrontier_Lounge9_EV_SetZeroMoneyText:
	.string "This process is quite difficult.\n"
	.string "It will cost you 10.000¥.\p"
	.string "Do you want to continue?$"

BattleFrontier_Lounge9_EV_SetZeroTextNoMoney:
	.string "You don't have enough money.\p"
	.string "Come back when you have.$"

BattleFrontier_Lounge9_EV_SetZeroTextSure:
	.string "Are you sure you want to reset\n"
	.string "this POKéMON EV values?\p"
	.string "This process can't be undone.$"

BattleFrontier_Lounge9_EV_Column_Text:
	.string "Welcome to the HYPER TRAINING GYM!\p"
	.string "Here you can train your POKéMON\n"
	.string "to improve their STATS.\p"
	.string "How does this work?\p"
	.string "Win one battle and obtain EFFORT VALUES\n"
	.string "for the selected stat of your POKéMON.\p"
	.string "TRAINER TIP 1:\p"
	.string "Reset EVs to zero by talking\n"
	.string "to the EV Man.\p"
	.string "TRAINER TIP 2:\p"
	.string "Equip a MACHO BRACE to multiply\n"
	.string "by 10 the EVs gained.\p"
	.string "TRAINER TIP 3:\p"
	.string "A new type of training is available\n"
	.string "at the right corner of the gym.$"

BattleFrontier_Lounge9_EV_HP_TEXT:
	.string "Hi ya buddy! I can train your\n"
	.string "POKéMON in HEALTH POINTS.\p"
	.string "Are you ready?$"

BattleFrontier_Lounge9_EV_HP_TEXT2:
	.string "Go LICKITUNG!$"

BattleFrontier_Lounge9_EV_ATK_TEXT:
	.string "OSHA! I can train your POKéMON\n"
	.string "in ATTACK.\p"
	.string "Are you ready?$"

BattleFrontier_Lounge9_EV_ATK_TEXT2:
	.string "Go HITMONLEE!$"

BattleFrontier_Lounge9_EV_SATK_TEXT:
	.string "Bonjour! I can train your POKéMON\n"
	.string "in SPECIAL ATTACK.\p"
	.string "Are you ready?$"

BattleFrontier_Lounge9_EV_SATK_TEXT2:
	.string "Go GENGAR!$"
	
BattleFrontier_Lounge9_EV_DEF_TEXT:
	.string "Sup' kid! I can train your\n"
	.string "POKéMON in DEFENSE.\p"
	.string "Are you ready?$"

BattleFrontier_Lounge9_EV_DEF_TEXT2:
	.string "Go STEELIX!$"

BattleFrontier_Lounge9_EV_SDEF_TEXT:
	.string "Bonjour mon trainer! I can train your\n"
	.string "POKéMON in SPECIAL DEFENSE.\p"
	.string "Are you ready?$"

BattleFrontier_Lounge9_EV_SDEF_TEXT2:
	.string "Go HITMONCHAN!$"

BattleFrontier_Lounge9_EV_SPD_TEXT:
	.string ". . .\l"
	.string "I can train your POKéMON\n"
	.string "in SPEED.\p"
	.string "Are you ready?$"

BattleFrontier_Lounge9_EV_SPD_TEXT2:
	.string "Go FURRET!$"

BattleFrontier_Lounge9_EV_No_TEXT:
	.string "No? Okay.$"

BattleFrontier_Lounge9_Chansey5::
	faceplayer
	goto_if_unset FLAG_CHANSEYTRAINERS, LavaridgeTown_ChanseyTrainersFirstEncounter
	goto_if_unset FLAG_IS_CHAMPION, BattleFrontier_Lounge9_Chansey5Champion
	msgbox BattleFrontier_Lounge9_Chansey5Repeat2, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, LavaridgeTown_Chansey1End
	case YES, BattleFrontier_Lounge9_Chansey5Battle
	case MULTI_B_PRESSED, LavaridgeTown_Chansey1End

BattleFrontier_Lounge9_Chansey5Battle::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_CHANSEY5, 0, BattleFrontier_Lounge9_Chansey5Intro, BattleFrontier_Lounge9_Chansey5Defeat, BattleFrontier_Lounge9_ChanseyBattleAgain
	end

BattleFrontier_Lounge9_ChanseyBattleAgain::
	faceplayer
	msgbox BattleFrontier_Lounge9_Chansey5Repeat, MSGBOX_AUTOCLOSE
	clearflag 0x85B
	end

BattleFrontier_Lounge9_Chansey5Champion:
	faceplayer
	msgbox BattleFrontier_Lounge9_Chansey5ChampionText, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge9_Chansey5ChampionText:
	.string "How many badges you have?\p"
	.string "Oh my! No, I cannot battle you!\n"
	.string "You are not strong enough.\p"
	.string "Come back when you get stronger.$"

BattleFrontier_Lounge9_Chansey5Intro:
	.string "My BLISSEY is ready!$"

BattleFrontier_Lounge9_Chansey5Defeat:
	.string "Oh no... BLISSEY, use SOFT-BOILED!$"

BattleFrontier_Lounge9_Chansey5Repeat:
	.string "We can battle again if you want!$"

BattleFrontier_Lounge9_Chansey5Repeat2:
	.string "Ready for a battle?$"
