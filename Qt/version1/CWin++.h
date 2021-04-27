#ifndef CWIN_H
#define CWIN_H

#include "Base.h"
void MoveCWin(QWindow x,int top,int left){
  x.position().setX(left);
  x.position().setY(top);
}

void MoveCWin(QWindow x,int top,int left,int X,int Y){
  x.position().setX(left);
  x.position().setY(top);
  x.size().setHeight(Y);
  x.size().setWidth(X);
}

class LayoutViewer {

};

class HTMLViewer {
  
};

class ColorDialog {
  
};

class FileDialog {
  
};

class OpenFileDialog : FileDialog {
  
};


class PaintArgs {
public:
  int                 Flags;
  const RECT          *prcExclude;
  const BLENDFUNCTION *pBlendFunction;
  PaintArgs(){}
};

#endif // CWIN_H
