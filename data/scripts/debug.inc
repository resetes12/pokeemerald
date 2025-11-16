Debug_ShowFieldMessageStringVar4::
	special ShowFieldMessageStringVar4
	waitmessage
	waitbuttonpress
	releaseall
	end

Debug_CheatStart::
	lockall
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_RESCUED_BIRCH
	setflag FLAG_HIDE_ROUTE_101_BIRCH_ZIGZAGOON_BATTLE
	setflag FLAG_ADVENTURE_STARTED
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_BIRCH
	setflag FLAG_HIDE_ROUTE_101_BIRCH_STARTERS_BAG
	setvar VAR_BIRCH_LAB_STATE, 2
	setvar VAR_ROUTE101_STATE, 3
	givemon SPECIES_TREECKO, 20, ITEM_NONE
	givemon SPECIES_TORCHIC, 20, ITEM_NONE
	givemon SPECIES_MUDKIP, 20, ITEM_NONE
	setflag FLAG_SYS_POKEDEX_GET
	special SetUnlockedPokedexFlags
	setflag FLAG_RECEIVED_POKEDEX_FROM_BIRCH
	setvar VAR_CABLE_CLUB_TUTORIAL_STATE, 1
	setflag FLAG_SYS_NATIONAL_DEX
	special EnableNationalPokedex
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	setvar VAR_LITTLEROOT_TOWN_STATE, 4 @ 4: Received Running Shoes
	setvar VAR_LITTLEROOT_INTRO_STATE, 7 @ 7: Told to go meet rival
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 2 @ 2: Met Rival's Mom (and is corresponding gender)
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 2 @ 2: Met Rival's Mom (and is corresponding gender)
	setvar VAR_LITTLEROOT_RIVAL_STATE, 4 @ 4: Received Pokedex
	setflag FLAG_RECEIVED_BIKE
	additem ITEM_ACRO_BIKE
	setvar VAR_BRINEY_HOUSE_STATE, 1
	setvar VAR_ROUTE116_STATE, 2
	setflag FLAG_HIDE_ROUTE_116_MR_BRINEY
	clearflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	clearflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	closemessage
	release
	end

Debug_Script_1::
	msgbox Debug_Script_1_Text, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, Debug_Script_1_Closemessage
	case MULTI_B_PRESSED, Debug_Script_1_Closemessage
	msgbox Debug_Script_1_Text2, MSGBOX_DEFAULT
	closemessage
	callnative Task_ChooseChallenge_NoNewGame
	end

Debug_Script_1_Closemessage::
	closemessage
	end

Debug_Script_1_Text:
	.string "{COLOR RED}WARNING! ENTERING THE CHALLENGES\n"
	.string "MENU COULD MESS YOUR SAVE DATA.\p"
	.string "SOME OPTIONS MAY NOT WORK UNLESS A\n"
	.string "NEW GAME IS STARTED. PLEASE BACKUP\p"
	.string "YOUR DATA BEFORE PRESSING YES.$"

Debug_Script_1_Text2:
	.string "{COLOR RED}ALL OPTIONS YOU CHOSE AT THE START\n"
	.string "OF THE GAME WILL RESET AND YOU\l"
	.string "WILL HAVE TO ENABLE THEM AGAIN.$"

@Update 2.4 up to 3.2
Debug_Script_2::
	msgbox Debug_Script_2_Text, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, Debug_Script_1_Closemessage
	case MULTI_B_PRESSED, Debug_Script_1_Closemessage
	callnative Update24to30
	special NameRival
	waitstate
	msgbox Debug_Script_2_Text_Which_WT, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, Debug_Script_2_Set_Unlimited_WT
	case NO, Debug_Script_2_Set_Limited_WT
	case MULTI_B_PRESSED, Debug_Script_2_Set_Limited_WT
	end

Debug_Script_2_Set_Limited_WT:
	setvar VAR_WONDERTRADE, 1
	msgbox Debug_Script_2_SelectFrontierBanOption_Text, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, Debug_Script_2_SelectFrontierBanOption_Zero
	case NO, Debug_Script_2_SelectFrontierBanOption_One
	case MULTI_B_PRESSED, Debug_Script_2_SelectFrontierBanOption_One
	end

Debug_Script_2_Set_Unlimited_WT:
	setvar VAR_WONDERTRADE, 0
	msgbox Debug_Script_2_SelectFrontierBanOption_Text, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, Debug_Script_2_SelectFrontierBanOption_Zero
	case NO, Debug_Script_2_SelectFrontierBanOption_One
	case MULTI_B_PRESSED, Debug_Script_2_SelectFrontierBanOption_One
	end

Debug_Script_2_SelectFrontierBanOption_Zero:
	callnative Update24to32_SetFrontierBansToZero
	closemessage
	warp MAP_PETALBURG_CITY, 16, 16
	end

Debug_Script_2_SelectFrontierBanOption_One:
	callnative Update24to32_SetFrontierBansToOne
	closemessage
	warp MAP_PETALBURG_CITY, 16, 16
	end

Debug_Script_2_Text:
	.string "{COLOR RED}WARNING! DON'T RUN THIS SCRIPT IF\n"
	.string "YOU'RE IN VERSION 3.3 OR UP!\p"
	.string "ONLY RUN THIS SCRIPT ONCE, FROM VERSION\n"
	.string "3.0 TO 3.2!\p"
	.string "This script will let you name your\n"
	.string "rival, fix other stuff, then teleport\l"
	.string "you to PETALBURG CITY, to fix maps.\p"
	.string "Continue?$"

Debug_Script_2_Text_Which_WT:
	.string "UNLIMITED WONDERTRADE option needs to\n"
	.string "be reset. Will you enable it?$"

Debug_Script_2_SelectFrontierBanOption_Text:
	.string "Would you like to enable the new\n"
	.string "option, FRONTIER BANS?\p"
	.string "According to your current difficulty,\n"
	.string "enabling it will ban legendaries from\l"
	.string "participating in the BATTLE FRONTIER.\p"
	.string "Disabling it will let lengendaries in,\n"
	.string "no matter your chosen difficulty.\p"
	.string "Would you like to enable the bans?\n"
	.string "{COLOR RED}(YES has been always the default option)$"

@Update 3.2 to 3.3
Debug_Script_3::
	msgbox Debug_Script_3_Text, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, Debug_Script_1_Closemessage
	case MULTI_B_PRESSED, Debug_Script_1_Closemessage
	case YES, Debug_Script_3_Update32to33_ShinyColors
	end

Debug_Script_3_Text::
	.string "{COLOR RED}WARNING! Don't run this script\n"
	.string "if you're NOT in VERSION 3.3 and you're\l"
	.string "updating FROM VERSION 3.2!\p"
	.string "Continue?$"

Debug_Script_3_Update32to33_ShinyColors::
	msgbox Debug_Script_3_Text_Shiny_Colors, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, Debug_Script_3_DisableShinyColors
	case MULTI_B_PRESSED, Debug_Script_3_DisableShinyColors
	case YES, Debug_Script_3_EnableShinyColors
	end

Debug_Script_3_DisableShinyColors::
	callnative Update32to33_SetShinyColorsToZero
	goto Debug_Script_3_Update32to33_TypeEffectiveness
	end

Debug_Script_3_EnableShinyColors::
	callnative Update32to33_SetShinyColorsToOne
	goto Debug_Script_3_Update32to33_TypeEffectiveness
	end

Debug_Script_3_Text_Shiny_Colors::
	.string "This script will let you enable the new\n"
	.string "SHINY COLORS option, which adds new\l"
	.string "shiny forms to certain {PKMN} species.\p"
	.string "Previously, it was always ON, now it's\n"
	.string "optional. Check online docs for more\l"
	.string "info and images on new shiny colors.\p"
	.string "Would you like to enable SHINY COLORS?$"

Debug_Script_3_Update32to33_TypeEffectiveness::
	msgbox Debug_Script_3_Text_TypeEffectiveness, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, Debug_Script_3_SetGenModernChart
	case MULTI_B_PRESSED, Debug_Script_3_SetGenModernChart
	case YES, Debug_Script_3_SetGen6Chart
	end

Debug_Script_3_SetGenModernChart::
	callnative Update32to33_TypeEffectiveness_Modern
	closemessage
	end

Debug_Script_3_SetGen6Chart::
	callnative Update32to33_TypeEffectiveness_GenVI
	closemessage
	end

Debug_Script_3_Text_TypeEffectiveness::
	.string "TYPE EFFECTIVENESS CHART option\n"
	.string "needs to be reset.\p"
	.string "GEN VI TYPE CHART nerfs steel not\n"
	.string "resisting GHOST and DARK.\p"
	.string "MODERN TYPE CHART buffs bad types into\n"
	.string "not so bad, like BUG. Check online docs\l"
	.string "for more info on this new option.\p"
	.string "Would you like to use GEN VI TYPE CHART?\p"
	.string "Selecting YES will enable GEN VI CHART.\n"
	.string "NO will set it to MODERN TYPE CHART.$"

Debug_Script_4::
	end

Debug_Script_5::
	end

Debug_Script_6::
	end

Debug_Script_7::
	callnative CheckNuzlockeMode
	buffernumberstring STR_VAR_1, VAR_UNUSED_0x40DB
	msgbox Text_Easy_Mode, MSGBOX_DEFAULT
	closemessage
	buffernumberstring STR_VAR_1, VAR_UNUSED_0x40FE
	msgbox Text_Normal_Mode, MSGBOX_DEFAULT
	closemessage
	buffernumberstring STR_VAR_1, VAR_UNUSED_0x40FF
	msgbox Text_Hard_Mode, MSGBOX_DEFAULT
	closemessage
	end

Text_Easy_Mode::
	.string "Easy mode is {STR_VAR_1}$"

Text_Normal_Mode::
	.string "Normal mode is {STR_VAR_1}$"

Text_Hard_Mode::
	.string "Hard mode is {STR_VAR_1}$"

Debug_Script_8::
	callnative Berry_Ready
	end

Debug_CheckSaveBlock::
	callnative CheckSaveBlock1Size
	msgbox Debug_SaveBlock1Size, MSGBOX_DEFAULT
	callnative CheckSaveBlock2Size
	msgbox Debug_SaveBlock2Size, MSGBOX_DEFAULT
	callnative CheckPokemonStorageSize
	msgbox Debug_PokemonStorageSize, MSGBOX_DEFAULT
	release
	end

Debug_SaveBlock1Size::
	.string "SaveBlock1 size: {STR_VAR_1}b/{STR_VAR_2}b.\n"
	.string "Free space: {STR_VAR_3}b.$"

Debug_SaveBlock2Size::
	.string "SaveBlock2 size: {STR_VAR_1}b/{STR_VAR_2}b.\n"
	.string "Free space: {STR_VAR_3}b.$"

Debug_PokemonStorageSize::
	.string "{PKMN}Storage size: {STR_VAR_1}b/{STR_VAR_2}b.\n"
	.string "Free space: {STR_VAR_3}b.$"

Debug_HatchAnEgg::
	lockall
	getpartysize
	goto_if_eq VAR_RESULT, 0, Debug_HatchAnEgg_NoPokemon
	special ChoosePartyMon
	waitstate
	goto_if_ge VAR_0x8004, PARTY_SIZE, Debug_HatchAnEgg_End
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_ne VAR_RESULT, SPECIES_EGG, DebugScript_HatchAnEgg_CantForceHatch
	special EggHatch
	waitstate
Debug_HatchAnEgg_End::
	releaseall
	end

Debug_HatchAnEgg_NoPokemon::
	msgbox DebugScript_HatchAnEgg_Text_EmptyParty, MSGBOX_DEFAULT
	releaseall
	end

DebugScript_HatchAnEgg_CantForceHatch::
	msgbox DebugScript_HatchAnEgg_Text_NotAnEgg, MSGBOX_DEFAULT
	releaseall
	end

DebugScript_HatchAnEgg_Text_EmptyParty::
	.string "You have no Pokémon nor Eggs.$"

DebugScript_HatchAnEgg_Text_NotAnEgg::
	.string "That's not a Pokémon Egg.$"
