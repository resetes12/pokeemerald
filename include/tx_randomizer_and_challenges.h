#ifndef GUARD_DIFFICULTY_H
#define GUARD_DIFFICULTY_H
//tx_randomizer_and_challenges

// default options set by the dev
#define TX_RANDOM_WILD_POKEMON          FALSE
#define TX_RANDOM_TRAINER               FALSE
#define TX_RANDOM_SIMILAR               FALSE
#define TX_RANDOM_MAP_BASED             TRUE
#define TX_RANDOM_INCLUDE_LEGENDARIES   FALSE
#define TX_RANDOM_TYPE                  FALSE
#define TX_RANDOM_MOVES                 FALSE
#define TX_RANDOM_ABILITIES             FALSE
#define TX_RANDOM_EVOLUTION             FALSE
#define TX_RANDOM_EVOLUTION_METHODE     FALSE
#define TX_RANDOM_TYPE_EFFECTIVENESS    FALSE
#define TX_RANDOM_CHAOS_MODE            FALSE
#define TX_RANDOM_ONE_FOR_ONE           FALSE //not yet implemented in menu


#define TX_CHALLENGE_EVO_LIMIT 0 //0 off, 1 first, 2 none
#define TX_CHALLENGE_PARTY_LIMIT 0
#define TX_CHALLENGE_NUZLOCKE 0
#define TX_CHALLENGE_NUZLOCKE_HARDCORE 0 //CAREFULL!!!!!
#define TX_CHALLENGE_NO_ITEM_PLAYER 0
#define TX_CHALLENGE_NO_ITEM_TRAINER 0
#define TX_CHALLENGE_PKMN_CENTER 0 //0 no limit, 1 none
#define TX_CHALLENGE_BASE_STAT_EQUALIZER 0 //0=off, 1=100, 2=255, 3=500

#define TX_CHALLENGE_TYPE_OFF 31
#define TX_CHALLENGE_TYPE TX_CHALLENGE_TYPE_OFF //TX_CHALLENGE_TYPE_OFF for off

// randomization types
#define TX_RANDOM_T_WILD_POKEMON    0
#define TX_RANDOM_T_TRAINER         1
#define TX_RANDOM_T_MOVES           2
#define TX_RANDOM_T_ABILITY         3
#define TX_RANDOM_T_EVO             4
#define TX_RANDOM_T_EVO_METH        5

void CB2_InitRandomizerMenu(void);
void CB2_InitChallengesMenu(void);
void tx_randomizer_SaveData(void);
void tx_challenges_SaveData(void);

bool8 IsRandomizerActivated(void);
bool8 IsChallengesActivated(void);
bool8 IsNuzlockeActivated(void);
bool8 IsPokecenterChallengeActivated(void);

// constants
#define TX_MENU_ITEMS_PER_PAGE 6

extern struct tx_randomizer_OptionsMenu *sRandomizerOptions;
extern struct tx_challenges_OptionsMenu *sChallengesOptions;

#endif // GUARD_DIFFICULTY_H