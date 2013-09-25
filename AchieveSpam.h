#pragma once

#include "Required.h" 
#include "Interfaces.h"
namespace Skim{
namespace AchievementSpam 
{ 
    void Init();
    void Shutdown();

    void RelockAll();
    void UnlockAll();
    void UnlockSingle();
    void UnlockSingleByName(const char*);
    void RelockSingleByName(const char*);
    void ListAchievements();

    void UnlockSingleByIndex(uint index, bool store);
    void UnlockAchievement(IAchievement*, bool store);
    void LockAchievement(IAchievement*);
};
};