// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include "stdio.h"
#include "windows.h"
#include "ConsoleApi.h"
#include "offsets.h"

//Player userPlayer = Player(getPhandle());
using namespace std;
int* health;

DWORD WINAPI patch(LPVOID lParam){
    FILE* pFile = nullptr;
    int * playerbase = (int*) 0x0050F4F4; 
    int health_offset = 0xf8; 
    health = (int*) (( *playerbase) + health_offset);
    if (AllocConsole()) {
        freopen_s(&pFile,"CONIN$", "rb", stdin);
        freopen_s(&pFile, "CONOUT$", "wb", stdout);
        freopen_s(&pFile, "CONOUT$", "wb", stderr);
        //cout << "injection success" << endl;
        printf("injection success\n");
        printf("base : %08x \n", playerbase);
        printf("base : %08x \n", * playerbase);
        printf("base : %08x \n", health );
        printf("health : %08x \n", *health);
        while (1) {
            if (*health < 100)
            {
                printf("low health");
                *health = 100;
            }
        }
    }
    //MessageBox(nullptr, L"injection success", L"dll injection", MB_OK);

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

