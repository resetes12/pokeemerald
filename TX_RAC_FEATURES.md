# Pokémon Emerald tx_randomizer_and_challenges

This branch combines an ingame randomizer with additional optionial challenges. Included are the following options:

## Randomizer
* Wild Pokémon
  * Randomizes wild encounter based on the species and the route
  * Every species gets swapped with another species
    * Example: a Zigzagoon on route 101 gets exchanged for a different Pokémon than on route 102
* Trainer
  * The Pokémon of Trainer parties get randomized based on the species and the route
  * It uses a different calculation than the wild encounter
    * Example: a wild Zigzagoon gets exchanged for a different Pokémon than a Trainer's Zigzagoon on the same route
* Evolution stages
  * All Pokémon are assigned a category based on their evolution stage: Evo0, Evo1, Evo2, Evo_Self and Evo_Legendary
    * Evo_Self: Some special Pokémon are not randomized
    * Evo_Legendary: Legendary Pokémon are only exchanged for other legendary Pokémon, unless the option below is activated
* Legendaries
  * Now includes legendary Pokémon in the rotation
* Type
  * Pokémon types are random per species
    * Example: all Zigzagoons have the same random types thoughout the game
* Moves
  * Moves are randomized based on the move id and the species
    * Example: all Zigzagoons have the same random move pool, but every Pokémon species gets another move for Tackle
* Abilities
  * Randomizes the abilities based on the ability id and the species
* Evolutions
  * Randomizes the outcome of a evolution based on species
    * Example: a Treecko evolves at level 16 into e.g. a Ditto
* Evolution methods
  * Randomizes the requierements for evolutions, based on species
    * Example: a Ditto could now evolve with a Leaf Stone into a Vileplume
* Type effectiveness
* Items
  * Randomizes items that a recieved from NPCs, found as Pokéballs in the overworld or are hidden in the overworld
    * Key items like the Bike or Fishing Rods are not randomized!
    * TMs only get exchanged for other TMs
* Chaos
  * NOT RECOMMENDED, no support
  * Throws all consistency over board and randomizes everything without pattern
  * Only applies for previously selected options



