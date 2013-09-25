#pragma once
#include "Interfaces.h"
#include "Required.h"
#include "CVarSaver.h"

#define Check(n) if(n==0) { strcat(ErroredInterfaces, #n); strcat(ErroredInterfaces, "\n"); HasError = true; }

namespace Skim{ namespace ValveInterfaces{

IBaseClientDLL        *pClient ;
IVEngineClient        *pEngine ;
IEngineTrace        *pEngineTrace;
ICvar                *pCvar;
IVModelInfoClient    *pModelInfo ;
IClientEntityList    *pClientEntList ;
vgui::IPanel        *pPanel ;
vgui::ISurface        *pSurface;
//IEngineVGui            *pEngineVGui;
IGameEventManager2    *pGameEventManager;
//IPrediction            *pPrediction;
IStudioRender        *pStudioRender;
IAchievementMgr        *pIAchievementMgr;
CAchievementMgr        *pCAchievementMgr;
ISteamUserStats002    *pSteamUserStats002;
ISteamUserStats010    *pSteamUserStats010;
ISteamFriends010    *pSteamFriends;
IInput                *pIInput;
CInput                *pCInput;
ISteamClient012        *pSteamClient;
//Not loaded
//IViewEffects        *pViewEffects;
//Not loaded
CGlobalVarsBase        *pGlobals;
IVRenderView        *pRenderView;
IViewRender            *pViewRender;
IMaterialSystem        *pMaterials;

char* ErroredInterfaces;
bool HasError;


void Init(CreateInterfaceFn factory)
{
    ErroredInterfaces = new char[64];
    ErroredInterfaces[0] = 0;
    CreateInterfaceFn EngineFactory = Sys_GetFactory("engine.dll");
    CreateInterfaceFn ClientFactory = Sys_GetFactory("client.dll");
    //CreateInterfaceFn VGUIFactory = Sys_GetFactory("vguimatsurface.dll");
    CreateInterfaceFn VGUI2Factory = Sys_GetFactory("vgui2.dll");
    //CreateInterfaceFn MaterialFactory = Sys_GetFactory("materialsystem.dll");
    CreateInterfaceFn StudioFactory = Sys_GetFactory("studiorender.dll");
    CreateInterfaceFn interfaceFactory = factory;

    g_pCVar                     = (ICvar*)interfaceFactory(CVAR_INTERFACE_VERSION, NULL);
    pCvar = g_pCVar;
    Check(pCvar);
    pEngine                     = (IVEngineClient*)EngineFactory(VENGINE_CLIENT_INTERFACE_VERSION, NULL);
    Check(pEngine);
    pModelInfo                  = (IVModelInfoClient*)EngineFactory(VMODELINFO_CLIENT_INTERFACE_VERSION,0);
    Check(pModelInfo);
    pEngineTrace                = (IEngineTrace*)EngineFactory(INTERFACEVERSION_ENGINETRACE_CLIENT, NULL);
    Check(pEngineTrace);
    //pPrediction               = (IPrediction*)ClientFactory(VCLIENT_PREDICTION_INTERFACE_VERSION, NULL);

    pClient                     = (IBaseClientDLL*)ClientFactory("VClient016", NULL);
    Check(pClient);
    pClientEntList              = (IClientEntityList*)ClientFactory(VCLIENTENTITYLIST_INTERFACE_VERSION, NULL);
    Check(pClientEntList);
    pGameEventManager           = (IGameEventManager2*) interfaceFactory( INTERFACEVERSION_GAMEEVENTSMANAGER2, NULL);
    Check(pGameEventManager);
    pMaterials                  = (IMaterialSystem*)interfaceFactory("VMaterialSystem080",0);
    Check(pMaterials);

    pStudioRender               = (IStudioRender*)StudioFactory("VStudioRender025", NULL);
    Check(pStudioRender);

    pRenderView                 = (IVRenderView*)EngineFactory("VEngineRenderView014", 0);
    Check(pRenderView);

    //pSurface                  = (vgui::ISurface*)VGUI2Factory(VGUI_SURFACE_INTERFACE_VERSION, NULL);
    //Check(pSurface);
    pPanel                      = (vgui::IPanel*)VGUI2Factory(VGUI_PANEL_INTERFACE_VERSION, NULL);
    Check(pPanel);

    pGlobals = (CGlobalVarsBase*) *(PDWORD)(*(PDWORD)*(PDWORD)pClient + 0x39);

    pIAchievementMgr = pEngine->GetAchievementMgr();
    Check(pIAchievementMgr);
    if(pIAchievementMgr)
        pCAchievementMgr = dynamic_cast<CAchievementMgr*>(pIAchievementMgr);

    CreateInterfaceFn scf = Sys_GetFactory("steamclient.dll");
    pSteamClient = (ISteamClient012*)scf("SteamClient012", NULL);
    if (!pSteamClient) Error(PLUGIN_ERROR "Could not get steam client from steamclient.dll\n");

    HSteamPipe pipe = pSteamClient->CreateSteamPipe();
    if (!pipe) Error(PLUGIN_ERROR "Could not get steam pipeline\n");

    HSteamUser user = pSteamClient->ConnectToGlobalUser(pipe);
    if (!user) Error(PLUGIN_ERROR "Could not get steam user\n");

    pSteamFriends = (ISteamFriends010*)pSteamClient->GetISteamFriends(user, pipe, "SteamFriends010");
    if (!pSteamFriends) Error(PLUGIN_ERROR "Could not get steam friends interface\n");
    
    pSteamUserStats010            = (ISteamUserStats010*)pSteamClient->GetISteamUserStats(user, pipe, "STEAMUSERSTATS_INTERFACE_VERSION010");
    if (!pSteamUserStats010) Error(PLUGIN_ERROR "Could not get steam user stats\n");

    pIInput = **make_ptr<IInput***>( getvfunc<void*>( pClient, CREATEMOVE_INDEX ), 0x28 ); // pClient->CreateMove
    Check(pIInput);

    PDWORD m = (PDWORD)pIInput;
    PDWORD k = *( PDWORD* )m;
    DWORD  s =(DWORD)k[22];//GetFullscreenMousePos
    pSurface=**(vgui::ISurface***)(s + 5);//5 after update, weird stuff added to each what used to be just jmp funcs
    Check(pSurface);

    if(pIInput)
        pCInput = dynamic_cast<CInput*>(pIInput);
    Check(pCInput);
    //CFullFileSystem            = (IFileSystem*)EngineFactory(FILESYSTEM_INTERFACE_VERSION, NULL);
    //CMaterialSystem             = (IMaterialSystem*)MaterialFactory(MATERIAL_SYSTEM_INTERFACE_VERSION, NULL); nowork
    //CSoundEmitterSystem         = (ISoundEmitterSystemBase*)ClientFactory(SOUNDEMITTERSYSTEM_INTERFACE_VERSION, NULL);
    //CMatSystemSurface         = (IMatSystemSurface*)m_pVGuiSurface->QueryInterface(MAT_SYSTEM_SURFACE_INTERFACE_VERSION);

    PDWORD cli = (PDWORD)pClient;
    PDWORD clivirt = *( PDWORD* )cli;//virtual table
    DWORD  rndrvw =(DWORD)clivirt[27];//Client::Render_View(..)
    pViewRender=**(IViewRender***)(rndrvw + 5);//push ebp; mov ebp, esp; mov ecx, IViewRender

}

// stuff crashes when we delete con commands and vars for whatever reason
//  crashes not in main thread which this executes in but in some other thread
void FreeConCMD(ConVar* asdf, bool save)
{
    //pCvar->UnregisterConCommand(asdf);

    if(save)
        CVarSaver::AddToList(asdf);
    else
        delete asdf;

    //delete asdf;
}

void FreeConCMD(ConCommand* asdf)
{
    //pCvar->UnregisterConCommand(asdf);
    //delete asdf;
}

}}