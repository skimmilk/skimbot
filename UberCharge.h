#include "Required.h"
#include "Interfaces.h"
//0x890 + client.dll = load dword
namespace UberCharger
{
	void DoSomething(void)
	{
		int loadval = 0x890 + (int)Skim::ValveInterfaces::ClientFactory;
		_asm
		{

			call loadval;
		}
	}
}