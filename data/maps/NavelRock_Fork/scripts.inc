NavelRock_Fork_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, NavelRock_Fork_Puzzle_All_Puzzles_Completed_Check
	map_script MAP_SCRIPT_ON_LOAD, NavelRock_Fork_Puzzle_All_Puzzles_Completed_Check
	map_script MAP_SCRIPT_ON_RESUME, EncounterTable
	.byte 0

NavelRock_Fork_Puzzle_All_Puzzles_Completed_Check::
	callnative ChangeEncounterTable
	call_if_unset FLAG_ALL_PUZZLES_COMPLETED, NavelRock_Fork_Close_Unown_Ruins
	end

NavelRock_Fork_Close_Unown_Ruins::
	setmetatile 12, 3, METATILE_Cave_RockWall, TRUE
	setmetatile 13, 3, METATILE_Cave_RockWall, TRUE
	setmetatile 14, 3, METATILE_Cave_RockWall, TRUE
	setmetatile 12, 4, METATILE_Cave_RockWall, TRUE
	setmetatile 13, 4, METATILE_Cave_RockWall, TRUE
	setmetatile 14, 4, METATILE_Cave_RockWall, TRUE
	return

@@Kabuto puzzle

NavelRock_Fork_Puzzle_Kabuto::
	goto_if_set FLAG_KABUTO_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Kabuto_Already_Solved
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_KABUTO
	setvar VAR_0x8005, FALSE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	goto_if_eq VAR_RESULT, TRUE, NavelRock_Fork_Puzzle_Kabuto_Finished
	end

NavelRock_Fork_Puzzle_Kabuto_Finished::
	setflag FLAG_KABUTO_PUZZLE_SOLVED
	msgbox Puzzle_Complete_Text, MSGBOX_DEFAULT
	closemessage
	goto_if_set FLAG_KABUTO_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Kabuto_Puzzle_Completed
	end

NavelRock_Fork_Puzzle_Kabuto_Already_Solved::
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_KABUTO
	setvar VAR_0x8005, TRUE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	end

@@Omanyte puzzle

NavelRock_Fork_Puzzle_Omanyte::
	goto_if_set FLAG_OMANYTE_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Omanyte_Already_Solved
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_OMANYTE
	setvar VAR_0x8005, FALSE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	goto_if_eq VAR_RESULT, TRUE, NavelRock_Fork_Puzzle_Omanyte_Finished
	end

NavelRock_Fork_Puzzle_Omanyte_Finished::
	setflag FLAG_OMANYTE_PUZZLE_SOLVED
	msgbox Puzzle_Complete_Text, MSGBOX_DEFAULT
	closemessage
	goto_if_set FLAG_KABUTO_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Kabuto_Puzzle_Completed
	end

NavelRock_Fork_Puzzle_Omanyte_Already_Solved::
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_OMANYTE
	setvar VAR_0x8005, TRUE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	end

@@Aerodactyl puzzle

NavelRock_Fork_Puzzle_Aerodactyl::
	goto_if_set FLAG_AERODACTYL_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Aerodactyl_Already_Solved
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_AERODACTYL
	setvar VAR_0x8005, FALSE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	goto_if_eq VAR_RESULT, TRUE, NavelRock_Fork_Puzzle_Aerodactyl_Finished
	end

NavelRock_Fork_Puzzle_Aerodactyl_Finished::
	setflag FLAG_AERODACTYL_PUZZLE_SOLVED
	msgbox Puzzle_Complete_Text, MSGBOX_DEFAULT
	closemessage
	goto_if_set FLAG_KABUTO_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Kabuto_Puzzle_Completed
	end

NavelRock_Fork_Puzzle_Aerodactyl_Already_Solved::
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_AERODACTYL
	setvar VAR_0x8005, TRUE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	end

@@Ho_Oh puzzle

NavelRock_Fork_Puzzle_Ho_Oh::
	goto_if_set FLAG_HO_OH_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Ho_Oh_Already_Solved
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_HO_OH
	setvar VAR_0x8005, FALSE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	goto_if_eq VAR_RESULT, TRUE, NavelRock_Fork_Puzzle_Ho_Oh_Finished
	end

NavelRock_Fork_Puzzle_Ho_Oh_Finished::
	setflag FLAG_HO_OH_PUZZLE_SOLVED
	msgbox Puzzle_Complete_Text, MSGBOX_DEFAULT
	closemessage
	goto_if_set FLAG_KABUTO_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Kabuto_Puzzle_Completed
	end

NavelRock_Fork_Puzzle_Ho_Oh_Already_Solved::
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_HO_OH
	setvar VAR_0x8005, TRUE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	
NavelRock_Fork_Puzzle_Kabuto_Puzzle_Completed::
	goto_if_set FLAG_OMANYTE_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Omanyte_Puzzle_Completed
	end

NavelRock_Fork_Puzzle_Omanyte_Puzzle_Completed::
	goto_if_set FLAG_AERODACTYL_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_Aerodactyl_Puzzle_Completed
	end

NavelRock_Fork_Puzzle_Aerodactyl_Puzzle_Completed::
	goto_if_set FLAG_HO_OH_PUZZLE_SOLVED, NavelRock_Fork_Puzzle_All_Puzzles_Completed
	end

NavelRock_Fork_Puzzle_All_Puzzles_Completed::
	setflag FLAG_ALL_PUZZLES_COMPLETED
	delay 60
	setmetatile 12, 3, METATILE_Cave_NormalHole_TopLeft, TRUE
	setmetatile 13, 3, METATILE_Cave_NormalHole_TopMid, TRUE
	setmetatile 14, 3, METATILE_Cave_NormalHole_TopRight, TRUE
	setmetatile 12, 4, METATILE_Cave_NormalHole_BottomLeft, TRUE
	setmetatile 13, 4, METATILE_Cave_NormalHole_BottomMid, FALSE
	setmetatile 14, 4, METATILE_Cave_NormalHole_BottomRight, TRUE
	special DrawWholeMapView
	playse SE_BANG
	release
	end

Puzzle_Complete_Text:
	.string "The puzzle now displays an\n"
	.string "ancient POKéMON.$"
