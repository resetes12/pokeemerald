const u8 gEasyChatWord_Highs[] = _("Highs");
const u8 gEasyChatWord_Lows[] = _("Lows");
const u8 gEasyChatWord_Um[] = _("Um");
const u8 gEasyChatWord_Rear[] = _("Rear");
const u8 gEasyChatWord_Things[] = _("Things");
const u8 gEasyChatWord_Thing[] = _("Thing");
const u8 gEasyChatWord_Below[] = _("Below");
const u8 gEasyChatWord_Above[] = _("Above");
const u8 gEasyChatWord_Back[] = _("Back");
const u8 gEasyChatWord_High[] = _("High");
const u8 gEasyChatWord_Here[] = _("Here");
const u8 gEasyChatWord_Inside[] = _("Inside");
const u8 gEasyChatWord_Outside[] = _("Outside");
const u8 gEasyChatWord_Beside[] = _("Beside");
const u8 gEasyChatWord_ThisIsItExcl[] = _("This Is It!");
const u8 gEasyChatWord_This[] = _("This");
const u8 gEasyChatWord_Every[] = _("Every");
const u8 gEasyChatWord_These[] = _("These");
const u8 gEasyChatWord_TheseWere[] = _("These Were");
const u8 gEasyChatWord_Down[] = _("Down");
const u8 gEasyChatWord_That[] = _("That");
const u8 gEasyChatWord_ThoseAre[] = _("Those Are");
const u8 gEasyChatWord_ThoseWere[] = _("Those Were");
const u8 gEasyChatWord_ThatsItExcl[] = _("That's It!");
const u8 gEasyChatWord_Am[] = _("Am");
const u8 gEasyChatWord_ThatWas[] = _("That Was");
const u8 gEasyChatWord_Front[] = _("Front");
const u8 gEasyChatWord_Up[] = _("Up");
const u8 gEasyChatWord_Choice[] = _("Choice");
const u8 gEasyChatWord_Far[] = _("Far");
const u8 gEasyChatWord_Away[] = _("Away");
const u8 gEasyChatWord_Near[] = _("Near");
const u8 gEasyChatWord_Where[] = _("Where");
const u8 gEasyChatWord_When[] = _("When");
const u8 gEasyChatWord_What[] = _("What");
const u8 gEasyChatWord_Deep[] = _("Deep");
const u8 gEasyChatWord_Shallow[] = _("Shallow");
const u8 gEasyChatWord_Why[] = _("Why");
const u8 gEasyChatWord_Confused[] = _("Confused");
const u8 gEasyChatWord_Opposite[] = _("Opposite");
const u8 gEasyChatWord_Left[] = _("Left");
const u8 gEasyChatWord_Right[] = _("Right");

const struct EasyChatWordInfo gEasyChatGroup_Misc[] = {
    [EC_INDEX(EC_WORD_HIGHS)] =
    {
        .text = gEasyChatWord_Highs,
        .alphabeticalOrder = 7,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LOWS)] =
    {
        .text = gEasyChatWord_Lows,
        .alphabeticalOrder = 24,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UM)] =
    {
        .text = gEasyChatWord_Um,
        .alphabeticalOrder = 30,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_REAR)] =
    {
        .text = gEasyChatWord_Rear,
        .alphabeticalOrder = 8,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THINGS)] =
    {
        .text = gEasyChatWord_Things,
        .alphabeticalOrder = 6,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THING)] =
    {
        .text = gEasyChatWord_Thing,
        .alphabeticalOrder = 13,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BELOW)] =
    {
        .text = gEasyChatWord_Below,
        .alphabeticalOrder = 28,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ABOVE)] =
    {
        .text = gEasyChatWord_Above,
        .alphabeticalOrder = 38,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BACK)] =
    {
        .text = gEasyChatWord_Back,
        .alphabeticalOrder = 35,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HIGH)] =
    {
        .text = gEasyChatWord_High,
        .alphabeticalOrder = 19,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HERE)] =
    {
        .text = gEasyChatWord_Here,
        .alphabeticalOrder = 16,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_INSIDE)] =
    {
        .text = gEasyChatWord_Inside,
        .alphabeticalOrder = 29,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_OUTSIDE)] =
    {
        .text = gEasyChatWord_Outside,
        .alphabeticalOrder = 26,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BESIDE)] =
    {
        .text = gEasyChatWord_Beside,
        .alphabeticalOrder = 10,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THIS_IS_IT_EXCL)] =
    {
        .text = gEasyChatWord_ThisIsItExcl,
        .alphabeticalOrder = 9,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THIS)] =
    {
        .text = gEasyChatWord_This,
        .alphabeticalOrder = 0,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EVERY)] =
    {
        .text = gEasyChatWord_Every,
        .alphabeticalOrder = 11,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THESE)] =
    {
        .text = gEasyChatWord_These,
        .alphabeticalOrder = 40,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THESE_WERE)] =
    {
        .text = gEasyChatWord_TheseWere,
        .alphabeticalOrder = 1,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DOWN)] =
    {
        .text = gEasyChatWord_Down,
        .alphabeticalOrder = 31,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THAT)] =
    {
        .text = gEasyChatWord_That,
        .alphabeticalOrder = 39,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THOSE_ARE)] =
    {
        .text = gEasyChatWord_ThoseAre,
        .alphabeticalOrder = 12,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THOSE_WERE)] =
    {
        .text = gEasyChatWord_ThoseWere,
        .alphabeticalOrder = 3,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THAT_S_IT_EXCL)] =
    {
        .text = gEasyChatWord_ThatsItExcl,
        .alphabeticalOrder = 41,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AM)] =
    {
        .text = gEasyChatWord_Am,
        .alphabeticalOrder = 36,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THAT_WAS)] =
    {
        .text = gEasyChatWord_ThatWas,
        .alphabeticalOrder = 20,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FRONT)] =
    {
        .text = gEasyChatWord_Front,
        .alphabeticalOrder = 25,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UP)] =
    {
        .text = gEasyChatWord_Up,
        .alphabeticalOrder = 23,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CHOICE)] =
    {
        .text = gEasyChatWord_Choice,
        .alphabeticalOrder = 17,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FAR)] =
    {
        .text = gEasyChatWord_Far,
        .alphabeticalOrder = 18,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AWAY)] =
    {
        .text = gEasyChatWord_Away,
        .alphabeticalOrder = 5,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NEAR)] =
    {
        .text = gEasyChatWord_Near,
        .alphabeticalOrder = 4,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WHERE)] =
    {
        .text = gEasyChatWord_Where,
        .alphabeticalOrder = 15,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WHEN)] =
    {
        .text = gEasyChatWord_When,
        .alphabeticalOrder = 14,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WHAT)] =
    {
        .text = gEasyChatWord_What,
        .alphabeticalOrder = 21,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DEEP)] =
    {
        .text = gEasyChatWord_Deep,
        .alphabeticalOrder = 22,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SHALLOW)] =
    {
        .text = gEasyChatWord_Shallow,
        .alphabeticalOrder = 2,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WHY)] =
    {
        .text = gEasyChatWord_Why,
        .alphabeticalOrder = 27,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CONFUSED)] =
    {
        .text = gEasyChatWord_Confused,
        .alphabeticalOrder = 34,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_OPPOSITE)] =
    {
        .text = gEasyChatWord_Opposite,
        .alphabeticalOrder = 33,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LEFT)] =
    {
        .text = gEasyChatWord_Left,
        .alphabeticalOrder = 32,
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RIGHT)] =
    {
        .text = gEasyChatWord_Right,
        .alphabeticalOrder = 37,
        .enabled = TRUE,
    },
};
