#ifndef GUARD_SOUL_LINK_H
#define GUARD_SOUL_LINK_H

#define SOUL_LINK_MAILBOX_MAGIC 0x4B4E4C53 // "SLNK" in little-endian memory
#define SOUL_LINK_PROTOCOL_VERSION 5
#define SOUL_LINK_SAVE_FORMAT_VERSION 2
#define SOUL_LINK_RANDOMIZER_SETTING_COUNT 15
#define SOUL_LINK_RANDOMIZER_SETTINGS_MASK 0x7FFF
#define SOUL_LINK_LOBBY_STATE_MASK 0x0007
#define SOUL_LINK_LOBBY_CONNECTED_SHIFT 4
#define SOUL_LINK_LOBBY_READY_SHIFT 8
#define SOUL_LINK_LOBBY_LOCAL_SHIFT 12
#define SOUL_LINK_LOBBY_PLAYER_MASK 0x000F
#define SOUL_LINK_INTENT_ACTIVE_MASK_SHIFT 8
#define SOUL_LINK_GATE_STATE_MASK 0x000F
#define SOUL_LINK_GATE_PLAYER_MASK_SHIFT 4

enum SoulLinkEventType
{
    SOUL_LINK_EVENT_NONE,
    SOUL_LINK_EVENT_PING,
    SOUL_LINK_EVENT_LOBBY_STATE,
    SOUL_LINK_EVENT_LOBBY_INTENT,
    SOUL_LINK_EVENT_LOBBY_START,
    SOUL_LINK_EVENT_GATE_STATE,
    SOUL_LINK_EVENT_SETTINGS,
};

enum SoulLinkLobbyIntent
{
    SOUL_LINK_INTENT_NONE,
    SOUL_LINK_INTENT_NEW_GAME,
    SOUL_LINK_INTENT_CONTINUE,
};

enum SoulLinkGateState
{
    SOUL_LINK_GATE_IDLE,
    SOUL_LINK_GATE_WAITING,
    SOUL_LINK_GATE_LOCKED,
    SOUL_LINK_GATE_APPROVED,
    SOUL_LINK_GATE_REJECTED,
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
extern volatile u8 gSoulLinkConnectedPlayerMask;
extern volatile u8 gSoulLinkReadyPlayerMask;
extern volatile u8 gSoulLinkLocalPlayerMask;
extern volatile u8 gSoulLinkGateState;
extern volatile u8 gSoulLinkLockedPlayerMask;
extern volatile struct SoulLinkSaveData gSoulLinkPendingRun;
extern u16 gSoulLinkPendingRandomizerSettings;

void SoulLink_Update(void);
void SoulLink_SetPendingRandomizerSettings(u16 settings);
bool8 SoulLink_SendLobbyIntent(u8 intent);
bool8 SoulLink_SendLobbyStart(void);
bool8 SoulLink_SendSettings(void);

#endif // GUARD_SOUL_LINK_H
