#pragma once
#include <Windows.h>
#include <WinUser.h>
#include <vector>
using namespace std;
struct WHISTORY {
	TCHAR** hSites;
};
class WINDATA {
public:
    HWND Form;
    RECT Rect = RECT();
    HRGN HRect;
    HBRUSH Brush = (HBRUSH)COLOR_WINDOW;
    WINDOWINFO WinInfo;
    WNDCLASSEX Class = { 0 };
    WINDATA(RECT h, HBRUSH hbr, WNDCLASSEX classex) {
        Rect = h;
        Brush = hbr;
        Class = classex;
    }
   
};

struct NOTIFINFO {
    LPCWSTR Title;
    LPCWSTR Text;
    HICON Icon;
    RECT Rect;
};
typedef NOTIFINFO* LPNOTIFINFO;
void DrawToastNotif(LPNOTIFINFO n) {

}
#define WinRet(x) return (int)x.wParam

struct BROWSTAB {
    LPCWSTR hURL = L"about:blank";
    LPCWSTR hDocumentText = L"<html></html>";
    UINT hIndex = 0;
	BOOL isSecure = true;
    WHISTORY* history = { 0 };
    void LoadHTML(UINT i) {
        HGLOBAL s = LoadResource(NULL, FindResourceW(NULL, MAKEINTRESOURCE(i), MAKEINTRESOURCE(i)));
    }
};
struct BROWSER {
    vector<BROWSTAB> hTabs = vector<BROWSTAB>();
};
BROWSER AddTab(BROWSER t, BROWSTAB f) {
    t.hTabs.push_back(f);
    return t;
}

WNDCLASSEX GetBrowserClass(HINSTANCE hInst) {
    WNDCLASSEX r;
    r.cbSize = sizeof(WNDCLASSEX);
    r.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
    r.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    r.lpszClassName = L"Browser";
    r.cbWndExtra = 0;
    r.cbClsExtra = 0;
    r.hInstance = hInst;
    RegisterClassExW(&r);
    return r;
}
#define WC_BROWSER L"Browser"
HWND CreateBrowser(BROWSER x,HWND h) {
	return CreateWindowExW(
        WS_EX_ACCEPTFILES | WS_EX_CLIENTEDGE,  // Predefined class; Unicode assumed 
        L"",      // Button text 
        WC_BROWSER,  // Styles 
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
        40,         // x position 
        30,         // y position 
        400,        // Button width
        500,        // Button height
        h,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(h, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.);
}

HWND CreateWin(WINDATA* t,HWND h) {
   return CreateWindowW(
       L"",      // Button text 
       L"",  // Styles 
       WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
       t->Rect.left,         // x position 
       t->Rect.top,         // y position 
       400,        // Button width
       500,        // Button height
       h,     // Parent window
       NULL,       // No menu.
       (HINSTANCE)GetWindowLongPtr(h, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.);
}

HWND CreateWin(WINDATA t, HWND h) {
    return CreateWindowW(
        L"",      // Button text 
        L"",  // Styles 
        WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
        t.Rect.left,         // x position 
        t.Rect.top,         // y position 
        400,        // Button width
        500,        // Button height
        h,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(h, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.);
}