#include "global.h"
#include "characters.h"
#include "constants/region_map_sections.h"
#include "main.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "soul_link.h"
#include "tx_randomizer_and_challenges.h"

EWRAM_DATA volatile struct SoulLinkMailbox gSoulLinkMailbox = {0};
EWRAM_DATA volatile u16 gSoulLinkLobbyState = SOUL_LINK_LOBBY_DISCONNECTED;
EWRAM_DATA volatile u8 gSoulLinkConnectedPlayerMask = 0;
EWRAM_DATA volatile u8 gSoulLinkReadyPlayerMask = 0;
EWRAM_DATA volatile u8 gSoulLinkLocalPlayerMask = 0;
EWRAM_DATA volatile u8 gSoulLinkGateState = SOUL_LINK_GATE_IDLE;
EWRAM_DATA volatile u8 gSoulLinkLockedPlayerMask = 0;
EWRAM_DATA volatile bool8 gSoulLinkPartyReady = FALSE;
EWRAM_DATA volatile struct SoulLinkSaveData gSoulLinkPendingRun = {0};
EWRAM_DATA u16 gSoulLinkPendingRandomizerSettings = 0;
static EWRAM_DATA bool8 sEncounterEventPending = FALSE;
static EWRAM_DATA struct SoulLinkMessage sPendingEncounterEvent = {0};
static EWRAM_DATA u16 sPendingDeathGroups[PARTY_SIZE] = {0};
static EWRAM_DATA u8 sPendingDeathCount = 0;
static EWRAM_DATA bool8 sCemeteryCleanupPending = FALSE;
static EWRAM_DATA u8 sLobbyIntent = SOUL_LINK_INTENT_NONE;
static EWRAM_DATA u16 sSnapshotIndex = 0;
static EWRAM_DATA u16 sSnapshotMemberCount = 0;
static EWRAM_DATA u8 sSnapshotState = 0;
static EWRAM_DATA bool8 sSnapshotPendingReplay = FALSE;
static EWRAM_DATA u8 sSnapshotPublishedLocations[SOUL_LINK_FAILED_LOCATION_BYTES] = {0};
static EWRAM_DATA u8 sRegistryPendingRequest = 0;
static EWRAM_DATA u8 sRegistryResultType = 0;
static EWRAM_DATA bool8 sRegistryResultReady = FALSE;
static EWRAM_DATA bool8 sRegistryResultValid = FALSE;
static EWRAM_DATA u16 sRegistryGroupCount = 0;
static EWRAM_DATA struct SoulLinkRegistryMember sRegistryMember = {0};
static EWRAM_DATA u8 sRegistryPlayerName[PLAYER_NAME_LENGTH + 1] = {0};

enum
{
    SNAPSHOT_IDLE,
    SNAPSHOT_BEGIN,
    SNAPSHOT_MEMBERS,
    SNAPSHOT_MISSED,
    SNAPSHOT_END,
};

#define SNAPSHOT_MON_COUNT (PARTY_SIZE + TOTAL_BOXES_COUNT * IN_BOX_COUNT)

STATIC_ASSERT(sizeof(struct SoulLinkMessage) == 24, SoulLinkMessageSize);
STATIC_ASSERT(sizeof(struct SoulLinkMailbox) == 68, SoulLinkMailboxSize);

static bool8 QueueEncounterEvent(u16 type, u32 personality, u32 otId,
                                 u16 species, u16 location);

u16 SoulLink_GetBoxMonGroupId(struct BoxPokemon *boxMon)
{
    return GetBoxMonData(boxMon, MON_DATA_SOUL_LINK_GROUP);
}

void SoulLink_SetBoxMonGroupId(struct BoxPokemon *boxMon, u16 groupId)
{
    SetBoxMonData(boxMon, MON_DATA_SOUL_LINK_GROUP, &groupId);
}

void SoulLink_SetPendingRandomizerSettings(u16 settings)
{
    gSoulLinkPendingRandomizerSettings =
        settings & SOUL_LINK_RANDOMIZER_SETTINGS_MASK;
}

static void ResetMailbox(void)
{
    gSoulLinkLobbyState = SOUL_LINK_LOBBY_DISCONNECTED;
    gSoulLinkConnectedPlayerMask = 0;
    gSoulLinkPartyReady = FALSE;
    gSoulLinkReadyPlayerMask = 0;
    gSoulLinkLocalPlayerMask = 0;
    gSoulLinkGateState = SOUL_LINK_GATE_IDLE;
    gSoulLinkLockedPlayerMask = 0;
    memset((void *)&gSoulLinkPendingRun, 0, sizeof(gSoulLinkPendingRun));
    memset((void *)&gSoulLinkMailbox, 0, sizeof(gSoulLinkMailbox));
    sSnapshotState = SNAPSHOT_IDLE;
    sLobbyIntent = SOUL_LINK_INTENT_NONE;
    sRegistryPendingRequest = 0;
    sRegistryResultType = 0;
    sRegistryResultReady = FALSE;
    sRegistryResultValid = FALSE;
    sRegistryGroupCount = 0;
    sPendingDeathCount = 0;
    gSoulLinkMailbox.protocolVersion = SOUL_LINK_PROTOCOL_VERSION;
    gSoulLinkMailbox.size = sizeof(gSoulLinkMailbox);

    // Publish the magic last so Lua never accepts a partially reset mailbox.
    gSoulLinkMailbox.magic = SOUL_LINK_MAILBOX_MAGIC;
}

static u16 GetRunRandomizerSettings(const struct SoulLinkSaveData *run)
{
    return run->randomizerSettings[0] | (run->randomizerSettings[1] << 8);
}

static void UpgradeRunVersion(struct SoulLinkSaveData *run)
{
    if (run->formatVersion == SOUL_LINK_PREVIOUS_SAVE_FORMAT_VERSION
     && run->protocolVersion == SOUL_LINK_LEGACY_PROTOCOL_VERSION)
    {
        run->formatVersion = SOUL_LINK_SAVE_FORMAT_VERSION;
        run->protocolVersion = SOUL_LINK_PROTOCOL_VERSION;
    }
    else if (run->formatVersion == SOUL_LINK_SAVE_FORMAT_VERSION
          && run->protocolVersion > SOUL_LINK_LEGACY_PROTOCOL_VERSION
          && run->protocolVersion < SOUL_LINK_PROTOCOL_VERSION)
        run->protocolVersion = SOUL_LINK_PROTOCOL_VERSION;
}

static bool8 IsFailedLocation(u16 location)
{
    const volatile struct SoulLinkSaveData *run =
        (gSoulLinkPendingRun.status & SOUL_LINK_RUN_STATUS_ACTIVE)
        ? &gSoulLinkPendingRun : &gSaveBlock2Ptr->soulLink;

    return location / 8 < SOUL_LINK_FAILED_LOCATION_BYTES
        && (run->failedLocations[location / 8] & (1 << (location % 8)));
}

static void SetFailedLocation(u16 location)
{
    if (location / 8 >= SOUL_LINK_FAILED_LOCATION_BYTES)
        return;
    gSaveBlock2Ptr->soulLink.failedLocations[location / 8] |= 1 << (location % 8);
    gSoulLinkPendingRun.failedLocations[location / 8] |= 1 << (location % 8);
}

static u8 CountPlayers(u8 playerMask)
{
    u8 count = 0;
    u8 bit;

    for (bit = 0; bit < 4; bit++)
    {
        if (playerMask & (1 << bit))
            count++;
    }
    return count;
}

static bool8 TryPublishOutgoing(u16 type, u32 personality, u32 otId,
                                u16 pairId, u16 species, u16 location,
                                u16 flags, u16 data)
{
    volatile struct SoulLinkMessage *message = &gSoulLinkMailbox.outgoing;
    u32 sequence;

    if (message->sequence != gSoulLinkMailbox.outgoingAck)
        return FALSE;

    sequence = message->sequence + 1;
    if (sequence == 0)
        sequence = 1;
    message->personality = personality;
    message->otId = otId;
    message->type = type;
    message->pairId = pairId;
    message->species = species;
    message->location = location;
    message->flags = flags;
    message->reserved = data;
    message->sequence = sequence;
    return TRUE;
}

bool8 SoulLink_IsActive(void)
{
    return (gSoulLinkPendingRun.status & SOUL_LINK_RUN_STATUS_ACTIVE)
        || (gSaveBlock2Ptr->soulLink.status & SOUL_LINK_RUN_STATUS_ACTIVE);
}

bool8 SoulLink_CanStartTrainerBattle(void)
{
    return !SoulLink_IsActive() || gSoulLinkPartyReady;
}

bool8 SoulLink_RequestRegistryCount(void)
{
    if (sRegistryPendingRequest)
        return sRegistryPendingRequest == SOUL_LINK_REGISTRY_REQUEST_COUNT;
    if (!TryPublishOutgoing(SOUL_LINK_EVENT_REGISTRY_REQUEST, 0, 0, 0,
            0, 0, SOUL_LINK_REGISTRY_REQUEST_COUNT, 0))
        return FALSE;
    sRegistryPendingRequest = SOUL_LINK_REGISTRY_REQUEST_COUNT;
    sRegistryResultReady = FALSE;
    return TRUE;
}

bool8 SoulLink_RequestRegistryMember(u16 row, u8 playerSlot)
{
    if (sRegistryPendingRequest)
        return sRegistryPendingRequest == SOUL_LINK_REGISTRY_REQUEST_MEMBER;
    if (playerSlot < 1 || playerSlot > 4
     || !TryPublishOutgoing(SOUL_LINK_EVENT_REGISTRY_REQUEST, 0, 0, row,
            0, playerSlot, SOUL_LINK_REGISTRY_REQUEST_MEMBER, 0))
        return FALSE;
    sRegistryPendingRequest = SOUL_LINK_REGISTRY_REQUEST_MEMBER;
    sRegistryResultReady = FALSE;
    return TRUE;
}

bool8 SoulLink_RequestRegistryGroupMember(u16 groupId, u8 playerSlot)
{
    if (sRegistryPendingRequest)
        return sRegistryPendingRequest == SOUL_LINK_REGISTRY_REQUEST_GROUP_MEMBER;
    if (groupId == SOUL_LINK_GROUP_NONE || playerSlot < 1 || playerSlot > 4
     || !TryPublishOutgoing(SOUL_LINK_EVENT_REGISTRY_REQUEST, 0, 0, groupId,
            0, playerSlot, SOUL_LINK_REGISTRY_REQUEST_GROUP_MEMBER, 0))
        return FALSE;
    sRegistryPendingRequest = SOUL_LINK_REGISTRY_REQUEST_GROUP_MEMBER;
    sRegistryResultReady = FALSE;
    return TRUE;
}

bool8 SoulLink_RequestRegistryPlayerName(u8 playerSlot)
{
    if (sRegistryPendingRequest)
        return sRegistryPendingRequest == SOUL_LINK_REGISTRY_REQUEST_PLAYER_NAME;
    if (playerSlot < 1 || playerSlot > 4
     || !TryPublishOutgoing(SOUL_LINK_EVENT_REGISTRY_REQUEST, 0, 0, 0,
            0, playerSlot, SOUL_LINK_REGISTRY_REQUEST_PLAYER_NAME, 0))
        return FALSE;
    sRegistryPendingRequest = SOUL_LINK_REGISTRY_REQUEST_PLAYER_NAME;
    sRegistryResultReady = FALSE;
    return TRUE;
}

bool8 SoulLink_TakeRegistryCount(u16 *count, bool8 *valid)
{
    if (!sRegistryResultReady
     || sRegistryResultType != SOUL_LINK_REGISTRY_REQUEST_COUNT)
        return FALSE;
    *count = sRegistryGroupCount;
    *valid = sRegistryResultValid;
    sRegistryResultReady = FALSE;
    return TRUE;
}

bool8 SoulLink_TakeRegistryMember(struct SoulLinkRegistryMember *member,
                                  bool8 *valid)
{
    if (!sRegistryResultReady
     || (sRegistryResultType != SOUL_LINK_REGISTRY_REQUEST_MEMBER
      && sRegistryResultType != SOUL_LINK_REGISTRY_REQUEST_GROUP_MEMBER))
        return FALSE;
    *member = sRegistryMember;
    *valid = sRegistryResultValid;
    sRegistryResultReady = FALSE;
    return TRUE;
}

bool8 SoulLink_TakeRegistryPlayerName(u8 *name, bool8 *valid)
{
    if (!sRegistryResultReady
     || sRegistryResultType != SOUL_LINK_REGISTRY_REQUEST_PLAYER_NAME)
        return FALSE;
    memcpy(name, sRegistryPlayerName, sizeof(sRegistryPlayerName));
    *valid = sRegistryResultValid;
    sRegistryResultReady = FALSE;
    return TRUE;
}

u8 SoulLink_GetPlayerSlot(void)
{
    return gSoulLinkPendingRun.playerSlot != 0
        ? gSoulLinkPendingRun.playerSlot : gSaveBlock2Ptr->soulLink.playerSlot;
}

u8 SoulLink_GetActivePlayerMask(void)
{
    return gSoulLinkPendingRun.activePlayerMask != 0
        ? gSoulLinkPendingRun.activePlayerMask
        : gSaveBlock2Ptr->soulLink.activePlayerMask;
}

void SoulLink_CancelRegistryRequest(void)
{
    sRegistryPendingRequest = 0;
    sRegistryResultReady = FALSE;
}

static void BeginLocalSnapshot(void)
{
    sSnapshotIndex = 0;
    sSnapshotMemberCount = 0;
    sSnapshotPendingReplay = FALSE;
    memset(sSnapshotPublishedLocations, 0, sizeof(sSnapshotPublishedLocations));
    sSnapshotState = SNAPSHOT_BEGIN;
}

void SoulLink_RefreshLocalSnapshot(void)
{
    if (SoulLink_IsActive())
        BeginLocalSnapshot();
}

void SoulLink_LinkStarter(struct Pokemon *mon)
{
    if (!SoulLink_IsActive()
     || SoulLink_GetBoxMonGroupId(&mon->box) != SOUL_LINK_GROUP_NONE)
        return;

    SoulLink_SetBoxMonGroupId(&mon->box, SOUL_LINK_STARTER_GROUP_ID);
    BeginLocalSnapshot();
}

static struct BoxPokemon *GetSnapshotBoxMon(u16 index)
{
    if (index < PARTY_SIZE)
        return &gPlayerParty[index].box;

    index -= PARTY_SIZE;
    return &gPokemonStoragePtr->boxes[index / IN_BOX_COUNT][index % IN_BOX_COUNT];
}

static u8 GetSnapshotPlayerSlot(void)
{
    if (gSoulLinkPendingRun.playerSlot != 0)
        return gSoulLinkPendingRun.playerSlot;
    return gSaveBlock2Ptr->soulLink.playerSlot;
}

static void PublishLocalSnapshot(void)
{
    struct BoxPokemon *boxMon;
    u8 stringBytes[POKEMON_NAME_LENGTH + 1];
    u32 word0 = 0;
    u32 word1 = 0;
    u16 word2 = 0;
    u16 groupId;
    u16 flags;

    if (sSnapshotState == SNAPSHOT_IDLE)
        return;

    if (sSnapshotState == SNAPSHOT_BEGIN)
    {
        memcpy(&word0, gSaveBlock2Ptr->playerName, sizeof(word0));
        memcpy(&word1, gSaveBlock2Ptr->playerName + sizeof(word0), sizeof(word1));
        if (TryPublishOutgoing(SOUL_LINK_EVENT_SNAPSHOT_BEGIN,
                word0, word1, GetSnapshotPlayerSlot(), 0, 0, 0, 0))
            sSnapshotState = SNAPSHOT_MEMBERS;
        return;
    }

    if (sSnapshotState == SNAPSHOT_END)
    {
        if (TryPublishOutgoing(SOUL_LINK_EVENT_SNAPSHOT_END, 0, 0,
                GetSnapshotPlayerSlot(), sSnapshotMemberCount, 0, 0, 0))
            sSnapshotState = SNAPSHOT_IDLE;
        return;
    }

    if (sSnapshotState == SNAPSHOT_MISSED)
    {
        while (sSnapshotIndex <= MAPSEC_SAFARI_ZONE_AREA6
            && (!IsFailedLocation(sSnapshotIndex)
             || (sSnapshotPublishedLocations[sSnapshotIndex / 8]
               & (1 << (sSnapshotIndex % 8)))))
            sSnapshotIndex++;
        if (sSnapshotIndex > MAPSEC_SAFARI_ZONE_AREA6)
        {
            sSnapshotState = SNAPSHOT_END;
            return;
        }
        if (TryPublishOutgoing(SOUL_LINK_EVENT_SNAPSHOT_MEMBER,
                0, 0, sSnapshotIndex + 1, SPECIES_NONE, sSnapshotIndex,
                SOUL_LINK_SNAPSHOT_FLAG_DEAD | SOUL_LINK_SNAPSHOT_FLAG_MISSED
                    | SOUL_LINK_SNAPSHOT_FLAG_FAILED, 0))
        {
            sSnapshotIndex++;
            sSnapshotMemberCount++;
        }
        return;
    }

    if (sSnapshotIndex >= SNAPSHOT_MON_COUNT)
    {
        sSnapshotIndex = 0;
        sSnapshotState = SNAPSHOT_MISSED;
        return;
    }

    boxMon = GetSnapshotBoxMon(sSnapshotIndex);
    groupId = SoulLink_GetBoxMonGroupId(boxMon);
    if (!GetBoxMonData(boxMon, MON_DATA_SANITY_HAS_SPECIES))
    {
        sSnapshotIndex++;
        return;
    }

    if (groupId != SOUL_LINK_STARTER_GROUP_ID
     && (groupId & SOUL_LINK_PENDING_GROUP_FLAG))
    {
        u16 location = (groupId & ~SOUL_LINK_PENDING_GROUP_FLAG) - 1;

        if (!sSnapshotPendingReplay)
        {
            if (QueueEncounterEvent(SOUL_LINK_EVENT_CATCH,
                    GetBoxMonData(boxMon, MON_DATA_PERSONALITY),
                    GetBoxMonData(boxMon, MON_DATA_OT_ID),
                    GetBoxMonData(boxMon, MON_DATA_SPECIES), location))
                sSnapshotPendingReplay = TRUE;
            return;
        }
        groupId = SOUL_LINK_GROUP_NONE;
    }

    if (groupId == SOUL_LINK_GROUP_NONE
     && GetBoxMonData(boxMon, MON_DATA_NUZLOCKE_RIBBON))
    {
        u16 location = GetBoxMonData(boxMon, MON_DATA_MET_LOCATION);

        if (location <= MAPSEC_SAFARI_ZONE_AREA6)
        {
            groupId = location + 1;
            SoulLink_SetBoxMonGroupId(boxMon, groupId);
            SetFailedLocation(location);
        }
    }
    if (groupId == SOUL_LINK_GROUP_NONE)
    {
        if (sSnapshotIndex >= PARTY_SIZE
         || TryPublishOutgoing(SOUL_LINK_EVENT_SNAPSHOT_MEMBER,
                0, 0, SOUL_LINK_GROUP_NONE,
                GetBoxMonData(boxMon, MON_DATA_SPECIES),
                GetBoxMonData(boxMon, MON_DATA_MET_LOCATION),
                SOUL_LINK_SNAPSHOT_FLAG_IN_PARTY, 0))
        {
            if (sSnapshotIndex < PARTY_SIZE)
                sSnapshotMemberCount++;
            sSnapshotIndex++;
            sSnapshotPendingReplay = FALSE;
        }
        return;
    }

    GetBoxMonData(boxMon, MON_DATA_NICKNAME, stringBytes);
    memcpy(&word0, stringBytes, sizeof(word0));
    memcpy(&word1, stringBytes + sizeof(word0), sizeof(word1));
    memcpy(&word2, stringBytes + sizeof(word0) + sizeof(word1), sizeof(word2));
    flags = GetBoxMonData(boxMon, MON_DATA_NUZLOCKE_RIBBON)
        ? SOUL_LINK_SNAPSHOT_FLAG_DEAD : 0;
    if (groupId != SOUL_LINK_STARTER_GROUP_ID
     && IsFailedLocation(groupId - 1))
        flags |= SOUL_LINK_SNAPSHOT_FLAG_FAILED;
    if (sSnapshotIndex < PARTY_SIZE)
        flags |= SOUL_LINK_SNAPSHOT_FLAG_IN_PARTY;
    if (TryPublishOutgoing(SOUL_LINK_EVENT_SNAPSHOT_MEMBER,
            word0, word1, groupId,
            GetBoxMonData(boxMon, MON_DATA_SPECIES),
            GetBoxMonData(boxMon, MON_DATA_MET_LOCATION),
            flags, word2))
    {
        if (groupId != SOUL_LINK_STARTER_GROUP_ID
         && (groupId - 1) / 8 < SOUL_LINK_FAILED_LOCATION_BYTES)
            sSnapshotPublishedLocations[(groupId - 1) / 8]
                |= 1 << ((groupId - 1) % 8);
        sSnapshotIndex++;
        sSnapshotMemberCount++;
    }
}

static struct BoxPokemon *FindOwnedBoxMon(u32 personality, u32 otId)
{
    u8 box;
    u8 position;

    for (position = 0; position < PARTY_SIZE; position++)
    {
        struct BoxPokemon *boxMon = &gPlayerParty[position].box;

        if (GetMonData(&gPlayerParty[position], MON_DATA_SPECIES) != SPECIES_NONE
         && GetBoxMonData(boxMon, MON_DATA_PERSONALITY) == personality
         && GetBoxMonData(boxMon, MON_DATA_OT_ID) == otId)
            return boxMon;
    }

    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (position = 0; position < IN_BOX_COUNT; position++)
        {
            struct BoxPokemon *boxMon = &gPokemonStoragePtr->boxes[box][position];

            if (GetBoxMonData(boxMon, MON_DATA_SANITY_HAS_SPECIES)
             && GetBoxMonData(boxMon, MON_DATA_PERSONALITY) == personality
             && GetBoxMonData(boxMon, MON_DATA_OT_ID) == otId)
                return boxMon;
        }
    }

    return NULL;
}

static void ApplyLinkCreated(const volatile struct SoulLinkMessage *message)
{
    struct BoxPokemon *boxMon;
    u16 currentGroup;

    if (message->pairId == SOUL_LINK_GROUP_NONE
     || message->flags != gSoulLinkLocalPlayerMask
     || !(message->pairId == SOUL_LINK_STARTER_GROUP_ID
       || message->pairId == message->location + 1))
        return;

    boxMon = FindOwnedBoxMon(message->personality, message->otId);
    if (boxMon == NULL)
        return;

    currentGroup = SoulLink_GetBoxMonGroupId(boxMon);
    if (currentGroup == SOUL_LINK_GROUP_NONE
     || currentGroup == (SOUL_LINK_PENDING_GROUP_FLAG | message->pairId))
        SoulLink_SetBoxMonGroupId(boxMon, message->pairId);
    if (currentGroup == SOUL_LINK_GROUP_NONE || currentGroup == message->pairId
     || currentGroup == (SOUL_LINK_PENDING_GROUP_FLAG | message->pairId))
        BeginLocalSnapshot();
}

static void ApplyEncounterClosed(const volatile struct SoulLinkMessage *message)
{
    struct BoxPokemon *boxMon;
    bool8 dead = TRUE;

    if (message->flags != gSoulLinkLocalPlayerMask
     || message->location > MAPSEC_SAFARI_ZONE_AREA6)
        return;

    NuzlockeFlagSet(message->location);
    SetFailedLocation(message->location);
    if (message->personality == 0 && message->otId == 0)
    {
        BeginLocalSnapshot();
        return;
    }

    boxMon = FindOwnedBoxMon(message->personality, message->otId);
    if (boxMon == NULL)
        return;
    if (SoulLink_GetBoxMonGroupId(boxMon) == SOUL_LINK_GROUP_NONE
     || SoulLink_GetBoxMonGroupId(boxMon)
          == (SOUL_LINK_PENDING_GROUP_FLAG | (message->location + 1)))
        SoulLink_SetBoxMonGroupId(boxMon, message->location + 1);
    SetBoxMonData(boxMon, MON_DATA_NUZLOCKE_RIBBON, &dead);
    sCemeteryCleanupPending = TRUE;
    BeginLocalSnapshot();
}

static void ApplyLinkDied(u16 groupId)
{
    u8 box, position;
    bool8 dead = TRUE;
    bool8 changed = FALSE;

    if (groupId == SOUL_LINK_GROUP_NONE)
        return;
    for (position = 0; position < PARTY_SIZE; position++)
    {
        struct Pokemon *mon = &gPlayerParty[position];

        if (GetMonData(mon, MON_DATA_SANITY_HAS_SPECIES)
         && SoulLink_GetBoxMonGroupId(&mon->box) == groupId
         && !GetMonData(mon, MON_DATA_NUZLOCKE_RIBBON))
        {
            SetMonData(mon, MON_DATA_NUZLOCKE_RIBBON, &dead);
            sCemeteryCleanupPending = changed = TRUE;
        }
    }
    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (position = 0; position < IN_BOX_COUNT; position++)
        {
            struct BoxPokemon *boxMon = &gPokemonStoragePtr->boxes[box][position];

            if (GetBoxMonData(boxMon, MON_DATA_SANITY_HAS_SPECIES)
             && SoulLink_GetBoxMonGroupId(boxMon) == groupId
             && !GetBoxMonData(boxMon, MON_DATA_NUZLOCKE_RIBBON))
            {
                SetBoxMonData(boxMon, MON_DATA_NUZLOCKE_RIBBON, &dead);
                changed = TRUE;
            }
        }
    }
    if (changed)
        BeginLocalSnapshot();
}

static void CleanupForfeitedPartyMons(void)
{
    u8 position;

    if (!sCemeteryCleanupPending || gMain.inBattle)
        return;
    for (position = 0; position < PARTY_SIZE; position++)
    {
        if (GetMonData(&gPlayerParty[position], MON_DATA_SANITY_HAS_SPECIES)
         && GetMonData(&gPlayerParty[position], MON_DATA_NUZLOCKE_RIBBON))
            NuzlockeDeletePartyMon(position);
    }
    CompactPartySlots();
    sCemeteryCleanupPending = FALSE;
    BeginLocalSnapshot();
}

bool8 SoulLink_SendLobbyIntent(u8 intent)
{
    struct SoulLinkSaveData *run = NULL;
    u16 flags = intent;
    u16 settings = 0;

    if (intent != SOUL_LINK_INTENT_NEW_GAME && intent != SOUL_LINK_INTENT_CONTINUE)
        return FALSE;
    if (intent == SOUL_LINK_INTENT_CONTINUE)
    {
        run = &gSaveBlock2Ptr->soulLink;
        UpgradeRunVersion(run);
        flags |= run->activePlayerMask << SOUL_LINK_INTENT_ACTIVE_MASK_SHIFT;
        flags |= (run->status & SOUL_LINK_RUN_STATUS_MASK)
            << SOUL_LINK_INTENT_STATUS_SHIFT;
        settings = GetRunRandomizerSettings(run);
    }
    sLobbyIntent = intent;
    return TryPublishOutgoing(SOUL_LINK_EVENT_LOBBY_INTENT,
        run == NULL ? 0 : run->runId[0], run == NULL ? 0 : run->runId[1],
        run == NULL ? SOUL_LINK_PROTOCOL_VERSION : run->protocolVersion,
        run == NULL ? SOUL_LINK_SAVE_FORMAT_VERSION : run->formatVersion,
        run == NULL ? 0 : run->playerSlot, flags, settings);
}

bool8 SoulLink_SendLobbyStart(void)
{
    return TryPublishOutgoing(SOUL_LINK_EVENT_LOBBY_START, 0, 0,
        SOUL_LINK_PROTOCOL_VERSION, SOUL_LINK_SAVE_FORMAT_VERSION, 0, 0, 0);
}

bool8 SoulLink_SendSettings(void)
{
    return TryPublishOutgoing(SOUL_LINK_EVENT_SETTINGS, 0, 0,
        SOUL_LINK_PROTOCOL_VERSION, SOUL_LINK_SAVE_FORMAT_VERSION, 0, 0,
        gSoulLinkPendingRandomizerSettings);
}

static bool8 QueueEncounterEvent(u16 type, u32 personality, u32 otId,
                                 u16 species, u16 location)
{
    struct SoulLinkSaveData *run = &gSaveBlock2Ptr->soulLink;

    if (!(run->status & SOUL_LINK_RUN_STATUS_ACTIVE))
        return FALSE;
    UpgradeRunVersion(run);
    if (run->protocolVersion != SOUL_LINK_PROTOCOL_VERSION)
        return FALSE;
    if (sEncounterEventPending)
        return FALSE;
    if (TryPublishOutgoing(type, personality, otId, 0, species, location, 0, 0))
        return TRUE;

    sPendingEncounterEvent.type = type;
    sPendingEncounterEvent.personality = personality;
    sPendingEncounterEvent.otId = otId;
    sPendingEncounterEvent.pairId = 0;
    sPendingEncounterEvent.species = species;
    sPendingEncounterEvent.location = location;
    sPendingEncounterEvent.flags = 0;
    sPendingEncounterEvent.reserved = 0;
    sEncounterEventPending = TRUE;
    return TRUE;
}

bool8 SoulLink_QueueCatch(u32 personality, u32 otId, u16 species, u16 location)
{
    struct BoxPokemon *boxMon;
    bool8 queued = QueueEncounterEvent(SOUL_LINK_EVENT_CATCH, personality,
        otId, species, location);

    if (queued && location <= MAPSEC_SAFARI_ZONE_AREA6)
    {
        boxMon = FindOwnedBoxMon(personality, otId);
        if (boxMon != NULL
         && SoulLink_GetBoxMonGroupId(boxMon) == SOUL_LINK_GROUP_NONE)
        {
            u16 pendingGroup = SOUL_LINK_PENDING_GROUP_FLAG | (location + 1);
            SoulLink_SetBoxMonGroupId(boxMon, pendingGroup);
        }
        BeginLocalSnapshot();
    }
    return queued;
}

bool8 SoulLink_QueueEncounterFailed(u16 location)
{
    return QueueEncounterEvent(SOUL_LINK_EVENT_ENCOUNTER_FAILED, 0, 0, 0,
        location);
}

bool8 SoulLink_QueueDeath(u16 groupId)
{
    struct SoulLinkSaveData *run = &gSaveBlock2Ptr->soulLink;
    u8 i;

    if (!(run->status & SOUL_LINK_RUN_STATUS_ACTIVE)
     || groupId == SOUL_LINK_GROUP_NONE
     || (groupId != SOUL_LINK_STARTER_GROUP_ID && (groupId & SOUL_LINK_PENDING_GROUP_FLAG)))
        return FALSE;
    UpgradeRunVersion(run);
    if (run->protocolVersion != SOUL_LINK_PROTOCOL_VERSION)
        return FALSE;
    for (i = 0; i < sPendingDeathCount; i++)
        if (sPendingDeathGroups[i] == groupId)
            return TRUE;
    if (sPendingDeathCount == 0
     && TryPublishOutgoing(SOUL_LINK_EVENT_DEATH, 0, 0, groupId, 0, 0, 0, 0))
        return TRUE;
    if (sPendingDeathCount >= ARRAY_COUNT(sPendingDeathGroups))
        return FALSE;
    sPendingDeathGroups[sPendingDeathCount++] = groupId;
    return TRUE;
}

void SoulLink_Update(void)
{
    u32 sequence;
    u16 flags;
    u16 state;
    u16 playerMask;

    if (gSoulLinkMailbox.magic != SOUL_LINK_MAILBOX_MAGIC
     || gSoulLinkMailbox.protocolVersion != SOUL_LINK_PROTOCOL_VERSION
     || gSoulLinkMailbox.size != sizeof(gSoulLinkMailbox))
        ResetMailbox();

    gSoulLinkMailbox.romHeartbeat++;

    sequence = gSoulLinkMailbox.incoming.sequence;
    if (sequence != 0 && sequence != gSoulLinkMailbox.incomingAck)
    {
        flags = gSoulLinkMailbox.incoming.flags;
        state = flags & SOUL_LINK_LOBBY_STATE_MASK;
        if (gSoulLinkMailbox.incoming.type == SOUL_LINK_EVENT_LOBBY_STATE
         && state <= SOUL_LINK_LOBBY_APPROVED)
        {
            gSoulLinkLobbyState = state;
            gSoulLinkConnectedPlayerMask =
                (flags >> SOUL_LINK_LOBBY_CONNECTED_SHIFT) & SOUL_LINK_LOBBY_PLAYER_MASK;
            gSoulLinkReadyPlayerMask =
                (flags >> SOUL_LINK_LOBBY_READY_SHIFT) & SOUL_LINK_LOBBY_PLAYER_MASK;
            gSoulLinkLocalPlayerMask =
                (flags >> SOUL_LINK_LOBBY_LOCAL_SHIFT) & SOUL_LINK_LOBBY_PLAYER_MASK;
        }
        else if (gSoulLinkMailbox.incoming.type == SOUL_LINK_EVENT_GATE_STATE
              && (flags & SOUL_LINK_GATE_STATE_MASK) <= SOUL_LINK_GATE_REJECTED)
        {
            state = flags & SOUL_LINK_GATE_STATE_MASK;
            playerMask =
                (flags >> SOUL_LINK_GATE_PLAYER_MASK_SHIFT) & SOUL_LINK_LOBBY_PLAYER_MASK;
            if (state != SOUL_LINK_GATE_APPROVED
             || (gSoulLinkMailbox.incoming.pairId == SOUL_LINK_PROTOCOL_VERSION
              && gSoulLinkMailbox.incoming.species == SOUL_LINK_SAVE_FORMAT_VERSION
              && gSoulLinkMailbox.incoming.location >= 1
              && gSoulLinkMailbox.incoming.location <= 4
              && (playerMask & (1 << (gSoulLinkMailbox.incoming.location - 1)))))
            {
                gSoulLinkGateState = state;
                gSoulLinkLockedPlayerMask = playerMask;
                if (state == SOUL_LINK_GATE_APPROVED)
                {
                    if (sLobbyIntent == SOUL_LINK_INTENT_CONTINUE)
                        memcpy((void *)gSoulLinkPendingRun.failedLocations,
                            gSaveBlock2Ptr->soulLink.failedLocations,
                            sizeof(gSoulLinkPendingRun.failedLocations));
                    else
                        memset((void *)gSoulLinkPendingRun.failedLocations, 0,
                            sizeof(gSoulLinkPendingRun.failedLocations));
                    gSoulLinkPendingRun.runId[0] = gSoulLinkMailbox.incoming.personality;
                    gSoulLinkPendingRun.runId[1] = gSoulLinkMailbox.incoming.otId;
                    gSoulLinkPendingRun.protocolVersion = gSoulLinkMailbox.incoming.pairId;
                    gSoulLinkPendingRun.formatVersion = gSoulLinkMailbox.incoming.species;
                    gSoulLinkPendingRun.playerSlot = gSoulLinkMailbox.incoming.location;
                    gSoulLinkPendingRun.activePlayerMask = playerMask;
                    gSoulLinkPendingRun.randomizerSettings[0] =
                        gSoulLinkMailbox.incoming.reserved;
                    gSoulLinkPendingRun.randomizerSettings[1] =
                        gSoulLinkMailbox.incoming.reserved >> 8;
                    gSoulLinkPendingRun.status = SOUL_LINK_RUN_STATUS_ACTIVE
                        | (CountPlayers(playerMask) << SOUL_LINK_RUN_PLAYER_COUNT_SHIFT);
                    BeginLocalSnapshot();
                }
                else
                {
                    memset((void *)&gSoulLinkPendingRun, 0, sizeof(gSoulLinkPendingRun));
                }
            }
        }
        else if (gSoulLinkMailbox.incoming.type == SOUL_LINK_EVENT_LINK_CREATED)
        {
            ApplyLinkCreated(&gSoulLinkMailbox.incoming);
        }
        else if (gSoulLinkMailbox.incoming.type == SOUL_LINK_EVENT_ENCOUNTER_FAILED)
        {
            ApplyEncounterClosed(&gSoulLinkMailbox.incoming);
        }
        else if (gSoulLinkMailbox.incoming.type == SOUL_LINK_EVENT_LINK_DIED)
        {
            ApplyLinkDied(gSoulLinkMailbox.incoming.pairId);
        }
        else if (gSoulLinkMailbox.incoming.type == SOUL_LINK_EVENT_PARTY_STATE)
        {
            gSoulLinkPartyReady = flags == 1;
        }
        else if (gSoulLinkMailbox.incoming.type == SOUL_LINK_EVENT_REGISTRY_RESULT
              && sRegistryPendingRequest != 0
              && sRegistryPendingRequest == (flags & 0xFF))
        {
            sRegistryResultType = sRegistryPendingRequest;
            if (sRegistryResultType == SOUL_LINK_REGISTRY_REQUEST_COUNT)
                sRegistryGroupCount = gSoulLinkMailbox.incoming.species;
            else if (sRegistryResultType == SOUL_LINK_REGISTRY_REQUEST_MEMBER
                  || sRegistryResultType == SOUL_LINK_REGISTRY_REQUEST_GROUP_MEMBER)
            {
                sRegistryMember.groupId = gSoulLinkMailbox.incoming.pairId;
                sRegistryMember.species = gSoulLinkMailbox.incoming.species;
                sRegistryMember.location = gSoulLinkMailbox.incoming.location;
                sRegistryMember.dead = (flags & SOUL_LINK_REGISTRY_RESULT_DEAD) != 0;
                sRegistryMember.missed =
                    (flags & SOUL_LINK_REGISTRY_RESULT_MISSED) != 0;
                memcpy(sRegistryMember.nickname,
                    (const void *)&gSoulLinkMailbox.incoming.personality, sizeof(u32));
                memcpy(sRegistryMember.nickname + sizeof(u32),
                    (const void *)&gSoulLinkMailbox.incoming.otId, sizeof(u32));
                memcpy(sRegistryMember.nickname + 2 * sizeof(u32),
                    (const void *)&gSoulLinkMailbox.incoming.reserved, sizeof(u16));
                sRegistryMember.nickname[POKEMON_NAME_LENGTH] = EOS;
            }
            else if (sRegistryResultType == SOUL_LINK_REGISTRY_REQUEST_PLAYER_NAME)
            {
                memcpy(sRegistryPlayerName,
                    (const void *)&gSoulLinkMailbox.incoming.personality, sizeof(u32));
                memcpy(sRegistryPlayerName + sizeof(u32),
                    (const void *)&gSoulLinkMailbox.incoming.otId, sizeof(u32));
                sRegistryPlayerName[PLAYER_NAME_LENGTH] = EOS;
            }
            sRegistryResultValid = (flags & SOUL_LINK_REGISTRY_RESULT_VALID) != 0;
            sRegistryPendingRequest = 0;
            sRegistryResultReady = TRUE;
        }

        // Unknown messages are consumed so malformed input cannot wedge the
        // single-message slot.
        gSoulLinkMailbox.incomingAck = sequence;
    }

    if (sEncounterEventPending
     && TryPublishOutgoing(sPendingEncounterEvent.type,
            sPendingEncounterEvent.personality, sPendingEncounterEvent.otId,
            sPendingEncounterEvent.pairId, sPendingEncounterEvent.species,
            sPendingEncounterEvent.location, sPendingEncounterEvent.flags,
            sPendingEncounterEvent.reserved))
        sEncounterEventPending = FALSE;

    if (sPendingDeathCount
     && TryPublishOutgoing(SOUL_LINK_EVENT_DEATH, 0, 0,
            sPendingDeathGroups[0], 0, 0, 0, 0))
    {
        u8 i;

        for (i = 1; i < sPendingDeathCount; i++)
            sPendingDeathGroups[i - 1] = sPendingDeathGroups[i];
        sPendingDeathCount--;
    }

    CleanupForfeitedPartyMons();
    PublishLocalSnapshot();
}
