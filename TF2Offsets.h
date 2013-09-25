#pragma once
#define GETOFF(n) 
namespace Skim{
    namespace TF2Offsets{

        static void InitOffsets();

        namespace TFPlayer
        {
            static int bGlowEnabled;// = 0x0CFC;
            static int shared;// = 0x15AC;
            static int hItem;// = 0x1994;
            //const int playerClass = 0x1460 - 4 - shared;
        }
        namespace TFPlayerShared
        {
            static int nPlayerCond;// = 0xB0;
            static int nPlayerState;// = 0xAC;
            static int nDisguiseTeam;// = 0x190;
            static int nDisguiseClass;// = 0x194;
            static int nMaskClass;// = 0x198;
            static int flCloakMeter;// = 0x27C;
            static int iDecapitations;// = 0x348;
            static int iRevengeCrits;// = 0x348;
            static int hCarriedObj;// = 0x328;
            static int bCarryingObj;// = 0x32C;
            static int flEnergyDrinkMeter;// = 0x284;
            static int flHypeMeter;// = 0x288;
            static int flChargeMeter;// = 0x28C;
            static int iStunFlags;// = 0x324;
            static int iDisguiseBody;// = 0x360;
            static int iDisguiseTargetIndex;// = 0x1a0;
        }//rev crits 1D42D1AC - 1D42B888 = 1924
        //1DAC9F74 - 1DAC8650
        namespace BaseCombatCharacter
        {
            static int hActiveWeapon;// = 0xCF8;
            static int hMyWeapons;// = 0xC38;
        }
        namespace BasePlayer
        {
            static int iHealth;// = 0x80;
            static int lifeState;// = 0x7f;
            static int flMaxspeed;// = 0x1000;
            static int fFlags;// = 0x2ec;
            static int iObserverMode;// = 0xFC4;
            static int hObserverTarget;// = 0xFC8;
            static int hViewModelArray;// = 0x109C;
        }
        namespace BaseEnt
        {
            static int vecOrgin;// = 0x214;
            static int angRotation;// = 0x1984;//0x220;
            static int angRotationLocal;// = 0x220;
            //static int EyeAngles = 0xF48;
            static int flPlayerHeight;// = 0xEC;
            static int iTeamNum;// = 0x98;
            static int iHealth;// = 0x15AC;
            static int playerClass;// = 0x1460;
            static int hBasePlayer;// = iHealth - Skim::TF2Offsets::BasePlayer::iHealth;
            static int shared;// = 0x1768 - Skim::TF2Offsets::TFPlayerShared::nDisguiseClass;
            static int tfPlayer;// = shared - Skim::TF2Offsets::TFPlayer::shared;
            static int baseCombatCharacter;// = 0xD18 - BaseCombatCharacter::hActiveWeapon;
        }//pos 1DAC8864 - 1DAC8650 = 214
        namespace TFPlayerClassShared
        {
            static int iClass;// = 4;
        }
        namespace Weapon
        {
            static int WeaponID = 0x54C;
            static int WeaponSpread = 0x5C8;
            static int IsShotCritical = 0x55C;
        }
    }
}