#include "pch.h"
#include <Windows.h>
#include <string>
#include <vector>

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
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        NOP(0x8731D0);
        //NOP(0x8730D6);
        NOP(0x885840);// don´t work on ( online servers )
        Sleep(1);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

