#ifndef GUARD_SOUL_LINK_H
#define GUARD_SOUL_LINK_H

#define SOUL_LINK_MAILBOX_MAGIC 0x4B4E4C53 // "SLNK" in little-endian memory
#define SOUL_LINK_PROTOCOL_VERSION 9
#define SOUL_LINK_PREVIOUS_PROTOCOL_VERSION 8
#define SOUL_LINK_SAVE_FORMAT_VERSION 3
#define SOUL_LINK_RANDOMIZER_SETTING_COUNT 15
#define SOUL_LINK_RANDOMIZER_SETTINGS_MASK 0x7FFF
#define SOUL_LINK_LOBBY_STATE_MASK 0x0007
#define SOUL_LINK_LOBBY_CONNECTED_SHIFT 4
#define SOUL_LINK_LOBBY_READY_SHIFT 8
#define SOUL_LINK_LOBBY_LOCAL_SHIFT 12
#define SOUL_LINK_LOBBY_PLAYER_MASK 0x000F
#define SOUL_LINK_INTENT_ACTIVE_MASK_SHIFT 8
#define SOUL_LINK_INTENT_STATUS_SHIFT 12
#define SOUL_LINK_GATE_STATE_MASK 0x000F
#define SOUL_LINK_GATE_PLAYER_MASK_SHIFT 4
#define SOUL_LINK_RUN_STATUS_ACTIVE (1 << 0)
#define SOUL_LINK_RUN_PLAYER_COUNT_SHIFT 1
#define SOUL_LINK_RUN_PLAYER_COUNT_MASK (7 << SOUL_LINK_RUN_PLAYER_COUNT_SHIFT)
#define SOUL_LINK_RUN_STATUS_MASK 0x0F
#define SOUL_LINK_GROUP_NONE 0
#define SOUL_LINK_STARTER_GROUP_ID 0xFFFF
#define SOUL_LINK_SNAPSHOT_FLAG_DEAD (1 << 0)
#define SOUL_LINK_REGISTRY_RESULT_VALID (1 << 8)
#define SOUL_LINK_REGISTRY_RESULT_DEAD (1 << 9)

struct BoxPokemon;
struct Pokemon;

enum SoulLinkEventType
{
    SOUL_LINK_EVENT_NONE,
    SOUL_LINK_EVENT_PING,
    SOUL_LINK_EVENT_LOBBY_STATE,
    SOUL_LINK_EVENT_LOBBY_INTENT,
    SOUL_LINK_EVENT_LOBBY_START,
    SOUL_LINK_EVENT_GATE_STATE,
    SOUL_LINK_EVENT_SETTINGS,
    SOUL_LINK_EVENT_CATCH,
    SOUL_LINK_EVENT_LINK_CREATED,
    SOUL_LINK_EVENT_SNAPSHOT_BEGIN,
    SOUL_LINK_EVENT_SNAPSHOT_MEMBER,
    SOUL_LINK_EVENT_SNAPSHOT_END,
    SOUL_LINK_EVENT_REGISTRY_REQUEST,
    SOUL_LINK_EVENT_REGISTRY_RESULT,
};

enum SoulLinkRegistryRequest
{
    SOUL_LINK_REGISTRY_REQUEST_COUNT = 1,
    SOUL_LINK_REGISTRY_REQUEST_MEMBER,
};

struct SoulLinkRegistryMember
{
    u16 groupId;
    u16 species;
    u16 location;
    bool8 dead;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
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
bool8 SoulLink_QueueCatch(u32 personality, u32 otId, u16 species, u16 location);
bool8 SoulLink_IsActive(void);
bool8 SoulLink_RequestRegistryCount(void);
bool8 SoulLink_TakeRegistryCount(u16 *count, bool8 *valid);
bool8 SoulLink_RequestRegistryMember(u16 row, u8 playerSlot);
bool8 SoulLink_TakeRegistryMember(struct SoulLinkRegistryMember *member,
                                  bool8 *valid);
u8 SoulLink_GetPlayerSlot(void);
void SoulLink_CancelRegistryRequest(void);
void SoulLink_LinkStarter(struct Pokemon *mon);
u16 SoulLink_GetBoxMonGroupId(struct BoxPokemon *boxMon);
void SoulLink_SetBoxMonGroupId(struct BoxPokemon *boxMon, u16 groupId);

#endif // GUARD_SOUL_LINK_H
