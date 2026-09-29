#ifndef GUARD_SOUL_LINK_H
#define GUARD_SOUL_LINK_H

#define SOUL_LINK_MAILBOX_MAGIC 0x4B4E4C53 // "SLNK" in little-endian memory
#define SOUL_LINK_PROTOCOL_VERSION 2

enum SoulLinkEventType
{
    SOUL_LINK_EVENT_NONE,
    SOUL_LINK_EVENT_PING,
    SOUL_LINK_EVENT_LOBBY_STATE,
};

enum SoulLinkLobbyState
{
    SOUL_LINK_LOBBY_DISCONNECTED,
    SOUL_LINK_LOBBY_WAITING,
    SOUL_LINK_LOBBY_READY,
    SOUL_LINK_LOBBY_REJECTED,
    SOUL_LINK_LOBBY_APPROVED,
};

// Writers fill the payload first and sequence last. Readers acknowledge each
// sequence once, so neither side overwrites an unconsumed message.
struct SoulLinkMessage
{
    u32 sequence;
    u32 personality;
    u32 otId;
    u16 type;
    u16 pairId;
    u16 species;
    u16 location;
    u16 flags;
    u16 reserved;
};

struct SoulLinkMailbox
{
    u32 magic;
    u16 protocolVersion;
    u16 size;
    u32 romHeartbeat;
    struct SoulLinkMessage outgoing;
    u32 outgoingAck;
    struct SoulLinkMessage incoming;
    u32 incomingAck;
};

extern volatile struct SoulLinkMailbox gSoulLinkMailbox;
extern volatile u16 gSoulLinkLobbyState;

void SoulLink_Update(void);

#endif // GUARD_SOUL_LINK_H
