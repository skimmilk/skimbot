#include "TF2Entity.h"

player_info_t * Skim::TF2Entity::GetInfo( TF2Player* player)
{
    static player_info_t* tmp;
    delete tmp;
    tmp = new player_info_t();
    CBaseEntity *pCE = (CBaseEntity*)player;
    Skim::ValveInterfaces::pEngine->GetPlayerInfo(pCE->index,tmp);
    return tmp;
}