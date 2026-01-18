const u8 gEasyChatWord_Listen[] = _("Listen");
const u8 gEasyChatWord_NotVery[] = _("Not Very");
const u8 gEasyChatWord_Mean[] = _("Mean");
const u8 gEasyChatWord_Lie[] = _("Lie");
const u8 gEasyChatWord_Lay[] = _("Lay");
const u8 gEasyChatWord_Recommend[] = _("Recommend");
const u8 gEasyChatWord_Nitwit[] = _("Nitwit");
const u8 gEasyChatWord_Quite[] = _("Quite");
const u8 gEasyChatWord_From[] = _("From");
const u8 gEasyChatWord_Feeling[] = _("Feeling");
const u8 gEasyChatWord_But[] = _("But");
const u8 gEasyChatWord_However[] = _("However");
const u8 gEasyChatWord_Case[] = _("Case");
const u8 gEasyChatWord_The[] = _("The");
const u8 gEasyChatWord_Miss[] = _("Miss");
const u8 gEasyChatWord_How[] = _("How");
const u8 gEasyChatWord_Hit[] = _("Hit");
const u8 gEasyChatWord_Enough[] = _("Enough");
const u8 gEasyChatWord_ALot[] = _("A Lot");
const u8 gEasyChatWord_ALittle[] = _("A Little");
const u8 gEasyChatWord_Absolutely[] = _("Absolutely");
const u8 gEasyChatWord_And[] = _("And");
const u8 gEasyChatWord_Only[] = _("Only");
const u8 gEasyChatWord_Around[] = _("Around");
const u8 gEasyChatWord_Probably[] = _("Probably");
const u8 gEasyChatWord_If[] = _("If");
const u8 gEasyChatWord_Very[] = _("Very");
const u8 gEasyChatWord_ATinyBit[] = _("A Tiny Bit");
const u8 gEasyChatWord_Wild[] = _("Wild");
const u8 gEasyChatWord_Thats[] = _("That's");
const u8 gEasyChatWord_Just[] = _("Just");
const u8 gEasyChatWord_EvenSo[] = _("Even So,");
const u8 gEasyChatWord_MustBe[] = _("Must Be");
const u8 gEasyChatWord_Naturally[] = _("Naturally");
const u8 gEasyChatWord_ForNow[] = _("For Now,");
const u8 gEasyChatWord_Understood[] = _("Understood");
const u8 gEasyChatWord_Joking[] = _("Joking");
const u8 gEasyChatWord_Ready[] = _("Ready");
const u8 gEasyChatWord_Something[] = _("Something");
const u8 gEasyChatWord_Somehow[] = _("Somehow");
const u8 gEasyChatWord_Although[] = _("Although");
const u8 gEasyChatWord_Also[] = _("Also");
const u8 gEasyChatWord_Perfect[] = _("Perfect");
const u8 gEasyChatWord_AsMuchAs[] = _("As Much As");
const u8 gEasyChatWord_Really[] = _("Really");
const u8 gEasyChatWord_Truly[] = _("Truly");
const u8 gEasyChatWord_Seriously[] = _("Seriously");
const u8 gEasyChatWord_Totally[] = _("Totally");
const u8 gEasyChatWord_Until[] = _("Until");
const u8 gEasyChatWord_AsIf[] = _("As If");
const u8 gEasyChatWord_Mood[] = _("Mood");
const u8 gEasyChatWord_Rather[] = _("Rather");
const u8 gEasyChatWord_Awfully[] = _("Awfully");
const u8 gEasyChatWord_Mode[] = _("Mode");
const u8 gEasyChatWord_More[] = _("More");
const u8 gEasyChatWord_TooLate[] = _("Too Late");
const u8 gEasyChatWord_Finally[] = _("Finally");
const u8 gEasyChatWord_Any[] = _("Any");
const u8 gEasyChatWord_Instead[] = _("Instead");
const u8 gEasyChatWord_Fantastic[] = _("Fantastic");

const struct EasyChatWordInfo gEasyChatGroup_Speech[] = {
    [EC_INDEX(EC_WORD_LISTEN)] =
    {
        .text = gEasyChatWord_Listen,
        .alphabeticalOrder = 19,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NOT_VERY)] =
    {
        .text = gEasyChatWord_NotVery,
        .alphabeticalOrder = 18,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MEAN)] =
    {
        .text = gEasyChatWord_Mean,
        .alphabeticalOrder = 27,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LIE)] =
    {
        .text = gEasyChatWord_Lie,
        .alphabeticalOrder = 20,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LAY)] =
    {
        .text = gEasyChatWord_Lay,
        .alphabeticalOrder = 41,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RECOMMEND)] =
    {
        .text = gEasyChatWord_Recommend,
        .alphabeticalOrder = 40,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NITWIT)] =
    {
        .text = gEasyChatWord_Nitwit,
        .alphabeticalOrder = 21,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_QUITE)] =
    {
        .text = gEasyChatWord_Quite,
        .alphabeticalOrder = 57,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FROM)] =
    {
        .text = gEasyChatWord_From,
        .alphabeticalOrder = 23,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FEELING)] =
    {
        .text = gEasyChatWord_Feeling,
        .alphabeticalOrder = 49,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BUT)] =
    {
        .text = gEasyChatWord_But,
        .alphabeticalOrder = 43,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HOWEVER)] =
    {
        .text = gEasyChatWord_However,
        .alphabeticalOrder = 52,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CASE)] =
    {
        .text = gEasyChatWord_Case,
        .alphabeticalOrder = 10,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THE)] =
    {
        .text = gEasyChatWord_The,
        .alphabeticalOrder = 12,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MISS)] =
    {
        .text = gEasyChatWord_Miss,
        .alphabeticalOrder = 17,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HOW)] =
    {
        .text = gEasyChatWord_How,
        .alphabeticalOrder = 31,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HIT)] =
    {
        .text = gEasyChatWord_Hit,
        .alphabeticalOrder = 59,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ENOUGH)] =
    {
        .text = gEasyChatWord_Enough,
        .alphabeticalOrder = 9,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_A_LOT)] =
    {
        .text = gEasyChatWord_ALot,
        .alphabeticalOrder = 56,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_A_LITTLE)] =
    {
        .text = gEasyChatWord_ALittle,
        .alphabeticalOrder = 34,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ABSOLUTELY)] =
    {
        .text = gEasyChatWord_Absolutely,
        .alphabeticalOrder = 8,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AND)] =
    {
        .text = gEasyChatWord_And,
        .alphabeticalOrder = 16,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ONLY)] =
    {
        .text = gEasyChatWord_Only,
        .alphabeticalOrder = 15,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AROUND)] =
    {
        .text = gEasyChatWord_Around,
        .alphabeticalOrder = 11,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PROBABLY)] =
    {
        .text = gEasyChatWord_Probably,
        .alphabeticalOrder = 25,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_IF)] =
    {
        .text = gEasyChatWord_If,
        .alphabeticalOrder = 58,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_VERY)] =
    {
        .text = gEasyChatWord_Very,
        .alphabeticalOrder = 36,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_A_TINY_BIT)] =
    {
        .text = gEasyChatWord_ATinyBit,
        .alphabeticalOrder = 30,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WILD)] =
    {
        .text = gEasyChatWord_Wild,
        .alphabeticalOrder = 4,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THAT_S)] =
    {
        .text = gEasyChatWord_Thats,
        .alphabeticalOrder = 3,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_JUST)] =
    {
        .text = gEasyChatWord_Just,
        .alphabeticalOrder = 0,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EVEN_SO)] =
    {
        .text = gEasyChatWord_EvenSo,
        .alphabeticalOrder = 2,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MUST_BE)] =
    {
        .text = gEasyChatWord_MustBe,
        .alphabeticalOrder = 14,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NATURALLY)] =
    {
        .text = gEasyChatWord_Naturally,
        .alphabeticalOrder = 53,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FOR_NOW)] =
    {
        .text = gEasyChatWord_ForNow,
        .alphabeticalOrder = 50,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNDERSTOOD)] =
    {
        .text = gEasyChatWord_Understood,
        .alphabeticalOrder = 54,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_JOKING)] =
    {
        .text = gEasyChatWord_Joking,
        .alphabeticalOrder = 32,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_READY)] =
    {
        .text = gEasyChatWord_Ready,
        .alphabeticalOrder = 33,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SOMETHING)] =
    {
        .text = gEasyChatWord_Something,
        .alphabeticalOrder = 6,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SOMEHOW)] =
    {
        .text = gEasyChatWord_Somehow,
        .alphabeticalOrder = 1,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ALTHOUGH)] =
    {
        .text = gEasyChatWord_Although,
        .alphabeticalOrder = 22,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ALSO)] =
    {
        .text = gEasyChatWord_Also,
        .alphabeticalOrder = 42,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PERFECT)] =
    {
        .text = gEasyChatWord_Perfect,
        .alphabeticalOrder = 24,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AS_MUCH_AS)] =
    {
        .text = gEasyChatWord_AsMuchAs,
        .alphabeticalOrder = 7,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_REALLY)] =
    {
        .text = gEasyChatWord_Really,
        .alphabeticalOrder = 51,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TRULY)] =
    {
        .text = gEasyChatWord_Truly,
        .alphabeticalOrder = 37,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SERIOUSLY)] =
    {
        .text = gEasyChatWord_Seriously,
        .alphabeticalOrder = 44,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TOTALLY)] =
    {
        .text = gEasyChatWord_Totally,
        .alphabeticalOrder = 5,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNTIL)] =
    {
        .text = gEasyChatWord_Until,
        .alphabeticalOrder = 46,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AS_IF)] =
    {
        .text = gEasyChatWord_AsIf,
        .alphabeticalOrder = 39,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MOOD)] =
    {
        .text = gEasyChatWord_Mood,
        .alphabeticalOrder = 38,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RATHER)] =
    {
        .text = gEasyChatWord_Rather,
        .alphabeticalOrder = 29,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AWFULLY)] =
    {
        .text = gEasyChatWord_Awfully,
        .alphabeticalOrder = 13,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MODE)] =
    {
        .text = gEasyChatWord_Mode,
        .alphabeticalOrder = 55,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MORE)] =
    {
        .text = gEasyChatWord_More,
        .alphabeticalOrder = 47,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TOO_LATE)] =
    {
        .text = gEasyChatWord_TooLate,
        .alphabeticalOrder = 45,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FINALLY)] =
    {
        .text = gEasyChatWord_Finally,
        .alphabeticalOrder = 35,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ANY)] =
    {
        .text = gEasyChatWord_Any,
        .alphabeticalOrder = 48,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_INSTEAD)] =
    {
        .text = gEasyChatWord_Instead,
        .alphabeticalOrder = 26,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FANTASTIC)] =
    {
        .text = gEasyChatWord_Fantastic,
        .alphabeticalOrder = 28,
        .enabled = TRUE,
    },
};
