#pragma once
#include "Required.h"

class IHack
{
public:
    virtual ~IHack(){}

    //Runs before CreateMove
    virtual void Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active){}
    //Runs after CreateMove
    virtual void Run(CUserCmd *cmd){}
    //Runs after PaintTraverse
    virtual void Run_PaintTraverse(){}
    //Runs after everything has been added to hack list
    virtual void Initialize(){}
};