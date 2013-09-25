#pragma once

namespace Skim{ namespace Weapons{

namespace TF2WeaponID{
enum TF2WeaponID_t{
    BAT = 0,
    BOTTLE = 1,
    FIREAXE = 2,
    CLUB = 3,
    KNIFE = 4,
    FISTS = 5,
    SHOVEL = 6,
    WRENCH = 7,
    BONESAW = 8,
    SHOTGUN_PRIMARY = 9,
    SHOTGUN_SOLDIER = 10,
    SHOTGUN_HWG = 11,
    SHOTGUN_PYRO = 12,
    SCATTERGUN = 13,
    SNIPERRIFLE = 14,
    MINIGUN = 15,
    SMG = 16,
    SYRINGEGUN_MEDIC = 17,
    ROCKETLAUNCHER = 18,
    GRENADELAUNCHER = 19,
    PIPEBOMBLAUNCHER = 20,
    FLAMETHROWER = 21,
    PISTOL = 22,
    PISTOL_SCOUT = 23,
    REVOLVER = 24,
    PDA_ENGINEER_BUILD = 25,
    PDA_ENGINEER_DESTROY = 26,
    PDA_SPY = 27,
    BUILDER = 28,
    MEDIGUN = 29,
    INVIS = 30,
    THE_KRITZKRIEG = 35,
    THE_BLUTSAUGER = 36,
    THE_UBERSAW = 37,
    THE_AXTINGUISHER = 38,
    THE_FLARE_GUN = 39,
    THE_BACKBURNER = 40,
    NATASCHA = 41,
    THE_SANDVICH = 42,
    THE_KILLING_GLOVES_OF_BOXING = 43,
    THE_SANDMAN = 44,
    THE_FORCE_A_NATURE = 45,
    BONK_ATOMIC_PUNCH = 46,
    THE_HUNTSMAN = 56,
    THE_RAZORBACK = 57,
    JARATE = 58,
    THE_DEAD_RINGER = 59,
    THE_CLOAK_AND_DAGGER = 60,
    THE_AMBASSADOR = 61,
    THE_DIRECT_HIT = 127,
    THE_EQUALIZER = 128,
    THE_BUFF_BANNER = 129,
    THE_SCOTTISH_RESISTANCE = 130,
    THE_CHARGIN_TARGE = 131,
    THE_EYELANDER = 132,
    THE_GUNBOATS = 133,
    THE_WRANGLER = 140,
    THE_FRONTIER_JUSTICE = 141,
    THE_GUNSLINGER = 142,
    THE_HOMEWRECKER = 153,
    THE_PAIN_TRAIN = 154,
    THE_SOUTHERN_HOSPITALITY = 155,
    THE_DALOKOHS_BAR = 159,
    CRIT_A_COLA = 163,
    THE_TRIBALMANS_SHIV = 171,
    THE_SCOTSMANS_SKULLCUTTER = 172,
    THE_VITA_SAW = 173,
    THE_POWERJACK = 214,
    THE_DEGREASER = 215,
    THE_SHORTSTOP = 220,
    THE_HOLY_MACKEREL = 221,
    MAD_MILK = 222,
    LETRANGER = 224,
    YOUR_ETERNAL_REWARD = 225,
    THE_BATTALIONS_BACKUP = 226,
    THE_BLACK_BOX = 228,
    THE_SYDNEY_SLEEPER = 230,
    DARWINS_DANGER_SHIELD = 231,
    THE_BUSHWACKA = 232,
    ROCKET_JUMPER = 237,
    GLOVES_OF_RUNNING_URGENTLY = 239,
    STICKYBOMB_JUMPER = 265,
    THE_HORSELESS_HEADLESS_HORSEMANS_HEADTAKER = 266,
    THE_AMPUTATOR = 304,
    THE_CRUSADERS_CROSSBOW = 305,
    THE_ULLAPOOL_CABER = 307,
    THE_LOCH_N_LOAD = 308,
    WARRIORS_SPIRIT = 310,
    THE_BUFFALO_STEAK_SANDVICH = 311,
    THE_BRASS_BEAST = 312,
    THE_CANDY_CANE = 317,
    THE_BOSTON_BASHER = 325,
    THE_BACK_SCRATCHER = 326,
    THE_CLAIDHEAMOHMOR = 327,
    THE_JAG = 329,
    FISTS_OF_STEEL = 331,
    THE_DETONATOR = 351,
    THE_CONCHEROR = 354,
    THE_FAN_OWAR = 355,
    CONNIVERS_KUNAI = 356,
    THE_HALF_ZATOICHI = 357,
    THE_SHAHANSHAH = 401,
    THE_BAZAAR_BARGAIN = 402,
    THE_PERSIAN_PERSUADER = 404,
    ALI_BABAS_WEE_BOOTIES = 405,
    THE_SPLENDID_SCREEN = 406,
    THE_QUICK_FIX = 411,
    THE_OVERDOSE = 412,
    THE_SOLEMN_VOW = 413,
    THE_LIBERTY_LAUNCHER = 414,
    THE_RESERVE_SHOOTER = 415,
    THE_MARKET_GARDENER = 416,
    TOMISLAV = 424,
    THE_FAMILY_BUSINESS = 425,
    THE_EVICTION_NOTICE = 426,
    THE_COW_MANGLER_5000 = 441,
    THE_RIGHTEOUS_BISON = 442,
    THE_MANTREADS = 444,
    THE_DISCIPLINARY_ACTION = 447,
    THE_SODA_POPPER = 448,
    THE_WINGER = 449,
    THE_ATOMIZER = 450,
    THREE_RUNE_BLADE = 452,
    THE_POSTAL_PUMMELER = 457,
    THE_ENFORCER = 460,
    THE_BIG_EARNER = 461,
    THE_MAUL = 466,
    NESSIES_NINE_IRON = 482,
    THE_ORIGINAL = 513,
    THE_DIAMONDBACK = 525,
    THE_MACHINA = 526,
    THE_WIDOWMAKER = 527,
    THE_SHORT_CIRCUIT = 528,
    UNARMED_COMBAT = 572,
    THE_WANGA_PRICK = 574,
    APOCO_FISTS = 587,
    THE_POMSON_6000 = 588,
    THE_EUREKA_EFFECT = 589,
    THE_THIRD_DEGREE = 593,
    THE_PHLOGISTINATOR = 594,
    THE_MANMELTER = 595,
    THE_BOOTLEGGER = 608,
    THE_SCOTTISH_HANDSHAKE = 609,
    THE_SHARP_DRESSER = 638,
    THE_WRAP_ASSASSIN = 648,
    THE_SPY_CICLE = 649,
    THE_HOLIDAY_PUNCH = 656,
    BUILDER_SPY = 735,
    UPGRADEABLE_BUILDER_SPY = 736,
    UPGRADEABLE_PDA_ENGINEER_BUILD = 737,
};
};

typedef enum TF2WeaponID::TF2WeaponID_t TF2WeaponID_t ;
const char* WeaponName(int index);
};
};

