#pragma once

#include "Required.h"
#include "BaseHook.h"
#include "..\game\client\viewrender.h"
#include "steam\ISteamUserStats002.h"

namespace Skim
{
    namespace ValveInterfaces
    {
        extern IBaseClientDLL        *pClient ;
        extern IVEngineClient        *pEngine ;
        extern IEngineTrace            *pEngineTrace;
        extern ICvar                *pCvar;
        extern IVModelInfoClient    *pModelInfo ;
        extern IClientEntityList    *pClientEntList;
        extern vgui::IPanel            *pPanel ;
        extern vgui::ISurface        *pSurface;
        //extern IEngineVGui            *pEngineVGui ;
        extern IGameEventManager2    *pGameEventManager;
        //extern IPrediction            *pPrediction ;
        extern IStudioRender        *pStudioRender;
        extern CGlobalVarsBase        *pGlobals;
        extern IVRenderView            *pRenderView;
        extern IViewRender            *pViewRender;
        extern IMaterialSystem        *pMaterials;
        
        extern IAchievementMgr        *pIAchievementMgr;
        extern CAchievementMgr        *pCAchievementMgr ;
        extern ISteamUserStats002    *pSteamUserStats002;
        extern ISteamUserStats010    *pSteamUserStats010;
        extern ISteamFriends010        *pSteamFriends;
        extern ISteamClient012        *pSteamClient;

        extern IInput                *pIInput;
        extern CInput                *pCInput;

        extern void Init(CreateInterfaceFn factory);

        extern char* ErroredInterfaces;
        extern bool HasError;
        
        void FreeConCMD(ConVar*, bool save = true);
        void FreeConCMD(ConCommand*);
    };
}