#include "pch.h"
#include <Windows.h>
#include <string>
#include <vector>
#include "detour/detours.h"
#pragma comment(lib, "detour/detours.lib")
void NOP(std::uint64_t offset)
{
    unsigned char patch[1] = { 0x90 };
    std::uint64_t address = (std::uint64_t)GetModuleHandle(L"GameAssembly.dll");

    void* addr = (void*)(address + offset);

    DWORD old;

    VirtualProtect(addr, sizeof(patch), PAGE_EXECUTE_READWRITE, &old);
    memcpy(addr, patch, sizeof(patch));
    VirtualProtect(addr, sizeof(patch), old, &old);
}

void cheat()
{
    //__int64 __fastcall sub_873130(__int64 a1, int a2)
    typedef __int64(__fastcall* sub_873130)(__int64 a1, int a2);

    sub_873130 function = (sub_873130)(GetModuleHandle(L"GameAssembly.dll") + 0x873130);

    function(30, 30);

}

void Redo()//aka un nop
{
    unsigned char patch[5] = { 0x42, 0x89, 0x7C, 0xC2, 0x20 };
    std::uint64_t address = (std::uint64_t)GetModuleHandle(L"GameAssembly.dll");

    void* addr = (void*)(address + 0x8731D0);

    DWORD old;

    VirtualProtect(addr, sizeof(patch), PAGE_EXECUTE_READWRITE, &old);
    memcpy(addr, patch, sizeof(patch));
    VirtualProtect(addr, sizeof(patch), old, &old);
}

/*
//GameAssembly.dll+885840 - 90 godmode
//GameAssembly.dll+8732D7 - 89 7C C2 20 - ammo ( don´t use ) -- this address is getting call for somthing else :(
//GameAssembly.dll+8730D6 - 89 7C C2 20 - nate
//GameAssembly.dll+885840 - 42 89 7C C2 20        - mov [rdx+r8*8+20],edi - godmode
//GameAssembly.dll+885806 - 8B 74 CA 20           - mov esi,[rdx+rcx*8+20]
/

*/



//GameAssembly.dll+885840 - 42 89 7C C2 20        - mov [rdx+r8*8+20],edi
//godmode

__int64 player = 0;

void set_HP(int value)
{
    typedef char(__fastcall* sub_885840)(__int64 a1, int a2);
    sub_885840 function = (sub_885840)((uintptr_t)GetModuleHandle(L"GameAssembly.dll") + 0x8857A0);
    while (true)
    {
        if (player != 0)
        {
            if (GetAsyncKeyState(VK_INSERT) & 1)
            {
                function(player, value);
            }
        }
        if (GetAsyncKeyState(VK_HOME) & 1)
        {
            MessageBoxA(0, "functions was stopped", 0, 0);
            break;
        }
        Sleep(1);
    }
}

DWORD WINAPI CHEAT(LPVOID a1)
{
    set_HP(100);
    return 0;
}

typedef char(__fastcall* sub_885840)(__int64 a1, int a2);
sub_885840 __GODMODE_t = (sub_885840)((uintptr_t)GetModuleHandle(L"GameAssembly.dll") + 0x8857A0);
char* __fastcall __GODMODE(__int64 a1, int a2) {

    if (a2 < 50)
        __GODMODE_t(a1, 100);
    else
        __GODMODE_t(a1, a2);
    return 0;
};

// ammo
typedef __int64(__fastcall* sub_873130)(__int64 a1, int a2);
sub_873130 __ammo_t = (sub_873130)((uintptr_t)GetModuleHandle(L"GameAssembly.dll") + 0x873130);
char* __fastcall __sub_873130(__int64 a1, int a2) {
    __ammo_t(a1, 30);
    return 0;
};



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(LPVOID&)__ammo_t, &__sub_873130);
        DetourTransactionCommit();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(LPVOID&)__GODMODE_t, &__GODMODE);
        DetourTransactionCommit();
        //NOP(0x8731D0); // ammo
        //NOP(0x8730D6);
        //NOP(0x885840);// don´t work on ( online servers )
        //cheat();
        Sleep(1);
      //  CreateThread(0, 0, CHEAT, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

