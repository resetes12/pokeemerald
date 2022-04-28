# Pokémon Emerald tx_randomizer_and_challenges

This branch combines an ingame randomizer with additional optionial challenges. Included are the following options:

## Randomizer
* **Wild Pokémon**
  * Randomizes wild encounter based on the species and the route
  * Every species gets swapped with another species
    * Example: a Zigzagoon on route 101 gets exchanged for a different Pokémon than on route 102
* **Trainer**
  * The Pokémon of Trainer parties get randomized based on the species and the route
  * It uses a different calculation than the wild encounter
    * Example: a wild Zigzagoon gets exchanged for a different Pokémon than a Trainer's Zigzagoon on the same route
* **Evolution stages**
  * All Pokémon are assigned a category based on their evolution stage: Evo0, Evo1, Evo2, Evo_Self and Evo_Legendary
    * Evo_Self: Some special Pokémon are not randomized
    * Evo_Legendary: Legendary Pokémon are only exchanged for other legendary Pokémon, unless the option below is activated
* **Legendaries**
  * Now includes legendary Pokémon in the rotation
* **Type**
  * Pokémon types are random per species
    * Example: all Zigzagoons have the same random types thoughout the game
* **Moves**
  * Moves are randomized based on the move id and the species
    * Example: all Zigzagoons have the same random move pool, but every Pokémon species gets another move for Tackle
* **Abilities**
  * Randomizes the abilities based on the ability id and the species
* **Evolutions**
  * Randomizes the outcome of a evolution based on species
    * Example: a Treecko evolves at level 16 into e.g. a Ditto
* **Evolution methods**
  * Randomizes the requierements for evolutions, based on species
    * Example: a Ditto could now evolve with a Leaf Stone into a Vileplume
* **Type effectiveness**
* **Items**
  * Randomizes items that a recieved from NPCs, found as Pokéballs in the overworld or are hidden in the overworld
    * Key items like the Bike or Fishing Rods are not randomized!
    * TMs only get exchanged for other TMs
* **Chaos**
  * NOT RECOMMENDED, no support
  * Throws all consistency over board and randomizes everything without pattern
  * Only applies for previously selected options


## Challenges
* **Evolution limit**
  * Limits the evolutions of Pokémon
    * First: Pokémon can only evolve into their first evolution in the evolution line
      * Example: Treecko can only evolve into Grovyle, but Grovyle will not evolve any further
    * All: Pokémon will not evolve at all!
* **Party limit**
  * Limits the amount of Pokémon the player can have in their party to 6/5/4/3/2/1
* **Nuzlocke mode**
  * Enables Nuzlocke mode with the following rules:
    * Only the first encounter per route can be caught, shown with a small 1 icon where the caught Pokéball otherwise is
    * If a Pokémon faints in battle it gets automatically released
      * Held items are returned to the player upon release
      * **WHITEOUT**
        * Normal: The first Pokémon in your storage gets added to your party. If your storage is empty, the game soft resets to the last save!
        * Hard: Savegame gets **DELETED**
    * Every Pokémon has to be nicknamed, including starter, wild catches, gifted Pokémon, from eggs hatched Pokémon
    * Species clause is active, meaining you can not catch a Pokémon in the same evolution line twice
      * Example: you can NOT catch a Zigzagoon on route 101 and a Zigzagoon on route 102, or any Linoone later
      * But a "first" encounter with an already caught species does NOT use up the current route first encounter!!!
    * Shiny clause active, meaning ALL shiny Pokémon can be caught regardless of any other rules!
* **Level cap**
  * Implements a level cap based on the gym badges collected
  * Pokémon will not recieve any EXP. Points once they reach the cap
    * Normal: Level cap is the level of the next gym's highest Pokémon
    * Hard: Level cap is the level of the next gym's **lowest** Pokémon
* **Experience multiplier**
  * Multiplies the EXP. Points gained by the factor choosen
  * Options are 1.0 (normal), 1.5, 2.0 and 0.0
    * On the 0.0 option Pokémon do NOT gain ANY experience
      * It probably needs an alternative system for leveling, like buyable rare candy to be viably used
* **Player items**
  * Disables the players ability to use items in battle like Potions or Burn Heals
  * Held items on Pokémon are still allowed!
* **Trainer items**
  * Enemy trainer can NOT use any items like Hyper Potions
* **Pokécenter usage**
  * Disables all free healing spots in the game like Pokécenter, Mom or any beds
* **One Type challenge**
  * Limits the Pokémon Type the player can use to ONE
  * Pokémon that do not have the specified type in slot1 or slot2 can NOT be caught
  * Starter Pokémon are randomized and all 3 options are of the choosen type
* **Base stat equalizer**
  * All Pokémon have the same base stat value choosen for each stat (Attack, Defense, Special Attack, Special Defense, Speed)
  * Options are: 100, 255 or 500
    * Stats still get calculated based on the Pokémon's level, IVs and EVs

