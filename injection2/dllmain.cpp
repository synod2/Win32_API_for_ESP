// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include "windows.h"

DWORD WINAPI patch(LPVOID lParam) {
    LPVOID TargetAddr = (LPVOID)0x0100579C;
    DWORD Protect = 0;

    VirtualProtect(TargetAddr, 0xB, PAGE_READWRITE, &Protect);
    // 0x0100579C 주소 영역에서 총 11바이트 만큼 공간에 write 권한을 부여함. 
    // ------- asm ------
    // mov 0100579c,00000000
    // nop 
    // ------- hex -----
    // C7 05 9C 57 00 01 00 00 00 00 
    // 90 
    *((LPBYTE)TargetAddr + 0) = 0xC7;
    *((LPBYTE)TargetAddr + 1) = 0x05;

    *((LPBYTE)TargetAddr + 2) = 0x9C;
    *((LPBYTE)TargetAddr + 3) = 0x57;
    *((LPBYTE)TargetAddr + 4) = 0x00;
    *((LPBYTE)TargetAddr + 5) = 0x01;

    *((LPBYTE)TargetAddr + 6) = 0x00;
    *((LPBYTE)TargetAddr + 7) = 0x00;
    *((LPBYTE)TargetAddr + 8) = 0x00;
    *((LPBYTE)TargetAddr + 9) = 0x00;

    *((LPBYTE)TargetAddr + 10) = 0x90;
    return 0;
}




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    HANDLE hThread = NULL;
    switch (ul_reason_for_call)
    {    
    case DLL_PROCESS_ATTACH:
        hThread = CreateThread(NULL, 0, patch, NULL, 0, NULL);
        CloseHandle(hThread);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

