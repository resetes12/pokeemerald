.set LOCALID_GRUNT, 3
.set LOCALID_DEVON_EMPLOYEE, 4
.set LOCALID_CELEBI, 17
.set LOCALID_OLD_MAN, 15
.set LOCALID_DUSCLOPS, 16

PetalburgWoods_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PetalburgWoods_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	.byte 0

PetalburgWoods_OnTransition::
	callnative ChangeEncounterTable
	goto_if_unset FLAG_EXTRA_LEGENDARIES, PetalburgWoods_SwitchToOld
	call_if_unset FLAG_SYS_GAME_CLEAR, PetalburgWoods_NoCelebiEvent
	call_if_set FLAG_SYS_GAME_CLEAR, PetalburgWoods_YesCelebiEvent
	end

PetalburgWoods_SwitchToOld::
	setmaplayoutindex LAYOUT_PETALBURG_WOODS_OLD
	setflag FLAG_OLD_MAN_AND_DUSCLOPS
	removeobject LOCALID_OLD_MAN
	removeobject LOCALID_DUSCLOPS
	return
	end

PetalburgWoods_NoCelebiEvent::
	checkflag FLAG_CELEBI_EVENT
	call_if_unset FLAG_CELEBI_EVENT, PetalburgWoods_CelebiEvent_End
	end

PetalburgWoods_YesCelebiEvent::
	checkflag FLAG_DEFEATEDCELEBI
	call_if_set FLAG_DEFEATEDCELEBI, PetalburgWoods_CelebiEvent_End
	call_if_unset FLAG_DEFEATEDCELEBI, PetalburgWoods_YesCelebiEvent2
	end

PetalburgWoods_YesCelebiEvent2::
	setflag FLAG_CELEBI_EVENT
	end

PetalburgWoods_CelebiEvent::
	lock
	playse SE_SUDOWOODO_SHAKE
	waitse
	msgbox Text_PetalburgWoods_Celebi0, MSGBOX_SIGN
	closemessage
	checkflag FLAG_CELEBI_EVENT
	call_if_unset FLAG_CELEBI_EVENT, PetalburgWoods_CelebiEvent_End
	call_if_set FLAG_CELEBI_EVENT, PetalburgWoods_CelebiEventGo
	end

PetalburgWoods_CelebiEventGo::
	applymovement OBJ_EVENT_ID_PLAYER, Movement_PetalburgWoods_Celebi1
	waitmovement 0
	msgbox Text_PetalburgWoods_Celebi1, MSGBOX_SIGN
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, PetalburgWoods_CelebiEvent_End
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_RAIKOU, PetalburgWoods_CelebiEvent_Continue1
	msgbox Text_PetalburgWoods_Celebi_Donthave, MSGBOX_SIGN
	end

PetalburgWoods_CelebiEvent_Continue1::
	msgbox Text_PetalburgWoods_Celebi2, MSGBOX_SIGN
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, PetalburgWoods_CelebiEvent_End
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_ENTEI, PetalburgWoods_CelebiEvent_Continue2
	msgbox Text_PetalburgWoods_Celebi_Donthave, MSGBOX_SIGN
	end

PetalburgWoods_CelebiEvent_Continue2::
	msgbox Text_PetalburgWoods_Celebi3, MSGBOX_SIGN
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, PetalburgWoods_CelebiEvent_End
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_SUICUNE, PetalburgWoods_CelebiBattle
	msgbox Text_PetalburgWoods_Celebi_Donthave, MSGBOX_SIGN
	end

PetalburgWoods_CelebiEvent_End::
	releaseall
	end

PetalburgWoods_CelebiBattle::
	faceplayer
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER, Movement_PetalburgWoods_Celebi1
	waitmovement 0
	msgbox Text_PetalburgWoods_Celebi4, MSGBOX_DEFAULT
	setwildbattle SPECIES_CELEBI, 50
	waitse
	playmoncry SPECIES_CELEBI, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	applymovement LOCALID_CELEBI, PetalburgWoods_EventScript_CelebiMoves
	waitmovement 0
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PetalburgWoods_CelebiEvent_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PetalburgWoods_CelebiEvent_Won
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PetalburgWoods_CelebiEvent_Ran
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PetalburgWoods_CelebiEvent_Ran
	releaseall
	end

PetalburgWoods_CelebiEvent_Won::
	applymovement LOCALID_CELEBI, PetalburgWoods_EventScript_CelebiMoves2
	waitmovement 0
	setflag FLAG_DEFEATEDCELEBI
	clearflag FLAG_CELEBI_EVENT
	end

PetalburgWoods_CelebiEvent_Ran::
	applymovement LOCALID_CELEBI, PetalburgWoods_EventScript_CelebiMoves2
	waitmovement 0
	end

PetalburgWoods_EventScript_CelebiMoves:
	walk_down
	set_visible
	jump_in_place_down
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	step_end

PetalburgWoods_EventScript_CelebiMoves2:
	set_invisible
	walk_up
	step_end

Text_PetalburgWoods_Celebi0:
	.string "The tree shakes softly.\p"
	.string "You sense a presence. Someone or\n"
	.string "something is watching you…\p"
	.string "Yet, no POKéMON is in sight.$"

Text_PetalburgWoods_Celebi1:
	.string ". . . \p"
	.string ". . . \p"
	.string "You can hear a dim voice.\p"
	.string "S..w .. ..e . . .ER . K. ON\p"
	.string ". . . \p"
	.string "S..w me the THUND.R PO..MON…$"

Text_PetalburgWoods_Celebi2:
	.string ". . . \p"
	.string ". . . \p"
	.string "Sh.. me .he .OLCANO . ..MON$"

Text_PetalburgWoods_Celebi3:
	.string ". . . \p"
	.string ". . . \p"
	.string "Sho.. me t.. AU.ORA …éMON.$"

Text_PetalburgWoods_Celebi4:
	.string ". . . \p"
	.string "Something is approaching…\p"
	.string ". . . $"

Text_PetalburgWoods_Celebi_Donthave:
	.string ". . . \p"
	.string "..u d..'t h… i….\p"
	.string "You don't have it…$"

Movement_PetalburgWoods_Celebi1:
	emote_exclamation_mark
	delay_16
	delay_16
	delay_16
	delay_16
	step_end

PetalburgWoods_Dusclops::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_DUSCLOPS, CRY_MODE_NORMAL
	msgbox PetalburgWoods_Text_Dusclops, MSGBOX_DEFAULT
	waitmoncry
	release
	end

PetalburgWoods_Text_Dusclops:
	.string "DUSKY: Dusk, dusk!$"

PetalburgWoods_Celebi_Sign::
	faceplayer
	goto_if_unset FLAG_PETALBURG_SHRINE_FIRST_TIME, PetalburgWoods_Celebi_SignFirstTime
	goto_if_set FLAG_DEFEATEDCELEBI, PetalburgWoods_Celebi_SignEventFinished
	goto_if_set FLAG_SYS_GAME_CLEAR, PetalburgWoods_Celebi_SignChampion
	msgbox Text_PetalburgWoods_Celebi_Sign, MSGBOX_NPC
	end

PetalburgWoods_Celebi_SignFirstTime::
	faceplayer
	msgbox Text_PetalburgWoods_Celebi_SignFirstTime, MSGBOX_NPC
	setflag FLAG_PETALBURG_SHRINE_FIRST_TIME
	end

PetalburgWoods_Celebi_SignEventFinished::
	faceplayer
	msgbox Text_PetalburgWoods_Celebi_SignEventFinished, MSGBOX_NPC
	end

PetalburgWoods_Celebi_SignChampion::
	faceplayer
	msgbox Text_PetalburgWoods_Celebi_SignEventChampion, MSGBOX_NPC
	end

Text_PetalburgWoods_Celebi_Sign:
	.string "Hi, {PLAYER}.\p"
	.string "Dangerous? No, no. Don’t worry.\l"
	.string "I always have DUSKY with me.$"

Text_PetalburgWoods_Celebi_SignFirstTime:
	.string "Oh, a trainer…\p"
	.string "What are you doing here in these\n"
	.string "dark, shady woods?\p"
	.string ". . .\p"
	.string "Oh, I see - you’re just passing by.\p"
	.string "What? Ah, me?\p"
	.string "I’m simply tending to the shrine,\n"
	.string "as I always have.\p"
	.string "You’re wondering why I take care of\n"
	.string "an old shrine in the middle of\l"
	.string "PETALBURG WOODS, aren’t you?\p"
	.string "Well, this shrine is special.\n"
	.string "In these woods lives a MYTHICAL\l"
	.string "POKéMON - the FOREST GUARDIAN.\p"
	.string "I’ve been watching over it for\n"
	.string "more than twenty years.\p"
	.string "And I can assure you, this story\n"
	.string "is as real as the air we breathe.\p"
	.string "The FOREST GUARDIAN is small, GREEN,\n"
	.string "and gazing at it feels…\p"
	.string "Like seeing the spirit of the forest.\p"
	.string "Sadly, I last saw it many, many\n"
	.string "years ago…\p"
	.string "Where could the GUARDIAN be now?\p"
	.string "Anyway, I’ll continue my duties.\n"
	.string "See you soon!$"

Text_PetalburgWoods_Celebi_SignEventFinished:
	.string ". . .\p"
	.string "I can hardly believe my eyes!\n"
	.string "You actually battled the GUARDIAN!?\p"
	.string "I’ll never forget this moment.\p"
	.string "I’ll keep tending to the shrine\l"
	.string "until the GUARDIAN returns!$"

Text_PetalburgWoods_Celebi_SignEventChampion:
	.string "Hello again, young trainer.\p"
	.string "Something strange is happening\n"
	.string "with the GUARDIAN shrine.\p"
	.string "I’ve been hearing faint muttering\n"
	.string "around the large tree.\p"
	.string "It’s hard to make out, but it speaks\n"
	.string "of a THUNDER, VOLCANO,\l"
	.string "and AURORA POKéMON…\p"
	.string "Wait… I know those descriptions…\n"
	.string "Three beasts… but…\p"
	.string "Not from this region…?\n"
	.string "Where could they be from?\p"
	.string "And if they’re here, could they be\n"
	.string "hiding in places tied to their\l"
	.string "elements?\p"
	.string "If you have time, perhaps you should\n"
	.string "investigate. What do you think?$"

PetalburgWoods_EventScript_DevonResearcherLeft::
	lockall
	call PetalburgWoods_EventScript_DevonResearcherIntro
	applymovement LOCALID_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherApproachPlayerLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_HaveYouSeenShroomish, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	applymovement LOCALID_GRUNT, PetalburgWoods_Movement_AquaEntrance
	waitmovement 0
	msgbox PetalburgWoods_Text_IWasGoingToAmbushYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT, PetalburgWoods_Movement_AquaApproachResearcherLeft
	waitmovement 0
	applymovement LOCALID_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_HandOverThosePapers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherFleeToPlayerLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_YouHaveToHelpMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT, PetalburgWoods_Movement_AquaApproachPlayer
	waitmovement 0
	msgbox PetalburgWoods_Text_NoOneCrossesTeamAqua, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_PETALBURG_WOODS, PetalburgWoods_Text_YoureKiddingMe
	applymovement LOCALID_GRUNT, PetalburgWoods_Movement_AquaBackOff
	waitmovement 0
	call PetalburgWoods_EventScript_DevonResearcherPostBattle
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgWoods_Movement_WatchResearcherLeave
	applymovement LOCALID_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherExitLeft
	waitmovement 0
	goto PetalburgWoods_EventScript_RemoveDevonResearcher
	end

PetalburgWoods_EventScript_DevonResearcherRight::
	lockall
	call PetalburgWoods_EventScript_DevonResearcherIntro
	applymovement LOCALID_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherApproachPlayerRight
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_HaveYouSeenShroomish, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	applymovement LOCALID_GRUNT, PetalburgWoods_Movement_AquaEntrance
	waitmovement 0
	msgbox PetalburgWoods_Text_IWasGoingToAmbushYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT, PetalburgWoods_Movement_AquaApproachResearcherRight
	waitmovement 0
	applymovement LOCALID_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_HandOverThosePapers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherFleeToPlayerRight
	waitmovement 0
	msgbox PetalburgWoods_Text_YouHaveToHelpMe, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_NoOneCrossesTeamAqua, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_PETALBURG_WOODS, PetalburgWoods_Text_YoureKiddingMe
	applymovement LOCALID_GRUNT, PetalburgWoods_Movement_AquaBackOff
	waitmovement 0
	call PetalburgWoods_EventScript_DevonResearcherPostBattle
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgWoods_Movement_WatchResearcherLeave
	applymovement LOCALID_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherExitRight
	waitmovement 0
	goto PetalburgWoods_EventScript_RemoveDevonResearcher
	end

PetalburgWoods_EventScript_DevonResearcherIntro::
	applymovement LOCALID_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherLookAround
	waitmovement 0
	msgbox PetalburgWoods_Text_NotAOneToBeFound, MSGBOX_DEFAULT
	closemessage
	return

PetalburgWoods_EventScript_DevonResearcherPostBattle::
	msgbox PetalburgWoods_Text_YouveGotSomeNerve, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT, PetalburgWoods_Movement_AquaRunAway
	waitmovement 0
	removeobject LOCALID_GRUNT
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox PetalburgWoods_Text_ThatWasAwfullyClose, MSGBOX_DEFAULT
	giveitem ITEM_GREAT_BALL
	goto_if_eq VAR_RESULT, FALSE, PetalburgWoods_EventScript_BagFull
	goto PetalburgWoods_EventScript_DevonResearcherFinish
	end

PetalburgWoods_EventScript_BagFull::
	msgbox PetalburgWoods_Text_YoureLoadedWithItems, MSGBOX_DEFAULT
	goto PetalburgWoods_EventScript_DevonResearcherFinish
	end

PetalburgWoods_EventScript_DevonResearcherFinish::
	msgbox PetalburgWoods_Text_TeamAquaAfterSomethingInRustboro, MSGBOX_DEFAULT
	applymovement LOCALID_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherStartExit
	waitmovement 0
	msgbox PetalburgWoods_Text_ICantBeWastingTime, MSGBOX_DEFAULT
	closemessage
	return

PetalburgWoods_EventScript_RemoveDevonResearcher::
	removeobject LOCALID_DEVON_EMPLOYEE
	setvar VAR_PETALBURG_WOODS_STATE, 1
	releaseall
	end

PetalburgWoods_Movement_DevonResearcherLookAround:
	face_up
	delay_16
	delay_4
	face_right
	delay_16
	delay_8
	face_left
	delay_16
	delay_8
	face_down
	delay_16
	face_right
	delay_16
	delay_8
	face_up
	delay_16
	step_end

PetalburgWoods_Movement_DevonResearcherExitLeft:
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

PetalburgWoods_Movement_DevonResearcherApproachPlayerLeft:
	delay_16
	face_player
	walk_down
	walk_down
	step_end

PetalburgWoods_Movement_DevonResearcherApproachPlayerRight:
	delay_16
	face_player
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

PetalburgWoods_Movement_DevonResearcherExitRight:
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

PetalburgWoods_Movement_WatchResearcherLeave:
	delay_16
	delay_16
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherFleeToPlayerLeft:
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherFleeToPlayerRight:
	walk_fast_down
	walk_fast_right
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherStartExit:
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	face_up
	step_end

PetalburgWoods_Movement_AquaApproachResearcherLeft:
	walk_fast_down
	walk_fast_down
	step_end

PetalburgWoods_Movement_AquaBackOff:
	lock_facing_direction
	walk_up
	unlock_facing_direction
	step_end

PetalburgWoods_Movement_AquaRunAway:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	delay_16
	delay_16
	step_end

PetalburgWoods_Movement_AquaApproachResearcherRight:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

PetalburgWoods_Movement_AquaEntrance:
	walk_down
	walk_down
	delay_16
	delay_16
	step_end

PetalburgWoods_Movement_AquaApproachPlayer:
	walk_down
	step_end

PetalburgWoods_EventScript_Boy1::
	msgbox PetalburgWoods_Text_StayOutOfTallGrass, MSGBOX_NPC
	end

PetalburgWoods_EventScript_Boy2::
	msgbox PetalburgWoods_Text_HiddenItemsExplanation, MSGBOX_NPC
	end

PetalburgWoods_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_MIRACLE_SEED, PetalburgWoods_EventScript_ExplainMiracleSeed
	msgbox PetalburgWoods_Text_TryUsingThisItem, MSGBOX_DEFAULT
	giveitem ITEM_MIRACLE_SEED
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MIRACLE_SEED
	release
	end

PetalburgWoods_EventScript_ExplainMiracleSeed::
	msgbox PetalburgWoods_Text_MiracleSeedExplanation, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_Sign1::
	msgbox PetalburgWoods_Text_TrainerTipsExperience, MSGBOX_SIGN
	end

PetalburgWoods_EventScript_Sign2::
	msgbox PetalburgWoods_Text_TrainerTipsPP, MSGBOX_SIGN
	end

PetalburgWoods_EventScript_Lyle::
	trainerbattle_single TRAINER_LYLE, PetalburgWoods_Text_GoBugPokemonTeam, PetalburgWoods_Text_ICouldntWin
	msgbox PetalburgWoods_Text_ImOutOfPokeBalls, MSGBOX_AUTOCLOSE
	end

PetalburgWoods_EventScript_James::
	trainerbattle_single TRAINER_JAMES_1, PetalburgWoods_Text_InstantlyPopularWithBugPokemon, PetalburgWoods_Text_CantBePopularIfILose, PetalburgWoods_EventScript_TryRegisterJames
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, PetalburgWoods_EventScript_JamesRematch
	setvar VAR_0x8004, TRAINER_JAMES_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, PetalburgWoods_EventScript_TryRegisterJames2
	msgbox PetalburgWoods_Text_PeopleRespectYou, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_TryRegisterJames::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, PetalburgWoods_EventScript_RegisterJames
	release
	end

PetalburgWoods_EventScript_RegisterJames::
	msgbox PetalburgWoods_Text_IWantRematch1, MSGBOX_DEFAULT
	register_matchcall TRAINER_JAMES_1
	release
	end

PetalburgWoods_EventScript_TryRegisterJames2::
	goto_if_set FLAG_HAS_MATCH_CALL, PetalburgWoods_EventScript_RegisterJames2
	msgbox PetalburgWoods_Text_PeopleRespectYou, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_RegisterJames2::
	msgbox PetalburgWoods_Text_IWantRematch2, MSGBOX_DEFAULT
	register_matchcall TRAINER_JAMES_1
	release
	end

PetalburgWoods_EventScript_JamesRematch::
	trainerbattle_rematch TRAINER_JAMES_1, PetalburgWoods_Text_MyPokemonHaveGrown, PetalburgWoods_Text_CantBePopularIfLose
	msgbox PetalburgWoods_Text_IveBeenTrainingHard, MSGBOX_AUTOCLOSE
	end

PetalburgWoods_Text_NotAOneToBeFound:
	.string "Hmmm…\n"
	.string "Not a one to be found…$"

PetalburgWoods_Text_HaveYouSeenShroomish:
	.string "Hello, have you seen any POKéMON\n"
	.string "called SHROOMISH around here?\p"
	.string "I really love that POKéMON.$"

PetalburgWoods_Text_IWasGoingToAmbushYou:
	.string "I was going to ambush you, but you\n"
	.string "had to dawdle in PETALBURG WOODS\l"
	.string "forever, didn't you?\p"
	.string "I got sick of waiting, so here I am!$"

PetalburgWoods_Text_HandOverThosePapers:
	.string "You! DEVON RESEARCHER!\p"
	.string "Hand over those papers!$"

PetalburgWoods_Text_YouHaveToHelpMe:
	.string "Aiyeeeh!\p"
	.string "You're a POKéMON TRAINER, aren't you?\n"
	.string "You've got to help me, please!$"

PetalburgWoods_Text_NoOneCrossesTeamAqua:
	.string "Hunh? What do you think you're doing?\n"
	.string "What, you're going to protect him?\p"
	.string "No one who crosses TEAM AQUA\n"
	.string "gets any mercy, not even a kid!\p"
	.string "Come on and battle me!$"

PetalburgWoods_Text_YoureKiddingMe:
	.string "You're kidding me! You're tough!$"

PetalburgWoods_Text_YouveGotSomeNerve:
	.string "Grrr… You've got some nerve\n"
	.string "meddling with TEAM AQUA!\l"
	.string "Come on and battle me again!\p"
	.string "I wish I could say that, but I'm out of\n"
	.string "POKéMON…\p"
	.string "And, hey, we of TEAM AQUA are also\n"
	.string "after something in RUSTBORO.\p"
	.string "I'll let you go today!$"

PetalburgWoods_Text_ThatWasAwfullyClose:
	.string "Whew…\n"
	.string "That was awfully close!\p"
	.string "Thanks to you, he didn't rob me of\n"
	.string "these important papers.\p"
	.string "I know, I'll give you a GREAT BALL as\n"
	.string "my thanks!$"

PetalburgWoods_Text_TeamAquaAfterSomethingInRustboro:
	.string "Didn't that TEAM AQUA thug say\n"
	.string "they were after something in\l"
	.string "RUSTBORO, too?$"

PetalburgWoods_Text_ICantBeWastingTime:
	.string "Uh-oh! It's a crisis!\n"
	.string "I can't be wasting time!$"

PetalburgWoods_Text_YoureLoadedWithItems:
	.string "You're loaded with items.\n"
	.string "I can't give you this GREAT BALL.$"

PetalburgWoods_Text_GoBugPokemonTeam:
	.string "I caught a whole bunch of POKéMON!\p"
	.string "Go, go, go!\n"
	.string "My BUG POKéMON team!$"

PetalburgWoods_Text_ICouldntWin:
	.string "I have all these POKéMON,\n"
	.string "but I couldn't win…$"

PetalburgWoods_Text_ImOutOfPokeBalls:
	.string "I caught a bunch of POKéMON.\n"
	.string "Now I'm out of POKé BALLS.$"

PetalburgWoods_Text_InstantlyPopularWithBugPokemon:
	.string "If you take BUG POKéMON to school,\n"
	.string "you get to be instantly popular!$"

PetalburgWoods_Text_CantBePopularIfILose:
	.string "I can't be popular if I lose.$"

PetalburgWoods_Text_PeopleRespectYou:
	.string "If you have a big BUG POKéMON,\n"
	.string "people respect you for it.$"

PetalburgWoods_Text_IWantRematch1:
	.string "I want a rematch when my BUG\n"
	.string "POKéMON grow up!\p"
	.string "I'm registering you in my POKéNAV!$"

PetalburgWoods_Text_IWantRematch2:
	.string "I want a rematch when my BUG \n"
	.string "POKéMON grow up!\p"
	.string "I'm registering you in my POKéNAV!$"

PetalburgWoods_Text_MyPokemonHaveGrown:
	.string "My BUG POKéMON have grown.\n"
	.string "I'll be popular again.$"

PetalburgWoods_Text_CantBePopularIfLose:
	.string "I can't be popular if I lose.$"

PetalburgWoods_Text_IveBeenTrainingHard:
	.string "You get to be popular if you have\n"
	.string "strong POKéMON, right?\l"
	.string "So, I've been training hard.$"

PetalburgWoods_Text_StayOutOfTallGrass:
	.string "Yo, there!\n"
	.string "Your POKéMON doing okay?\p"
	.string "If your POKéMON are weak and you want\n"
	.string "to avoid battles, you should stay out\l"
	.string "of tall grass.$"

PetalburgWoods_Text_HiddenItemsExplanation:
	.string "Sometimes, there are things on the\n"
	.string "ground even if you can't see them.\p"
	.string "That's why I always check where I'm\n"
	.string "walking.$"

PetalburgWoods_Text_TryUsingThisItem:
	.string "Oh, neat!\n"
	.string "That's the BADGE from RUSTBORO GYM!\p"
	.string "You must be a TRAINER.\n"
	.string "You should try using this item.$"

PetalburgWoods_Text_MiracleSeedExplanation:
	.string "It's a MIRACLE SEED.\n"
	.string "If a POKéMON holds that item, its\l"
	.string "GRASS-type moves become stronger.\p"
	.string "Besides MIRACLE SEED, I think there are\n"
	.string "other convenient items for POKéMON\l"
	.string "to hold.$"

PetalburgWoods_Text_TrainerTipsExperience:
	.string "TRAINER TIPS\p"
	.string "Any POKéMON that appears even once\n"
	.string "in a battle is awarded EXP Points.\p"
	.string "To raise a weak POKéMON, put it at the\n"
	.string "left of the team list.\p"
	.string "As soon as a battle starts, switch it\n"
	.string "out. It will earn EXP Points without\l"
	.string "being exposed to any harm.$"

PetalburgWoods_Text_TrainerTipsPP:
	.string "TRAINER TIPS\p"
	.string "In addition to Hit Points (HP), POKéMON\n"
	.string "have Power Points (PP) that are used to\l"
	.string "make moves during battle.\p"
	.string "If a POKéMON runs out of PP, it must be\n"
	.string "taken to a POKéMON CENTER.$"

