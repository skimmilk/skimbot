#include "Required.h"
#include "TF2Entity.h"

namespace Skim{
    class SteamFriendTools{
    public:
        static bool isFriend(int index);
        static bool isFriend(TF2Player* p);
        
        // returns false if none found, returns true and sets index if name is found
        static bool GetPlayerIndexFromName(const char*, int& index);
        static bool GetPlayerFromName(const char*, TF2Player*);
};};