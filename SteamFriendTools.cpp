#include "SteamFriendTools.h"

#include "steam\ISteamClient012.h"
#include "steam\ISteamFriends010.h"

#include "steam\CGameID.h"
#include "Interfaces.h"

namespace Skim{ 
    bool SteamFriendTools::isFriend(int index)
    {
        return SteamFriendTools::isFriend(TF2Entity::GetPlayer(index));
    }
    bool SteamFriendTools::isFriend(TF2Player* p)
    {
        player_info_t* inf0 = TF2Entity::GetInfo(p);
        if (!inf0->friendsID)
            return false;// is a bot, console gets filled with errors if its 0
        CSteamID steamid = CSteamID( inf0->friendsID, k_EUniversePublic, k_EAccountTypeIndividual );
        return ValveInterfaces::pSteamFriends->HasFriend( steamid, k_EFriendFlagImmediate );
    }
    bool SteamFriendTools::GetPlayerIndexFromName(const char* name, int& index)
    {
        int maxplayers = ValveInterfaces::pEngine->GetMaxClients() + 1;
        for(int i = 0; i < maxplayers; i++)
        {
            if( i == ValveInterfaces::pEngine->GetLocalPlayer())
                continue;
            player_info_t plyr;
            ValveInterfaces::pEngine->GetPlayerInfo(i, &plyr);
            if(!strcmp(plyr.name, name))
            {
                index = i;
                return true;
            }
        }
        return false;
    }
    bool SteamFriendTools::GetPlayerFromName(const char* name, TF2Player* player)
    {
        int index = 0;
        if (!GetPlayerIndexFromName(name, index))
            return false;
        player = TF2Entity::GetPlayer(index);
        return true;
    }
};