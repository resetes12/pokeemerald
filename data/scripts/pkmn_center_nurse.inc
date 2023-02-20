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
	@playse SE_PIN
	setflag FLAG_OLDSAVE
	setflag FLAG_ENABLE_SHIP_SOUTHERN_ISLAND
	setflag FLAG_ENABLE_SHIP_NAVEL_ROCK
	setflag FLAG_ENABLE_SHIP_BIRTH_ISLAND
	setflag FLAG_ENABLE_SHIP_FARAWAY_ISLAND
	clearflag FLAG_DEFEAT_SUICUNE
	clearflag FLAG_DEFEAT_ENTEI
	clearflag FLAG_DEFEAT_RAIKOU
	msgbox Common_EventScript_PkmnCenterNurseOldSaveText1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox Common_EventScript_PkmnCenterNurseOldSaveText1_5, MSGBOX_DEFAULT
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
	clearflag FLAG_RECEIVED_TM05
	clearflag FLAG_RECEIVED_TM09
	clearflag FLAG_RECEIVED_TM19
	clearflag FLAG_RECEIVED_TM28
	clearflag FLAG_RECEIVED_TM31
	clearflag FLAG_RECEIVED_TM36
	clearflag FLAG_RECEIVED_TM41
	clearflag FLAG_RECEIVED_TM44
	clearflag FLAG_RECEIVED_TM45
	clearflag FLAG_RECEIVED_TM46
	clearflag FLAG_RECEIVED_TM49
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
	.string "Oh! Looks like you are loading an OLD\n"
	.string "SAVEGAME! Let's fix the SAVEGAME\l"
	.string "so you can fully enjoy this HACKROM.\p"
	.string "Let's start with the basics...\p"
	.string "In the PROF. BIRCH LAB, there're 3 PCs.\n"
	.string "If you check the TOP one, you\p"
	.string "can change the GAMEMODE.\p"
	.string "There are 2 GAMEMODES.\p"
	.string "CLASSIC, which doesn't implement\n"
	.string "the PHYSICAL/SPECIAL split\p"
	.string "created in GEN IV; and\n"
	.string "MODERN, which it does.\p"
	.string "Then, the PC on the LEFT contains a\n"
	.string "DIFFICULTY changer. NORMAL is the\p"
	.string "default experience, while EASY and HARD\n"
	.string "make the levels scale up or down\p"
	.string "10 levels after being the CHAMPION.\n"
	.string "Now... let's try to fix other things.\p"
	.string "Old EXP. SHARES are not deleted\n"
	.string "so it's recommended that you load\p"
	.string "the original SAVE into a clean\n"
	.string "version of POKéMON EMERALD, delete\p"
	.string "them, and then import the SAVE again\n"
	.string "to this HACKROM. If not, old EXP\p"
	.string "SHARES will not be deleteable\n"
	.string "from the BAG.$"

Common_EventScript_PkmnCenterNurseOldSaveText1_5:
	.string "Then, let's talk about TMs.\p"
	.string "TMs are now infinite, which means\n"
	.string "if you have used them, they are gone.\p"
	.string "Let me respawn all TMs that can be\n"
	.string "found in the overworld for you.\p"
	.string "Be sure to also talk to all\n"
	.string "the NPCs who give TMs!\p"
	.string "The MART in BATTLEFRONTIER contains\n"
	.string "all the TMs from GYM LEADERS.\p"
	.string "Go get them there!$"

Common_EventScript_PkmnCenterNurseOldSaveText2:
	.string "There. You can go look for them!\p"
	.string "And lastly, let's talk about BIKES.\p"
	.string "I'll give you this BIKE. This BIKE\n"
	.string "is the MACH and ACRO version in one.\p"
	.string "If you have the ACRO BIKE it'll be\n"
	.string "removed. I'll do the same with the\p"
	.string "EXP. SHARE, which is now a KEY ITEM.$"

Common_EventScript_PkmnCenterNurseOldSaveText3:
	.string "Thanks for your cooperation!\n"
	.string "Have fun!$"

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
