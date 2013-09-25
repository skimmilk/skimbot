#pragma once
#include "Required.h"
#include "Interfaces.h"

namespace Skim{
namespace Namer
{
    void Init();
    void Shutdown();

    //notstealen
    void NameTo(const char*);
    //stealen
    void NameToEncrypted(const char*);
    //Fired by concommand, takes array of args and merges into single space-separated string
    char* CondenseToString(const CCommand &args);
};
};