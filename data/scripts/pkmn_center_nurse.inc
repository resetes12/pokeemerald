Common_EventScript_PkmnCenterNurse::
	lock
	faceplayer
	goto_if_unset FLAG_OLDSAVE, Common_EventScript_PkmnCenterNurseOldSave
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, CountPlayerTrainerStars
	goto_if_eq VAR_RESULT, 4, EventScript_PkmnCenterNurse_GoldCard
	goto EventScript_PkmnCenterNurse_HealPkmn
	end

EventScript_PkmnCenterNurse_Goodbye::
	message gText_WeHopeToSeeYouAgain
	return

Common_EventScript_PkmnCenterNurseOldSave::
	setflag FLAG_OLDSAVE
	msgbox Common_EventScript_PkmnCenterNurseOldSaveText1, MSGBOX_DEFAULT
	closemessage
	delay 20
	clearflag FLAG_ITEM_ROUTE_115_TM01
	clearflag FLAG_ITEM_METEOR_FALLS_B1F_2R_TM02
	clearflag FLAG_ITEM_FIERY_PATH_TM06
	clearflag FLAG_ITEM_SHOAL_CAVE_ICE_ROOM_TM07
	clearflag FLAG_ITEM_SCORCHED_SLAB_TM11
	clearflag FLAG_ITEM_ABANDONED_SHIP_ROOMS_B1F_TM13
	clearflag FLAG_ITEM_ABANDONED_SHIP_HIDDEN_FLOOR_ROOM_1_TM18
	clearflag FLAG_ITEM_SAFARI_ZONE_NORTH_WEST_TM22
	clearflag FLAG_ITEM_METEOR_FALLS_1F_1R_TM23
	clearflag FLAG_ITEM_SEAFLOOR_CAVERN_ROOM_9_TM26
	clearflag FLAG_ITEM_VICTORY_ROAD_B1F_TM29
	clearflag FLAG_ITEM_MT_PYRE_6F_TM30
	clearflag FLAG_ITEM_ROUTE_111_TM37
	clearflag FLAG_ITEM_MT_PYRE_EXTERIOR_TM48
	giveitem ITEM_TM03
	giveitem ITEM_TM04
	giveitem ITEM_TM05
	giveitem ITEM_TM08
	giveitem ITEM_TM09
	giveitem ITEM_TM12
	giveitem ITEM_TM19
	giveitem ITEM_TM21
	giveitem ITEM_TM27
	giveitem ITEM_TM28
	giveitem ITEM_TM31
	giveitem ITEM_TM34
	giveitem ITEM_TM36
	giveitem ITEM_TM39
	giveitem ITEM_TM40
	giveitem ITEM_TM41
	giveitem ITEM_TM42
	giveitem ITEM_TM44
	giveitem ITEM_TM45
	giveitem ITEM_TM46
	giveitem ITEM_TM47
	giveitem ITEM_TM49
	giveitem ITEM_TM50
	msgbox Common_EventScript_PkmnCenterNurseOldSaveText2, MSGBOX_DEFAULT
	closemessage
	delay 20
	removeitem ITEM_ACRO_BIKE
	giveitem ITEM_MACH_BIKE
	removeitem ITEM_EXP_SHARE, 99
	giveitem ITEM_EXP_SHARE
	msgbox Common_EventScript_PkmnCenterNurseOldSaveText3, MSGBOX_DEFAULT
	closemessage
	delay 20
	goto Common_EventScript_PkmnCenterNurse
	end

Common_EventScript_PkmnCenterNurseOldSaveText1:
	.string "Oh! Looks like you are loading an\n"
	.string "OLD SAVEGAME. Let's fix the SAVEGAME\p"
	.string "so you can fully enjoy this HACKROM.\n"
	.string "Let's start with the basics...\p"
	.string "In the PROF. BIRCH LAB, there's 3 PCs.\p"
	.string "If you check the TOP one, you will be\n"
	.string "to change the GAMEMODE. There's 2\p"
	.string "GAMEMODES: CLASSIC, which doesn't\n"
	.string "implement the PHYSICAL/SPECIAL split\p"
	.string "created in GEN IV; and MODERN, which\n"
	.string "it does.\p"
	.string "Then, the PC on the LEFT contains a\n"
	.string "DIFFICULTY changer. NORMAL is the\p"
	.string "default experience, while EASY and\n"
	.string "HARD makes the levels scale up to\p"
	.string "6 levels (10 after being the CHAMPION).\n"
	.string "Now, let's try to fix other things.\p"
	.string "TMs are now infinite, which means\n"
	.string "if you have used them, they are gone.\p"
	.string "Let me respawn all TMs that can be\n"
	.string "found in the overworld for you...$"

Common_EventScript_PkmnCenterNurseOldSaveText2:
	.string "There. You can go look for them!\p"
	.string "Now... I also gave you copies of\n"
	.string "TMs that were obtained from NPCs.\p"
	.string "I'll give you a BIKE. This\n"
	.string "BIKE is the MACH and ACRO version\p"
	.string "in one. If you have the ACRO BIKE\n"
	.string "it'll be removed. I'll do the same\p"
	.string "with the EXP. SHARE, which is now a\n"
	.string "KEY ITEM.$"

Common_EventScript_PkmnCenterNurseOldSaveText3:
	.string "Old EXP. SHARES are not deleted\n"
	.string "so it's recommended that you load\p"
	.string "the original SAVE into a clean\n"
	.string "version of POKéMON EMERALD, delete\p"
	.string "them, and then import the SAVE again\n"
	.string "to this HACKROM. If not, old EXP\p"
	.string "SHARES will not be deleteable from\n"
	.string "the BAG.$"

@ VAR_0x8004 is 1 when player has Gold Card; jumps are identical
EventScript_PkmnCenterNurse_HealPkmn::
	incrementgamestat GAME_STAT_USED_POKECENTER
	call_if_eq VAR_0x8004, 0, EventScript_PkmnCenterNurse_IllTakeYourPkmn
	call_if_eq VAR_0x8004, 1, EventScript_PkmnCenterNurse_IllTakeYourPkmn2
	waitmessage
	call EventScript_PkmnCenterNurse_TakeAndHealPkmn
	goto_if_unset FLAG_POKERUS_EXPLAINED, EventScript_PkmnCenterNurse_CheckPokerus
	goto EventScript_PkmnCenterNurse_ReturnPkmn
	end

EventScript_PkmnCenterNurse_IllTakeYourPkmn::
	message gText_IllTakeYourPkmn
	return

EventScript_PkmnCenterNurse_IllTakeYourPkmn2::
	message gText_IllTakeYourPkmn2
	return

EventScript_PkmnCenterNurse_TakeAndHealPkmn::
	applymovement VAR_0x800B, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	dofieldeffect FLDEFF_POKECENTER_HEAL
	waitfieldeffect FLDEFF_POKECENTER_HEAL
	applymovement VAR_0x800B, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	special HealPlayerParty
	return

EventScript_PkmnCenterNurse_CheckTrainerHillAndUnionRoom::
	specialvar VAR_RESULT, PlayerNotAtTrainerHillEntrance
	goto_if_eq VAR_RESULT, 0, EventScript_PkmnCenterNurse_ReturnPkmn
	specialvar VAR_RESULT, BufferUnionRoomPlayerName
	copyvar VAR_0x8008, VAR_RESULT
	goto_if_eq VAR_0x8008, 0, EventScript_PkmnCenterNurse_ReturnPkmn
	goto_if_eq VAR_0x8008, 1, EventScript_PkmnCenterNurse_PlayerWaitingInUnionRoom
	end

@ VAR_0x8004 is 1 when player has Gold Card
EventScript_PkmnCenterNurse_ReturnPkmn::
	goto_if_eq VAR_0x8004, 1, EventScript_PkmnCenterNurse_ReturnPkmn2
	message gText_RestoredPkmnToFullHealth
	waitmessage
	applymovement VAR_0x800B, Movement_PkmnCenterNurse_Bow
	waitmovement 0
	message gText_WeHopeToSeeYouAgain
	goto EventScript_PkmnCenterNurse_PlayerTurn
	return

EventScript_PkmnCenterNurse_ReturnPkmn2::
	message gText_ThankYouForWaiting
	waitmessage
	applymovement VAR_0x800B, Movement_PkmnCenterNurse_Bow
	waitmovement 0
	message gText_WeHopeToSeeYouAgain2
	goto EventScript_PkmnCenterNurse_PlayerTurn
	return

EventScript_PkmnCenterNurse_PlayerWaitingInUnionRoom::
	goto_if_set FLAG_NURSE_UNION_ROOM_REMINDER, EventScript_PkmnCenterNurse_ReturnPkmn
	msgbox gText_RestoredPkmnToFullHealth, MSGBOX_DEFAULT
	setflag FLAG_NURSE_UNION_ROOM_REMINDER
	message CableClub_Text_PlayerIsWaiting
	waitmessage
	applymovement VAR_0x800B, Movement_PkmnCenterNurse_Bow
	waitmovement 0
	message gText_WeHopeToSeeYouAgain
	return

EventScript_PkmnCenterNurse_CheckPokerus::
	specialvar VAR_RESULT, IsPokerusInParty
	goto_if_eq VAR_RESULT, TRUE, EventScript_PkmnCenterNurse_ExplainPokerus
	goto EventScript_PkmnCenterNurse_ReturnPkmn
	end

EventScript_PkmnCenterNurse_ExplainPokerus::
	message gText_PokerusExplanation
	setflag FLAG_POKERUS_EXPLAINED
	return

EventScript_PkmnCenterNurse_GoldCard::
	goto_if_set FLAG_NURSE_MENTIONS_GOLD_CARD, EventScript_PkmnCenterNurse_AskForUsual
	setflag FLAG_NURSE_MENTIONS_GOLD_CARD
	msgbox gText_WelcomeCutShort, MSGBOX_DEFAULT
	playse SE_PIN
	applymovement VAR_0x800B, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_0x800B, Common_Movement_Delay48
	waitmovement 0
	msgbox gText_NoticesGoldCard, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_PkmnCenterNurse_GoldCardHealPkmn
	message gText_WeHopeToSeeYouAgain2
	return

EventScript_PkmnCenterNurse_AskForUsual::
	msgbox gText_YouWantTheUsual, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_PkmnCenterNurse_GoldCardHealPkmn
	message gText_WeHopeToSeeYouAgain2
	return

EventScript_PkmnCenterNurse_GoldCardHealPkmn::
	setvar VAR_0x8004, 1
	goto EventScript_PkmnCenterNurse_HealPkmn
	end

Movement_PkmnCenterNurse_Bow:
	nurse_joy_bow
	delay_4
	step_end

EventScript_PkmnCenterNurse_PlayerTurn::
   closemessage
   applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
   waitmovement 0
   release
   end
