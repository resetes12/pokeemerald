#include "global.h"
#include "soul_link.h"

EWRAM_DATA volatile struct SoulLinkMailbox gSoulLinkMailbox = {0};
EWRAM_DATA volatile u16 gSoulLinkLobbyState = SOUL_LINK_LOBBY_DISCONNECTED;
EWRAM_DATA volatile u8 gSoulLinkConnectedPlayerMask = 0;
EWRAM_DATA volatile u8 gSoulLinkReadyPlayerMask = 0;

STATIC_ASSERT(sizeof(struct SoulLinkMessage) == 24, SoulLinkMessageSize);
STATIC_ASSERT(sizeof(struct SoulLinkMailbox) == 68, SoulLinkMailboxSize);

static void ResetMailbox(void)
{
    gSoulLinkLobbyState = SOUL_LINK_LOBBY_DISCONNECTED;
    gSoulLinkConnectedPlayerMask = 0;
    gSoulLinkReadyPlayerMask = 0;
    memset((void *)&gSoulLinkMailbox, 0, sizeof(gSoulLinkMailbox));
    gSoulLinkMailbox.protocolVersion = SOUL_LINK_PROTOCOL_VERSION;
    gSoulLinkMailbox.size = sizeof(gSoulLinkMailbox);

    // Publish the magic last so Lua never accepts a partially reset mailbox.
    gSoulLinkMailbox.magic = SOUL_LINK_MAILBOX_MAGIC;
}

void SoulLink_Update(void)
{
    u32 sequence;
    u16 flags;
    u16 state;

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
        }

        // Unknown messages are consumed so malformed input cannot wedge the
        // single-message slot.
        gSoulLinkMailbox.incomingAck = sequence;
    }
}
