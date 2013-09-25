#pragma once
#include "entry.h"
#include "Memory.h"
#include "loader.h"

volatile bool g_signal_exit = false;
bool errored = false;
HMODULE hmCheat;
void* hGameWindow = 0;



enum ErrorLoadingType{ nullappsys, none, nullg_pCVar, connectTierError, interfaces};

inline ErrorLoadingType getPointers()
{
    //Get the appsystemfactory I love    
    HMODULE EngineModule                    = NULL; 
    while(!EngineModule){    EngineModule    = GetModuleHandle( "engine.dll" );
                            Sleep(100);
    }
    CreateInterfaceFn pfnAppSystem = 0;
    void* appsys = (void*)Zeus::Utils::FindPattern( (DWORD)EngineModule, 0x623000, (BYTE*)"\x83\xC4\x04\x50\xA1\x00\x00\x00\x00\x50\xFF\xD2\x84\xC0\x75", "xxxxx????xxxxxx" );
    if ( appsys ) pfnAppSystem = **(CreateInterfaceFn**)( (size_t)appsys + 0x5 ); 
    else return nullappsys;


    LoadHack(pfnAppSystem);//caint even tell it was injected or plugin now

    if(errored) return connectTierError;
    return none;
}

DWORD WINAPI InitThread( LPVOID param )
{
    HMODULE engineModule = NULL;
    HMODULE clientModule = NULL;
    HMODULE steamModule = NULL;

    engineModule = GetModuleHandleA("engine.dll");
    clientModule = GetModuleHandleA("client.dll");
    steamModule = GetModuleHandleA("steamclient.dll");
    //I don't wanna wait so I got them beforehand
    while( !engineModule ) {
        engineModule = GetModuleHandleA("engine.dll");
        Sleep(100);
    }
    while( !clientModule ) {
        clientModule = GetModuleHandleA("client.dll");
        Sleep(100);
    }
    while( !steamModule ) {
        steamModule = GetModuleHandleA("steamclient.dll");
        Sleep(100);
    }

    getPointers();
    while ( !g_signal_exit)
    {
        Sleep( 100 );
    }
    Sleep( 100 );
    FreeLibraryAndExitThread( hmCheat, 0 );
}


bool __stdcall DllMain( HMODULE me, DWORD dwReason, LPVOID lpReserved )
{
    //int asdf;
    //asdf = -1;
    //asdf += 1;
    //int jk = 1 / asdf;
    if ( dwReason==DLL_PROCESS_ATTACH )
    {
        // Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications
        DisableThreadLibraryCalls(me);
        // Save our module handle
        hmCheat = me;
        CreateThread( 0, 0, &InitThread, 0, 0, 0 );
    }
    return true;
}