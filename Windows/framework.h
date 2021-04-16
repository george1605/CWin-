// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <WinBase.h>
#include <WinUser.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#define defaultFont (HFONT)GetStockObject(DEVICE_DEFAULT_FONT)
#define okPress(Msg) (Msg == IDOK)
#define BTN_CREATE WM_CREATE | BS_PUSHBUTTON
#define BTN_CLICK  WM_COMMAND | BS_PUSHBUTTON