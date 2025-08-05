NavelRock_UnownChamber_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, NavelRock_UnownChamber_CheckUnownChamberState
	map_script MAP_SCRIPT_ON_LOAD, NavelRock_UnownChamber_CheckUnownChamberState
	.byte 0

NavelRock_UnownChamber_CheckUnownChamberState:
	call_if_ne VAR_UNOWN_CHAMBER_STATE, 8, NavelRock_UnownChamber_CloseExitDoor
	end

NavelRock_UnownChamber_CloseExitDoor:
	setmetatile 11, 4, METATILE_Cave_EntranceCover, TRUE
	setmetatile 12, 4, METATILE_Cave_EntranceCover, TRUE
	setmetatile 13, 4, METATILE_Cave_EntranceCover, TRUE
	setmetatile 11, 5, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 12, 5, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 13, 5, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	return

NavelRock_UnownChamber_Braille_Main_Door_Left::
	braillemessage UnownChamber_Left_Braille_Text
	waitbuttonpress
	closebraillemessage
	end

NavelRock_UnownChamber_Braille_Main_Door_Mid::
	braillemessage UnownChamber_Mid_Braille_Text
	waitbuttonpress
	closebraillemessage
	end

NavelRock_UnownChamber_Braille_Main_Door_Right::
	braillemessage UnownChamber_Right_Braille_Text
	waitbuttonpress
	closebraillemessage
	end

NavelRock_UnownChamber_Braille_Top_Left::
	braillemessage UnownChamber_Random_Text_1
	waitbuttonpress
	closebraillemessage
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, EndUnownScript
	setvar VAR_UNOWN_CHAMBER_STATE, 1
	end

NavelRock_UnownChamber_Braille_Top_Mid_Left::
	braillemessage UnownChamber_Random_Text_2
	waitbuttonpress
	closebraillemessage
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, EndUnownScript
	goto_if_ne VAR_UNOWN_CHAMBER_STATE, 2, ResetUnownChamberState
	setvar VAR_UNOWN_CHAMBER_STATE, 3
	end

NavelRock_UnownChamber_Braille_Bottom_Mid_Left::
	braillemessage UnownChamber_Random_Text_3
	waitbuttonpress
	closebraillemessage
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, EndUnownScript
	goto_if_ne VAR_UNOWN_CHAMBER_STATE, 4, ResetUnownChamberState
	setvar VAR_UNOWN_CHAMBER_STATE, 5
	end


NavelRock_UnownChamber_Braille_Bottom_Left::
	braillemessage UnownChamber_Random_Text_4
	waitbuttonpress
	closebraillemessage
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, EndUnownScript
	goto_if_ne VAR_UNOWN_CHAMBER_STATE, 6, ResetUnownChamberState
	setvar VAR_UNOWN_CHAMBER_STATE, 7
	end

NavelRock_UnownChamber_Braille_Top_Right::
	braillemessage UnownChamber_Random_Text_5
	waitbuttonpress
	closebraillemessage
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, EndUnownScript
	goto_if_ne VAR_UNOWN_CHAMBER_STATE, 1, ResetUnownChamberState
	setvar VAR_UNOWN_CHAMBER_STATE, 2
	end

NavelRock_UnownChamber_Braille_Top_Mid_Right::
	braillemessage UnownChamber_Random_Text_6
	waitbuttonpress
	closebraillemessage
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, EndUnownScript
	goto_if_ne VAR_UNOWN_CHAMBER_STATE, 3, ResetUnownChamberState
	setvar VAR_UNOWN_CHAMBER_STATE, 4
	end

NavelRock_UnownChamber_Braille_Bottom_Mid_Right::
	braillemessage UnownChamber_Random_Text_7
	waitbuttonpress
	closebraillemessage
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, EndUnownScript
	goto_if_ne VAR_UNOWN_CHAMBER_STATE, 5, ResetUnownChamberState
	setvar VAR_UNOWN_CHAMBER_STATE, 6
	end


NavelRock_UnownChamber_Braille_Bottom_Right::
	braillemessage UnownChamber_Random_Text_8
	waitbuttonpress
	closebraillemessage
	goto_if_set FLAG_UNOWN_CHAMBER_OPEN, EndUnownScript
	goto_if_ne VAR_UNOWN_CHAMBER_STATE, 7, ResetUnownChamberState
	setvar VAR_UNOWN_CHAMBER_STATE, 8
	goto_if_eq VAR_UNOWN_CHAMBER_STATE, 8, OpenUnownChamber
	end

OpenUnownChamber:
	delay 60
	setmetatile 11, 4, METATILE_Cave_SealedChamberEntrance_TopLeft, TRUE
	setmetatile 12, 4, METATILE_Cave_SealedChamberEntrance_TopMid, TRUE
	setmetatile 13, 4, METATILE_Cave_SealedChamberEntrance_TopRight, TRUE
	setmetatile 11, 5, METATILE_Cave_SealedChamberEntrance_BottomLeft, TRUE
	setmetatile 12, 5, METATILE_Cave_SealedChamberEntrance_BottomMid, FALSE
	setmetatile 13, 5, METATILE_Cave_SealedChamberEntrance_BottomRight, TRUE
	special DrawWholeMapView
	playse SE_BANG
	setflag FLAG_UNOWN_CHAMBER_OPEN
	end

EndUnownScript:
	end

ResetUnownChamberState:
	setvar VAR_UNOWN_CHAMBER_STATE, 0
	end

Test_Message_Unown:
	.string "Test$"
