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
#include "tx_difficulty_challenges.h"
#include "pokemon.h"

#define Y_DIFF 16 // Difference in pixels between items.

// Menu items page 1
enum
{
    MENUITEM_RAND_CHAOS,
    MENUITEM_RAND_ENCOUNTER,
    MENUITEM_RAND_TYPE,
    MENUITEM_RAND_TYPE_EFFEC,
    MENUITEM_RAND_ABILITIES,
    MENUITEM_RAND_MOVES,
    MENUITEM_RAND_TRAINER,
    MENUITEM_RAND_EVOLUTIONS,
    MENUITEM_RAND_EVOLUTIONS_METHODE,
    MENUITEM_DIFF_EVO_LIMIT,
    MENUITEM_DIFF_PARTY_LIMIT,
    MENUITEM_DIFF_NUZLOCKE,
    MENUITEM_DIFF_ITEM,
    MENUITEM_DIFF_POKECENTER,
    MENUITEM_DIFF_TYPE_CHALLENGE,
    MENUITEM_CANCEL,
    MENUITEM_COUNT,
};

// Window Ids
enum
{
    WIN_OPTIONS,
    WIN_DESCRIPTION
};

struct tx_DC_OptionMenu
{
    u8 sel[MENUITEM_COUNT];
    int menuCursor;
    int visibleCursor;
};

// this file's functions
static void tx_DC_Task_OptionMenuFadeIn(u8 taskId);
static void tx_DC_Task_OptionMenuProcessInput(u8 taskId);
static void tx_DC_Task_OptionMenuSave(u8 taskId);
static void tx_DC_Task_OptionMenuFadeOut(u8 taskId);
static void tx_DC_HighlightOptionMenuItem(int cursor);
static void tx_DC_DrawDescriptions(void);
static void tx_DC_DrawDescriptionsFirstTime(void);
static void tx_DC_DrawOptionMenuTexts(void);
static void DrawBgWindowFrames(void);
static int tx_DC_FourOptions_ProcessInput(int selection);
static int tx_DC_ThreeOptions_ProcessInput(int selection);
static int tx_DC_TwoOptions_ProcessInput(int selection);
static int tx_DC_ElevenOptions_ProcessInput(int selection);
static int tx_DC_SixOptions_ProcessInput(int selection);
static void FourOptions_DrawChoices(const u8 *const *const strings, int selection, int y, u8 textSpeed);

static void DrawChoices_Rand_OnOff(int selection, int y, u8 textSpeed);
static void DrawChoices_Rand_OffChaos(int selection, int y, u8 textSpeed);
static void DrawChoices_Rand_NormalRandom(int selection, int y, u8 textSpeed);
static void DrawChoices_Diff_EvoLimit(int selection, int y, u8 textSpeed);
static void DrawChoices_Diff_PartyLimit(int selection, int y, u8 textSpeed);
static void DrawChoices_Diff_Nuzlocke(int selection, int y, u8 textSpeed);
static void DrawChoices_Diff_Items(int selection, int y, u8 textSpeed);
static void DrawChoices_Diff_Pokecenters(int selection, int y, u8 textSpeed);
static void DrawChoices_Diff_TypeChallenge(int selection, int y, u8 textSpeed);





struct
{
    void (*drawChoices)(int selection, int y, u8 textSpeed);
    int (*processInput)(int selection);
} static const sItemFunctions[MENUITEM_COUNT] =
{
    [MENUITEM_RAND_CHAOS]               = {DrawChoices_Rand_OffChaos, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_RAND_ENCOUNTER]           = {DrawChoices_Rand_NormalRandom, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_RAND_TYPE]                = {DrawChoices_Rand_NormalRandom, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_RAND_TYPE_EFFEC]          = {DrawChoices_Rand_NormalRandom, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_RAND_ABILITIES]           = {DrawChoices_Rand_NormalRandom, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_RAND_MOVES]               = {DrawChoices_Rand_NormalRandom, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_RAND_TRAINER]             = {DrawChoices_Rand_NormalRandom, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_RAND_EVOLUTIONS]          = {DrawChoices_Rand_NormalRandom, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_RAND_EVOLUTIONS_METHODE]  = {DrawChoices_Rand_NormalRandom, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_DIFF_EVO_LIMIT]           = {DrawChoices_Diff_EvoLimit, tx_DC_ThreeOptions_ProcessInput},
    [MENUITEM_DIFF_PARTY_LIMIT]         = {DrawChoices_Diff_PartyLimit, tx_DC_SixOptions_ProcessInput},
    [MENUITEM_DIFF_NUZLOCKE]            = {DrawChoices_Diff_Nuzlocke, tx_DC_ThreeOptions_ProcessInput},
    [MENUITEM_DIFF_ITEM]                = {DrawChoices_Diff_Items, tx_DC_FourOptions_ProcessInput},
    [MENUITEM_DIFF_POKECENTER]          = {DrawChoices_Diff_Pokecenters, tx_DC_TwoOptions_ProcessInput},
    [MENUITEM_DIFF_TYPE_CHALLENGE]      = {DrawChoices_Diff_TypeChallenge, tx_DC_ElevenOptions_ProcessInput},
    [MENUITEM_CANCEL] = {NULL, NULL},
};

// EWRAM vars
EWRAM_DATA struct tx_DC_OptionMenu *sOptions = NULL;

// const rom data
static const u16 sOptionMenuText_Pal[] = INCBIN_U16("graphics/misc/option_menu_text.gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/misc/option_menu_equals_sign.4bpp");

static const u8 gText_Chaos[] =             _("CHAOS MODE");
static const u8 gText_Encounter[] =         _("ENCOUNTER");
static const u8 gText_Type[] =              _("TYPE");
static const u8 gText_TypeEff[] =           _("EFFECTIVNESS");
static const u8 gText_Abilities[] =         _("ABILTIES");
static const u8 gText_Moves[] =             _("MOVES");
static const u8 gText_Trainer[] =           _("TRAINER");
static const u8 gText_Evolutions[] =        _("EVOLUTIONS");
static const u8 gText_EvolutionMethodes[] = _("EVO METHODES");
static const u8 gText_EvoLimit[] =          _("EVO LIMIT");
static const u8 gText_PartyLimit[] =        _("PARTY LIMIT");
static const u8 gText_Nuzlocke[] =          _("NUZLOCKE");
static const u8 gText_Items[] =             _("ITEM LIMIT");
static const u8 gText_Pokecenter[] =        _("POKECENTER");
static const u8 gText_TypeChall[] =         _("TYPE LIMIT");


static const u8 gText_Save[] = _("SAVE");
static const u8 *const sOptionMenuItemNames[MENUITEM_COUNT] =
{
    [MENUITEM_RAND_CHAOS]               = gText_Chaos,          
    [MENUITEM_RAND_ENCOUNTER]           = gText_Encounter,      
    [MENUITEM_RAND_TYPE]                = gText_Type,           
    [MENUITEM_RAND_TYPE_EFFEC]          = gText_TypeEff,        
    [MENUITEM_RAND_ABILITIES]           = gText_Abilities,      
    [MENUITEM_RAND_MOVES]               = gText_Moves,          
    [MENUITEM_RAND_TRAINER]             = gText_Trainer,        
    [MENUITEM_RAND_EVOLUTIONS]          = gText_Evolutions,     
    [MENUITEM_RAND_EVOLUTIONS_METHODE]  = gText_EvolutionMethodes,
    [MENUITEM_DIFF_EVO_LIMIT]           = gText_EvoLimit,       
    [MENUITEM_DIFF_PARTY_LIMIT]         = gText_PartyLimit,     
    [MENUITEM_DIFF_NUZLOCKE]            = gText_Nuzlocke,       
    [MENUITEM_DIFF_ITEM]                = gText_Items,          
    [MENUITEM_DIFF_POKECENTER]          = gText_Pokecenter,     
    [MENUITEM_DIFF_TYPE_CHALLENGE]      = gText_TypeChall,
    [MENUITEM_CANCEL]                   = gText_Save,
};

static const u8 gText_Description_00[] = _("Enable {COLOR RED}{SHADOW LIGHT_RED}Chaos mode");
static const u8 gText_Description_01[] = _("Randomize wild encounters");
static const u8 gText_Description_02[] = _("Randomize mon types");
static const u8 gText_Description_03[] = _("Randomize type effectivness");
static const u8 gText_Description_04[] = _("Randomize abilities");
static const u8 gText_Description_05[] = _("Randomize moves");
static const u8 gText_Description_06[] = _("Randomize enemy trainer parties");
static const u8 gText_Description_07[] = _("Randomize evolutions");
static const u8 gText_Description_08[] = _("Randomize evolution methodes");
static const u8 gText_Description_09[] = _("Limit evolutions");
static const u8 gText_Description_10[] = _("Limit your parties size");
static const u8 gText_Description_11[] = _("Enable nuzlocke mode");
static const u8 gText_Description_12[] = _("Impose item limits");
static const u8 gText_Description_13[] = _("Pokecenter use");
static const u8 gText_Description_14[] = _("Enable only one allowed mon type");
static const u8 gText_Description_15[] = _("Save your changes and proceed");
static const u8 *const sOptionMenuItemDescriptions[MENUITEM_COUNT] =
{
    [MENUITEM_RAND_CHAOS]               = gText_Description_00,
    [MENUITEM_RAND_ENCOUNTER]           = gText_Description_01,
    [MENUITEM_RAND_TYPE]                = gText_Description_02,
    [MENUITEM_RAND_TYPE_EFFEC]          = gText_Description_03,
    [MENUITEM_RAND_ABILITIES]           = gText_Description_04,
    [MENUITEM_RAND_MOVES]               = gText_Description_05,
    [MENUITEM_RAND_TRAINER]             = gText_Description_06,
    [MENUITEM_RAND_EVOLUTIONS]          = gText_Description_07,
    [MENUITEM_RAND_EVOLUTIONS_METHODE]  = gText_Description_08,
    [MENUITEM_DIFF_EVO_LIMIT]           = gText_Description_09,
    [MENUITEM_DIFF_PARTY_LIMIT]         = gText_Description_10,
    [MENUITEM_DIFF_NUZLOCKE]            = gText_Description_11,
    [MENUITEM_DIFF_ITEM]                = gText_Description_12,
    [MENUITEM_DIFF_POKECENTER]          = gText_Description_13,
    [MENUITEM_DIFF_TYPE_CHALLENGE]      = gText_Description_14,
    [MENUITEM_CANCEL]                   = gText_Description_15,
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
        sItemFunctions[id].drawChoices(sOptions->sel[id], y, textSpeed);
}

void CB2_InitDifficultyChallengesOptionMenu(void)
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
        tx_DC_DrawDescriptionsFirstTime();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_OPTIONS);
        tx_DC_DrawOptionMenuTexts();
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
        taskId = CreateTask(tx_DC_Task_OptionMenuFadeIn, 0);

        //tx_difficulty_challenges
        gSaveBlock1Ptr->txRandChaos                =   TX_RANDOM_CHAOS_MODE;
        gSaveBlock1Ptr->txRandEncounter            =   TX_RANDOM_ENCOUNTER;
        gSaveBlock1Ptr->txRandEncounterSimilar     =   TX_RANDOM_ENCOUNTER_SIMILAR;
        gSaveBlock1Ptr->txRandType                 =   TX_RANDOM_TYPE;
        gSaveBlock1Ptr->txRandTypeEffectiveness    =   TX_RANDOM_TYPE_EFFECTIVENESS;
        gSaveBlock1Ptr->txRandAbilities            =   TX_RANDOM_ABILITIES;
        gSaveBlock1Ptr->txRandMoves                =   TX_RANDOM_MOVES;
        gSaveBlock1Ptr->txRandTrainer              =   TX_RANDOM_TRAINER;
        gSaveBlock1Ptr->txRandEvolutions           =   TX_RANDOM_EVOLUTION;
        gSaveBlock1Ptr->txRandEvolutionMethodes    =   TX_RANDOM_EVOLUTION_METHODE;
        gSaveBlock1Ptr->txRandEvoLimit             =   TX_CHALLANGE_EVO_LIMIT;
        gSaveBlock1Ptr->txRandNuzlocke             =   TX_CHALLENGE_NUZLOCKE;
        gSaveBlock1Ptr->txRandNuzlockeHardcore     =   TX_CHALLENGE_NUZLOCKE_HARDCORE;
        gSaveBlock1Ptr->txRandNoItemPlayer         =   TX_CHALLENGE_NO_ITEM_PLAYER;
        gSaveBlock1Ptr->txRandNoItemTrainer        =   TX_CHALLENGE_NO_ITEM_TRAINER;
        gSaveBlock1Ptr->txRandTypeChallenge        =   TX_CHALLENGE_TYPE;
        gSaveBlock1Ptr->txRandPartyLimit           =   TX_CHALLANGE_PARTY_LIMIT;
        gSaveBlock1Ptr->txRandPkmnCenter           =   TX_CHALLENGE_PKMN_CENTER;

        sOptions = AllocZeroed(sizeof(*sOptions));

        sOptions->sel[MENUITEM_RAND_CHAOS]               = gSaveBlock1Ptr->txRandChaos;
        sOptions->sel[MENUITEM_RAND_ENCOUNTER]           = gSaveBlock1Ptr->txRandEncounter;
        sOptions->sel[MENUITEM_RAND_TYPE]                = gSaveBlock1Ptr->txRandType;
        sOptions->sel[MENUITEM_RAND_TYPE_EFFEC]          = gSaveBlock1Ptr->txRandTypeEffectiveness;
        sOptions->sel[MENUITEM_RAND_ABILITIES]           = gSaveBlock1Ptr->txRandAbilities;
        sOptions->sel[MENUITEM_RAND_MOVES]               = gSaveBlock1Ptr->txRandMoves;
        sOptions->sel[MENUITEM_RAND_TRAINER]             = gSaveBlock1Ptr->txRandTrainer;
        sOptions->sel[MENUITEM_RAND_EVOLUTIONS]          = gSaveBlock1Ptr->txRandEvolutions;
        sOptions->sel[MENUITEM_RAND_EVOLUTIONS_METHODE]  = gSaveBlock1Ptr->txRandEvolutionMethodes;
        sOptions->sel[MENUITEM_DIFF_EVO_LIMIT]           = gSaveBlock1Ptr->txRandEvoLimit;
        sOptions->sel[MENUITEM_DIFF_PARTY_LIMIT]         = 6 - gSaveBlock1Ptr->txRandPartyLimit;

        if (gSaveBlock1Ptr->txRandNuzlocke && gSaveBlock1Ptr->txRandNuzlockeHardcore)
            sOptions->sel[MENUITEM_DIFF_NUZLOCKE] = 2;
        else if (gSaveBlock1Ptr->txRandNuzlocke)
            sOptions->sel[MENUITEM_DIFF_NUZLOCKE] = 1;
        else
            sOptions->sel[MENUITEM_DIFF_NUZLOCKE] = 0;
        
        if (gSaveBlock1Ptr->txRandNoItemPlayer && gSaveBlock1Ptr->txRandNoItemTrainer)
            sOptions->sel[MENUITEM_DIFF_ITEM] = 3;
        else if (gSaveBlock1Ptr->txRandNoItemPlayer)
            sOptions->sel[MENUITEM_DIFF_ITEM] = 1;
        else if (gSaveBlock1Ptr->txRandNoItemTrainer)
            sOptions->sel[MENUITEM_DIFF_ITEM] = 2;
        else
            sOptions->sel[MENUITEM_DIFF_ITEM] = 0;
        
        sOptions->sel[MENUITEM_DIFF_POKECENTER]          = gSaveBlock1Ptr->txRandPkmnCenter;
        sOptions->sel[MENUITEM_DIFF_TYPE_CHALLENGE]      = gSaveBlock1Ptr->txRandTypeChallenge;


        for (i = 0; i < 6; i++)
            DrawChoices(i, i * Y_DIFF, 0xFF);

        tx_DC_HighlightOptionMenuItem(sOptions->menuCursor);

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

static void tx_DC_Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = tx_DC_Task_OptionMenuProcessInput;
}

static void ScrollMenu(int direction)
{
    int menuItem, pos;
    if (direction == 0) // scroll down
        menuItem = sOptions->menuCursor + 2, pos = 5;
    else
        menuItem = sOptions->menuCursor - 3, pos = 0;

    // Hide one
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF, PIXEL_FILL(0));
    // Show one
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, Y_DIFF * pos, 26 * 8, Y_DIFF);
    // Print
    DrawChoices(menuItem, pos * Y_DIFF, 0xFF);
    AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNames[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    CopyWindowToVram(WIN_OPTIONS, 2);
}

static void ScrollAll(int direction) // to bottom or top
{
    int i, y, menuItem, pos;
    int scrollCount = MENUITEM_COUNT - 6;
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
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNames[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    }
    CopyWindowToVram(WIN_OPTIONS, 2);
}

static void tx_DC_Task_OptionMenuProcessInput(u8 taskId)
{
    int i, scrollCount = 0;
    if (JOY_NEW(A_BUTTON))
    {
        if (sOptions->menuCursor == MENUITEM_CANCEL)
            gTasks[taskId].func = tx_DC_Task_OptionMenuSave;
    }
    // else if (JOY_NEW(B_BUTTON))
    // {
    //     gTasks[taskId].func = tx_DC_Task_OptionMenuSave;
    // }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sOptions->visibleCursor == 3) // don't advance visible cursor until scrolled to the bottom
        {
            if (--sOptions->menuCursor == sOptions->visibleCursor - 1)
                sOptions->visibleCursor--;
            else
                ScrollMenu(1);
        }
        else
        {
            if (--sOptions->menuCursor < 0) // Scroll all the way to the bottom.
            {
                sOptions->visibleCursor = sOptions->menuCursor = 3;
                ScrollAll(0);
                sOptions->visibleCursor = 5;
                sOptions->menuCursor = MENUITEM_COUNT - 1;
            }
            else
            {
                sOptions->visibleCursor--;
            }
        }
        tx_DC_HighlightOptionMenuItem(sOptions->visibleCursor);
        tx_DC_DrawDescriptions();
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sOptions->visibleCursor == 3) // don't advance visible cursor until scrolled to the bottom
        {
            if (++sOptions->menuCursor == MENUITEM_COUNT - 2)
                sOptions->visibleCursor++;
            else
                ScrollMenu(0);
        }
        else
        {
            if (++sOptions->menuCursor >= MENUITEM_COUNT) // Scroll all the way to the top.
            {
                sOptions->visibleCursor = 3;
                sOptions->menuCursor = MENUITEM_COUNT - 4;
                ScrollAll(1);
                sOptions->visibleCursor = sOptions->menuCursor = 0;
            }
            else
            {
                sOptions->visibleCursor++;
            }
        }
        tx_DC_HighlightOptionMenuItem(sOptions->visibleCursor);
        tx_DC_DrawDescriptions();
    }
    else if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        int cursor = sOptions->menuCursor;
        u8 previousOption = sOptions->sel[cursor];
        if (sItemFunctions[cursor].processInput != NULL)
            sOptions->sel[cursor] = sItemFunctions[cursor].processInput(previousOption);

        if (previousOption != sOptions->sel[cursor])
            DrawChoices(cursor, sOptions->visibleCursor * Y_DIFF, 0);
    }
}

static void tx_DC_Task_OptionMenuSave(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = tx_DC_Task_OptionMenuFadeOut;
}

void tx_DC_SaveData(void)
{
    gSaveBlock1Ptr->txRandChaos                = sOptions->sel[MENUITEM_RAND_CHAOS];
    gSaveBlock1Ptr->txRandEncounter            = sOptions->sel[MENUITEM_RAND_ENCOUNTER];
    gSaveBlock1Ptr->txRandType                 = sOptions->sel[MENUITEM_RAND_TYPE];
    gSaveBlock1Ptr->txRandTypeEffectiveness    = sOptions->sel[MENUITEM_RAND_TYPE_EFFEC];
    gSaveBlock1Ptr->txRandAbilities            = sOptions->sel[MENUITEM_RAND_ABILITIES];
    gSaveBlock1Ptr->txRandMoves                = sOptions->sel[MENUITEM_RAND_MOVES];
    gSaveBlock1Ptr->txRandTrainer              = sOptions->sel[MENUITEM_RAND_TRAINER];
    gSaveBlock1Ptr->txRandEvolutions           = sOptions->sel[MENUITEM_RAND_EVOLUTIONS];
    gSaveBlock1Ptr->txRandEvolutionMethodes    = sOptions->sel[MENUITEM_RAND_EVOLUTIONS_METHODE];
    gSaveBlock1Ptr->txRandEvoLimit             = sOptions->sel[MENUITEM_DIFF_EVO_LIMIT];

    switch (sOptions->sel[MENUITEM_DIFF_NUZLOCKE])
    {
    case 0:
        gSaveBlock1Ptr->txRandNuzlocke          = FALSE;
        gSaveBlock1Ptr->txRandNuzlockeHardcore  = FALSE;
        break;
    case 1:
        gSaveBlock1Ptr->txRandNuzlocke          = TRUE;
        gSaveBlock1Ptr->txRandNuzlockeHardcore  = FALSE;
        break;
    case 2:
        gSaveBlock1Ptr->txRandNuzlocke          = TRUE;
        gSaveBlock1Ptr->txRandNuzlockeHardcore  = TRUE;
        break;
    }

    switch (sOptions->sel[MENUITEM_DIFF_ITEM])
    {
    case 0:
        gSaveBlock1Ptr->txRandNoItemPlayer  = FALSE;
        gSaveBlock1Ptr->txRandNoItemTrainer = FALSE;
        break;
    case 1:
        gSaveBlock1Ptr->txRandNoItemPlayer  = TRUE;
        gSaveBlock1Ptr->txRandNoItemTrainer = FALSE;
        break;
    case 2:
        gSaveBlock1Ptr->txRandNoItemPlayer  = FALSE;
        gSaveBlock1Ptr->txRandNoItemTrainer = TRUE;
        break;
    case 3:
        gSaveBlock1Ptr->txRandNoItemPlayer  = TRUE;
        gSaveBlock1Ptr->txRandNoItemTrainer = TRUE;
        break;
    }

    if (sOptions->sel[MENUITEM_DIFF_TYPE_CHALLENGE] >= NUMBER_OF_MON_TYPES-1)
        gSaveBlock1Ptr->txRandTypeChallenge = TX_CHALLENGE_TYPE_OFF;
    else if (sOptions->sel[MENUITEM_DIFF_TYPE_CHALLENGE] >= TYPE_MYSTERY)
        gSaveBlock1Ptr->txRandTypeChallenge = sOptions->sel[MENUITEM_DIFF_TYPE_CHALLENGE] + 1;
    else
        gSaveBlock1Ptr->txRandTypeChallenge = sOptions->sel[MENUITEM_DIFF_TYPE_CHALLENGE];
    
    gSaveBlock1Ptr->txRandPartyLimit           = 6 - sOptions->sel[MENUITEM_DIFF_PARTY_LIMIT];
    gSaveBlock1Ptr->txRandPkmnCenter           = sOptions->sel[MENUITEM_DIFF_POKECENTER];

    FREE_AND_SET_NULL(sOptions);
    RandomizeSpeciesListEWRAM(1);
    RandomizeTypeEffectivenessListEWRAM();
}

static void tx_DC_Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        // FREE_AND_SET_NULL(sOptions);
        SetMainCallback2(gMain.savedCallback);
    }
}

static void tx_DC_HighlightOptionMenuItem(int cursor)
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

static int tx_DC_ThreeOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(3, selection);
}

static int tx_DC_FourOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(4, selection);
}

static int tx_DC_ElevenOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(NUMBER_OF_MON_TYPES, selection);
}

static int tx_DC_SixOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(6, selection);
}

static int tx_DC_TwoOptions_ProcessInput(int selection)
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

static void tx_DC_DrawDescriptions(void)
{
    u8 n = sOptions->menuCursor;
    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_DESCRIPTION, 1, sOptionMenuItemDescriptions[n], 8, 1, 0, NULL);
    CopyWindowToVram(WIN_DESCRIPTION, 3);
}
static void tx_DC_DrawDescriptionsFirstTime(void)
{
    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_DESCRIPTION, 1, sOptionMenuItemDescriptions[0], 8, 1, 0, NULL);
    CopyWindowToVram(WIN_DESCRIPTION, 3);
}

static void tx_DC_DrawOptionMenuTexts(void)
{
    u32 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < 7; i++)
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemNames[i], 8, (i * Y_DIFF) + 1, 0, NULL);

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

static void FourOptions_DrawChoices(const u8 *const *const strings, int selection, int y, u8 textSpeed)
{
    static const u8 choiceOrders[][3] =
    {
        {0, 1, 2},
        {0, 1, 2},
        {1, 2, 3},
        {1, 2, 3},
    };
    u8 styles[4] = {0};
    int xMid;
    const u8 *order = choiceOrders[selection];

    styles[selection] = 1;
    xMid = GetMiddleX(strings[order[0]], strings[order[1]], strings[order[2]]);

    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 104, y, 26 * 8 - 104, Y_DIFF);
    CopyWindowToVram(WIN_OPTIONS, 2);

    DrawOptionMenuChoice(strings[order[0]], 104, y, styles[order[0]], textSpeed);
    DrawOptionMenuChoice(strings[order[1]], xMid, y, styles[order[1]], textSpeed);
    DrawOptionMenuChoice(strings[order[2]], GetStringRightAlignXOffset(1, strings[order[2]], 198), y, styles[order[2]], textSpeed);
}

// static const u8 gText_ = _("");
// static const u8 gText_ = _("{COLOR }{SHADOW }");

static const u8 gText_Off[] = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}OFF");
static const u8 gText_On[]  = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}ON");
static const u8 gText_None[]  = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}NONE");
static void DrawChoices_Rand_OnOff(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_On, GetStringRightAlignXOffset(1, gText_On, 198), y, styles[1], textSpeed);
}

static const u8 gText_Rand_Chaos[] = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}CHAOS");
static void DrawChoices_Rand_OffChaos(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Rand_Chaos, GetStringRightAlignXOffset(1, gText_Rand_Chaos, 198), y, styles[1], textSpeed);
}

static const u8 gText_Normal[]  = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}NORMAL");
static const u8 gText_Random[]  = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}RANDOM");
static void DrawChoices_Rand_NormalRandom(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Normal, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Random, GetStringRightAlignXOffset(1, gText_Random, 198), y, styles[1], textSpeed);
}



static const u8 gText_Diff_EvoLimit_First[] = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}FIRST");
static void DrawChoices_Diff_EvoLimit(int selection, int y, u8 textSpeed)
{
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_Off, gText_Diff_EvoLimit_First, gText_None);

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Diff_EvoLimit_First, xMid, y, styles[1], textSpeed);
    DrawOptionMenuChoice(gText_None, GetStringRightAlignXOffset(1, gText_None, 198), y, styles[2], textSpeed);
}

static void DrawChoices_Diff_PartyLimit(int selection, int y, u8 textSpeed)
{
    u8 n = 6 - selection;
    if (selection == 0)
        DrawOptionMenuChoice(gText_Off, 104, y, 1, textSpeed);
    else
    {
        u8 textPlus[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}+1{0x77}{0x77}{0x77}{0x77}{0x77}"); // 0x77 is to clear INSTANT text
        textPlus[7] = CHAR_0 + n;
        DrawOptionMenuChoice(textPlus, 104, y, 1, textSpeed);
    }
}

static const u8 gText_Diff_Nuzlocke_Hardcore[] = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}HARD");
static void DrawChoices_Diff_Nuzlocke(int selection, int y, u8 textSpeed)
{
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_Off, gText_On, gText_Diff_Nuzlocke_Hardcore);

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_On, xMid, y, styles[1], textSpeed);
    DrawOptionMenuChoice(gText_Diff_Nuzlocke_Hardcore, GetStringRightAlignXOffset(1, gText_Diff_Nuzlocke_Hardcore, 198), y, styles[2], textSpeed);
}

// static const u8 gText_Diff_Items_Player[]   = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}PLAYER");
// static const u8 gText_Diff_Items_Trainer[]  = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}TRAINER");
// static const u8 gText_Diff_Items_Both[]     = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}BOTH");
static const u8 gText_Diff_Items_Player[]   = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}PL");
static const u8 gText_Diff_Items_Trainer[]  = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}TR");
static const u8 gText_Diff_Items_Both[]     = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}BOTH");
static const u8 *const sTextItemsStrings[]  = {gText_Off, gText_Diff_Items_Player, gText_Diff_Items_Trainer, gText_Diff_Items_Both};
static void DrawChoices_Diff_Items(int selection, int y, u8 textSpeed)
{
    FourOptions_DrawChoices(sTextItemsStrings, selection, y, textSpeed);
}

static void DrawChoices_Diff_Pokecenters(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_On, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Off, GetStringRightAlignXOffset(1, gText_Off, 198), y, styles[1], textSpeed);
}

static void DrawChoices_Diff_TypeChallenge(int selection, int y, u8 textSpeed)
{
    u8 n = selection;

    if (n >= NUMBER_OF_MON_TYPES-1)
        StringCopyPadded(gStringVar1, gText_Off, 0, 15);
    else if (n >= TYPE_MYSTERY)
        StringCopyPadded(gStringVar1, gTypeNames[n+1], 0, 10);
    else
        StringCopyPadded(gStringVar1, gTypeNames[n], 0, 10);

    DrawOptionMenuChoice(gStringVar1, 104, y, 0, textSpeed);
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
