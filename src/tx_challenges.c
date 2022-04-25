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
    MENUITEM_CHALLENGES_EVO_LIMIT,
    MENUITEM_CHALLENGES_PARTY_LIMIT,
    MENUITEM_CHALLENGES_NUZLOCKE,
    MENUITEM_CHALLENGES_ITEM_PLAYER,
    MENUITEM_CHALLENGES_ITEM_TRAINER,
    MENUITEM_CHALLENGES_POKECENTER,
    MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE,
    MENUITEM_CHALLENGES_BASE_STAT_EQUALIZER,
    MENUITEM_SAVE,
    MENUITEM_COUNT,
};

// Window Ids
enum
{
    WIN_OPTIONS,
    WIN_DESCRIPTION
};

struct tx_challenges_OptionsMenu
{
    u8 sel[MENUITEM_COUNT];
    int menuCursor;
    int visibleCursor;
};

// this file's functions
static void tx_challenges_Task_OptionMenuFadeIn(u8 taskId);
static void tx_challenges_Task_OptionMenuProcessInput(u8 taskId);
static void tx_challenges_Task_OptionMenuSave(u8 taskId);
static void tx_challenges_Task_OptionMenuFadeOut(u8 taskId);
static void tx_challenges_HighlightOptionMenuItem(int cursor);
static void tx_challenges_DrawDescriptions(void);
static void tx_challenges_DrawDescriptionsFirstTime(void);
static void tx_challenges_DrawOptionMenuTexts(void);
static void DrawBgWindowFrames(void);
static int tx_challenges_FourOptions_ProcessInput(int selection);
static int tx_challenges_ThreeOptions_ProcessInput(int selection);
static int tx_challenges_TwoOptions_ProcessInput(int selection);
static int tx_challenges_OneTypeChallengeOptions_ProcessInput(int selection);
static int tx_challenges_SixOptions_ProcessInput(int selection);
static void FourOptions_DrawChoices(const u8 *const *const strings, int selection, int y, u8 textSpeed);

static void DrawChoices_Challenges_YesNo(int selection, int y, u8 textSpeed);
static void DrawChoices_Challenges_EvoLimit(int selection, int y, u8 textSpeed);
static void DrawChoices_Challenges_PartyLimit(int selection, int y, u8 textSpeed);
static void DrawChoices_Challenges_Nuzlocke(int selection, int y, u8 textSpeed);
static void DrawChoices_Challenges_Pokecenters(int selection, int y, u8 textSpeed);
static void DrawChoices_Challenges_OneTypeChallenge(int selection, int y, u8 textSpeed);
static void DrawChoices_Challenges_BaseStatEqualizer(int selection, int y, u8 textSpeed);

struct
{
    void (*drawChoices)(int selection, int y, u8 textSpeed);
    int (*processInput)(int selection);
} static const sItemFunctions[MENUITEM_COUNT] =
{
    [MENUITEM_CHALLENGES_EVO_LIMIT]           = {DrawChoices_Challenges_EvoLimit, tx_challenges_ThreeOptions_ProcessInput},
    [MENUITEM_CHALLENGES_PARTY_LIMIT]         = {DrawChoices_Challenges_PartyLimit, tx_challenges_SixOptions_ProcessInput},
    [MENUITEM_CHALLENGES_NUZLOCKE]            = {DrawChoices_Challenges_Nuzlocke, tx_challenges_ThreeOptions_ProcessInput},
    [MENUITEM_CHALLENGES_ITEM_PLAYER]         = {DrawChoices_Challenges_YesNo, tx_challenges_TwoOptions_ProcessInput},
    [MENUITEM_CHALLENGES_ITEM_TRAINER]        = {DrawChoices_Challenges_YesNo, tx_challenges_TwoOptions_ProcessInput},
    [MENUITEM_CHALLENGES_POKECENTER]          = {DrawChoices_Challenges_YesNo, tx_challenges_TwoOptions_ProcessInput},
    [MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE]  = {DrawChoices_Challenges_OneTypeChallenge, tx_challenges_OneTypeChallengeOptions_ProcessInput},
    [MENUITEM_CHALLENGES_BASE_STAT_EQUALIZER] = {DrawChoices_Challenges_BaseStatEqualizer, tx_challenges_FourOptions_ProcessInput},
    [MENUITEM_SAVE] = {NULL, NULL},
};

// EWRAM vars
EWRAM_DATA struct tx_challenges_OptionsMenu *sChallengesOptions = NULL;

// const rom data
static const u16 sOptionMenuText_Pal[] = INCBIN_U16("graphics/interface/option_menu_text.gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/interface/option_menu_equals_sign.4bpp");

static const u8 gText_EvoLimit[]            = _("EVO LIMIT");
static const u8 gText_PartyLimit[]          = _("PARTY LIMIT");
static const u8 gText_Nuzlocke[]            = _("NUZLOCKE");
static const u8 gText_Items_Player[]        = _("PLAYER ITEMS");
static const u8 gText_Items_Trainer[]       = _("TRAINER ITEMS");
static const u8 gText_Pokecenter[]          = _("POKéCENTER");
static const u8 gText_OneTypeChallenge[]    = _("ONE TYPE ONLY");
static const u8 gText_BaseStatEqualizer[]   = _("STAT EQUALIZER");


static const u8 gText_Save[] = _("SAVE");
static const u8 *const sChallengesOptionMenuItemNames[MENUITEM_COUNT] =
{
    [MENUITEM_CHALLENGES_EVO_LIMIT]           = gText_EvoLimit,
    [MENUITEM_CHALLENGES_PARTY_LIMIT]         = gText_PartyLimit,
    [MENUITEM_CHALLENGES_NUZLOCKE]            = gText_Nuzlocke,
    [MENUITEM_CHALLENGES_ITEM_PLAYER]         = gText_Items_Player,
    [MENUITEM_CHALLENGES_ITEM_TRAINER]        = gText_Items_Trainer,
    [MENUITEM_CHALLENGES_POKECENTER]          = gText_Pokecenter,
    [MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE]  = gText_OneTypeChallenge,
    [MENUITEM_CHALLENGES_BASE_STAT_EQUALIZER] = gText_BaseStatEqualizer,
    [MENUITEM_SAVE]                           = gText_Save,
};

static const u8 gText_Description_Challenges_Evo_Limit[]        = _("{COLOR 6}{SHADOW 7}Limit evolutions to first stage only\nor disallow all evolutions.");
static const u8 gText_Description_Challenges_Party_Limit[]      = _("{COLOR 6}{SHADOW 7}Limit the amount of POKéMON in the\nplayers party.");
static const u8 gText_Description_Nuzlocke[]                    = _("{COLOR 6}{SHADOW 7}Enable nuzlocke mode.\nHard Mode: {COLOR RED}{SHADOW LIGHT_RED}Delete save on whiteout!");
static const u8 gText_Description_Challenges_Items_Player[]     = _("{COLOR 6}{SHADOW 7}The player can use items.");
static const u8 gText_Description_Challenges_Items_Trainer[]    = _("{COLOR 6}{SHADOW 7}Enemy trainer can use items.");
static const u8 gText_Description_Challenges_Pokecenter[]       = _("{COLOR 6}{SHADOW 7}The player can visit Pokécenters to\nheal their party.");
static const u8 gText_Description_Challenges_OneTypeChallenge[] = _("{COLOR 6}{SHADOW 7}Allow only one POKéMON type the\nplayer can capture and use.");
static const u8 gText_Description_Challenges_BaseStatEqualizer[]= _("{COLOR 6}{SHADOW 7}All POKéMON have the same base stats.\nEqualizes stregths and weaknesses.");
static const u8 gText_Description_Save[]                        = _("{COLOR 6}{SHADOW 7}Save choices and continue...");

static const u8 *const sOptionMenuItemDescriptions[MENUITEM_COUNT] =
{
    [MENUITEM_CHALLENGES_EVO_LIMIT]             = gText_Description_Challenges_Evo_Limit,
    [MENUITEM_CHALLENGES_PARTY_LIMIT]           = gText_Description_Challenges_Party_Limit,
    [MENUITEM_CHALLENGES_NUZLOCKE]              = gText_Description_Nuzlocke,
    [MENUITEM_CHALLENGES_ITEM_PLAYER]           = gText_Description_Challenges_Items_Player,
    [MENUITEM_CHALLENGES_ITEM_TRAINER]          = gText_Description_Challenges_Items_Trainer,
    [MENUITEM_CHALLENGES_POKECENTER]            = gText_Description_Challenges_Pokecenter,
    [MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE]    = gText_Description_Challenges_OneTypeChallenge,
    [MENUITEM_CHALLENGES_BASE_STAT_EQUALIZER]   = gText_Description_Challenges_BaseStatEqualizer,
    [MENUITEM_SAVE]                             = gText_Description_Save,
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
bool8 IsChallengesActivated(void)
{
    if (gSaveBlock1Ptr->tx_Challenges_EvoLimit
        || gSaveBlock1Ptr->tx_Challenges_Nuzlocke
        || gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore
        || gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge != TX_CHALLENGE_TYPE_OFF
        || gSaveBlock1Ptr->tx_Challenges_PartyLimit
        || gSaveBlock1Ptr->tx_Challenges_NoItemPlayer
        || gSaveBlock1Ptr->tx_Challenges_NoItemTrainer
        || gSaveBlock1Ptr->tx_Challenges_PkmnCenter)
        return TRUE;

    return FALSE;
}

bool8 IsNuzlockeActivated(void)
{
    return gSaveBlock1Ptr->tx_Challenges_Nuzlocke;
}

bool8 IsPokecenterChallengeActivated(void)
{
    return gSaveBlock1Ptr->tx_Challenges_PkmnCenter;
}

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
        sItemFunctions[id].drawChoices(sChallengesOptions->sel[id], y, textSpeed);
}

void CB2_InitChallengesMenu(void)
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
        tx_challenges_DrawDescriptionsFirstTime();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_OPTIONS);
        tx_challenges_DrawOptionMenuTexts();
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
        taskId = CreateTask(tx_challenges_Task_OptionMenuFadeIn, 0);

        //tx_randomizer_and_challenges
        gSaveBlock1Ptr->tx_Challenges_EvoLimit             = TX_CHALLENGE_EVO_LIMIT;
        gSaveBlock1Ptr->tx_Challenges_PartyLimit           = TX_CHALLENGE_PARTY_LIMIT;
        gSaveBlock1Ptr->tx_Challenges_Nuzlocke             = TX_CHALLENGE_NUZLOCKE;
        gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore     = TX_CHALLENGE_NUZLOCKE_HARDCORE;
        gSaveBlock1Ptr->tx_Challenges_NoItemPlayer         = TX_CHALLENGE_NO_ITEM_PLAYER;
        gSaveBlock1Ptr->tx_Challenges_NoItemTrainer        = TX_CHALLENGE_NO_ITEM_TRAINER;
        gSaveBlock1Ptr->tx_Challenges_PkmnCenter           = TX_CHALLENGE_PKMN_CENTER;
        gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge     = TX_CHALLENGE_TYPE;
        gSaveBlock1Ptr->tx_Challenges_BaseStatEqualizer    = TX_CHALLENGE_BASE_STAT_EQUALIZER;

        sChallengesOptions = AllocZeroed(sizeof(*sChallengesOptions));

        sChallengesOptions->sel[MENUITEM_CHALLENGES_EVO_LIMIT]   = gSaveBlock1Ptr->tx_Challenges_EvoLimit;
        sChallengesOptions->sel[MENUITEM_CHALLENGES_PARTY_LIMIT] = gSaveBlock1Ptr->tx_Challenges_PartyLimit;

        if (gSaveBlock1Ptr->tx_Challenges_Nuzlocke && gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore)
            sChallengesOptions->sel[MENUITEM_CHALLENGES_NUZLOCKE] = 2;
        else if (gSaveBlock1Ptr->tx_Challenges_Nuzlocke)
            sChallengesOptions->sel[MENUITEM_CHALLENGES_NUZLOCKE] = 1;
        else
            sChallengesOptions->sel[MENUITEM_CHALLENGES_NUZLOCKE] = 0;
        
        sChallengesOptions->sel[MENUITEM_CHALLENGES_ITEM_PLAYER]    = gSaveBlock1Ptr->tx_Challenges_NoItemPlayer;
        sChallengesOptions->sel[MENUITEM_CHALLENGES_ITEM_TRAINER]   = gSaveBlock1Ptr->tx_Challenges_NoItemTrainer;        
        sChallengesOptions->sel[MENUITEM_CHALLENGES_POKECENTER]     = gSaveBlock1Ptr->tx_Challenges_PkmnCenter;
        sChallengesOptions->sel[MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE]  = gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge;
        sChallengesOptions->sel[MENUITEM_CHALLENGES_BASE_STAT_EQUALIZER] = gSaveBlock1Ptr->tx_Challenges_BaseStatEqualizer;


        for (i = 0; i < 6; i++)
            DrawChoices(i, i * Y_DIFF, 0xFF);

        tx_challenges_HighlightOptionMenuItem(sChallengesOptions->menuCursor);

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

static void tx_challenges_Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = tx_challenges_Task_OptionMenuProcessInput;
}

static void ScrollMenu(int direction)
{
    int menuItem, pos;
    if (direction == 0) // scroll down
        menuItem = sChallengesOptions->menuCursor + 2, pos = 5;
    else
        menuItem = sChallengesOptions->menuCursor - 3, pos = 0;

    // Hide one
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF, PIXEL_FILL(0));
    // Show one
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, Y_DIFF * pos, 26 * 8, Y_DIFF);
    // Print
    DrawChoices(menuItem, pos * Y_DIFF, 0xFF);
    AddTextPrinterParameterized(WIN_OPTIONS, 1, sChallengesOptionMenuItemNames[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    CopyWindowToVram(WIN_OPTIONS, 2);
}

static void ScrollAll(int direction) // to bottom or top
{
    int i, y, menuItem, pos;
    int scrollCount = MENUITEM_COUNT - 2;
    // Move items up/down
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF * scrollCount, PIXEL_FILL(1));

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
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sChallengesOptionMenuItemNames[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    }
    CopyWindowToVram(WIN_OPTIONS, 2);
}

static void tx_challenges_Task_OptionMenuProcessInput(u8 taskId)
{
    int i, scrollCount = 0;
    if (JOY_NEW(A_BUTTON))
    {
        if (sChallengesOptions->menuCursor == MENUITEM_SAVE)
            gTasks[taskId].func = tx_challenges_Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sChallengesOptions->visibleCursor == 3) // don't advance visible cursor until scrolled to the bottom
        {
            if (--sChallengesOptions->menuCursor == sChallengesOptions->visibleCursor - 1)
                sChallengesOptions->visibleCursor--;
            else
                ScrollMenu(1);
        }
        else
        {
            if (--sChallengesOptions->menuCursor < 0) // Scroll all the way to the bottom.
            {
                sChallengesOptions->visibleCursor = sChallengesOptions->menuCursor = 3;
                ScrollAll(0);
                sChallengesOptions->visibleCursor = 5;
                sChallengesOptions->menuCursor = MENUITEM_COUNT - 1;
            }
            else
            {
                sChallengesOptions->visibleCursor--;
            }
        }
        tx_challenges_HighlightOptionMenuItem(sChallengesOptions->visibleCursor);
        tx_challenges_DrawDescriptions();
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sChallengesOptions->visibleCursor == 3) // don't advance visible cursor until scrolled to the bottom
        {
            if (++sChallengesOptions->menuCursor == MENUITEM_COUNT - 2)
                sChallengesOptions->visibleCursor++;
            else
                ScrollMenu(0);
        }
        else
        {
            if (++sChallengesOptions->menuCursor >= MENUITEM_COUNT) // Scroll all the way to the top.
            {
                sChallengesOptions->visibleCursor = 3;
                sChallengesOptions->menuCursor = MENUITEM_COUNT - 4;
                ScrollAll(1);
                sChallengesOptions->visibleCursor = sChallengesOptions->menuCursor = 0;
            }
            else
            {
                sChallengesOptions->visibleCursor++;
            }
        }
        tx_challenges_HighlightOptionMenuItem(sChallengesOptions->visibleCursor);
        tx_challenges_DrawDescriptions();
    }
    else if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        int cursor = sChallengesOptions->menuCursor;
        u8 previousOption = sChallengesOptions->sel[cursor];
        if (sItemFunctions[cursor].processInput != NULL)
            sChallengesOptions->sel[cursor] = sItemFunctions[cursor].processInput(previousOption);

        if (previousOption != sChallengesOptions->sel[cursor])
            DrawChoices(cursor, sChallengesOptions->visibleCursor * Y_DIFF, 0);
    }
}

static void tx_challenges_Task_OptionMenuSave(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = tx_challenges_Task_OptionMenuFadeOut;
}

void tx_challenges_SaveData(void)
{
    gSaveBlock1Ptr->tx_Challenges_EvoLimit             = sChallengesOptions->sel[MENUITEM_CHALLENGES_EVO_LIMIT];
    gSaveBlock1Ptr->tx_Challenges_PartyLimit           = sChallengesOptions->sel[MENUITEM_CHALLENGES_PARTY_LIMIT];

    switch (sChallengesOptions->sel[MENUITEM_CHALLENGES_NUZLOCKE])
    {
    case 0:
        gSaveBlock1Ptr->tx_Challenges_Nuzlocke          = FALSE;
        gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore  = FALSE;
        break;
    case 1:
        gSaveBlock1Ptr->tx_Challenges_Nuzlocke          = TRUE;
        gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore  = FALSE;
        break;
    case 2:
        gSaveBlock1Ptr->tx_Challenges_Nuzlocke          = TRUE;
        gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore  = TRUE;
        break;
    }

    gSaveBlock1Ptr->tx_Challenges_NoItemPlayer  = sChallengesOptions->sel[MENUITEM_CHALLENGES_ITEM_PLAYER];
    gSaveBlock1Ptr->tx_Challenges_NoItemTrainer = sChallengesOptions->sel[MENUITEM_CHALLENGES_ITEM_TRAINER];
    gSaveBlock1Ptr->tx_Challenges_PkmnCenter           = sChallengesOptions->sel[MENUITEM_CHALLENGES_POKECENTER];

    if (sChallengesOptions->sel[MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE] >= NUMBER_OF_MON_TYPES-1)
        gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge = TX_CHALLENGE_TYPE_OFF;
    else if (sChallengesOptions->sel[MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE] >= TYPE_MYSTERY)
        gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge = sChallengesOptions->sel[MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE] + 1;
    else
        gSaveBlock1Ptr->tx_Challenges_OneTypeChallenge = sChallengesOptions->sel[MENUITEM_CHALLENGES_ONE_TYPE_CHALLENGE];
    
    gSaveBlock1Ptr->tx_Challenges_BaseStatEqualizer    = sChallengesOptions->sel[MENUITEM_CHALLENGES_BASE_STAT_EQUALIZER];
}

static void tx_challenges_Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        // FREE_AND_SET_NULL(sChallengesOptions);
        SetMainCallback2(gMain.savedCallback);
    }
}

static void tx_challenges_HighlightOptionMenuItem(int cursor)
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

static int tx_challenges_ThreeOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(3, selection);
}

static int tx_challenges_FourOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(4, selection);
}

static int tx_challenges_OneTypeChallengeOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(NUMBER_OF_MON_TYPES, selection);
}

static int tx_challenges_SixOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(6, selection);
}

static int tx_challenges_TwoOptions_ProcessInput(int selection)
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

static void tx_challenges_DrawDescriptions(void)
{
    u8 n = sChallengesOptions->menuCursor;
    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_DESCRIPTION, 1, sOptionMenuItemDescriptions[n], 8, 1, 0, NULL);
    CopyWindowToVram(WIN_DESCRIPTION, 3);
}
static void tx_challenges_DrawDescriptionsFirstTime(void)
{
    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_DESCRIPTION, 1, sOptionMenuItemDescriptions[0], 8, 1, 0, NULL);
    CopyWindowToVram(WIN_DESCRIPTION, 3);
}

static void tx_challenges_DrawOptionMenuTexts(void)
{
    u32 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < 7; i++)
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sChallengesOptionMenuItemNames[i], 8, (i * Y_DIFF) + 1, 0, NULL);

    CopyWindowToVram(WIN_OPTIONS, 3);
}

// Draw Choices functions
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style, u8 textSpeed)
{
    u8 dst[16];
    u32 i;

    for (i = 0; *text != EOS && i <= 14; i++)
        dst[i] = *(text++);

    if (style != 0) //choosen option text color
    {
        dst[2] = 4; //color
        dst[5] = 5; //shadow
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

static const u8 gText_Off[]  = _("{COLOR 6}{SHADOW 7}OFF");
static const u8 gText_On[]   = _("{COLOR 6}{SHADOW 7}ON");
static const u8 gText_None[] = _("{COLOR 6}{SHADOW 7}NONE");

static const u8 gText_Yes[] = _("{COLOR 6}{SHADOW 7}YES");
static const u8 gText_No[]  = _("{COLOR 6}{SHADOW 7}NO");
static void DrawChoices_Challenges_YesNo(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Yes, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_No, GetStringRightAlignXOffset(1, gText_No, 198), y, styles[1], textSpeed);
}

static const u8 gText_Challenges_EvoLimit_First[]   = _("{COLOR 6}{SHADOW 7}FIRST");
static const u8 gText_Challenges_EvoLimit_All[]     = _("{COLOR 6}{SHADOW 7}ALL");
static void DrawChoices_Challenges_EvoLimit(int selection, int y, u8 textSpeed)
{
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_Off, gText_Challenges_EvoLimit_First, gText_None);

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Challenges_EvoLimit_First, xMid, y, styles[1], textSpeed);
    DrawOptionMenuChoice(gText_Challenges_EvoLimit_All, GetStringRightAlignXOffset(1, gText_Challenges_EvoLimit_All, 198), y, styles[2], textSpeed);
}

static void DrawChoices_Challenges_PartyLimit(int selection, int y, u8 textSpeed)
{
    u8 n = 6 - selection;
    if (selection == 0)
        DrawOptionMenuChoice(gText_Off, 104, y, 1, textSpeed);
    else
    {
        u8 textPlus[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}{0x77}{0x77}{0x77}{0x77}{0x77}"); // 0x77 is to clear INSTANT text
        textPlus[7] = CHAR_0 + n;
        DrawOptionMenuChoice(textPlus, 104, y, 1, textSpeed);
    }
}

static const u8 gText_Challenges_Nuzlocke_Normal[] = _("{COLOR 6}{SHADOW 7}NORMAL");
static const u8 gText_Challenges_Nuzlocke_Hardcore[] = _("{COLOR 6}{SHADOW 7}HARD");
static void DrawChoices_Challenges_Nuzlocke(int selection, int y, u8 textSpeed)
{
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_Off, gText_Challenges_Nuzlocke_Normal, gText_Challenges_Nuzlocke_Hardcore);

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_Off, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Challenges_Nuzlocke_Normal, xMid, y, styles[1], textSpeed);
    DrawOptionMenuChoice(gText_Challenges_Nuzlocke_Hardcore, GetStringRightAlignXOffset(1, gText_Challenges_Nuzlocke_Hardcore, 198), y, styles[2], textSpeed);
}

static void DrawChoices_Challenges_Pokecenters(int selection, int y, u8 textSpeed)
{
    u8 styles[2] = {0};

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_On, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_Off, GetStringRightAlignXOffset(1, gText_Off, 198), y, styles[1], textSpeed);
}

static void DrawChoices_Challenges_OneTypeChallenge(int selection, int y, u8 textSpeed)
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

static const u8 gText_Challenges_BaseStatEqualizer_100[]   = _("{COLOR 6}{SHADOW 7}100");
static const u8 gText_Challenges_BaseStatEqualizer_255[]   = _("{COLOR 6}{SHADOW 7}255");
static const u8 gText_Challenges_BaseStatEqualizer_500[]   = _("{COLOR 6}{SHADOW 7}500");
static const u8 *const gText_Challenges_BaseStatEqualizer_Strings[] = {gText_Off, gText_Challenges_BaseStatEqualizer_100, gText_Challenges_BaseStatEqualizer_255, gText_Challenges_BaseStatEqualizer_500};
static void DrawChoices_Challenges_BaseStatEqualizer(int selection, int y, u8 textSpeed)
{
    FourOptions_DrawChoices(gText_Challenges_BaseStatEqualizer_Strings, selection, y, textSpeed);
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
