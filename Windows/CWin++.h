#include "framework.h"
#include <iostream>
#include <string>
#include <WinBase.h>
#include <CommCtrl.h>
#include <chrono>
#include <Richedit.h>
#include <datetimeapi.h>
#include <thread> 
#include <map>
#include <commdlg.h>
#include <commctrl.h>
#include <cctype>
#include <vector>
#include <shellapi.h>
#include <functional>
#include "LastControls.h"
#include "Colors.h"
#include "BaseProc.h"
#include <processenv.h>
#include "WNDClassesh.h"
#include "Controls.h"
#include <uxtheme.h>
#include "WinExtend.h"
#include "BaseGUI.h"
#include "CLR.h"
#include <fstream>
#define GWL_HINSTANCE (-6)
using namespace std;

#define Monitors GetSystemMetrics(SM_CMONITORS)
#define CursorY GetSystemMetrics(SM_CYCURSOR)
#define CursorX GetSystemMetrics(SM_CXCURSOR)
#define ActBorder GetSysColor(COLOR_ACTIVEBORDER)
#define IDC_PROJECT IDC_LASTCONTROLS
#define IDI_PROJECTIC IDI_ICON
#define BaseFont DS_SHELLFONT 
#define MAX_LOADSTRING 100
#define ShuttingDown GetSystemMetrics(SM_SHUTTINGDOWN)
int CALLBACK MainProc(HWND x, UINT Message, LPARAM l, WPARAM w);
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

enum CheckState {
  Checked,
  Medium,
  NoCheck,
  Disabled
};

class PaintArgs : BP_PAINTPARAMS  {
  
};

class Font {
private:
  HFONT i;
public:
  Font(){}
  Font(int Width,int Height){}
};

namespace MyFonts {
  
}

LPCWSTR getWStr(std::string p) {
    std::wstring stemp = std::wstring(p.begin(), p.end());
    LPCWSTR sw = stemp.c_str();
    return sw;
}
HWND getCrnWin() {
    return FindWindowW(szWindowClass, szTitle);
}
void view_file(string k) {
    std::wstring stemp = std::wstring(k.begin(), k.end());
    LPCWSTR sw = stemp.c_str();
    ShellExecute(NULL, L"open", sw,
        NULL, NULL, SW_SHOWNORMAL);
}
bool CALLBACK SetLFont(HWND child, LPARAM font) {
    SendMessage(child, WM_SETFONT, font, true);
    return true;
}
void setBaseFont(HWND Parent) {
    EnumChildWindows(Parent, (WNDENUMPROC)SetLFont, (LPARAM)GetStockObject(DS_SHELLFONT));
}

void MoveCWin(HWND hw,int X,int Y,int Height,int Width) {
    MoveWindow(
        hw,
        X,
        Y,
        Width,
        Height,
        TRUE
    );
}
void setFont(HWND hEdit, HFONT hFont) {
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
}

VOID CALLBACK TimerProc(HWND hwnd,
    UINT uMsg,
    UINT idEvent,
    DWORD dwTime)
{
    MessageBox(NULL, L"The Time Elapsed!", L"Timer Procedure", MB_OK);
    if (!KillTimer(hwnd, idEvent)) {
        MessageBox(NULL, L"Cannot stop the timer.Please close the window manually.", L"Timer Procedure", MB_OK);
    }
}
#define notInit(hWnd) if(hWnd == NULL){ MessageBoxW(NULL,L"The Window did not created",L"Error",MB_ICONERROR | MB_OK); }
void newRoundRect(HWND h = NULL, int a = 10, int b = 10, int c = 210, int d = 210, int w1 = 10, int h1 = 10) {
    HRGN x = CreateRoundRectRgn(a, b, c, d, w1, h1);
    SetWindowRgn(h, x, true);
}

WNDCLASSEX getDefWin() {
    WNDCLASSEX wce;
    GetClassInfoEx(GetModuleHandle(nullptr), szWindowClass, &wce);
    return wce;
}

HBRUSH getCrnBrush() {
    return getDefWin().hbrBackground;
}

void setToCrnBrush(HBRUSH& k) {
    k = getCrnBrush();
}

class CommandEvent {
public:
    LPARAM lp;
    WPARAM wp;
    UINT message = WM_CREATE;
    HWND win = NULL;
};

void MsgBox(LPCWSTR message = L"", LPCWSTR title = L"", UINT Buttons = MB_OK) {
    MessageBoxW(NULL, message, title, Buttons);
}

void MsgBeep(UINT Buttons = MB_ICONASTERISK) {
    MessageBeep(Buttons);
}

class MsgBoxArgs : public MSGBOXPARAMSA{
public:
   MsgBoxArgs(){}
};

class Style {
public:
    COLORREF Color = RGB(255,255,255);
    HCURSOR Cursor;
    HBRUSH Brush = (HBRUSH)RGB(255, 255, 255);
    HICON Icon;
    HICON IconSm;
    int X, Y;
    int Top, Left;
    UINT Mode = WS_BORDER;
    HWND Parent = NULL;
    void LoadCrs(LPWSTR k) {
        Cursor = LoadCursor(nullptr, k);
    }
    void LoadImg(){
    
    }
    void setBack(int r, int g, int b) {
        Brush = (HBRUSH)RGB(r, g, b);
    }
};

WNDCLASSEX createClass(LPCWSTR Name, Style style) {
    WNDCLASSEX f;
    f.cbSize = sizeof(WNDCLASSEX);

    f.cbClsExtra = 0;
    f.cbWndExtra = 0;
    f.hbrBackground = style.Brush;
    f.hCursor = style.Cursor;
    f.style = style.Mode;
    f.lpszClassName = Name;
    f.hIcon = style.Icon;
    f.hIconSm = style.IconSm;
    f.hInstance = (HINSTANCE)NULL;

    RegisterClassEx(&f);
    return f;
}

HWND createNewWin(WNDCLASSEX t, Style p) {
    return CreateWindowEx(WS_EX_CLIENTEDGE, t.lpszClassName, L"", WS_BORDER, p.Left, p.Top, p.X, p.Y, p.Parent, CreateMenu(), NULL, NULL);
}
HWND createWinType(LPCWSTR Name, Style style) {
    return createNewWin(createClass(Name, style), style);
}

enum Events {
  command = WM_COMMAND
}

INT_PTR CALLBACK SelDate(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else {
        }
        break;
    }
    return (INT_PTR)FALSE;
}

class Point {
public:
    int X, Y;
    Point():X(0),Y(0) {}
    Point(int a,int b){
     X = a;
     Y = b;
    }
  Point(POINT u){
    X = u.X;
    Y = u.Y;
  }
};

class Size {
public:
    int Width, Height;
    Size(int x, int y) {
        Width = x;
        Height = y;
    }
};

class Control {
public:
    bool Visible = true;
    int Width, Height;
    int Top, Left;
    HWND Parent = NULL;
    HWND win = NULL;
    void SetParent(Control* n) {
        Parent = (*n).win;
    }
    void Show() {

    }
    void Destroy() {
        ShowWindow(this->win, SW_HIDE);
        UpdateWindow(win);
        SendMessage(this->win,WM_DESTROY,0,0);
    }
    void SendMsg(UINT type,WPARAM w,LPARAM l) {
        SendMessage(win, type, w, l);
    }
    ~Control() {
        DestroyWindow(win);
    }
};

class Panel : public Control {

};


int Close(HWND x,int code = 0) {
    SendMessage(x, WM_DESTROY, 0, 0);
    return code;
}

void Wait(int n) {
    this_thread::sleep_for(std::chrono::seconds(n));
}

LPWSTR WinType(HWND x) {
    LPWSTR n = (LPWSTR)NULL;
    GetClassName(x, n, 100);
    return n;
}

class Button : public Control {
public:
    int Width = 75;
    int Height = 23;
    int Top = 10;
    int Left = 10;
    LPCWSTR Text = L"";
    HWND win = NULL;
    HWND Parent = NULL;
    void Show() {
        win = CreateWindow(
            L"BUTTON", 
            Text,
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_PUSHLIKE | BS_FLAT,
            Left,      
            Top,  
            Width,  
            Height,    
            Parent,  
            NULL,
            (HINSTANCE)GetWindowLongPtr(Parent, GWLP_HINSTANCE),
            NULL);
        SendMessageA(win, BTN_CREATE, 0, 0);
    }
    void Click() {

    }
    void Disable() {

    }
};

class Icon {
protected:
    HICON h;
    HANDLE handle;
public:
    void Draw(HWND hwnd = NULL, int a = 0, int b = 0) {
        HDC hdc = GetDC(hwnd);
        DrawIcon(hdc, a, b, h);
    }
    void Load(HINSTANCE hInst,UINT name) {
        h = LoadIconA(hInst, MAKEINTRESOURCEA(name));
    }
    void LoadImg(LPCWSTR p) {
        h = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
            NULL,             // hInstance must be NULL when loading from a file
            p,   // the icon file name
            IMAGE_ICON,       // specifies that the file is an icon
            0,                // width of the image (we'll specify default later on)
            0,                // height of the image
            LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
            LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
            LR_SHARED         // let the system release the handle when it's no longer used
        );
    }

};

#define baseTimerID 0x11ff11
int ActiveTimers = 0;

class Timer : public Control {
public:
    UINT_PTR ID = baseTimerID + ActiveTimers;
    TIMERPROC OnElapse = (TIMERPROC)TimerProc;
    UINT Interval = 1000;
    HWND Parent = NULL;
    Timer() {
        ActiveTimers++;
    }
    ~Timer() {
        ActiveTimers--;
    }
    void Start() {
        ID = SetTimer(
            Parent,
            IDT_TIMER,
            Interval,
            OnElapse
        );
    }
    void Stop() {
        KillTimer(Parent, IDT_TIMER);
    }
};

Icon createIcon(HINSTANCE n,UINT j) {
    Icon l;
    l.Load(n, j);
    return l;
}
void MsgLoop(MSG& msg, HACCEL hAccelTable) {
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

class PrintDialog {
private:
    PRINTDLGW dial;
public:
    PrintDialog() {
        dial.Flags = PD_ALLPAGES;
        dial.hwndOwner = NULL;
        dial.nCopies = 1;
        dial.hDC = GetDC(dial.hwndOwner);
    }
    void SetParent(HWND x) {
        dial.hwndOwner = x;
    }
    void Show() {
        PrintDlg(&dial);
    }
};


class Window {
public:
    HWND Parent = NULL;
    HWND win;
    int Width, Height;
    int Top, Left;
    WNDCLASSEX ClassEx = { 0 };
    WNDCLASS Class = { 0 };
    bool Closed = false;
    Window() : Top(100), win(NULL), Left(100), Width(200), Height(300) {
        ClassEx.cbSize = sizeof(WNDCLASSEX);
    }
    void RegisterAll() {
        RegisterClassEx(&ClassEx);
        RegisterClass(&Class);
    }
    void Register() {
        RegisterClass(&Class);
    }
    void RegisterEx() {
        RegisterClassEx;
    }
    void Create() {
        win = CreateWindowEx(WS_EX_CLIENTEDGE, ClassEx.lpszClassName, L"MyWin", WS_BORDER | WS_CHILD, Top, Left, Width, Height, Parent, (HMENU)CreateMenu(), (HINSTANCE)NULL, NULL);
    }
    void Close() {
        DestroyWindow(this->win);
        Closed = true;
    }
};


class DatePicker : public Control {
public:
    HWND win;
    string Date;
    DatePicker() {
        win = NULL;
    }
    void Show() {
        win = CreateWindowEx( //
            /* Extended Style   */0, //
            /* Class Name       */DATETIMEPICK_CLASS, //
            /* Element Name     */L"", //
            /* Style            */(WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_TIMEFORMAT), //
            /* X Position       */Left, //
            /* Y Position       */Top, //
            /* Width            */Width, //
            /* Height           */Height, //
            /* Parent Window    */Parent, //
            /* Menu             */NULL, //
            /* Instance         */NULL, //
            /* Parameter        */NULL);
    }
};

class ColorDialog : public Control {
public:
    CHOOSECOLORA chooser = { 0 };
    void Show() {
        chooser.hwndOwner = Parent;
        chooser.lpTemplateName = (LPCSTR)"";
        chooser.lpCustColors = (COLORREF*)(&Yellow);
        chooser.lStructSize = sizeof(CHOOSECOLORA);
        chooser.Flags = CC_FULLOPEN;
        ChooseColorA(&chooser);
    }
};

TCITEM getTabItem(LPWSTR Text) {
    TCITEM r;
    r.mask = TCIF_TEXT;
    r.pszText = Text;
    r.iImage = (-1);
    return r;
}

class TabControl : public Control{
public:
    vector<TCITEM> Items;
    int Width = 200;
    int Height = 400;
    int Index = 0;
    void Show() {
       win = CreateWindow(
            WC_TABCONTROL,  // Predefined class; Unicode assumed 
            L"",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
            Left,         // x position 
            Top,         // y position 
            Width,        // Button width
            Height,        // Button height
            Parent,     // Parent window
            NULL,       // No menu.
            (HINSTANCE)GetWindowLongPtr(Parent, GWLP_HINSTANCE),
            NULL);
    }
    void AddPage(TCITEM x) {
        Index++;
        Items.push_back(x);
        SendMessageW(win, TCM_INSERTITEM, Index, (LPARAM)&x);
    }
};

class TextBox : public Control {
public:
    int Width = 80;
    int Height = 25;
    int Top = 10;
    int Left = 10;
    LPCWSTR Text;
    HWND Parent = NULL;
    HWND win = NULL;
    void Show() {
       win = CreateWindow(
            L"EDIT",  // Predefined class; Unicode assumed 
            Text,      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER,  // Styles 
            Left,         // x position 
            Top,         // y position 
            Width,        // Button width
            Height,        // Button height
            Parent,     // Parent window
            NULL,       // No menu.
            (HINSTANCE)GetWindowLongPtr(Parent, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.
    }
    LPCWSTR getText() {
        TCHAR text[256];
        SendMessage(win, WM_GETTEXT, sizeof(text) / sizeof(text[0]), LPARAM(text));
        return text;
    }
    void setLimit(int k) {
        SendMessage(win, EM_SETLIMITTEXT, (WPARAM)k, 0);
    }
};

class Label : public Control{
public:
    LPCWSTR Text = L"";
    HWND win;
    bool BorderActive = false;
    void Show() {
        UINT k = (BorderActive) ? WS_BORDER : 1;
        win = CreateWindow(
            L"STATIC",
            Text,
            WS_VISIBLE | WS_CHILD | SS_CENTER | k,
            10,
            10,
            100,
            20,
            Parent,
            CreateMenu(),
            (HINSTANCE)GetWindowLong(Parent, GWL_HINSTANCE),
            NULL);
    }
};

void UnRefParam(HINSTANCE hPrevInstance,LPWSTR lpCmdLine) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
}
#define intENTR int APIENTRY
#define voidENTR void APIENTRY
#define BadImg() throw WINCODEC_ERR_BADIMAGE
#define BadHeader() throw WINCODEC_ERR_BADHEADER
#define FrameMiss() throw WINCODEC_ERR_FRAMEMISSING
#define EvUnHook(voidptr) UnhookWinEvent((HANDLE)voidptr)
#define BlueBack BACKGROUND_BLUE
#define EvHook(evntproc,procID,threadID) SetWinEventHook(EVENT_MIN,EVENT_MAX,GetModuleHandle(),(WINEVENTPROC)evntproc,(UINT)procID,(UINT)threadID,WINEVENT_INCONTEXT)
#define ViewNormal WS_BORDER | WS_CHILD | WS_VISIBLE


class ComboBox : public Control {
public:
    std::vector<TCHAR*> Items;
    std::string Text = "myComboBox";
    int Width = 80;
    int Height = 65;
    int Top = 10;
    int Left = 10;
    HWND Parent = NULL;
    HWND win = NULL;
    void Show() {
        CreateWindow(WC_COMBOBOX, TEXT(""),
            CBS_DROPDOWN | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            Left, Top, Width, Height, Parent, NULL, (HINSTANCE)GetWindowLongPtr(Parent, GWLP_HINSTANCE), NULL);
    }
    void AddItem(TCHAR* p) {
        Items.push_back(p);
        SendMessage(win, CB_ADDSTRING, 0,(LPARAM)p);
        SendMessage(win, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
    }
};
ComboBox CreateComboBox(HWND r) {
    ComboBox d;
    d.Parent = r;
    return d;
}
#define IDC_MONTHCAL 101
class MonthCalendar : public Control {
public:
    HWND win;
    void Show() {
        HWND hwndDlg = NULL;
        HINSTANCE g_hInst = NULL;
        win = CreateWindowEx(0,
            MONTHCAL_CLASS,
            L"",
            WS_BORDER | WS_CHILD | WS_VISIBLE | MCS_DAYSTATE,
            0, 0, 300, 200, // resize it later
            Parent,
            CreateMenu(),
            g_hInst,
            NULL);

    }
};

ComboBox newComboBox(HWND Parent) {
    ComboBox g;
    g.Parent = Parent;
    return g;
}

ComboBox createComboBox(HWND Parent) {
    ComboBox g;
    g.Parent = Parent;
    return g;
}

Button createBtn(LPCWSTR Text, HWND Parent) {
    Button k;
    k.Text = Text;
    k.Parent = Parent;
    return k;
}

POINT newPoint(int a,int b) {
    POINT r;
    r.x = a;
    r.y = b;
    return r;
}

Button createBtn(LPCWSTR Text, HWND Parent,POINT pos) {
    Button k;
    k.Text = Text;
    k.Parent = Parent;
    k.Top = pos.y;
    k.Left = pos.x;
    return k;
}

TextBox createTextBox(LPCWSTR Text = L"", HWND Parent = NULL) {
    TextBox k;
    k.Text = Text;
    k.Parent = Parent;
    return k;
}


class Color {
public:
    static const int White = 1;
    static const int Gray = 2;
    static const int LightGray = 8;
    static const int Black = 3;
};

class ToolStrip : public Control{
public:
    HWND Tools;
    int Width = 100;
    int Height = 30;
    void Show() {
     win = CreateWindow(
            REBARCLASSNAME,
            NULL,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
            WS_CLIPCHILDREN | RBS_VARHEIGHT |
            CCS_TOP | CCS_NOMOVEX| RBS_BANDBORDERS,
            0, 0, 100, 30,
            Parent,
            NULL,
            hInst,
            NULL);
      Tools = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_TOOLTIPS,
         0, 0, 100, 30, win, (HMENU)0, hInst, NULL);
    }
    void Add() {
        
    }
};

class UpDownButton : public Control {
public:
    int Height = 25;
    int Width = 72;
    int Min = 0;
    int Max = 100;
    int Value = 50;
    HWND Spinner;
    void Show() {
        win = CreateWindowEx(WS_EX_LEFT | WS_EX_CLIENTEDGE | WS_EX_CONTEXTHELP,    //Extended window styles.
            WC_EDIT,
            L"0",
            WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER    // Window styles.
            | ES_NUMBER | ES_LEFT ,                     // Edit control styles.
            Left,
            Top,
            Width,
            Height,
            Parent,
            NULL,
            hInst,
            NULL);
        Spinner = CreateWindow(UPDOWN_CLASS, L"SP_RED",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP
            | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT
            | UDS_SETBUDDYINT | WS_BORDER,
             Left + Width - 2, Top + 1, Width - 1,20,
            win, CreateMenu(),
            (HINSTANCE)GetWindowLong(win, GWL_HINSTANCE), NULL);
    }
    void Increment() {
       
    }
};
class EventArgs {
public:
    LPARAM l;
    WPARAM w;
    EventArgs(LPARAM lpar, WPARAM wpar) {
        l = lpar;
        w = wpar;
    }
};

class CheckBox : public Control {
public:
    HWND Parent = NULL;
    HWND win;
    int Width = 50;
    int Height = 30;
    int Top = 10;
    int Left = 10;
    bool ThreeState = false;
    LPCWSTR Text = L"CheckBox";
    CheckBox(CheckState a){
      switch(a){
          
      }
    }
    CheckBox(bool r){
        ThreeState = r;
    }
    void Show() {
        if (!ThreeState) {
            win = CreateWindowEx(
                0,
                L"BUTTON",
                Text,
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                Left,
                Top,
                80,
                20,
                Parent,
                (HMENU)CreateMenu(),
                (HINSTANCE)NULL,
                NULL);
        }
        else {
            win = CreateWindowEx(
                0,
                L"BUTTON",
                Text,
                WS_VISIBLE | WS_CHILD | BS_AUTO3STATE,
                10,
                10,
                80,
                20,
                Parent,
                (HMENU)CreateMenu(),
                (HINSTANCE)NULL,
                NULL);
        }
    }
    void Check(bool n = true) {
        UINT in = (n) ? BST_CHECKED : BST_UNCHECKED;
        SendMessage(win, BM_SETCHECK,in, 0);
    }
};
class GroupBox : public Control {
public:
    HWND win;
    LPCWSTR Text;
    int ConTop = 30;
    void Show() {
        win = CreateWindowEx(
            0,
            L"BUTTON",
            Text,
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10,
            10,
            250,
            200,
            Parent,
            (HMENU)CreateMenu(),
            (HINSTANCE)NULL,
            NULL);
    }
    void Add(Control w) {
        w.Parent = Parent;
        w.Top = this->Top + ConTop;
        w.Show();
        ConTop += 20;
    }
};
class Image : public Icon{
private:
    HBITMAP bit;
public:
    void LoadBit(HINSTANCE x = nullptr, UINT def = IDC_PROJECT) {
        bit = LoadBitmapW(x, MAKEINTRESOURCEW(def));
        this->h = (HICON)bit;
    }
};
class Accelerator {
private:
    HACCEL acc;
public:
    void Load(HINSTANCE x = nullptr,UINT def = IDC_PROJECT) {
        acc = LoadAcceleratorsW(x, MAKEINTRESOURCEW(def));
    }
    HACCEL getAcc() {
        return acc;
    }
};

void MyActiv(HWND hWnd, UINT message, EventArgs x) {
    MessageBox(NULL, L"Action Performed", L"", MB_APPLMODAL | MB_OK);
}

void ShowWinObj(WinObj x) {
    if (x.cHandle == NULL) {
        x.cHandle = CreateWindowW(x.exClass.lpszClassName, L"", WS_BORDER | WS_CAPTION, 40, 40, 400, 500, NULL, CreateMenu(), nullptr, NULL);
    }
}

class SplitButton : public Control {
public:
    HWND Parent = NULL;
    HWND win;
    int Width = 50;
    int Height = 30;
    int Top = 10;
    int Left = 10;
    LPCWSTR Text = L"CheckBox";
    void Show() {
        win = CreateWindowEx(

            0,
            L"BUTTON",
            Text,
            WS_VISIBLE | WS_CHILD | BS_SPLITBUTTON,
            10,
            10,
            80,
            20,
            Parent,
            (HMENU)CreateMenu(),
            (HINSTANCE)NULL,
            NULL);
    }
};

UINT WidthOf(HWND h) {
    RECT z;
    GetWindowRect(h, &z);
    return abs(z.left - z.right);
}

UINT HeightOf(HWND h) {
    RECT z;
    GetWindowRect(h, &z);
    return abs(z.top - z.bottom);
}

UINT AreaOf(HWND h) {
    return (WidthOf(h) * HeightOf(h));
}
class ThisProgram {
public:
   static void Start(_In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPWSTR    lpCmdLine,
        _In_ int       nCmdShow) 
    {
        wWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    }
};
#define Main wWinMain
#define MainEntry intENTR Main
#define CALLINT int CALLBACK


class ScrollBar : public Control {
public:
    HWND win;
    void AddTo(HWND hWnd) {
        Parent = hWnd;
        win = CreateWindowEx(0L,
            L"SCROLLBAR",
            NULL, // There is no text to display
            WS_CHILD | WS_VISIBLE | SBS_VERT |
            SBS_TOPALIGN,
            WidthOf(Parent)-41,
            0,
            18,
            HeightOf(Parent) - 65,
            hWnd,
            NULL,
            hInst,
            NULL
        );
    }
};

HBRUSH getBrush(int colOff) {
    return (HBRUSH)(COLOR_WINDOW + colOff);
}

class RadioButton : public Control {
public:
    HWND win = NULL;
    LPCWSTR Text = L"RadioButton";
    void Create() {
        win = CreateWindowEx(

            0,
            L"BUTTON",
            Text,
            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
            10,
            10,
            80,
            20,
            Parent,
            (HMENU)CreateMenu(),
            (HINSTANCE)NULL,
            NULL);
    }
    void Check(bool k = true) {

    }
    void Show() {
        Create();
    }
};

class TreeView : public Control {
public:
    HTREEITEM tree;
    HWND win = NULL;
    int inst = 10101;
    void Show() {
        win = CreateWindowEx(0,
            WC_TREEVIEW,
            TEXT("Tree View"),
            WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
            10,
            10,
            Width,
            Height,
            Parent,
            CreateMenu(),
            (HINSTANCE)inst,
            NULL);
    }
    void AddNode(HTREEITEM After, LPWSTR Text) {
        TV_INSERTSTRUCT n;
        n.hParent = tree;
        n.hInsertAfter = After;
        n.item.pszText = Text;
        SendDlgItemMessage(win, inst,
            TVM_INSERTITEM, 0, (LPARAM)&n);
    }
};

#define SaveMode 0x0000000111
#define OpenMode 0x0000000110

class FileDialog {
public:
    string Filter = "All\0*.*";
    LPCWSTR FileName;
    string Title;
    int Result;
    int Mode = OpenMode;
    void Show() {
        OPENFILENAME ofn;       // common dialog box structure
        TCHAR szFile[260] = { 0 };       // if using TCHAR macros

        // Initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = (HWND)NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (Mode == OpenMode) {
            if (GetOpenFileName(&ofn) == TRUE)
            {
                FileName = ofn.lpstrFile;
                Result = 0x000001;
            }
            else {
                Result = 0x000000;
            }
        }
        else {
            if (GetSaveFileName(&ofn) == TRUE)
            {
                FileName = ofn.lpstrFile;
                Result = 0x000001;
            }
            else {
                Result = 0x000000;
            }
        }

    }
    void ShowPath() {
        MessageBox(NULL, FileName, L"Path", MB_OK);
    }
};

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MainProc(hWnd, message, wParam, lParam);
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
        setBaseFont(hWnd);
        DefWindowProc(hWnd, message, wParam, lParam);
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.

class ListBox : public Control {
public:
    void Create() {
        win = CreateWindow(WC_LISTBOX, L"", WS_BORDER | WS_VISIBLE | WS_CHILD, 20, 20, 170, 350, Parent, (HMENU)CreateMenu(), (HINSTANCE)NULL, NULL);
    }
    void Add(LPCWSTR p) {
        SendMessage(win, LB_INSERTSTRING, 0, (LPARAM)p);
    }
    void Reset() {
        SendMessage(win, LB_RESETCONTENT, 0, 0);
    }
    void DelStr(int index) {
        SendMessage(win,LB_DELETESTRING, index, 0);
    }
    void Show() {
        Create();
    }
};

class CheckedListBox : public ListBox {
private:
    int crntTop = 10;
public:
    vector<CheckBox> Boxes;
    void Add(CheckBox p) {
        Boxes.push_back(p);
        p.Parent = win;
        p.Show();
    }
    void Add(LPCWSTR Text) {
        CheckBox t;
        t.Text = Text;
        t.Parent = (this->win);
        t.Top = crntTop;
        t.Show();
        crntTop += 30;
        Boxes.push_back(t);
    }
};

class BtnListBox : public ListBox {
private:
    int crntTop = 10;
public:
    vector<Button> Buttons;
    void Add(Button p) {
        Buttons.push_back(p);
        p.Parent = win;
        p.Show();
    }
    void Add(LPCWSTR Text) {
        Button t;
        t.Text = Text;
        t.Parent = (this->win);
        t.Top = crntTop;
        t.Show();
        crntTop += 30;
        Buttons.push_back(t);
    }
    ~BtnListBox() {
        free(&Buttons);
    }
};

class EvHandler {
public:
    HANDLE Handle;
    void Create() {
        Handle = CreateEvent(
            NULL,               // default security attributes
            TRUE,               // manual-reset event
            FALSE,              // initial state is nonsignaled
            TEXT("WriteEvent")  // object name
        );
    }
    void Close() {
        CloseHandle(Handle);
    }
};

class BoundRect {
public:
    RECT cont;
    void getFrom(HWND x) {
        GetWindowRect(x, &cont);
    }
    void Inflate(int dx, int dy) {
        InflateRect(&cont, dx, dy);
    }
    void Deflate(int dx, int dy) {
        Inflate(-dx, -dy);
    }
    ~BoundRect() {
        free(&cont);
    }
};
#define inoptINST _In_opt_ HINSTANCE 
typedef LPCWSTR WConStr;
typedef LPWSTR WStr;
class MenuItem {
public:
    UINT type = MF_STRING;
    LPCWSTR value = L"";
    UINT_PTR id;
    MenuItem(LPCWSTR l) {
        value = l;
    }
};

class StatusBar {
public:
    LPCWSTR Data = L"";
    HWND win = NULL;
    HWND Parent = NULL;
    UINT id = 9000;
    void Show() {
        win = CreateWindowEx(
            0, 
            STATUSCLASSNAME,     
            (PCTSTR)Data,
            SBARS_SIZEGRIP |         
            WS_CHILD | WS_VISIBLE,  
            0, 0, 0, 0, 
            Parent,    
            (HMENU)CreateMenu(), 
            (HINSTANCE)NULL,      
            NULL);   

        UpdateWindow(Parent);
    }
};

void CloseWinMs(HWND win) {
    if (MessageBox(NULL, L"Are you sure you want to quit?", L"My App0", MB_OKCANCEL) == IDOK)
    {
        DestroyWindow(win);
    }
}
void SetFont(HWND h, HFONT font) {
    SendMessage(h, WM_SETFONT, (LPARAM)font, true);
}

class Menu {
public:
    HWND Parent = NULL;
    HMENU comp = CreateMenu();
    MENUINFO info;
    MENUBARINFO barInfo;
    UINT lastID = 1001;
    void AddTo(HWND s) {
        SetMenu(s, comp);
        Parent = s;
    }
    void AddTo() {
        SetMenu(Parent, comp);
    }
    void GetFrom(HWND hWnd) {
        comp = GetMenu(hWnd);
    }
    void GetInfo() {
        GetMenuInfo(comp, &info);
    }
    void GetBarInfo() {

    }
    ULONG_PTR GetData() {
        GetInfo();
        return info.dwMenuData;
    }
    void Add(UINT type, UINT_PTR ID, LPCWSTR Data) {
        AppendMenu(comp, type, ID, Data);
    }
    void Add(MenuItem Item) {
        AppendMenu(comp, Item.type, Item.id, Item.value);
    }
};

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTIC));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


INT_PTR CALLBACK DefaultDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_INITDIALOG:
        return TRUE;
        break;  //Don't forget about the break;
    case WM_COMMAND:
        switch (wParam)
        {
        case IDOK:
            EndDialog(hWndDlg, 0);
            return TRUE;
        }
        break;
    }
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND* n)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    *n = hWnd;
    return TRUE;
}

class ErrorMsg {
public:
    static void Show(LPCWSTR j) {
        MessageBox(NULL, j, L"Error", MB_OK | MB_ICONERROR);
    }
};

class ErrorProvider : public Control {
public:
    Icon i;
    HWND win;
    ErrorProvider() {
        
    }
    void Show() {
        win = CreateWindowW(L"STATIC",L"Error:", WS_VISIBLE| WS_BORDER | BS_CENTER,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,nullptr,nullptr,nullptr,nullptr);
        i.Load(nullptr, 32513);
        i.Draw(win, 10, 10);
    }
};

class Separator : public Control {
public:
    void AddTo(Menu k) {
        k.lastID++;
        AppendMenu(k.comp, MF_DISABLED | MF_STRING, k.lastID, L"|");
    }
};

class TextGrid : public Control{
private:
    int crntLeft;
    int crntTop;
public:
    int Rows;
    int Cols;
    TextGrid(int a = 5,int b = 5) {
        Rows = a;
        Cols = b;
    }
    void Show() {
        crntLeft = Left;
        crntTop = Top;
        for (int a = 0;a < Rows;a++) {
            for (int b = 0;b < Cols;b++) {
                TextBox p;
                crntLeft += 73;
                p.Parent = Parent;
                p.Left = crntLeft;
                p.Top = crntTop;
                p.Show();

            }
            crntTop += 25;
            crntLeft = Left;
        }
    }
};

class ButtonGrid : public Control {
protected:
    int crntLeft;
    int crntTop;
public:
    int Rows;
    int Cols;
    ButtonGrid(int a = 5, int b = 5){
        Rows = a;
        Cols = b;
    }
    void Show() {
        crntLeft = Left;
        crntTop = Top;
        for (int a = 0; a < Rows; a++) {
            for (int b = 0; b < Cols; b++) {
                Button p;
                crntLeft += 73;
                p.Parent = Parent;
                p.Left = crntLeft;
                p.Top = crntTop;
                p.Show();

            }
            crntTop += 25;
            crntLeft = Left;
        }
    }
};

class SqButtonGrid : public ButtonGrid {
public:
    void Show() {
        crntLeft = Left;
        crntTop = Top;
        for (int a = 0; a < Rows; a++) {
            for (int b = 0; b < Cols; b++) {
                Button p;
                crntLeft += 50;
                p.Width = 50;
                p.Height = 50;
                p.Parent = Parent;
                p.Left = crntLeft;
                p.Top = crntTop;
                p.Show();

            }
            crntTop += 50;
            crntLeft = Left;
        }
    }

};


enum HTMLCursor
{
    Cursor_Default = (UINT)IDC_ARROW,
    Cursor_Link = (UINT)IDC_HAND,
    Cursor_Text
};

class Column : public Control {
private:
    int crntLeft;
    int crntTop;
public:
    vector<LPCWSTR> Data;
    int Cols;
    Column(int controls) {
        Cols = controls;
    }
    void Show() {
        crntLeft = Left;
        for (int b = 0;b < Cols;b++) {
            Label p;
            p.BorderActive = true;
            p.Text = Data[b];
            p.Parent = Parent;
            p.Left = crntLeft;
            crntLeft += 100;
            p.Top = Top;
            p.Show();
        }
    }
};

void OnContextMenu(HWND x, UINT Message, LPARAM l, WPARAM w,function<void(HWND,UINT,LPARAM,WPARAM)> t) {
    if (Message == WM_CONTEXTMENU) {
        t(x, Message, l, w);
    }
}

void OnMouseClick(HWND x, UINT Message, LPARAM l, WPARAM w,function<void(HWND, UINT, LPARAM, WPARAM)> t) {
    if (Message == WM_RBUTTONDOWN || Message == WM_LBUTTONDOWN) {
        t(x, Message, l, w);
    }
}

class ScrollList : public Control {

};

BOOL IsASCII(LPVOID k) {
   return IsTextUnicode(k, sizeof(k), (LPINT)IS_TEXT_UNICODE_ASCII16);
}

class DataGrid : public Control {
public:
    void Show() {
        win =  CreateWindow(WC_LISTVIEW, L"Column",
            WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_ALIGNMASK,
            10, 10, 100, 100,
            Parent, nullptr, nullptr, 0);
    }
};

HWND InitForm(HINSTANCE hInstance,int nCmdShow) {
    HWND thisForm;
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow,&thisForm))
    {
        return NULL;
    }
    return thisForm;
}

#define MainEvent CALLINT MainProc
void InitAll(HWND& h, RECT& x, HRGN rct, HINSTANCE hInstance, int nCmdShow) {
    h = InitForm(hInstance, nCmdShow);
    GetWindowRect(h, &x);
    GetWindowRgn(h, rct);
}

void InitWin(HINSTANCE hPrevInstance, HINSTANCE hInstance, int nCmdShow, LPWSTR lpCmdLine, HWND& tForm, RECT& tRect, HBRUSH& tBrush, HRGN& tHRect) {
    UnRefParam(hPrevInstance, lpCmdLine);
    InitAll(tForm, tRect, tHRect, hInstance, nCmdShow);
    setToCrnBrush(tBrush);
}

void InitWin(HINSTANCE hPrevInstance, HINSTANCE hInstance, int nCmdShow, LPWSTR lpCmdLine, HWND& tForm, RECT& tRect, HRGN& tHRect) {
    UnRefParam(hPrevInstance, lpCmdLine);
    InitAll(tForm, tRect, tHRect, hInstance, nCmdShow);
}

WNDCLASSEX setBrush(WNDCLASSEX n,COLORREF ncol) {
    n.hbrBackground = (HBRUSH)ncol;
    return n;
}

void newDialog(DLGPROC x = DefaultDlgProc, HINSTANCE hInst = (HINSTANCE)0, UINT Value = IDD_ABOUTBOX, HWND Parent = NULL) {
    LPCSTR k = MAKEINTRESOURCEA(Value);
    DialogBoxA(hInst, k, Parent, x);
}

class ProgressBar : public Control {
public:
    int value = 0;
    HWND win = NULL;
    void Show() {
        win = CreateWindowEx(
            0,
            PROGRESS_CLASS,
            (LPCWSTR)NULL,
            WS_VISIBLE | WS_CHILD | PBS_SMOOTH,
            Left,
            Top,
            200,
            20,
            Parent,
            (HMENU)IDPB_PROGRESS_BAR,
            (HINSTANCE)GetWindowLong(Parent, GWL_HINSTANCE),
            NULL);
        SendMessageW(win, PBM_SETBARCOLOR, 0, (LPARAM)RGB(5,210,41));
    }
    void setVal(int r) {
        value = r;
        SendMessage(win, PBM_SETPOS, r, 0);
    }
    void Increment(int rate = 1,int times = 1) {
        for (int a = 0;a < times;a++) {
            value += rate;
            setVal(value);
            Wait(1);
        }
    }
};

class CommandLink : public Control {
public:
    LPCWSTR Text = L"Command";
    LPCWSTR Note = L"Note";
    HWND win;
    void Show() {
       win = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed
            L"",        // Text will be defined later
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_COMMANDLINK,  // Styles
            200,        // x position 
            10,         // y position 
            120,        // Button width
            50,        // Button height
            Parent,     // Parent window
            NULL,       // No menu
            (HINSTANCE)GetWindowLongPtr(Parent, GWLP_HINSTANCE),
            NULL);
       SendMessage(win, WM_SETTEXT, 0, (LPARAM)Text);
       SendMessage(win, BCM_SETNOTE, 0, (LPARAM)Note);
    }
};

void LoadHTML(UINT value, LPCWSTR name) {
    MAKEINTRESOURCEW(name);
}

POINT CursorPos() {
    POINT k;
    GetPhysicalCursorPos(&k);
    return k;
}

void LoadIconFile(HWND h) {
    FileDialog n;
    n.Show();
    Icon j;
    j.LoadImg(n.FileName);
    j.Draw();
}

class RichBox : public Control {
public:
    HWND win = NULL;
    LPCWSTR Text;
    void Show() {
        LoadLibrary(L"riched20.dll");
        win = CreateWindowEx(
            ES_SUNKEN,
            RICHEDIT_CLASS,
            Text,
            WS_BORDER | WS_VISIBLE | WS_CHILD | ES_MULTILINE,
            0, 40, 300, 300,
            Parent, 0, nullptr, NULL);
    }
};

typedef vector<CheckBox> CheckList;
typedef vector<Button> ButtonList;
typedef vector<ComboBox> ComboList;
typedef vector<Label> LabelList;
typedef vector<RichBox> RichBoxList;
typedef vector<ProgressBar> ProgressList;

void Handle(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    y(hWnd, message, x);
}

void Handle(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, LPARAM, WPARAM)> y) {
    y(hWnd, message, x.l, x.w);
}

void OnClose(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == WM_CLOSE) {
        y(hWnd, message, x);
    }
}

void OnActivApp(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == WM_ACTIVATEAPP) {
        y(hWnd, message, x);
    }
}

void OnMove(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == WM_MOVE) {
        y(hWnd, message, x);
    }
}

void OnSize(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == WM_SIZING) {
        y(hWnd, message, x);
    }
}
void OnCommand(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == WM_COMMAND) {
        y(hWnd, message, x);
    }
}

void OnDropFile(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == WM_DROPFILES) {
        y(hWnd, message, x);
    }
}

void OnListBoxAdd(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == LB_ADDSTRING) {
        y(hWnd, message, x);
    }
}

void OnComboBoxAdd(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == CB_ADDSTRING) {
        y(hWnd, message, x);
    }
}

typedef LPVOID voidptr;

void OnListBoxSelect(HWND hWnd, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (message == LBN_DBLCLK || message == LBN_SELCHANGE) {
        y(hWnd, message, x);
    }
}

void OnCommandReq(HWND hWnd, HWND needed, UINT message, EventArgs x, function<void(HWND, UINT, EventArgs)> y) {
    if (hWnd == needed) {
        if (message == WM_COMMAND) {
            y(hWnd, message, x);
        }
    }
}
WNDCLASSEX setBrush(WNDCLASSEX t, int ColorCode) {
    t.hbrBackground = getBrush(ColorCode);
    return t;
}

WNDCLASSEX setCursor(WNDCLASSEX t, LPWSTR style) {
    t.hCursor = LoadCursor(nullptr, style);
    return t;
}

WNDCLASSEX setSize(WNDCLASSEX t) {
    t.cbSize = sizeof(WNDCLASSEX);
    return t;
}
WNDCLASSEX setIcon(WNDCLASSEX t) {
    t.hIcon = LoadIcon((HINSTANCE)NULL, MAKEINTRESOURCE(IDI_LASTCONTROLS));
    return t;
}

Control createControl(string type) {
    if (type == "Button") {
        Button n;
        return n;
    }
    if (type == "TextBox") {
        TextBox k;
        return k;
    }
    Control j;
    return j;
}

#define CompList map<string,HWND>

class Desktop {
private:
    HDESK d;
public:

};

typedef PROCESS_INFORMATION_CLASS ProcInfo;

class Task {
public:
    HANDLE Proc;
    DWORD ID;
    void GetCrnt() {
        Proc = GetCurrentProcess();
        ID = GetCurrentProcessId();
    }
    void GetInfo(ProcInfo Type,LPVOID value) {
        GetProcessInformation(Proc,Type,value,1024);
    }
};

class Slider : public Control {
public:
    int Top = 10;
    int Left = 10;
    void Show() {
        win = CreateWindowEx(
            0,                               // no extended styles 
            TRACKBAR_CLASS,                  // class name 
            L"",              // title (caption) 
            WS_CHILD |
            WS_VISIBLE |
            TBS_AUTOTICKS |
            TBS_ENABLESELRANGE,              // style 
            Left, Top,                          // position 
            180, 23,                         // size 
            Parent,                         // parent window 
            CreateMenu(),                     // control identifier 
            NULL,                         // instance 
            NULL                             // no WM_CREATE parameter 
        );
    }
};

class SliderList : public ListBox {
private:
    int crntTop = 10;
public:
    void Add() {
        Slider t;
        t.Parent = win;
        t.Top = crntTop;
        t.Show();
        crntTop += 20;
    }
};

class Form : public Control{
public:
    LPCWSTR Title = L"New Form";
    int Width = 500;
    int Height = 300;
    void Show() {
        win = CreateWindowEx(
            ES_CENTER,
            szWindowClass,
            Title,
            WS_BORDER | WS_VISIBLE ,
            Left, Top, Width, Height,
            NULL, 0, nullptr, NULL);
    }
    Form(){}
    ~Form() {
        DestroyWindow(win);
    }
    Form(int Wid, int Heig) {
        Width = Wid;
        Height = Heig;
    }
    void operator =(Form* p) {
        Width = p->Width;
        Height = p->Height;
        try{
          free(p);
        }catch(...){
          p = NULL;
        }
    }
    void AddControls(vector<Control> c) {
        Show();
        for(Control k : c) {
            k.Parent = win;
            k.Show();
        }
    }
    void AddNShow(Control k) {
        Show();
        k.Parent = win;
        k.Show();
    }
    void Move(int x, int y) {
        MoveWindow(win, x, y, Width, Height, TRUE);
    }
};

class WinFrame : public Form {
public:
    WinFrame(LPCWSTR Text, POINT p, Size s) {
        Title = Text;
        Width = s.Width;
        Height = s.Height;
        Top = p.y;
        Left = p.x;
    }
};

class ScrollWin : public Control{
public:
    LPCWSTR Title = L"New Form";
    int Width = 500;
    int Height = 300;
    void Show() {
        win =  CreateWindowEx(
            ES_CENTER,
            szWindowClass,
            Title,
            WS_BORDER | WS_VISIBLE,
            Left, Top, Width, Height,
            NULL, 0, nullptr, NULL);
        ScrollBar t;
        t.Parent = win;
        t.Show();
    }
};

class CheckGroup : public GroupBox {
private:
    int crntTop = 10;
public:
    vector<CheckBox> Boxes;
    void Add(LPCWSTR Text) {
        CheckBox f;
        f.Text = Text;
        f.Top = crntTop;
        f.Parent = win;
        f.Show();
        crntTop += 20;
    }
};
#define WStr(x) (LPWSTR)TEXT(x)

class HtmlWin : public Control {

};

WNDCLASSEXW setWinClass(HINSTANCE hInstance, UINT Style = CS_HREDRAW | CS_VREDRAW, WNDPROC process = WndProc, LPCWSTR name = szWindowClass, LPWSTR cursorStyle = IDC_ARROW, int ColStyle = 0) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = Style;
    wcex.lpfnWndProc = process;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LASTCONTROLS));
    wcex.hCursor = LoadCursor(nullptr, cursorStyle);
    wcex.hbrBackground = getBrush(ColStyle);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LASTCONTROLS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return wcex;
}

class Application {
private:
  void* MainWin;
public:
  void setMainWin(void* p){
    MainWin = p;
  }
};

class MainWindow : public Form {
public:
  MainWindow(){}
  MainWindow(Application i){
    i.setMainWin(this);
  }
};


#define AddComp(Message,hWnd,x,name) if(Message == WM_CREATE){ x[name] = hWnd; }

class TextBoxList : public ListBox {
public:
    int Width = 180;
    vector<TextBox> Boxes;
    void Add(LPCWSTR Text) {
        TextBox y;
        y.Text = Text;
        y.Parent = win;
        y.Width = Width - 40;
        y.Show();
        y.setLimit(50);
        Boxes.push_back(y);
    }
};

void DrawRect(HWND h,RECT f) {
    Rectangle(GetDC(h), f.left, f.top, f.right, f.bottom);
}

RECT DefRect(int top,int bottom,int left,int right) {
    RECT k = { 0 };
    k.top = top;
    k.bottom = bottom;
    k.left = left;
    k.right = right;
    return k;
}

void DefRect(int top, int bottom, int left, int right,RECT& n) {
    RECT k = { 0 };
    k.top = top;
    k.bottom = bottom;
    k.left = left;
    k.right = right;
    n = k;
}
#define InvCol(r,g,b) RGB(255-r,255-g,255-b)

void Redraw(RECT n, HWND h, UINT Message) {
    if (Message == WM_SIZE || Message == WM_MOVING) {
        DrawRect(h, n);
    }
}

class Canvas : public Control{
private:
    HDC Context;
public:
    void GetFrom(HWND r) {
        Context = GetDC(r);
    }
};

