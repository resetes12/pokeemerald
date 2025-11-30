# Pokémon Modern Emerald

This is a decompilation of Pokémon Emerald, edited to be "Pokémon Modern Emerald".
You can get more information about Modern Emerald at [Pokecommunity](https://www.pokecommunity.com/showthread.php?t=494005)
You can the hackrom documentation following [this link](https://docs.google.com/spreadsheets/d/1QEFJmFhfaZqgxSUnM7MFpufrnrRk_NMzNoNWl2m3y_0/edit#gid=414283818)

# If you want to compile:

Please follow [Pret's guide on how to build the rom](https://github.com/pret/pokeemerald/blob/master/INSTALL.md) but using this branch instead of theirs. 
When compiling, **use the modern compiler** with the "make modern" command. 
_Compiling using the old compiler won't work._
The game was compiled using `(devkitARM release 62) 13.2.0`, but it _should_ work up to `release 65` without any issues. Higher versions may not work, so manual fixes might be required.

# 📃 DOCUMENTATION:
[Documentation can be found online](https://docs.google.com/spreadsheets/d/1QEFJmFhfaZqgxSUnM7MFpufrnrRk_NMzNoNWl2m3y_0/edit?gid=1310408794#gid=1310408794)
Includes:
- Pokémon location and changes (plus an extra, more specific Pokémon location).
- Static and special encounters.
- New item locations (only includes new locations, old locations still work unless told otherwise).
- Other information that could be relevant.

# ✨FEATURES

**Selectable options (at the start of the game):**
Check which options are enabled or disabled by going to any Pokécenter PC and opening the "CHALLENGES" option.

Gamemode page:

* Choose between a "Classic" or "Modern" preset, or customize it to your liking. **WARNING! All selected options are permanent until starting a new game.**
* Encounter modes: Original, New (aka Modern), and Post-game.
    - "Original encounters" are vanilla Emerald encounters with zero changes.
    - "New (modern) encounters" include all 423 Pokémon between the game and the post-game.
    - "Post-game encounters" are vanilla Emerald encounters UNTIL beating the game. Afterwards, it uses the "New encounters" tables.
* Modern Typings: Some Pokémon have their types changed to buff them (Check docs).
* Add Fairy Type: Adds Fairy Type to Pokémon that had it added in Generation 6.
* Better Stats: Some Pokémon have their stats changed to buff them (Check docs).
* Extra Legendaries: Adds new legendaries that weren't available in vanilla Emerald. Check docs for the location, or explore by yourself!
* Legendary abilities: Buffs some legendaries giving them a better ability than "Pressure".
* Modern Movepool: Adds 15 new moves, and modifies all Pokémon movepool to add them. TM/HMs and egg moves are also modified.
* Type chart: The type effectiveness has been slightly balanced to slightly power up less useful types. 
    - Dark and Ghost do 1x to Steel
    - Water does 0.5x damage to Ice
    - Ice does 1x to Water
    - Bug does 1x to Ghost and Fairy
    - Steel does 1x to Ice
    - Rock does 0,5x to Rock
    - Rock does 1x to Ground
    - Ground does 1x to Rock
      Choosing the "GenVI+" type chart only adds Steel not resisting Ghost and Dark.
* Nature mints: Adds nature mints to the game, available after the 4th gym, or after becoming champion (if disabled from the start).
* Synchronize: Choose if this ability works like in modern games or like in Gen 3.
* Sturdy: Choose if this ability works like in modern games or like in Gen 3.
* Reusable TMs: Choose between a faithful usage of TMs or a simplistic option that makes TMs infinite. All TMs can be bought in the Battlefrontier PokéMart only if you have Reusable TMs off, and makes Move Tutors one time only just like in the original (Move tutors are infinite if you enable Reusable TMs).
* Sitrus berry: Choose if it works like Gen 3 or like Gen 4+.
* Survive Poison: If enabled, your Pokémon will survive poison damage with 1hp when outside of battle.

Features page:
* RTC Type: Choose between using a real clock, or using a fake clock. Fake clock rate is 1h irl, 1 day ingame.
* Shiny Chance: 8192 (Emerald default) - 4096 (Gen VI+) - 2048 - 1024 - 512.
* Shiny Colors: Enables or disables new shiny colored forms for 24 Pokémon.
* Item Drops: Items held by wild Pokémon, when defeated, will be dropped and obtained by the player. Forget about catching it or using Thief!
* Uncapped wondertrade: No 3-daily limit.
* Easier Feebas: If enabled, Feebas have a 5% chance to appear around the whole Route 119.
* Frontier bans: Decide if you want legendaries banned or not in Battle Frontier. If enabled, remember that the bans depend on your chosen difficulty!

Randomizer page:
* Includes every option that any randomizer has, and it's completely modular.
  For technical reasons, some special or particular Pokémon might not be randomized. 

Nuzlocke page:
* Any option that any Nuzlocker would want to use. Includes EASY, NORMAL, and a HARDCORE mode, plus many options.

Difficulty page:
* Lock difficulty: locks the current select option that was selected during Birch's Speech and can't be changed in-game. Hard sets "Battle Style" to "Set" always. Beating the game disables the lock.
* Number of Party Members limit: From 1 to 5
* Level caps: Includes two difficulties, Normal caps and Hard caps.
* Exp. Multipliers: Choose between x1, x1.5, x2 or x0.
* Hard mode Experience: Reverts the exp. nerf that Hard Mode does (Not recommended, Hard Mode exp. gain has been manually tested)
* Less chance of running away: Enabling this feature will be like in SMT games; running away from battles isn't as easy!
* Player items: Control whether you can use items in battles, or not.
* Trainer items: Control whether enemy trainers can use items in battles, or not.
* Player IVs: Sets all IVs from wild Pokémon to 31 if "No" is chosen. If you choose "No (HP)", it uses between 30 and 31 to allow for different Hidden Powers.
* Trainer IVs: Control whether enemy trainers have 31 IVs in every stat, or if they should have the default IV value set by the game. "Scale" uses gym badges to slowly add IVs. If enabled together with "Player IVs", you'll completely remove IVs from the game (everyone will have 31 IVs)!
* Player EVs: Control whether your Pokémon can get EVs, or not. "Off" is Emerald default, "Scale" offers a good challenge, and "Hard" and "Extreme" are what they say they are.
* Trainer EVs: Control whether enemy Pokémon can get EVs, or not. Doesn't affect the Battle Frontier, they still have EVs!
* Escape Rope / Dig: Prevent the usage of Escape Rope and prevent using Dig while in dungeons.

Challenges page:
* Pokécenter challenge: Play without Pokémon Centres.
* PC doesn't heal: Enabled by default in the Pokécenter Challenge.
* Ultra expensive challenge: Don't you always have spare money? Maybe not anymore. Offers x5, x10, and x50.
* Evolution limits.
* One type only challenges: You can catch only one type of Pokémon.
* BST equalizers.
* Mirror Mode.
* Mirror Mode Thief.

**Story related:**
* You can now name your rival!
* Mom gives you a new "Outfit box" so you can switch between the Emerald outfit and the Ruby and Sapphire outfits.
* Gym rematches are easier to trigger. After 10 wild battles or 5 trainer battles won, there is a 50% chance of getting a rematch.
* The Elite Four can be rematched after battling with Steven, and they can be single or double battles (you choose!)
* After completing the Elite Four Rematch, a rematch with Steven will be available. You will get a unique, special prize.
* The Sealed Chambers puzzles have changed slightly. Learn Braille and find out what changed!
* Also, there are 6 Regis. Try to discover where the new 3!
* All the trainer rematches scale up a lot more than in the original game, and their parties have been changed.
* All the trainers in the Battle Frontier have new Pokémon in their teams and have been buffed or modified. Includes all the new Post-Gen 3 Pokémon.
* All the gym leaders, Elite Four, the 2 champions, Wally, Magma / Aqua leaders, and Red and Leaf will appear during the Battle Frontier challenges.


**Pokémon related:**
* Following Pokémon (Optional, with a second option to enable or not Big Followers like Rayquaza).
* Unique surfing animations (Optional).
* 44 new Pokémon species, mostly from Generation 4, and a few Gen 9 (Annihilape, Dudunsparce, Farigiraf).
* 1 new box space, for a total of 450 Pokémon box space.
* Birch's bag can show shiny starters!
* All the buffs from later generations are in _(Optional)_.
* Extra buffs for other Pokémon are in. Includes stats, abilities and/or typings. (Ex. Arbok is now POISON / DARK and Meganium is now GRASS / FAIRY) _(Optional)_.
* Pokémon have new learnsets, which are a mix from Gen 3 and newer generations _(Optional)_.
* All Egg moves and tutor moves have been improved with data from later generations, plus some extra ones.
* New evolution methods.
* Pokémon inherit 5 IV's from their parents, no item is needed.
* Everstone works on male or female Pokémon and guarantees nature.
* Gen. VIII Synchronize _(Optional)_.
* Shuckle can make berry juice just like in Gen. II!! Yay?
* Nature Mints are available to buy in the Flower Shop after the 4th Gym _(Optional)_, or after becoming champion if not enabled from the start.
* Deoxys forms can be changed at Birth Island, using the meteorites.
* All Hoenn and National Dex Pokémon need to be obtained to obtain the Completion Diploma, or it won't count as completed.


**Battle related:**
* Modern Battle Frontier, Battle Tents and Trainer Hill. Your Pokémon will be limited to level 50 when playing in those battle facilities, even if your level is 1 or 100.
* Trainer Hill also gives money depending on your finishing time, up to 1.000.000$ if you finish in 10 minutes or less.
* Some of the Move buffs AND nerfs from later generations are in, with small changes to make them work in a 3rd gen game _(Optional)_.
* HM01 Cut is now Grass type, Night Shade does 50 static damage, Hidden Power is now 60 static damage and shows the type in the summary screen and in battle, and more move buffs and changes.
* 15 new Moves from Gen IV to buff typings that didn't have a certain Physical / Special move. (Ex. Dark Pulse, as Dark type didn't have a Special Dark type move). _(Optional)_.
* Fairy type introduced _(Optional)_.
* 4 New abilities for Sylveon, Regidrago, Regieleki and Arceus.
* Gen. VI EXP. SHARE and Gen III EXP. SHARE in the same game. "EXP. SHARE S" can be obtained at the Slateport Mart after obtaining the "EXP. SHARE" at Devon Corp.
* A new ability tutor, after becoming champion, is available in Lilycove.
* EV Training is available in Lilycove.
* IV Maximizer is available in Lylicove, after beating the game, with the option to set IVs to 30 or 31 to allow different Hidden Potentials. Needs a level 100 Pokémon.
* A nurse NPC is available after beating the game to farm EXP in Lilycove.
* New battle backgrounds, completely optional, in the options menu.
* Faster battle intros. Enable "Fast Intros" option in the options menu.
* Faster-paced battles. Enable "Fast Battles" option in the options menu.
* Win streaks in the Battle Frontier multiply the amount of Battle Points obtained even more.
* 3 beeps when low-health, then it stops.
* Press START while selecting a move to open a new Submenu with information about the selected move.
* Trainer class-based Pokéballs.
* Catching EXP.
* Macho Brace multiplies EV gain * 5.
* Gen. IV Sitrus Berry _(Optional)_.


**UI related:**
* New Pokédex! You can now see important information on the new "Stats" page. It's very, VERY useful, and it's like having the game documentation in-game.
* The Pokédex can now be scrolled faster: if you hold left or right, it will advance like before, but without the need to keep pressing left and right.
* The Pokédex can now be scrolled faster than faster! If you hold left or right AND you hold the R button, it will scroll even faster than explained above!
* Faster trainer transitions ported from Fire Red.
* Choose between holding L+R+A or holding B when entering a wild battle to instantly run, or pressing B when the battle has started to run away faster. _(Optional)_.
* You can now register 2 key items: Pressing (as usual) and holding SELECT!
* Swap Pokémon in the Party Menu by pressing SELECT.
* Colored Stats (red = good, blue = bad).
* Pressing L in the stats section of a Pokémon will bring the EVs, pressing R will bring the IVs, and pressing START will bring the default stats.
* HM moves don't need to be taught anymore. If you have a Pokémon that can use a certain HM, if you have the correct HM in the bag, and if you have the required badge, you will be able to perform an HM move.
* (Nuzlocke only) HMs do not impede advancing in the game. Pokémon that usually don't learn certain HMs will now learn them in order not to halt your progress.
* HM moves can be deleted since they are not that important anymore.
* Some TMs had their price changed, especially if you are not using Infinite TMs.
* The bag now holds up to 90 items, and item capacity has been upgraded to x999.
* When the bag is full, items go to the PC.
* You can change the ball your Pokémon is in using a different ball from the bag.
* Reusable repel prompt.
* The time on the clock can be changed by pressing R, and time events should work. Can be also changed the official way by pressing R+B+LEFT on the main menu.
* Three pages with additional options in the options menu.
* New friendship and shiny indicator in the Summary Page of every Pokémon (ported from Heart and Soul)
* Debug menu can be enabled by everybody, so you can cheat or modify whatever you want. **BE MINDFUL THAT IT CAN BREAK YOUR SAVE IF USED INCORRECTLY!** Refer to the Faq to learn how.


**Gameplay related:**
* RNG is fixed and properly works.
* Wonder Trade is available on the basement floor of every Pokémon Center, available after the 5th badge (unless you are doing a randomizer, which makes it available from the start, or a challenge, which enables WT after beating the game). The number of Wonder-trades is 3 per day, unless using the "Unlimited Wondertrades" option.
* Wonder Trade uses a tier system, so rare Pokémon are rare to obtain as well. 
* 3 difficulty modes (EASY, NORMAL and HARD). Selected at the start of the game, can be changed anytime from the options menu (unless using the "Limit difficulty" option).

    EASY mode: Makes the game quite a lot easier by scaling levels down, and obtaining more EXP (+20%).
    - Trainer Pokémon and Wild Pokémon scale down to 10 levels compared to the original game. More badges, lower levels.
    - There are no restrictions on the Battle Frontier.

    NORMAL mode: Vanilla.
    - No changes, except rematches and small things (also on EASY mode).

    HARD mode: Makes the game a bit more difficult. This mode does not intend to offer a "difficulty" hack-rom, it only tries to be a bit more difficult than vanilla, especially mixed with other options like EV/IV scaling, for example.
    - Experience gain has been lowered by 40% to make up for the level scaling, plus some more. There's an option to remove this slower Exp. gain, although it is not recommended, as it has been hand-crafted and tested.
    - Trainer Pokémon and Wild Pokémon scale up to 10 levels compared to the original game. More badges, more levels.
    - Certain ace Pokémon have had their abilities or items changed to make everything a bit more difficult. This mode does not change anything else in most trainer parties or their strategies.
    - "SET MODE" is automatically selected and can't be disabled if you lock the difficulty.
    - There are more restrictions on the Battle Frontier _(Optional)_.
    - The GEN VI Exp. Share will give less Exp. to the battling Pokémon.
    - Legendaries will have higher stats WHILE battling, to make it more challenging, and some will have their catch rate reduced. They are now true Boss Battles! Good luck!

* Optional PHYSICAL / SPECIAL MOVE split from Gen. IV+. Selected in the options menu, second page.
* Day / Night System. Now, Daytime is from 6AM to 20PM. Night-time is from 20PM to 6AM. For the 2 new evolutions, Morning is from 6AM to 9AM. Also includes cool lighting at night!
* Run everywhere.
* Autorun (in the options menu).
* HM moves text and interaction is way faster.
* Link with Fire Red / Leaf Green available from the start.
* One-time tutors are infinite, but you have to pay now (only if Infinite TMs is on).
* Trainer Hill prizes are the berries that were not available in the GBA games.
* Match and Acro Bike are now one. Change between them by pressing "R".
* Easier fishing has been added to the options menu (FR/LG fishing).
* All tickets are available to buy in the Battle Frontier.
* Faster nurse Joy healing, and now with an even faster version in the options menu (with a confirmation sound).
* A new item, the Big Nugget! It can be sold for a very high price. Available from Clamperl or Rich trainers (rematch only).
* You can check the Soot Sack to know how much ash you have.
* Interacting with berry trees is faster.
* Berry trees that are in rainy routes don't need to be watered, and berry trees don't decay.
* Higher berry yield (6 max, 4 min).
* A new "Growth Mulch" item, which makes berries instantly grow.
* New Self-trader to force trade evolutions (trading with another game still works).
* PokéMarts' items change with every badge.
* AI improvements, although nothing too drastic.
* Amulet coin works always, doesn't matter who has it.
* New NPC in the Battle Frontier Pokécenter that lets you turn off some enabled challenges from your savegame (EvoLimit, Mirror, Mirror Thief, Limit Difficulty, One Type challenge, Party Limit, PokeCenter Challenge, No Items (Player), No Items (Trainer))

**Map related:**
* Navel Rock is now a proper dungeon.
* Unown Chamber is now part of Navel Rock.
* A few new maps to introduce the new Regis and the legendary events.
* New zone in the Safari zone to capture the Hoenn starters _(not available for "Original Encounters")_
* Mirage Island can be forced with a certain Pokémon in the party, apart from its unusual rate, and works even if they are sitting on the PC.

**Sound related:**
* Added all the music and sound effects from Pokémon Diamond, Pearl, Platinum, HeartGold, and SoulSilver.
* You can listen to all this music using the combo "Right DPAD + Select + B" in the Title Screen.
* All legendaries have their music swapped for something different using all the newly included tracks from the games mentioned above.
* Option to disable or enable music.
* Option to disable or enable Surf and Bike music.
* Option to choose between different tracks for Wild Battles, Trainers and Frontier Trainers ("Hoenn", "Kanto 1", "Sinnoh", "Johto", "Kanto 2", "Random").
* Option to choose between "Gen 3", "DPPl", and "HGSS" sound effects.

# 🔧QUICK FAQ FOR THIS REPO

**I can't compile Modern Emerald!!**
* First thing: When compiling, **you have to use the modern compiler** with the "make modern" command. The old compiler will never work.
* Second thing: The game was compiled using `(devkitARM release 62) 13.2.0`, but it should work up to `release 65`. Higher versions WON'T probably work, for many reasons, as of right now.

**My game crashes on an emulator that's not recent / it's not mGBA or real hardware!**
Decompilation hack-roms may crash or have strange bugs if you are using other emulators other than mGBA or a real GBA. This is not something I can fix, as it relies on the base pokeemerald project adding support or fixes to it.

**Help! PkHex / PKSM / Similar tools or apps can't open the savefile!**
These programs rely on knowing where to find the data on the savefile, and Modern Emerald has modified certain parts of the savedata which makes it incompatible with these apps or tools. Use the debug menu to cheat, as it provides the same options, mostly.
