// application I made for this is in winject folder on desktop
#include "TF2WeaponID.h"
const char* Skim::Weapons::WeaponName(int index)
{
    switch(index)
    {
    case 0:
        return "Bat";
    case 1:
        return "Scrumpy";
    case 2:
        return "Fireaxe";
    case 3:
        return "Club";
    case 4:
        return "Knife";
    case 5:
        return "Fists";
    case 6:
        return "Shovel";
    case 7:
        return "Wrench";
    case 8:
        return "Bonesaw";
    case 9:
        return "Shotgun Primary";
    case 10:
        return "Shotgun Soldier";
    case 11:
        return "Shotgun Hwg";
    case 12:
        return "Shotgun Pyro";
    case 13:
        return "Scattergun";
    case 14:
        return "Sniperrifle";
    case 15:
        return "Minigun";
    case 16:
        return "Smg";
    case 17:
        return "Syringegun Medic";
    case 18:
        return "Rocketlauncher";
    case 19:
        return "Grenadelauncher";
    case 20:
        return "Pipebomblauncher";
    case 21:
        return "Flamethrower";
    case 22:
        return "Pistol";
    case 23:
        return "Pistol Scout";
    case 24:
        return "Revolver";
    case 25:
        return "Pda Engineer Build";
    case 26:
        return "Pda Engineer Destroy";
    case 27:
        return "Pda Spy";
    case 28:
        return "Builder";
    case 29:
        return "Medigun";
    case 30:
        return "Invis";
    case 35:
        return "The Kritzkrieg";
    case 36:
        return "The Blutsauger";
    case 37:
        return "The Ubersaw";
    case 38:
        return "The Axtinguisher";
    case 39:
        return "The Flare Gun";
    case 40:
        return "The Backburner";
    case 41:
        return "Natascha";
    case 42:
        return "The Sandvich";
    case 43:
        return "The Killing Gloves Of Boxing";
    case 44:
        return "The Sandman";
    case 45:
        return "The Force-a-nature";
    case 46:
        return "Bonk! Atomic Punch";
    case 56:
        return "The Huntsman";
    case 57:
        return "The Razorback";
    case 58:
        return "Jarate";
    case 59:
        return "The Dead Ringer";
    case 60:
        return "The Cloak And Dagger";
    case 61:
        return "The Ambassador";
    case 127:
        return "The Direct Hit";
    case 128:
        return "The Equalizer";
    case 129:
        return "The Buff Banner";
    case 130:
        return "The Scottish Resistance";
    case 131:
        return "The Chargin' Targe";
    case 132:
        return "The Eyelander";
    case 133:
        return "The Gunboats";
    case 140:
        return "The Wrangler";
    case 141:
        return "The Frontier Justice";
    case 142:
        return "The Gunslinger";
    case 153:
        return "The Homewrecker";
    case 154:
        return "The Pain Train";
    case 155:
        return "The Southern Hospitality";
    case 159:
        return "The Dalokohs Bar";
    case 163:
        return "Crit-a-cola";
    case 171:
        return "The Tribalman's Shiv";
    case 172:
        return "The Scotsman's Skullcutter";
    case 173:
        return "The Vita-saw";
    case 214:
        return "The Powerjack";
    case 215:
        return "The Degreaser";
    case 220:
        return "The Shortstop";
    case 221:
        return "The Holy Mackerel";
    case 222:
        return "Mad Milk";
    case 224:
        return "L'etranger";
    case 225:
        return "Your Eternal Reward";
    case 226:
        return "The Battalion's Backup";
    case 228:
        return "The Black Box";
    case 230:
        return "The Sydney Sleeper";
    case 231:
        return "Darwin's Danger Shield";
    case 232:
        return "The Bushwacka";
    case 237:
        return "Rocket Jumper";
    case 239:
        return "Gloves Of Running Urgently";
    case 265:
        return "Stickybomb Jumper";
    case 266:
        return "The Horseless Headless Horseman's Headtaker";
    case 304:
        return "The Amputator";
    case 305:
        return "The Crusader's Crossbow";
    case 307:
        return "The Ullapool Caber";
    case 308:
        return "The Loch-n-load";
    case 310:
        return "Warrior's Spirit";
    case 311:
        return "The Buffalo Steak Sandvich";
    case 312:
        return "The Brass Beast";
    case 317:
        return "The Candy Cane";
    case 325:
        return "The Boston Basher";
    case 326:
        return "The Back Scratcher";
    case 327:
        return "The Claidheamohmor";
    case 329:
        return "The Jag";
    case 331:
        return "Fists Of Steel";
    case 351:
        return "The Detonator";
    case 354:
        return "The Concheror";
    case 355:
        return "The Fan O'war";
    case 356:
        return "Conniver's Kunai";
    case 357:
        return "The Half-zatoichi";
    case 401:
        return "The Shahanshah";
    case 402:
        return "The Bazaar Bargain";
    case 404:
        return "The Persian Persuader";
    case 405:
        return "Ali Baba's Wee Booties";
    case 406:
        return "The Splendid Screen";
    case 411:
        return "The Quick-fix";
    case 412:
        return "The Overdose";
    case 413:
        return "The Solemn Vow";
    case 414:
        return "The Liberty Launcher";
    case 415:
        return "The Reserve Shooter";
    case 416:
        return "The Market Gardener";
    case 424:
        return "Tomislav";
    case 425:
        return "The Family Business";
    case 426:
        return "The Eviction Notice";
    case 441:
        return "The Cow Mangler 5000";
    case 442:
        return "The Righteous Bison";
    case 444:
        return "The Mantreads";
    case 447:
        return "The Disciplinary Action";
    case 448:
        return "The Soda Popper";
    case 449:
        return "The Winger";
    case 450:
        return "The Atomizer";
    case 452:
        return "Three-rune Blade";
    case 457:
        return "The Postal Pummeler";
    case 460:
        return "The Enforcer";
    case 461:
        return "The Big Earner";
    case 466:
        return "The Maul";
    case 482:
        return "Nessie's Nine Iron";
    case 513:
        return "The Original";
    case 525:
        return "The Diamondback";
    case 526:
        return "The Machina";
    case 527:
        return "The Widowmaker";
    case 528:
        return "The Short Circuit";
    case 572:
        return "Unarmed Combat";
    case 574:
        return "The Wanga Prick";
    case 587:
        return "Apoco-fists";
    case 588:
        return "The Pomson 6000";
    case 589:
        return "The Eureka Effect";
    case 593:
        return "The Third Degree";
    case 594:
        return "The Phlogistinator";
    case 595:
        return "The Manmelter";
    case 608:
        return "The Bootlegger";
    case 609:
        return "The Scottish Handshake";
    case 638:
        return "The Sharp Dresser";
    case 648:
        return "The Wrap Assassin";
    case 649:
        return "The Spy-cicle";
    case 656:
        return "The Holiday Punch";
    case 735:
        return "Builder Spy";
    case 736:
        return "Upgradeable Builder Spy";
    case 737:
        return "Upgradeable Pda Engineer Build";
    default:
        return "";
    }
}