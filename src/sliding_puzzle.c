#include "global.h"
#include "bg.h"
#include "decompress.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/sliding_puzzles.h"

static void CB2_LoadSlidingPuzzle(void);
static void CB2_SlidingPuzzle(void);
static void VBlankCB_SlidingPuzzle(void);
static void Task_SlidingPuzzle_WaitFadeIn(u8);
static void Task_SlidingPuzzle_HandleInput(u8);
static void Task_SlidingPuzzle_Glow(u8);
static void Task_SlidingPuzzle_Solved(u8);
static void Task_SlidingPuzzle_Exit(u8);
static void DrawInstructionsBar(u8);
static void CreateCursorSprite(void);
static void CreateTileSprites(void);
static void MoveCursor(s8, s8);
static void MoveCursorSprite(s8, s8, u8);
static bool32 CursorIsOnTile(void);
static bool32 CursorIsOnImmovableTile(void);
static void SelectTile(void);
static void PlaceTile(void);
static void RotateTile(u8);
static void CheckForSolution(void);
static void ExitSlidingPuzzle(u8);
static void SpriteCB_Cursor(struct Sprite *);

#define __ 0
#define sTimer        data[0]
#define sAnimating    data[1]
#define sRow          data[2]
#define sCol          data[3]
#define sTileId       data[4]
#define sOrientation  data[5]

#include "data/sliding_puzzles.h"

struct SlidingPuzzle
{
    u8 tiles[NUM_ROWS][NUM_COLS];
    u8 puzzleId;
    u8 cursorSpriteId;
    u8 heldTile;
    bool8 solved;
};

static EWRAM_DATA struct SlidingPuzzle *sSlidingPuzzle = NULL;

void DoSlidingPuzzle(void)
{
    SetMainCallback2(CB2_LoadSlidingPuzzle);
    gMain.savedCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
}

static void CB2_LoadSlidingPuzzle(void)
{
    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    DmaFillLarge16(3, 0, (void *)VRAM, VRAM_SIZE, 0x1000);
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    LoadPalette(sSlidingPuzzle_Pal, 0, 32);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_1D_MAP);
    ShowBg(0);
    ShowBg(1);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    LZ77UnCompVram(sSlidingPuzzle_Gfx, (void *)VRAM);
    LZ77UnCompVram(sSlidingPuzzle_Tilemap, (u16 *)BG_SCREEN_ADDR(7));
    InitWindows(sWindowTemplates);
    LoadPalette(GetTextWindowPalette(2), 0xD0, 32);
    LoadCompressedSpriteSheet(&sSpriteSheet_Cursor);
    sSlidingPuzzle = AllocZeroed(sizeof(sSlidingPuzzle));
    sSlidingPuzzle->heldTile = __;
    sSlidingPuzzle->puzzleId = gSpecialVar_0x8004;
    sSlidingPuzzle->solved = gSpecialVar_0x8005;
    LoadCompressedSpriteSheet(&sSpriteSheet_Tiles[sSlidingPuzzle->puzzleId]);
    LoadSpritePalettes(sSpritePalettes_CursorTiles);
    if (sSlidingPuzzle->solved)
        DrawInstructionsBar(INSTRUCTION_CONTINUE);
    else
        DrawInstructionsBar(INSTRUCTION_NO_SELECTION);
    CreateCursorSprite();
    CreateTileSprites();
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
    EnableInterrupts(INTR_FLAG_VBLANK);
    SetVBlankCallback(VBlankCB_SlidingPuzzle);
    SetMainCallback2(CB2_SlidingPuzzle);
    CreateTask(Task_SlidingPuzzle_WaitFadeIn, 0);
}

static void CB2_SlidingPuzzle(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_SlidingPuzzle(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_SlidingPuzzle_WaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (!sSlidingPuzzle->solved)
            gTasks[taskId].func = Task_SlidingPuzzle_HandleInput;
        else
            gTasks[taskId].func = Task_SlidingPuzzle_Solved;
    }
}

static void Task_SlidingPuzzle_HandleInput(u8 taskId)
{
    // Prevent input if the puzzle has been solved
    if (sSlidingPuzzle->solved)
    {
        gTasks[taskId].func = Task_SlidingPuzzle_Glow;
        return;
    }

    // Prevent input while the tile is rotating
    if (sSlidingPuzzle->heldTile != __)
    {
        struct Sprite *sprite = &gSprites[sSlidingPuzzle->heldTile];

        if (sprite->sAnimating && !sprite->affineAnimEnded)
            return;
    }

    if (JOY_NEW(B_BUTTON))
        ExitSlidingPuzzle(taskId);
    else if (JOY_NEW(DPAD_LEFT))
        MoveCursor(-1,  0);
    else if (JOY_NEW(DPAD_RIGHT))
        MoveCursor( 1,  0);
    else if (JOY_NEW(DPAD_UP))
        MoveCursor( 0, -1);
    else if (JOY_NEW(DPAD_DOWN))
        MoveCursor( 0,  1);

    if (sSlidingPuzzle->heldTile == __)
    {
        // Not holding a tile
        if (CursorIsOnTile() && !CursorIsOnImmovableTile())
        {
            DrawInstructionsBar(INSTRUCTION_PICK_UP);
            if (JOY_NEW(A_BUTTON))
                SelectTile();
        }
        else
        {
            DrawInstructionsBar(INSTRUCTION_NO_SELECTION);
        }
    }
    else
    {
        // Currently holding a tile
        if (CursorIsOnTile() && !CursorIsOnImmovableTile())
        {
            DrawInstructionsBar(INSTRUCTION_SWAP);
            if (JOY_NEW(A_BUTTON))
                SelectTile();
        }
        else if (CursorIsOnImmovableTile())
        {
            DrawInstructionsBar(INSTRUCTION_ROTATE);
        }
        else
        {
            DrawInstructionsBar(INSTRUCTION_PLACE);
            if (JOY_NEW(A_BUTTON))
                PlaceTile();
        }

        if (JOY_NEW(L_BUTTON))
            RotateTile(ROTATE_ANTICLOCKWISE);
        else if (JOY_NEW(R_BUTTON))
            RotateTile(ROTATE_CLOCKWISE);
    }
}

#define tState  data[0]
#define tTimer  data[1]
#define tGlow   data[2]

static void Task_SlidingPuzzle_Glow(u8 taskId)
{
    u16 color;
    s16* data = gTasks[taskId].data;
    switch (tState)
    {
    case 0:
        DrawInstructionsBar(INSTRUCTION_CONTINUE);
        tState++;
        break;
    case 1:
        DestroySprite(&gSprites[sSlidingPuzzle->cursorSpriteId]);
        PlayFanfare(MUS_LEVEL_UP);
        tGlow = 22;
        tState++;
        break;
    case 2:
        tTimer++;
        if ((tTimer % 4) == 0)
        {
            tGlow++;
            if (tGlow == 31)
                tState++;
            color = RGB(tGlow, tGlow, tGlow);
            LoadPalette(&color, 0x103, sizeof(color));
        }
        break;
    case 3:
        tTimer++;
        if ((tTimer % 4) == 0)
        {
            tGlow--;
            if (tGlow > 22)
            {
                color = RGB(tGlow, tGlow, tGlow);
            }
            else
            {
                color = RGB(28, 22, 13);
                tState++;
            }
            LoadPalette(&color, 0x103, sizeof(color));
        }
        break;
    default:
        if (IsFanfareTaskInactive())
            gTasks[taskId].func = Task_SlidingPuzzle_Solved;
        break;
    }
}

#undef tState
#undef tTimer
#undef tGlow

static void Task_SlidingPuzzle_Solved(u8 taskId)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
        ExitSlidingPuzzle(taskId);
}

static void Task_SlidingPuzzle_Exit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gSpecialVar_Result = sSlidingPuzzle->solved;
        Free(sSlidingPuzzle);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void DrawInstructionsBar(u8 stringId)
{
    const u8 color[3] = { TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY };

    FillWindowPixelBuffer(0, PIXEL_FILL(15));
    AddTextPrinterParameterized3(0, FONT_SMALL, 2, 1, color, 0, sInstructions[stringId]);
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
    ScheduleBgCopyTilemapToVram(0);
}

static void CreateCursorSprite(void)
{
    u8 spriteId = SPRITE_NONE;

    if (!sSlidingPuzzle->solved)
    {
        spriteId = CreateSprite(&sSpriteTemplate_Cursor,
                                sColumnXCoords[0],
                                sRowYCoords[0],
                                1);
        gSprites[spriteId].sTimer = 0;
        gSprites[spriteId].sAnimating = TRUE;
        gSprites[spriteId].sRow = 0;
        gSprites[spriteId].sCol = 0;
    }

    sSlidingPuzzle->cursorSpriteId = spriteId;
}

static void CreateTileSprites(void)
{
    u8 row, col, puzzleId, tile;
    for (row = 0; row < NUM_ROWS; ++row)
    {
        for (col = 0; col < NUM_COLS; ++col)
        {
            if (sSlidingPuzzle->solved)
                puzzleId = PUZZLE_SOLVED;
            else
                puzzleId = sSlidingPuzzle->puzzleId;

            tile = sPuzzleLayouts[puzzleId][row][col];
            if (tile != __)
            {
                u8 spriteId = CreateSprite(&sSpriteTemplate_Tiles,
                                           sColumnXCoords[col],
                                           sRowYCoords[row],
                                           2);
                struct Sprite *sprite = &gSprites[spriteId];
                sprite->sAnimating = FALSE;
                sprite->sRow = row;
                sprite->sCol = col;
                sprite->sTileId = tile;
                sprite->sOrientation = sTileOrientations[puzzleId][row][col];
                if (sprite->sOrientation >= ORIENTATION_MAX)
                    sprite->sOrientation = ORIENTATION_0;
                StartSpriteAnim(sprite, tile - 1);
                StartSpriteAffineAnim(sprite, sprite->sOrientation);
                tile = spriteId;
            }

            sSlidingPuzzle->tiles[row][col] = tile;
        }
    }
}

static void MoveCursor(s8 deltaX, s8 deltaY)
{
    MoveCursorSprite(deltaX, deltaY, sSlidingPuzzle->cursorSpriteId);

    if (sSlidingPuzzle->heldTile != __)
    {
        MoveCursorSprite(deltaX, deltaY, sSlidingPuzzle->heldTile);
        PlaySE(SE_BALL_TRAY_ENTER);
    }
}

static void MoveCursorSprite(s8 deltaX, s8 deltaY, u8 spriteId)
{
    struct Sprite *sprite = &gSprites[spriteId];
    s8 row = sprite->sRow + deltaY;
    s8 col = sprite->sCol + deltaX;

    if (row < FIRST_ROW)
        row = FINAL_ROW;
    else if (row > FINAL_ROW)
        row = FIRST_ROW;

    if (col < FIRST_COL)
        col = FINAL_COL;
    else if (col > FINAL_COL)
        col = FIRST_COL;

    sprite->sCol = col;
    sprite->sRow = row;
    sprite->x = sColumnXCoords[sprite->sCol];
    sprite->y = sRowYCoords[sprite->sRow];

    sprite->invisible = FALSE;
    sprite->sTimer = 0;
}

static bool32 CursorIsOnTile(void)
{
    struct Sprite *cursor = &gSprites[sSlidingPuzzle->cursorSpriteId];

    if (sSlidingPuzzle->tiles[cursor->sRow][cursor->sCol] != __)
        return TRUE;

    return FALSE;
}

static bool32 CursorIsOnImmovableTile(void)
{
    struct Sprite *cursor = &gSprites[sSlidingPuzzle->cursorSpriteId];

    if (sTileOrientations[sSlidingPuzzle->puzzleId][cursor->sRow][cursor->sCol] == IMMOVABLE_TILE)
        return TRUE;

    return FALSE;
}

static void SelectTile(void)
{
    struct Sprite *cursor = &gSprites[sSlidingPuzzle->cursorSpriteId];

    u8 tile = sSlidingPuzzle->heldTile;
    if (tile != __)
        gSprites[tile].subpriority = 2;

    sSlidingPuzzle->heldTile = sSlidingPuzzle->tiles[cursor->sRow][cursor->sCol];
    gSprites[sSlidingPuzzle->heldTile].subpriority = 0;

    sSlidingPuzzle->tiles[cursor->sRow][cursor->sCol] = tile;

    cursor->sAnimating = FALSE;
    PlaySE(SE_SELECT);
    CheckForSolution();
}

static void PlaceTile(void)
{
    struct Sprite *cursor = &gSprites[sSlidingPuzzle->cursorSpriteId];

    sSlidingPuzzle->tiles[cursor->sRow][cursor->sCol] = sSlidingPuzzle->heldTile;
    gSprites[sSlidingPuzzle->heldTile].subpriority = 2;

    sSlidingPuzzle->heldTile = __;

    cursor->sAnimating = TRUE;
    PlaySE(SE_SELECT);
    CheckForSolution();
}

static void RotateTile(u8 rotDir)
{
    struct Sprite *sprite = &gSprites[sSlidingPuzzle->heldTile];
    u8 affineAnimation;

    if (rotDir == ROTATE_ANTICLOCKWISE)
    {
        affineAnimation = sprite->sOrientation + 4;
        if (sprite->sOrientation)
            sprite->sOrientation--;
        else
            sprite->sOrientation = ORIENTATION_270;
    }
    else if (rotDir == ROTATE_CLOCKWISE)
    {
        affineAnimation = sprite->sOrientation + 8;
        sprite->sOrientation++;
        sprite->sOrientation = sprite->sOrientation % ORIENTATION_MAX;
    }

    StartSpriteAffineAnim(sprite, affineAnimation);
    sprite->sAnimating = TRUE;
}

static void CheckForSolution(void)
{
    u8 row, col, tile;
    for (row = 0; row < NUM_ROWS; row++)
    {
        for (col = 0; col < NUM_COLS; col++)
        {
            struct Sprite *tile = &gSprites[sSlidingPuzzle->tiles[row][col]];
            u8 tileId = sPuzzleLayouts[PUZZLE_SOLVED][row][col];

            if (tile->sTileId != tileId || tile->sOrientation != ORIENTATION_0)
            {
                sSlidingPuzzle->solved = FALSE;
                return;
            }
        }
    }

    sSlidingPuzzle->solved = TRUE;
}

static void ExitSlidingPuzzle(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_SlidingPuzzle_Exit;
}

static void SpriteCB_Cursor(struct Sprite *sprite)
{
    if (++sprite->sTimer % 16 == 0)
    {
        sprite->sTimer = 0;
        if (sprite->sAnimating)
            sprite->invisible ^= 1;
        else
            sprite->invisible = FALSE;
    }
}

#undef __
#undef sTimer
#undef sAnimating
#undef sRow
#undef sCol
#undef sTileId
#undef sOrientation
