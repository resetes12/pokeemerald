.set LOCALID_OLD_WOMAN, 2
.set LOCALID_TM_CLERK, 6

BattleFrontier_Mart_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_Mart_OnTransition
	.byte 0

BattleFrontier_Mart_OnTransition::
	call_if_unset FLAG_FINITE_TMS, BattleFrontier_Mart_HideTMClerk
	call_if_set FLAG_FINITE_TMS, BattleFrontier_Mart_ShowTMClerk
	end

BattleFrontier_Mart_HideTMClerk::
	setflag FLAG_FINITE_TMS_CLERK
	removeobject LOCALID_TM_CLERK
	return

BattleFrontier_Mart_ShowTMClerk::
	clearflag FLAG_FINITE_TMS_CLERK
	addobject LOCALID_TM_CLERK
	return

BattleFrontier_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart 0
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Mart_EventScript_Clerk2::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart BattleFrontier_Mart_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
BattleFrontier_Mart_Pokemart:
	.2byte ITEM_SAFARI_BALL
	.2byte ITEM_MAX_REVIVE
	.2byte ITEM_PP_UP
	.2byte ITEM_ETHER
	.2byte ITEM_MAX_ETHER
	.2byte ITEM_ELIXIR
	.2byte ITEM_MAX_ELIXIR
	.2byte ITEM_NONE
	release
	end

BattleFrontier_Mart_EventScript_ClerkTMs::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart BattleFrontier_Mart_PokemartTMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
BattleFrontier_Mart_PokemartTMs:
	.2byte ITEM_TM01_FOCUS_PUNCH
	.2byte ITEM_TM02_DRAGON_CLAW
	.2byte ITEM_TM03_WATER_PULSE
	.2byte ITEM_TM04_CALM_MIND
	.2byte ITEM_TM05_ROAR
	.2byte ITEM_TM06_TOXIC
	.2byte ITEM_TM07_HAIL
	.2byte ITEM_TM08_BULK_UP
	.2byte ITEM_TM09_BULLET_SEED
	.2byte ITEM_TM11_SUNNY_DAY
	.2byte ITEM_TM12_TAUNT
	.2byte ITEM_TM18_RAIN_DANCE
	.2byte ITEM_TM19_GIGA_DRAIN
	.2byte ITEM_TM22_SOLAR_BEAM
	.2byte ITEM_TM23_IRON_TAIL
	.2byte ITEM_TM26_EARTHQUAKE
	.2byte ITEM_TM28_DIG
	.2byte ITEM_TM30_SHADOW_BALL
	.2byte ITEM_TM31_BRICK_BREAK
	.2byte ITEM_TM33_REFLECT
	.2byte ITEM_TM34_SHOCK_WAVE
	.2byte ITEM_TM36_SLUDGE_BOMB
	.2byte ITEM_TM37_SANDSTORM
	.2byte ITEM_TM39_ROCK_TOMB
	.2byte ITEM_TM40_AERIAL_ACE
	.2byte ITEM_TM41_TORMENT
	.2byte ITEM_TM42_FACADE
	.2byte ITEM_TM44_REST
	.2byte ITEM_TM45_ATTRACT
	.2byte ITEM_TM46_THIEF
	.2byte ITEM_TM47_STEEL_WING
	.2byte ITEM_TM48_SKILL_SWAP
	.2byte ITEM_TM49_SNATCH
	.2byte ITEM_TM50_OVERHEAT
	.2byte ITEM_NONE
	release
	end

BattleFrontier_Mart_EventScript_OldMan::
	msgbox BattleFrontier_Mart_Text_ChaperonGrandson, MSGBOX_NPC
	end

BattleFrontier_Mart_EventScript_OldWoman::
	lock
	applymovement LOCALID_OLD_WOMAN, Common_Movement_FaceDown
	waitmovement 0
	msgbox BattleFrontier_Mart_Text_ProteinMakeNiceGift, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Mart_EventScript_Boy::
	msgbox BattleFrontier_Mart_Text_FacilitiesDontAllowItems, MSGBOX_NPC
	end

BattleFrontier_Mart_Text_ChaperonGrandson:
	.string "We came here to chaperon our\n"
	.string "grandson.\p"
	.string "But since we're here, we thought\n"
	.string "we should get some souvenirs.$"

BattleFrontier_Mart_Text_ProteinMakeNiceGift:
	.string "Dear, what do you think of this?\n"
	.string "Wouldn't this make a nice gift?\p"
	.string "It's…PRO…TE…IN?\n"
	.string "It sounds delicious, doesn't it?$"

BattleFrontier_Mart_Text_FacilitiesDontAllowItems:
	.string "A lot of the BATTLE FRONTIER's\n"
	.string "facilities don't allow the use of items\l"
	.string "during battles.\p"
	.string "That rule makes things tougher than\n"
	.string "they already are!$"

