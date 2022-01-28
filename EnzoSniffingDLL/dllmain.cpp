// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <iostream>

HINSTANCE hThisDLL;
HHOOK hMsgHook;
HWND hwndApp = NULL;

#define WM_ENZO_MESSAGE (WM_USER + 1)

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    hThisDLL = hinstDLL;
    return TRUE;
}

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //do what you want to do
  //  if (WM_KEYDOWN == wParam)
   // {
        SendMessage(hwndApp, WM_ENZO_MESSAGE, wParam, lParam);
  //  }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) BOOL InstallHook(HWND hwndMainApp)
{
    if (!hMsgHook)
    {
        hwndApp = hwndMainApp;
        hMsgHook = SetWindowsHookEx(WH_KEYBOARD_LL, &GetMsgProc, hThisDLL, 0);
      //  SendMessage(hwndApp, WM_ENZO_MESSAGE, 0, 0);
    }
    return (hMsgHook != NULL);
}

extern "C" __declspec(dllexport) VOID UninstallHook()
{
    if (hMsgHook)
    {
        UnhookWindowsHookEx(hMsgHook);
        hMsgHook = NULL;
    }
}
