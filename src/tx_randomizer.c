#include "global.h"
// #include "option_menu.h"
#include "main.h"
#include "menu.h"
#include "scanline_effect.h"
#include "palette.h"
#include "sprite.h"
#include "sound.h"
#include "task.h"
#include "malloc.h"
#include "bg.h"
#include "gpu_regs.h"
#include "window.h"
#include "text.h"
#include "text_window.h"
#include "international_string_util.h"
#include "strings.h"
#include "string_util.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"
#include "battle_main.h"
#include "tx_randomizer_and_challenges.h"
#include "pokemon.h"

#define Y_DIFF 16 // Difference in pixels between items.

// Menu items page 1
enum
{
    MENUITEM_RANDOM_ON_OFF,
    MENUITEM_RANDOM_WILD_PKMN,
    MENUITEM_RANDOM_TRAINER,
    MENUITEM_RANDOM_SIMILAR_EVOLUTION_LEVEL,
    MENUITEM_RANDOM_INCLUDE_LEGENDARIES,
    MENUITEM_RANDOM_TYPE,
    MENUITEM_RANDOM_MOVES,
    MENUITEM_RANDOM_ABILITIES,
    MENUITEM_RANDOM_EVOLUTIONS,
    MENUITEM_RANDOM_EVOLUTIONS_METHODE,
    MENUITEM_RANDOM_TYPE_EFFEC,
    MENUITEM_RANDOM_CHAOS,
    MENUITEM_SAVE,
    MENUITEM_COUNT,
};

// Window Ids
enum
{
    WIN_OPTIONS,
    WIN_DESCRIPTION
};

struct tx_randomizer_OptionsMenu
{
    u8 sel[MENUITEM_COUNT];
    int menuCursor;
    int visibleCursor;
};

// functions
static void tx_randomizer_Task_OptionMenuFadeIn(u8 taskId);
static void tx_randomizer_Task_OptionMenuProcessInput(u8 taskId);
static void tx_randomizer_Task_OptionMenuSave(u8 taskId);
static void tx_randomizer_Task_OptionMenuFadeOut(u8 taskId);
static void tx_randomizer_HighlightOptionMenuItem(int cursor);
static void tx_randomizer_DrawDescriptions(void);
static void tx_randomizer_DrawDescriptionsFirstTime(void);
static void tx_randomizer_DrawOptionMenuTexts(void);
static void tx_randomizer_DrawOptionMenuTextsLeft(void);
static void DrawBgWindowFrames(void);
static int tx_randomizer_TwoOptions_ProcessInput(int selection);

static void DrawChoices_Random_OffOn(int selection, int y, u8 textSpeed);
static void DrawChoices_Random_OffChaos(int selection, int y, u8 textSpeed);
static void DrawChoices_Random_OffRandom(int selection, int y, u8 textSpeed);

struct
{
    void (*drawChoices)(int selection, int y, u8 textSpeed);
    int (*processInput)(int selection);
} static const sItemFunctions[MENUITEM_COUNT] =
{
    [MENUITEM_RANDOM_ON_OFF]                  = {DrawChoices_Random_OffOn, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_WILD_PKMN]               = {DrawChoices_Random_OffRandom, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_TRAINER]                 = {DrawChoices_Random_OffRandom, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_SIMILAR_EVOLUTION_LEVEL] = {DrawChoices_Random_OffOn, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_INCLUDE_LEGENDARIES]     = {DrawChoices_Random_OffOn, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_TYPE]                    = {DrawChoices_Random_OffRandom, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_MOVES]                   = {DrawChoices_Random_OffRandom, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_ABILITIES]               = {DrawChoices_Random_OffRandom, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_EVOLUTIONS]              = {DrawChoices_Random_OffRandom, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_EVOLUTIONS_METHODE]      = {DrawChoices_Random_OffRandom, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_TYPE_EFFEC]              = {DrawChoices_Random_OffRandom, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_RANDOM_CHAOS]                   = {DrawChoices_Random_OffChaos, tx_randomizer_TwoOptions_ProcessInput},
    [MENUITEM_SAVE] = {NULL, NULL},
};

// EWRAM vars
EWRAM_DATA struct tx_randomizer_OptionsMenu *sRandomizerOptions = NULL;

// const rom data
static const u16 sOptionMenuText_Pal[] = INCBIN_U16("graphics/interface/option_menu_text.gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/interface/option_menu_equals_sign.4bpp");

static const u8 gText_Randomizer[] =                _("RANDOMIZER");
static const u8 gText_WildPkmn[] =                  _("WILD POKEMON");
static const u8 gText_Trainer[] =                   _("TRAINER");
static const u8 gText_SimiliarEvolutionLevel[] =    _("EVO STAGES");
static const u8 gText_InlcudeLegendaries[]=         _("LEGENDARIES");
static const u8 gText_Type[] =                      _("TYPE");
static const u8 gText_Moves[] =                     _("MOVES");
static const u8 gText_Abilities[] =                 _("ABILTIES");
static const u8 gText_Evolutions[] =                _("EVOLUTIONS");
static const u8 gText_EvolutionMethodes[] =         _("EVO METHODES");
static const u8 gText_TypeEff[] =                   _("EFFECTIVENESS");
static const u8 gText_Chaos[] =                     _("CHAOS MODE");
static const u8 gText_Save[] =                      _("SAVE");

static const u8 *const sOptionMenuItemNames[MENUITEM_COUNT] =
{
    [MENUITEM_RANDOM_ON_OFF]                  = gText_Randomizer,
    [MENUITEM_RANDOM_WILD_PKMN]               = gText_WildPkmn,
    [MENUITEM_RANDOM_TRAINER]                 = gText_Trainer,
    [MENUITEM_RANDOM_SIMILAR_EVOLUTION_LEVEL] = gText_SimiliarEvolutionLevel,
    [MENUITEM_RANDOM_INCLUDE_LEGENDARIES]     = gText_InlcudeLegendaries,
    [MENUITEM_RANDOM_TYPE]                    = gText_Type,
    [MENUITEM_RANDOM_MOVES]                   = gText_Moves,
    [MENUITEM_RANDOM_ABILITIES]               = gText_Abilities,
    [MENUITEM_RANDOM_EVOLUTIONS]              = gText_Evolutions,
    [MENUITEM_RANDOM_EVOLUTIONS_METHODE]      = gText_EvolutionMethodes,
    [MENUITEM_RANDOM_TYPE_EFFEC]              = gText_TypeEff,
    [MENUITEM_RANDOM_CHAOS]                   = gText_Chaos,
    [MENUITEM_SAVE]                           = gText_Save,
};

static const u8 gText_Gray_WildPkmn[] =                  _("{COLOR GREEN}{SHADOW LIGHT_GREEN}WILD POKEMON");
static const u8 gText_Gray_Trainer[] =                   _("{COLOR GREEN}{SHADOW LIGHT_GREEN}TRAINER");
static const u8 gText_Gray_SimiliarEvolutionLevel[] =    _("{COLOR GREEN}{SHADOW LIGHT_GREEN}EVO STAGES");
static const u8 gText_Gray_InlcudeLegendaries[]=         _("{COLOR GREEN}{SHADOW LIGHT_GREEN}LEGENDARIES");
static const u8 gText_Gray_Type[] =                      _("{COLOR GREEN}{SHADOW LIGHT_GREEN}TYPE");
static const u8 gText_Gray_Moves[] =                     _("{COLOR GREEN}{SHADOW LIGHT_GREEN}MOVES");
static const u8 gText_Gray_Abilities[] =                 _("{COLOR GREEN}{SHADOW LIGHT_GREEN}ABILTIES");
static const u8 gText_Gray_Evolutions[] =                _("{COLOR GREEN}{SHADOW LIGHT_GREEN}EVOLUTIONS");
static const u8 gText_Gray_EvolutionMethodes[] =         _("{COLOR GREEN}{SHADOW LIGHT_GREEN}EVO METHODES");
static const u8 gText_Gray_TypeEff[] =                   _("{COLOR GREEN}{SHADOW LIGHT_GREEN}EFFECTIVENESS");
static const u8 gText_Gray_Chaos[] =                     _("{COLOR GREEN}{SHADOW LIGHT_GREEN}CHAOS MODE");

static const u8 *const sOptionMenuItemNamesGray[MENUITEM_COUNT] =
{
    [MENUITEM_RANDOM_ON_OFF]                  = gText_Randomizer,
    [MENUITEM_RANDOM_WILD_PKMN]               = gText_Gray_WildPkmn,
    [MENUITEM_RANDOM_TRAINER]                 = gText_Gray_Trainer,
    [MENUITEM_RANDOM_SIMILAR_EVOLUTION_LEVEL] = gText_Gray_SimiliarEvolutionLevel,
    [MENUITEM_RANDOM_INCLUDE_LEGENDARIES]     = gText_Gray_InlcudeLegendaries,
    [MENUITEM_RANDOM_TYPE]                    = gText_Gray_Type,
    [MENUITEM_RANDOM_MOVES]                   = gText_Gray_Moves,
    [MENUITEM_RANDOM_ABILITIES]               = gText_Gray_Abilities,
    [MENUITEM_RANDOM_EVOLUTIONS]              = gText_Gray_Evolutions,
    [MENUITEM_RANDOM_EVOLUTIONS_METHODE]      = gText_Gray_EvolutionMethodes,
    [MENUITEM_RANDOM_TYPE_EFFEC]              = gText_Gray_TypeEff,
    [MENUITEM_RANDOM_CHAOS]                   = gText_Gray_Chaos,
    [MENUITEM_SAVE]                           = gText_Save,
};

static const u8 gText_Description_Randomizer[]              = _("Play the game randomized.\nSettings below:");
static const u8 gText_Description_WildPokemon[]             = _("Randomize wild encounter.");
static const u8 gText_Description_Random_Trainer[]          = _("Randomize enemy trainer parties.");
static const u8 gText_Description_SimiliarEvolutionLevel[]  = _("Ensure randomized mon are of the same\nevolution stage as the base species.");
static const u8 gText_Description_IncludeLegendaries[]      = _("Include legendary Pokémon.");
static const u8 gText_Description_Random_Types[]            = _("Randomize all Pokémon types.");
static const u8 gText_Description_Random_Moves[]            = _("Randomize all Pokémon moves.");
static const u8 gText_Description_Random_Abilities[]        = _("Randomize all Pokémon abilities.");
static const u8 gText_Description_Random_Evos[]             = _("Randomize all Pokémon evolutions.");
static const u8 gText_Description_Random_Evo_Methodes[]     = _("Randomize all Pokémon evolution\nmethodes.");
static const u8 gText_Description_Random_Effectiveness[]    = _("Randomize type effectiveness.");
static const u8 gText_Description_Chaos_Mode[]              = _("Enable {COLOR RED}{SHADOW LIGHT_RED}Chaos mode\nNOT recommended!");
static const u8 gText_Description_Save[]                    = _("Save choices and continue...");

static const u8 *const sOptionMenuItemDescriptions[MENUITEM_COUNT] =
{
    [MENUITEM_RANDOM_ON_OFF]                  = gText_Description_Randomizer,
    [MENUITEM_RANDOM_WILD_PKMN]               = gText_Description_WildPokemon,
    [MENUITEM_RANDOM_TRAINER]                 = gText_Description_Random_Trainer,
    [MENUITEM_RANDOM_SIMILAR_EVOLUTION_LEVEL] = gText_Description_SimiliarEvolutionLevel,
    [MENUITEM_RANDOM_INCLUDE_LEGENDARIES]     = gText_Description_IncludeLegendaries,
    [MENUITEM_RANDOM_TYPE]                    = gText_Description_Random_Types,
    [MENUITEM_RANDOM_MOVES]                   = gText_Description_Random_Moves,
    [MENUITEM_RANDOM_ABILITIES]               = gText_Description_Random_Abilities,
    [MENUITEM_RANDOM_EVOLUTIONS]              = gText_Description_Random_Evos,
    [MENUITEM_RANDOM_EVOLUTIONS_METHODE]      = gText_Description_Random_Evo_Methodes,
    [MENUITEM_RANDOM_TYPE_EFFEC]              = gText_Description_Random_Effectiveness,
    [MENUITEM_RANDOM_CHAOS]                   = gText_Description_Chaos_Mode,
    [MENUITEM_SAVE]                           = gText_Description_Save,
};

static const struct WindowTemplate sDifficultyChallengesOptionMenuWinTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 12,
        .paletteNum = 1,
        .baseBlock = 2,
    },
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 1,
        .baseBlock = 314,
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
    {
       .bg = 0,
       .charBaseIndex = 1,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0
    },
    {
       .bg = 1,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0
    },
};

static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};

// code
static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void DrawChoices(u32 id, int y, u8 textSpeed)
{
    if (sItemFunctions[id].drawChoices != NULL)
        sItemFunctions[id].drawChoices(sRandomizerOptions->sel[id], y, textSpeed);
}

void CB2_InitRandomizerMenu(void)
{
    u32 i, taskId;
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void*)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
        ResetBgPositions();
        InitWindows(sDifficultyChallengesOptionMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, 1);
        SetGpuReg(REG_OFFSET_WINOUT, 35);
        SetGpuReg(REG_OFFSET_BLDCNT, 193);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, 0, sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, 0x70, 0x20);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sOptionMenuText_Pal, 16, sizeof(sOptionMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(WIN_DESCRIPTION);
        tx_randomizer_DrawDescriptionsFirstTime();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_OPTIONS);
        tx_randomizer_DrawOptionMenuTexts();
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
        taskId = CreateTask(tx_randomizer_Task_OptionMenuFadeIn, 0);

        //tx_randomizer_and_challenges
        gSaveBlock1Ptr->tx_Random_WildPokemon           = TX_RANDOM_WILD_POKEMON;
        gSaveBlock1Ptr->tx_Random_Trainer               = TX_RANDOM_TRAINER;
        gSaveBlock1Ptr->tx_Random_Similar               = TX_RANDOM_SIMILAR;
        gSaveBlock1Ptr->tx_Random_MapBased              = TX_RANDOM_MAP_BASED;
        gSaveBlock1Ptr->tx_Random_IncludeLegendaries    = TX_RANDOM_INCLUDE_LEGENDARIES;
        gSaveBlock1Ptr->tx_Random_Type                  = TX_RANDOM_TYPE;
        gSaveBlock1Ptr->tx_Random_Moves                 = TX_RANDOM_MOVES;
        gSaveBlock1Ptr->tx_Random_Abilities             = TX_RANDOM_ABILITIES;
        gSaveBlock1Ptr->tx_Random_Evolutions            = TX_RANDOM_EVOLUTION;
        gSaveBlock1Ptr->tx_Random_EvolutionMethodes     = TX_RANDOM_EVOLUTION_METHODE;
        gSaveBlock1Ptr->tx_Random_TypeEffectiveness     = TX_RANDOM_TYPE_EFFECTIVENESS;
        gSaveBlock1Ptr->tx_Random_Chaos                 = TX_RANDOM_CHAOS_MODE;

        sRandomizerOptions = AllocZeroed(sizeof(*sRandomizerOptions));
        sRandomizerOptions->sel[MENUITEM_RANDOM_ON_OFF] = FALSE;
        sRandomizerOptions->sel[MENUITEM_RANDOM_WILD_PKMN]                = gSaveBlock1Ptr->tx_Random_WildPokemon;
        sRandomizerOptions->sel[MENUITEM_RANDOM_TRAINER]                  = gSaveBlock1Ptr->tx_Random_Trainer;
        sRandomizerOptions->sel[MENUITEM_RANDOM_SIMILAR_EVOLUTION_LEVEL]  = gSaveBlock1Ptr->tx_Random_Similar;
        sRandomizerOptions->sel[MENUITEM_RANDOM_INCLUDE_LEGENDARIES]      = gSaveBlock1Ptr->tx_Random_IncludeLegendaries;
        sRandomizerOptions->sel[MENUITEM_RANDOM_TYPE]                     = gSaveBlock1Ptr->tx_Random_Type;
        sRandomizerOptions->sel[MENUITEM_RANDOM_MOVES]                    = gSaveBlock1Ptr->tx_Random_Moves;
        sRandomizerOptions->sel[MENUITEM_RANDOM_ABILITIES]                = gSaveBlock1Ptr->tx_Random_Abilities;
        sRandomizerOptions->sel[MENUITEM_RANDOM_EVOLUTIONS]               = gSaveBlock1Ptr->tx_Random_Evolutions;
        sRandomizerOptions->sel[MENUITEM_RANDOM_EVOLUTIONS_METHODE]       = gSaveBlock1Ptr->tx_Random_EvolutionMethodes;
        sRandomizerOptions->sel[MENUITEM_RANDOM_TYPE_EFFEC]               = gSaveBlock1Ptr->tx_Random_TypeEffectiveness;
        sRandomizerOptions->sel[MENUITEM_RANDOM_CHAOS]                    = gSaveBlock1Ptr->tx_Random_Chaos;

        tx_randomizer_DrawOptionMenuTexts();

        for (i = 0; i < TX_MENU_ITEMS_PER_PAGE; i++)
            DrawChoices(i, i * Y_DIFF, 0xFF);

        tx_randomizer_HighlightOptionMenuItem(sRandomizerOptions->menuCursor);

        CopyWindowToVram(WIN_OPTIONS, 3);
        gMain.state++;
        break;
    case 11:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void tx_randomizer_Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = tx_randomizer_Task_OptionMenuProcessInput;
}

static void ScrollMenu(int direction)
{
    int menuItem, pos;
    if (direction == 0) // scroll down
        menuItem = sRandomizerOptions->menuCursor + 2, pos = TX_MENU_ITEMS_PER_PAGE-1;
    else
        menuItem = sRandomizerOptions->menuCursor - 3, pos = 0;

    // Hide one
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF, PIXEL_FILL(0));
    // Show one
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, Y_DIFF * pos, 26 * 8, Y_DIFF);
    // Print
    DrawChoices(menuItem, pos * Y_DIFF, 0xFF);
    
    if (sRandomizerOptions->sel[MENUITEM_RANDOM_ON_OFF] == FALSE)
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNamesGray[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    else
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNames[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    CopyWindowToVram(WIN_OPTIONS, 2);
}

static void ScrollAll(int direction) // to bottom or top
{
    int i, y, menuItem, pos;
    int scrollCount = MENUITEM_COUNT - 4;
    // Move items up/down
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF * scrollCount, PIXEL_FILL(0));

    // Clear moved items
    // if (direction == 0)
    // {
    //     y = 6 - scrollCount;
    //     if (y < 0)
    //         y = 6;
    //     y *= Y_DIFF;
    // }
    // else
    // {
    //     y = 0;
    // }
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, 0, 26 * 8, Y_DIFF * scrollCount);
    
    // Print new texts
    for (i = 0; i < scrollCount; i++)
    {
        if (direction == 0) // From top to bottom
            menuItem = MENUITEM_COUNT - 1 - i, pos = 5 - i;
        else // From bottom to top
            menuItem = i, pos = i;
        DrawChoices(menuItem, pos * Y_DIFF, 0xFF);
        
        if (sRandomizerOptions->sel[MENUITEM_RANDOM_ON_OFF] == FALSE)
            AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNamesGray[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
        else
            AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNames[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    }
    CopyWindowToVram(WIN_OPTIONS, 2);
}

static void tx_randomizer_Task_OptionMenuProcessInput(u8 taskId)
{
    int i, scrollCount = 0;
    if (JOY_NEW(A_BUTTON))
    {
        if (sRandomizerOptions->menuCursor == MENUITEM_SAVE)
            gTasks[taskId].func = tx_randomizer_Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sRandomizerOptions->sel[MENUITEM_RANDOM_ON_OFF] == FALSE)
        {
            if (--sRandomizerOptions->menuCursor < 0)
            {
                sRandomizerOptions->visibleCursor = sRandomizerOptions->menuCursor = 3;
                ScrollAll(0);
                sRandomizerOptions->visibleCursor = 5;
                sRandomizerOptions->menuCursor = MENUITEM_COUNT - 1;
            }
            else
            {
                sRandomizerOptions->visibleCursor = 3;
                sRandomizerOptions->menuCursor = MENUITEM_COUNT - 4;
                ScrollAll(1);
                sRandomizerOptions->visibleCursor = sRandomizerOptions->menuCursor = 0;
            }
        }
        else if (sRandomizerOptions->visibleCursor == 3) // don't advance visible cursor until scrolled to the bottom
        {
            if (--sRandomizerOptions->menuCursor == sRandomizerOptions->visibleCursor - 1)
                sRandomizerOptions->visibleCursor--;
            else
                ScrollMenu(1);
        }
        else
        {
            if (--sRandomizerOptions->menuCursor < 0) // Scroll all the way to the bottom.
            {
                sRandomizerOptions->visibleCursor = sRandomizerOptions->menuCursor = 3;
                ScrollAll(0);
                sRandomizerOptions->visibleCursor = 5;
                sRandomizerOptions->menuCursor = MENUITEM_COUNT - 1;
            }
            else
            {
                sRandomizerOptions->visibleCursor--;
            }
        }
        tx_randomizer_HighlightOptionMenuItem(sRandomizerOptions->visibleCursor);
        tx_randomizer_DrawDescriptions();
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sRandomizerOptions->sel[MENUITEM_RANDOM_ON_OFF] == FALSE)
        {
            if (sRandomizerOptions->menuCursor == 0)
            {
                sRandomizerOptions->visibleCursor = sRandomizerOptions->menuCursor = 3;
                ScrollAll(0);
                sRandomizerOptions->visibleCursor = 5;
                sRandomizerOptions->menuCursor = MENUITEM_COUNT - 1;
            }
            else
            {
                sRandomizerOptions->visibleCursor = 3;
                sRandomizerOptions->menuCursor = MENUITEM_COUNT - 4;
                ScrollAll(1);
                sRandomizerOptions->visibleCursor = sRandomizerOptions->menuCursor = 0;
            }
        }
        else if (sRandomizerOptions->visibleCursor == 3) // don't advance visible cursor until scrolled to the bottom
        {
            if (++sRandomizerOptions->menuCursor == MENUITEM_COUNT - 2)
                sRandomizerOptions->visibleCursor++;
            else
                ScrollMenu(0);
        }
        else
        {
            if (++sRandomizerOptions->menuCursor >= MENUITEM_COUNT) // Scroll all the way to the top.
            {
                sRandomizerOptions->visibleCursor = 3;
                sRandomizerOptions->menuCursor = MENUITEM_COUNT - 4;
                ScrollAll(1);
                sRandomizerOptions->visibleCursor = sRandomizerOptions->menuCursor = 0;
            }
            else
            {
                sRandomizerOptions->visibleCursor++;
            }
        }
        tx_randomizer_HighlightOptionMenuItem(sRandomizerOptions->visibleCursor);
        tx_randomizer_DrawDescriptions();
    }
    else if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        int cursor = sRandomizerOptions->menuCursor;
        u8 previousOption = sRandomizerOptions->sel[cursor];
        if (sItemFunctions[cursor].processInput != NULL)
            sRandomizerOptions->sel[cursor] = sItemFunctions[cursor].processInput(previousOption);

        if (previousOption != sRandomizerOptions->sel[cursor])
            DrawChoices(cursor, sRandomizerOptions->visibleCursor * Y_DIFF, 0);

        if (cursor == 0)
        {
            tx_randomizer_DrawOptionMenuTextsLeft();

            tx_randomizer_HighlightOptionMenuItem(sRandomizerOptions->menuCursor);
        }
    }
}

static void tx_randomizer_Task_OptionMenuSave(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = tx_randomizer_Task_OptionMenuFadeOut;
}

void tx_randomizer_SaveData(void)
{
    if (sRandomizerOptions->sel[MENUITEM_RANDOM_ON_OFF] == TRUE)
    {
        gSaveBlock1Ptr->tx_Random_WildPokemon        = sRandomizerOptions->sel[MENUITEM_RANDOM_WILD_PKMN];
        gSaveBlock1Ptr->tx_Random_Trainer            = sRandomizerOptions->sel[MENUITEM_RANDOM_TRAINER];
        gSaveBlock1Ptr->tx_Random_Similar            = sRandomizerOptions->sel[MENUITEM_RANDOM_SIMILAR_EVOLUTION_LEVEL];
        gSaveBlock1Ptr->tx_Random_MapBased           = TX_RANDOM_MAP_BASED;
        gSaveBlock1Ptr->tx_Random_IncludeLegendaries = sRandomizerOptions->sel[MENUITEM_RANDOM_INCLUDE_LEGENDARIES];
        gSaveBlock1Ptr->tx_Random_Type               = sRandomizerOptions->sel[MENUITEM_RANDOM_TYPE];
        gSaveBlock1Ptr->tx_Random_Moves              = sRandomizerOptions->sel[MENUITEM_RANDOM_MOVES];
        gSaveBlock1Ptr->tx_Random_Abilities          = sRandomizerOptions->sel[MENUITEM_RANDOM_ABILITIES];
        gSaveBlock1Ptr->tx_Random_Evolutions         = sRandomizerOptions->sel[MENUITEM_RANDOM_EVOLUTIONS];
        gSaveBlock1Ptr->tx_Random_EvolutionMethodes  = sRandomizerOptions->sel[MENUITEM_RANDOM_EVOLUTIONS_METHODE];
        gSaveBlock1Ptr->tx_Random_TypeEffectiveness  = sRandomizerOptions->sel[MENUITEM_RANDOM_TYPE_EFFEC];
        gSaveBlock1Ptr->tx_Random_Chaos              = sRandomizerOptions->sel[MENUITEM_RANDOM_CHAOS];
    }
    else
    {
        gSaveBlock1Ptr->tx_Random_WildPokemon        = FALSE;
        gSaveBlock1Ptr->tx_Random_Trainer            = FALSE;
        gSaveBlock1Ptr->tx_Random_Similar            = FALSE;
        gSaveBlock1Ptr->tx_Random_MapBased           = FALSE;
        gSaveBlock1Ptr->tx_Random_IncludeLegendaries = FALSE;
        gSaveBlock1Ptr->tx_Random_Type               = FALSE;
        gSaveBlock1Ptr->tx_Random_Moves              = FALSE;
        gSaveBlock1Ptr->tx_Random_Abilities          = FALSE;
        gSaveBlock1Ptr->tx_Random_Evolutions         = FALSE;
        gSaveBlock1Ptr->tx_Random_EvolutionMethodes  = FALSE;
        gSaveBlock1Ptr->tx_Random_TypeEffectiveness  = FALSE;
        gSaveBlock1Ptr->tx_Random_Chaos              = FALSE;
    }

    FREE_AND_SET_NULL(sRandomizerOptions);
    RandomizeSpeciesListEWRAM(1);
    RandomizeTypeEffectivenessListEWRAM(1);
}

static void tx_randomizer_Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        // FREE_AND_SET_NULL(sRandomizerOptions);
        SetMainCallback2(gMain.savedCallback);
    }
}

static void tx_randomizer_HighlightOptionMenuItem(int cursor)
{
    cursor -= 2;
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(Y_DIFF, 224));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(cursor * Y_DIFF + 40, cursor * Y_DIFF + 56));
}

// Process Input functions
static int XOptions_ProcessInput(int x, int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (++selection > (x - 1))
            selection = 0;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (--selection < 0)
            selection = (x - 1);
    }
    return selection;
}

static int tx_randomizer_TwoOptions_ProcessInput(int selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
        selection ^= 1;

    return selection;
}

static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3)
{
    int xMid;
    int widthLeft = GetStringWidth(1, txt1, 0);
    int widthMid = GetStringWidth(1, txt2, 0);
    int widthRight = GetStringWidth(1, txt3, 0);

    widthMid -= (198 - 104);
    xMid = (widthLeft - widthMid - widthRight) / 2 + 104;
    return xMid;
}

static void tx_randomizer_DrawDescriptions(void)
{
    u8 n = sRandomizerOptions->menuCursor;
    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_DESCRIPTION, 1, sOptionMenuItemDescriptions[n], 8, 1, 0, NULL);
    CopyWindowToVram(WIN_DESCRIPTION, 3);
}
static void tx_randomizer_DrawDescriptionsFirstTime(void)
{
    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_DESCRIPTION, 1, sOptionMenuItemDescriptions[0], 8, 1, 0, NULL);
    CopyWindowToVram(WIN_DESCRIPTION, 3);
}

static void tx_randomizer_DrawOptionMenuTexts(void)
{
    u32 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < TX_MENU_ITEMS_PER_PAGE; i++)
    {
        if (sRandomizerOptions->sel[MENUITEM_RANDOM_ON_OFF] == FALSE)
            AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNamesGray[i], 8, (i * Y_DIFF) + 1, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNames[i], 8, (i * Y_DIFF) + 1, 0, NULL);
    }

    CopyWindowToVram(WIN_OPTIONS, 3);
}

static void tx_randomizer_DrawOptionMenuTextsLeft(void)
{
    u32 i;

    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, 0, 100, 100);
    for (i = 0; i < TX_MENU_ITEMS_PER_PAGE; i++)
    {
        if (sRandomizerOptions->sel[MENUITEM_RANDOM_ON_OFF] == FALSE)
            AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNamesGray[i], 8, (i * Y_DIFF) + 1, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNames[i], 8, (i * Y_DIFF) + 1, 0, NULL);
    }

    CopyWindowToVram(WIN_OPTIONS, 3);
}

// Draw Choices functions
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style, u8 textSpeed)
{
    u8 dst[16];
    u32 i;

    for (i = 0; *text != EOS && i <= 14; i++)
        dst[i] = *(text++);

    if (style != 0)
    {
        dst[2] = 4;
        dst[5] = 5;
    }

    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_OPTIONS, 1, dst, x, y + 1, textSpeed, NULL);
}

static const u8 gText_Off[]  = _("{COLOR DARK_GRAY}{SHADOW LIGHT_GRAY}OFF");
static const u8 gText_On[]   = _("{COLOR DARK_GRAY}{SHADOW LIGHT_GRAY}ON");
static const u8 gText_None[] = _("{COLOR DARK_GRAY}{SHADOW LIGHT_GRAY}NONE");
static void DrawChoices_Random_OffOn(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_On, GetStringRightAlignXOffset(1, gText_On, 198), y, styles[1], textSpeed);
}

static const u8 gText_Random_Chaos[] = _("{COLOR DARK_GRAY}{SHADOW LIGHT_GRAY}CHAOS");
static void DrawChoices_Random_OffChaos(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Random_Chaos, GetStringRightAlignXOffset(1, gText_Random_Chaos, 198), y, styles[1], textSpeed);
}

static const u8 gText_Random[]  = _("{COLOR DARK_GRAY}{SHADOW LIGHT_GRAY}RANDOM");
static void DrawChoices_Random_OffRandom(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Random, GetStringRightAlignXOffset(1, gText_Random, 198), y, styles[1], textSpeed);
}

#define TILE_TOP_CORNER_L 0x1A2 // 418
#define TILE_TOP_EDGE     0x1A3 // 419
#define TILE_TOP_CORNER_R 0x1A4 // 420
#define TILE_LEFT_EDGE    0x1A5 // 421
#define TILE_RIGHT_EDGE   0x1A7 // 423
#define TILE_BOT_CORNER_L 0x1A8 // 424
#define TILE_BOT_EDGE     0x1A9 // 425
#define TILE_BOT_CORNER_R 0x1AA // 426

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  0, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 13,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 13, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 13,  1,  1,  7);

    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1, 14,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2, 14, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28, 14,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1, 15,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28, 15,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
