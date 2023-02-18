BattleFrontier_Lounge6_MapScripts::
	.byte 0

BattleFrontier_Lounge6_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_BATTLE_FRONTIER_TRADE_DONE, BattleFrontier_Lounge6_EventScript_TradeCompleted
	setvar VAR_0x8008, INGAME_TRADE_MEOWTH
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	msgbox BattleFrontier_Lounge6_Text_WouldYouLikeToTrade, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge6_EventScript_DeclineTrade
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge6_EventScript_DeclineTrade
	copyvar VAR_0x8005, VAR_0x800A
	specialvar VAR_RESULT, GetTradeSpecies
	copyvar VAR_0x800B, VAR_RESULT
	goto_if_ne VAR_RESULT, VAR_0x8009, BattleFrontier_Lounge6_EventScript_NotRequestedMon
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	msgbox BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt, MSGBOX_DEFAULT
	setflag FLAG_BATTLE_FRONTIER_TRADE_DONE
	release
	end

BattleFrontier_Lounge6_EventScript_DeclineTrade::
	msgbox BattleFrontier_Lounge6_Text_WellThatsFineToo, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_TradeCompleted::
	msgbox BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_Text_WouldYouLikeToTrade:
	.string "My POKéMON is a {STR_VAR_2}.\n"
	.string "Do you know it?\l"
	.string "It's quite cute and rather nice.\p"
	.string "This little one, I could trade with\n"
	.string "pride!\p"
	.string "Would you like to trade me a {STR_VAR_1}\n"
	.string "for my {STR_VAR_2}?$"

BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt:
	.string "Oh, it's adorable!\n"
	.string "Thank you!\l"
	.string "I promise I'll be good to it!\p"
	.string "Oh! I hope you'll be good to\n"
	.string "my {STR_VAR_2}, too!$"

BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon:
	.string "Oh, I'm sorry!\n"
	.string "I don't intend to trade for anything\l"
	.string "but a {STR_VAR_1}.$"

BattleFrontier_Lounge6_Text_WellThatsFineToo:
	.string "Oh, you won't?\n"
	.string "Well, that's fine, too.\l"
	.string "Please come visit us again.$"

BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined:
	.string "Giggle!\n"
	.string "A SKITTY is so much cuter than I had\l"
	.string "imagined. I'm delighted!\p"
	.string "By the way, have you seen there's a\n"
	.string "wild MEOWTH around the island?\p"
	.string "I heard it's hoarding\n"
	.string "all the money he finds.$"

BattleFrontier_Lounge6_EventScript_TotodileTrade::
	lock
	faceplayer
	goto_if_set FLAG_TOTOTRADE, BattleFrontier_Lounge6_EventScript_TradeCompletedTotodile
	setvar VAR_0x8008, INGAME_TRADE_TOTODILE
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	msgbox BattleFrontier_Lounge6_Text_WouldYouLikeToTradeTotodile, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge6_EventScript_DeclineTradeTotodile
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge6_EventScript_DeclineTradeTotodile
	copyvar VAR_0x8005, VAR_0x800A
	specialvar VAR_RESULT, GetTradeSpecies
	copyvar VAR_0x800B, VAR_RESULT
	goto_if_ne VAR_RESULT, VAR_0x8009, BattleFrontier_Lounge6_EventScript_NotRequestedMonTotodile
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	msgbox BattleFrontier_Lounge6_Text_PromiseIllBeGoodToItTotodile, MSGBOX_DEFAULT
	setflag FLAG_TOTOTRADE
	release
	end

BattleFrontier_Lounge6_EventScript_CyndaTrade::
	lock
	faceplayer
	goto_if_set FLAG_CYNDATRADE, BattleFrontier_Lounge6_EventScript_TradeCompletedTotodile
	setvar VAR_0x8008, INGAME_TRADE_CYNDAQUIL
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	msgbox BattleFrontier_Lounge6_Text_WouldYouLikeToTradeCyndaquil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge6_EventScript_DeclineTradeTotodile
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge6_EventScript_DeclineTradeTotodile
	copyvar VAR_0x8005, VAR_0x800A
	specialvar VAR_RESULT, GetTradeSpecies
	copyvar VAR_0x800B, VAR_RESULT
	goto_if_ne VAR_RESULT, VAR_0x8009, BattleFrontier_Lounge6_EventScript_NotRequestedMonTotodile
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	msgbox BattleFrontier_Lounge6_Text_PromiseIllBeGoodToItCynda, MSGBOX_DEFAULT
	setflag FLAG_CYNDATRADE
	release
	end

BattleFrontier_Lounge6_EventScript_ChikoTrade::
	lock
	faceplayer
	goto_if_set FLAG_CHIKORITRADE, BattleFrontier_Lounge6_EventScript_TradeCompletedTotodile
	setvar VAR_0x8008, INGAME_TRADE_CHIKO
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	msgbox BattleFrontier_Lounge6_Text_WouldYouLikeToTradeChikorita, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge6_EventScript_DeclineTradeTotodile
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge6_EventScript_DeclineTradeTotodile
	copyvar VAR_0x8005, VAR_0x800A
	specialvar VAR_RESULT, GetTradeSpecies
	copyvar VAR_0x800B, VAR_RESULT
	goto_if_ne VAR_RESULT, VAR_0x8009, BattleFrontier_Lounge6_EventScript_NotRequestedMonTotodile
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	msgbox BattleFrontier_Lounge6_Text_PromiseIllBeGoodToItChiko, MSGBOX_DEFAULT
	setflag FLAG_CHIKORITRADE
	release
	end

BattleFrontier_Lounge6_EventScript_DeclineTradeTotodile::
	msgbox BattleFrontier_Lounge6_Text_WellThatsFineTooTotodile, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_NotRequestedMonTotodile::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox BattleFrontier_Lounge6_Text_DontTradeForAnythingButMonTotodile, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_TradeCompletedTotodile::
	msgbox BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImaginedTotodile, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_Text_WouldYouLikeToTradeTotodile:
	.string "I'm looking for a {STR_VAR_1}\n"
	.string "with a different TRAINER ID.\p"
	.string "Do you happen to have one?\n"
	.string "I will trade you my {STR_VAR_2}.$"

BattleFrontier_Lounge6_Text_WouldYouLikeToTradeCyndaquil:
	.string "I'm looking for a {STR_VAR_1}\n"
	.string "from another region.\p"
	.string "Do you happen to have one?\n"
	.string "I will trade you my {STR_VAR_2}.$"

BattleFrontier_Lounge6_Text_WouldYouLikeToTradeChikorita:
	.string "I'm looking for a {STR_VAR_1}.\n"
	.string "Do you have it?\p"
	.string "I have a {STR_VAR_2} that I can trade.$"

BattleFrontier_Lounge6_Text_PromiseIllBeGoodToItTotodile:
	.string "Amazing!! Maybe with this trade\n"
	.string "I will be able to win the LOTTERY\p"
	.string "in LILYCOVE CITY!$"
	
BattleFrontier_Lounge6_Text_PromiseIllBeGoodToItCynda:
	.string "Thanks. Is it from another region?\n"
	.string "I hope I can find more SHINY\p"
	.string "POKéMON from now on.$"

BattleFrontier_Lounge6_Text_PromiseIllBeGoodToItChiko:
	.string "Cute! Why did I need this POKéMON\n"
	.string "for anyway?$"

BattleFrontier_Lounge6_Text_DontTradeForAnythingButMonTotodile:
	.string "Nope. This is not what\n" 
	.string "I'm looking for.$"

BattleFrontier_Lounge6_Text_WellThatsFineTooTotodile:
	.string "No? Well, okay.$"

BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImaginedTotodile:
	.string "Amazing!! Maybe with this trade\n"
	.string "I will be able to win the LOTTERY\p"
	.string "in LILYCOVE CITY!$"
