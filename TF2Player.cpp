#include "TF2Player.h"

// Glory to the maker of Zeus TF2 base for getting the math part down
bool TF2Player::GetHitboxPos(Vector& v, int hitboxindex, BonePos::Enum pos)
{
    if (hitboxindex == -1){
        v = vecOrigin();
        return true;
    }
    int correctedhitbox = CorrectedHitbox(hitboxindex);
        
    if (correctedhitbox > 20)
        return false;

    matrix3x4_t pmatrix[MAXSTUDIOBONES];

    IClientEntity* clientent = ValveInterfaces::pClientEntList->GetClientEntity( this->index() );

    if(!clientent || !clientent->SetupBones(pmatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0)) return false;

    studiohdr_t* pStudioHdr = 0;

    if(isDisguised())
    {
        const int ispyindex = ValveInterfaces::pModelInfo->GetModelIndex("models/player/spy.mdl");
        const model_t *pSpyModel = ValveInterfaces::pModelInfo->GetModel( ispyindex );
        pStudioHdr = ValveInterfaces::pModelInfo->GetStudiomodel( pSpyModel );
    }
    else
    {
        const model_t* modelinfo = clientent->GetModel();
        pStudioHdr = ValveInterfaces::pModelInfo->GetStudiomodel(modelinfo);
    }

    mstudiobbox_t* hitbox = pStudioHdr->pHitbox(correctedhitbox, 0);

    MatrixAngles( pmatrix[ hitbox->bone ], angEyeAngles(), v );

    Vector min,max;
    VectorTransform( hitbox->bbmin, pmatrix[ hitbox->bone ], min );
    VectorTransform( hitbox->bbmax, pmatrix[ hitbox->bone ], max );

    if (pos == BonePos::MIDDLE)
        v = ((min + max) * 0.5f);//LOL FLOATING POINT OPTIMIZATIONS
    else if (pos == BonePos::MIN)
        v = min;
    else 
        v = max;
    return true;
}