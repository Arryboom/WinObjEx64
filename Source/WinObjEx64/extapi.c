/*******************************************************************************
*
*  (C) COPYRIGHT AUTHORS, 2017 - 2018
*
*  TITLE:       EXTAPI.C
*
*  VERSION:     1.53
*
*  DATE:        07 Mar 2018
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
*******************************************************************************/
#include "global.h"

EXTENDED_API_SET g_ExtApiSet = { NULL, NULL, NULL };

HWINSTA StubNtUserOpenWindowStation(
    _In_ OBJECT_ATTRIBUTES ObjectAttributes,
    _In_ ACCESS_MASK DesiredAccess);

extern DWORD dwNtUserOpenWindowStation;

/*
* ExApiSetInit
*
* Purpose:
*
* Initializes newest Windows version specific function pointers.
*
* Called once during supInit
*
*/
NTSTATUS ExApiSetInit(
    VOID
    )
{
    NTSTATUS Status = STATUS_SOME_NOT_MAPPED;
    HANDLE hNtdll = NULL;
    HANDLE hWin32u = NULL;

    //
    // New Partition API introduced in Windows 10.
    //
    hNtdll = GetModuleHandle(TEXT("ntdll.dll"));
    if (hNtdll) {
        g_ExtApiSet.NtOpenPartition = (pfnNtOpenPartition)GetProcAddress(hNtdll, "NtOpenPartition");
        g_ExtApiSet.NtManagePartition = (pfnNtManagePartition)GetProcAddress(hNtdll, "NtManagePartition");
<<<<<<< HEAD

        if ((g_ExtApiSet.NtOpenPartition) &&
            (g_ExtApiSet.NtManagePartition))
            Status = STATUS_SUCCESS;
    }

    //
    // Win32k Native API now available in win32u.dll (same as ntdll stubs) since Windows 10 RS1.
    //
    if (g_WinObj.osver.dwBuildNumber >= 14393) {

        hWin32u = GetModuleHandle(TEXT("win32u.dll"));
        if (hWin32u == NULL) {
            hWin32u = LoadLibraryEx(TEXT("win32u.dll"), NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
            if (hWin32u == NULL)
                Status = STATUS_SOME_NOT_MAPPED;
        }

        g_ExtApiSet.NtUserOpenWindowStation = (pfnNtUserOpenWindowStation)GetProcAddress(hWin32u, "NtUserOpenWindowStation");
        if (g_ExtApiSet.NtUserOpenWindowStation == NULL)
            Status = STATUS_SOME_NOT_MAPPED;
        else
            Status = STATUS_SUCCESS;
    }
    else {

        g_ExtApiSet.NtUserOpenWindowStation = (pfnNtUserOpenWindowStation)&StubNtUserOpenWindowStation;

        //
        // If win32u unavailable use hardcode and select proper syscall id.
        //
        switch (g_WinObj.osver.dwBuildNumber) {

        case 7600:
        case 7601:
        case 9200:
            dwNtUserOpenWindowStation = 4256;
            break;
        case 9600:
            dwNtUserOpenWindowStation = 4257;
            break;
        case 10240:
        case 10586:
            dwNtUserOpenWindowStation = 4258;
            break;
        default:
            dwNtUserOpenWindowStation = 4256;
            break;
        }

    }
=======

        if ((g_ExtApiSet.NtOpenPartition) &&
            (g_ExtApiSet.NtManagePartition))
            Status = STATUS_SUCCESS;
    }

>>>>>>> refs/remotes/origin/master
    return Status;
}
