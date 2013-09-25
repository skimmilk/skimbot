#include "Namer.h"

namespace Skim{
//================================================
//========Functions called by concommands=========
//================================================
void _EncryptNameTo(const CCommand &args)
{
    if(args.ArgC() == 0 || !(strlen(args.Arg(1))))
    {
        Msg(PLUGIN_NAME " Usage: skim_namen_stealen name");
    }
    Namer::NameToEncrypted( Namer::CondenseToString(args) );
}
void _NameTo(const CCommand &args)
{
    if(args.ArgC() == 0 || !(strlen(args.Arg(1))))
    {
        Msg(PLUGIN_NAME " Usage: " PREFIX "namen name");
    }
    Namer::NameTo( Namer::CondenseToString(args) );
}

//================================================
//==========Loads and unloads this hack===========
//================================================
ConCommand *cc1, *cc2;
void Namer::Init()
{
    cc1 = new ConCommand(PREFIX "namen", _NameTo, "Rename yourself");
    cc2 = new ConCommand(PREFIX "namen_stealen", _EncryptNameTo, "Rename yourself; good for stealing names, keeps yours unique");
}
void Namer::Shutdown(){
    Skim::ValveInterfaces::FreeConCMD(cc1);
    Skim::ValveInterfaces::FreeConCMD(cc2);
}

void Namer::NameTo(const char* name)
{
    using namespace Skim;
    ValveInterfaces::pSteamFriends->SetPersonaName(name);
}

void Namer::NameToEncrypted(const char* name)
{
    char *full;
    full = new char[strlen(name) + 3];

    strcpy(full, name);
    strcat(full, " %");

    NameTo(full);
    delete full;
}

char* Namer::CondenseToString(const CCommand &args)
{
    static char *full;
    delete full;
    full = new char[64];//Even though this is bigger than the max, steam will size it down, also we won't hopefully get overflows
    //with it being static, there will only ever be one instance of the string
    //no need to clean up afterwards
    full[0] = 0;//set first element to null so strcat will work

    //loops through all of the args and adds them to the full string
    for(int i = 1; i < args.ArgC(); i++)
    {
        strcat(full,args.Arg(i));
        if(i != args.ArgC()-1)
            strcat(full," ");
    }
    return full;
}

};