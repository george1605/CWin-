#ifdef _WIN32_ 
  #include "Windows/CWin++.h"
  #define _MAIN WinMain
#elif _WIN64_
  #include "Windows/CWin++.h"
  #define _MAIN WinMain
#else
   #include "Qt/version 1/CWin++.h"
   #define _MAIN int main()
#endif
