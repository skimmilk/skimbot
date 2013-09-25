#include "CVarSaver.h"
#include <fstream>
#include <stdstring.h>
#include <vector>


namespace CVarSaver {

    std::vector<ConVar*> varList;

    void Init()
    {

    }

    void AddToList( ConVar* poni )
    {
        varList.push_back(poni);
    }

    void WriteAndShutdown()
    {
        std::string strToWrite;

        int len = varList.size();
        for (int i = 0; i < len; i++)
        {
            // Name of cvar
            strToWrite += varList[i]->GetName();
            strToWrite += " ";
            // Current value of cvar in string
            strToWrite += varList[i]->GetString();
            strToWrite += "\n";
        }
        std::fstream writeTo ( "tf\\cfg\\skimbot.cfg", std::fstream::out );
        if (!writeTo.bad())
            writeTo.write(strToWrite.c_str(), strToWrite.length());
        writeTo.close();

        varList.clear();

    }
}