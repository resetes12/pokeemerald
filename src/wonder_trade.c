#include "global.h"
#include "pokemon.h"
#include "strings.h"
#include "random.h"
#include "text.h"
#include "event_data.h"
#include "region_map.h"
#include "constants/species.h"
#include "constants/items.h"
#include "constants/abilities.h"
#include "data/text/wonder_trade_OT_names.h"
#include "constants/region_map_sections.h"
#include "item.h"
#include "constants/item.h"
#include "constants/hold_effects.h"
#include "mail.h"
#include "constants/pokemon.h"
#ifdef POKEMON_EXPANSION
#include "party_menu.h"
#include "field_weather.h"
#include "constants/weather.h"
#endif

extern struct Evolution gEvolutionTable[][EVOS_PER_MON];

struct InGameTrade {
    /*0x00*/ u8 nickname[POKEMON_NAME_LENGTH + 1];
    /*0x0C*/ u16 species;
    /*0x0E*/ u8 ivs[NUM_STATS];
    /*0x0E*/ u8 ivs31[NUM_STATS];
    /*0x14*/ u8 abilityNum;
    /*0x18*/ u32 otId;
    /*0x1C*/ u8 conditions[CONTEST_CATEGORIES_COUNT];
    /*0x24*/ u32 personality;
    /*0x28*/ u16 heldItem;
    /*0x2A*/ u8 mailNum;
    /*0x2B*/ u8 otName[11];
    /*0x36*/ u8 otGender;
    /*0x37*/ u8 sheen;
    /*0x38*/ u16 requestedSpecies;
};

static const u16 sIsValidSpecies[] =
{
    SPECIES_BULBASAUR,
    SPECIES_CHARMANDER,
    SPECIES_SQUIRTLE,
    SPECIES_CATERPIE,
    SPECIES_WEEDLE,
    SPECIES_PIDGEY,
    SPECIES_RATTATA,
    SPECIES_SPEAROW,
    SPECIES_EKANS,
    SPECIES_SANDSHREW,
    SPECIES_NIDORAN_F,
    SPECIES_NIDORAN_M,
    SPECIES_VULPIX,
    SPECIES_ZUBAT,
    SPECIES_ODDISH,
    SPECIES_PARAS,
    SPECIES_VENONAT,
    SPECIES_DIGLETT,
    SPECIES_MEOWTH,
    SPECIES_PSYDUCK,
    SPECIES_MANKEY,
    SPECIES_GROWLITHE,
    SPECIES_POLIWAG,
    SPECIES_ABRA,
    SPECIES_MACHOP,
    SPECIES_BELLSPROUT,
    SPECIES_TENTACOOL,
    SPECIES_GEODUDE,
    SPECIES_PONYTA,
    SPECIES_SLOWPOKE,
    SPECIES_MAGNEMITE,
    SPECIES_FARFETCHD,
    SPECIES_DODUO,
    SPECIES_SEEL,
    SPECIES_GRIMER,
    SPECIES_SHELLDER,
    SPECIES_GASTLY,
    SPECIES_ONIX,
    SPECIES_DROWZEE,
    SPECIES_KRABBY,
    SPECIES_VOLTORB,
    SPECIES_EXEGGCUTE,
    SPECIES_CUBONE,
    SPECIES_LICKITUNG,
    SPECIES_KOFFING,
    SPECIES_RHYHORN,
    SPECIES_TANGELA,
    SPECIES_KANGASKHAN,
    SPECIES_HORSEA,
    SPECIES_GOLDEEN,
    SPECIES_STARYU,
    SPECIES_SCYTHER,
    SPECIES_PINSIR,
    SPECIES_TAUROS,
    SPECIES_MAGIKARP,
    SPECIES_LAPRAS,
    SPECIES_DITTO,
    SPECIES_EEVEE,
    SPECIES_PORYGON,
    SPECIES_OMANYTE,
    SPECIES_KABUTO,
    SPECIES_DRATINI,
    SPECIES_CHIKORITA,
    SPECIES_CYNDAQUIL,
    SPECIES_TOTODILE,
    SPECIES_SENTRET,
    SPECIES_HOOTHOOT,
    SPECIES_LEDYBA,
    SPECIES_SPINARAK,
    SPECIES_CHINCHOU,
    SPECIES_PICHU,
    SPECIES_CLEFFA,
    SPECIES_IGGLYBUFF,
    SPECIES_TOGEPI,
    SPECIES_NATU,
    SPECIES_MAREEP,
    SPECIES_HOPPIP,
    SPECIES_AIPOM,
    SPECIES_SUNKERN,
    SPECIES_YANMA,
    SPECIES_WOOPER,
    SPECIES_MURKROW,
    SPECIES_MISDREAVUS,
    SPECIES_GIRAFARIG,
    SPECIES_PINECO,
    SPECIES_DUNSPARCE,
    SPECIES_GLIGAR,
    SPECIES_SNUBBULL,
    SPECIES_QWILFISH,
    SPECIES_SHUCKLE,
    SPECIES_HERACROSS,
    SPECIES_SNEASEL,
    SPECIES_TEDDIURSA,
    SPECIES_SLUGMA,
    SPECIES_SWINUB,
    SPECIES_CORSOLA,
    SPECIES_REMORAID,
    SPECIES_DELIBIRD,
    SPECIES_SKARMORY,
    SPECIES_HOUNDOUR,
    SPECIES_PHANPY,
    SPECIES_STANTLER,
    SPECIES_SMEARGLE,
    SPECIES_TYROGUE,
    SPECIES_SMOOCHUM,
    SPECIES_ELEKID,
    SPECIES_MAGBY,
    SPECIES_MILTANK,
    SPECIES_LARVITAR,
    SPECIES_TREECKO,
    SPECIES_TORCHIC,
    SPECIES_MUDKIP,
    SPECIES_POOCHYENA,
    SPECIES_ZIGZAGOON,
    SPECIES_WURMPLE,
    SPECIES_LOTAD,
    SPECIES_SEEDOT,
    SPECIES_NINCADA,
    SPECIES_TAILLOW,
    SPECIES_SHROOMISH,
    SPECIES_SPINDA,
    SPECIES_WINGULL,
    SPECIES_SURSKIT,
    SPECIES_WAILMER,
    SPECIES_SKITTY,
    SPECIES_KECLEON,
    SPECIES_BALTOY,
    SPECIES_NOSEPASS,
    SPECIES_TORKOAL,
    SPECIES_SABLEYE,
    SPECIES_BARBOACH,
    SPECIES_LUVDISC,
    SPECIES_CORPHISH,
    SPECIES_FEEBAS,
    SPECIES_CARVANHA,
    SPECIES_TRAPINCH,
    SPECIES_MAKUHITA,
    SPECIES_ELECTRIKE,
    SPECIES_NUMEL,
    SPECIES_SPHEAL,
    SPECIES_CACNEA,
    SPECIES_SNORUNT,
    SPECIES_LUNATONE,
    SPECIES_SOLROCK,
    SPECIES_AZURILL,
    SPECIES_SPOINK,
    SPECIES_PLUSLE,
    SPECIES_MINUN,
    SPECIES_MAWILE,
    SPECIES_MEDITITE,
    SPECIES_SWABLU,
    SPECIES_WYNAUT,
    SPECIES_DUSKULL,
    SPECIES_SLAKOTH,
    SPECIES_GULPIN,
    SPECIES_TROPIUS,
    SPECIES_WHISMUR,
    SPECIES_CLAMPERL,
    SPECIES_ABSOL,
    SPECIES_SHUPPET,
    SPECIES_SEVIPER,
    SPECIES_ZANGOOSE,
    SPECIES_ARON,
    SPECIES_CASTFORM,
    SPECIES_VOLBEAT,
    SPECIES_ILLUMISE,
    SPECIES_LILEEP,
    SPECIES_ANORITH,
    SPECIES_RALTS,
    SPECIES_BAGON,
    SPECIES_BELDUM,
    SPECIES_BONSLY,
    SPECIES_BUDEW,
    SPECIES_CHINGLING,
    SPECIES_HAPPINY,
    SPECIES_MANTYKE,
    SPECIES_MIME_JR,
    SPECIES_MUNCHLAX,
};

static u16 PickRandomSpecies(void);
void CreateWonderTradePokemon(u8 whichPlayerMon);
u16 determineEvolution(struct Pokemon *);
u8 getWonderTradeOT(u8 *name);
u16 GetValidWonderTradeItem(u16 item);

static u16 PickRandomSpecies() // picks only base forms
{
    u16 species = sIsValidSpecies[Random() % NELEMS(sIsValidSpecies)];
    return species;
}

u8 getWonderTradeOT(u8 *name)
{
    u8 randGender = (Random() % 2); // 0 for male, 1 for female
    u8 numOTNames = 250;
    u8 selectedName = Random() %numOTNames;
    u8 i;
    if (randGender == MALE) // male OT selected
    {
        randGender = 0;
        for (i = 0; i < 8; ++i)
        {
            name[i] = maleWTNames[selectedName][i];
        }
        name[8] = EOS;
    }
    else                    // female OT selected
    {
        randGender = 0xFF;
        for (i = 0; i < 8; ++i)
        {
            name[i] = femaleWTNames[selectedName][i];
        }
        name[8] = EOS;
    }
    return randGender;
}

void CreateWonderTradePokemon(u8 whichPlayerMon)
{
    struct InGameTrade *inGameTrade;
    struct Pokemon *pokemon = &gEnemyParty[0];
    u16 species = PickRandomSpecies();
    u8 chanceToEvolve = Random() % 255;
    u8 name[POKEMON_NAME_LENGTH + 1];
    u8 nameOT[8];
    u8 genderOT;
    u8 level = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_LEVEL); // gets level of player's selected Pokemon
    u16 playerSpecies = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES); // gets species of player's selected Pokemon
    u32 OTID = ((Random() << 16) | Random());
    u32 personality = ((Random() << 16) | Random());
    u16 heldItem = 0;
    u16 currHeldItem = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HELD_ITEM);
    u8 metLocation = METLOC_IN_GAME_TRADE;
    u8 i;
    struct InGameTrade gIngameTrades[] = {
        {
            _(""), species,
            (Random() % 32), (Random() % 32), (Random() % 32), (Random() % 32), (Random() % 32), (Random() % 32),
            31, 31, 31, 31, 31, 31,
            (Random() % 2), OTID,
            0, 0, 0, 0, 0,
            personality,
            heldItem, -1,
            _("ERROR"), FEMALE, 0,
            playerSpecies
        }
    };

    genderOT = getWonderTradeOT(nameOT);
    inGameTrade = &gIngameTrades[0];
    CreateMon(pokemon, species, level, 0, FALSE, 0, TRUE, (Random() << 16) | Random());

    if (chanceToEvolve >= 200) // evolves to highest stage
    {
        species = determineEvolution(pokemon);
        CreateMon(pokemon, species, level, 0, FALSE, 0, TRUE, (Random() << 16) | Random());
        species = determineEvolution(pokemon);
        CreateMon(pokemon, species, level, 0, FALSE, 0, TRUE, (Random() << 16) | Random());
    }
    else if (chanceToEvolve >= 100 && chanceToEvolve < 200) // evolves once
    {
        species = determineEvolution(pokemon);
        CreateMon(pokemon, species, level, 0, FALSE, 0, TRUE, (Random() << 16) | Random());
    }

    GetSpeciesName(name, species);

    // 10% chance of having the generated Wonder Traded 'mon carry an item.
    if ((Random() % 99) < 15)
        heldItem = GetValidWonderTradeItem(heldItem);

    if (currHeldItem == ITEM_NONE)
    {
        for (i = 0; i < EVOS_PER_MON; i++)
        {
            if (gEvolutionTable[species][i].method == EVO_TRADE)
            {
                // 30% chance for the in coming Pokémon to hold an Everstone if they evolve by trading
                if (Random() % 255 <= 77)
                {
                    heldItem = ITEM_EVERSTONE;
                    SetMonData(pokemon, MON_DATA_HELD_ITEM, &heldItem);
                }
            }
            else if (gEvolutionTable[species][i].method == EVO_TRADE_ITEM)
            {
                // 30% chance for the in coming Pokémon to hold the item they need to evolve if they need one
                if (Random() % 255 <= 77)
                {
                    heldItem = gEvolutionTable[species][i].param;
                    SetMonData(pokemon, MON_DATA_HELD_ITEM, &heldItem);
                }
            }
        }
    }

    SetMonData(pokemon, MON_DATA_HELD_ITEM, &heldItem);
    if (gSaveBlock1Ptr->MaxPartyIVs == 0)
    {
        SetMonData(pokemon, MON_DATA_HP_IV, &inGameTrade->ivs[0]);
        SetMonData(pokemon, MON_DATA_ATK_IV, &inGameTrade->ivs[1]);
        SetMonData(pokemon, MON_DATA_DEF_IV, &inGameTrade->ivs[2]);
        SetMonData(pokemon, MON_DATA_SPEED_IV, &inGameTrade->ivs[3]);
        SetMonData(pokemon, MON_DATA_SPATK_IV, &inGameTrade->ivs[4]);
        SetMonData(pokemon, MON_DATA_SPDEF_IV, &inGameTrade->ivs[5]);
    }
    else if (gSaveBlock1Ptr->MaxPartyIVs == 1)
    {
        SetMonData(pokemon, MON_DATA_HP_IV, &inGameTrade->ivs31[0]);
        SetMonData(pokemon, MON_DATA_ATK_IV, &inGameTrade->ivs31[1]);
        SetMonData(pokemon, MON_DATA_DEF_IV, &inGameTrade->ivs31[2]);
        SetMonData(pokemon, MON_DATA_SPEED_IV, &inGameTrade->ivs31[3]);
        SetMonData(pokemon, MON_DATA_SPATK_IV, &inGameTrade->ivs31[4]);
        SetMonData(pokemon, MON_DATA_SPDEF_IV, &inGameTrade->ivs31[5]);
    }
    SetMonData(pokemon, MON_DATA_NICKNAME, name);
    SetMonData(pokemon, MON_DATA_OT_NAME, nameOT);
    SetMonData(pokemon, MON_DATA_BEAUTY, &inGameTrade->conditions[1]);
    SetMonData(pokemon, MON_DATA_CUTE, &inGameTrade->conditions[2]);
    SetMonData(pokemon, MON_DATA_COOL, &inGameTrade->conditions[0]);
    SetMonData(pokemon, MON_DATA_SMART, &inGameTrade->conditions[3]);
    SetMonData(pokemon, MON_DATA_TOUGH, &inGameTrade->conditions[4]);
    SetMonData(pokemon, MON_DATA_SHEEN, &inGameTrade->sheen);
    SetMonData(pokemon, MON_DATA_MET_LOCATION, &metLocation);
    SetMonData(pokemon, MON_DATA_OT_GENDER, &genderOT);
    CalculateMonStats(&gEnemyParty[0]);
}

u16 determineEvolution(struct Pokemon *mon)
{
    int i;
    u16 targetSpecies = 0;
    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, 0);
    u16 upperPersonality = personality >> 16;
    u8 level = GetMonData(mon, MON_DATA_LEVEL, 0);
    u16 eeveelution = Random() % 5;

    if (species == SPECIES_EEVEE || species == SPECIES_NINCADA)
    {
        if (species == SPECIES_NINCADA && level >= 20)
        {
            if ((Random() % 2) == 0)
                targetSpecies = SPECIES_NINJASK;
            else
                targetSpecies = SPECIES_SHEDINJA;
            return targetSpecies;
        }
        else if (species == SPECIES_EEVEE && level >= 25)
        {
            if (eeveelution == 0)
                targetSpecies = SPECIES_VAPOREON;
            else if (eeveelution == 1)
                targetSpecies = SPECIES_JOLTEON;
            else if (eeveelution == 2)
                targetSpecies = SPECIES_FLAREON;
            else if (eeveelution == 3)
                targetSpecies = SPECIES_ESPEON;
            else if (eeveelution == 4)
                targetSpecies = SPECIES_UMBREON;
            /*else if (eeveelution == 5)
                targetSpecies = SPECIES_LEAFEON;
            else if (eeveelution == 6)
                targetSpecies = SPECIES_GLACEON;
            else if (eeveelution == 7)
                targetSpecies = SPECIES_SYLVEON;*/
            return targetSpecies;
        }
    }

    for (i = 0; i < 5; i++)
    {
        switch (gEvolutionTable[species][i].method)
        {
        case EVO_FRIENDSHIP:
            if ((species == SPECIES_PICHU || species == SPECIES_CLEFFA || species == SPECIES_IGGLYBUFF
              || species == SPECIES_TOGEPI || species == SPECIES_AZURILL) && level >= 16)
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            else if ((species == SPECIES_GOLBAT || species == SPECIES_CHANSEY) && level >= 35)
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            break;
        case EVO_LEVEL:
            if (species == SPECIES_SLOWPOKE && level >= 37)
            {
                if ((Random() % 2) == 0)
                    targetSpecies = SPECIES_SLOWBRO;
                else
                    targetSpecies = SPECIES_SLOWKING;
            }
            else if (gEvolutionTable[species][i].param <= level)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            break;
        case EVO_LEVEL_ATK_GT_DEF:
            if (gEvolutionTable[species][i].param <= level)
            {
                if (GetMonData(mon, MON_DATA_ATK, 0) > GetMonData(mon, MON_DATA_DEF, 0))
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            break;
        case EVO_LEVEL_ATK_EQ_DEF:
            if (gEvolutionTable[species][i].param <= level)
            {
                if (GetMonData(mon, MON_DATA_ATK, 0) == GetMonData(mon, MON_DATA_DEF, 0))
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            break;
        case EVO_LEVEL_ATK_LT_DEF:
            if (gEvolutionTable[species][i].param <= level)
            {
                if (GetMonData(mon, MON_DATA_ATK, 0) < GetMonData(mon, MON_DATA_DEF, 0))
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            break;
        case EVO_LEVEL_SILCOON:
            if (gEvolutionTable[species][i].param <= level && (upperPersonality % 10) <= 4)
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            break;
        case EVO_LEVEL_CASCOON:
            if (gEvolutionTable[species][i].param <= level && (upperPersonality % 10) > 4)
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            break;
        case EVO_BEAUTY:
            if (level >= 30)
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            break;
        case EVO_ITEM:
            if (species == SPECIES_GLOOM && level >= 36)
            {
                if ((Random() % 2) == 0)
                    targetSpecies = SPECIES_VILEPLUME;
                else
                    targetSpecies = SPECIES_BELLOSSOM;
            }
            else if (species == SPECIES_WEEPINBELL && level >= 36)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if ((species == SPECIES_VULPIX || species == SPECIES_GROWLITHE) && level >= 32)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if ((species == SPECIES_SHELLDER || species == SPECIES_STARYU) && level >= 43)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if ((species == SPECIES_NIDORINA || species == SPECIES_NIDORINO || species == SPECIES_EXEGGCUTE) && level >= 26)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if ((species == SPECIES_JIGGLYPUFF || species == SPECIES_CLEFAIRY || species == SPECIES_SKITTY) && level >= 38)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if ((species == SPECIES_LOMBRE || species == SPECIES_NUZLEAF) && level >= 38)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if (species == SPECIES_POLIWHIRL && level >= 44)
            {
                if ((Random() % 2) == 0)
                    targetSpecies = SPECIES_POLIWRATH;
                else
                    targetSpecies = SPECIES_POLITOED;
            }
            else if (species == SPECIES_PIKACHU && level >= 27)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if (species == SPECIES_SUNKERN && level >= 15)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            break;
        case EVO_TRADE_ITEM:
            if ((species == SPECIES_ONIX || species == SPECIES_SEADRA) && level >= 40)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if (species == SPECIES_SCYTHER && level >= 26)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if (species == SPECIES_PORYGON && level >= 21)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
            }
            else if (species == SPECIES_CLAMPERL && level >= 22)
            {
                if ((Random() % 2) == 0)
                    targetSpecies = SPECIES_HUNTAIL;
                else
                    targetSpecies = SPECIES_GOREBYSS;
            }
            break;
        }
    }

    if (targetSpecies == 0)
        return species;
    else
        return targetSpecies;
}

u16 GetValidWonderTradeItem(u16 item)
{
    int i;
    u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES);

    ROLL:
        item = Random() % ITEMS_COUNT;

    if (item == ITEM_NONE || item == ITEM_ENIGMA_BERRY)
        goto ROLL;
    else if (IS_ITEM_MAIL(item))
        goto ROLL;
    else if (ItemId_GetPocket(item) == POCKET_KEY_ITEMS)
        goto ROLL;
    else if (item >= ITEM_HM01 && item <= ITEM_HM08)
        goto ROLL;
    else if (item == ITEM_THICK_CLUB && (species != SPECIES_CUBONE || species != SPECIES_MAROWAK))
        goto ROLL;
    else if (item == ITEM_LIGHT_BALL && species != SPECIES_PIKACHU)
        goto ROLL;
    else if ((item == ITEM_DEEP_SEA_SCALE || item == ITEM_DEEP_SEA_TOOTH) && species != SPECIES_CLAMPERL)
        goto ROLL;
    else if (item == ITEM_METAL_POWDER && species != SPECIES_DITTO)
        goto ROLL;

    return item;
}
