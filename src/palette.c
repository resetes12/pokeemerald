#include "global.h"
#include "palette.h"
#include "util.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "task.h"
#include "constants/rgb.h"

enum
{
    NORMAL_FADE,
    FAST_FADE,
    HARDWARE_FADE,
    TIME_OF_DAY_FADE,
};

// These are structs for some unused palette system.
// The full functionality of this system is unknown.

struct PaletteStructTemplate
{
    u16 uid;
    u16 *src;
    u16 pst_field_8_0:1;
    u16 pst_field_8_1:9;
    u16 size:5;
    u16 pst_field_9_7:1;
    u8 pst_field_A;
    u8 srcCount:5;
    u8 pst_field_B_5:3;
    u8 pst_field_C;
};

struct PaletteStruct
{
    const struct PaletteStructTemplate *base;
    u32 ps_field_4_0:1;
    u16 ps_field_4_1:1;
    u32 baseDestOffset:9;
    u16 destOffset:10;
    u16 srcIndex:7;
    u8 ps_field_8;
    u8 ps_field_9;
};

static void unused_sub_80A1CDC(struct PaletteStruct *, u32 *);
static void unused_sub_80A1E40(struct PaletteStruct *, u32 *);
static void unused_sub_80A1F00(struct PaletteStruct *);
static u8 GetPaletteNumByUid(u16);
static u8 UpdateNormalPaletteFade(void);
static void BeginFastPaletteFadeInternal(u8);
static u8 UpdateFastPaletteFade(void);
static u8 UpdateHardwarePaletteFade(void);
static u8 UpdateTimeOfDayPaletteFade(void);
static void UpdateBlendRegisters(void);
static bool8 IsSoftwarePaletteFadeFinishing(void);
static void Task_BlendPalettesGradually(u8 taskId);

// palette buffers require alignment with agbcc because
// unaligned word reads are issued in BlendPalette otherwise
ALIGNED(4) EWRAM_DATA u16 gPlttBufferUnfaded[PLTT_BUFFER_SIZE] = {0};
ALIGNED(4) EWRAM_DATA u16 gPlttBufferFaded[PLTT_BUFFER_SIZE] = {0};
EWRAM_DATA struct PaletteStruct sPaletteStructs[0x10] = {0};
EWRAM_DATA struct PaletteFadeControl gPaletteFade = {0};
static EWRAM_DATA u32 sFiller = 0;
static EWRAM_DATA u32 sPlttBufferTransferPending = 0;
EWRAM_DATA u8 gPaletteDecompressionBuffer[PLTT_DECOMP_BUFFER_SIZE] = {0};

static const struct PaletteStructTemplate gDummyPaletteStructTemplate = {
    .uid = 0xFFFF,
    .pst_field_B_5 = 1
};

static const u8 sRoundedDownGrayscaleMap[] = {
     0,  0,  0,  0,  0,
     5,  5,  5,  5,  5,
    11, 11, 11, 11, 11,
    16, 16, 16, 16, 16,
    21, 21, 21, 21, 21,
    27, 27, 27, 27, 27,
    31, 31
};

void LoadCompressedPalette(const u32 *src, u16 offset, u16 size)
{
    LZDecompressWram(src, gPaletteDecompressionBuffer);
    CpuCopy16(gPaletteDecompressionBuffer, gPlttBufferUnfaded + offset, size);
    CpuCopy16(gPaletteDecompressionBuffer, gPlttBufferFaded + offset, size);
}

void LoadPalette(const void *src, u16 offset, u16 size)
{
    CpuCopy16(src, gPlttBufferUnfaded + offset, size);
    CpuCopy16(src, gPlttBufferFaded + offset, size);
}

void FillPalette(u16 value, u16 offset, u16 size)
{
    CpuFill16(value, gPlttBufferUnfaded + offset, size);
    CpuFill16(value, gPlttBufferFaded + offset, size);
}

void TransferPlttBuffer(void)
{
    if (!gPaletteFade.bufferTransferDisabled)
    {
        void *src = gPlttBufferFaded;
        void *dest = (void *)PLTT;
        DmaCopy16(3, src, dest, PLTT_SIZE);
        sPlttBufferTransferPending = 0;
        if (gPaletteFade.mode == HARDWARE_FADE && gPaletteFade.active)
            UpdateBlendRegisters();
    }
}

u8 UpdatePaletteFade(void)
{
    u8 result;
    u8 dummy = 0;

    if (sPlttBufferTransferPending)
        return PALETTE_FADE_STATUS_LOADING;

    if (gPaletteFade.mode == NORMAL_FADE)
        result = UpdateNormalPaletteFade();
    else if (gPaletteFade.mode == FAST_FADE)
        result = UpdateFastPaletteFade();
    else if (gPaletteFade.mode == TIME_OF_DAY_FADE)
        result = UpdateTimeOfDayPaletteFade();
    else
        result = UpdateHardwarePaletteFade();

    sPlttBufferTransferPending = gPaletteFade.multipurpose1 | dummy;

    return result;
}

void ResetPaletteFade(void)
{
    u8 i;

    for (i = 0; i < 16; i++)
        ResetPaletteStruct(i);

    ResetPaletteFadeControl();
}

void ReadPlttIntoBuffers(void)
{
    u16 i;
    u16 *pltt = (u16 *)PLTT;

    for (i = 0; i < PLTT_SIZE / 2; i++)
    {
        gPlttBufferUnfaded[i] = pltt[i];
        gPlttBufferFaded[i] = pltt[i];
    }
}

bool8 BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, u16 blendColor)
{
    u8 temp;
    u16 color = blendColor;

    if (gPaletteFade.active)
    {
        return FALSE;
    }
    else
    {
        gPaletteFade.deltaY = 2;

        if (delay < 0)
        {
            gPaletteFade.deltaY += (delay * -1);
            delay = 0;
        }

        gPaletteFade_selectedPalettes = selectedPalettes;
        gPaletteFade.delayCounter = delay;
        gPaletteFade_delay = delay;
        gPaletteFade.y = startY;
        gPaletteFade.targetY = targetY;
        gPaletteFade.blendColor = color;
        gPaletteFade.active = 1;
        gPaletteFade.mode = NORMAL_FADE;

        if (startY < targetY)
            gPaletteFade.yDec = 0;
        else
            gPaletteFade.yDec = 1;

        UpdatePaletteFade();

        temp = gPaletteFade.bufferTransferDisabled;
        gPaletteFade.bufferTransferDisabled = 0;
        CpuCopy32(gPlttBufferFaded, (void *)PLTT, PLTT_SIZE);
        sPlttBufferTransferPending = 0;
        if (gPaletteFade.mode == HARDWARE_FADE && gPaletteFade.active)
            UpdateBlendRegisters();
        gPaletteFade.bufferTransferDisabled = temp;
        return TRUE;
    }
}

// Like normal palette fade but respects sprite/tile palettes immune to time of day fading
bool8 BeginTimeOfDayPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, struct BlendSettings *bld0, struct BlendSettings *bld1, u16 weight, u16 color)
{
    u8 temp;

    if (gPaletteFade.active)
    {
        return FALSE;
    }
    else
    {
        gPaletteFade.deltaY = 2;

        if (delay < 0)
        {
            gPaletteFade.deltaY += (delay * -1);
            delay = 0;
        }

        gPaletteFade_selectedPalettes = selectedPalettes;
        gPaletteFade.delayCounter = delay;
        gPaletteFade_delay = delay;
        gPaletteFade.y = startY;
        gPaletteFade.targetY = targetY;
        gPaletteFade.active = 1;
        gPaletteFade.mode = TIME_OF_DAY_FADE;

        gPaletteFade.blendColor = color;
        gPaletteFade.bld0 = bld0;
        gPaletteFade.bld1 = bld1;
        gPaletteFade.weight = weight;

        if (startY < targetY)
            gPaletteFade.yDec = 0;
        else
            gPaletteFade.yDec = 1;

        UpdatePaletteFade();

        temp = gPaletteFade.bufferTransferDisabled;
        gPaletteFade.bufferTransferDisabled = 0;
        CpuCopy32(gPlttBufferFaded, (void *)PLTT, PLTT_SIZE);
        sPlttBufferTransferPending = 0;
        if (gPaletteFade.mode == HARDWARE_FADE && gPaletteFade.active)
            UpdateBlendRegisters();
        gPaletteFade.bufferTransferDisabled = temp;
        return TRUE;
    }
}

bool8 unref_sub_80A1C1C(u32 a1, u8 a2, u8 a3, u8 a4, u16 a5)
{
    ReadPlttIntoBuffers();
    return BeginNormalPaletteFade(a1, a2, a3, a4, a5);
}

void unref_sub_80A1C64(u8 a1, u32 *a2)
{
    u8 i;

    for (i = 0; i < 16; i++)
    {
        struct PaletteStruct *palstruct = &sPaletteStructs[i];
        if (palstruct->ps_field_4_0)
        {
            if (palstruct->base->pst_field_8_0 == a1)
            {
                u8 val1 = palstruct->srcIndex;
                u8 val2 = palstruct->base->srcCount;
                if (val1 == val2)
                {
                    unused_sub_80A1F00(palstruct);
                    if (!palstruct->ps_field_4_0)
                        continue;
                }
                if (palstruct->ps_field_8 == 0)
                    unused_sub_80A1CDC(palstruct, a2);
                else
                    palstruct->ps_field_8--;

                unused_sub_80A1E40(palstruct, a2);
            }
        }
    }
}

static void unused_sub_80A1CDC(struct PaletteStruct *a1, u32 *a2)
{
    s32 srcIndex;
    s32 srcCount;
    u8 i = 0;
    u16 srcOffset = a1->srcIndex * a1->base->size;

    if (!a1->base->pst_field_8_0)
    {
        while (i < a1->base->size)
        {
            gPlttBufferUnfaded[a1->destOffset] = a1->base->src[srcOffset];
            gPlttBufferFaded[a1->destOffset] = a1->base->src[srcOffset];
            i++;
            a1->destOffset++;
            srcOffset++;
        }
    }
    else
    {
        while (i < a1->base->size)
        {
            gPlttBufferFaded[a1->destOffset] = a1->base->src[srcOffset];
            i++;
            a1->destOffset++;
            srcOffset++;
        }
    }

    a1->destOffset = a1->baseDestOffset;
    a1->ps_field_8 = a1->base->pst_field_A;
    a1->srcIndex++;

    srcIndex = a1->srcIndex;
    srcCount = a1->base->srcCount;

    if (srcIndex >= srcCount)
    {
        if (a1->ps_field_9)
            a1->ps_field_9--;
        a1->srcIndex = 0;
    }

    *a2 |= 1 << (a1->baseDestOffset >> 4);
}

static void unused_sub_80A1E40(struct PaletteStruct *a1, u32 *a2)
{
    if (gPaletteFade.active && ((1 << (a1->baseDestOffset >> 4)) & gPaletteFade_selectedPalettes))
    {
        if (!a1->base->pst_field_8_0)
        {
            if (gPaletteFade.delayCounter != gPaletteFade_delay)
            {
                BlendPalette(
                    a1->baseDestOffset,
                    a1->base->size,
                    gPaletteFade.y,
                    gPaletteFade.blendColor);
            }
        }
        else
        {
            if (!gPaletteFade.delayCounter)
            {
                if (a1->ps_field_8 != a1->base->pst_field_A)
                {
                    u32 srcOffset = a1->srcIndex * a1->base->size;
                    u8 i;

                    for (i = 0; i < a1->base->size; i++)
                        gPlttBufferFaded[a1->baseDestOffset + i] = a1->base->src[srcOffset + i];
                }
            }
        }
    }
}

static void unused_sub_80A1F00(struct PaletteStruct *a1)
{
    if (!a1->ps_field_9)
    {
        s32 val = a1->base->pst_field_B_5;

        if (!val)
        {
            a1->srcIndex = 0;
            a1->ps_field_8 = a1->base->pst_field_A;
            a1->ps_field_9 = a1->base->pst_field_C;
            a1->destOffset = a1->baseDestOffset;
        }
        else
        {
            if (val < 0)
                return;
            if (val > 2)
                return;
            ResetPaletteStructByUid(a1->base->uid);
        }
    }
    else
    {
        a1->ps_field_9--;
    }
}

void ResetPaletteStructByUid(u16 a1)
{
    u8 paletteNum = GetPaletteNumByUid(a1);
    if (paletteNum != 16)
        ResetPaletteStruct(paletteNum);
}

void ResetPaletteStruct(u8 paletteNum)
{
    sPaletteStructs[paletteNum].base = &gDummyPaletteStructTemplate;
    sPaletteStructs[paletteNum].ps_field_4_0 = 0;
    sPaletteStructs[paletteNum].baseDestOffset = 0;
    sPaletteStructs[paletteNum].destOffset = 0;
    sPaletteStructs[paletteNum].srcIndex = 0;
    sPaletteStructs[paletteNum].ps_field_4_1 = 0;
    sPaletteStructs[paletteNum].ps_field_8 = 0;
    sPaletteStructs[paletteNum].ps_field_9 = 0;
}

void ResetPaletteFadeControl(void)
{
    gPaletteFade.multipurpose1 = 0;
    gPaletteFade.multipurpose2 = 0;
    gPaletteFade.delayCounter = 0;
    gPaletteFade.y = 0;
    gPaletteFade.targetY = 0;
    gPaletteFade.blendColor = 0;
    gPaletteFade.active = 0;
    gPaletteFade.multipurpose2 = 0; // assign same value twice
    gPaletteFade.yDec = 0;
    gPaletteFade.bufferTransferDisabled = 0;
    gPaletteFade.shouldResetBlendRegisters = 0;
    gPaletteFade.hardwareFadeFinishing = 0;
    gPaletteFade.softwareFadeFinishing = 0;
    gPaletteFade.softwareFadeFinishingCounter = 0;
    gPaletteFade.objPaletteToggle = 0;
    gPaletteFade.deltaY = 2;
}

void unref_sub_80A2048(u16 uid)
{
    u8 paletteNum = GetPaletteNumByUid(uid);
    if (paletteNum != 16)
        sPaletteStructs[paletteNum].ps_field_4_1 = 1;
}

void unref_sub_80A2074(u16 uid)
{
    u8 paletteNum = GetPaletteNumByUid(uid);
    if (paletteNum != 16)
        sPaletteStructs[paletteNum].ps_field_4_1 = 0;
}

static u8 GetPaletteNumByUid(u16 uid)
{
    u8 i;

    for (i = 0; i < 16; i++)
        if (sPaletteStructs[i].base->uid == uid)
            return i;

    return 16;
}

// Like normal palette fade, but respects sprite/tile palettes immune to time of day fading
static u8 UpdateTimeOfDayPaletteFade(void)
{
    u8 paletteNum;
    u16 paletteOffset;
    u16 selectedPalettes;
    u16 timePalettes = 0; // palettes passed to the time-blender
    u16 copyPalettes;
    u16 * src;
    u16 * dst;

    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;

    if (IsSoftwarePaletteFadeFinishing())
      return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;

    if (!gPaletteFade.objPaletteToggle)
    {
        if (gPaletteFade.delayCounter < gPaletteFade_delay)
        {
            gPaletteFade.delayCounter++;
            return 2;
        }
        gPaletteFade.delayCounter = 0;
    }

    paletteOffset = 0;

    if (!gPaletteFade.objPaletteToggle)
    {
        selectedPalettes = gPaletteFade_selectedPalettes;
    }
    else
    {
        selectedPalettes = gPaletteFade_selectedPalettes >> 16;
        paletteOffset = 256;
    }

    src = gPlttBufferUnfaded + paletteOffset;
    dst = gPlttBufferFaded + paletteOffset;

    // First pply TOD blend to relevant subset of palettes
    if (gPaletteFade.objPaletteToggle) { // Sprite palettes, don't blend those with tags
      u8 i;
      u16 j = 1;
      for (i = 0; i < 16; i++, j <<= 1) { // Mask out palettes that should not be light blended
        if ((selectedPalettes & j) && !(GetSpritePaletteTagByPaletteNum(i) >> 15))
          timePalettes |= j;
      }

    } else { // tile palettes, don't blend [13, 15]
      timePalettes = selectedPalettes & 0x1FFF;
    }
    TimeMixPalettes(timePalettes, src, dst, gPaletteFade.bld0, gPaletteFade.bld1, gPaletteFade.weight);

    // palettes that were not blended above must be copied through
    if ((copyPalettes = ~timePalettes)) {
      u16 * src1 = src;
      u16 * dst1 = dst;
      while (copyPalettes) {
        if (copyPalettes & 1)
          CpuFastCopy(src1, dst1, 32);
        copyPalettes >>= 1;
        src1 += 16;
        dst1 += 16;
      }
    }

    // Then, blend from faded->faded with native BlendPalettes
    BlendPalettesFine(selectedPalettes, dst, dst, gPaletteFade.y, gPaletteFade.blendColor);

    gPaletteFade.objPaletteToggle ^= 1;

    if (!gPaletteFade.objPaletteToggle)
    {
        if ((gPaletteFade.yDec && gPaletteFade.y == 0) || (!gPaletteFade.yDec && gPaletteFade.y == gPaletteFade.targetY))
        {
            gPaletteFade_selectedPalettes = 0;
            gPaletteFade.softwareFadeFinishing = 1;
        }
        else
        {
            s8 val;

            if (!gPaletteFade.yDec)
            {
                val = gPaletteFade.y;
                val += gPaletteFade.deltaY;
                if (val > gPaletteFade.targetY)
                    val = gPaletteFade.targetY;
                gPaletteFade.y = val;
            }
            else
            {
                val = gPaletteFade.y;
                val -= gPaletteFade.deltaY;
                if (val < 0)
                    val = 0;
                gPaletteFade.y = val;
            }
        }
    }

    // gPaletteFade.active cannot change since the last time it was checked. So this
    // is equivalent to `return PALETTE_FADE_STATUS_ACTIVE;`
    return PALETTE_FADE_STATUS_ACTIVE;
}

static u8 UpdateNormalPaletteFade(void)
{
    u16 paletteOffset;
    u16 selectedPalettes;

    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;

    if (IsSoftwarePaletteFadeFinishing())
    {
        return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;
    }
    else
    {
        if (!gPaletteFade.objPaletteToggle)
        {
            if (gPaletteFade.delayCounter < gPaletteFade_delay)
            {
                gPaletteFade.delayCounter++;
                return 2;
            }
            gPaletteFade.delayCounter = 0;
        }

        paletteOffset = 0;

        if (!gPaletteFade.objPaletteToggle)
        {
            selectedPalettes = gPaletteFade_selectedPalettes;
        }
        else
        {
            selectedPalettes = gPaletteFade_selectedPalettes >> 16;
            paletteOffset = 256;
        }

        while (selectedPalettes)
        {
            if (selectedPalettes & 1)
                BlendPalette(
                    paletteOffset,
                    16,
                    gPaletteFade.y,
                    gPaletteFade.blendColor);
            selectedPalettes >>= 1;
            paletteOffset += 16;
        }

        gPaletteFade.objPaletteToggle ^= 1;

        if (!gPaletteFade.objPaletteToggle)
        {
            if (gPaletteFade.y == gPaletteFade.targetY)
            {
                gPaletteFade_selectedPalettes = 0;
                gPaletteFade.softwareFadeFinishing = 1;
            }
            else
            {
                s8 val;

                if (!gPaletteFade.yDec)
                {
                    val = gPaletteFade.y;
                    val += gPaletteFade.deltaY;
                    if (val > gPaletteFade.targetY)
                        val = gPaletteFade.targetY;
                    gPaletteFade.y = val;
                }
                else
                {
                    val = gPaletteFade.y;
                    val -= gPaletteFade.deltaY;
                    if (val < gPaletteFade.targetY)
                        val = gPaletteFade.targetY;
                    gPaletteFade.y = val;
                }
            }
        }

        // gPaletteFade.active cannot change since the last time it was checked. So this
        // is equivalent to `return PALETTE_FADE_STATUS_ACTIVE;`
        return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;
    }
}

void InvertPlttBuffer(u32 selectedPalettes)
{
    u16 paletteOffset = 0;

    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
        {
            u8 i;
            for (i = 0; i < 16; i++)
                gPlttBufferFaded[paletteOffset + i] = ~gPlttBufferFaded[paletteOffset + i];
        }
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

void TintPlttBuffer(u32 selectedPalettes, s8 r, s8 g, s8 b)
{
    u16 paletteOffset = 0;

    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
        {
            u8 i;
            for (i = 0; i < 16; i++)
            {
                struct PlttData *data = (struct PlttData *)&gPlttBufferFaded[paletteOffset + i];
                data->r += r;
                data->g += g;
                data->b += b;
            }
        }
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

void UnfadePlttBuffer(u32 selectedPalettes)
{
    u16 paletteOffset = 0;

    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
        {
            u8 i;
            for (i = 0; i < 16; i++)
                gPlttBufferFaded[paletteOffset + i] = gPlttBufferUnfaded[paletteOffset + i];
        }
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

void BeginFastPaletteFade(u8 submode)
{
    gPaletteFade.deltaY = 2;
    BeginFastPaletteFadeInternal(submode);
}

static void BeginFastPaletteFadeInternal(u8 submode)
{
    gPaletteFade.y = 31;
    gPaletteFade_submode = submode & 0x3F;
    gPaletteFade.active = 1;
    gPaletteFade.mode = FAST_FADE;

    if (submode == FAST_FADE_IN_FROM_BLACK)
        CpuFill16(RGB_BLACK, gPlttBufferFaded, PLTT_SIZE);

    if (submode == FAST_FADE_IN_FROM_WHITE)
        CpuFill16(RGB_WHITE, gPlttBufferFaded, PLTT_SIZE);

    UpdatePaletteFade();
}

static u8 UpdateFastPaletteFade(void)
{
    u16 i;
    u16 paletteOffsetStart;
    u16 paletteOffsetEnd;
    s8 r0;
    s8 g0;
    s8 b0;
    s8 r;
    s8 g;
    s8 b;

    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;

    if (IsSoftwarePaletteFadeFinishing())
        return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;

    if (gPaletteFade.objPaletteToggle)
    {
        paletteOffsetStart = 256;
        paletteOffsetEnd = 512;
    }
    else
    {
        paletteOffsetStart = 0;
        paletteOffsetEnd = 256;
    }

    switch (gPaletteFade_submode)
    {
    case FAST_FADE_IN_FROM_WHITE:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; i++)
        {
            struct PlttData *unfaded;
            struct PlttData *faded;

            unfaded = (struct PlttData *)&gPlttBufferUnfaded[i];
            r0 = unfaded->r;
            g0 = unfaded->g;
            b0 = unfaded->b;

            faded = (struct PlttData *)&gPlttBufferFaded[i];
            r = faded->r - 2;
            g = faded->g - 2;
            b = faded->b - 2;

            if (r < r0)
                r = r0;
            if (g < g0)
                g = g0;
            if (b < b0)
                b = b0;

            gPlttBufferFaded[i] = RGB(r, g, b);
        }
        break;
    case FAST_FADE_OUT_TO_WHITE:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; i++)
        {
            struct PlttData *data = (struct PlttData *)&gPlttBufferFaded[i];
            r = data->r + 2;
            g = data->g + 2;
            b = data->b + 2;

            if (r > 31)
                r = 31;
            if (g > 31)
                g = 31;
            if (b > 31)
                b = 31;

            gPlttBufferFaded[i] = RGB(r, g, b);
        }
        break;
    case FAST_FADE_IN_FROM_BLACK:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; i++)
        {
            struct PlttData *unfaded;
            struct PlttData *faded;

            unfaded = (struct PlttData *)&gPlttBufferUnfaded[i];
            r0 = unfaded->r;
            g0 = unfaded->g;
            b0 = unfaded->b;

            faded = (struct PlttData *)&gPlttBufferFaded[i];
            r = faded->r + 2;
            g = faded->g + 2;
            b = faded->b + 2;

            if (r > r0)
                r = r0;
            if (g > g0)
                g = g0;
            if (b > b0)
                b = b0;

            gPlttBufferFaded[i] = RGB(r, g, b);
        }
        break;
    case FAST_FADE_OUT_TO_BLACK:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; i++)
        {
            struct PlttData *data = (struct PlttData *)&gPlttBufferFaded[i];
            r = data->r - 2;
            g = data->g - 2;
            b = data->b - 2;

            if (r < 0)
                r = 0;
            if (g < 0)
                g = 0;
            if (b < 0)
                b = 0;

            gPlttBufferFaded[i] = RGB(r, g, b);
        }
    }

    gPaletteFade.objPaletteToggle ^= 1;

    if (gPaletteFade.objPaletteToggle)
        // gPaletteFade.active cannot change since the last time it was checked. So this
        // is equivalent to `return PALETTE_FADE_STATUS_ACTIVE;`
        return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;

    if (gPaletteFade.y - gPaletteFade.deltaY < 0)
        gPaletteFade.y = 0;
    else
        gPaletteFade.y -= gPaletteFade.deltaY;

    if (gPaletteFade.y == 0)
    {
        switch (gPaletteFade_submode)
        {
        case FAST_FADE_IN_FROM_WHITE:
        case FAST_FADE_IN_FROM_BLACK:
            CpuCopy32(gPlttBufferUnfaded, gPlttBufferFaded, PLTT_SIZE);
            break;
        case FAST_FADE_OUT_TO_WHITE:
            CpuFill32(0xFFFFFFFF, gPlttBufferFaded, PLTT_SIZE);
            break;
        case FAST_FADE_OUT_TO_BLACK:
            CpuFill32(0x00000000, gPlttBufferFaded, PLTT_SIZE);
            break;
        }

        gPaletteFade.mode = NORMAL_FADE;
        gPaletteFade.softwareFadeFinishing = 1;
    }
    // gPaletteFade.active cannot change since the last time it was checked. So this
    // is equivalent to `return PALETTE_FADE_STATUS_ACTIVE;`
    return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;
}

void BeginHardwarePaletteFade(u8 blendCnt, u8 delay, u8 y, u8 targetY, u8 shouldResetBlendRegisters)
{
    gPaletteFade_blendCnt = blendCnt;
    gPaletteFade.delayCounter = delay;
    gPaletteFade_delay = delay;
    gPaletteFade.y = y;
    gPaletteFade.targetY = targetY;
    gPaletteFade.active = 1;
    gPaletteFade.mode = HARDWARE_FADE;
    gPaletteFade.shouldResetBlendRegisters = shouldResetBlendRegisters & 1;
    gPaletteFade.hardwareFadeFinishing = 0;

    if (y < targetY)
        gPaletteFade.yDec = 0;
    else
        gPaletteFade.yDec = 1;
}

static u8 UpdateHardwarePaletteFade(void)
{
    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;

    if (gPaletteFade.delayCounter < gPaletteFade_delay)
    {
        gPaletteFade.delayCounter++;
        return PALETTE_FADE_STATUS_DELAY;
    }

    gPaletteFade.delayCounter = 0;

    if (!gPaletteFade.yDec)
    {
        gPaletteFade.y++;
        if (gPaletteFade.y > gPaletteFade.targetY)
        {
            gPaletteFade.hardwareFadeFinishing++;
            gPaletteFade.y--;
        }
    }
    else
    {
        s32 y = gPaletteFade.y--;
        if (y - 1 < gPaletteFade.targetY)
        {
            gPaletteFade.hardwareFadeFinishing++;
            gPaletteFade.y++;
        }
    }

    if (gPaletteFade.hardwareFadeFinishing)
    {
        if (gPaletteFade.shouldResetBlendRegisters)
        {
            gPaletteFade_blendCnt = 0;
            gPaletteFade.y = 0;
        }
        gPaletteFade.shouldResetBlendRegisters = 0;
    }

    // gPaletteFade.active cannot change since the last time it was checked. So this
    // is equivalent to `return PALETTE_FADE_STATUS_ACTIVE;`
    return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;
}

static void UpdateBlendRegisters(void)
{
    SetGpuReg(REG_OFFSET_BLDCNT, (u16)gPaletteFade_blendCnt);
    SetGpuReg(REG_OFFSET_BLDY, gPaletteFade.y);
    if (gPaletteFade.hardwareFadeFinishing)
    {
        gPaletteFade.hardwareFadeFinishing = 0;
        gPaletteFade.mode = 0;
        gPaletteFade_blendCnt = 0;
        gPaletteFade.y = 0;
        gPaletteFade.active = 0;
    }
}

static bool8 IsSoftwarePaletteFadeFinishing(void)
{
    if (gPaletteFade.softwareFadeFinishing)
    {
        if (gPaletteFade.softwareFadeFinishingCounter == 4)
        {
            gPaletteFade.active = 0;
            gPaletteFade.softwareFadeFinishing = 0;
            gPaletteFade.softwareFadeFinishingCounter = 0;
        }
        else
        {
            gPaletteFade.softwareFadeFinishingCounter++;
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// optimized based on lucktyphlosion's BlendPalettesFine
void BlendPalettesFine(u32 palettes, u16 *src, u16 *dst, u32 coeff, u32 color) {
  s32 newR, newG, newB;

  if (!palettes)
    return;

  coeff *= 2;
  newR = (color << 27) >> 27;
  newG = (color << 22) >> 27;
  newB = (color << 17) >> 27;

  do {
    if (palettes & 1) {
      u16 *srcEnd = src + 16;
      while (src != srcEnd) { // Transparency is blended because it can matter for tile palettes
        u32 srcColor = *src;
        s32 r = (srcColor << 27) >> 27;
        s32 g = (srcColor << 22) >> 27;
        s32 b = (srcColor << 17) >> 27;

        *dst++ = ((r + (((newR - r) * coeff) >> 5)) << 0)
               | ((g + (((newG - g) * coeff) >> 5)) << 5)
               | ((b + (((newB - b) * coeff) >> 5)) << 10);
        src++;
      }
    } else {
      src += 16;
      dst += 16;
    }
    palettes >>= 1;
  } while (palettes);
}

void BlendPalettes(u32 palettes, u8 coeff, u16 color) {
  BlendPalettesFine(palettes, gPlttBufferUnfaded, gPlttBufferFaded, coeff, color);
}

#define DEFAULT_LIGHT_COLOR 0x3f9f

// Like BlendPalette, but ignores blendColor if the transparency high bit is set
// Optimization help by lucktyphlosion
void TimeBlendPalette(u16 palOffset, u32 coeff, u32 blendColor) {
  s32 newR, newG, newB, defR, defG, defB;
  u16 * src = gPlttBufferUnfaded + palOffset;
  u16 * dst = gPlttBufferFaded + palOffset;
  u32 defaultBlendColor = DEFAULT_LIGHT_COLOR;
  u16 *srcEnd = src + 16;
  u16 altBlendIndices = *dst++ = *src++; // color 0 is copied through unchanged
  u32 altBlendColor;

  coeff *= 2;
  newR = (blendColor << 27) >> 27;
  newG = (blendColor << 22) >> 27;
  newB = (blendColor << 17) >> 27;

  if (altBlendIndices >> 15) { // High bit set; bitmask of which colors to alt-blend
    // Note that bit 0 of altBlendIndices specifies color 1
    altBlendColor = src[14]; // color 15
    if (altBlendColor >> 15) { // Set alternate blend color
      defR = (altBlendColor << 27) >> 27;
      defG = (altBlendColor << 22) >> 27;
      defB = (altBlendColor << 17) >> 27;
    } else { // Set default blend color
      defR = (defaultBlendColor << 27) >> 27;
      defG = (defaultBlendColor << 22) >> 27;
      defB = (defaultBlendColor << 17) >> 27;
    }
  } else {
    altBlendIndices = 0;
  }
  while (src != srcEnd) {
    u32 srcColor = *src;
    s32 r = (srcColor << 27) >> 27;
    s32 g = (srcColor << 22) >> 27;
    s32 b = (srcColor << 17) >> 27;

    if (altBlendIndices & 1) {
      *dst = ((r + (((defR - r) * coeff) >> 5)) << 0)
                  | ((g + (((defG - g) * coeff) >> 5)) << 5)
                  | ((b + (((defB - b) * coeff) >> 5)) << 10);
    } else { // Use provided blend color
      *dst = ((r + (((newR - r) * coeff) >> 5)) << 0)
                  | ((g + (((newG - g) * coeff) >> 5)) << 5)
                  | ((b + (((newB - b) * coeff) >> 5)) << 10);
    }
    src++;
    dst++;
    altBlendIndices >>= 1;
  }
}

// Blends a weighted average of two blend parameters
// Parameters can be either blended (as in BlendPalettes) or tinted (as in TintPaletteRGB_Copy)
void TimeMixPalettes(u32 palettes, u16 *src, u16 *dst, struct BlendSettings *blend0, struct BlendSettings *blend1, u16 weight0) {
  s32 r0, g0, b0, r1, g1, b1, defR, defG, defB, altR, altG, altB;
  u32 color0, coeff0, color1, coeff1;
  bool8 tint0, tint1;
  u16 weight1;
  u32 defaultColor = DEFAULT_LIGHT_COLOR;

  if (!palettes)
    return;

  color0 = blend0->blendColor;
  tint0 = blend0->isTint;
  coeff0 = tint0 ? 8*2 : blend0->coeff*2;
  color1 = blend1->blendColor;
  tint1 = blend1->isTint;
  coeff1 = tint1 ? 8*2 : blend1->coeff*2;

  r0 = (color0 << 27) >> 27;
  g0 = (color0 << 22) >> 27;
  b0 = (color0 << 17) >> 27;
  r1 = (color1 << 27) >> 27;
  g1 = (color1 << 22) >> 27;
  b1 = (color1 << 17) >> 27;
  defR = (defaultColor << 27) >> 27;
  defG = (defaultColor << 22) >> 27;
  defB = (defaultColor << 17) >> 27;
  weight1 = 256 - weight0;

  do {
    if (palettes & 1) {
      u16 *srcEnd = src + 16;
      u16 altBlendIndices = *dst++ = *src++; // color 0 is copied through
      u32 altBlendColor;
      if (altBlendIndices >> 15) { // High bit set; bitmask of which colors to alt-blend
        // Note that bit 0 of altBlendIndices specifies color 1
        altBlendColor = src[14]; // color 15
        if (altBlendColor >> 15) { // Set alternate blend color
          altR = (altBlendColor << 27) >> 27;
          altG = (altBlendColor << 22) >> 27;
          altB = (altBlendColor << 17) >> 27;
        } else {
          altBlendColor = 0;
        }
      } else {
        altBlendIndices = 0;
      }
      while (src != srcEnd) {
        u32 srcColor = *src;
        s32 r = (srcColor << 27) >> 27;
        s32 g = (srcColor << 22) >> 27;
        s32 b = (srcColor << 17) >> 27;

        if (altBlendIndices & 1) {
          if (altBlendColor) { // Use alternate blend color
            r = (weight0*(r + (((altR - r) * coeff0) >> 5)) + weight1*(r + (((altR - r) * coeff1) >> 5))) >> 8;
            g = (weight0*(g + (((altG - g) * coeff0) >> 5)) + weight1*(g + (((altG - g) * coeff1) >> 5))) >> 8;
            b = (weight0*(b + (((altB - b) * coeff0) >> 5)) + weight1*(b + (((altB - b) * coeff1) >> 5))) >> 8;
            *dst = RGB2(r, g, b);
          } else { // Use default blend color
            r = (weight0*(r + (((defR - r) * coeff0) >> 5)) + weight1*(r + (((defR - r) * coeff1) >> 5))) >> 8;
            g = (weight0*(g + (((defG - g) * coeff0) >> 5)) + weight1*(g + (((defG - g) * coeff1) >> 5))) >> 8;
            b = (weight0*(b + (((defB - b) * coeff0) >> 5)) + weight1*(b + (((defB - b) * coeff1) >> 5))) >> 8;
            *dst = RGB2(r, g, b);
          }
        } else { // Use provided blend colors
          s32 r2, g2, b2, r3, g3, b3;
          if (!tint0) { // blend-based
            r2 = (r + (((r0 - r) * coeff0) >> 5));
            g2 = (g + (((g0 - g) * coeff0) >> 5));
            b2 = (b + (((b0 - b) * coeff0) >> 5));
          } else { // tint-based
            r2 = (u16)(((r0 << 3) * r)) >> 8;
            g2 = (u16)(((g0 << 3) * g)) >> 8;
            b2 = (u16)(((b0 << 3) * b)) >> 8;
            if (r2 > 31)
                r2 = 31;
            if (g2 > 31)
                g2 = 31;
            if (b2 > 31)
                b2 = 31;
          }
          if (!tint1) { // blend-based
            r3 = (r + (((r1 - r) * coeff1) >> 5));
            g3 = (g + (((g1 - g) * coeff1) >> 5));
            b3 = (b + (((b1 - b) * coeff1) >> 5));
          } else { // tint-based
            r3 = (u16)(((r1 << 3) * r)) >> 8;
            g3 = (u16)(((g1 << 3) * g)) >> 8;
            b3 = (u16)(((b1 << 3) * b)) >> 8;
            if (r3 > 31)
                r3 = 31;
            if (g3 > 31)
                g3 = 31;
            if (b3 > 31)
                b3 = 31;
          }
          r = (weight0*r2 + weight1*r3) >> 8;
          g = (weight0*g2 + weight1*g3) >> 8;
          b = (weight0*b2 + weight1*b3) >> 8;
          *dst = RGB2(r, g, b);
        }
        src++;
        dst++;
        altBlendIndices >>= 1;
      }
    } else {
      src += 16;
      dst += 16;
    }
    palettes >>= 1;
  } while (palettes);
}

void BlendPalettesUnfaded(u32 selectedPalettes, u8 coeff, u16 color)
{
    void *src = gPlttBufferUnfaded;
    void *dest = gPlttBufferFaded;
    DmaCopy32(3, src, dest, PLTT_SIZE);
    BlendPalettes(selectedPalettes, coeff, color);
}

void TintPalette_GrayScale(u16 *palette, u16 count)
{
    s32 r, g, b, i;
    u32 gray;

    for (i = 0; i < count; i++)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);

        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;

        *palette++ = RGB2(gray, gray, gray);
    }
}

void TintPalette_GrayScale2(u16 *palette, u16 count)
{
    s32 r, g, b, i;
    u32 gray;

    for (i = 0; i < count; i++)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);

        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;

        if (gray > 31)
            gray = 31;

        gray = sRoundedDownGrayscaleMap[gray];

        *palette++ = RGB2(gray, gray, gray);
    }
}

void TintPalette_SepiaTone(u16 *palette, u16 count)
{
    s32 r, g, b, i;
    u32 gray;

    for (i = 0; i < count; i++)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);

        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;

        r = (u16)((Q_8_8(1.2) * gray)) >> 8;
        g = (u16)((Q_8_8(1.0) * gray)) >> 8;
        b = (u16)((Q_8_8(0.94) * gray)) >> 8;

        if (r > 31)
            r = 31;

        *palette++ = RGB2(r, g, b);
    }
}

void TintPalette_CustomTone(u16 *palette, u16 count, u16 rTone, u16 gTone, u16 bTone)
{
    s32 r, g, b, i;
    u32 gray;

    for (i = 0; i < count; i++)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);

        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;

        r = (u16)((rTone * gray)) >> 8;
        g = (u16)((gTone * gray)) >> 8;
        b = (u16)((bTone * gray)) >> 8;

        if (r > 31)
            r = 31;
        if (g > 31)
            g = 31;
        if (b > 31)
            b = 31;

        *palette++ = RGB2(r, g, b);
    }
}

// Tints from Unfaded to Faded, using a 15-bit GBA color
void TintPalette_RGB_Copy(u16 palOffset, u32 blendColor) {
  s32 newR, newG, newB, rTone, gTone, bTone;
  u16 * src = gPlttBufferUnfaded + palOffset;
  u16 * dst = gPlttBufferFaded + palOffset;
  u32 defaultBlendColor = DEFAULT_LIGHT_COLOR;
  u16 *srcEnd = src + 16;
  u16 altBlendIndices = *dst++ = *src++; // color 0 is copied through unchanged
  u32 altBlendColor;

  newR = ((blendColor << 27) >> 27) << 3;
  newG = ((blendColor << 22) >> 27) << 3;
  newB = ((blendColor << 17) >> 27) << 3;

  if (altBlendIndices >> 15) { // High bit set; bitmask of which colors to alt-blend
    // Note that bit 0 of altBlendIndices specifies color 1
    altBlendColor = src[14]; // color 15
    if (altBlendColor >> 15) { // Set alternate blend color
      rTone = ((altBlendColor << 27) >> 27) << 3;
      gTone = ((altBlendColor << 22) >> 27) << 3;
      bTone = ((altBlendColor << 17) >> 27) << 3;
    } else { // Set default blend color
      rTone = ((defaultBlendColor << 27) >> 27) << 3;
      gTone = ((defaultBlendColor << 22) >> 27) << 3;
      bTone = ((defaultBlendColor << 17) >> 27) << 3;
    }
  } else {
    altBlendIndices = 0;
  }
  while (src != srcEnd) {
    u32 srcColor = *src;
    s32 r = (srcColor << 27) >> 27;
    s32 g = (srcColor << 22) >> 27;
    s32 b = (srcColor << 17) >> 27;

    if (altBlendIndices & 1) {
      r = (u16)((rTone * r)) >> 8;
      g = (u16)((gTone * g)) >> 8;
      b = (u16)((bTone * b)) >> 8;
    } else { // Use provided blend color
      r = (u16)((newR * r)) >> 8;
      g = (u16)((newG * g)) >> 8;
      b = (u16)((newB * b)) >> 8;
    }
    if (r > 31)
        r = 31;
    if (g > 31)
        g = 31;
    if (b > 31)
        b = 31;
    src++;
    *dst++ = RGB2(r, g, b);
    altBlendIndices >>= 1;
  }
}

#define tCoeff       data[0]
#define tCoeffTarget data[1]
#define tCoeffDelta  data[2]
#define tDelay       data[3]
#define tDelayTimer  data[4]
#define tPalettes    5 // data[5] and data[6], set/get via Set/GetWordTaskArg
#define tColor       data[7]
#define tId          data[8]

// Blend the selected palettes in a series of steps toward or away from the color.
// Only used by the Groudon/Kyogre fight scene to flash the screen for lightning
// One call is used to fade the bg from white, while another fades the duo from black
void BlendPalettesGradually(u32 selectedPalettes, s8 delay, u8 coeff, u8 coeffTarget, u16 color, u8 priority, u8 id)
{
    u8 taskId;

    taskId = CreateTask((void *)Task_BlendPalettesGradually, priority);
    gTasks[taskId].tCoeff = coeff;
    gTasks[taskId].tCoeffTarget = coeffTarget;

    if (delay >= 0)
    {
        gTasks[taskId].tDelay = delay;
        gTasks[taskId].tCoeffDelta = 1;
    }
    else
    {
        gTasks[taskId].tDelay = 0;
        gTasks[taskId].tCoeffDelta = -delay + 1;
    }

    if (coeffTarget < coeff)
        gTasks[taskId].tCoeffDelta *= -1;

    SetWordTaskArg(taskId, tPalettes, selectedPalettes);
    gTasks[taskId].tColor = color;
    gTasks[taskId].tId = id;
    gTasks[taskId].func(taskId);
}

// Unused
static bool32 IsBlendPalettesGraduallyTaskActive(u8 id)
{
    int i;

    for (i = 0; i < NUM_TASKS; i++)
        if ((gTasks[i].isActive == TRUE)
            && (gTasks[i].func == Task_BlendPalettesGradually)
            && (gTasks[i].tId == id))
            return TRUE;

    return FALSE;
}

// Unused
static void DestroyBlendPalettesGraduallyTask(void)
{
    u8 taskId;

    while (1)
    {
        taskId = FindTaskIdByFunc(Task_BlendPalettesGradually);
        if (taskId == TASK_NONE)
            break;
        DestroyTask(taskId);
    }
}

static void Task_BlendPalettesGradually(u8 taskId)
{
    u32 palettes;
    s16 *data;
    s16 target;

    data = gTasks[taskId].data;
    palettes = GetWordTaskArg(taskId, tPalettes);

    if (++tDelayTimer > tDelay)
    {
        tDelayTimer = 0;
        BlendPalettes(palettes, tCoeff, tColor);
        target = tCoeffTarget;
        if (tCoeff == target)
        {
            DestroyTask(taskId);
        }
        else
        {
            tCoeff += tCoeffDelta;
            if (tCoeffDelta >= 0)
            {
                if (tCoeff < target)
                    return;
            }
            else if (tCoeff > target)
            {
                return;
            }
            tCoeff = target;
        }
    }
}
