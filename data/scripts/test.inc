.set LOCALID_A, 3
.set LOCALID_B, 3
.set LOCALID_C, 3
.set LOCALID_D, 4
.set LOCALID_E, 2
.set LOCALID_F, 4
.set LOCALID_G, 14
.set LOCALID_H, 6
.set LOCALID_I, 7
.set LOCALID_J, 2
.set LOCALID_K, 3
.set LOCALID_L, 3
.set LOCALID_M, 9
.set LOCALID_N, 4
.set LOCALID_O, 3
.set LOCALID_P, 11
.set LOCALID_Q, 8
.set LOCALID_R, 10
.set LOCALID_S, 2
.set LOCALID_T, 12
.set LOCALID_U, 3
.set LOCALID_V, 4
.set LOCALID_W, 6
.set LOCALID_X, 3
.set LOCALID_Y, 6
.set LOCALID_Z, 9
.set LOCALID_EM, 1
.set LOCALID_QM, 2
@…
@‘
@’

@A

U_A_Sign::
	lockall
	msgbox U_A_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x26A, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_A_Check_Flag
	end

U_Show_A_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_A
	end

U_Show_A::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_A, U_Show_A_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_A_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_A_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_A, Went_Away_U_A_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x26A
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_A_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_A_Movement:
	emote_exclamation_mark
	delay_8
	face_left
	step_end

Went_Away_U_A_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_A_Text:
	.string "POKéMON magazines!\n"
	.string "POKéMON PAL…\p"
	.string "POKéMON HANDBOOK…\n"
	.string "ADORABLE POKéMON…\p"
	.string "The 28 forms of UNOWN…\n"
	.string "The first one is A.$"

U_A_Text_Found:
	.string "A strange creature appeared.\p"
	.string "{COLOR BLUE}You found UNOWN A!\p"
	.string "UNOWN A returned to its cave.$"

@B

U_B_Sign::
	lockall
	msgbox U_B_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x26B, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_B_Check_Flag
	end

U_Show_B_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_B
	end

U_Show_B::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_A, U_Show_B_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_B_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_B_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_B, Went_Away_U_B_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x26B
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_B_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_B_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_B_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_B_Text:
	.string "This vase looks expensive…\n"
	.string "Peered inside…\p"
	.string "There's a note inside. It reads\n"
	.string "as follows:\p" 
	.string "‘When alone, nothing happens.\n"
	.string "However, if there are two or more,\l"
	.string "an odd power is said to emerge.'$"

U_B_Text_Found:
	.string "A strange creature appeared.\p"
	.string "{COLOR BLUE}You found UNOWN B!\p"
	.string "UNOWN B returned to its cave.$"

@C

U_C_Sign::
	lockall
	msgbox U_C_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x26C, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_C_Check_Flag
	end

U_Show_C_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_C
	end

U_Show_C::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_C, U_Show_C_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_C_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_C_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_C, Went_Away_U_C_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x26C
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_C_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_C_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_C_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_C_Text:
	.string "There's a note on the fridge. It reads\n"
	.string "as follows:\p"
	.string "In the beginning, there was the WORD,\n"
	.string "and the WORD was with GOD,\l"
	.string "and the WORD was GOD.\p"
	.string "There's also a C-shaped chocolate\n"
	.string "cake inside.$"

U_C_Text_Found:
	.string "The chocolate cake moved on its own.\n"
	.string "It was actually a POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN C!\p"
	.string "UNOWN C returned to its cave.$"

@D

U_D_Sign::
	lockall
	msgbox U_D_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x26D, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_D_Check_Flag
	end

U_Show_D_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_D
	end

U_Show_D::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_D, U_Show_D_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_D_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_D_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_D, Went_Away_U_D_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x26D
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_D_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_D_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_D_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_D_Text:
	.string "There's a journal dating from many\n"
	.string "years ago. It's quite unreadable, but\l"
	.string "you can make out some pages.\p"
	.string "‘One refers to a strange, letter-like\n"
	.string "creature, able to transcend and\l"
	.string "move between different dimensions.'$"

U_D_Text_Found:
	.string "There was actually something else\n"
	.string "behind the diary. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN D!\p"
	.string "UNOWN D returned to its cave.$"

@E

U_E_Sign::
	lockall
	msgbox U_E_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x26E, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_E_Check_Flag
	end

U_Show_E_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_E
	end

U_Show_E::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_E, U_Show_E_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_E_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_E_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_E, Went_Away_U_E_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x26E
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_E_Movement:
	walk_left
	walk_left
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_E_Movement:
	emote_exclamation_mark
	delay_8
	face_down
	step_end

Went_Away_U_E_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_E_Text:
	.string "This rock…\p"
	.string "There's something about it.\p"
	.string "It's marked with the letter ‘E'.$"

U_E_Text_Found:
	.string "A POKéMON suddenly jumped from\n"
	.string "behind the rock!\p"
	.string "{COLOR BLUE}You found UNOWN E!\p"
	.string "UNOWN E returned to its cave.$"

@F

U_F_Sign::
	lockall
	msgbox U_F_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x26F, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_F_Check_Flag
	end

U_Show_F_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_F
	end

U_Show_F::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_F, U_Show_F_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_F_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_F_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_F, Went_Away_U_F_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x26F
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_F_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_F_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_F_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_F_Text:
	.string "The telephone has a signal, and there's\n"
	.string "a voice that loops constantly.\p"
	.string "‘They seem to communicate among\n"
	.string "themselves telepathically.\l"
	.string "They are always found stuck to walls.'\p"
	.string "You put the phone away.$"

U_F_Text_Found:
	.string "There was actually something stuck\n"
	.string "behind a poster. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN F!\p"
	.string "UNOWN F returned to its cave.$"

@G

U_G_Sign::
	lockall
	msgbox U_G_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x270, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_G_Check_Flag
	end

U_Show_G_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_G
	end

U_Show_G::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_G, U_Show_G_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_G_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_G_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_G, Went_Away_U_G_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x270
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_G_Movement:
	walk_down
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_G_Movement:
	emote_exclamation_mark
	delay_8
	face_down
	step_end

Went_Away_U_G_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_G_Text:
	.string "There are strange markings\n"
	.string "on the wall.\p"
	.string "Whatever is written on it doesn't\n"
	.string "make much sense, but you can\l"
	.string "distinguish the letter ‘G'.$"

U_G_Text_Found:
	.string "A creature jumped from the\n"
	.string "left counter!\p"
	.string "{COLOR BLUE}You found UNOWN G!\p"
	.string "UNOWN G returned to its cave.$"

@H

U_H_Sign::
	lockall
	msgbox U_H_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x271, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_H_Check_Flag
	end

U_Show_H_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_H
	end

U_Show_H::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_H, U_Show_H_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_H_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_H_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_H, Went_Away_U_H_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x271
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_H_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_H_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_H_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_H_Text:
	.string "There's something odd about\n"
	.string "the drawer.\p"
	.string "There's a sticker that has the\n"
	.string "letter ‘H' written on it.$"

U_H_Text_Found:
	.string "A POKéMON jump from the drawer!\p"
	.string "{COLOR BLUE}You found UNOWN H!\p"
	.string "UNOWN H returned to its cave.$"

@I

U_I_Sign::
	lockall
	msgbox U_I_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x272, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_I_Check_Flag
	end

U_Show_I_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_I
	end

U_Show_I::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_I, U_Show_I_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_I_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_I_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_I, Went_Away_U_I_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x272
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_I_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_I_Movement:
	emote_exclamation_mark
	delay_8
	face_left
	step_end

Went_Away_U_I_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_I_Text:
	.string "The shelves brim with all sorts of\n"
	.string "POKéMON merchandise.\p"
	.string "There's even a strangely I-shaped\n"
	.string "plush POKéMON!$"

U_I_Text_Found:
	.string "The plush was actually a POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN I!\p"
	.string "UNOWN I returned to its cave.$"

@J

U_J_Sign::
	lockall
	msgbox U_J_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x273, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_J_Check_Flag
	end

U_Show_J_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_J
	end

U_Show_J::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_J, U_Show_J_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_J_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_J_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_J, Went_Away_U_J_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x273
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_J_Movement:
	walk_right
	walk_right
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_J_Movement:
	emote_exclamation_mark
	delay_8
	face_up
	step_end

Went_Away_U_J_Movement:
	set_invisible
	walk_left
	walk_left
	step_end

U_J_Text:
	.string "There's a post-it note. It reads\n"
	.string "as follows:\p"
	.string "‘Shaped like ancient writing,\n"
	.string "it is a huge mystery whether\l"
	.string "language or them came first.'$"

U_J_Text_Found:
	.string "There was actually something else\n"
	.string "behind the boxes. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN J!\p"
	.string "UNOWN J returned to its cave.$"

@K

U_K_Sign::
	lockall
	msgbox U_K_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x274, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_K_Check_Flag
	end

U_Show_K_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_K
	end

U_Show_K::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_K, U_Show_K_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_K_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_K_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_K, Went_Away_U_K_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x274
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_K_Movement:
	walk_left
	walk_left
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_K_Movement:
	emote_exclamation_mark
	delay_8
	face_down
	step_end

Went_Away_U_K_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_K_Text:
	.string "The VENDING MACHINE is pretty\n"
	.string "much empty. Only one can of\l"
	.string "coffee remains.\p"
	.string "It looks like the can is moving\n"
	.string "on its own. That can't be.$"

U_K_Text_Found:
	.string "The can of coffee moved!\n"
	.string "It was a POKéMON all along!\p"
	.string "{COLOR BLUE}You found UNOWN K!\p"
	.string "UNOWN K returned to its cave.$"

@L

U_L_Sign::
	lockall
	msgbox U_L_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x275, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_L_Check_Flag
	end

U_Show_L_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_L
	end

U_Show_L::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_L, U_Show_L_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_L_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_L_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_L, Went_Away_U_L_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x275
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_L_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_L_Movement:
	emote_exclamation_mark
	delay_8
	face_left
	step_end

Went_Away_U_L_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_L_Text:
	.string "There's something odd about\n"
	.string "the drawer.\p"
	.string "There's a label that has an\n"
	.string "L-shaped drawing.$"

U_L_Text_Found:
	.string "A POKéMON jump from the drawer!\p"
	.string "{COLOR BLUE}You found UNOWN L!\p"
	.string "UNOWN L returned to its cave.$"

@M

U_M_Sign::
	lockall
	msgbox U_M_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x276, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_M_Check_Flag
	end

U_Show_M_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_M
	end

U_Show_M::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_M, U_Show_M_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_M_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_M_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_M, Went_Away_U_M_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x276
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_M_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_M_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_M_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_M_Text:
	.string "It's filled with all sorts of books.\p"
	.string "There's an insteresting book about\n"
	.string "ancient writings.\p"
	.string "‘They are shaped like ancient writing.\n"
	.string "It is a mystery as to which came first.\l" 
	.string "Research into this topic is ongoing\l"
	.string "but nothing is known.'$"

U_M_Text_Found:
	.string "There was actually something else\n"
	.string "behind the bookshelf. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN M!\p"
	.string "UNOWN M returned to its cave.$"

@N

U_N_Sign::
	lockall
	msgbox U_N_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x277, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_N_Check_Flag
	end

U_Show_N_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_N
	end

U_Show_N::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_N, U_Show_N_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_N_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_N_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_N, Went_Away_U_N_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x277
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_N_Movement:
	walk_right
	walk_up
	walk_up
	walk_up
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_N_Movement:
	emote_exclamation_mark
	delay_8
	face_up
	step_end

Went_Away_U_N_Movement:
	set_invisible
	walk_down
	walk_down
	walk_down
	walk_left
	step_end

U_N_Text:
	.string "There's an oddly shaped fruit!\n"
	.string "It looks like the ‘N' letter.$" 

U_N_Text_Found:
	.string "That was no fruit!\n"
	.string "It was a POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN N!\p"
	.string "UNOWN N returned to its cave.$"

@O

U_O_Sign::
	lockall
	msgbox U_O_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x278, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_O_Check_Flag
	end

U_Show_O_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_O
	end

U_Show_O::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_O, U_Show_O_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_O_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_O_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_O, Went_Away_U_O_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x278
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_O_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_O_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_O_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_O_Text:
	.string "This tombstone has some text\n"
	.string "engraved on its surface:\p"
	.string "‘They exist partially in a parallel\n"
	.string "dimension scholars call the …….'\p"
	.string "The last sentence is unreadable.\n"
	.string "It continues:\p"
	.string "‘Objects and creatures there are\n"
	.string "shaped not by matter, but by ideas.'$"

U_O_Text_Found:
	.string "A POKéMON was behind the grave!\p"
	.string "{COLOR BLUE}You found UNOWN O!\p"
	.string "UNOWN O returned to its cave.$"

@P

U_P_Sign::
	lockall
	msgbox U_P_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x279, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_P_Check_Flag
	end

U_Show_P_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_P
	end

U_Show_P::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_P, U_Show_P_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_P_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_P_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_P, Went_Away_U_P_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x279
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_P_Movement:
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_P_Movement:
	emote_exclamation_mark
	delay_8
	face_DOWN
	step_end

Went_Away_U_P_Movement:
	set_invisible
	step_end

U_P_Text:
	.string "There's a small plaque with text:\p"
	.string "‘Ancient stone carvings depict people\n"
	.string "kneeling before swirling clouds of\l"
	.string "them, suggesting early civilizations\l"
	.string "revered - or feared - them\l"
	.string "as messengers.’$"

U_P_Text_Found:
	.string "A POKéMON suddenly appeared!\p"
	.string "{COLOR BLUE}You found UNOWN P!\p"
	.string "UNOWN P returned to its cave.$"

@Q

U_Q_Sign::
	lockall
	msgbox U_Q_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x27A, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_Q_Check_Flag
	end

U_Show_Q_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_Q
	end

U_Show_Q::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_Q, U_Show_Q_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_Q_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_Q_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_Q, Went_Away_U_Q_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x27A
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_Q_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_Q_Movement:
	emote_exclamation_mark
	delay_8
	face_left
	step_end

Went_Away_U_Q_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_Q_Text:
	.string "There's a biology book from a\n"
	.string "famous scientist:\p"
	.string "‘When their activity peaks,\n"
	.string "compass needles spin endlessly,\l"
	.string "clocks run backwards or skip\l"
	.string "minutes, carvings change shape\l"
	.string "when unobserved, and whispered\l"
	.string "words echo in your own voice.'$"

U_Q_Text_Found:
	.string "There was actually something else\n"
	.string "behind the furniture. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN Q!\p"
	.string "UNOWN Q returned to its cave.$"

@R

U_R_Sign::
	lockall
	msgbox U_R_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x27B, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_R_Check_Flag
	end

U_Show_R_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_R
	end

U_Show_R::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_R, U_Show_R_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_R_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_R_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_R, Went_Away_U_R_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x27B
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_R_Movement:
	walk_right
	walk_right
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_R_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_R_Movement:
	set_invisible
	walk_up
	walk_up
	walk_up
	step_end

U_R_Text:
	.string "There's a piece of coal on the cabinet.\n"
	.string "Its shape oddly resembles the ‘R' letter.$"

U_R_Text_Found:
	.string "That was no coal! It was a POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN R!\p"
	.string "UNOWN R returned to its cave.$"

@S

U_S_Sign::
	lockall
	msgbox U_S_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x27C, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_S_Check_Flag
	end

U_Show_S_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_S
	end

U_Show_S::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_S, U_Show_S_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_S_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_S_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_S, Went_Away_U_S_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x27C
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_S_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_S_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_S_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_S_Text:
	.string "{CLEAR_TO 0x0a}Wanted item{CLEAR_TO 0x7c}Trade item\n"
	.string "{CLEAR_TO 0x0f}RED SHARD{CLEAR_TO 0x59}{LEFT_ARROW}{RIGHT_ARROW}{CLEAR_TO 0x7b}FIRE STONE{CLEAR_TO 0xc8}\p"
	.string "{CLEAR_TO 0x0a}Wanted item{CLEAR_TO 0x7c}Trade item\n"
	.string "{CLEAR_TO 0x06}YELLOW SHARD{CLEAR_TO 0x59}{LEFT_ARROW}{RIGHT_ARROW}{CLEAR_TO 0x73}THUNDERSTONE{CLEAR_TO 0xc8}\p"
	.string "{CLEAR_TO 0x0a}Wanted item{CLEAR_TO 0x7c}Trade item\n"
	.string "{CLEAR_TO 0x0c}BLUE SHARD{CLEAR_TO 0x59}{LEFT_ARROW}{RIGHT_ARROW}{CLEAR_TO 0x79}WATER STONE{CLEAR_TO 0xc8}\p"
	.string "{CLEAR_TO 0x0a}Wanted item{CLEAR_TO 0x7c}Trade item\n"
	.string "{CLEAR_TO 0x08}GREEN SHARD{CLEAR_TO 0x59}{LEFT_ARROW}{RIGHT_ARROW}{CLEAR_TO 0x7b}LEAF STONE\p"
	.string "There's also a big, bold letter ‘S'.$"

U_S_Text_Found:
	.string "Huh? There's something behind the\n"
	.string "poster. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN S!\p"
	.string "UNOWN S returned to its cave.$"

@T

U_T_Sign::
	lockall
	msgbox U_T_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x27D, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_T_Check_Flag
	end

U_Show_T_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_T
	end

U_Show_T::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_T, U_Show_T_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_T_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_T_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_T, Went_Away_U_T_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x27D
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_T_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_T_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_T_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_T_Text:
	.string "There's something on the screen:\p"
	.string "‘A rare formation, called the Eye\n"
	.string "Spiral, has only been seen twice.\l"
	.string "Witnesses describe a sensation of\l"
	.string "falling inward endlessly, followed\l"
	.string "by waking in a place slightly\l"
	.string "wrong where colors are duller,\l"
	.string "and shadows linger too long.'$"

U_T_Text_Found:
	.string "There was actually something else\n"
	.string "behind the machine. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN T!\p"
	.string "UNOWN T returned to its cave.$"

@U

U_U_Sign::
	lockall
	msgbox U_U_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x27E, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_U_Check_Flag
	end

U_Show_U_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_U
	end

U_Show_U::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_U, U_Show_U_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_U_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_U_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_U, Went_Away_U_U_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x27E
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_U_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_U_Movement:
	emote_exclamation_mark
	delay_8
	face_left
	step_end

Went_Away_U_U_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_U_Text:
	.string "The model ship is incredible!\p"
	.string "It's shaped like the letter ‘U'.$"

U_U_Text_Found:
	.string "There was actually something else\n"
	.string "behind the model. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN U!\p"
	.string "UNOWN U returned to its cave.$"

@V

U_V_Sign::
	lockall
	msgbox U_V_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x27F, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_V_Check_Flag
	end

U_Show_V_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_V
	end

U_Show_V::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_V, U_Show_V_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_V_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_V_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_V, Went_Away_U_V_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x27F
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_V_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_V_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_V_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_V_Text:
	.string "There's something odd about\n"
	.string "the drawer.\p"
	.string "There's written ‘V'.$"

U_V_Text_Found:
	.string "A POKéMON jump from the drawer!\p"
	.string "{COLOR BLUE}You found UNOWN V!\p"
	.string "UNOWN V returned to its cave.$"

@W

U_W_Sign::
	lockall
	msgbox U_W_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x280, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_W_Check_Flag
	end

U_Show_W_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_W
	end

U_Show_W::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_W, U_Show_W_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_W_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_W_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_W, Went_Away_U_W_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x280
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_W_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_W_Movement:
	emote_exclamation_mark
	delay_8
	face_left
	step_end

Went_Away_U_W_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_W_Text:
	.string "POKéMON magazines!\n"
	.string "POKéMON PAL…\p"
	.string "POKéMON HANDBOOK…\n"
	.string "ADORABLE POKéMON…\p"
	.string "THE STRANGE SHAPES OF UNOWN…$"

U_W_Text_Found:
	.string "A strange creature appeared.\p"
	.string "{COLOR BLUE}You found UNOWN W!\p"
	.string "UNOWN W returned to its cave.$"

@X

U_X_Sign::
	lockall
	msgbox U_X_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x281, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_X_Check_Flag
	end

U_Show_X_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_X
	end

U_Show_X::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_X, U_Show_X_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_X_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_X_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_X, Went_Away_U_X_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x281
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_X_Movement:
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_X_Movement:
	emote_exclamation_mark
	delay_8
	face_up
	step_end

Went_Away_U_X_Movement:
	set_invisible
	step_end

U_X_Text:
	.string "This rock…\p"
	.string "It's marked with an ‘X'.$"

U_X_Text_Found:
	.string "A POKéMON suddenly jumped from\n"
	.string "behind the rock!\p"
	.string "{COLOR BLUE}You found UNOWN X!\p"
	.string "UNOWN X returned to its cave.$"

@Y

U_Y_Sign::
	lockall
	msgbox U_Y_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x282, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_Y_Check_Flag
	end

U_Show_Y_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_Y
	end

U_Show_Y::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_Y, U_Show_Y_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_Y_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_Y_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_Y, Went_Away_U_Y_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x282
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_Y_Movement:
	walk_up
	walk_up
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_Y_Movement:
	emote_exclamation_mark
	delay_8
	face_left
	step_end

Went_Away_U_Y_Movement:
	set_invisible
	walk_down
	walk_down
	step_end

U_Y_Text:
	.string "The tree's leaves are slowly\n"
	.string "moving, though no air current\l"
	.string "surrounds them.$"

U_Y_Text_Found:
	.string "There was actually something else\n"
	.string "behind the tree. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN Y!\p"
	.string "UNOWN Y returned to its cave.$"

@Z

U_Z_Sign::
	lockall
	msgbox U_Z_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x283, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_Z_Check_Flag
	end

U_Show_Z_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_Z
	end

U_Show_Z::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_Z, U_Show_Z_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_Z_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_Z_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_Z, Went_Away_U_Z_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x283
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_Z_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_Z_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_Z_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_Z_Text:
	.string "The box reads, in bright red letters:\n"
	.string "{COLOR RED}‘DO NOT OPEN! DANGER!'$"

U_Z_Text_Found:
	.string "Curiosity got the better of you!\p"
	.string "And after opening the box, something\n"
	.string "leapt out with force. A POKéMON!\p"
	.string "{COLOR BLUE}You found UNOWN Z!\p"
	.string "UNOWN Z returned to its cave.$"

@EM

U_EM_Sign::
	lockall
	msgbox U_EM_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x284, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_EM_Check_Flag
	end

U_Show_EM_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_EM
	end

U_Show_EM::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_EM, U_Show_EM_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_EM_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_EM_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_EM, Went_Away_U_EM_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x284
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_EM_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_EM_Movement:
	emote_exclamation_mark
	delay_8
	face_right
	step_end

Went_Away_U_EM_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_EM_Text:
	.string "There are markings on the wall.\n"
	.string "They read:\p"
	.string "‘This room is a mirror!'$"

U_EM_Text_Found:
	.string "A POKéMON suddently appeared!\p"
	.string "{COLOR BLUE}You found UNOWN ‘!'!\p"
	.string "UNOWN ‘!' returned to its cave.$"

@QM

U_QM_Sign::
	lockall
	msgbox U_QM_Text, MSGBOX_SIGN
	closemessage
	goto_if_set FLAG_UNUSED_0x285, Test_Event_Check_U_Remaining
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, U_Show_QM_Check_Flag
	end

U_Show_QM_Check_Flag::
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, U_Show_QM
	end

U_Show_QM::
	callnative RemoveFollowingPokemon
	applymovement LOCALID_QM, U_Show_QM_Movement
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Show_Player_U_QM_Movement
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox U_QM_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_QM, Went_Away_U_QM_Movement
	waitmovement 0
	subvar VAR_TEST, 1
	setflag FLAG_UNUSED_0x285
	callnative UpdateFollowingPokemon
	goto_if_eq VAR_TEST, 0, Test_Event_All_U_Found
	goto Test_Event_Check_U_Remaining
	releaseall
	end

U_Show_QM_Movement:
	walk_down
	walk_down
	set_visible
	jump_in_place_down
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	step_end

Show_Player_U_QM_Movement:
	emote_exclamation_mark
	delay_8
	face_left
	step_end

Went_Away_U_QM_Movement:
	set_invisible
	walk_up
	walk_up
	step_end

U_QM_Text:
	.string "There are markings on the wall.\n"
	.string "They read:\p"
	.string "‘This room is a mirror?'$"

U_QM_Text_Found:
	.string "A POKéMON suddently appeared!\p"
	.string "{COLOR BLUE}You found UNOWN ‘?'!\p"
	.string "UNOWN ‘?' returned to its cave.$"

@COMMON SCRIPTS

Test_Event_Check_End::
	releaseall
	end

Test_Event_Check_U_Remaining::
	fadedefaultbgm
	buffernumberstring STR_VAR_1, VAR_TEST
	msgbox Test_Event_Check_U_Remaining_Text, MSGBOX_DEFAULT
	closemessage
	return

Test_Event_Check_U_Remaining_Text:
	.string "{COLOR RED}There are {STR_VAR_1} UNOWN remaining.$"

Test_Event_All_U_Found::
	playmoncry SPECIES_UNOWN, CRY_MODE_NORMAL
	waitmoncry
	msgbox Test_Event_All_U_Found_Text, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_TEST_DONE
	releaseall
	end

Test_Event_All_U_Found_Text:
	.string "{COLOR RED}You found all UNOWN forms!\p"
	.string "Checking UNOWN CHAMBER would be wise.$"
