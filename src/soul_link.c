#include "global.h"
#include "soul_link.h"

EWRAM_DATA volatile struct SoulLinkMailbox gSoulLinkMailbox = {0};
EWRAM_DATA volatile u16 gSoulLinkLobbyState = SOUL_LINK_LOBBY_DISCONNECTED;

STATIC_ASSERT(sizeof(struct SoulLinkMessage) == 24, SoulLinkMessageSize);
STATIC_ASSERT(sizeof(struct SoulLinkMailbox) == 68, SoulLinkMailboxSize);

static void ResetMailbox(void)
{
    gSoulLinkLobbyState = SOUL_LINK_LOBBY_DISCONNECTED;
    memset((void *)&gSoulLinkMailbox, 0, sizeof(gSoulLinkMailbox));
    gSoulLinkMailbox.protocolVersion = SOUL_LINK_PROTOCOL_VERSION;
    gSoulLinkMailbox.size = sizeof(gSoulLinkMailbox);

    // Publish the magic last so Lua never accepts a partially reset mailbox.
    gSoulLinkMailbox.magic = SOUL_LINK_MAILBOX_MAGIC;
}

void SoulLink_Update(void)
{
    u32 sequence;

    if (gSoulLinkMailbox.magic != SOUL_LINK_MAILBOX_MAGIC
     || gSoulLinkMailbox.protocolVersion != SOUL_LINK_PROTOCOL_VERSION
     || gSoulLinkMailbox.size != sizeof(gSoulLinkMailbox))
        ResetMailbox();

    gSoulLinkMailbox.romHeartbeat++;

    sequence = gSoulLinkMailbox.incoming.sequence;
    if (sequence != 0 && sequence != gSoulLinkMailbox.incomingAck)
    {
        if (gSoulLinkMailbox.incoming.type == SOUL_LINK_EVENT_LOBBY_STATE
         && gSoulLinkMailbox.incoming.flags <= SOUL_LINK_LOBBY_APPROVED)
            gSoulLinkLobbyState = gSoulLinkMailbox.incoming.flags;

        // Unknown messages are consumed so malformed input cannot wedge the
        // single-message slot.
        gSoulLinkMailbox.incomingAck = sequence;
    }
}
