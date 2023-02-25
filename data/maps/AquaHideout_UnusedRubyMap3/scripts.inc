AquaHideout_UnusedRubyMap3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AquaHideout_UnusedRubyMap3_OnTransition
	.byte 0

AquaHideout_UnusedRubyMap3_OnTransition::
	clearflag 0x85C
	clearflag 0x85D
	clearflag 0x85E
	clearflag 0x85F
	end

BFNewZone_EventScript_Frontier1Trainer::
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_GYM_LEADER
	trainerbattle_single TRAINER_FRONTIER1, BFNewZone_EventScript_Frontier1TrainerIntro, BFNewZone_EventScript_Frontier1TrainerDefeat
	msgbox BFNewZone_EventScript_Frontier1TrainerPost, MSGBOX_AUTOCLOSE
	end

BFNewZone_EventScript_Frontier1TrainerIntro:
	.string "Sup' buddy.\p"
	.string "Welcome to our training island.\p"
	.string "I'm LUCAS, and I've won\n"
	.string "the UNOVA, JOHTO, KANTO\p"
	.string "and HOENN LEAGUE.\p"
	.string "Are you ready for my challenge?$"

BFNewZone_EventScript_Frontier1TrainerDefeat:
	.string "This... this was AMAZING!$"

BFNewZone_EventScript_Frontier1TrainerPost:
	.string "Thanks for battling me.\n"
	.string "If you want to battle us again,\p"
	.string "just go to the BATTLEFRONTIER\n"
	.string "and just come back.$"

BFNewZone_EventScript_Frontier2Trainer::
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_GYM_LEADER
	trainerbattle_single TRAINER_FRONTIER2, BFNewZone_EventScript_Frontier2TrainerIntro, BFNewZone_EventScript_Frontier2TrainerDefeat
	msgbox BFNewZone_EventScript_Frontier2TrainerPost, MSGBOX_AUTOCLOSE
	end

BFNewZone_EventScript_Frontier2TrainerIntro:
	.string "Last week, I completed the\n"
	.string "ISLAND CHALLENGE of ALOLA.\p"
	.string "Do you think you can beat me?$"

BFNewZone_EventScript_Frontier2TrainerDefeat:
	.string "Ah... Not bad, not bad...$"

BFNewZone_EventScript_Frontier2TrainerPost:
	.string ". . . \p"
	.string "I feel... Hopeful.\p"
	.string "Next time, you won't win.\p"
	.string "Want to battle us again?\p"
	.string "Just go to the BATTLEFRONTIER\n"
	.string "and then come back.$"

BFNewZone_EventScript_Frontier3Trainer::
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_GYM_LEADER
	trainerbattle_single TRAINER_FRONTIER3, BFNewZone_EventScript_Frontier3TrainerIntro, BFNewZone_EventScript_Frontier3TrainerDefeat
	msgbox BFNewZone_EventScript_Frontier3TrainerPost, MSGBOX_AUTOCLOSE
	end

BFNewZone_EventScript_Frontier3TrainerIntro:
	.string "When I was 35 years-old I became\n"
	.string "the CHAMPION of HOENN for 4 years.\p"
	.string "You think you can challenge me?$"

BFNewZone_EventScript_Frontier3TrainerDefeat:
	.string "Whew! That was great!$"

BFNewZone_EventScript_Frontier3TrainerPost:
	.string "You remind me of the challenger\n"
	.string "that became the CHAMPION after\p"
	.string "defeating me. Good times!\p"
	.string "If you want to battle me again,\n"
	.string "just go back to the BATTLEFRONTIER\p"
	.string "and talk to me again.$"

BFNewZone_EventScript_Frontier4Trainer::
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_GYM_LEADER
	trainerbattle_single TRAINER_FRONTIER4, BFNewZone_EventScript_Frontier4TrainerIntro, BFNewZone_EventScript_Frontier4TrainerDefeat
	msgbox BFNewZone_EventScript_Frontier4TrainerPost, MSGBOX_AUTOCLOSE
	end

BFNewZone_EventScript_Frontier4TrainerIntro:
	.string "I may look like a fisherman...\p"
	.string "And I am! But I also train more\n"
	.string "POKéMON that are not WATER TYPE.\p"
	.string "With this team, I won the KANTO\n"
	.string "LEAGUE many years ago. Let's battle!$"

BFNewZone_EventScript_Frontier4TrainerDefeat:
	.string "Oh... I'm going to fish."

BFNewZone_EventScript_Frontier4TrainerPost:
	.string ". . . \p"
	.string "Don't talk with me, I'm angry..\p"
	.string ". . . \p"
	.string ". . . \p"
	.string "Come back later if you want\n"
	.string "to battle me again.$"

BFNewZone_EventScript_Snorlax::
	lock
	faceplayer
	msgbox BFNewZone_EventScript_SnorlaxText, MSGBOX_DEFAULT
	closemessage
	setwildbattle SPECIES_SNORLAX, 60
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_WILD
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BFNewZone_EventScript_Snorlax_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BFNewZone_EventScript_Snorlax_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BFNewZone_EventScript_Snorlax_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BFNewZone_EventScript_Snorlax_Ran
	release
	end

BFNewZone_EventScript_SnorlaxText:
	.string "A big POKéMON is sleeping.\p"
	.string "Wait, he's not sleeping!\p"
	.string "SNOOOR!!$"

BFNewZone_EventScript_Snorlax_Won::
	setflag FLAG_SNORLAX
	goto Common_EventScript_RemoveStaticPokemon
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

BFNewZone_EventScript_Snorlax_Ran::
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

BFNewZone_EventScript_Sudo2::
	lock
	faceplayer
	msgbox BFNewZone_EventScript_SudoText, MSGBOX_DEFAULT
	closemessage
	setwildbattle SPECIES_SUDOWOODO, 40
	waitse
	playmoncry SPECIES_SUDOWOODO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_WILD
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BFNewZone_EventScript_Sudo2_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BFNewZone_EventScript_Sudo2_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BFNewZone_EventScript_Sudo2_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BFNewZone_EventScript_Sudo2_Ran
	release
	end

BFNewZone_EventScript_SudoText:
	.string "Wait, I've see a tree like\n"
	.string "this before...\p"
	.string "It's not a tree! It attacked!$"

BFNewZone_EventScript_Sudo2_Won::
	setflag FLAG_UNUSED_0x923
	goto Common_EventScript_RemoveStaticPokemon
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

BFNewZone_EventScript_Sudo2_Ran::
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

BFNewZone_EventScript_Sudo3::
	lock
	faceplayer
	setwildbattle SPECIES_SUDOWOODO, 40
	waitse
	playmoncry SPECIES_SUDOWOODO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_SYS_SET_BATTLE_BGM
	setvar VAR_TEMP_F, MUS_RG_VS_WILD
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BFNewZone_EventScript_Sudo3_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BFNewZone_EventScript_Sudo3_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BFNewZone_EventScript_Sudo3_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BFNewZone_EventScript_Sudo3_Ran
	release
	end

BFNewZone_EventScript_Sudo3_Won::
	setflag FLAG_UNUSED_0x924
	goto Common_EventScript_RemoveStaticPokemon
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end

BFNewZone_EventScript_Sudo3_Ran::
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	end
