#include "global.h"
#include "soul_link.h"

EWRAM_DATA volatile struct SoulLinkMailbox gSoulLinkMailbox = {0};
EWRAM_DATA volatile u16 gSoulLinkLobbyState = SOUL_LINK_LOBBY_DISCONNECTED;
EWRAM_DATA volatile u8 gSoulLinkConnectedPlayerMask = 0;
EWRAM_DATA volatile u8 gSoulLinkReadyPlayerMask = 0;
EWRAM_DATA volatile u8 gSoulLinkLocalPlayerMask = 0;
EWRAM_DATA volatile u8 gSoulLinkGateState = SOUL_LINK_GATE_IDLE;
EWRAM_DATA volatile u8 gSoulLinkLockedPlayerMask = 0;
EWRAM_DATA volatile struct SoulLinkSaveData gSoulLinkPendingRun = {0};
EWRAM_DATA u16 gSoulLinkPendingRandomizerSettings = 0;

STATIC_ASSERT(sizeof(struct SoulLinkMessage) == 24, SoulLinkMessageSize);
STATIC_ASSERT(sizeof(struct SoulLinkMailbox) == 68, SoulLinkMailboxSize);

void SoulLink_SetPendingRandomizerSettings(u16 settings)
{
    gSoulLinkPendingRandomizerSettings =
        settings & SOUL_LINK_RANDOMIZER_SETTINGS_MASK;
}

static void ResetMailbox(void)
{
    gSoulLinkLobbyState = SOUL_LINK_LOBBY_DISCONNECTED;
    gSoulLinkConnectedPlayerMask = 0;
    gSoulLinkReadyPlayerMask = 0;
    gSoulLinkLocalPlayerMask = 0;
    gSoulLinkGateState = SOUL_LINK_GATE_IDLE;
    gSoulLinkLockedPlayerMask = 0;
    memset((void *)&gSoulLinkPendingRun, 0, sizeof(gSoulLinkPendingRun));
    memset((void *)&gSoulLinkMailbox, 0, sizeof(gSoulLinkMailbox));
    gSoulLinkMailbox.protocolVersion = SOUL_LINK_PROTOCOL_VERSION;
    gSoulLinkMailbox.size = sizeof(gSoulLinkMailbox);

    // Publish the magic last so Lua never accepts a partially reset mailbox.
    gSoulLinkMailbox.magic = SOUL_LINK_MAILBOX_MAGIC;
}

static bool8 TryPublishOutgoing(u16 type, const struct SoulLinkSaveData *run, u16 flags)
{
    volatile struct SoulLinkMessage *message = &gSoulLinkMailbox.outgoing;
    u32 sequence;

    if (message->sequence != gSoulLinkMailbox.outgoingAck)
        return FALSE;

    sequence = message->sequence + 1;
    if (sequence == 0)
        sequence = 1;
    message->personality = run == NULL ? 0 : run->runId[0];
    message->otId = run == NULL ? 0 : run->runId[1];
    message->type = type;
    message->pairId = run == NULL ? SOUL_LINK_PROTOCOL_VERSION : run->protocolVersion;
    message->species = run == NULL ? SOUL_LINK_SAVE_FORMAT_VERSION : run->formatVersion;
    message->location = run == NULL ? 0 : run->playerSlot;
    message->flags = flags;
    message->reserved = 0;
    message->sequence = sequence;
    return TRUE;
}

bool8 SoulLink_SendLobbyIntent(u8 intent)
{
    const struct SoulLinkSaveData *run = NULL;
    u16 flags = intent;

    if (intent != SOUL_LINK_INTENT_NEW_GAME && intent != SOUL_LINK_INTENT_CONTINUE)
        return FALSE;
    if (intent == SOUL_LINK_INTENT_CONTINUE)
    {
        run = &gSaveBlock2Ptr->soulLink;
        flags |= run->activePlayerMask << SOUL_LINK_INTENT_ACTIVE_MASK_SHIFT;
    }
    return TryPublishOutgoing(SOUL_LINK_EVENT_LOBBY_INTENT, run, flags);
}

bool8 SoulLink_SendLobbyStart(void)
{
    return TryPublishOutgoing(SOUL_LINK_EVENT_LOBBY_START, NULL, 0);
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
                    gSoulLinkPendingRun.runId[0] = gSoulLinkMailbox.incoming.personality;
                    gSoulLinkPendingRun.runId[1] = gSoulLinkMailbox.incoming.otId;
                    gSoulLinkPendingRun.protocolVersion = gSoulLinkMailbox.incoming.pairId;
                    gSoulLinkPendingRun.formatVersion = gSoulLinkMailbox.incoming.species;
                    gSoulLinkPendingRun.playerSlot = gSoulLinkMailbox.incoming.location;
                    gSoulLinkPendingRun.activePlayerMask = playerMask;
                }
                else
                {
                    memset((void *)&gSoulLinkPendingRun, 0, sizeof(gSoulLinkPendingRun));
                }
            }
        }

        // Unknown messages are consumed so malformed input cannot wedge the
        // single-message slot.
        gSoulLinkMailbox.incomingAck = sequence;
    }
}
