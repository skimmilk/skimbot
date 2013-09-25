/*
Zeus TF2 Basehook
Copyright (C) 2009 Xeno123

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef Memory_h__
#define Memory_h__

#include <windows.h>

namespace Zeus
{
    namespace Utils
    {
        inline bool DataCompare( const BYTE* pData, const BYTE* bMask, const char* szMask )
        {
            for( ; *szMask; ++szMask, ++pData, ++bMask )
            {
                if( *szMask == 'x' && *pData != *bMask )
                    return false;
            }

            return ( *szMask ) == NULL;
        }

        // Finds a pattern at the specified address
        inline DWORD FindPattern ( DWORD dwAddress, DWORD dwSize, BYTE* pbMask, char* szMask )
        {
            for( DWORD i = NULL; i < dwSize; i++ )
            {
                if( DataCompare( (BYTE*) ( dwAddress + i ), pbMask, szMask ) )
                    return (DWORD)( dwAddress + i );
            }
            return 0;
        }
    }
}
#endif // Memory_h__
