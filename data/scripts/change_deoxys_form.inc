ChangeDeoxysForm::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, ChangeDeoxysForm_Movement
	waitmovement 0
	msgbox ChangeDeoxysForm_Text_Beginning, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq ChangeDeoxysForm_Start
ChangeDeoxysForm_End:
	msgbox ChangeDeoxysForm_Text_End, MSGBOX_DEFAULT
	callnative UpdateFollowingPokemon
	releaseall
	end

ChangeDeoxysForm_End2::
	msgbox ChangeDeoxysForm_Text_End2, MSGBOX_DEFAULT
	releaseall
	end

ChangeDeoxysForm_Start::
	msgbox ChangeDeoxysForm_Text_Select, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq ChangeDeoxysForm_End
	compare VAR_RESULT, YES
	goto_if_eq ChangeDeoxysForm_Start2

ChangeDeoxysForm_Start2::
	special ChoosePartyMon
	waitstate
	compare VAR_0x8004, 255
	goto_if_eq ChangeDeoxysForm_End
	special TryChangeDeoxysForm
	compare VAR_RESULT, TRUE
	goto_if_eq ChangeDeoxysForm_Success
	compare VAR_RESULT, FALSE
	goto_if_eq ChangeDeoxysForm_End2
	goto ChangeDeoxysForm_End

ChangeDeoxysForm_Success:
	playmoncry SPECIES_DEOXYS, 0
	msgbox ChangeDeoxysForm_Text_Success, MSGBOX_DEFAULT
	waitmoncry
	waitbuttonpress
	goto ChangeDeoxysForm_End

ChangeDeoxysForm_Text_Beginning:
	.string "It's a strange meteorite\n"
	.string "Do you want to touch it?$"

ChangeDeoxysForm_Text_End:
	.string "I should get going.$"

ChangeDeoxysForm_Text_End2:
	.string "My POKéMON enjoyed touching the\n"
	.string "meteorite more than I expected!\p"
	.string "I should get going.$"

ChangeDeoxysForm_Text_Select:
	.string "…\p"
	.string "…\p"
	.string "The surface has a rough feeling.\p"
	.string "Should I let any of my POKéMON\n"
	.string "touch this meteorite?$"

ChangeDeoxysForm_Text_Success:
	.string "Something has happened to DEOXYS!$"

ChangeDeoxysForm_Movement:
	emote_question_mark
	step_end
