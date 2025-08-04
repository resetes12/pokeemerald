NavelRock_Fork_MapScripts::
	.byte 0

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
	msgbox YouSolvedIt, MSGBOX_DEFAULT
	closemessage
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
	msgbox YouSolvedIt, MSGBOX_DEFAULT
	closemessage
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
	msgbox YouSolvedIt, MSGBOX_DEFAULT
	closemessage
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
	msgbox YouSolvedIt, MSGBOX_DEFAULT
	closemessage
	end

NavelRock_Fork_Puzzle_Ho_Oh_Already_Solved::
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, SLIDING_PUZZLE_HO_OH
	setvar VAR_0x8005, TRUE // or TRUE if you want to display the solved puzzle
	special DoSlidingPuzzle
	waitstate
	end

YouSolvedIt:
	.string "You solved the puzzle.$"