//CREDITS
//TheXaman:             https://github.com/TheXaman/pokeemerald/tree/tx_debug_system
//CODE USED FROM:
//ketsuban:             https://github.com/pret/pokeemerald/wiki/Add-a-debug-menu
//Pyredrid:             https://github.com/Pyredrid/pokeemerald/tree/debugmenu
//AsparagusEduardo:     https://github.com/AsparagusEduardo/pokeemerald/tree/InfusedEmerald_v2
//Ghoulslash:           https://github.com/ghoulslash/pokeemerald
//Jaizu:                https://jaizu.moe/
#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "coins.h"
#include "credits.h"
#include "data.h"
#include "daycare.h"
#include "debug.h"
#include "debug_pokemon_creator.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_message_box.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "international_string_util.h"
#include "item.h"
#include "item_icon.h"
#include "list_menu.h"
#include "m4a.h"
#include "main.h"
#include "main_menu.h"
#include "malloc.h"
#include "map_name_popup.h"
#include "menu.h"
#include "money.h"
#include "naming_screen.h"
#include "new_game.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "region_map.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "pokemon_summary_screen.h"
#include "wild_encounter.h"
#include "constants/abilities.h"
#include "constants/battle_frontier.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/map_groups.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/weather.h"
#include "save.h"


#if TX_DEBUG_SYSTEM_ENABLE == TRUE
// *******************************
// Enums
enum { // Main
    DEBUG_MENU_ITEM_UTILITIES,
    DEBUG_MENU_ITEM_SCRIPTS,
    DEBUG_MENU_ITEM_FLAGVAR,
    DEBUG_MENU_ITEM_BATTLE,
    DEBUG_MENU_ITEM_GIVE,
    DEBUG_MENU_ITEM_POKEMON_CREATOR,
    DEBUG_MENU_ITEM_FILL,
    DEBUG_MENU_ITEM_SOUND,
    DEBUG_MENU_ITEM_ACCESS_PC,
    DEBUG_MENU_ITEM_CANCEL
};
enum { // Util
    DEBUG_UTIL_MENU_ITEM_HEAL_PARTY,
    DEBUG_UTIL_MENU_ITEM_FLY,
    DEBUG_UTIL_MENU_ITEM_WARP,
    DEBUG_UTIL_MENU_ITEM_POISON_MONS,
    DEBUG_UTIL_MENU_ITEM_SAVEBLOCK,
    DEBUG_UTIL_MENU_ITEM_WEATHER,
    DEBUG_UTIL_MENU_ITEM_CHECKWALLCLOCK,
    DEBUG_UTIL_MENU_ITEM_SETWALLCLOCK,
    DEBUG_UTIL_MENU_ITEM_WATCHCREDITS,
    DEBUG_UTIL_MENU_ITEM_TRAINER_NAME,
    DEBUG_UTIL_MENU_ITEM_TRAINER_GENDER,
    DEBUG_UTIL_MENU_ITEM_TRAINER_ID,
    DEBUG_UTIL_MENU_ITEM_CHEAT,
};
enum { // Scripts
    DEBUG_UTIL_MENU_ITEM_SCRIPT_1,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_2,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_3,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_4,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_5,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_6,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_7,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_8,
};
enum { // Flags and Vars
    DEBUG_FLAGVAR_MENU_ITEM_FLAGS,
    DEBUG_FLAGVAR_MENU_ITEM_VARS,
    DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_ALL,
    DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_RESET,
    DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_SEEN,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKEDEX,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_NATDEX,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKENAV,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_RUN_SHOES,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_LOCATIONS,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BADGES_ALL,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_FRONTIER_PASS,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BATTLE_DMG,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_COLISSION,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_ENCOUNTER,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_TRAINER_SEE,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BAG_USE,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_CATCHING,
};
enum { // Battle 0 Type
    DEBUG_BATTLE_0_MENU_ITEM_WILD,
#ifdef BATTLE_ENGINE
    DEBUG_BATTLE_0_MENU_ITEM_WILD_DOUBLE,
#endif
    DEBUG_BATTLE_0_MENU_ITEM_SINGLE,
    DEBUG_BATTLE_0_MENU_ITEM_DOUBLE,
    DEBUG_BATTLE_0_MENU_ITEM_MULTI,
};
enum { // Battle 1 AI FLags
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_00,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_01,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_02,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_03,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_04,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_05,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_06,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_07,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_08,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_09,
#ifdef BATTLE_ENGINE
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_10,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_11,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_12,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_13,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_14,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_15,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_16,
#endif
    DEBUG_BATTLE_1_MENU_ITEM_CONTINUE,
};
enum { // Battle 2 Terrain
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_0,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_1,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_2,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_3,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_4,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_5,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_6,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_7,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_8,   
    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_9,
};   
enum { // Give
    DEBUG_GIVE_MENU_ITEM_ITEM_X,
    DEBUG_GIVE_MENU_ITEM_ALLTMS,
    DEBUG_GIVE_MENU_ITEM_POKEMON_SIMPLE,
    DEBUG_GIVE_MENU_ITEM_POKEMON_COMPLEX,
    DEBUG_GIVE_MENU_ITEM_MAX_MONEY,
    DEBUG_GIVE_MENU_ITEM_MAX_COINS,
    DEBUG_GIVE_MENU_ITEM_MAX_BATTLE_POINTS,
    DEBUG_GIVE_MENU_ITEM_DAYCARE_EGG,
    DEBUG_GIVE_MENU_ITEM_HATCH_AN_EGG,
};
enum {
    DEBUG_PKM_CREATOR_MENU_ITEM_PARTY_ADD,               // Add to party
    DEBUG_PKM_CREATOR_MENU_ITEM_PARTY_EDIT,              // Edit party
    DEBUG_PKM_CREATOR_MENU_ITEM_PC_EDIT,                 // Edit PC Box
    DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_EDIT,        // Edit enemy party (usable as a sandbox)
    DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_EDIT_DEBUG,  // Edit enemy party for debug battle
    DEBUG_PKM_CREATOR_MENU_ITEM_DEBUG_EDIT,              // Edit party for debug battle
    DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_ADD,         // Add to enemy party (Unused)
    DEBUG_PKM_CREATOR_MENU_ITEM_TESTING,                 // Testing mode (Dont alter parties)
    DEBUG_PKM_CREATOR_MENU_ITEM_TESTING_COPY,            // Testing mode (use first mon as template)
};
enum { // Give Fill
    DEBUG_FILL_MENU_ITEM_PC_BOXES_FAST,
    DEBUG_FILL_MENU_ITEM_PC_BOXES_SLOW,
    DEBUG_FILL_MENU_ITEM_PC_ITEMS,
    DEBUG_FILL_MENU_ITEM_POCKET_ITEMS,
    DEBUG_FILL_MENU_ITEM_POCKET_BALLS,
    DEBUG_FILL_MENU_ITEM_POCKET_TMHM,
    DEBUG_FILL_MENU_ITEM_POCKET_BERRIES,
    DEBUG_FILL_MENU_ITEM_POCKET_KEY_ITEMS,
};
enum { //Sound
    DEBUG_SOUND_MENU_ITEM_SE,
    DEBUG_SOUND_MENU_ITEM_MUS,
};


// *******************************
// Constants
#define DEBUG_MENU_WIDTH_MAIN 16
#define DEBUG_MENU_HEIGHT_MAIN 9

#define DEBUG_MENU_WIDTH_EXTRA 10
#define DEBUG_MENU_HEIGHT_EXTRA 4

#define DEBUG_MENU_WIDTH_WEATHER 15
#define DEBUG_MENU_HEIGHT_WEATHER 3

#define DEBUG_MENU_WIDTH_SOUND 20
#define DEBUG_MENU_HEIGHT_SOUND 6

#define DEBUG_MENU_WIDTH_FLAGVAR 4
#define DEBUG_MENU_HEIGHT_FLAGVAR 2

#define DEBUG_NUMBER_DIGITS_FLAGS 4
#define DEBUG_NUMBER_DIGITS_VARIABLES 5
#define DEBUG_NUMBER_DIGITS_VARIABLE_VALUE 5
#define DEBUG_NUMBER_DIGITS_ITEMS 4
#define DEBUG_NUMBER_DIGITS_ITEM_QUANTITY 2

#define DEBUG_NUMBER_ICON_X 210
#define DEBUG_NUMBER_ICON_Y 50

// *******************************
struct DebugMonData
{
    u16 mon_speciesId;
    u8  mon_level;
    u8  isShiny;
    u16 mon_natureId;
    u16 mon_abilityNum;
    u8  mon_iv_hp;
    u8  mon_iv_atk;
    u8  mon_iv_def;
    u8  mon_iv_speed;
    u8  mon_iv_satk;
    u8  mon_iv_sdef;
    u16 mon_move_0;
    u16 mon_move_1;
    u16 mon_move_2;
    u16 mon_move_3;
};

struct DebugMenuListData
{
    struct ListMenuItem listItems[20 + 1];
    u8 itemNames[PC_ITEMS_COUNT + 1][26];
    u8 listId;
};

struct DebugBattleData
{
    u8 submenu;
    u8 battleType;
    u8 battleTerrain;
#ifdef BATTLE_ENGINE
    bool8 aiFlags[17];
#else
    bool8 aiFlags[10];
#endif
};

// EWRAM
static EWRAM_DATA struct DebugMonData *sDebugMonData = NULL;
static EWRAM_DATA struct DebugMenuListData *sDebugMenuListData = NULL;
static EWRAM_DATA struct DebugBattleData *sDebugBattleData = NULL;
EWRAM_DATA bool8 gIsDebugBattle = FALSE;
EWRAM_DATA u32 gDebugAIFlags = 0;

// *******************************
// Define functions
static void Debug_ReShowMainMenu(void);
static u8 Debug_ShowMenu(void (*HandleInput)(u8), struct ListMenuTemplate LMtemplate);
static void Debug_DestroyMenu(u8 taskId);
static void Debug_DestroyMenu_Full(u8 taskId);
static void DebugAction_Cancel(u8 taskId);
static void DebugAction_DestroyExtraWindow(u8 taskId);
static void DebugTask_HandleMenuInput(u8 taskId, void (*HandleInput)(u8));
static void Debug_InitDebugBattleData(void);
static void Debug_RefreshListMenu(u8 taskId);
static void Debug_RedrawListMenu(u8 taskId);

static void DebugAction_Util_Script_1(u8 taskId);
static void DebugAction_Util_Script_2(u8 taskId);
static void DebugAction_Util_Script_3(u8 taskId);
static void DebugAction_Util_Script_4(u8 taskId);
static void DebugAction_Util_Script_5(u8 taskId);
static void DebugAction_Util_Script_6(u8 taskId);
static void DebugAction_Util_Script_7(u8 taskId);
static void DebugAction_Util_Script_8(u8 taskId);

static void DebugAction_OpenUtilitiesMenu(u8 taskId);
static void DebugAction_OpenScriptsMenu(u8 taskId);
static void DebugAction_OpenFlagsVarsMenu(u8 taskId);
static void DebugAction_OpenBattleMenu(u8 taskId);
static void DebugAction_OpenGiveMenu(u8 taskId);
static void DebugAction_OpenPokemonCreator(u8 taskId);
static void DebugAction_OpenFillMenu(u8 taskId);
static void DebugAction_OpenSoundMenu(u8 taskId);
static void DebugAction_AccessPC(u8 taskId);
static void DebugTask_HandleMenuInput_Main(u8 taskId);
static void DebugTask_HandleMenuInput_Utilities(u8 taskId);
static void DebugTask_HandleMenuInput_Scripts(u8 taskId);
static void DebugTask_HandleMenuInput_FlagsVars(u8 taskId);
static void DebugTask_HandleMenuInput_Battle(u8 taskId);
static void DebugTask_HandleMenuInput_Give(u8 taskId);
static void DebugTask_HandleMenuInput_PkmCreator(u8 taskId);
static void DebugTask_HandleMenuInput_Fill(u8 taskId);
static void DebugTask_HandleMenuInput_Sound(u8 taskId);

static void DebugAction_Util_HealParty(u8 taskId);
static void DebugAction_Util_Fly(u8 taskId);
static void DebugAction_Util_Warp_Warp(u8 taskId);
static void DebugAction_Util_Warp_SelectMapGroup(u8 taskId);
static void DebugAction_Util_Warp_SelectMap(u8 taskId);
static void DebugAction_Util_Warp_SelectWarp(u8 taskId);
static void DebugAction_FlagsVars_RunningShoes(u8 taskId);
static void DebugAction_Util_PoisonMons(u8 taskId);
static void DebugAction_Util_CheckSaveBlock(u8 taskId);
static void DebugAction_Util_Weather(u8 taskId);
static void DebugAction_Util_Weather_SelectId(u8 taskId);
static void DebugAction_Util_CheckWallClock(u8 taskId);
static void DebugAction_Util_SetWallClock(u8 taskId);
static void DebugAction_Util_WatchCredits(u8 taskId);
static void DebugAction_Util_Trainer_Name(u8 taskId);
static void DebugAction_Util_Trainer_Gender(u8 taskId);
static void DebugAction_Util_Trainer_Id(u8 taskId);
static void DebugAction_Util_CheatStart(u8 taskId);

static void DebugAction_FlagsVars_Flags(u8 taskId);
static void DebugAction_FlagsVars_FlagsSelect(u8 taskId);
static void DebugAction_FlagsVars_Vars(u8 taskId);
static void DebugAction_FlagsVars_Select(u8 taskId);
static void DebugAction_FlagsVars_SetValue(u8 taskId);
static void DebugAction_FlagsVars_PokedexFlags_All(u8 taskId);
static void DebugAction_FlagsVars_PokedexFlags_Reset(u8 taskId);
static void DebugAction_FlagsVars_PokedexFlags_Seen(u8 taskId);
static void DebugAction_FlagsVars_SwitchDex(u8 taskId);
static void DebugAction_FlagsVars_SwitchNatDex(u8 taskId);
static void DebugAction_FlagsVars_SwitchPokeNav(u8 taskId);
static void DebugAction_FlagsVars_ToggleFlyFlags(u8 taskId);
static void DebugAction_FlagsVars_ToggleBadgeFlags(u8 taskId);
static void DebugAction_FlagsVars_ToggleFrontierPass(u8 taskId);
static void DebugAction_FlagsVars_BattleDmgOnOff(u8 taskId);
static void DebugAction_FlagsVars_CollisionOnOff(u8 taskId);
static void DebugAction_FlagsVars_EncounterOnOff(u8 taskId);
static void DebugAction_FlagsVars_TrainerSeeOnOff(u8 taskId);
static void DebugAction_FlagsVars_BagUseOnOff(u8 taskId);
static void DebugAction_FlagsVars_CatchingOnOff(u8 taskId);

static void Debug_InitializeBattle(u8 taskId);

static void DebugAction_Give_Item(u8 taskId);
static void DebugAction_Give_Item_SelectId(u8 taskId);
static void DebugAction_Give_Item_SelectQuantity(u8 taskId);
static void DebugAction_Give_AllTMs(u8 taskId);
static void DebugAction_Give_PokemonSimple(u8 taskId);
static void DebugAction_Give_PokemonComplex(u8 taskId);
static void DebugAction_Give_Pokemon_SelectId(u8 taskId);
static void DebugAction_Give_Pokemon_SelectLevel(u8 taskId);
static void DebugAction_Give_Pokemon_SelectShiny(u8 taskId);
static void DebugAction_Give_Pokemon_SelectNature(u8 taskId);
static void DebugAction_Give_Pokemon_SelectAbility(u8 taskId);
static void DebugAction_Give_Pokemon_SelectIVs(u8 taskId);
static void DebugAction_Give_Pokemon_ComplexCreateMon(u8 taskId);
static void DebugAction_Give_Pokemon_Move(u8 taskId);
static void DebugAction_Give_MaxMoney(u8 taskId);
static void DebugAction_Give_MaxCoins(u8 taskId);
static void DebugAction_Give_MaxBattlePoints(u8 taskId);
static void DebugAction_Give_DayCareEgg(u8 taskId);

static void DebugAction_PkmCreator_Party_Add(u8 taskid);
static void DebugAction_PkmCreator_Party_Edit(u8 taskid);
static void DebugAction_PkmCreator_PC_Edit(u8 taskid);
static void DebugAction_PkmCreator_Enemy_Party_Edit(u8 taskId);
static void DebugAction_PkmCreator_Enemy_Party_Edit_Debug(u8 taskId);
static void DebugAction_PkmCreator_Debug_Edit(u8 taskid);
static void DebugAction_PkmCreator_Enemy_Party_Add(u8 taskid);
static void DebugAction_PkmCreator_Testing(u8 taskid);
static void DebugAction_PkmCreator_Testing_Copy(u8 taskid); 

static void DebugAction_Fill_PCBoxes_Fast(u8 taskId);
static void DebugAction_Fill_PCBoxes_Slow(u8 taskId);
static void DebugAction_Fill_PCItemStorage(u8 taskId);
static void DebugAction_Fill_PocketItems(u8 taskId);
static void DebugAction_Fill_PocketPokeBalls(u8 taskId);
static void DebugAction_Fill_PocketTMHM(u8 taskId);
static void DebugAction_Fill_PocketBerries(u8 taskId);
static void DebugAction_Fill_PocketKeyItems(u8 taskId);

static void DebugAction_Sound_SE(u8 taskId);
static void DebugAction_Sound_SE_SelectId(u8 taskId);
static void DebugAction_Sound_MUS(u8 taskId);
static void DebugAction_Sound_MUS_SelectId(u8 taskId);

static void DebugAction_HatchAnEgg(u8 taskId);

extern u8 Debug_Script_1[];
extern u8 Debug_Script_2[];
extern u8 Debug_Script_3[];
extern u8 Debug_Script_4[];
extern u8 Debug_Script_5[];
extern u8 Debug_Script_6[];
extern u8 Debug_Script_7[];
extern u8 Debug_Script_8[];
extern const u8 Debug_CheckSaveBlock[];
extern const u8 Debug_HatchAnEgg[];

extern u8 Debug_ShowFieldMessageStringVar4[];
extern u8 Debug_CheatStart[];
extern u8 PlayersHouse_2F_EventScript_SetWallClock[];
extern u8 PlayersHouse_2F_EventScript_CheckWallClock[];
#ifdef BATTLE_ENGINE
#define ABILITY_NAME_LENGTH 16
#else
#define ABILITY_NAME_LENGTH 12
#endif
extern const u8 gAbilityNames[][ABILITY_NAME_LENGTH + 1];


// *******************************
//Maps per map group COPY FROM /include/constants/map_groups.h
static const u8 MAP_GROUP_COUNT[] = {57, 5, 5, 6, 7, 8, 9, 7, 7, 14, 8, 17, 10, 23, 13, 15, 15, 2, 2, 2, 3, 1, 1, 1, 126, 61, 93, 2, 1, 13, 1, 1, 3, 1, 0};

// Text
// General
static const u8 sDebugText_True[] =          _("TRUE");
static const u8 sDebugText_False[] =         _("FALSE");
static const u8 sDebugText_Colored_True[] =  _("{COLOR GREEN}TRUE");
static const u8 sDebugText_Colored_False[] = _("{COLOR RED}FALSE");
static const u8 sDebugText_Dashes[] =        _("---");
static const u8 sDebugText_Empty[] =         _("");
static const u8 sDebugText_Continue[] =      _("Continue…{CLEAR_TO 110}{RIGHT_ARROW}");
// Main Menu
static const u8 sDebugText_Utilities[] =        _("Utilities…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Scripts[] =          _("Scripts…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_FlagsVars[] =        _("Flags & Vars…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle[] =           _("Battle Test…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Give[] =             _("Give X…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_PkmCreator[] =       _("Pokémon Creator…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Fill[] =             _("Fill PC/Pockets…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Sound[] =            _("Sound…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_AccessPC[] =         _("Access PC…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Cancel[] =           _("Cancel");
// Script menu
static const u8 sDebugText_Util_Script_1[] =               _("Open Challenges Menu");
static const u8 sDebugText_Util_Script_2[] =               _("Grow Berry Tree");
static const u8 sDebugText_Util_Script_3[] =               _("Update 2.4 -> 3.0+");
static const u8 sDebugText_Util_Script_4[] =               _("Nuzlocke Diff. info");
static const u8 sDebugText_Util_Script_5[] =               _("Script 5 (Empty)");
static const u8 sDebugText_Util_Script_6[] =               _("Script 6 (Empty)");
static const u8 sDebugText_Util_Script_7[] =               _("Script 7 (Empty)");
static const u8 sDebugText_Util_Script_8[] =               _("Script 8 (Empty)");
// Util Menu
static const u8 sDebugText_Util_HealParty[] =               _("Heal Party");
static const u8 sDebugText_Util_Fly[] =                     _("Fly to map…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Util_WarpToMap[] =               _("Warp to map warp…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Util_WarpToMap_SelectMapGroup[] =_("Group: {STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n\n{STR_VAR_3}{CLEAR_TO 90}");
static const u8 sDebugText_Util_WarpToMap_SelectMap[] =     _("Map: {STR_VAR_1}{CLEAR_TO 90}\nMapSec:{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}\n{STR_VAR_3}{CLEAR_TO 90}");
static const u8 sDebugText_Util_WarpToMap_SelectWarp[] =    _("Warp:{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_3}{CLEAR_TO 90}");
static const u8 sDebugText_Util_WarpToMap_SelMax[] =        _("{STR_VAR_1} / {STR_VAR_2}");
static const u8 sDebugText_Util_PoisonMons[] =              _("Poison all mons");
static const u8 sDebugText_Util_SaveBlockSpace[] =          _("SaveBlock Space…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Util_Weather[] =                 _("Set weather…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Util_Weather_ID[] =              _("Weather Id: {STR_VAR_3}\n{STR_VAR_1}\n{STR_VAR_2}");
static const u8 sDebugText_Util_CheckWallClock[] =          _("Check Wall Clock…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Util_SetWallClock[] =            _("Set Wall Clock…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Util_WatchCredits[] =            _("Watch Credits…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Util_Trainer_Name[] =            _("Trainer name");
static const u8 sDebugText_Util_Trainer_Gender[] =          _("Toggle T. Gender");
static const u8 sDebugText_Util_Trainer_Id[] =              _("New Trainer Id");
static const u8 sDebugText_Util_CheatStart[] =              _("CHEAT Start");
// Flags/Vars Menu
static const u8 sDebugText_FlagsVars_Flags[] =                  _("Set Flag XYZ…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_FlagsVars_Flag[] =                   _("Flag: {STR_VAR_1}{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}\n{STR_VAR_3}");
static const u8 sDebugText_FlagsVars_FlagHex[] =                _("{STR_VAR_1}{CLEAR_TO 90}\n0x{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_FlagsVars_Vars[] =                   _("Set Var XYZ…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_FlagsVars_VariableHex[] =            _("{STR_VAR_1}{CLEAR_TO 90}\n0x{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_FlagsVars_Variable[] =               _("Var: {STR_VAR_1}{CLEAR_TO 90}\nVal: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_2}");
static const u8 sDebugText_FlagsVars_VariableValueSet[] =       _("Var: {STR_VAR_1}{CLEAR_TO 90}\nVal: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_2}");
static const u8 sDebugText_FlagsVars_PokedexFlags_All[] =       _("Pokédex Flags All");
static const u8 sDebugText_FlagsVars_PokedexFlags_Reset[] =     _("Pokédex Flags Reset");
static const u8 sDebugText_FlagsVars_PokedexFlags_Seen[] =       _("Pokédex Flags Seen");
static const u8 sDebugText_FlagsVars_SwitchDex[] =              _("Toggle {STR_VAR_1}Pokédex");
static const u8 sDebugText_FlagsVars_SwitchNationalDex[] =      _("Toggle {STR_VAR_1}NatDex");
static const u8 sDebugText_FlagsVars_SwitchPokeNav[] =          _("Toggle {STR_VAR_1}PokéNav");
static const u8 sDebugText_FlagsVars_RunningShoes[] =           _("Toggle {STR_VAR_1}Running Shoes");
static const u8 sDebugText_FlagsVars_ToggleFlyFlags[] =         _("Toggle {STR_VAR_1}Fly Flags");
static const u8 sDebugText_FlagsVars_ToggleAllBadges[] =        _("Toggle {STR_VAR_1}All badges");
static const u8 sDebugText_FlagsVars_ToggleFrontierPass[] =     _("Toggle {STR_VAR_1}Frontier Pass");
static const u8 sDebugText_FlagsVars_BattleDmg[] =              _("Toggle {STR_VAR_1}BattleDmg OFF");
static const u8 sDebugText_FlagsVars_SwitchCollision[] =        _("Toggle {STR_VAR_1}Collision OFF");
static const u8 sDebugText_FlagsVars_SwitchEncounter[] =        _("Toggle {STR_VAR_1}Encounter OFF");
static const u8 sDebugText_FlagsVars_SwitchTrainerSee[] =       _("Toggle {STR_VAR_1}TrainerSee OFF");
static const u8 sDebugText_FlagsVars_SwitchBagUse[] =           _("Toggle {STR_VAR_1}BagUse OFF");
static const u8 sDebugText_FlagsVars_SwitchCatching[] =         _("Toggle {STR_VAR_1}Catching OFF");
// Battle
static const u8 sDebugText_Battle_0_Wild[] =        _("Wild…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_0_WildDouble[] =  _("Wild Double…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_0_Single[] =      _("Single…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_0_Double[] =      _("Double…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_0_Mulit[] =       _("Multi…{CLEAR_TO 110}{RIGHT_ARROW}");
#ifndef BATTLE_ENGINE
static const u8 sDebugText_Battle_1_AIFlag_00[] =   _("{STR_VAR_1}Check bad move");
static const u8 sDebugText_Battle_1_AIFlag_01[] =   _("{STR_VAR_1}Try to faint");
static const u8 sDebugText_Battle_1_AIFlag_02[] =   _("{STR_VAR_1}Check viability");
static const u8 sDebugText_Battle_1_AIFlag_03[] =   _("{STR_VAR_1}Setup first turn");
static const u8 sDebugText_Battle_1_AIFlag_04[] =   _("{STR_VAR_1}Risky");
static const u8 sDebugText_Battle_1_AIFlag_05[] =   _("{STR_VAR_1}Prefer power extremes");
static const u8 sDebugText_Battle_1_AIFlag_06[] =   _("{STR_VAR_1}Prefer baton pass");
static const u8 sDebugText_Battle_1_AIFlag_07[] =   _("{STR_VAR_1}Double battle");
static const u8 sDebugText_Battle_1_AIFlag_08[] =   _("{STR_VAR_1}Hp aware");
static const u8 sDebugText_Battle_1_AIFlag_09[] =   _("{STR_VAR_1}Try sunny day start");
#else
static const u8 sDebugText_Battle_1_AIFlag_00[] =   _("{STR_VAR_1}Check bad move");
static const u8 sDebugText_Battle_1_AIFlag_01[] =   _("{STR_VAR_1}Try to faint");
static const u8 sDebugText_Battle_1_AIFlag_02[] =   _("{STR_VAR_1}Check viability");
static const u8 sDebugText_Battle_1_AIFlag_03[] =   _("{STR_VAR_1}Setup first turn");
static const u8 sDebugText_Battle_1_AIFlag_04[] =   _("{STR_VAR_1}Risky");
static const u8 sDebugText_Battle_1_AIFlag_05[] =   _("{STR_VAR_1}Prefer strongest move");
static const u8 sDebugText_Battle_1_AIFlag_06[] =   _("{STR_VAR_1}Prefer baton pass");
static const u8 sDebugText_Battle_1_AIFlag_07[] =   _("{STR_VAR_1}Double battle");
static const u8 sDebugText_Battle_1_AIFlag_08[] =   _("{STR_VAR_1}Hp aware");
static const u8 sDebugText_Battle_1_AIFlag_09[] =   _("{STR_VAR_1}Negate unaware");
static const u8 sDebugText_Battle_1_AIFlag_10[] =   _("{STR_VAR_1}Will suicide");
static const u8 sDebugText_Battle_1_AIFlag_11[] =   _("{STR_VAR_1}Help partner");
static const u8 sDebugText_Battle_1_AIFlag_12[] =   _("{STR_VAR_1}Prefer status moves");
static const u8 sDebugText_Battle_1_AIFlag_13[] =   _("{STR_VAR_1}Stall");
static const u8 sDebugText_Battle_1_AIFlag_14[] =   _("{STR_VAR_1}Screener");
static const u8 sDebugText_Battle_1_AIFlag_15[] =   _("{STR_VAR_1}Smart switching");
static const u8 sDebugText_Battle_1_AIFlag_16[] =   _("{STR_VAR_1}Ace pokemon");
#endif
static const u8 sDebugText_Battle_2_Terrain_0[] =   _("Grass…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_1[] =   _("Long grass…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_2[] =   _("Sand…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_3[] =   _("Underwater…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_4[] =   _("Water…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_5[] =   _("Pond…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_6[] =   _("Mountain…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_7[] =   _("Cave…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_8[] =   _("Building…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Battle_2_Terrain_9[] =   _("Plain…{CLEAR_TO 110}{RIGHT_ARROW}");
// Give Menu
static const u8 sDebugText_Give_GiveItem[] =            _("Give item XYZ…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_ItemQuantity[] =             _("Quantity:{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n\n{STR_VAR_2}");
static const u8 sDebugText_ItemID[] =                   _("Item Id: {STR_VAR_3}\n{STR_VAR_1}{CLEAR_TO 90}\n\n{STR_VAR_2}");
static const u8 sDebugText_Give_AllTMs[] =              _("Give all TMs");
static const u8 sDebugText_Give_GivePokemonSimple[] =   _("Pkm (lvl)…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Give_GivePokemonComplex[] =  _("Pkm (l,s,n,a,IV,mov)…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_PokemonID[] =                _("Species: {STR_VAR_3}\n{STR_VAR_1}{CLEAR_TO 90}\n\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonLevel[] =             _("Level:{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonShiny[] =             _("Shiny:{CLEAR_TO 90}\n   {STR_VAR_2}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{CLEAR_TO 90}");
static const u8 sDebugText_PokemonNature[] =            _("NatureId: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonAbility[] =           _("AbilityNum: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonIVs[] =               _("All IVs:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonIV_0[] =              _("IV HP:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonIV_1[] =              _("IV Attack:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonIV_2[] =              _("IV Defense:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonIV_3[] =              _("IV Speed:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonIV_4[] =              _("IV Sp. Attack:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonIV_5[] =              _("IV Sp. Defense:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonMove_0[] =            _("Move 0: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonMove_1[] =            _("Move 1: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonMove_2[] =            _("Move 2: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_PokemonMove_3[] =            _("Move 3: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_Give_MaxMoney[] =            _("Max Money");
static const u8 sDebugText_Give_MaxCoins[] =            _("Max Coins");
static const u8 sDebugText_Give_BattlePoints[] =        _("Max Battle Points");
static const u8 sDebugText_Give_DaycareEgg[] =          _("Daycare Egg");
// Pokemon Creator
static const u8 sDebugText_PkmCreator_Party_Add[] =                 _("Party add");
static const u8 sDebugText_PkmCreator_Party_Edit[] =                _("Party edit");
static const u8 sDebugText_PkmCreator_PC_Edit[] =                   _("PC edit");
static const u8 sDebugText_PkmCreator_Enemy_Party_Edit[] =          _("Enemy edit");
static const u8 sDebugText_PkmCreator_Enemy_Party_Edit_Debug[] =    _("Enemy edit debug");
static const u8 sDebugText_PkmCreator_Debug_Edit[] =                _("Debug edit");
static const u8 sDebugText_PkmCreator_Enemy_Party_Add[] =           _("Enemy add");
static const u8 sDebugText_PkmCreator_Testing[] =                   _("Testing");
static const u8 sDebugText_PkmCreator_Testing_Copy[] =              _("Testing (copy 1st mon)");
// Fill Menu
static const u8 sDebugText_Fill_Pc_Fast[] =        _("Fill PCBoxes Fast");
static const u8 sDebugText_Fill_Pc_Slow[] =        _("Fill PCBoxes Slow (LAG!)");
static const u8 sDebugText_Fill_Pc_Items[] =       _("Fill PCItems");
static const u8 sDebugText_Fill_PocketItems[] =    _("Fill Pocket Items");
static const u8 sDebugText_Fill_PocketPokeBalls[] =_("Fill Pocket PokeBalls");
static const u8 sDebugText_Fill_PocketTMHM[] =     _("Fill Pocket TMHM");
static const u8 sDebugText_Fill_PocketBerries[] =  _("Fill Pocket Berries");
static const u8 sDebugText_Fill_PocketKeyItems[] = _("Fill Pocket KeyItems");
// Sound Mneu
static const u8 sDebugText_Sound_SE[] =                 _("Effects…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Sound_SE_ID[] =              _("Sound Id: {STR_VAR_3}\n{STR_VAR_1}    \n{STR_VAR_2}");
static const u8 sDebugText_Sound_MUS[] =                _("Music…{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_Sound_MUS_ID[] =             _("Music Id: {STR_VAR_3}\n{STR_VAR_1}    \n{STR_VAR_2}");

static const u8 sDebugText_PartyBoxes_HatchAnEgg[] =         _("Hatch an Egg");

static const u8 digitInidicator_1[] =               _("{LEFT_ARROW}+1{RIGHT_ARROW}        ");
static const u8 digitInidicator_10[] =              _("{LEFT_ARROW}+10{RIGHT_ARROW}       ");
static const u8 digitInidicator_100[] =             _("{LEFT_ARROW}+100{RIGHT_ARROW}      ");
static const u8 digitInidicator_1000[] =            _("{LEFT_ARROW}+1000{RIGHT_ARROW}     ");
static const u8 digitInidicator_10000[] =           _("{LEFT_ARROW}+10000{RIGHT_ARROW}    ");
static const u8 digitInidicator_100000[] =          _("{LEFT_ARROW}+100000{RIGHT_ARROW}   ");
static const u8 digitInidicator_1000000[] =         _("{LEFT_ARROW}+1000000{RIGHT_ARROW}  ");
static const u8 digitInidicator_10000000[] =        _("{LEFT_ARROW}+10000000{RIGHT_ARROW} ");
const u8 * const gText_DigitIndicator[] =
{
    digitInidicator_1,
    digitInidicator_10,
    digitInidicator_100,
    digitInidicator_1000,
    digitInidicator_10000,
    digitInidicator_100000,
    digitInidicator_1000000,
    digitInidicator_10000000
};
static const s32 sPowersOfTen[] =
{
             1,
            10,
           100,
          1000,
         10000,
        100000,
       1000000,
      10000000,
     100000000,
    1000000000,
};

// *******************************
// List Menu Items
static const struct ListMenuItem sDebugMenu_Items_Main[] =
{
    [DEBUG_MENU_ITEM_UTILITIES]       = {sDebugText_Utilities,  DEBUG_MENU_ITEM_UTILITIES},
    [DEBUG_MENU_ITEM_SCRIPTS]         = {sDebugText_Scripts,    DEBUG_MENU_ITEM_SCRIPTS},
    [DEBUG_MENU_ITEM_FLAGVAR]         = {sDebugText_FlagsVars,  DEBUG_MENU_ITEM_FLAGVAR},
    [DEBUG_MENU_ITEM_BATTLE]          = {sDebugText_Battle,     DEBUG_MENU_ITEM_BATTLE},
    [DEBUG_MENU_ITEM_GIVE]            = {sDebugText_Give,       DEBUG_MENU_ITEM_GIVE},
    [DEBUG_MENU_ITEM_POKEMON_CREATOR] = {sDebugText_PkmCreator, DEBUG_MENU_ITEM_POKEMON_CREATOR},
    [DEBUG_MENU_ITEM_FILL]            = {sDebugText_Fill,       DEBUG_MENU_ITEM_FILL},
    [DEBUG_MENU_ITEM_SOUND]           = {sDebugText_Sound,      DEBUG_MENU_ITEM_SOUND},
    [DEBUG_MENU_ITEM_ACCESS_PC]       = {sDebugText_AccessPC,   DEBUG_MENU_ITEM_ACCESS_PC},
    [DEBUG_MENU_ITEM_CANCEL]          = {sDebugText_Cancel,     DEBUG_MENU_ITEM_CANCEL}
};
static const struct ListMenuItem sDebugMenu_Items_Utilities[] =
{
    [DEBUG_UTIL_MENU_ITEM_HEAL_PARTY]       = {sDebugText_Util_HealParty,        DEBUG_UTIL_MENU_ITEM_HEAL_PARTY},
    [DEBUG_UTIL_MENU_ITEM_FLY]              = {sDebugText_Util_Fly,              DEBUG_UTIL_MENU_ITEM_FLY},
    [DEBUG_UTIL_MENU_ITEM_WARP]             = {sDebugText_Util_WarpToMap,        DEBUG_UTIL_MENU_ITEM_WARP},
    [DEBUG_UTIL_MENU_ITEM_POISON_MONS]      = {sDebugText_Util_PoisonMons,       DEBUG_UTIL_MENU_ITEM_POISON_MONS},
    [DEBUG_UTIL_MENU_ITEM_SAVEBLOCK]        = {sDebugText_Util_SaveBlockSpace,   DEBUG_UTIL_MENU_ITEM_SAVEBLOCK},
    [DEBUG_UTIL_MENU_ITEM_WEATHER]          = {sDebugText_Util_Weather,          DEBUG_UTIL_MENU_ITEM_WEATHER},
    [DEBUG_UTIL_MENU_ITEM_CHECKWALLCLOCK]   = {sDebugText_Util_CheckWallClock,   DEBUG_UTIL_MENU_ITEM_CHECKWALLCLOCK},
    [DEBUG_UTIL_MENU_ITEM_SETWALLCLOCK]     = {sDebugText_Util_SetWallClock,     DEBUG_UTIL_MENU_ITEM_SETWALLCLOCK},
    [DEBUG_UTIL_MENU_ITEM_WATCHCREDITS]     = {sDebugText_Util_WatchCredits,     DEBUG_UTIL_MENU_ITEM_WATCHCREDITS},
    [DEBUG_UTIL_MENU_ITEM_TRAINER_NAME]     = {sDebugText_Util_Trainer_Name,     DEBUG_UTIL_MENU_ITEM_TRAINER_NAME},
    [DEBUG_UTIL_MENU_ITEM_TRAINER_GENDER]   = {sDebugText_Util_Trainer_Gender,   DEBUG_UTIL_MENU_ITEM_TRAINER_GENDER},
    [DEBUG_UTIL_MENU_ITEM_TRAINER_ID]       = {sDebugText_Util_Trainer_Id,       DEBUG_UTIL_MENU_ITEM_TRAINER_ID},
    [DEBUG_UTIL_MENU_ITEM_CHEAT]            = {sDebugText_Util_CheatStart,        DEBUG_UTIL_MENU_ITEM_CHEAT},
};
static const struct ListMenuItem sDebugMenu_Items_Scripts[] =
{
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_1]     = {sDebugText_Util_Script_1,    DEBUG_UTIL_MENU_ITEM_SCRIPT_1},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_2]     = {sDebugText_Util_Script_2,    DEBUG_UTIL_MENU_ITEM_SCRIPT_2},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_3]     = {sDebugText_Util_Script_3,    DEBUG_UTIL_MENU_ITEM_SCRIPT_3},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_4]     = {sDebugText_Util_Script_4,    DEBUG_UTIL_MENU_ITEM_SCRIPT_4},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_5]     = {sDebugText_Util_Script_5,    DEBUG_UTIL_MENU_ITEM_SCRIPT_5},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_6]     = {sDebugText_Util_Script_6,    DEBUG_UTIL_MENU_ITEM_SCRIPT_6},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_7]     = {sDebugText_Util_Script_7,    DEBUG_UTIL_MENU_ITEM_SCRIPT_7},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_8]     = {sDebugText_Util_Script_8,    DEBUG_UTIL_MENU_ITEM_SCRIPT_8},
};
static const struct ListMenuItem sDebugMenu_Items_FlagsVars[] =
{
    [DEBUG_FLAGVAR_MENU_ITEM_FLAGS]                = {sDebugText_FlagsVars_Flags,              DEBUG_FLAGVAR_MENU_ITEM_FLAGS},
    [DEBUG_FLAGVAR_MENU_ITEM_VARS]                 = {sDebugText_FlagsVars_Vars,               DEBUG_FLAGVAR_MENU_ITEM_VARS},
    [DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_ALL]         = {sDebugText_FlagsVars_PokedexFlags_All,   DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_ALL},
    [DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_RESET]       = {sDebugText_FlagsVars_PokedexFlags_Reset, DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_RESET},
    [DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_SEEN]        = {sDebugText_FlagsVars_PokedexFlags_Seen,  DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_SEEN},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKEDEX]       = {sDebugText_FlagsVars_SwitchDex,          DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKEDEX},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_NATDEX]        = {sDebugText_FlagsVars_SwitchNationalDex,  DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_NATDEX},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKENAV]       = {sDebugText_FlagsVars_SwitchPokeNav,      DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKENAV},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_RUN_SHOES]     = {sDebugText_FlagsVars_RunningShoes,       DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_RUN_SHOES},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_LOCATIONS]     = {sDebugText_FlagsVars_ToggleFlyFlags,     DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_LOCATIONS},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BADGES_ALL]    = {sDebugText_FlagsVars_ToggleAllBadges,    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BADGES_ALL},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_FRONTIER_PASS] = {sDebugText_FlagsVars_ToggleFrontierPass, DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_FRONTIER_PASS},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BATTLE_DMG]    = {sDebugText_FlagsVars_BattleDmg,          DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BATTLE_DMG},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_COLISSION]     = {sDebugText_FlagsVars_SwitchCollision,    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_COLISSION},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_ENCOUNTER]     = {sDebugText_FlagsVars_SwitchEncounter,    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_ENCOUNTER},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_TRAINER_SEE]   = {sDebugText_FlagsVars_SwitchTrainerSee,   DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_TRAINER_SEE},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BAG_USE]       = {sDebugText_FlagsVars_SwitchBagUse,       DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BAG_USE},
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_CATCHING]      = {sDebugText_FlagsVars_SwitchCatching,     DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_CATCHING},
};
static const struct ListMenuItem sDebugMenu_Items_Battle_0[] =
{
    [DEBUG_BATTLE_0_MENU_ITEM_WILD]        = {sDebugText_Battle_0_Wild,       DEBUG_BATTLE_0_MENU_ITEM_WILD},
    #ifdef BATTLE_ENGINE
    [DEBUG_BATTLE_0_MENU_ITEM_WILD_DOUBLE] = {sDebugText_Battle_0_WildDouble, DEBUG_BATTLE_0_MENU_ITEM_WILD_DOUBLE},
    #endif
    [DEBUG_BATTLE_0_MENU_ITEM_SINGLE]      = {sDebugText_Battle_0_Single,     DEBUG_BATTLE_0_MENU_ITEM_SINGLE},
    [DEBUG_BATTLE_0_MENU_ITEM_DOUBLE]      = {sDebugText_Battle_0_Double,     DEBUG_BATTLE_0_MENU_ITEM_DOUBLE},
    [DEBUG_BATTLE_0_MENU_ITEM_MULTI]       = {sDebugText_Battle_0_Mulit,      DEBUG_BATTLE_0_MENU_ITEM_MULTI},
};
static const struct ListMenuItem sDebugMenu_Items_Battle_1[] =
{
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_00] = {sDebugText_Battle_1_AIFlag_00, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_00},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_01] = {sDebugText_Battle_1_AIFlag_01, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_01},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_02] = {sDebugText_Battle_1_AIFlag_02, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_02},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_03] = {sDebugText_Battle_1_AIFlag_03, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_03},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_04] = {sDebugText_Battle_1_AIFlag_04, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_04},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_05] = {sDebugText_Battle_1_AIFlag_05, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_05},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_06] = {sDebugText_Battle_1_AIFlag_06, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_06},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_07] = {sDebugText_Battle_1_AIFlag_07, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_07},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_08] = {sDebugText_Battle_1_AIFlag_08, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_08},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_09] = {sDebugText_Battle_1_AIFlag_09, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_09},
#ifdef BATTLE_ENGINE
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_10] = {sDebugText_Battle_1_AIFlag_10, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_10},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_11] = {sDebugText_Battle_1_AIFlag_11, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_11},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_12] = {sDebugText_Battle_1_AIFlag_12, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_12},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_13] = {sDebugText_Battle_1_AIFlag_13, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_13},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_14] = {sDebugText_Battle_1_AIFlag_14, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_14},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_15] = {sDebugText_Battle_1_AIFlag_15, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_15},
    [DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_16] = {sDebugText_Battle_1_AIFlag_16, DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_16},
#endif
    [DEBUG_BATTLE_1_MENU_ITEM_CONTINUE]   = {sDebugText_Continue,           DEBUG_BATTLE_1_MENU_ITEM_CONTINUE},
};
static const struct ListMenuItem sDebugMenu_Items_Battle_2[] =
{
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_0]     = {sDebugText_Battle_2_Terrain_0,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_0},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_1]     = {sDebugText_Battle_2_Terrain_1,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_1},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_2]     = {sDebugText_Battle_2_Terrain_2,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_2},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_3]     = {sDebugText_Battle_2_Terrain_3,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_3},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_4]     = {sDebugText_Battle_2_Terrain_4,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_4},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_5]     = {sDebugText_Battle_2_Terrain_5,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_5},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_6]     = {sDebugText_Battle_2_Terrain_6,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_6},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_7]     = {sDebugText_Battle_2_Terrain_7,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_7},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_8]     = {sDebugText_Battle_2_Terrain_8,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_8},
    [DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_9]     = {sDebugText_Battle_2_Terrain_9,    DEBUG_BATTLE_2_MENU_ITEM_TERRAIN_9},
};
static const struct ListMenuItem sDebugMenu_Items_Give[] =
{
    [DEBUG_GIVE_MENU_ITEM_ITEM_X]            = {sDebugText_Give_GiveItem,           DEBUG_GIVE_MENU_ITEM_ITEM_X},
    [DEBUG_GIVE_MENU_ITEM_ALLTMS]            = {sDebugText_Give_AllTMs,             DEBUG_GIVE_MENU_ITEM_ALLTMS},
    [DEBUG_GIVE_MENU_ITEM_POKEMON_SIMPLE]    = {sDebugText_Give_GivePokemonSimple,  DEBUG_GIVE_MENU_ITEM_POKEMON_SIMPLE},
    [DEBUG_GIVE_MENU_ITEM_POKEMON_COMPLEX]   = {sDebugText_Give_GivePokemonComplex, DEBUG_GIVE_MENU_ITEM_POKEMON_COMPLEX},
    [DEBUG_GIVE_MENU_ITEM_MAX_MONEY]         = {sDebugText_Give_MaxMoney,           DEBUG_GIVE_MENU_ITEM_MAX_MONEY},
    [DEBUG_GIVE_MENU_ITEM_MAX_COINS]         = {sDebugText_Give_MaxCoins,           DEBUG_GIVE_MENU_ITEM_MAX_COINS},
    [DEBUG_GIVE_MENU_ITEM_MAX_BATTLE_POINTS] = {sDebugText_Give_BattlePoints,       DEBUG_GIVE_MENU_ITEM_MAX_BATTLE_POINTS},
    [DEBUG_GIVE_MENU_ITEM_DAYCARE_EGG]       = {sDebugText_Give_DaycareEgg,         DEBUG_GIVE_MENU_ITEM_DAYCARE_EGG},
    [DEBUG_GIVE_MENU_ITEM_HATCH_AN_EGG]      = {sDebugText_PartyBoxes_HatchAnEgg,   DEBUG_GIVE_MENU_ITEM_HATCH_AN_EGG},
};
static const struct ListMenuItem sDebugMenu_Items_PkmCreator[] =
{
    [DEBUG_PKM_CREATOR_MENU_ITEM_PARTY_ADD]              =  {sDebugText_PkmCreator_Party_Add,              DEBUG_PKM_CREATOR_MENU_ITEM_PARTY_ADD},
    [DEBUG_PKM_CREATOR_MENU_ITEM_PARTY_EDIT]             =  {sDebugText_PkmCreator_Party_Edit,             DEBUG_PKM_CREATOR_MENU_ITEM_PARTY_EDIT},
    [DEBUG_PKM_CREATOR_MENU_ITEM_PC_EDIT]                =  {sDebugText_PkmCreator_PC_Edit,                DEBUG_PKM_CREATOR_MENU_ITEM_PC_EDIT},
    [DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_EDIT]       =  {sDebugText_PkmCreator_Enemy_Party_Edit,       DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_EDIT},
    [DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_EDIT_DEBUG] =  {sDebugText_PkmCreator_Enemy_Party_Edit_Debug, DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_EDIT_DEBUG},
    [DEBUG_PKM_CREATOR_MENU_ITEM_DEBUG_EDIT]             =  {sDebugText_PkmCreator_Debug_Edit,             DEBUG_PKM_CREATOR_MENU_ITEM_DEBUG_EDIT},
    [DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_ADD]        =  {sDebugText_PkmCreator_Enemy_Party_Add,        DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_ADD},
    [DEBUG_PKM_CREATOR_MENU_ITEM_TESTING]                =  {sDebugText_PkmCreator_Testing,                DEBUG_PKM_CREATOR_MENU_ITEM_TESTING},
    [DEBUG_PKM_CREATOR_MENU_ITEM_TESTING_COPY]           =  {sDebugText_PkmCreator_Testing_Copy,           DEBUG_PKM_CREATOR_MENU_ITEM_TESTING_COPY},
};
static const struct ListMenuItem sDebugMenu_Items_Fill[] =
{
    [DEBUG_FILL_MENU_ITEM_PC_BOXES_FAST]    = {sDebugText_Fill_Pc_Fast,         DEBUG_FILL_MENU_ITEM_PC_BOXES_FAST},
    [DEBUG_FILL_MENU_ITEM_PC_BOXES_SLOW]    = {sDebugText_Fill_Pc_Slow,         DEBUG_FILL_MENU_ITEM_PC_BOXES_SLOW},
    [DEBUG_FILL_MENU_ITEM_PC_ITEMS]         = {sDebugText_Fill_Pc_Items ,       DEBUG_FILL_MENU_ITEM_PC_ITEMS},
    [DEBUG_FILL_MENU_ITEM_POCKET_ITEMS]     = {sDebugText_Fill_PocketItems,     DEBUG_FILL_MENU_ITEM_POCKET_ITEMS},
    [DEBUG_FILL_MENU_ITEM_POCKET_BALLS]     = {sDebugText_Fill_PocketPokeBalls, DEBUG_FILL_MENU_ITEM_POCKET_BALLS},
    [DEBUG_FILL_MENU_ITEM_POCKET_TMHM]      = {sDebugText_Fill_PocketTMHM,      DEBUG_FILL_MENU_ITEM_POCKET_TMHM},
    [DEBUG_FILL_MENU_ITEM_POCKET_BERRIES]   = {sDebugText_Fill_PocketBerries,   DEBUG_FILL_MENU_ITEM_POCKET_BERRIES},
    [DEBUG_FILL_MENU_ITEM_POCKET_KEY_ITEMS] = {sDebugText_Fill_PocketKeyItems,  DEBUG_FILL_MENU_ITEM_POCKET_KEY_ITEMS},
};
static const struct ListMenuItem sDebugMenu_Items_Sound[] =
{
    [DEBUG_SOUND_MENU_ITEM_SE]            = {sDebugText_Sound_SE,         DEBUG_SOUND_MENU_ITEM_SE},
    [DEBUG_SOUND_MENU_ITEM_MUS]           = {sDebugText_Sound_MUS,        DEBUG_SOUND_MENU_ITEM_MUS},
};

// *******************************
// Menu Actions
static void (*const sDebugMenu_Actions_Main[])(u8) =
{
    [DEBUG_MENU_ITEM_UTILITIES]       = DebugAction_OpenUtilitiesMenu,
    [DEBUG_MENU_ITEM_SCRIPTS]         = DebugAction_OpenScriptsMenu,
    [DEBUG_MENU_ITEM_FLAGVAR]         = DebugAction_OpenFlagsVarsMenu,
    [DEBUG_MENU_ITEM_BATTLE]          = DebugAction_OpenBattleMenu,
    [DEBUG_MENU_ITEM_GIVE]            = DebugAction_OpenGiveMenu,
    [DEBUG_MENU_ITEM_POKEMON_CREATOR] = DebugAction_OpenPokemonCreator,
    [DEBUG_MENU_ITEM_FILL]            = DebugAction_OpenFillMenu,
    [DEBUG_MENU_ITEM_SOUND]           = DebugAction_OpenSoundMenu,
    [DEBUG_MENU_ITEM_ACCESS_PC]       = DebugAction_AccessPC,
    [DEBUG_MENU_ITEM_CANCEL]          = DebugAction_Cancel
};
static void (*const sDebugMenu_Actions_Utilities[])(u8) =
{
    [DEBUG_UTIL_MENU_ITEM_HEAL_PARTY]       = DebugAction_Util_HealParty,
    [DEBUG_UTIL_MENU_ITEM_FLY]              = DebugAction_Util_Fly,
    [DEBUG_UTIL_MENU_ITEM_WARP]             = DebugAction_Util_Warp_Warp,
    [DEBUG_UTIL_MENU_ITEM_POISON_MONS]      = DebugAction_Util_PoisonMons,
    [DEBUG_UTIL_MENU_ITEM_SAVEBLOCK]        = DebugAction_Util_CheckSaveBlock,
    [DEBUG_UTIL_MENU_ITEM_WEATHER]          = DebugAction_Util_Weather,
    [DEBUG_UTIL_MENU_ITEM_CHECKWALLCLOCK]   = DebugAction_Util_CheckWallClock,
    [DEBUG_UTIL_MENU_ITEM_SETWALLCLOCK]     = DebugAction_Util_SetWallClock,
    [DEBUG_UTIL_MENU_ITEM_WATCHCREDITS]     = DebugAction_Util_WatchCredits,
    [DEBUG_UTIL_MENU_ITEM_TRAINER_NAME]     = DebugAction_Util_Trainer_Name,
    [DEBUG_UTIL_MENU_ITEM_TRAINER_GENDER]   = DebugAction_Util_Trainer_Gender,
    [DEBUG_UTIL_MENU_ITEM_TRAINER_ID]       = DebugAction_Util_Trainer_Id,
    [DEBUG_UTIL_MENU_ITEM_CHEAT]            = DebugAction_Util_CheatStart,
};
static void (*const sDebugMenu_Actions_Scripts[])(u8) =
{
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_1]     = DebugAction_Util_Script_1,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_2]     = DebugAction_Util_Script_2,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_3]     = DebugAction_Util_Script_3,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_4]     = DebugAction_Util_Script_4,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_5]     = DebugAction_Util_Script_5,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_6]     = DebugAction_Util_Script_6,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_7]     = DebugAction_Util_Script_7,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_8]     = DebugAction_Util_Script_8,
};
static void (*const sDebugMenu_Actions_Flags[])(u8) =
{
    [DEBUG_FLAGVAR_MENU_ITEM_FLAGS]                = DebugAction_FlagsVars_Flags,
    [DEBUG_FLAGVAR_MENU_ITEM_VARS]                 = DebugAction_FlagsVars_Vars,
    [DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_ALL]         = DebugAction_FlagsVars_PokedexFlags_All,
    [DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_RESET]       = DebugAction_FlagsVars_PokedexFlags_Reset,
    [DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_SEEN]        = DebugAction_FlagsVars_PokedexFlags_Seen,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKEDEX]       = DebugAction_FlagsVars_SwitchDex,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_NATDEX]        = DebugAction_FlagsVars_SwitchNatDex,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKENAV]       = DebugAction_FlagsVars_SwitchPokeNav,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_RUN_SHOES]     = DebugAction_FlagsVars_RunningShoes,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_LOCATIONS]     = DebugAction_FlagsVars_ToggleFlyFlags,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BADGES_ALL]    = DebugAction_FlagsVars_ToggleBadgeFlags,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_FRONTIER_PASS] = DebugAction_FlagsVars_ToggleFrontierPass,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BATTLE_DMG]    = DebugAction_FlagsVars_BattleDmgOnOff,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_COLISSION]     = DebugAction_FlagsVars_CollisionOnOff,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_ENCOUNTER]     = DebugAction_FlagsVars_EncounterOnOff,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_TRAINER_SEE]   = DebugAction_FlagsVars_TrainerSeeOnOff,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BAG_USE]       = DebugAction_FlagsVars_BagUseOnOff,
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_CATCHING]      = DebugAction_FlagsVars_CatchingOnOff,
};
static void (*const sDebugMenu_Actions_Give[])(u8) =
{
    [DEBUG_GIVE_MENU_ITEM_ITEM_X]            = DebugAction_Give_Item,
    [DEBUG_GIVE_MENU_ITEM_ALLTMS]            = DebugAction_Give_AllTMs,
    [DEBUG_GIVE_MENU_ITEM_POKEMON_SIMPLE]    = DebugAction_Give_PokemonSimple,
    [DEBUG_GIVE_MENU_ITEM_POKEMON_COMPLEX]   = DebugAction_Give_PokemonComplex,
    [DEBUG_GIVE_MENU_ITEM_MAX_MONEY]         = DebugAction_Give_MaxMoney,
    [DEBUG_GIVE_MENU_ITEM_MAX_COINS]         = DebugAction_Give_MaxCoins,
    [DEBUG_GIVE_MENU_ITEM_MAX_BATTLE_POINTS] = DebugAction_Give_MaxBattlePoints,
    [DEBUG_GIVE_MENU_ITEM_DAYCARE_EGG]       = DebugAction_Give_DayCareEgg,
    [DEBUG_GIVE_MENU_ITEM_HATCH_AN_EGG]      = DebugAction_HatchAnEgg,
};
static void (*const sDebugMenu_Actions_PkmCreator[])(u8) =
{
    [DEBUG_PKM_CREATOR_MENU_ITEM_PARTY_ADD]              = DebugAction_PkmCreator_Party_Add,
    [DEBUG_PKM_CREATOR_MENU_ITEM_PARTY_EDIT]             = DebugAction_PkmCreator_Party_Edit,
    [DEBUG_PKM_CREATOR_MENU_ITEM_PC_EDIT]                = DebugAction_PkmCreator_PC_Edit,
    [DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_EDIT]       = DebugAction_PkmCreator_Enemy_Party_Edit,
    [DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_EDIT_DEBUG] = DebugAction_PkmCreator_Enemy_Party_Edit_Debug,
    [DEBUG_PKM_CREATOR_MENU_ITEM_DEBUG_EDIT]             = DebugAction_PkmCreator_Debug_Edit,
    [DEBUG_PKM_CREATOR_MENU_ITEM_ENEMY_PARTY_ADD]        = DebugAction_PkmCreator_Enemy_Party_Add,
    [DEBUG_PKM_CREATOR_MENU_ITEM_TESTING]                = DebugAction_PkmCreator_Testing,
    [DEBUG_PKM_CREATOR_MENU_ITEM_TESTING_COPY]           = DebugAction_PkmCreator_Testing_Copy,
};
static void (*const sDebugMenu_Actions_Fill[])(u8) =
{
    [DEBUG_FILL_MENU_ITEM_PC_BOXES_FAST]    = DebugAction_Fill_PCBoxes_Fast,
    [DEBUG_FILL_MENU_ITEM_PC_BOXES_SLOW]    = DebugAction_Fill_PCBoxes_Slow,
    [DEBUG_FILL_MENU_ITEM_PC_ITEMS]         = DebugAction_Fill_PCItemStorage,
    [DEBUG_FILL_MENU_ITEM_POCKET_ITEMS]     = DebugAction_Fill_PocketItems,
    [DEBUG_FILL_MENU_ITEM_POCKET_BALLS]     = DebugAction_Fill_PocketPokeBalls,
    [DEBUG_FILL_MENU_ITEM_POCKET_TMHM]      = DebugAction_Fill_PocketTMHM,
    [DEBUG_FILL_MENU_ITEM_POCKET_BERRIES]   = DebugAction_Fill_PocketBerries,
    [DEBUG_FILL_MENU_ITEM_POCKET_KEY_ITEMS] = DebugAction_Fill_PocketKeyItems,
};

static void (*const sDebugMenu_Actions_Sound[])(u8) =
{
    [DEBUG_SOUND_MENU_ITEM_SE]      = DebugAction_Sound_SE,
    [DEBUG_SOUND_MENU_ITEM_MUS]     = DebugAction_Sound_MUS,
};


// *******************************
// Windows
static const struct WindowTemplate sDebugMenuWindowTemplateMain =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_MAIN,
    .height = 2 * DEBUG_MENU_HEIGHT_MAIN,
    .paletteNum = 15,
    .baseBlock = 1,
};
static const struct WindowTemplate sDebugMenuWindowTemplateExtra =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_EXTRA - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_EXTRA,
    .height = 2 * DEBUG_MENU_HEIGHT_EXTRA,
    .paletteNum = 15,
    .baseBlock = 1,
};
static const struct WindowTemplate sDebugMenuWindowTemplateWeather =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_WEATHER - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_WEATHER,
    .height = 2 * DEBUG_MENU_HEIGHT_WEATHER,
    .paletteNum = 15,
    .baseBlock = 1,
};
static const struct WindowTemplate sDebugMenuWindowTemplateSound =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_SOUND - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_SOUND,
    .height = DEBUG_MENU_HEIGHT_SOUND,
    .paletteNum = 15,
    .baseBlock = 1,
};
static const struct WindowTemplate sDebugMenuWindowTemplateFlagsVars =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_FLAGVAR - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_FLAGVAR,
    .height = DEBUG_MENU_HEIGHT_FLAGVAR,
    .paletteNum = 15,
    .baseBlock = 1 + DEBUG_MENU_WIDTH_MAIN * DEBUG_MENU_HEIGHT_MAIN * 2,
};

// *******************************
// List Menu Templates
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Main =
{
    .items = sDebugMenu_Items_Main,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Main),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Utilities =
{
    .items = sDebugMenu_Items_Utilities,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Utilities),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Scripts =
{
    .items = sDebugMenu_Items_Scripts,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Scripts),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_FlagsVars =
{
    .items = sDebugMenu_Items_FlagsVars,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_FlagsVars),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Battle_0 =
{
    .items = sDebugMenu_Items_Battle_0,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Battle_0),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Battle_1 =
{
    .items = sDebugMenu_Items_Battle_1,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Battle_1),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Battle_2 =
{
    .items = sDebugMenu_Items_Battle_2,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Battle_2),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Give =
{
    .items = sDebugMenu_Items_Give,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Give),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_PkmCreator =
{
    .items = sDebugMenu_Items_PkmCreator,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_PkmCreator),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Fill =
{
    .items = sDebugMenu_Items_Fill,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Fill),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Sound =
{
    .items = sDebugMenu_Items_Sound,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Sound),
};


// *******************************
// Functions universal
void Debug_ShowMainMenu(void)
{
    sDebugBattleData = AllocZeroed(sizeof(*sDebugBattleData));
    sDebugMenuListData = AllocZeroed(sizeof(*sDebugMenuListData));
    Debug_InitDebugBattleData();

    Debug_ShowMenu(DebugTask_HandleMenuInput_Main, sDebugMenu_ListTemplate_Main);
}
static void Debug_ReShowMainMenu(void)
{
    Debug_ShowMenu(DebugTask_HandleMenuInput_Main, sDebugMenu_ListTemplate_Main);
}
static u8 Debug_ShowMenu(void (*HandleInput)(u8), struct ListMenuTemplate LMtemplate)
{
    struct ListMenuTemplate menuTemplate;
    u8 windowId;
    u8 menuTaskId;
    u8 inputTaskId;

    // create window
    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateMain);
    DrawStdWindowFrame(windowId, FALSE);

    // create list menu
    menuTemplate = LMtemplate;
    menuTemplate.maxShowed = DEBUG_MENU_HEIGHT_MAIN;
    menuTemplate.windowId = windowId;
    menuTemplate.header_X = 0;
    menuTemplate.item_X = 8;
    menuTemplate.cursor_X = 0;
    menuTemplate.upText_Y = 1;
    menuTemplate.cursorPal = 2;
    menuTemplate.fillValue = 1;
    menuTemplate.cursorShadowPal = 3;
    menuTemplate.lettersSpacing = 1;
    menuTemplate.itemVerticalPadding = 0;
    menuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    menuTemplate.fontId = 1;
    menuTemplate.cursorKind = 0;
    menuTaskId = ListMenuInit(&menuTemplate, 0, 0);

    // create input handler task
    inputTaskId = CreateTask(HandleInput, 3);
    gTasks[inputTaskId].data[0] = menuTaskId;
    gTasks[inputTaskId].data[1] = windowId;
    gTasks[inputTaskId].data[2] = 0;

    Debug_RefreshListMenu(inputTaskId);

    // draw everything
    CopyWindowToVram(windowId, 3);

    // return taskId for use right after
    return inputTaskId;
}
void Debug_ReShowBattleDebugMenu(void)
{
    u8 taskId = Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, gMultiuseListMenuTemplate);
    Debug_RedrawListMenu(taskId);
}
static void Debug_DestroyMenu(u8 taskId)
{
    DestroyListMenuTask(gTasks[taskId].data[0], NULL, NULL);
    RemoveWindow(gTasks[taskId].data[1]);
    DestroyTask(taskId);
}
static void Debug_DestroyMenu_Full(u8 taskId)
{
    if (gTasks[taskId].data[2] != 0)
    {
        ClearStdWindowAndFrame(gTasks[taskId].data[2], FALSE);
        DebugAction_DestroyExtraWindow(taskId);
    }
    DestroyListMenuTask(gTasks[taskId].data[0], NULL, NULL);
    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);
    DestroyTask(taskId);
    UnfreezeObjectEvents();
    Free(sDebugMenuListData);
    Free(sDebugBattleData);
}
static void Debug_DestroyMenu_Full_Script(u8 taskId, const u8 *script)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    FreezeObjectEvents();
    ScriptContext_SetupScript(script);
}
static void DebugAction_Cancel(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_DestroyExtraWindow(u8 taskId)
{
    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    ClearStdWindowAndFrame(gTasks[taskId].data[2], TRUE);
    RemoveWindow(gTasks[taskId].data[2]);

    DestroyTask(taskId);
    ScriptContext_Enable();
    UnfreezeObjectEvents();
}

static u8 Debug_CheckToggleFlags(u8 id)
{
    u8 result = FALSE;

    switch (id)
    {
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKEDEX:
            result = FlagGet(FLAG_SYS_POKEDEX_GET);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_NATDEX:
            result = IsNationalPokedexEnabled();
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKENAV:
            result = FlagGet(FLAG_SYS_POKENAV_GET);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_RUN_SHOES:
            result = FlagGet(FLAG_SYS_B_DASH);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_LOCATIONS:
            result = FlagGet(FLAG_VISITED_LITTLEROOT_TOWN) &&
                FlagGet(FLAG_VISITED_OLDALE_TOWN) &&
                FlagGet(FLAG_VISITED_DEWFORD_TOWN) &&
                FlagGet(FLAG_VISITED_LAVARIDGE_TOWN) &&
                FlagGet(FLAG_VISITED_FALLARBOR_TOWN) &&
                FlagGet(FLAG_VISITED_VERDANTURF_TOWN) &&
                FlagGet(FLAG_VISITED_PACIFIDLOG_TOWN) &&
                FlagGet(FLAG_VISITED_PETALBURG_CITY) &&
                FlagGet(FLAG_VISITED_SLATEPORT_CITY) &&
                FlagGet(FLAG_VISITED_MAUVILLE_CITY) &&
                FlagGet(FLAG_VISITED_RUSTBORO_CITY) &&
                FlagGet(FLAG_VISITED_FORTREE_CITY) &&
                FlagGet(FLAG_VISITED_LILYCOVE_CITY) &&
                FlagGet(FLAG_VISITED_MOSSDEEP_CITY) &&
                FlagGet(FLAG_VISITED_SOOTOPOLIS_CITY) &&
                FlagGet(FLAG_VISITED_EVER_GRANDE_CITY) &&
                FlagGet(FLAG_LANDMARK_POKEMON_LEAGUE) &&
                FlagGet(FLAG_LANDMARK_BATTLE_FRONTIER);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BADGES_ALL:
            result = FlagGet(FLAG_BADGE01_GET) &&
                FlagGet(FLAG_BADGE02_GET) &&
                FlagGet(FLAG_BADGE03_GET) &&
                FlagGet(FLAG_BADGE04_GET) &&
                FlagGet(FLAG_BADGE05_GET) &&
                FlagGet(FLAG_BADGE06_GET) &&
                FlagGet(FLAG_BADGE07_GET) &&
                FlagGet(FLAG_BADGE08_GET);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_FRONTIER_PASS:
            result = FlagGet(FLAG_SYS_FRONTIER_PASS);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BATTLE_DMG:
            result = FlagGet(FLAG_SYS_NO_BATTLE_DMG);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_COLISSION:
            result = FlagGet(FLAG_SYS_NO_COLLISION);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_ENCOUNTER:
            result = FlagGet(FLAG_SYS_NO_ENCOUNTER);
            break; 
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_TRAINER_SEE:
            result = FlagGet(FLAG_SYS_NO_TRAINER_SEE);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BAG_USE:
            result = FlagGet(FLAG_SYS_NO_BAG_USE);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_CATCHING:
            result = FlagGet(FLAG_SYS_NO_CATCHING);
            break;
        default:
            result = 0xFF;
            break;
    }

    return result;
}

static void Debug_InitDebugBattleData(void)
{
    u32 i;
    sDebugBattleData->submenu       = 0;
    sDebugBattleData->battleType    = 0xFF;
    sDebugBattleData->battleTerrain = 0xFF;
    
#ifdef BATTLE_ENGINE
    for (i = 0; i < 17; i++)
        sDebugBattleData->aiFlags[i] = FALSE;
#else
    for (i = 0; i < 10; i++)
        sDebugBattleData->aiFlags[i] = FALSE;
#endif
}

static void Debug_RefreshListMenu(u8 taskId)
{
    u16 i;
    const u8 sColor_Red[] = _("{COLOR RED}");
    const u8 sColor_Green[] = _("{COLOR GREEN}");
    u8 listTaskId = gTasks[taskId].data[0];
    struct ListMenu *list = (void*) gTasks[listTaskId].data;
    u8 totalItems, flagResult;
    u8 const * name;

    if (sDebugMenuListData->listId == 0)
    {
        gMultiuseListMenuTemplate = sDebugMenu_ListTemplate_FlagsVars;
        totalItems = gMultiuseListMenuTemplate.totalItems;
    }
    else if (sDebugMenuListData->listId == 1 && sDebugBattleData->submenu <= 1)
    {
        gMultiuseListMenuTemplate = sDebugMenu_ListTemplate_Battle_1;
        totalItems = gMultiuseListMenuTemplate.totalItems;
    }
    else if (sDebugMenuListData->listId == 1 && sDebugBattleData->submenu > 1)
    {
        gMultiuseListMenuTemplate = sDebugMenu_ListTemplate_Battle_2;
        totalItems = 7;
    }

    // Copy item names for all entries but the last (which is Cancel)
    for(i = 0; i < totalItems; i++)
    {
        
        if (sDebugMenuListData->listId == 1 && sDebugBattleData->submenu > 1)
        {
            u16 species;
            if (i == 6)
            {
                name = sDebugText_Continue;
                StringCopy(&sDebugMenuListData->itemNames[i][0], name);
            }
            else if (GetMonData(&gEnemyParty[i], MON_DATA_SANITY_HAS_SPECIES))
            {
                species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES);
                GetSpeciesName(gStringVar1, species);
                StringCopy(&sDebugMenuListData->itemNames[i][0], gStringVar1);
            }
            else
            {
                StringCopy(&sDebugMenuListData->itemNames[i][0], sDebugText_Dashes);
            }
        }
        else
        {
            if (sDebugMenuListData->listId == 0)
            {
                flagResult = Debug_CheckToggleFlags(i);
                name = sDebugMenu_Items_FlagsVars[i].name;
            }
            else if (sDebugMenuListData->listId == 1)
            {
                flagResult = sDebugBattleData->aiFlags[i];
                if (i == totalItems - 1)
                    flagResult == 0xFF;
                name = sDebugMenu_Items_Battle_1[i].name;
            }
        
            if (flagResult == 0xFF)
            {
                StringCopy(&sDebugMenuListData->itemNames[i][0], name);
            }
            else if (flagResult)
            {
                StringCopy(gStringVar1, sColor_Green);
                StringExpandPlaceholders(gStringVar4, name);
                StringCopy(&sDebugMenuListData->itemNames[i][0], gStringVar4);
            }
            else
            {
                StringCopy(gStringVar1, sColor_Red);
                StringExpandPlaceholders(gStringVar4, name);
                StringCopy(&sDebugMenuListData->itemNames[i][0], gStringVar4);
            }
        }

        sDebugMenuListData->listItems[i].name = &sDebugMenuListData->itemNames[i][0];
        sDebugMenuListData->listItems[i].id = i;
    }

    // Set list menu data
    gMultiuseListMenuTemplate.items = sDebugMenuListData->listItems;
    gMultiuseListMenuTemplate.totalItems = totalItems;
    gMultiuseListMenuTemplate.maxShowed = DEBUG_MENU_HEIGHT_MAIN;
    gMultiuseListMenuTemplate.windowId = gTasks[taskId].data[1];
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.upText_Y = 1;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 1;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.lettersSpacing = 1;
    gMultiuseListMenuTemplate.itemVerticalPadding = 0;
    gMultiuseListMenuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    gMultiuseListMenuTemplate.fontId = 1;
    gMultiuseListMenuTemplate.cursorKind = 0;
}
static void Debug_RedrawListMenu(u8 taskId)
{
    u8 listTaskId = gTasks[taskId].data[0];
    u16 scrollOffset, selectedRow;
    ListMenuGetScrollAndRow(listTaskId, &scrollOffset, &selectedRow);

    DestroyListMenuTask(gTasks[taskId].data[0], &scrollOffset, &selectedRow);
    Debug_RefreshListMenu(taskId);
    gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, scrollOffset, selectedRow);
}


// *******************************
// Handle Inputs
static void DebugTask_HandleMenuInput_Main(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Main[input]) != NULL)
            func(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu_Full(taskId);
        ScriptContext_Enable();
    }
}
static void DebugTask_HandleMenuInput_Utilities(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Utilities[input]) != NULL)
            func(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ReShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_Scripts(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Scripts[input]) != NULL)
            func(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ReShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_FlagsVars(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (gMain.newKeys & A_BUTTON)
    {        
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Flags[input]) != NULL)
        {
            func(taskId);
            Debug_RedrawListMenu(taskId);
        }
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        
        ClearStdWindowAndFrame(gTasks[taskId].data[2], TRUE);
        RemoveWindow(gTasks[taskId].data[2]);

        Debug_ReShowMainMenu();
    }
}

static void DebugTask_HandleBattleMenuReDraw(u8 taskId)
{
    Debug_RefreshListMenu(taskId);
    switch (sDebugBattleData->submenu)
    {
    case 0:
        Debug_DestroyMenu(taskId);
        Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, sDebugMenu_ListTemplate_Battle_0);
        break;
    case 1:
        Debug_DestroyMenu(taskId);
        Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, gMultiuseListMenuTemplate);
        break;
    case 2:
        Debug_DestroyMenu(taskId);
        Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, sDebugMenu_ListTemplate_Battle_2);
        break;
    case 3:
        Debug_DestroyMenu(taskId);
        Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, gMultiuseListMenuTemplate);
        break;
    }
}
static void DebugTask_HandleMenuInput_Battle(u8 taskId)
{
    void (*func)(u8);
    u8 listTaskId = gTasks[taskId].data[0];
    u32 input = ListMenu_ProcessInput(listTaskId);
    u16 idx;

    ListMenuGetCurrentItemArrayId(listTaskId, &idx);

    if (gMain.newKeys & A_BUTTON)
    {        
        PlaySE(SE_SELECT);

        switch (sDebugBattleData->submenu)
        {
        case 0: // Battle type
            sDebugBattleData->battleType = idx;
            sDebugBattleData->submenu++;
            Debug_DestroyMenu(taskId);

            if (sDebugBattleData->battleType == DEBUG_BATTLE_0_MENU_ITEM_WILD // Skip AI Flag selection if wild battle
                #ifdef BATTLE_ENGINE
                || sDebugBattleData->battleType == DEBUG_BATTLE_0_MENU_ITEM_WILD_DOUBLE
                #endif
                )
            {
                sDebugBattleData->submenu++;
                Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, sDebugMenu_ListTemplate_Battle_2);
            }
            else
            {
                Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, gMultiuseListMenuTemplate);
            }
            break;
        case 1: // AI Flags
            if (idx == sDebugMenu_ListTemplate_Battle_1.totalItems - 1)
            {
                sDebugBattleData->submenu++;
                Debug_DestroyMenu(taskId);
                Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, sDebugMenu_ListTemplate_Battle_2);
            }
            else
            {
                sDebugBattleData->aiFlags[idx] = !sDebugBattleData->aiFlags[idx];
                Debug_RedrawListMenu(taskId);
            }
                
            break;
        case 2: // Terrain
            sDebugBattleData->submenu++;
            sDebugBattleData->battleTerrain = idx;
            Debug_DestroyMenu(taskId);
            Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, gMultiuseListMenuTemplate);
            break;
        case 3: // Enemy pokemon
            if (idx == 6)
                Debug_InitializeBattle(taskId);
            else
            {
                if (GetMonData(&gEnemyParty[idx], MON_DATA_SANITY_HAS_SPECIES))
                {
                    Debug_DestroyMenu(taskId);
                    DebugPkmCreator_Init(10, idx);
                }
                else
                {
                    Debug_DestroyMenu(taskId);
                    DebugPkmCreator_Init(9, 0xFF);
                }
            }
            break;
        }
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        switch (sDebugBattleData->submenu)
        {
        case 0: // Return to Main menu
            PlaySE(SE_SELECT);
            Debug_DestroyMenu(taskId);
            Debug_ReShowMainMenu();
            break;
        case 2: // Skip AI Flag selection if wild battle
            if (sDebugBattleData->battleType == DEBUG_BATTLE_0_MENU_ITEM_WILD 
                #ifdef BATTLE_ENGINE
                || sDebugBattleData->battleType == DEBUG_BATTLE_0_MENU_ITEM_WILD_DOUBLE
                #endif
                )
            {
                sDebugBattleData->submenu = 0;
            }
            else
                sDebugBattleData->submenu--;
            DebugTask_HandleBattleMenuReDraw(taskId);
            break;
        default:
            sDebugBattleData->submenu--;
            DebugTask_HandleBattleMenuReDraw(taskId);
            break;
        }
    }
}
static void Debug_InitializeBattle(u8 taskId)
{
    u32 i;
    gBattleTypeFlags = 0;

    // Set main battle flags
    switch (sDebugBattleData->battleType)
    {
    case DEBUG_BATTLE_0_MENU_ITEM_WILD:
        break;
    case DEBUG_BATTLE_0_MENU_ITEM_SINGLE:
        gBattleTypeFlags = (BATTLE_TYPE_TRAINER);
        break;
    case DEBUG_BATTLE_0_MENU_ITEM_DOUBLE:
        gBattleTypeFlags = (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_TRAINER);
        break;
    case DEBUG_BATTLE_0_MENU_ITEM_MULTI:
        gBattleTypeFlags = (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_TRAINER | BATTLE_TYPE_INGAME_PARTNER);
        break;
    }
    
    // Set terrain
    gBattleTerrain = sDebugBattleData->battleTerrain;

    // Set AI flags
    for (i = 0; i < ARRAY_COUNT(sDebugBattleData->aiFlags); i++)
    {
        if (sDebugBattleData->aiFlags[i])
            gDebugAIFlags |= (1 << i);
    }

    gIsDebugBattle = TRUE;
    BattleSetup_StartTrainerBattle_Debug();


    Debug_DestroyMenu_Full(taskId);
}

static void DebugTask_HandleMenuInput_Give(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Give[input]) != NULL)
            func(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ReShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_PkmCreator(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_PkmCreator[input]) != NULL)
            func(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ReShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_Fill(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Fill[input]) != NULL)
            func(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ReShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_Sound(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Sound[input]) != NULL)
            func(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ReShowMainMenu();
    }
}

// *******************************
// Open sub-menus
static void DebugAction_OpenUtilitiesMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Utilities, sDebugMenu_ListTemplate_Utilities);
}
static void DebugAction_OpenScriptsMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Scripts, sDebugMenu_ListTemplate_Scripts);
}
static void DebugAction_OpenFlagsVarsMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    sDebugMenuListData->listId = 0;
    Debug_ShowMenu(DebugTask_HandleMenuInput_FlagsVars, gMultiuseListMenuTemplate);
}
static void DebugAction_OpenBattleMenu(u8 taskId)
{
    // Clean enemy party
    //u32 i;
    //for (i = 0; i < PARTY_SIZE; i++)
    //    ZeroMonData(&gEnemyParty[i]);
    Debug_DestroyMenu(taskId);
    sDebugMenuListData->listId = 1;
    Debug_ShowMenu(DebugTask_HandleMenuInput_Battle, sDebugMenu_ListTemplate_Battle_0);
}
static void DebugAction_OpenGiveMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Give, sDebugMenu_ListTemplate_Give);
}
static void DebugAction_OpenPokemonCreator(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_PkmCreator, sDebugMenu_ListTemplate_PkmCreator);
}
static void DebugAction_OpenFillMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Fill, sDebugMenu_ListTemplate_Fill);
}
static void DebugAction_OpenSoundMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Sound, sDebugMenu_ListTemplate_Sound);
}

// *******************************
// Actions Utilities
static void DebugAction_Util_HealParty(u8 taskId)
{
    PlaySE(SE_USE_ITEM);
    HealPlayerParty();
    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}
static void DebugAction_Util_Fly(u8 taskId)
{
    FlagSet(FLAG_VISITED_LITTLEROOT_TOWN);
    FlagSet(FLAG_VISITED_OLDALE_TOWN);
    FlagSet(FLAG_VISITED_DEWFORD_TOWN);
    FlagSet(FLAG_VISITED_LAVARIDGE_TOWN);
    FlagSet(FLAG_VISITED_FALLARBOR_TOWN);
    FlagSet(FLAG_VISITED_VERDANTURF_TOWN);
    FlagSet(FLAG_VISITED_PACIFIDLOG_TOWN);
    FlagSet(FLAG_VISITED_PETALBURG_CITY);
    FlagSet(FLAG_VISITED_SLATEPORT_CITY);
    FlagSet(FLAG_VISITED_MAUVILLE_CITY);
    FlagSet(FLAG_VISITED_RUSTBORO_CITY);
    FlagSet(FLAG_VISITED_FORTREE_CITY);
    FlagSet(FLAG_VISITED_LILYCOVE_CITY);
    FlagSet(FLAG_VISITED_MOSSDEEP_CITY);
    FlagSet(FLAG_VISITED_SOOTOPOLIS_CITY);
    FlagSet(FLAG_VISITED_EVER_GRANDE_CITY);
    FlagSet(FLAG_LANDMARK_POKEMON_LEAGUE);
    FlagSet(FLAG_LANDMARK_BATTLE_FRONTIER);
    Debug_DestroyMenu_Full(taskId);
    SetMainCallback2(CB2_OpenFlyMap);
}

static void DebugAction_Util_Warp_Warp(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);


    ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
    ConvertIntToDecimalStringN(gStringVar2, MAP_GROUPS_COUNT-1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
    StringCopy(gStringVar3, gText_DigitIndicator[0]);
    StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMapGroup);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Util_Warp_SelectMapGroup;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 0;            //Current Flag
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[5] = 0; //Map Group
    gTasks[taskId].data[6] = 0; //Map
    gTasks[taskId].data[7] = 0; //warp
}
static void DebugAction_Util_Warp_SelectMapGroup(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);
        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > MAP_GROUPS_COUNT-1)
                gTasks[taskId].data[3] = MAP_GROUPS_COUNT-1;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        ConvertIntToDecimalStringN(gStringVar2, MAP_GROUPS_COUNT-1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMapGroup);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        gTasks[taskId].data[5] = gTasks[taskId].data[3];
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        ConvertIntToDecimalStringN(gStringVar2, MAP_GROUP_COUNT[gTasks[taskId].data[5]]-1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        GetMapName(gStringVar2, Overworld_GetMapHeaderByGroupAndId(gTasks[taskId].data[5], gTasks[taskId].data[3])->regionMapSectionId, 0);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMap);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Util_Warp_SelectMap;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Util_Warp_SelectMap(u8 taskId)
{
    u8 max_value = MAP_GROUP_COUNT[gTasks[taskId].data[5]]; //maps in the selected map group

    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);
        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > max_value-1)
                gTasks[taskId].data[3] = max_value-1;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        ConvertIntToDecimalStringN(gStringVar2, MAP_GROUP_COUNT[gTasks[taskId].data[5]]-1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        GetMapName(gStringVar2, Overworld_GetMapHeaderByGroupAndId(gTasks[taskId].data[5], gTasks[taskId].data[3])->regionMapSectionId, 0);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMap);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        gTasks[taskId].data[6] = gTasks[taskId].data[3];
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectWarp);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
        gTasks[taskId].func = DebugAction_Util_Warp_SelectWarp;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Util_Warp_SelectWarp(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);
        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > 10)
                gTasks[taskId].data[3] = 10;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }

        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectWarp);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        gTasks[taskId].data[7] = gTasks[taskId].data[3];
        //WARP
        SetWarpDestinationToMapWarp(gTasks[taskId].data[5], gTasks[taskId].data[6], gTasks[taskId].data[7]); //If not warp with the number available -> center of map
        DoWarp();
        ResetInitialPlayerAvatarState();
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Util_PoisonMons(u8 taskId)
{
    int i;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, 0)
            && GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG) != SPECIES_NONE
            && GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG) != SPECIES_EGG)
        {
            u32 curStatus = STATUS1_POISON;
            SetMonData(&gPlayerParty[i], MON_DATA_STATUS, &curStatus);
        }
    }
    PlaySE(SE_FIELD_POISON);
}

void CheckSaveBlock1Size(struct ScriptContext *ctx)
{
    u32 currSb1Size = sizeof(struct SaveBlock1);
    u32 maxSb1Size = SECTOR_DATA_SIZE * (SECTOR_ID_SAVEBLOCK1_END - SECTOR_ID_SAVEBLOCK1_START + 1);
    ConvertIntToDecimalStringN(gStringVar1, currSb1Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, maxSb1Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, maxSb1Size - currSb1Size, STR_CONV_MODE_LEFT_ALIGN, 6);
}

void CheckSaveBlock2Size(struct ScriptContext *ctx)
{
    u32 currSb2Size = (sizeof(struct SaveBlock2));
    u32 maxSb2Size = SECTOR_DATA_SIZE;
    ConvertIntToDecimalStringN(gStringVar1, currSb2Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, maxSb2Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, maxSb2Size - currSb2Size, STR_CONV_MODE_LEFT_ALIGN, 6);
}

void CheckPokemonStorageSize(struct ScriptContext *ctx)
{
    u32 currPkmnStorageSize = sizeof(struct PokemonStorage);
    u32 maxPkmnStorageSize = SECTOR_DATA_SIZE * (SECTOR_ID_PKMN_STORAGE_END - SECTOR_ID_PKMN_STORAGE_START + 1);
    ConvertIntToDecimalStringN(gStringVar1, currPkmnStorageSize, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, maxPkmnStorageSize, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, maxPkmnStorageSize - currPkmnStorageSize, STR_CONV_MODE_LEFT_ALIGN, 6);
}

static void DebugAction_Util_CheckSaveBlock(u8 taskId)
{
    Debug_DestroyMenu_Full_Script(taskId, Debug_CheckSaveBlock);
}

static const u8 sWeatherNames[22][24] = {
    [WEATHER_NONE]               = _("NONE"),
    [WEATHER_SUNNY_CLOUDS]       = _("SUNNY CLOUDS"),
    [WEATHER_SUNNY]              = _("SUNNY"),
    [WEATHER_RAIN]               = _("RAIN"),
    [WEATHER_SNOW]               = _("SNOW"),
    [WEATHER_RAIN_THUNDERSTORM]  = _("RAIN THUNDERSTORM"),
    [WEATHER_FOG_HORIZONTAL]     = _("FOG HORIZONTAL"),
    [WEATHER_VOLCANIC_ASH]       = _("VOLCANIC ASH"),
    [WEATHER_SANDSTORM]          = _("SANDSTORM"),
    [WEATHER_FOG_DIAGONAL]       = _("FOG DIAGONAL"),
    [WEATHER_UNDERWATER]         = _("UNDERWATER"),
    [WEATHER_SHADE]              = _("SHADE"),
    [WEATHER_DROUGHT]            = _("DROUGHT"),
    [WEATHER_DOWNPOUR]           = _("DOWNPOUR"),
    [WEATHER_UNDERWATER_BUBBLES] = _("UNDERWATER BUBBLES"),
    [WEATHER_ABNORMAL]           = _("ABNORMAL(NOT WORKING)"),
    [WEATHER_ROUTE119_CYCLE]     = _("ROUTE119 CYCLE"),
    [WEATHER_ROUTE123_CYCLE]     = _("ROUTE123 CYCLE"),
};
static const u8 sText_WeatherNotDefined[] = _("NOT DEFINED!!!");
static void DebugAction_Util_Weather(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateWeather);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar1, sWeatherNames[0], CHAR_SPACE, 30);
    StringExpandPlaceholders(gStringVar4, sDebugText_Util_Weather_ID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Util_Weather_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 0;            //Current ID
    gTasks[taskId].data[4] = 0;            //Digit Selected
}
static void DebugAction_Util_Weather_SelectId(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > WEATHER_ROUTE123_CYCLE)
                gTasks[taskId].data[3] = WEATHER_ROUTE123_CYCLE;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < WEATHER_NONE)
                gTasks[taskId].data[3] = WEATHER_NONE;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);

        if (gTasks[taskId].data[3] <= 15 || gTasks[taskId].data[3] >= 20)
            StringCopyPadded(gStringVar1, sWeatherNames[gTasks[taskId].data[3]], CHAR_SPACE, 30);
        else
            StringCopyPadded(gStringVar1, sText_WeatherNotDefined, CHAR_SPACE, 30); 

        StringExpandPlaceholders(gStringVar4, sDebugText_Util_Weather_ID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        if (gTasks[taskId].data[3] <= 14 || gTasks[taskId].data[3] >= 20)
        {
            gTasks[taskId].data[5] = gTasks[taskId].data[3];
            SetWeather(gTasks[taskId].data[5]);
        }
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Util_CheckWallClock(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(PlayersHouse_2F_EventScript_CheckWallClock);
}
static void DebugAction_Util_SetWallClock(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(PlayersHouse_2F_EventScript_SetWallClock);
}
static void DebugAction_Util_WatchCredits(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    Debug_DestroyMenu_Full(taskId);
    SetMainCallback2(CB2_StartCreditsSequence);
}
static void DebugAction_Util_Trainer_Name(u8 taskId)
{
    NewGameBirchSpeech_SetDefaultPlayerName(Random() % 20);
    DoNamingScreen(0, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnToFieldContinueScript);
}
static void DebugAction_Util_Trainer_Gender(u8 taskId)
{
    if (gSaveBlock2Ptr->playerGender == 0) // 0 Male, 1 Female
        gSaveBlock2Ptr->playerGender = 1;
    else
        gSaveBlock2Ptr->playerGender = 0;
    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}
static void DebugAction_Util_Trainer_Id(u8 taskId)
{
    u32 trainerId = ((Random() << 16) | Random());
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_Util_CheatStart(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_CheatStart);
}

// *******************************
// Actions Scripts
static void DebugAction_Util_Script_1(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_1);
}
static void DebugAction_Util_Script_2(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_2);
}
static void DebugAction_Util_Script_3(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_3);
}
static void DebugAction_Util_Script_4(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_4);
}
static void DebugAction_Util_Script_5(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_5);
}
static void DebugAction_Util_Script_6(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_6);
}
static void DebugAction_Util_Script_7(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_7);
}
static void DebugAction_Util_Script_8(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_8);
}

// *******************************
// Actions Flags and Vars
static void DebugAction_FlagsVars_Flags(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial Flag
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_FLAGS);
    ConvertIntToHexStringN(gStringVar2, 1, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_FlagHex);
    if (FlagGet(FLAG_TEMP_1))
        StringCopyPadded(gStringVar2, sDebugText_True, CHAR_SPACE, 15);
    else
        StringCopyPadded(gStringVar2, sDebugText_False, CHAR_SPACE, 15);
    StringCopy(gStringVar3, gText_DigitIndicator[0]);
    StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_Flag);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_FlagsVars_FlagsSelect;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = FLAG_TEMP_1; //Current Flag
    gTasks[taskId].data[4] = 0;           //Digit Selected
}
static void DebugAction_FlagsVars_FlagsSelect(u8 taskId)
{
    if (gMain.newKeys & A_BUTTON)
        FlagToggle(gTasks[taskId].data[3]);
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }

    if (gMain.newKeys & DPAD_UP)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[3] >= FLAGS_COUNT){
            gTasks[taskId].data[3] = FLAGS_COUNT - 1;
        }
    }
    if (gMain.newKeys & DPAD_DOWN)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[3] < 1){
            gTasks[taskId].data[3] = 1;
        }
    }
    if (gMain.newKeys & DPAD_LEFT)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[4] -= 1;
        if (gTasks[taskId].data[4] < 0)
        {
            gTasks[taskId].data[4] = 0;
        }
    }
    if (gMain.newKeys & DPAD_RIGHT)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[4] += 1;
        if (gTasks[taskId].data[4] > DEBUG_NUMBER_DIGITS_FLAGS-1)
        {
            gTasks[taskId].data[4] = DEBUG_NUMBER_DIGITS_FLAGS-1;
        }
    }

    if (gMain.newKeys & DPAD_ANY || gMain.newKeys & A_BUTTON)
    {
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_FLAGS);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].data[3], STR_CONV_MODE_LEFT_ALIGN, 3);
        StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_FlagHex);
        if (FlagGet(gTasks[taskId].data[3]) == TRUE)
            StringCopyPadded(gStringVar2, sDebugText_True, CHAR_SPACE, 15);
        else
            StringCopyPadded(gStringVar2, sDebugText_False, CHAR_SPACE, 15);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_Flag);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }
}

static void DebugAction_FlagsVars_Vars(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial Variable
    ConvertIntToDecimalStringN(gStringVar1, VARS_START, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
    ConvertIntToHexStringN(gStringVar2, VARS_START, STR_CONV_MODE_LEFT_ALIGN, 4);
    StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_VariableHex);
    ConvertIntToDecimalStringN(gStringVar3, 0, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
    StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_Variable);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_FlagsVars_Select;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = VARS_START;            //Current Variable
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[5] = 0;            //Current Variable VALUE
}

static void DebugAction_FlagsVars_Select(u8 taskId)
{
    if (gMain.newKeys & DPAD_UP)
    {
        gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[3] > VARS_END){
            gTasks[taskId].data[3] = VARS_END;
        }
    }
    if (gMain.newKeys & DPAD_DOWN)
    {
        gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[3] < VARS_START){
            gTasks[taskId].data[3] = VARS_START;
        }
    }
    if (gMain.newKeys & DPAD_LEFT)
    {
        gTasks[taskId].data[4] -= 1;
        if (gTasks[taskId].data[4] < 0)
        {
            gTasks[taskId].data[4] = 0;
        }
    }
    if (gMain.newKeys & DPAD_RIGHT)
    {
        gTasks[taskId].data[4] += 1;
        if (gTasks[taskId].data[4] > DEBUG_NUMBER_DIGITS_VARIABLES-1)
        {
            gTasks[taskId].data[4] = DEBUG_NUMBER_DIGITS_VARIABLES-1;
        }
    }

    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].data[3], STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_VariableHex);
        if (VarGetIfExist(gTasks[taskId].data[3]) == 65535) //Current value, if 65535 the value hasnt been set
            gTasks[taskId].data[5] = 0;
        else
            gTasks[taskId].data[5] = VarGet(gTasks[taskId].data[3]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[5], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]); //Current digit

        //Combine str's to full window string
        StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_Variable);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        gTasks[taskId].data[4] = 0;

        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].data[3], STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_VariableHex);
        if (VarGetIfExist(gTasks[taskId].data[3]) == 65535) //Current value if 65535 the value hasnt been set
            gTasks[taskId].data[5] = 0;
        else
            gTasks[taskId].data[5] = VarGet(gTasks[taskId].data[3]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[5], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]); //Current digit
        StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_VariableValueSet);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].data[6] = gTasks[taskId].data[5]; //New value selector
        gTasks[taskId].func = DebugAction_FlagsVars_SetValue;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }
}
static void DebugAction_FlagsVars_SetValue(u8 taskId)
{
    if (gMain.newKeys & DPAD_UP)
    {
        if (gTasks[taskId].data[6] + sPowersOfTen[gTasks[taskId].data[4]] <= 32000)
            gTasks[taskId].data[6] += sPowersOfTen[gTasks[taskId].data[4]];
        else
            gTasks[taskId].data[6] = 32000-1;
        if (gTasks[taskId].data[6] >= 32000){
            gTasks[taskId].data[6] = 32000-1;
        }
    }
    if (gMain.newKeys & DPAD_DOWN)
    {
        gTasks[taskId].data[6] -= sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[6] < 0){
            gTasks[taskId].data[6] = 0;
        }
    }
    if (gMain.newKeys & DPAD_LEFT)
    {
        gTasks[taskId].data[4] -= 1;
        if (gTasks[taskId].data[4] < 0)
        {
            gTasks[taskId].data[4] = 0;
        }
    }
    if (gMain.newKeys & DPAD_RIGHT)
    {
        gTasks[taskId].data[4] += 1;
        if (gTasks[taskId].data[4] > 4)
        {
            gTasks[taskId].data[4] = 4;
        }
    }

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        VarSet(gTasks[taskId].data[3], gTasks[taskId].data[6]);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }

    if (gMain.newKeys & DPAD_ANY || gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].data[3], STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_VariableHex);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[6], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]); //Current digit
        StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_VariableValueSet);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }
}

static void DebugAction_FlagsVars_PokedexFlags_All(u8 taskId)
{
    u16 i;
    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        GetSetPokedexFlag(i + 1, FLAG_SET_CAUGHT);
        GetSetPokedexFlag(i + 1, FLAG_SET_SEEN);
    }
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_FlagsVars_PokedexFlags_Reset(u8 taskId)
{
    int boxId, boxPosition, partyId;
    u16 species;

    // Reset Pokedex to emtpy
    memset(&gSaveBlock2Ptr->pokedex.owned, 0, sizeof(gSaveBlock2Ptr->pokedex.owned));
    memset(&gSaveBlock2Ptr->pokedex.seen, 0, sizeof(gSaveBlock2Ptr->pokedex.seen));

    // Add party Pokemon to Pokedex
    for (partyId = 0; partyId < PARTY_SIZE; partyId++)
    {
        if (GetMonData(&gPlayerParty[partyId], MON_DATA_SANITY_HAS_SPECIES))
        {
            species = GetMonData(&gPlayerParty[partyId], MON_DATA_SPECIES);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_SEEN);
        }
    }

    // Add box Pokemon to Pokedex
    for (boxId = 0; boxId < TOTAL_BOXES_COUNT; boxId++)
    {
        for (boxPosition = 0; boxPosition < IN_BOX_COUNT; boxPosition++)
        {
            if (GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SANITY_HAS_SPECIES))
            {
                species = GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SPECIES);
                GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT);
                GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_SEEN);
            }
        }
    }
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_FlagsVars_PokedexFlags_Seen(u8 taskId)
{
    u16 i;
    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        GetSetPokedexFlag(i + 1, FLAG_SET_SEEN);
    }
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_FlagsVars_SwitchDex(u8 taskId)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_POKEDEX_GET);
}
static void DebugAction_FlagsVars_SwitchNatDex(u8 taskId)
{
    if (IsNationalPokedexEnabled())
    {
        DisableNationalPokedex();
        PlaySE(SE_PC_OFF);
    }else{
        EnableNationalPokedex();
        PlaySE(SE_PC_LOGIN);
    }
}
static void DebugAction_FlagsVars_SwitchPokeNav(u8 taskId)
{
    if (FlagGet(FLAG_SYS_POKENAV_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_POKENAV_GET);
}
static void DebugAction_FlagsVars_RunningShoes(u8 taskId)
{
    if (FlagGet(FLAG_SYS_B_DASH))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_B_DASH);
}
static void DebugAction_FlagsVars_ToggleFlyFlags(u8 taskId)
{
    // Sound effect
    if (FlagGet(FLAG_LANDMARK_BATTLE_FRONTIER))
    {
        PlaySE(SE_PC_OFF);

        FlagClear(FLAG_VISITED_LITTLEROOT_TOWN);
        FlagClear(FLAG_VISITED_OLDALE_TOWN);
        FlagClear(FLAG_VISITED_DEWFORD_TOWN);
        FlagClear(FLAG_VISITED_LAVARIDGE_TOWN);
        FlagClear(FLAG_VISITED_FALLARBOR_TOWN);
        FlagClear(FLAG_VISITED_VERDANTURF_TOWN);
        FlagClear(FLAG_VISITED_PACIFIDLOG_TOWN);
        FlagClear(FLAG_VISITED_PETALBURG_CITY);
        FlagClear(FLAG_VISITED_SLATEPORT_CITY);
        FlagClear(FLAG_VISITED_MAUVILLE_CITY);
        FlagClear(FLAG_VISITED_RUSTBORO_CITY);
        FlagClear(FLAG_VISITED_FORTREE_CITY);
        FlagClear(FLAG_VISITED_LILYCOVE_CITY);
        FlagClear(FLAG_VISITED_MOSSDEEP_CITY);
        FlagClear(FLAG_VISITED_SOOTOPOLIS_CITY);
        FlagClear(FLAG_VISITED_EVER_GRANDE_CITY);
        FlagClear(FLAG_LANDMARK_POKEMON_LEAGUE);
        FlagClear(FLAG_LANDMARK_BATTLE_FRONTIER);
    }
    else
    {
        PlaySE(SE_PC_LOGIN);
    
        FlagSet(FLAG_VISITED_LITTLEROOT_TOWN);
        FlagSet(FLAG_VISITED_OLDALE_TOWN);
        FlagSet(FLAG_VISITED_DEWFORD_TOWN);
        FlagSet(FLAG_VISITED_LAVARIDGE_TOWN);
        FlagSet(FLAG_VISITED_FALLARBOR_TOWN);
        FlagSet(FLAG_VISITED_VERDANTURF_TOWN);
        FlagSet(FLAG_VISITED_PACIFIDLOG_TOWN);
        FlagSet(FLAG_VISITED_PETALBURG_CITY);
        FlagSet(FLAG_VISITED_SLATEPORT_CITY);
        FlagSet(FLAG_VISITED_MAUVILLE_CITY);
        FlagSet(FLAG_VISITED_RUSTBORO_CITY);
        FlagSet(FLAG_VISITED_FORTREE_CITY);
        FlagSet(FLAG_VISITED_LILYCOVE_CITY);
        FlagSet(FLAG_VISITED_MOSSDEEP_CITY);
        FlagSet(FLAG_VISITED_SOOTOPOLIS_CITY);
        FlagSet(FLAG_VISITED_EVER_GRANDE_CITY);
        FlagSet(FLAG_LANDMARK_POKEMON_LEAGUE);
        FlagSet(FLAG_LANDMARK_BATTLE_FRONTIER);
    }
}
static void DebugAction_FlagsVars_ToggleBadgeFlags(u8 taskId)
{
    // Sound effect
    if (FlagGet(FLAG_BADGE08_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_BADGE01_GET);
    FlagToggle(FLAG_BADGE02_GET);
    FlagToggle(FLAG_BADGE03_GET);
    FlagToggle(FLAG_BADGE04_GET);
    FlagToggle(FLAG_BADGE05_GET);
    FlagToggle(FLAG_BADGE06_GET);
    FlagToggle(FLAG_BADGE07_GET);
    FlagToggle(FLAG_BADGE08_GET);
}
static void DebugAction_FlagsVars_ToggleFrontierPass(u8 taskId)
{
    // Sound effect
    if (FlagGet(FLAG_SYS_FRONTIER_PASS))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_FRONTIER_PASS);
}
static void DebugAction_FlagsVars_BattleDmgOnOff(u8 taskId)
{
    if (FlagGet(FLAG_SYS_NO_BATTLE_DMG))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_NO_BATTLE_DMG);
}
static void DebugAction_FlagsVars_CollisionOnOff(u8 taskId)
{
    if (FlagGet(FLAG_SYS_NO_COLLISION))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_NO_COLLISION);
}
static void DebugAction_FlagsVars_EncounterOnOff(u8 taskId)
{
    if (FlagGet(FLAG_SYS_NO_ENCOUNTER))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_NO_ENCOUNTER);
}
static void DebugAction_FlagsVars_TrainerSeeOnOff(u8 taskId)
{
    if (FlagGet(FLAG_SYS_NO_TRAINER_SEE))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_NO_TRAINER_SEE);
}
static void DebugAction_FlagsVars_BagUseOnOff(u8 taskId)
{
    if (FlagGet(FLAG_SYS_NO_BAG_USE))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_NO_BAG_USE);
}
static void DebugAction_FlagsVars_CatchingOnOff(u8 taskId)
{
    if (FlagGet(FLAG_SYS_NO_CATCHING))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_NO_CATCHING);
}

// *******************************
// Actions Give
#define ITEM_TAG 0xFDF3
static void DebugAction_Give_Item(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    CopyItemName(1, gStringVar1);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, sDebugText_ItemID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Give_Item_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 1;            //Current ID
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[6] = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gTasks[taskId].data[3]);
    gSprites[gTasks[taskId].data[6]].x2 = DEBUG_NUMBER_ICON_X+10;
    gSprites[gTasks[taskId].data[6]].y2 = DEBUG_NUMBER_ICON_Y+10;
    gSprites[gTasks[taskId].data[6]].oam.priority = 0;
}
static void DebugAction_Give_Item_SelectId(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] >= ITEMS_COUNT)
                gTasks[taskId].data[3] = ITEMS_COUNT - 1;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < DEBUG_NUMBER_DIGITS_ITEMS-1)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        CopyItemName(gTasks[taskId].data[3], gStringVar1);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_ItemID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        FreeSpriteTilesByTag(ITEM_TAG);                         //Destroy item icon
        FreeSpritePaletteByTag(ITEM_TAG);                       //Destroy item icon
        FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[6]]); //Destroy item icon
        DestroySprite(&gSprites[gTasks[taskId].data[6]]);       //Destroy item icon
        gTasks[taskId].data[6] = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gTasks[taskId].data[3]);
        gSprites[gTasks[taskId].data[6]].x2 = DEBUG_NUMBER_ICON_X+10;
        gSprites[gTasks[taskId].data[6]].y2 = DEBUG_NUMBER_ICON_Y+10;
        gSprites[gTasks[taskId].data[6]].oam.priority = 0;
    }

    if (gMain.newKeys & A_BUTTON)
    {
        gTasks[taskId].data[5] = gTasks[taskId].data[3];
        gTasks[taskId].data[3] = 1;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEM_QUANTITY);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_ItemQuantity);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Item_SelectQuantity;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        FreeSpriteTilesByTag(ITEM_TAG);                         //Destroy item icon
        FreeSpritePaletteByTag(ITEM_TAG);                       //Destroy item icon
        FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[6]]); //Destroy item icon
        DestroySprite(&gSprites[gTasks[taskId].data[6]]);       //Destroy item icon

        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Item_SelectQuantity(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] >= 100)
                gTasks[taskId].data[3] = 99;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEM_QUANTITY);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_ItemQuantity);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        FreeSpriteTilesByTag(ITEM_TAG);                         //Destroy item icon
        FreeSpritePaletteByTag(ITEM_TAG);                       //Destroy item icon
        FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[6]]); //Destroy item icon
        DestroySprite(&gSprites[gTasks[taskId].data[6]]);       //Destroy item icon

        PlaySE(MUS_OBTAIN_ITEM);
        AddBagItem(gTasks[taskId].data[5], gTasks[taskId].data[3]);
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        FreeSpriteTilesByTag(ITEM_TAG);                         //Destroy item icon
        FreeSpritePaletteByTag(ITEM_TAG);                       //Destroy item icon
        FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[6]]); //Destroy item icon
        DestroySprite(&gSprites[gTasks[taskId].data[6]]);       //Destroy item icon

        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

//TMs
static void DebugAction_Give_AllTMs(u8 taskId)
{
    u16 i;
    PlayFanfare(MUS_OBTAIN_TMHM);
    for (i = ITEM_TM01; i <= ITEM_TM50; i++)
        if (!CheckBagHasItem(i, 1))
            AddBagItem(i, 1);
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

//Pokemon
static void ResetMonDataStruct(struct DebugMonData *sDebugMonData)
{
    sDebugMonData->mon_speciesId    = 1;
    sDebugMonData->mon_level        = 1;
    sDebugMonData->isShiny          = 0;
    sDebugMonData->mon_natureId     = 0;
    sDebugMonData->mon_abilityNum   = 0;
    sDebugMonData->mon_iv_hp        = 0;
    sDebugMonData->mon_iv_atk       = 0;
    sDebugMonData->mon_iv_def       = 0;
    sDebugMonData->mon_iv_speed     = 0;
    sDebugMonData->mon_iv_satk      = 0;
    sDebugMonData->mon_iv_sdef      = 0;
}
static void DebugAction_Give_PokemonSimple(u8 taskId)
{
    u8 windowId;

    //Mon data struct
    sDebugMonData = AllocZeroed(sizeof(struct DebugMonData));
    ResetMonDataStruct(sDebugMonData);

    //Window initialization
    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringCopy(gStringVar1, gSpeciesNames[1]);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, sDebugText_PokemonID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    //Set task data
    gTasks[taskId].func = DebugAction_Give_Pokemon_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 1;            //Current ID
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[5] = 0;            //Complex?
    FreeMonIconPalettes();                 //Free space for new pallete
    LoadMonIconPalette(gTasks[taskId].data[3]); //Loads pallete for current mon
    #ifndef POKEMON_EXPANSION
        gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0, TRUE); //Create pokemon sprite
    #endif
    #ifdef POKEMON_EXPANSION
        gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0); //Create pokemon sprite
    #endif
    gSprites[gTasks[taskId].data[6]].oam.priority = 0; //Mon Icon ID
}
static void DebugAction_Give_PokemonComplex(u8 taskId)
{
    u8 windowId;

    //Mon data struct
    sDebugMonData = AllocZeroed(sizeof(struct DebugMonData));
    ResetMonDataStruct(sDebugMonData);

    //Window initialization
    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, 4);
    StringCopy(gStringVar1, gSpeciesNames[1]);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, sDebugText_PokemonID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);


    gTasks[taskId].func = DebugAction_Give_Pokemon_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 1;            //Current ID
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[5] = 1;            //Complex?
    FreeMonIconPalettes();                 //Free space for new palletes
    LoadMonIconPalette(gTasks[taskId].data[3]); //Loads pallete for current mon
    #ifndef POKEMON_EXPANSION
        gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0, TRUE); //Create pokemon sprite
    #endif
    #ifdef POKEMON_EXPANSION
        gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0); //Create pokemon sprite
    #endif
    gSprites[gTasks[taskId].data[6]].oam.priority = 0; //Mon Icon ID
    gTasks[taskId].data[7] = 0;             //iterator
}

static void DebugAction_Give_Pokemon_SelectId(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > SPECIES_CELEBI && gTasks[taskId].data[3] < SPECIES_TREECKO)
                gTasks[taskId].data[3] = SPECIES_TREECKO;
            if (gTasks[taskId].data[3] >= NUM_SPECIES)
                gTasks[taskId].data[3] = NUM_SPECIES - 1;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < SPECIES_TREECKO && gTasks[taskId].data[3] > SPECIES_CELEBI)
                gTasks[taskId].data[3] = SPECIES_CELEBI;
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < DEBUG_NUMBER_DIGITS_ITEMS-1)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringCopy(gStringVar1, gSpeciesNames[gTasks[taskId].data[3]]); //CopyItemName(gTasks[taskId].data[3], gStringVar1);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 4);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].data[6]]);
        FreeMonIconPalettes(); //Free space for new pallete
        LoadMonIconPalette(gTasks[taskId].data[3]); //Loads pallete for current mon
        #ifndef POKEMON_EXPANSION
            gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0, TRUE); //Create pokemon sprite
        #endif
        #ifdef POKEMON_EXPANSION
            gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0); //Create pokemon sprite
        #endif
        gSprites[gTasks[taskId].data[6]].oam.priority = 0;
    }

    if (gMain.newKeys & A_BUTTON)
    {
        sDebugMonData->mon_speciesId = gTasks[taskId].data[3]; //Species ID
        gTasks[taskId].data[3] = 1;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonLevel);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectLevel;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].data[6]]); //Destroy pokemon sprite
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_SelectLevel(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > 100)
                gTasks[taskId].data[3] = 100;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonLevel);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].data[6]]); //Destroy pokemon sprite
        if (gTasks[taskId].data[5] == 0)
        {
            PlaySE(MUS_LEVEL_UP);
            ScriptGiveMon(sDebugMonData->mon_speciesId, gTasks[taskId].data[3], ITEM_NONE, 0,0,0);
            Free(sDebugMonData); //Frees EWRAM of MonData Struct
            DebugAction_DestroyExtraWindow(taskId);
        }
        else
        {
            sDebugMonData->mon_level = gTasks[taskId].data[3]; //Level
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;

            ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 0);
            StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
            StringCopyPadded(gStringVar2, sDebugText_False, CHAR_SPACE, 15);
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonShiny);
            AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

            gTasks[taskId].func = DebugAction_Give_Pokemon_SelectShiny;
        }
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].data[6]]); //Destroy pokemon sprite
        DebugAction_DestroyExtraWindow(taskId);
    }
}
//If complex
static void DebugAction_Give_Pokemon_SelectShiny(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > 1)
                gTasks[taskId].data[3] = 1;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }

        if (gTasks[taskId].data[3] == 1)
            StringCopyPadded(gStringVar2, sDebugText_True, CHAR_SPACE, 15);
        else
            StringCopyPadded(gStringVar2, sDebugText_False, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 0);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonShiny);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        sDebugMonData->isShiny = gTasks[taskId].data[3]; //isShiny
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar1, gNatureNamePointers[0]);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonNature);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectNature;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_SelectNature(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > NUM_NATURES-1)
                gTasks[taskId].data[3] = NUM_NATURES-1;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar1, gNatureNamePointers[gTasks[taskId].data[3]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonNature);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        u8 abilityId;
        sDebugMonData->mon_natureId = gTasks[taskId].data[3]; //NatureId
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        abilityId = GetAbilityBySpecies(sDebugMonData->mon_speciesId, 0);
        StringCopy(gStringVar1, gAbilityNames[abilityId]);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonAbility);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectAbility;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_SelectAbility(u8 taskId)
{
    u8 abilityId;
    u8 abilityCount = 2 - 1; //-1 for proper iteration
    u8 i = 0;
    #ifdef POKEMON_EXPANSION
        abilityCount = NUM_ABILITY_SLOTS - 1;
    #endif

    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > abilityCount)
                gTasks[taskId].data[3] = abilityCount;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }

        while (GetAbilityBySpecies(sDebugMonData->mon_speciesId, gTasks[taskId].data[3] - i) == ABILITY_NONE)
        {
            i++;
        }
        abilityId = GetAbilityBySpecies(sDebugMonData->mon_speciesId, gTasks[taskId].data[3] - i);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar1, gAbilityNames[abilityId]);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonAbility);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        sDebugMonData->mon_abilityNum = gTasks[taskId].data[3] - i; //AbilityNum
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_0);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectIVs;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_SelectIVs(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > 31)
                gTasks[taskId].data[3] = 31;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        switch (gTasks[taskId].data[7])
        {
        case 0:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_0);
            break;
        case 1:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_1);
            break;
        case 2:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_2);
            break;
        case 3:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_3);
            break;
        case 4:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_4);
            break;
        case 5:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_5);
            break;
        }
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    //If A or B button
    if (gMain.newKeys & A_BUTTON)
    {
        switch (gTasks[taskId].data[7])
        {
        case 0:
            sDebugMonData->mon_iv_hp = gTasks[taskId].data[3];
            break;
        case 1:
            sDebugMonData->mon_iv_atk = gTasks[taskId].data[3];
            break;
        case 2:
            sDebugMonData->mon_iv_def = gTasks[taskId].data[3];
            break;
        case 3:
            sDebugMonData->mon_iv_speed = gTasks[taskId].data[3];
            break;
        case 4:
            sDebugMonData->mon_iv_satk = gTasks[taskId].data[3];
            break;
        case 5:
            sDebugMonData->mon_iv_sdef = gTasks[taskId].data[3];
            break;
        }

        //Check if all IVs set
        if (gTasks[taskId].data[7] != 5)
        {
            gTasks[taskId].data[7] += 1;
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;

            StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
            ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
            StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
            switch (gTasks[taskId].data[7])
            {
            case 0:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_0);
                break;
            case 1:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_1);
                break;
            case 2:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_2);
                break;
            case 3:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_3);
                break;
            case 4:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_4);
                break;
            case 5:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_5);
                break;
            }
            AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

            gTasks[taskId].func = DebugAction_Give_Pokemon_SelectIVs;
        }
        else
        {
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;
            gTasks[taskId].data[7] = 0; //Reset iterator

            StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
            StringCopy(gStringVar1, gMoveNames[gTasks[taskId].data[3]]);
            StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
            ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_0);
            AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

            gTasks[taskId].func = DebugAction_Give_Pokemon_Move;
        }
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_Move(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        PlaySE(SE_SELECT);

        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] >= MOVES_COUNT)
                gTasks[taskId].data[3] = MOVES_COUNT - 1;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < 3)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringCopy(gStringVar1, gMoveNames[gTasks[taskId].data[3]]);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
        switch (gTasks[taskId].data[7])
        {
        case 0:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_0);
            break;
        case 1:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_1);
            break;
        case 2:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_2);
            break;
        case 3:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_3);
            break;
        }
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        //If MOVE_NONE selected, stop asking for additional moves
        if (gTasks[taskId].data[3] == 0)
            gTasks[taskId].data[7] = 4;

        //Set current value
        switch (gTasks[taskId].data[7])
        {
        case 0:
            sDebugMonData->mon_move_0 = gTasks[taskId].data[3];
            break;
        case 1:
            sDebugMonData->mon_move_1 = gTasks[taskId].data[3];
            break;
        case 2:
            sDebugMonData->mon_move_2 = gTasks[taskId].data[3];
            break;
        case 3:
            sDebugMonData->mon_move_3 = gTasks[taskId].data[3];
            break;
        }

        //If NOT last move or selected MOVE_NONE ask for next move, else make mon
        if (gTasks[taskId].data[7] < 3)
        {
            gTasks[taskId].data[7] += 1;
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;

            StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
            StringCopy(gStringVar1, gMoveNames[gTasks[taskId].data[3]]);
            StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
            ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
            switch (gTasks[taskId].data[7])
            {
            case 0:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_0);
                break;
            case 1:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_1);
                break;
            case 2:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_2);
                break;
            case 3:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_3);
                break;
            }
            AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

            gTasks[taskId].func = DebugAction_Give_Pokemon_Move;
        }
        else
        {
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;

            PlaySE(MUS_LEVEL_UP);
            gTasks[taskId].func = DebugAction_Give_Pokemon_ComplexCreateMon;
        }
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_ComplexCreateMon(u8 taskId) //https://github.com/ghoulslash/pokeemerald/tree/custom-givemon
{
    u16 nationalDexNum;
    int sentToPc;
    struct Pokemon mon;
    u8 i;
    u16 moves[4];
    u8 IVs[6];
    u8 iv_val;
    u16 species     = sDebugMonData->mon_speciesId;
    u8 level        = sDebugMonData->mon_level;
    u8 isShiny      = sDebugMonData->isShiny; //Shiny: no 0, yes 1
    u8 nature       = sDebugMonData->mon_natureId;
    u8 abilityNum   = sDebugMonData->mon_abilityNum;
    moves[0]        = sDebugMonData->mon_move_0;
    moves[1]        = sDebugMonData->mon_move_1;
    moves[2]        = sDebugMonData->mon_move_2;
    moves[3]        = sDebugMonData->mon_move_3;
    IVs[0]          = sDebugMonData->mon_iv_hp;
    IVs[1]          = sDebugMonData->mon_iv_atk;
    IVs[2]          = sDebugMonData->mon_iv_def;
    IVs[3]          = sDebugMonData->mon_iv_speed;
    IVs[4]          = sDebugMonData->mon_iv_satk;
    IVs[5]          = sDebugMonData->mon_iv_sdef;


    //Nature
    if (nature == NUM_NATURES || nature == 0xFF)
        nature = Random() % NUM_NATURES;

    //Shinyness
    if (isShiny == 1)
    {
        u32 personality;
        u32 otid = gSaveBlock2Ptr->playerTrainerId[0]
            | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
            | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
            | (gSaveBlock2Ptr->playerTrainerId[3] << 24);

        do
        {
            personality = Random32();
            personality = ((((Random() % 8) ^ (HIHALF(otid) ^ LOHALF(otid))) ^ LOHALF(personality)) << 16) | LOHALF(personality);
        } while (nature != GetNatureFromPersonality(personality));

        CreateMon(&mon, species, level, 32, 1, personality, OT_ID_PRESET, otid);
    }
    else
        CreateMonWithNature(&mon, species, level, 32, nature);

    //EVs/IVs
    for (i = 0; i < NUM_STATS; i++)
    {
        // ev
        // if (evs[i] != 0xFF && evTotal < 510)
        // {
        //     // only up to 510 evs
        //     if ((evTotal + evs[i]) > 510)
        //         evs[i] = (510 - evTotal);

        //     evTotal += evs[i];
        //     SetMonData(&mon, MON_DATA_HP_EV + i, &evs[i]);
        // }

        // iv
        iv_val = IVs[i];
        if (iv_val != 32 && iv_val != 0xFF)
            SetMonData(&mon, MON_DATA_HP_IV + i, &iv_val);
    }
    CalculateMonStats(&mon);

    //Moves
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (moves[i] == 0 || moves[i] == 0xFF || moves[i] >= MOVES_COUNT)
            continue;

        SetMonMoveSlot(&mon, moves[i], i);
    }

    //Ability
    if (abilityNum == 0xFF || GetAbilityBySpecies(species, abilityNum) == 0)
    {
        do {
            abilityNum = Random() % 3;  // includes hidden abilities
        } while (GetAbilityBySpecies(species, abilityNum) == 0);
    }

    SetMonData(&mon, MON_DATA_ABILITY_NUM, &abilityNum);

    //ball
    // if (ball <= POKEBALL_COUNT)
    //     SetMonData(&mon, MON_DATA_POKEBALL, &ball);

    //item
    // heldItem[0] = item;
    // heldItem[1] = item >> 8;
    // SetMonData(&mon, MON_DATA_HELD_ITEM, heldItem);

    // give player the mon
    //sentToPc = GiveMonToPlayer(&mon);
    SetMonData(&mon, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
    SetMonData(&mon, MON_DATA_OT_GENDER, &gSaveBlock2Ptr->playerGender);
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            break;
    }

    if (i >= PARTY_SIZE)
        sentToPc = CopyMonToPC(&mon);
    else
    {
        sentToPc = MON_GIVEN_TO_PARTY;
        CopyMon(&gPlayerParty[i], &mon, sizeof(mon));
        gPlayerPartyCount = i + 1;
    }

    //Pokedex entry
    nationalDexNum = SpeciesToNationalPokedexNum(species); 
    switch(sentToPc)
    {
    case MON_GIVEN_TO_PARTY:
    case MON_GIVEN_TO_PC:
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);
        break;
    case MON_CANT_GIVE:
        break;
    }

    Free(sDebugMonData); //Frees EWRAM of MonData Struct
    DebugAction_DestroyExtraWindow(taskId); //return sentToPc;
}

static void DebugAction_Give_MaxMoney(u8 taskId)
{
    SetMoney(&gSaveBlock1Ptr->money, 999999);
}

static void DebugAction_Give_MaxCoins(u8 taskId)
{
    SetCoins(9999);
}

static void DebugAction_Give_MaxBattlePoints(u8 taskId)
{
    gSaveBlock2Ptr->frontier.battlePoints = MAX_BATTLE_FRONTIER_POINTS;
}

static void DebugAction_Give_DayCareEgg(u8 taskId)
{
    TriggerPendingDaycareEgg();
}

// *******************************
// Actions Pokemon Creator
static void DebugAction_PkmCreator_Party_Add(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(0, 0xFF);
}
static void DebugAction_PkmCreator_Party_Edit(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(1, 0xFF);
}
static void DebugAction_PkmCreator_PC_Edit(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(2, 0xFF);
}
static void DebugAction_PkmCreator_Enemy_Party_Edit(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(3, 0xFF);
}
static void DebugAction_PkmCreator_Enemy_Party_Edit_Debug(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(4, 0xFF);
}
static void DebugAction_PkmCreator_Debug_Edit(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(5, 0xFF);
}
static void DebugAction_PkmCreator_Enemy_Party_Add(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(6, 0xFF);
}
static void DebugAction_PkmCreator_Testing(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(7, 0xFF);
}
static void DebugAction_PkmCreator_Testing_Copy(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DebugPkmCreator_Init(8, 0xFF);
}

// *******************************
// Actions Fill
static void DebugAction_Fill_PCBoxes_Fast(u8 taskId) //Credit: Sierraffinity
{
    int boxId, boxPosition;
    u32 personality;
    struct BoxPokemon boxMon;

    personality = Random32();

    CreateBoxMon(&boxMon,
                 SPECIES_BULBASAUR,
                 100,
                 32,
                 personality,
                 0,
                 OT_ID_PLAYER_ID,
                 0);

    for (boxId = 0; boxId < TOTAL_BOXES_COUNT; boxId++)
    {
        for (boxPosition = 0; boxPosition < IN_BOX_COUNT; boxPosition++)
        {
            if (!GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SANITY_HAS_SPECIES))
            {
                gPokemonStoragePtr->boxes[boxId][boxPosition] = boxMon;
            }
        }
    }

    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_Fill_PCBoxes_Slow(u8 taskId)
{
    int boxId, boxPosition;
    u32 personality;
    struct BoxPokemon boxMon;
    u32 i = 1;

    personality = Random32();

    for (boxId = 0; boxId < TOTAL_BOXES_COUNT; boxId++)
    {
        for (boxPosition = 0; boxPosition < IN_BOX_COUNT; boxPosition++)
        {
            if (!GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SANITY_HAS_SPECIES))
            {
                CreateBoxMon(&boxMon,
                    i,
                    100,
                    32,
                    personality,
                    0,
                    OT_ID_PLAYER_ID,
                    0);

            #ifndef POKEMON_EXPANSION
                if (i < SPECIES_CELEBI)
                    i += 1;
                else if (i == SPECIES_CELEBI)
                    i = SPECIES_TREECKO;
                else if (i < SPECIES_CHIMECHO)
                    i += 1;
                else
                    i = 1;
            #else
                if (i < FORMS_START - 1)
                    i += 1;
                else
                    i = 1;
            #endif

                gPokemonStoragePtr->boxes[boxId][boxPosition] = boxMon;
            }
        }
    }

    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_Fill_PCItemStorage(u8 taskId)
{
    u16 itemId;

    for (itemId = 1; itemId < ITEMS_COUNT; itemId++)
    {
        if (!CheckPCHasItem(itemId, MAX_PC_ITEM_CAPACITY))
            AddPCItem(itemId, MAX_PC_ITEM_CAPACITY);
    }
}
static void DebugAction_Fill_PocketItems(u8 taskId)
{
    u16 itemId;

    for (itemId = 1; itemId < ITEMS_COUNT; itemId++)
    {
        if (ItemId_GetPocket(itemId) == POCKET_ITEMS && CheckBagHasSpace(itemId, MAX_BAG_ITEM_CAPACITY))
            AddBagItem(itemId, MAX_BAG_ITEM_CAPACITY);
    }
}
static void DebugAction_Fill_PocketPokeBalls(u8 taskId)
{
    u16 itemId;

    for (itemId = FIRST_BALL; itemId < LAST_BALL; itemId++)
    {
        if (CheckBagHasSpace(itemId, MAX_BAG_ITEM_CAPACITY))
            AddBagItem(itemId, MAX_BAG_ITEM_CAPACITY);
    }
}
static void DebugAction_Fill_PocketTMHM(u8 taskId)
{
    u16 itemId;

    for (itemId = ITEM_TM01; itemId <= ITEM_HM08; itemId++)
    {
        if (CheckBagHasSpace(itemId, 1) && ItemIdToBattleMoveId(itemId) != MOVE_NONE)
            AddBagItem(itemId, 1);
    }
}
static void DebugAction_Fill_PocketBerries(u8 taskId)
{
    u16 itemId;

    for (itemId = FIRST_BERRY_INDEX; itemId < LAST_BERRY_INDEX; itemId++)
    {
        if (CheckBagHasSpace(itemId, MAX_BERRY_CAPACITY))
            AddBagItem(itemId, MAX_BERRY_CAPACITY);
    }
}
static void DebugAction_Fill_PocketKeyItems(u8 taskId)
{
    u16 itemId;

    for (itemId = 1; itemId < ITEMS_COUNT; itemId++)
    {
        if (ItemId_GetPocket(itemId) == POCKET_KEY_ITEMS && CheckBagHasSpace(itemId, 1))
            AddBagItem(itemId, 1);
    }
}

// *******************************
// Actions Sound
static const u8 *const gBGMNames[];
static const u8 *const gSENames[];
static void DebugAction_Sound_SE(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateSound);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringCopyPadded(gStringVar1, gSENames[0], CHAR_SPACE, 35);
    StringExpandPlaceholders(gStringVar4, sDebugText_Sound_SE_ID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    StopMapMusic(); //Stop map music to better hear sounds

    gTasks[taskId].func = DebugAction_Sound_SE_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 1;                         //Current ID
    gTasks[taskId].data[4] = 0;                         //Digit Selected
    gTasks[taskId].data[5] = gTasks[taskId].data[3];    //Last song played (for stopping)
}
static void DebugAction_Sound_SE_SelectId(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > END_SE)
                gTasks[taskId].data[3] = END_SE;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < DEBUG_NUMBER_DIGITS_ITEMS-1)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringCopyPadded(gStringVar1, gSENames[gTasks[taskId].data[3]-1], CHAR_SPACE, 35);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_Sound_SE_ID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        m4aSongNumStop(gTasks[taskId].data[5]);
        gTasks[taskId].data[5] = gTasks[taskId].data[3];
        m4aSongNumStart(gTasks[taskId].data[3]);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        m4aSongNumStop(gTasks[taskId].data[5]);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Sound_MUS(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateSound);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, MUS_LITTLEROOT_TEST, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringCopyPadded(gStringVar1, gBGMNames[0], CHAR_SPACE, 35);
    StringExpandPlaceholders(gStringVar4, sDebugText_Sound_MUS_ID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    StopMapMusic(); //Stop map music to better hear new music

    gTasks[taskId].func = DebugAction_Sound_MUS_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = MUS_LITTLEROOT_TEST;                 //Current ID
    gTasks[taskId].data[4] = 0;                         //Digit Selected
    gTasks[taskId].data[5] = gTasks[taskId].data[3];    //Last song played (for stopping)
}
static void DebugAction_Sound_MUS_SelectId(u8 taskId)
{
    if (gMain.newKeys & DPAD_ANY)
    {
        if (gMain.newKeys & DPAD_UP)
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > END_MUS)
                gTasks[taskId].data[3] = END_MUS;
        }
        if (gMain.newKeys & DPAD_DOWN)
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < MUS_LITTLEROOT_TEST)
                gTasks[taskId].data[3] = MUS_LITTLEROOT_TEST;
        }
        if (gMain.newKeys & DPAD_LEFT)
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (gMain.newKeys & DPAD_RIGHT)
        {
            if (gTasks[taskId].data[4] < DEBUG_NUMBER_DIGITS_ITEMS-1)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringCopyPadded(gStringVar1, gBGMNames[gTasks[taskId].data[3]-MUS_LITTLEROOT_TEST], CHAR_SPACE, 35);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_Sound_MUS_ID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        m4aSongNumStop(gTasks[taskId].data[5]);
        gTasks[taskId].data[5] = gTasks[taskId].data[3];
        m4aSongNumStart(gTasks[taskId].data[3]);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        // m4aSongNumStop(gTasks[taskId].data[5]);   //Uncomment if music should stop after leaving menu
        DebugAction_DestroyExtraWindow(taskId);
    }
}

#define SOUND_LIST_BGM \
    X(MUS_LITTLEROOT_TEST             , "MUS-LITTLEROOT-TEST"             ) \
    X(MUS_GSC_ROUTE38                 , "MUS-GSC-ROUTE38"                 ) \
    X(MUS_CAUGHT                      , "MUS-CAUGHT"                      ) \
    X(MUS_VICTORY_WILD                , "MUS-VICTORY-WILD"                ) \
    X(MUS_VICTORY_GYM_LEADER          , "MUS-VICTORY-GYM-LEADER"          ) \
    X(MUS_VICTORY_LEAGUE              , "MUS-VICTORY-LEAGUE"              ) \
    X(MUS_C_COMM_CENTER               , "MUS-C-COMM-CENTER"               ) \
    X(MUS_GSC_PEWTER                  , "MUS-GSC-PEWTER"                  ) \
    X(MUS_C_VS_LEGEND_BEAST           , "MUS-C-VS-LEGEND-BEAST"           ) \
    X(MUS_ROUTE101                    , "MUS-ROUTE101"                    ) \
    X(MUS_ROUTE110                    , "MUS-ROUTE110"                    ) \
    X(MUS_ROUTE120                    , "MUS-ROUTE120"                    ) \
    X(MUS_PETALBURG                   , "MUS-PETALBURG"                   ) \
    X(MUS_OLDALE                      , "MUS-OLDALE"                      ) \
    X(MUS_GYM                         , "MUS-GYM"                         ) \
    X(MUS_SURF                        , "MUS-SURF"                        ) \
    X(MUS_PETALBURG_WOODS             , "MUS-PETALBURG-WOODS"             ) \
    X(MUS_LEVEL_UP                    , "MUS-LEVEL-UP"                    ) \
    X(MUS_HEAL                        , "MUS-HEAL"                        ) \
    X(MUS_OBTAIN_BADGE                , "MUS-OBTAIN-BADGE"                ) \
    X(MUS_OBTAIN_ITEM                 , "MUS-OBTAIN-ITEM"                 ) \
    X(MUS_EVOLVED                     , "MUS-EVOLVED"                     ) \
    X(MUS_OBTAIN_TMHM                 , "MUS-OBTAIN-TMHM"                 ) \
    X(MUS_LILYCOVE_MUSEUM             , "MUS-LILYCOVE-MUSEUM"             ) \
    X(MUS_ROUTE122                    , "MUS-ROUTE122"                    ) \
    X(MUS_OCEANIC_MUSEUM              , "MUS-OCEANIC-MUSEUM"              ) \
    X(MUS_EVOLUTION_INTRO             , "MUS-EVOLUTION-INTRO"             ) \
    X(MUS_EVOLUTION                   , "MUS-EVOLUTION"                   ) \
    X(MUS_MOVE_DELETED                , "MUS-MOVE-DELETED"                ) \
    X(MUS_ENCOUNTER_GIRL              , "MUS-ENCOUNTER-GIRL"              ) \
    X(MUS_ENCOUNTER_MALE              , "MUS-ENCOUNTER-MALE"              ) \
    X(MUS_ABANDONED_SHIP              , "MUS-ABANDONED-SHIP"              ) \
    X(MUS_FORTREE                     , "MUS-FORTREE"                     ) \
    X(MUS_BIRCH_LAB                   , "MUS-BIRCH-LAB"                   ) \
    X(MUS_B_TOWER_RS                  , "MUS-B-TOWER-RS"                  ) \
    X(MUS_ENCOUNTER_SWIMMER           , "MUS-ENCOUNTER-SWIMMER"           ) \
    X(MUS_CAVE_OF_ORIGIN              , "MUS-CAVE-OF-ORIGIN"              ) \
    X(MUS_OBTAIN_BERRY                , "MUS-OBTAIN-BERRY"                ) \
    X(MUS_AWAKEN_LEGEND               , "MUS-AWAKEN-LEGEND"               ) \
    X(MUS_SLOTS_JACKPOT               , "MUS-SLOTS-JACKPOT"               ) \
    X(MUS_SLOTS_WIN                   , "MUS-SLOTS-WIN"                   ) \
    X(MUS_TOO_BAD                     , "MUS-TOO-BAD"                     ) \
    X(MUS_ROULETTE                    , "MUS-ROULETTE"                    ) \
    X(MUS_LINK_CONTEST_P1             , "MUS-LINK-CONTEST-P1"             ) \
    X(MUS_LINK_CONTEST_P2             , "MUS-LINK-CONTEST-P2"             ) \
    X(MUS_LINK_CONTEST_P3             , "MUS-LINK-CONTEST-P3"             ) \
    X(MUS_LINK_CONTEST_P4             , "MUS-LINK-CONTEST-P4"             ) \
    X(MUS_ENCOUNTER_RICH              , "MUS-ENCOUNTER-RICH"              ) \
    X(MUS_VERDANTURF                  , "MUS-VERDANTURF"                  ) \
    X(MUS_RUSTBORO                    , "MUS-RUSTBORO"                    ) \
    X(MUS_POKE_CENTER                 , "MUS-POKE-CENTER"                 ) \
    X(MUS_ROUTE104                    , "MUS-ROUTE104"                    ) \
    X(MUS_ROUTE119                    , "MUS-ROUTE119"                    ) \
    X(MUS_CYCLING                     , "MUS-CYCLING"                     ) \
    X(MUS_POKE_MART                   , "MUS-POKE-MART"                   ) \
    X(MUS_LITTLEROOT                  , "MUS-LITTLEROOT"                  ) \
    X(MUS_MT_CHIMNEY                  , "MUS-MT-CHIMNEY"                  ) \
    X(MUS_ENCOUNTER_FEMALE            , "MUS-ENCOUNTER-FEMALE"            ) \
    X(MUS_LILYCOVE                    , "MUS-LILYCOVE"                    ) \
    X(MUS_ROUTE111                    , "MUS-ROUTE111"                    ) \
    X(MUS_HELP                        , "MUS-HELP"                        ) \
    X(MUS_UNDERWATER                  , "MUS-UNDERWATER"                  ) \
    X(MUS_VICTORY_TRAINER             , "MUS-VICTORY-TRAINER"             ) \
    X(MUS_TITLE                       , "MUS-TITLE"                       ) \
    X(MUS_INTRO                       , "MUS-INTRO"                       ) \
    X(MUS_ENCOUNTER_MAY               , "MUS-ENCOUNTER-MAY"               ) \
    X(MUS_ENCOUNTER_INTENSE           , "MUS-ENCOUNTER-INTENSE"           ) \
    X(MUS_ENCOUNTER_COOL              , "MUS-ENCOUNTER-COOL"              ) \
    X(MUS_ROUTE113                    , "MUS-ROUTE113"                    ) \
    X(MUS_ENCOUNTER_AQUA              , "MUS-ENCOUNTER-AQUA"              ) \
    X(MUS_FOLLOW_ME                   , "MUS-FOLLOW-ME"                   ) \
    X(MUS_ENCOUNTER_BRENDAN           , "MUS-ENCOUNTER-BRENDAN"           ) \
    X(MUS_EVER_GRANDE                 , "MUS-EVER-GRANDE"                 ) \
    X(MUS_ENCOUNTER_SUSPICIOUS        , "MUS-ENCOUNTER-SUSPICIOUS"        ) \
    X(MUS_VICTORY_AQUA_MAGMA          , "MUS-VICTORY-AQUA-MAGMA"          ) \
    X(MUS_CABLE_CAR                   , "MUS-CABLE-CAR"                   ) \
    X(MUS_GAME_CORNER                 , "MUS-GAME-CORNER"                 ) \
    X(MUS_DEWFORD                     , "MUS-DEWFORD"                     ) \
    X(MUS_SAFARI_ZONE                 , "MUS-SAFARI-ZONE"                 ) \
    X(MUS_VICTORY_ROAD                , "MUS-VICTORY-ROAD"                ) \
    X(MUS_AQUA_MAGMA_HIDEOUT          , "MUS-AQUA-MAGMA-HIDEOUT"          ) \
    X(MUS_SAILING                     , "MUS-SAILING"                     ) \
    X(MUS_MT_PYRE                     , "MUS-MT-PYRE"                     ) \
    X(MUS_SLATEPORT                   , "MUS-SLATEPORT"                   ) \
    X(MUS_MT_PYRE_EXTERIOR            , "MUS-MT-PYRE-EXTERIOR"            ) \
    X(MUS_SCHOOL                      , "MUS-SCHOOL"                      ) \
    X(MUS_HALL_OF_FAME                , "MUS-HALL-OF-FAME"                ) \
    X(MUS_FALLARBOR                   , "MUS-FALLARBOR"                   ) \
    X(MUS_SEALED_CHAMBER              , "MUS-SEALED-CHAMBER"              ) \
    X(MUS_CONTEST_WINNER              , "MUS-CONTEST-WINNER"              ) \
    X(MUS_CONTEST                     , "MUS-CONTEST"                     ) \
    X(MUS_ENCOUNTER_MAGMA             , "MUS-ENCOUNTER-MAGMA"             ) \
    X(MUS_INTRO_BATTLE                , "MUS-INTRO-BATTLE"                ) \
    X(MUS_WEATHER_KYOGRE              , "MUS-WEATHER-KYOGRE"              ) \
    X(MUS_WEATHER_GROUDON             , "MUS-WEATHER-GROUDON"             ) \
    X(MUS_SOOTOPOLIS                  , "MUS-SOOTOPOLIS"                  ) \
    X(MUS_CONTEST_RESULTS             , "MUS-CONTEST-RESULTS"             ) \
    X(MUS_HALL_OF_FAME_ROOM           , "MUS-HALL-OF-FAME-ROOM"           ) \
    X(MUS_TRICK_HOUSE                 , "MUS-TRICK-HOUSE"                 ) \
    X(MUS_ENCOUNTER_TWINS             , "MUS-ENCOUNTER-TWINS"             ) \
    X(MUS_ENCOUNTER_ELITE_FOUR        , "MUS-ENCOUNTER-ELITE-FOUR"        ) \
    X(MUS_ENCOUNTER_HIKER             , "MUS-ENCOUNTER-HIKER"             ) \
    X(MUS_CONTEST_LOBBY               , "MUS-CONTEST-LOBBY"               ) \
    X(MUS_ENCOUNTER_INTERVIEWER       , "MUS-ENCOUNTER-INTERVIEWER"       ) \
    X(MUS_ENCOUNTER_CHAMPION          , "MUS-ENCOUNTER-CHAMPION"          ) \
    X(MUS_CREDITS                     , "MUS-CREDITS"                     ) \
    X(MUS_END                         , "MUS-END"                         ) \
    X(MUS_VS_WILD                     , "MUS-VS-WILD"                     ) \
    X(MUS_VS_AQUA_MAGMA               , "MUS-VS-AQUA-MAGMA"               ) \
    X(MUS_VS_TRAINER                  , "MUS-VS-TRAINER"                  ) \
    X(MUS_VS_GYM_LEADER               , "MUS-VS-GYM-LEADER"               ) \
    X(MUS_VS_CHAMPION                 , "MUS-VS-CHAMPION"                 ) \
    X(MUS_VS_REGI                     , "MUS-VS-REGI"                     ) \
    X(MUS_VS_KYOGRE_GROUDON           , "MUS-VS-KYOGRE-GROUDON"           ) \
    X(MUS_VS_RIVAL                    , "MUS-VS-RIVAL"                    ) \
    X(MUS_VS_ELITE_FOUR               , "MUS-VS-ELITE-FOUR"               ) \
    X(MUS_VS_AQUA_MAGMA_LEADER        , "MUS-VS-AQUA-MAGMA-LEADER"        ) \
    X(MUS_RG_FOLLOW_ME                , "MUS-RG-FOLLOW-ME"                ) \
    X(MUS_RG_GAME_CORNER              , "MUS-RG-GAME-CORNER"              ) \
    X(MUS_RG_ROCKET_HIDEOUT           , "MUS-RG-ROCKET-HIDEOUT"           ) \
    X(MUS_RG_GYM                      , "MUS-RG-GYM"                      ) \
    X(MUS_RG_JIGGLYPUFF               , "MUS-RG-JIGGLYPUFF"               ) \
    X(MUS_RG_INTRO_FIGHT              , "MUS-RG-INTRO-FIGHT"              ) \
    X(MUS_RG_TITLE                    , "MUS-RG-TITLE"                    ) \
    X(MUS_RG_CINNABAR                 , "MUS-RG-CINNABAR"                 ) \
    X(MUS_RG_LAVENDER                 , "MUS-RG-LAVENDER"                 ) \
    X(MUS_RG_HEAL                     , "MUS-RG-HEAL"                     ) \
    X(MUS_RG_CYCLING                  , "MUS-RG-CYCLING"                  ) \
    X(MUS_RG_ENCOUNTER_ROCKET         , "MUS-RG-ENCOUNTER-ROCKET"         ) \
    X(MUS_RG_ENCOUNTER_GIRL           , "MUS-RG-ENCOUNTER-GIRL"           ) \
    X(MUS_RG_ENCOUNTER_BOY            , "MUS-RG-ENCOUNTER-BOY"            ) \
    X(MUS_RG_HALL_OF_FAME             , "MUS-RG-HALL-OF-FAME"             ) \
    X(MUS_RG_VIRIDIAN_FOREST          , "MUS-RG-VIRIDIAN-FOREST"          ) \
    X(MUS_RG_MT_MOON                  , "MUS-RG-MT-MOON"                  ) \
    X(MUS_RG_POKE_MANSION             , "MUS-RG-POKE-MANSION"             ) \
    X(MUS_RG_CREDITS                  , "MUS-RG-CREDITS"                  ) \
    X(MUS_RG_ROUTE1                   , "MUS-RG-ROUTE1"                   ) \
    X(MUS_RG_ROUTE24                  , "MUS-RG-ROUTE24"                  ) \
    X(MUS_RG_ROUTE3                   , "MUS-RG-ROUTE3"                   ) \
    X(MUS_RG_ROUTE11                  , "MUS-RG-ROUTE11"                  ) \
    X(MUS_RG_VICTORY_ROAD             , "MUS-RG-VICTORY-ROAD"             ) \
    X(MUS_RG_VS_GYM_LEADER            , "MUS-RG-VS-GYM-LEADER"            ) \
    X(MUS_RG_VS_TRAINER               , "MUS-RG-VS-TRAINER"               ) \
    X(MUS_RG_VS_WILD                  , "MUS-RG-VS-WILD"                  ) \
    X(MUS_RG_VS_CHAMPION              , "MUS-RG-VS-CHAMPION"              ) \
    X(MUS_RG_PALLET                   , "MUS-RG-PALLET"                   ) \
    X(MUS_RG_OAK_LAB                  , "MUS-RG-OAK-LAB"                  ) \
    X(MUS_RG_OAK                      , "MUS-RG-OAK"                      ) \
    X(MUS_RG_POKE_CENTER              , "MUS-RG-POKE-CENTER"              ) \
    X(MUS_RG_SS_ANNE                  , "MUS-RG-SS-ANNE"                  ) \
    X(MUS_RG_SURF                     , "MUS-RG-SURF"                     ) \
    X(MUS_RG_POKE_TOWER               , "MUS-RG-POKE-TOWER"               ) \
    X(MUS_RG_SILPH                    , "MUS-RG-SILPH"                    ) \
    X(MUS_RG_FUCHSIA                  , "MUS-RG-FUCHSIA"                  ) \
    X(MUS_RG_CELADON                  , "MUS-RG-CELADON"                  ) \
    X(MUS_RG_VICTORY_TRAINER          , "MUS-RG-VICTORY-TRAINER"          ) \
    X(MUS_RG_VICTORY_WILD             , "MUS-RG-VICTORY-WILD"             ) \
    X(MUS_RG_VICTORY_GYM_LEADER       , "MUS-RG-VICTORY-GYM-LEADER"       ) \
    X(MUS_RG_VERMILLION               , "MUS-RG-VERMILLION"               ) \
    X(MUS_RG_PEWTER                   , "MUS-RG-PEWTER"                   ) \
    X(MUS_RG_ENCOUNTER_RIVAL          , "MUS-RG-ENCOUNTER-RIVAL"          ) \
    X(MUS_RG_RIVAL_EXIT               , "MUS-RG-RIVAL-EXIT"               ) \
    X(MUS_RG_DEX_RATING               , "MUS-RG-DEX-RATING"               ) \
    X(MUS_RG_OBTAIN_KEY_ITEM          , "MUS-RG-OBTAIN-KEY-ITEM"          ) \
    X(MUS_RG_CAUGHT_INTRO             , "MUS-RG-CAUGHT-INTRO"             ) \
    X(MUS_RG_PHOTO                    , "MUS-RG-PHOTO"                    ) \
    X(MUS_RG_GAME_FREAK               , "MUS-RG-GAME-FREAK"               ) \
    X(MUS_RG_CAUGHT                   , "MUS-RG-CAUGHT"                   ) \
    X(MUS_RG_NEW_GAME_INSTRUCT        , "MUS-RG-NEW-GAME-INSTRUCT"        ) \
    X(MUS_RG_NEW_GAME_INTRO           , "MUS-RG-NEW-GAME-INTRO"           ) \
    X(MUS_RG_NEW_GAME_EXIT            , "MUS-RG-NEW-GAME-EXIT"            ) \
    X(MUS_RG_POKE_JUMP                , "MUS-RG-POKE-JUMP"                ) \
    X(MUS_RG_UNION_ROOM               , "MUS-RG-UNION-ROOM"               ) \
    X(MUS_RG_NET_CENTER               , "MUS-RG-NET-CENTER"               ) \
    X(MUS_RG_MYSTERY_GIFT             , "MUS-RG-MYSTERY-GIFT"             ) \
    X(MUS_RG_BERRY_PICK               , "MUS-RG-BERRY-PICK"               ) \
    X(MUS_RG_SEVII_CAVE               , "MUS-RG-SEVII-CAVE"               ) \
    X(MUS_RG_TEACHY_TV_SHOW           , "MUS-RG-TEACHY-TV-SHOW"           ) \
    X(MUS_RG_SEVII_ROUTE              , "MUS-RG-SEVII-ROUTE"              ) \
    X(MUS_RG_SEVII_DUNGEON            , "MUS-RG-SEVII-DUNGEON"            ) \
    X(MUS_RG_SEVII_123                , "MUS-RG-SEVII-123"                ) \
    X(MUS_RG_SEVII_45                 , "MUS-RG-SEVII-45"                 ) \
    X(MUS_RG_SEVII_67                 , "MUS-RG-SEVII-67"                 ) \
    X(MUS_RG_POKE_FLUTE               , "MUS-RG-POKE-FLUTE"               ) \
    X(MUS_RG_VS_DEOXYS                , "MUS-RG-VS-DEOXYS"                ) \
    X(MUS_RG_VS_MEWTWO                , "MUS-RG-VS-MEWTWO"                ) \
    X(MUS_RG_VS_LEGEND                , "MUS-RG-VS-LEGEND"                ) \
    X(MUS_RG_ENCOUNTER_GYM_LEADER     , "MUS-RG-ENCOUNTER-GYM-LEADER"     ) \
    X(MUS_RG_ENCOUNTER_DEOXYS         , "MUS-RG-ENCOUNTER-DEOXYS"         ) \
    X(MUS_RG_TRAINER_TOWER            , "MUS-RG-TRAINER-TOWER"            ) \
    X(MUS_RG_SLOW_PALLET              , "MUS-RG-SLOW-PALLET"              ) \
    X(MUS_RG_TEACHY_TV_MENU           , "MUS-RG-TEACHY-TV-MENU"           ) \
    X(MUS_ABNORMAL_WEATHER            , "MUS-ABNORMAL-WEATHER"            ) \
    X(MUS_B_FRONTIER                  , "MUS-B-FRONTIER"                  ) \
    X(MUS_B_ARENA                     , "MUS-B-ARENA"                     ) \
    X(MUS_OBTAIN_B_POINTS             , "MUS-OBTAIN-B-POINTS"             ) \
    X(MUS_REGISTER_MATCH_CALL         , "MUS-REGISTER-MATCH-CALL"         ) \
    X(MUS_B_PYRAMID                   , "MUS-B-PYRAMID"                   ) \
    X(MUS_B_PYRAMID_TOP               , "MUS-B-PYRAMID-TOP"               ) \
    X(MUS_B_PALACE                    , "MUS-B-PALACE"                    ) \
    X(MUS_RAYQUAZA_APPEARS            , "MUS-RAYQUAZA-APPEARS"            ) \
    X(MUS_B_TOWER                     , "MUS-B-TOWER"                     ) \
    X(MUS_OBTAIN_SYMBOL               , "MUS-OBTAIN-SYMBOL"               ) \
    X(MUS_B_DOME                      , "MUS-B-DOME"                      ) \
    X(MUS_B_PIKE                      , "MUS-B-PIKE"                      ) \
    X(MUS_B_FACTORY                   , "MUS-B-FACTORY"                   ) \
    X(MUS_VS_RAYQUAZA                 , "MUS-VS-RAYQUAZA"                 ) \
    X(MUS_VS_FRONTIER_BRAIN           , "MUS-VS-FRONTIER-BRAIN"           ) \
    X(MUS_VS_MEW                      , "MUS-VS-MEW"                      ) \
    X(MUS_B_DOME_LOBBY                , "MUS-B-DOME-LOBBY"                ) \
    X(MUS_DP_TWINLEAF_DAY             , "MUS-DP-TWINLEAF-DAY"             ) \
    X(MUS_DP_SANDGEM_DAY              , "MUS-DP-SANDGEM-DAY"              ) \
    X(MUS_DP_FLOAROMA_DAY             , "MUS-DP-FLOAROMA-DAY"             ) \
    X(MUS_DP_SOLACEON_DAY             , "MUS-DP-SOLACEON-DAY"             ) \
    X(MUS_DP_ROUTE225_DAY             , "MUS-DP-ROUTE225-DAY"             ) \
    X(MUS_DP_VALOR_LAKEFRONT_DAY      , "MUS-DP-VALOR-LAKEFRONT-DAY"      ) \
    X(MUS_DP_JUBILIFE_DAY             , "MUS-DP-JUBILIFE-DAY"             ) \
    X(MUS_DP_CANALAVE_DAY             , "MUS-DP-CANALAVE-DAY"             ) \
    X(MUS_DP_OREBURGH_DAY             , "MUS-DP-OREBURGH-DAY"             ) \
    X(MUS_DP_ETERNA_DAY               , "MUS-DP-ETERNA-DAY"               ) \
    X(MUS_DP_HEARTHOME_DAY            , "MUS-DP-HEARTHOME-DAY"            ) \
    X(MUS_DP_VEILSTONE_DAY            , "MUS-DP-VEILSTONE-DAY"            ) \
    X(MUS_DP_SUNYSHORE_DAY            , "MUS-DP-SUNYSHORE-DAY"            ) \
    X(MUS_DP_SNOWPOINT_DAY            , "MUS-DP-SNOWPOINT-DAY"            ) \
    X(MUS_DP_POKEMON_LEAGUE_DAY       , "MUS-DP-POKEMON-LEAGUE-DAY"       ) \
    X(MUS_DP_FIGHT_AREA_DAY           , "MUS-DP-FIGHT-AREA-DAY"           ) \
    X(MUS_DP_ROUTE201_DAY             , "MUS-DP-ROUTE201-DAY"             ) \
    X(MUS_DP_ROUTE203_DAY             , "MUS-DP-ROUTE203-DAY"             ) \
    X(MUS_DP_ROUTE205_DAY             , "MUS-DP-ROUTE205-DAY"             ) \
    X(MUS_DP_ROUTE206_DAY             , "MUS-DP-ROUTE206-DAY"             ) \
    X(MUS_DP_ROUTE209_DAY             , "MUS-DP-ROUTE209-DAY"             ) \
    X(MUS_DP_ROUTE210_DAY             , "MUS-DP-ROUTE210-DAY"             ) \
    X(MUS_DP_ROUTE216_DAY             , "MUS-DP-ROUTE216-DAY"             ) \
    X(MUS_DP_ROUTE228_DAY             , "MUS-DP-ROUTE228-DAY"             ) \
    X(MUS_DP_ROWAN                    , "MUS-DP-ROWAN"                    ) \
    X(MUS_DP_TV_BROADCAST             , "MUS-DP-TV-BROADCAST"             ) \
    X(MUS_DP_TWINLEAF_NIGHT           , "MUS-DP-TWINLEAF-NIGHT"           ) \
    X(MUS_DP_SANDGEM_NIGHT            , "MUS-DP-SANDGEM-NIGHT"            ) \
    X(MUS_DP_FLOAROMA_NIGHT           , "MUS-DP-FLOAROMA-NIGHT"           ) \
    X(MUS_DP_SOLACEON_NIGHT           , "MUS-DP-SOLACEON-NIGHT"           ) \
    X(MUS_DP_ROUTE225_NIGHT           , "MUS-DP-ROUTE225-NIGHT"           ) \
    X(MUS_DP_VALOR_LAKEFRONT_NIGHT    , "MUS-DP-VALOR-LAKEFRONT-NIGHT"    ) \
    X(MUS_DP_JUBILIFE_NIGHT           , "MUS-DP-JUBILIFE-NIGHT"           ) \
    X(MUS_DP_CANALAVE_NIGHT           , "MUS-DP-CANALAVE-NIGHT"           ) \
    X(MUS_DP_OREBURGH_NIGHT           , "MUS-DP-OREBURGH-NIGHT"           ) \
    X(MUS_DP_ETERNA_NIGHT             , "MUS-DP-ETERNA-NIGHT"             ) \
    X(MUS_DP_HEARTHOME_NIGHT          , "MUS-DP-HEARTHOME-NIGHT"          ) \
    X(MUS_DP_VEILSTONE_NIGHT          , "MUS-DP-VEILSTONE-NIGHT"          ) \
    X(MUS_DP_SUNYSHORE_NIGHT          , "MUS-DP-SUNYSHORE-NIGHT"          ) \
    X(MUS_DP_SNOWPOINT_NIGHT          , "MUS-DP-SNOWPOINT-NIGHT"          ) \
    X(MUS_DP_POKEMON_LEAGUE_NIGHT     , "MUS-DP-POKEMON-LEAGUE-NIGHT"     ) \
    X(MUS_DP_FIGHT_AREA_NIGHT         , "MUS-DP-FIGHT-AREA-NIGHT"         ) \
    X(MUS_DP_ROUTE201_NIGHT           , "MUS-DP-ROUTE201-NIGHT"           ) \
    X(MUS_DP_ROUTE203_NIGHT           , "MUS-DP-ROUTE203-NIGHT"           ) \
    X(MUS_DP_ROUTE205_NIGHT           , "MUS-DP-ROUTE205-NIGHT"           ) \
    X(MUS_DP_ROUTE206_NIGHT           , "MUS-DP-ROUTE206-NIGHT"           ) \
    X(MUS_DP_ROUTE209_NIGHT           , "MUS-DP-ROUTE209-NIGHT"           ) \
    X(MUS_DP_ROUTE210_NIGHT           , "MUS-DP-ROUTE210-NIGHT"           ) \
    X(MUS_DP_ROUTE216_NIGHT           , "MUS-DP-ROUTE216-NIGHT"           ) \
    X(MUS_DP_ROUTE228_NIGHT           , "MUS-DP-ROUTE228-NIGHT"           ) \
    X(MUS_DP_UNDERGROUND              , "MUS-DP-UNDERGROUND"              ) \
    X(MUS_DP_FLAG_CAPTURED            , "MUS-DP-FLAG-CAPTURED"            ) \
    X(MUS_DP_VICTORY_ROAD             , "MUS-DP-VICTORY-ROAD"             ) \
    X(MUS_DP_ETERNA_FOREST            , "MUS-DP-ETERNA-FOREST"            ) \
    X(MUS_DP_OLD_CHATEAU              , "MUS-DP-OLD-CHATEAU"              ) \
    X(MUS_DP_LAKE_CAVERNS             , "MUS-DP-LAKE-CAVERNS"             ) \
    X(MUS_DP_AMITY_SQUARE             , "MUS-DP-AMITY-SQUARE"             ) \
    X(MUS_DP_GALACTIC_HQ              , "MUS-DP-GALACTIC-HQ"              ) \
    X(MUS_DP_GALACTIC_ETERNA_BUILDING , "MUS-DP-GALACTIC-ETERNA-BUILDING" ) \
    X(MUS_DP_GREAT_MARSH              , "MUS-DP-GREAT-MARSH"              ) \
    X(MUS_DP_LAKE                     , "MUS-DP-LAKE"                     ) \
    X(MUS_DP_MT_CORONET               , "MUS-DP-MT-CORONET"               ) \
    X(MUS_DP_SPEAR_PILLAR             , "MUS-DP-SPEAR-PILLAR"             ) \
    X(MUS_DP_STARK_MOUNTAIN           , "MUS-DP-STARK-MOUNTAIN"           ) \
    X(MUS_DP_OREBURGH_GATE            , "MUS-DP-OREBURGH-GATE"            ) \
    X(MUS_DP_OREBURGH_MINE            , "MUS-DP-OREBURGH-MINE"            ) \
    X(MUS_DP_INSIDE_POKEMON_LEAGUE    , "MUS-DP-INSIDE-POKEMON-LEAGUE"    ) \
    X(MUS_DP_HALL_OF_FAME_ROOM        , "MUS-DP-HALL-OF-FAME-ROOM"        ) \
    X(MUS_DP_POKE_CENTER_DAY          , "MUS-DP-POKE-CENTER-DAY"          ) \
    X(MUS_DP_POKE_CENTER_NIGHT        , "MUS-DP-POKE-CENTER-NIGHT"        ) \
    X(MUS_DP_GYM                      , "MUS-DP-GYM"                      ) \
    X(MUS_DP_ROWAN_LAB                , "MUS-DP-ROWAN-LAB"                ) \
    X(MUS_DP_CONTEST_LOBBY            , "MUS-DP-CONTEST-LOBBY"            ) \
    X(MUS_DP_POKE_MART                , "MUS-DP-POKE-MART"                ) \
    X(MUS_DP_GAME_CORNER              , "MUS-DP-GAME-CORNER"              ) \
    X(MUS_DP_B_TOWER                  , "MUS-DP-B-TOWER"                  ) \
    X(MUS_DP_TV_STATION               , "MUS-DP-TV-STATION"               ) \
    X(MUS_DP_GALACTIC_HQ_BASEMENT     , "MUS-DP-GALACTIC-HQ-BASEMENT"     ) \
    X(MUS_DP_AZURE_FLUTE              , "MUS-DP-AZURE-FLUTE"              ) \
    X(MUS_DP_HALL_OF_ORIGIN           , "MUS-DP-HALL-OF-ORIGIN"           ) \
    X(MUS_DP_GTS                      , "MUS-DP-GTS"                      ) \
    X(MUS_DP_ENCOUNTER_BOY            , "MUS-DP-ENCOUNTER-BOY"            ) \
    X(MUS_DP_ENCOUNTER_TWINS          , "MUS-DP-ENCOUNTER-TWINS"          ) \
    X(MUS_DP_ENCOUNTER_INTENSE        , "MUS-DP-ENCOUNTER-INTENSE"        ) \
    X(MUS_DP_ENCOUNTER_GALACTIC       , "MUS-DP-ENCOUNTER-GALACTIC"       ) \
    X(MUS_DP_ENCOUNTER_LADY           , "MUS-DP-ENCOUNTER-LADY"           ) \
    X(MUS_DP_ENCOUNTER_HIKER          , "MUS-DP-ENCOUNTER-HIKER"          ) \
    X(MUS_DP_ENCOUNTER_RICH           , "MUS-DP-ENCOUNTER-RICH"           ) \
    X(MUS_DP_ENCOUNTER_SAILOR         , "MUS-DP-ENCOUNTER-SAILOR"         ) \
    X(MUS_DP_ENCOUNTER_SUSPICIOUS     , "MUS-DP-ENCOUNTER-SUSPICIOUS"     ) \
    X(MUS_DP_ENCOUNTER_ACE_TRAINER    , "MUS-DP-ENCOUNTER-ACE-TRAINER"    ) \
    X(MUS_DP_ENCOUNTER_GIRL           , "MUS-DP-ENCOUNTER-GIRL"           ) \
    X(MUS_DP_ENCOUNTER_CYCLIST        , "MUS-DP-ENCOUNTER-CYCLIST"        ) \
    X(MUS_DP_ENCOUNTER_ARTIST         , "MUS-DP-ENCOUNTER-ARTIST"         ) \
    X(MUS_DP_ENCOUNTER_ELITE_FOUR     , "MUS-DP-ENCOUNTER-ELITE-FOUR"     ) \
    X(MUS_DP_ENCOUNTER_CHAMPION       , "MUS-DP-ENCOUNTER-CHAMPION"       ) \
    X(MUS_DP_VS_WILD                  , "MUS-DP-VS-WILD"                  ) \
    X(MUS_DP_VS_GYM_LEADER            , "MUS-DP-VS-GYM-LEADER"            ) \
    X(MUS_DP_VS_UXIE_MESPRIT_AZELF    , "MUS-DP-VS-UXIE-MESPRIT-AZELF"    ) \
    X(MUS_DP_VS_TRAINER               , "MUS-DP-VS-TRAINER"               ) \
    X(MUS_DP_VS_GALACTIC_BOSS         , "MUS-DP-VS-GALACTIC-BOSS"         ) \
    X(MUS_DP_VS_DIALGA_PALKIA         , "MUS-DP-VS-DIALGA-PALKIA"         ) \
    X(MUS_DP_VS_CHAMPION              , "MUS-DP-VS-CHAMPION"              ) \
    X(MUS_DP_VS_GALACTIC              , "MUS-DP-VS-GALACTIC"              ) \
    X(MUS_DP_VS_RIVAL                 , "MUS-DP-VS-RIVAL"                 ) \
    X(MUS_DP_VS_ARCEUS                , "MUS-DP-VS-ARCEUS"                ) \
    X(MUS_DP_VS_LEGEND                , "MUS-DP-VS-LEGEND"                ) \
    X(MUS_DP_VICTORY_WILD             , "MUS-DP-VICTORY-WILD"             ) \
    X(MUS_DP_VICTORY_TRAINER          , "MUS-DP-VICTORY-TRAINER"          ) \
    X(MUS_DP_VICTORY_GYM_LEADER       , "MUS-DP-VICTORY-GYM-LEADER"       ) \
    X(MUS_DP_VICTORY_CHAMPION         , "MUS-DP-VICTORY-CHAMPION"         ) \
    X(MUS_DP_VICTORY_GALACTIC         , "MUS-DP-VICTORY-GALACTIC"         ) \
    X(MUS_DP_VICTORY_ELITE_FOUR       , "MUS-DP-VICTORY-ELITE-FOUR"       ) \
    X(MUS_DP_VS_GALACTIC_COMMANDER    , "MUS-DP-VS-GALACTIC-COMMANDER"    ) \
    X(MUS_DP_CONTEST                  , "MUS-DP-CONTEST"                  ) \
    X(MUS_DP_VS_ELITE_FOUR            , "MUS-DP-VS-ELITE-FOUR"            ) \
    X(MUS_DP_FOLLOW_ME                , "MUS-DP-FOLLOW-ME"                ) \
    X(MUS_DP_RIVAL                    , "MUS-DP-RIVAL"                    ) \
    X(MUS_DP_LAKE_EVENT               , "MUS-DP-LAKE-EVENT"               ) \
    X(MUS_DP_EVOLUTION                , "MUS-DP-EVOLUTION"                ) \
    X(MUS_DP_LUCAS                    , "MUS-DP-LUCAS"                    ) \
    X(MUS_DP_DAWN                     , "MUS-DP-DAWN"                     ) \
    X(MUS_DP_LEGEND_APPEARS           , "MUS-DP-LEGEND-APPEARS"           ) \
    X(MUS_DP_CATASTROPHE              , "MUS-DP-CATASTROPHE"              ) \
    X(MUS_DP_POKE_RADAR               , "MUS-DP-POKE-RADAR"               ) \
    X(MUS_DP_SURF                     , "MUS-DP-SURF"                     ) \
    X(MUS_DP_CYCLING                  , "MUS-DP-CYCLING"                  ) \
    X(MUS_DP_LETS_GO_TOGETHER         , "MUS-DP-LETS-GO-TOGETHER"         ) \
    X(MUS_DP_TV_END                   , "MUS-DP-TV-END"                   ) \
    X(MUS_DP_LEVEL_UP                 , "MUS-DP-LEVEL-UP"                 ) \
    X(MUS_DP_EVOLVED                  , "MUS-DP-EVOLVED"                  ) \
    X(MUS_DP_OBTAIN_KEY_ITEM          , "MUS-DP-OBTAIN-KEY-ITEM"          ) \
    X(MUS_DP_OBTAIN_ITEM              , "MUS-DP-OBTAIN-ITEM"              ) \
    X(MUS_DP_CAUGHT_INTRO             , "MUS-DP-CAUGHT-INTRO"             ) \
    X(MUS_DP_DEX_RATING               , "MUS-DP-DEX-RATING"               ) \
    X(MUS_DP_OBTAIN_BADGE             , "MUS-DP-OBTAIN-BADGE"             ) \
    X(MUS_DP_POKETCH                  , "MUS-DP-POKETCH"                  ) \
    X(MUS_DP_OBTAIN_TMHM              , "MUS-DP-OBTAIN-TMHM"              ) \
    X(MUS_DP_OBTAIN_ACCESSORY         , "MUS-DP-OBTAIN-ACCESSORY"         ) \
    X(MUS_DP_MOVE_DELETED             , "MUS-DP-MOVE-DELETED"             ) \
    X(MUS_DP_HEAL                     , "MUS-DP-HEAL"                     ) \
    X(MUS_DP_OBTAIN_BERRY             , "MUS-DP-OBTAIN-BERRY"             ) \
    X(MUS_DP_CONTEST_DRESS_UP         , "MUS-DP-CONTEST-DRESS-UP"         ) \
    X(MUS_DP_HALL_OF_FAME             , "MUS-DP-HALL-OF-FAME"             ) \
    X(MUS_DP_INTRO                    , "MUS-DP-INTRO"                    ) \
    X(MUS_DP_TITLE                    , "MUS-DP-TITLE"                    ) \
    X(MUS_DP_MYSTERY_GIFT             , "MUS-DP-MYSTERY-GIFT"             ) \
    X(MUS_DP_WFC                      , "MUS-DP-WFC"                      ) \
    X(MUS_DP_DANCE_EASY               , "MUS-DP-DANCE-EASY"               ) \
    X(MUS_DP_DANCE_DIFFICULT          , "MUS-DP-DANCE-DIFFICULT"          ) \
    X(MUS_DP_CONTEST_RESULTS          , "MUS-DP-CONTEST-RESULTS"          ) \
    X(MUS_DP_CONTEST_WINNER           , "MUS-DP-CONTEST-WINNER"           ) \
    X(MUS_DP_POFFINS                  , "MUS-DP-POFFINS"                  ) \
    X(MUS_DP_SLOTS_WIN                , "MUS-DP-SLOTS-WIN"                ) \
    X(MUS_DP_SLOTS_JACKPOT            , "MUS-DP-SLOTS-JACKPOT"            ) \
    X(MUS_DP_CREDITS                  , "MUS-DP-CREDITS"                  ) \
    X(MUS_DP_SLOTS_UNUSED             , "MUS-DP-SLOTS-UNUSED"             ) \
    X(MUS_PL_FIGHT_AREA_DAY           , "MUS-PL-FIGHT-AREA-DAY"           ) \
    X(MUS_PL_TV_BROADCAST             , "MUS-PL-TV-BROADCAST"             ) \
    X(MUS_PL_TV_END                   , "MUS-PL-TV-END"                   ) \
    X(MUS_PL_INTRO                    , "MUS-PL-INTRO"                    ) \
    X(MUS_PL_TITLE                    , "MUS-PL-TITLE"                    ) \
    X(MUS_PL_DISTORTION_WORLD         , "MUS-PL-DISTORTION-WORLD"         ) \
    X(MUS_PL_B_ARCADE                 , "MUS-PL-B-ARCADE"                 ) \
    X(MUS_PL_B_HALL                   , "MUS-PL-B-HALL"                   ) \
    X(MUS_PL_B_CASTLE                 , "MUS-PL-B-CASTLE"                 ) \
    X(MUS_PL_B_FACTORY                , "MUS-PL-B-FACTORY"                ) \
    X(MUS_PL_GLOBAL_TERMINAL          , "MUS-PL-GLOBAL-TERMINAL"          ) \
    X(MUS_PL_LILYCOVE_BOSSA_NOVA      , "MUS-PL-LILYCOVE-BOSSA-NOVA"      ) \
    X(MUS_PL_LOOKER                   , "MUS-PL-LOOKER"                   ) \
    X(MUS_PL_VS_GIRATINA              , "MUS-PL-VS-GIRATINA"              ) \
    X(MUS_PL_VS_FRONTIER_BRAIN        , "MUS-PL-VS-FRONTIER-BRAIN"        ) \
    X(MUS_PL_VICTORY_FRONTIER_BRAIN   , "MUS-PL-VICTORY-FRONTIER-BRAIN"   ) \
    X(MUS_PL_VS_REGI                  , "MUS-PL-VS-REGI"                  ) \
    X(MUS_PL_CONTEST_COOL             , "MUS-PL-CONTEST-COOL"             ) \
    X(MUS_PL_CONTEST_SMART            , "MUS-PL-CONTEST-SMART"            ) \
    X(MUS_PL_CONTEST_CUTE             , "MUS-PL-CONTEST-CUTE"             ) \
    X(MUS_PL_CONTEST_TOUGH            , "MUS-PL-CONTEST-TOUGH"            ) \
    X(MUS_PL_CONTEST_BEAUTY           , "MUS-PL-CONTEST-BEAUTY"           ) \
    X(MUS_PL_SPIN_TRADE               , "MUS-PL-SPIN-TRADE"               ) \
    X(MUS_PL_WIFI_MINIGAMES           , "MUS-PL-WIFI-MINIGAMES"           ) \
    X(MUS_PL_WIFI_PLAZA               , "MUS-PL-WIFI-PLAZA"               ) \
    X(MUS_PL_WIFI_PARADE              , "MUS-PL-WIFI-PARADE"              ) \
    X(MUS_PL_GIRATINA_APPEARS_1       , "MUS-PL-GIRATINA-APPEARS-1"       ) \
    X(MUS_PL_GIRATINA_APPEARS_2       , "MUS-PL-GIRATINA-APPEARS-2"       ) \
    X(MUS_PL_MYSTERY_GIFT             , "MUS-PL-MYSTERY-GIFT"             ) \
    X(MUS_PL_TWINLEAF_MUSIC_BOX       , "MUS-PL-TWINLEAF-MUSIC-BOX"       ) \
    X(MUS_PL_OBTAIN_ARCADE_POINTS     , "MUS-PL-OBTAIN-ARCADE-POINTS"     ) \
    X(MUS_PL_OBTAIN_CASTLE_POINTS     , "MUS-PL-OBTAIN-CASTLE-POINTS"     ) \
    X(MUS_PL_OBTAIN_B_POINTS          , "MUS-PL-OBTAIN-B-POINTS"          ) \
    X(MUS_PL_WIN_MINIGAME             , "MUS-PL-WIN-MINIGAME"             ) \
    X(MUS_HG_INTRO                    , "MUS-HG-INTRO"                    ) \
    X(MUS_HG_TITLE                    , "MUS-HG-TITLE"                    ) \
    X(MUS_HG_NEW_GAME                 , "MUS-HG-NEW-GAME"                 ) \
    X(MUS_HG_EVOLUTION                , "MUS-HG-EVOLUTION"                ) \
    X(MUS_HG_EVOLUTION_NO_INTRO       , "MUS-HG-EVOLUTION-NO-INTRO"       ) \
    X(MUS_HG_CYCLING                  , "MUS-HG-CYCLING"                  ) \
    X(MUS_HG_SURF                     , "MUS-HG-SURF"                     ) \
    X(MUS_HG_HALL_OF_FAME             , "MUS-HG-HALL-OF-FAME"             ) \
    X(MUS_HG_CREDITS                  , "MUS-HG-CREDITS"                  ) \
    X(MUS_HG_END                      , "MUS-HG-END"                      ) \
    X(MUS_HG_NEW_BARK                 , "MUS-HG-NEW-BARK"                 ) \
    X(MUS_HG_CHERRYGROVE              , "MUS-HG-CHERRYGROVE"              ) \
    X(MUS_HG_VIOLET                   , "MUS-HG-VIOLET"                   ) \
    X(MUS_HG_AZALEA                   , "MUS-HG-AZALEA"                   ) \
    X(MUS_HG_GOLDENROD                , "MUS-HG-GOLDENROD"                ) \
    X(MUS_HG_ECRUTEAK                 , "MUS-HG-ECRUTEAK"                 ) \
    X(MUS_HG_CIANWOOD                 , "MUS-HG-CIANWOOD"                 ) \
    X(MUS_HG_ROUTE29                  , "MUS-HG-ROUTE29"                  ) \
    X(MUS_HG_ROUTE30                  , "MUS-HG-ROUTE30"                  ) \
    X(MUS_HG_ROUTE34                  , "MUS-HG-ROUTE34"                  ) \
    X(MUS_HG_ROUTE38                  , "MUS-HG-ROUTE38"                  ) \
    X(MUS_HG_ROUTE42                  , "MUS-HG-ROUTE42"                  ) \
    X(MUS_HG_VERMILION                , "MUS-HG-VERMILION"                ) \
    X(MUS_HG_PEWTER                   , "MUS-HG-PEWTER"                   ) \
    X(MUS_HG_CERULEAN                 , "MUS-HG-CERULEAN"                 ) \
    X(MUS_HG_LAVENDER                 , "MUS-HG-LAVENDER"                 ) \
    X(MUS_HG_CELADON                  , "MUS-HG-CELADON"                  ) \
    X(MUS_HG_PALLET                   , "MUS-HG-PALLET"                   ) \
    X(MUS_HG_CINNABAR                 , "MUS-HG-CINNABAR"                 ) \
    X(MUS_HG_ROUTE1                   , "MUS-HG-ROUTE1"                   ) \
    X(MUS_HG_ROUTE3                   , "MUS-HG-ROUTE3"                   ) \
    X(MUS_HG_ROUTE11                  , "MUS-HG-ROUTE11"                  ) \
    X(MUS_HG_ROUTE24                  , "MUS-HG-ROUTE24"                  ) \
    X(MUS_HG_ROUTE26                  , "MUS-HG-ROUTE26"                  ) \
    X(MUS_HG_POKE_CENTER              , "MUS-HG-POKE-CENTER"              ) \
    X(MUS_HG_POKE_MART                , "MUS-HG-POKE-MART"                ) \
    X(MUS_HG_GYM                      , "MUS-HG-GYM"                      ) \
    X(MUS_HG_ELM_LAB                  , "MUS-HG-ELM-LAB"                  ) \
    X(MUS_HG_OAK                      , "MUS-HG-OAK"                      ) \
    X(MUS_HG_DANCE_THEATER            , "MUS-HG-DANCE-THEATER"            ) \
    X(MUS_HG_GAME_CORNER              , "MUS-HG-GAME-CORNER"              ) \
    X(MUS_HG_B_TOWER                  , "MUS-HG-B-TOWER"                  ) \
    X(MUS_HG_B_TOWER_RECEPTION        , "MUS-HG-B-TOWER-RECEPTION"        ) \
    X(MUS_HG_SPROUT_TOWER             , "MUS-HG-SPROUT-TOWER"             ) \
    X(MUS_HG_UNION_CAVE               , "MUS-HG-UNION-CAVE"               ) \
    X(MUS_HG_RUINS_OF_ALPH            , "MUS-HG-RUINS-OF-ALPH"            ) \
    X(MUS_HG_NATIONAL_PARK            , "MUS-HG-NATIONAL-PARK"            ) \
    X(MUS_HG_BURNED_TOWER             , "MUS-HG-BURNED-TOWER"             ) \
    X(MUS_HG_BELL_TOWER               , "MUS-HG-BELL-TOWER"               ) \
    X(MUS_HG_LIGHTHOUSE               , "MUS-HG-LIGHTHOUSE"               ) \
    X(MUS_HG_TEAM_ROCKET_HQ           , "MUS-HG-TEAM-ROCKET-HQ"           ) \
    X(MUS_HG_ICE_PATH                 , "MUS-HG-ICE-PATH"                 ) \
    X(MUS_HG_DRAGONS_DEN              , "MUS-HG-DRAGONS-DEN"              ) \
    X(MUS_HG_ROCK_TUNNEL              , "MUS-HG-ROCK-TUNNEL"              ) \
    X(MUS_HG_VIRIDIAN_FOREST          , "MUS-HG-VIRIDIAN-FOREST"          ) \
    X(MUS_HG_VICTORY_ROAD             , "MUS-HG-VICTORY-ROAD"             ) \
    X(MUS_HG_POKEMON_LEAGUE           , "MUS-HG-POKEMON-LEAGUE"           ) \
    X(MUS_HG_FOLLOW_ME_1              , "MUS-HG-FOLLOW-ME-1"              ) \
    X(MUS_HG_FOLLOW_ME_2              , "MUS-HG-FOLLOW-ME-2"              ) \
    X(MUS_HG_ENCOUNTER_RIVAL          , "MUS-HG-ENCOUNTER-RIVAL"          ) \
    X(MUS_HG_RIVAL_EXIT               , "MUS-HG-RIVAL-EXIT"               ) \
    X(MUS_HG_BUG_CONTEST_PREP         , "MUS-HG-BUG-CONTEST-PREP"         ) \
    X(MUS_HG_BUG_CATCHING_CONTEST     , "MUS-HG-BUG-CATCHING-CONTEST"     ) \
    X(MUS_HG_RADIO_ROCKET             , "MUS-HG-RADIO-ROCKET"             ) \
    X(MUS_HG_ROCKET_TAKEOVER          , "MUS-HG-ROCKET-TAKEOVER"          ) \
    X(MUS_HG_MAGNET_TRAIN             , "MUS-HG-MAGNET-TRAIN"             ) \
    X(MUS_HG_SS_AQUA                  , "MUS-HG-SS-AQUA"                  ) \
    X(MUS_HG_MT_MOON_SQUARE           , "MUS-HG-MT-MOON-SQUARE"           ) \
    X(MUS_HG_RADIO_JINGLE             , "MUS-HG-RADIO-JINGLE"             ) \
    X(MUS_HG_RADIO_LULLABY            , "MUS-HG-RADIO-LULLABY"            ) \
    X(MUS_HG_RADIO_MARCH              , "MUS-HG-RADIO-MARCH"              ) \
    X(MUS_HG_RADIO_UNOWN              , "MUS-HG-RADIO-UNOWN"              ) \
    X(MUS_HG_RADIO_POKE_FLUTE         , "MUS-HG-RADIO-POKE-FLUTE"         ) \
    X(MUS_HG_RADIO_OAK                , "MUS-HG-RADIO-OAK"                ) \
    X(MUS_HG_RADIO_BUENA              , "MUS-HG-RADIO-BUENA"              ) \
    X(MUS_HG_EUSINE                   , "MUS-HG-EUSINE"                   ) \
    X(MUS_HG_CLAIR                    , "MUS-HG-CLAIR"                    ) \
    X(MUS_HG_ENCOUNTER_GIRL_1         , "MUS-HG-ENCOUNTER-GIRL-1"         ) \
    X(MUS_HG_ENCOUNTER_BOY_1          , "MUS-HG-ENCOUNTER-BOY-1"          ) \
    X(MUS_HG_ENCOUNTER_SUSPICIOUS_1   , "MUS-HG-ENCOUNTER-SUSPICIOUS-1"   ) \
    X(MUS_HG_ENCOUNTER_SAGE           , "MUS-HG-ENCOUNTER-SAGE"           ) \
    X(MUS_HG_ENCOUNTER_KIMONO_GIRL    , "MUS-HG-ENCOUNTER-KIMONO-GIRL"    ) \
    X(MUS_HG_ENCOUNTER_ROCKET         , "MUS-HG-ENCOUNTER-ROCKET"         ) \
    X(MUS_HG_ENCOUNTER_GIRL_2         , "MUS-HG-ENCOUNTER-GIRL-2"         ) \
    X(MUS_HG_ENCOUNTER_BOY_2          , "MUS-HG-ENCOUNTER-BOY-2"          ) \
    X(MUS_HG_ENCOUNTER_SUSPICIOUS_2   , "MUS-HG-ENCOUNTER-SUSPICIOUS-2"   ) \
    X(MUS_HG_VS_WILD                  , "MUS-HG-VS-WILD"                  ) \
    X(MUS_HG_VS_TRAINER               , "MUS-HG-VS-TRAINER"               ) \
    X(MUS_HG_VS_GYM_LEADER            , "MUS-HG-VS-GYM-LEADER"            ) \
    X(MUS_HG_VS_RIVAL                 , "MUS-HG-VS-RIVAL"                 ) \
    X(MUS_HG_VS_ROCKET                , "MUS-HG-VS-ROCKET"                ) \
    X(MUS_HG_VS_SUICUNE               , "MUS-HG-VS-SUICUNE"               ) \
    X(MUS_HG_VS_ENTEI                 , "MUS-HG-VS-ENTEI"                 ) \
    X(MUS_HG_VS_RAIKOU                , "MUS-HG-VS-RAIKOU"                ) \
    X(MUS_HG_VS_CHAMPION              , "MUS-HG-VS-CHAMPION"              ) \
    X(MUS_HG_VS_WILD_KANTO            , "MUS-HG-VS-WILD-KANTO"            ) \
    X(MUS_HG_VS_TRAINER_KANTO         , "MUS-HG-VS-TRAINER-KANTO"         ) \
    X(MUS_HG_VS_GYM_LEADER_KANTO      , "MUS-HG-VS-GYM-LEADER-KANTO"      ) \
    X(MUS_HG_VICTORY_TRAINER          , "MUS-HG-VICTORY-TRAINER"          ) \
    X(MUS_HG_VICTORY_WILD             , "MUS-HG-VICTORY-WILD"             ) \
    X(MUS_HG_CAUGHT                   , "MUS-HG-CAUGHT"                   ) \
    X(MUS_HG_VICTORY_GYM_LEADER       , "MUS-HG-VICTORY-GYM-LEADER"       ) \
    X(MUS_HG_VS_HO_OH                 , "MUS-HG-VS-HO-OH"                 ) \
    X(MUS_HG_VS_LUGIA                 , "MUS-HG-VS-LUGIA"                 ) \
    X(MUS_HG_POKEATHLON_LOBBY         , "MUS-HG-POKEATHLON-LOBBY"         ) \
    X(MUS_HG_POKEATHLON_START         , "MUS-HG-POKEATHLON-START"         ) \
    X(MUS_HG_POKEATHLON_BEFORE        , "MUS-HG-POKEATHLON-BEFORE"        ) \
    X(MUS_HG_POKEATHLON_EVENT         , "MUS-HG-POKEATHLON-EVENT"         ) \
    X(MUS_HG_POKEATHLON_FINALS        , "MUS-HG-POKEATHLON-FINALS"        ) \
    X(MUS_HG_POKEATHLON_RESULTS       , "MUS-HG-POKEATHLON-RESULTS"       ) \
    X(MUS_HG_POKEATHLON_END           , "MUS-HG-POKEATHLON-END"           ) \
    X(MUS_HG_POKEATHLON_WINNER        , "MUS-HG-POKEATHLON-WINNER"        ) \
    X(MUS_HG_B_FACTORY                , "MUS-HG-B-FACTORY"                ) \
    X(MUS_HG_B_HALL                   , "MUS-HG-B-HALL"                   ) \
    X(MUS_HG_B_ARCADE                 , "MUS-HG-B-ARCADE"                 ) \
    X(MUS_HG_B_CASTLE                 , "MUS-HG-B-CASTLE"                 ) \
    X(MUS_HG_VS_FRONTIER_BRAIN        , "MUS-HG-VS-FRONTIER-BRAIN"        ) \
    X(MUS_HG_VICTORY_FRONTIER_BRAIN   , "MUS-HG-VICTORY-FRONTIER-BRAIN"   ) \
    X(MUS_HG_WFC                      , "MUS-HG-WFC"                      ) \
    X(MUS_HG_MYSTERY_GIFT             , "MUS-HG-MYSTERY-GIFT"             ) \
    X(MUS_HG_WIFI_PLAZA               , "MUS-HG-WIFI-PLAZA"               ) \
    X(MUS_HG_WIFI_MINIGAMES           , "MUS-HG-WIFI-MINIGAMES"           ) \
    X(MUS_HG_WIFI_PARADE              , "MUS-HG-WIFI-PARADE"              ) \
    X(MUS_HG_GLOBAL_TERMINAL          , "MUS-HG-GLOBAL-TERMINAL"          ) \
    X(MUS_HG_SPIN_TRADE               , "MUS-HG-SPIN-TRADE"               ) \
    X(MUS_HG_GTS                      , "MUS-HG-GTS"                      ) \
    X(MUS_HG_ROUTE47                  , "MUS-HG-ROUTE47"                  ) \
    X(MUS_HG_SAFARI_ZONE_GATE         , "MUS-HG-SAFARI-ZONE-GATE"         ) \
    X(MUS_HG_SAFARI_ZONE              , "MUS-HG-SAFARI-ZONE"              ) \
    X(MUS_HG_ETHAN                    , "MUS-HG-ETHAN"                    ) \
    X(MUS_HG_LYRA                     , "MUS-HG-LYRA"                     ) \
    X(MUS_HG_GAME_CORNER_WIN          , "MUS-HG-GAME-CORNER-WIN"          ) \
    X(MUS_HG_KIMONO_GIRL_DANCE        , "MUS-HG-KIMONO-GIRL-DANCE"        ) \
    X(MUS_HG_KIMONO_GIRL              , "MUS-HG-KIMONO-GIRL"              ) \
    X(MUS_HG_HO_OH_APPEARS            , "MUS-HG-HO-OH-APPEARS"            ) \
    X(MUS_HG_LUGIA_APPEARS            , "MUS-HG-LUGIA-APPEARS"            ) \
    X(MUS_HG_SPIKY_EARED_PICHU        , "MUS-HG-SPIKY-EARED-PICHU"        ) \
    X(MUS_HG_SINJOU_RUINS             , "MUS-HG-SINJOU-RUINS"             ) \
    X(MUS_HG_RADIO_ROUTE101           , "MUS-HG-RADIO-ROUTE101"           ) \
    X(MUS_HG_RADIO_ROUTE201           , "MUS-HG-RADIO-ROUTE201"           ) \
    X(MUS_HG_RADIO_TRAINER            , "MUS-HG-RADIO-TRAINER"            ) \
    X(MUS_HG_RADIO_VARIETY            , "MUS-HG-RADIO-VARIETY"            ) \
    X(MUS_HG_VS_KYOGRE_GROUDON        , "MUS-HG-VS-KYOGRE-GROUDON"        ) \
    X(MUS_HG_POKEWALKER               , "MUS-HG-POKEWALKER"               ) \
    X(MUS_HG_VS_ARCEUS                , "MUS-HG-VS-ARCEUS"                ) \
    X(MUS_HG_HEAL                     , "MUS-HG-HEAL"                     ) \
    X(MUS_HG_LEVEL_UP                 , "MUS-HG-LEVEL-UP"                 ) \
    X(MUS_HG_OBTAIN_ITEM              , "MUS-HG-OBTAIN-ITEM"              ) \
    X(MUS_HG_OBTAIN_KEY_ITEM          , "MUS-HG-OBTAIN-KEY-ITEM"          ) \
    X(MUS_HG_EVOLVED                  , "MUS-HG-EVOLVED"                  ) \
    X(MUS_HG_OBTAIN_BADGE             , "MUS-HG-OBTAIN-BADGE"             ) \
    X(MUS_HG_OBTAIN_TMHM              , "MUS-HG-OBTAIN-TMHM"              ) \
    X(MUS_HG_OBTAIN_ACCESSORY         , "MUS-HG-OBTAIN-ACCESSORY"         ) \
    X(MUS_HG_MOVE_DELETED             , "MUS-HG-MOVE-DELETED"             ) \
    X(MUS_HG_OBTAIN_BERRY             , "MUS-HG-OBTAIN-BERRY"             ) \
    X(MUS_HG_DEX_RATING_1             , "MUS-HG-DEX-RATING-1"             ) \
    X(MUS_HG_DEX_RATING_2             , "MUS-HG-DEX-RATING-2"             ) \
    X(MUS_HG_DEX_RATING_3             , "MUS-HG-DEX-RATING-3"             ) \
    X(MUS_HG_DEX_RATING_4             , "MUS-HG-DEX-RATING-4"             ) \
    X(MUS_HG_DEX_RATING_5             , "MUS-HG-DEX-RATING-5"             ) \
    X(MUS_HG_DEX_RATING_6             , "MUS-HG-DEX-RATING-6"             ) \
    X(MUS_HG_OBTAIN_EGG               , "MUS-HG-OBTAIN-EGG"               ) \
    X(MUS_HG_BUG_CONTEST_1ST_PLACE    , "MUS-HG-BUG-CONTEST-1ST-PLACE"    ) \
    X(MUS_HG_BUG_CONTEST_2ND_PLACE    , "MUS-HG-BUG-CONTEST-2ND-PLACE"    ) \
    X(MUS_HG_BUG_CONTEST_3RD_PLACE    , "MUS-HG-BUG-CONTEST-3RD-PLACE"    ) \
    X(MUS_HG_CARD_FLIP                , "MUS-HG-CARD-FLIP"                ) \
    X(MUS_HG_CARD_FLIP_GAME_OVER      , "MUS-HG-CARD-FLIP-GAME-OVER"      ) \
    X(MUS_HG_POKEGEAR_REGISTERED      , "MUS-HG-POKEGEAR-REGISTERED"      ) \
    X(MUS_HG_LETS_GO_TOGETHER         , "MUS-HG-LETS-GO-TOGETHER"         ) \
    X(MUS_HG_POKEATHLON_READY         , "MUS-HG-POKEATHLON-READY"         ) \
    X(MUS_HG_POKEATHLON_1ST_PLACE     , "MUS-HG-POKEATHLON-1ST-PLACE"     ) \
    X(MUS_HG_RECEIVE_POKEMON          , "MUS-HG-RECEIVE-POKEMON"          ) \
    X(MUS_HG_OBTAIN_ARCADE_POINTS     , "MUS-HG-OBTAIN-ARCADE-POINTS"     ) \
    X(MUS_HG_OBTAIN_CASTLE_POINTS     , "MUS-HG-OBTAIN-CASTLE-POINTS"     ) \
    X(MUS_HG_OBTAIN_B_POINTS          , "MUS-HG-OBTAIN-B-POINTS"          ) \
    X(MUS_HG_WIN_MINIGAME             , "MUS-HG-WIN-MINIGAME"             ) \
    X(MUS_BW_VS_LEGEND                , "MUS-BW-VS-LEGEND"                ) \
    X(MUS_BW_VS_IRIS                  , "MUS-BW-VS-IRIS"                  ) \

#define SOUND_LIST_SE \
    X(SE_USE_ITEM              , "SE-USE-ITEM"              ) \
    X(SE_PC_LOGIN              , "SE-PC-LOGIN"              ) \
    X(SE_PC_OFF                , "SE-PC-OFF"                ) \
    X(SE_PC_ON                 , "SE-PC-ON"                 ) \
    X(SE_SELECT                , "SE-SELECT"                ) \
    X(SE_WIN_OPEN              , "SE-WIN-OPEN"              ) \
    X(SE_WALL_HIT              , "SE-WALL-HIT"              ) \
    X(SE_DOOR                  , "SE-DOOR"                  ) \
    X(SE_EXIT                  , "SE-EXIT"                  ) \
    X(SE_LEDGE                 , "SE-LEDGE"                 ) \
    X(SE_BIKE_BELL             , "SE-BIKE-BELL"             ) \
    X(SE_NOT_EFFECTIVE         , "SE-NOT-EFFECTIVE"         ) \
    X(SE_EFFECTIVE             , "SE-EFFECTIVE"             ) \
    X(SE_SUPER_EFFECTIVE       , "SE-SUPER-EFFECTIVE"       ) \
    X(SE_BALL_OPEN             , "SE-BALL-OPEN"             ) \
    X(SE_FAINT                 , "SE-FAINT"                 ) \
    X(SE_FLEE                  , "SE-FLEE"                  ) \
    X(SE_SLIDING_DOOR          , "SE-SLIDING-DOOR"          ) \
    X(SE_SHIP                  , "SE-SHIP"                  ) \
    X(SE_BANG                  , "SE-BANG"                  ) \
    X(SE_PIN                   , "SE-PIN"                   ) \
    X(SE_BOO                   , "SE-BOO"                   ) \
    X(SE_BALL                  , "SE-BALL"                  ) \
    X(SE_CONTEST_PLACE         , "SE-CONTEST-PLACE"         ) \
    X(SE_A                     , "SE-A"                     ) \
    X(SE_I                     , "SE-I"                     ) \
    X(SE_U                     , "SE-U"                     ) \
    X(SE_E                     , "SE-E"                     ) \
    X(SE_O                     , "SE-O"                     ) \
    X(SE_N                     , "SE-N"                     ) \
    X(SE_SUCCESS               , "SE-SUCCESS"               ) \
    X(SE_FAILURE               , "SE-FAILURE"               ) \
    X(SE_EXP                   , "SE-EXP"                   ) \
    X(SE_BIKE_HOP              , "SE-BIKE-HOP"              ) \
    X(SE_SWITCH                , "SE-SWITCH"                ) \
    X(SE_CLICK                 , "SE-CLICK"                 ) \
    X(SE_FU_ZAKU               , "SE-FU-ZAKU"               ) \
    X(SE_CONTEST_CONDITION_LOSE, "SE-CONTEST-CONDITION-LOSE") \
    X(SE_LAVARIDGE_FALL_WARP   , "SE-LAVARIDGE-FALL-WARP"   ) \
    X(SE_ICE_STAIRS            , "SE-ICE-STAIRS"            ) \
    X(SE_ICE_BREAK             , "SE-ICE-BREAK"             ) \
    X(SE_ICE_CRACK             , "SE-ICE-CRACK"             ) \
    X(SE_FALL                  , "SE-FALL"                  ) \
    X(SE_UNLOCK                , "SE-UNLOCK"                ) \
    X(SE_WARP_IN               , "SE-WARP-IN"               ) \
    X(SE_WARP_OUT              , "SE-WARP-OUT"              ) \
    X(SE_REPEL                 , "SE-REPEL"                 ) \
    X(SE_ROTATING_GATE         , "SE-ROTATING-GATE"         ) \
    X(SE_TRUCK_MOVE            , "SE-TRUCK-MOVE"            ) \
    X(SE_TRUCK_STOP            , "SE-TRUCK-STOP"            ) \
    X(SE_TRUCK_UNLOAD          , "SE-TRUCK-UNLOAD"          ) \
    X(SE_TRUCK_DOOR            , "SE-TRUCK-DOOR"            ) \
    X(SE_BERRY_BLENDER         , "SE-BERRY-BLENDER"         ) \
    X(SE_CARD                  , "SE-CARD"                  ) \
    X(SE_SAVE                  , "SE-SAVE"                  ) \
    X(SE_BALL_BOUNCE_1         , "SE-BALL-BOUNCE-1"         ) \
    X(SE_BALL_BOUNCE_2         , "SE-BALL-BOUNCE-2"         ) \
    X(SE_BALL_BOUNCE_3         , "SE-BALL-BOUNCE-3"         ) \
    X(SE_BALL_BOUNCE_4         , "SE-BALL-BOUNCE-4"         ) \
    X(SE_BALL_TRADE            , "SE-BALL-TRADE"            ) \
    X(SE_BALL_THROW            , "SE-BALL-THROW"            ) \
    X(SE_NOTE_C                , "SE-NOTE-C"                ) \
    X(SE_NOTE_D                , "SE-NOTE-D"                ) \
    X(SE_NOTE_E                , "SE-NOTE-E"                ) \
    X(SE_NOTE_F                , "SE-NOTE-F"                ) \
    X(SE_NOTE_G                , "SE-NOTE-G"                ) \
    X(SE_NOTE_A                , "SE-NOTE-A"                ) \
    X(SE_NOTE_B                , "SE-NOTE-B"                ) \
    X(SE_NOTE_C_HIGH           , "SE-NOTE-C-HIGH"           ) \
    X(SE_PUDDLE                , "SE-PUDDLE"                ) \
    X(SE_BRIDGE_WALK           , "SE-BRIDGE-WALK"           ) \
    X(SE_ITEMFINDER            , "SE-ITEMFINDER"            ) \
    X(SE_DING_DONG             , "SE-DING-DONG"             ) \
    X(SE_BALLOON_RED           , "SE-BALLOON-RED"           ) \
    X(SE_BALLOON_BLUE          , "SE-BALLOON-BLUE"          ) \
    X(SE_BALLOON_YELLOW        , "SE-BALLOON-YELLOW"        ) \
    X(SE_BREAKABLE_DOOR        , "SE-BREAKABLE-DOOR"        ) \
    X(SE_MUD_BALL              , "SE-MUD-BALL"              ) \
    X(SE_FIELD_POISON          , "SE-FIELD-POISON"          ) \
    X(SE_ESCALATOR             , "SE-ESCALATOR"             ) \
    X(SE_THUNDERSTORM          , "SE-THUNDERSTORM"          ) \
    X(SE_THUNDERSTORM_STOP     , "SE-THUNDERSTORM-STOP"     ) \
    X(SE_DOWNPOUR              , "SE-DOWNPOUR"              ) \
    X(SE_DOWNPOUR_STOP         , "SE-DOWNPOUR-STOP"         ) \
    X(SE_RAIN                  , "SE-RAIN"                  ) \
    X(SE_RAIN_STOP             , "SE-RAIN-STOP"             ) \
    X(SE_THUNDER               , "SE-THUNDER"               ) \
    X(SE_THUNDER2              , "SE-THUNDER2"              ) \
    X(SE_ELEVATOR              , "SE-ELEVATOR"              ) \
    X(SE_LOW_HEALTH            , "SE-LOW-HEALTH"            ) \
    X(SE_EXP_MAX               , "SE-EXP-MAX"               ) \
    X(SE_ROULETTE_BALL         , "SE-ROULETTE-BALL"         ) \
    X(SE_ROULETTE_BALL2        , "SE-ROULETTE-BALL2"        ) \
    X(SE_TAILLOW_WING_FLAP     , "SE-TAILLOW-WING-FLAP"     ) \
    X(SE_SHOP                  , "SE-SHOP"                  ) \
    X(SE_CONTEST_HEART         , "SE-CONTEST-HEART"         ) \
    X(SE_CONTEST_CURTAIN_RISE  , "SE-CONTEST-CURTAIN-RISE"  ) \
    X(SE_CONTEST_CURTAIN_FALL  , "SE-CONTEST-CURTAIN-FALL"  ) \
    X(SE_CONTEST_ICON_CHANGE   , "SE-CONTEST-ICON-CHANGE"   ) \
    X(SE_CONTEST_ICON_CLEAR    , "SE-CONTEST-ICON-CLEAR"    ) \
    X(SE_CONTEST_MONS_TURN     , "SE-CONTEST-MONS-TURN"     ) \
    X(SE_SHINY                 , "SE-SHINY"                 ) \
    X(SE_INTRO_BLAST           , "SE-INTRO-BLAST"           ) \
    X(SE_MUGSHOT               , "SE-MUGSHOT"               ) \
    X(SE_APPLAUSE              , "SE-APPLAUSE"              ) \
    X(SE_VEND                  , "SE-VEND"                  ) \
    X(SE_ORB                   , "SE-ORB"                   ) \
    X(SE_DEX_SCROLL            , "SE-DEX-SCROLL"            ) \
    X(SE_DEX_PAGE              , "SE-DEX-PAGE"              ) \
    X(SE_POKENAV_ON            , "SE-POKENAV-ON"            ) \
    X(SE_POKENAV_OFF           , "SE-POKENAV-OFF"           ) \
    X(SE_DEX_SEARCH            , "SE-DEX-SEARCH"            ) \
    X(SE_EGG_HATCH             , "SE-EGG-HATCH"             ) \
    X(SE_BALL_TRAY_ENTER       , "SE-BALL-TRAY-ENTER"       ) \
    X(SE_BALL_TRAY_BALL        , "SE-BALL-TRAY-BALL"        ) \
    X(SE_BALL_TRAY_EXIT        , "SE-BALL-TRAY-EXIT"        ) \
    X(SE_GLASS_FLUTE           , "SE-GLASS-FLUTE"           ) \
    X(SE_M_THUNDERBOLT         , "SE-M-THUNDERBOLT"         ) \
    X(SE_M_THUNDERBOLT2        , "SE-M-THUNDERBOLT2"        ) \
    X(SE_M_HARDEN              , "SE-M-HARDEN"              ) \
    X(SE_M_NIGHTMARE           , "SE-M-NIGHTMARE"           ) \
    X(SE_M_VITAL_THROW         , "SE-M-VITAL-THROW"         ) \
    X(SE_M_VITAL_THROW2        , "SE-M-VITAL-THROW2"        ) \
    X(SE_M_BUBBLE              , "SE-M-BUBBLE"              ) \
    X(SE_M_BUBBLE2             , "SE-M-BUBBLE2"             ) \
    X(SE_M_BUBBLE3             , "SE-M-BUBBLE3"             ) \
    X(SE_M_RAIN_DANCE          , "SE-M-RAIN-DANCE"          ) \
    X(SE_M_CUT                 , "SE-M-CUT"                 ) \
    X(SE_M_STRING_SHOT         , "SE-M-STRING-SHOT"         ) \
    X(SE_M_STRING_SHOT2        , "SE-M-STRING-SHOT2"        ) \
    X(SE_M_ROCK_THROW          , "SE-M-ROCK-THROW"          ) \
    X(SE_M_GUST                , "SE-M-GUST"                ) \
    X(SE_M_GUST2               , "SE-M-GUST2"               ) \
    X(SE_M_DOUBLE_SLAP         , "SE-M-DOUBLE-SLAP"         ) \
    X(SE_M_DOUBLE_TEAM         , "SE-M-DOUBLE-TEAM"         ) \
    X(SE_M_RAZOR_WIND          , "SE-M-RAZOR-WIND"          ) \
    X(SE_M_ICY_WIND            , "SE-M-ICY-WIND"            ) \
    X(SE_M_THUNDER_WAVE        , "SE-M-THUNDER-WAVE"        ) \
    X(SE_M_COMET_PUNCH         , "SE-M-COMET-PUNCH"         ) \
    X(SE_M_MEGA_KICK           , "SE-M-MEGA-KICK"           ) \
    X(SE_M_MEGA_KICK2          , "SE-M-MEGA-KICK2"          ) \
    X(SE_M_CRABHAMMER          , "SE-M-CRABHAMMER"          ) \
    X(SE_M_JUMP_KICK           , "SE-M-JUMP-KICK"           ) \
    X(SE_M_FLAME_WHEEL         , "SE-M-FLAME-WHEEL"         ) \
    X(SE_M_FLAME_WHEEL2        , "SE-M-FLAME-WHEEL2"        ) \
    X(SE_M_FLAMETHROWER        , "SE-M-FLAMETHROWER"        ) \
    X(SE_M_FIRE_PUNCH          , "SE-M-FIRE-PUNCH"          ) \
    X(SE_M_TOXIC               , "SE-M-TOXIC"               ) \
    X(SE_M_SACRED_FIRE         , "SE-M-SACRED-FIRE"         ) \
    X(SE_M_SACRED_FIRE2        , "SE-M-SACRED-FIRE2"        ) \
    X(SE_M_EMBER               , "SE-M-EMBER"               ) \
    X(SE_M_TAKE_DOWN           , "SE-M-TAKE-DOWN"           ) \
    X(SE_M_BLIZZARD            , "SE-M-BLIZZARD"            ) \
    X(SE_M_BLIZZARD2           , "SE-M-BLIZZARD2"           ) \
    X(SE_M_SCRATCH             , "SE-M-SCRATCH"             ) \
    X(SE_M_VICEGRIP            , "SE-M-VICEGRIP"            ) \
    X(SE_M_WING_ATTACK         , "SE-M-WING-ATTACK"         ) \
    X(SE_M_FLY                 , "SE-M-FLY"                 ) \
    X(SE_M_SAND_ATTACK         , "SE-M-SAND-ATTACK"         ) \
    X(SE_M_RAZOR_WIND2         , "SE-M-RAZOR-WIND2"         ) \
    X(SE_M_BITE                , "SE-M-BITE"                ) \
    X(SE_M_HEADBUTT            , "SE-M-HEADBUTT"            ) \
    X(SE_M_SURF                , "SE-M-SURF"                ) \
    X(SE_M_HYDRO_PUMP          , "SE-M-HYDRO-PUMP"          ) \
    X(SE_M_WHIRLPOOL           , "SE-M-WHIRLPOOL"           ) \
    X(SE_M_HORN_ATTACK         , "SE-M-HORN-ATTACK"         ) \
    X(SE_M_TAIL_WHIP           , "SE-M-TAIL-WHIP"           ) \
    X(SE_M_MIST                , "SE-M-MIST"                ) \
    X(SE_M_POISON_POWDER       , "SE-M-POISON-POWDER"       ) \
    X(SE_M_BIND                , "SE-M-BIND"                ) \
    X(SE_M_DRAGON_RAGE         , "SE-M-DRAGON-RAGE"         ) \
    X(SE_M_SING                , "SE-M-SING"                ) \
    X(SE_M_PERISH_SONG         , "SE-M-PERISH-SONG"         ) \
    X(SE_M_PAY_DAY             , "SE-M-PAY-DAY"             ) \
    X(SE_M_DIG                 , "SE-M-DIG"                 ) \
    X(SE_M_DIZZY_PUNCH         , "SE-M-DIZZY-PUNCH"         ) \
    X(SE_M_SELF_DESTRUCT       , "SE-M-SELF-DESTRUCT"       ) \
    X(SE_M_EXPLOSION           , "SE-M-EXPLOSION"           ) \
    X(SE_M_ABSORB_2            , "SE-M-ABSORB-2"            ) \
    X(SE_M_ABSORB              , "SE-M-ABSORB"              ) \
    X(SE_M_SCREECH             , "SE-M-SCREECH"             ) \
    X(SE_M_BUBBLE_BEAM         , "SE-M-BUBBLE-BEAM"         ) \
    X(SE_M_BUBBLE_BEAM2        , "SE-M-BUBBLE-BEAM2"        ) \
    X(SE_M_SUPERSONIC          , "SE-M-SUPERSONIC"          ) \
    X(SE_M_BELLY_DRUM          , "SE-M-BELLY-DRUM"          ) \
    X(SE_M_METRONOME           , "SE-M-METRONOME"           ) \
    X(SE_M_BONEMERANG          , "SE-M-BONEMERANG"          ) \
    X(SE_M_LICK                , "SE-M-LICK"                ) \
    X(SE_M_PSYBEAM             , "SE-M-PSYBEAM"             ) \
    X(SE_M_FAINT_ATTACK        , "SE-M-FAINT-ATTACK"        ) \
    X(SE_M_SWORDS_DANCE        , "SE-M-SWORDS-DANCE"        ) \
    X(SE_M_LEER                , "SE-M-LEER"                ) \
    X(SE_M_SWAGGER             , "SE-M-SWAGGER"             ) \
    X(SE_M_SWAGGER2            , "SE-M-SWAGGER2"            ) \
    X(SE_M_HEAL_BELL           , "SE-M-HEAL-BELL"           ) \
    X(SE_M_CONFUSE_RAY         , "SE-M-CONFUSE-RAY"         ) \
    X(SE_M_SNORE               , "SE-M-SNORE"               ) \
    X(SE_M_BRICK_BREAK         , "SE-M-BRICK-BREAK"         ) \
    X(SE_M_GIGA_DRAIN          , "SE-M-GIGA-DRAIN"          ) \
    X(SE_M_PSYBEAM2            , "SE-M-PSYBEAM2"            ) \
    X(SE_M_SOLAR_BEAM          , "SE-M-SOLAR-BEAM"          ) \
    X(SE_M_PETAL_DANCE         , "SE-M-PETAL-DANCE"         ) \
    X(SE_M_TELEPORT            , "SE-M-TELEPORT"            ) \
    X(SE_M_MINIMIZE            , "SE-M-MINIMIZE"            ) \
    X(SE_M_SKETCH              , "SE-M-SKETCH"              ) \
    X(SE_M_SWIFT               , "SE-M-SWIFT"               ) \
    X(SE_M_REFLECT             , "SE-M-REFLECT"             ) \
    X(SE_M_BARRIER             , "SE-M-BARRIER"             ) \
    X(SE_M_DETECT              , "SE-M-DETECT"              ) \
    X(SE_M_LOCK_ON             , "SE-M-LOCK-ON"             ) \
    X(SE_M_MOONLIGHT           , "SE-M-MOONLIGHT"           ) \
    X(SE_M_CHARM               , "SE-M-CHARM"               ) \
    X(SE_M_CHARGE              , "SE-M-CHARGE"              ) \
    X(SE_M_STRENGTH            , "SE-M-STRENGTH"            ) \
    X(SE_M_HYPER_BEAM          , "SE-M-HYPER-BEAM"          ) \
    X(SE_M_WATERFALL           , "SE-M-WATERFALL"           ) \
    X(SE_M_REVERSAL            , "SE-M-REVERSAL"            ) \
    X(SE_M_ACID_ARMOR          , "SE-M-ACID-ARMOR"          ) \
    X(SE_M_SANDSTORM           , "SE-M-SANDSTORM"           ) \
    X(SE_M_TRI_ATTACK          , "SE-M-TRI-ATTACK"          ) \
    X(SE_M_TRI_ATTACK2         , "SE-M-TRI-ATTACK2"         ) \
    X(SE_M_ENCORE              , "SE-M-ENCORE"              ) \
    X(SE_M_ENCORE2             , "SE-M-ENCORE2"             ) \
    X(SE_M_BATON_PASS          , "SE-M-BATON-PASS"          ) \
    X(SE_M_MILK_DRINK          , "SE-M-MILK-DRINK"          ) \
    X(SE_M_ATTRACT             , "SE-M-ATTRACT"             ) \
    X(SE_M_ATTRACT2            , "SE-M-ATTRACT2"            ) \
    X(SE_M_MORNING_SUN         , "SE-M-MORNING-SUN"         ) \
    X(SE_M_FLATTER             , "SE-M-FLATTER"             ) \
    X(SE_M_SAND_TOMB           , "SE-M-SAND-TOMB"           ) \
    X(SE_M_GRASSWHISTLE        , "SE-M-GRASSWHISTLE"        ) \
    X(SE_M_SPIT_UP             , "SE-M-SPIT-UP"             ) \
    X(SE_M_DIVE                , "SE-M-DIVE"                ) \
    X(SE_M_EARTHQUAKE          , "SE-M-EARTHQUAKE"          ) \
    X(SE_M_TWISTER             , "SE-M-TWISTER"             ) \
    X(SE_M_SWEET_SCENT         , "SE-M-SWEET-SCENT"         ) \
    X(SE_M_YAWN                , "SE-M-YAWN"                ) \
    X(SE_M_SKY_UPPERCUT        , "SE-M-SKY-UPPERCUT"        ) \
    X(SE_M_STAT_INCREASE       , "SE-M-STAT-INCREASE"       ) \
    X(SE_M_HEAT_WAVE           , "SE-M-HEAT-WAVE"           ) \
    X(SE_M_UPROAR              , "SE-M-UPROAR"              ) \
    X(SE_M_HAIL                , "SE-M-HAIL"                ) \
    X(SE_M_COSMIC_POWER        , "SE-M-COSMIC-POWER"        ) \
    X(SE_M_TEETER_DANCE        , "SE-M-TEETER-DANCE"        ) \
    X(SE_M_STAT_DECREASE       , "SE-M-STAT-DECREASE"       ) \
    X(SE_M_HAZE                , "SE-M-HAZE"                ) \
    X(SE_M_HYPER_BEAM2         , "SE-M-HYPER-BEAM2"         ) \
    X(SE_RG_DOOR               , "SE-RG-DOOR"               ) \
    X(SE_RG_CARD_FLIP          , "SE-RG-CARD-FLIP"          ) \
    X(SE_RG_CARD_FLIPPING      , "SE-RG-CARD-FLIPPING"      ) \
    X(SE_RG_CARD_OPEN          , "SE-RG-CARD-OPEN"          ) \
    X(SE_RG_BAG_CURSOR         , "SE-RG-BAG-CURSOR"         ) \
    X(SE_RG_BAG_POCKET         , "SE-RG-BAG-POCKET"         ) \
    X(SE_RG_BALL_CLICK         , "SE-RG-BALL-CLICK"         ) \
    X(SE_RG_SHOP               , "SE-RG-SHOP"               ) \
    X(SE_RG_SS_ANNE_HORN       , "SE-RG-SS-ANNE-HORN"       ) \
    X(SE_RG_HELP_OPEN          , "SE-RG-HELP-OPEN"          ) \
    X(SE_RG_HELP_CLOSE         , "SE-RG-HELP-CLOSE"         ) \
    X(SE_RG_HELP_ERROR         , "SE-RG-HELP-ERROR"         ) \
    X(SE_RG_DEOXYS_MOVE        , "SE-RG-DEOXYS-MOVE"        ) \
    X(SE_RG_POKE_JUMP_SUCCESS  , "SE-RG-POKE-JUMP-SUCCESS"  ) \
    X(SE_RG_POKE_JUMP_FAILURE  , "SE-RG-POKE-JUMP-FAILURE"  ) \
    X(SE_PHONE_CALL            , "SE-PHONE-CALL"            ) \
    X(SE_PHONE_CLICK           , "SE-PHONE-CLICK"           ) \
    X(SE_ARENA_TIMEUP1         , "SE-ARENA-TIMEUP1"         ) \
    X(SE_ARENA_TIMEUP2         , "SE-ARENA-TIMEUP2"         ) \
    X(SE_PIKE_CURTAIN_CLOSE    , "SE-PIKE-CURTAIN-CLOSE"    ) \
    X(SE_PIKE_CURTAIN_OPEN     , "SE-PIKE-CURTAIN-OPEN"     ) \
    X(SE_SUDOWOODO_SHAKE       , "SE-SUDOWOODO-SHAKE"       ) \
    X(MUS_DUMMY                , "MUS-DUMMY"                ) \
    X(PH_TRAP_BLEND            , "PH-TRAP-BLEND"            ) \
    X(PH_TRAP_HELD             , "PH-TRAP-HELD"             ) \
    X(PH_TRAP_SOLO             , "PH-TRAP-SOLO"             ) \
    X(PH_FACE_BLEND            , "PH-FACE-BLEND"            ) \
    X(PH_FACE_HELD             , "PH-FACE-HELD"             ) \
    X(PH_FACE_SOLO             , "PH-FACE-SOLO"             ) \
    X(PH_CLOTH_BLEND           , "PH-CLOTH-BLEND"           ) \
    X(PH_CLOTH_HELD            , "PH-CLOTH-HELD"            ) \
    X(PH_CLOTH_SOLO            , "PH-CLOTH-SOLO"            ) \
    X(PH_DRESS_BLEND           , "PH-DRESS-BLEND"           ) \
    X(PH_DRESS_HELD            , "PH-DRESS-HELD"            ) \
    X(PH_DRESS_SOLO            , "PH-DRESS-SOLO"            ) \
    X(PH_FLEECE_BLEND          , "PH-FLEECE-BLEND"          ) \
    X(PH_FLEECE_HELD           , "PH-FLEECE-HELD"           ) \
    X(PH_FLEECE_SOLO           , "PH-FLEECE-SOLO"           ) \
    X(PH_KIT_BLEND             , "PH-KIT-BLEND"             ) \
    X(PH_KIT_HELD              , "PH-KIT-HELD"              ) \
    X(PH_KIT_SOLO              , "PH-KIT-SOLO"              ) \
    X(PH_PRICE_BLEND           , "PH-PRICE-BLEND"           ) \
    X(PH_PRICE_HELD            , "PH-PRICE-HELD"            ) \
    X(PH_PRICE_SOLO            , "PH-PRICE-SOLO"            ) \
    X(PH_LOT_BLEND             , "PH-LOT-BLEND"             ) \
    X(PH_LOT_HELD              , "PH-LOT-HELD"              ) \
    X(PH_LOT_SOLO              , "PH-LOT-SOLO"              ) \
    X(PH_GOAT_BLEND            , "PH-GOAT-BLEND"            ) \
    X(PH_GOAT_HELD             , "PH-GOAT-HELD"             ) \
    X(PH_GOAT_SOLO             , "PH-GOAT-SOLO"             ) \
    X(PH_THOUGHT_BLEND         , "PH-THOUGHT-BLEND"         ) \
    X(PH_THOUGHT_HELD          , "PH-THOUGHT-HELD"          ) \
    X(PH_THOUGHT_SOLO          , "PH-THOUGHT-SOLO"          ) \
    X(PH_CHOICE_BLEND          , "PH-CHOICE-BLEND"          ) \
    X(PH_CHOICE_HELD           , "PH-CHOICE-HELD"           ) \
    X(PH_CHOICE_SOLO           , "PH-CHOICE-SOLO"           ) \
    X(PH_MOUTH_BLEND           , "PH-MOUTH-BLEND"           ) \
    X(PH_MOUTH_HELD            , "PH-MOUTH-HELD"            ) \
    X(PH_MOUTH_SOLO            , "PH-MOUTH-SOLO"            ) \
    X(PH_FOOT_BLEND            , "PH-FOOT-BLEND"            ) \
    X(PH_FOOT_HELD             , "PH-FOOT-HELD"             ) \
    X(PH_FOOT_SOLO             , "PH-FOOT-SOLO"             ) \
    X(PH_GOOSE_BLEND           , "PH-GOOSE-BLEND"           ) \
    X(PH_GOOSE_HELD            , "PH-GOOSE-HELD"            ) \
    X(PH_GOOSE_SOLO            , "PH-GOOSE-SOLO"            ) \
    X(PH_STRUT_BLEND           , "PH-STRUT-BLEND"           ) \
    X(PH_STRUT_HELD            , "PH-STRUT-HELD"            ) \
    X(PH_STRUT_SOLO            , "PH-STRUT-SOLO"            ) \
    X(PH_CURE_BLEND            , "PH-CURE-BLEND"            ) \
    X(PH_CURE_HELD             , "PH-CURE-HELD"             ) \
    X(PH_CURE_SOLO             , "PH-CURE-SOLO"             ) \
    X(PH_NURSE_BLEND           , "PH-NURSE-BLEND"           ) \
    X(PH_NURSE_HELD            , "PH-NURSE-HELD"            ) \
    X(PH_NURSE_SOLO            , "PH-NURSE-SOLO"            ) \

// Create BGM list
#define X(songId, name) static const u8 sBGMName_##songId[] = _(name);
SOUND_LIST_BGM
#undef X

#define X(songId, name) sBGMName_##songId,
static const u8 *const gBGMNames[] =
{
SOUND_LIST_BGM
};
#undef X

// Create SE list
#define X(songId, name) static const u8 sSEName_##songId[] = _(name);
SOUND_LIST_SE
#undef X

#define X(songId, name) sSEName_##songId,
static const u8 *const gSENames[] =
{
SOUND_LIST_SE
};
#undef X

// *******************************
// Actions Other
static void Task_WaitFadeAccessPC(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FlagSet(FLAG_SYS_PC_FROM_DEBUG_MENU);
        EnterPokeStorage(2);
    }
}
static void DebugAction_AccessPC(u8 taskId)
{
    Debug_DestroyMenu_Full_Script(taskId, EventScript_PC);
}

static void DebugAction_HatchAnEgg(u8 taskId)
{
    Debug_DestroyMenu_Full_Script(taskId, Debug_HatchAnEgg);
}

#endif
