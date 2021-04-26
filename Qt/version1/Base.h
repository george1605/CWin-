#ifndef BASE_H
#define BASE_H

#ifndef USE_WIN
  #define USE_WIN 0x0002
#endif

#include <QWindow>
#include <QMainWindow>
#include <QColor>
#include <Qt3DCore/QComponent>
#include <QPushButton>
#include <QAction>
#include <QMessageBox>
#include <QSlider>
#include <QLabel>
#include <QFont>
#include <QCheckBox>
#include <QProgressBar>
#include <QPrintDialog> 

class PrintDialog : public QPrintDialog {
public:
   char* FileName;
   PrintDialog(){}
   void Show(){
     this->open(NULL,"");
   }
}

class Control : public Qt3DCore::QComponent {
public:
};

void Destroy(Control* j){
 free(j);
}

class ProgressBar : public QProgressBar{
public:
    void Show(){
        this->show();
    }
};

enum CheckState {
  Checked,
  Medium,
  NoCheck,
  Disabled
};

class CheckBox : public QCheckBox{
public:
    bool ThreeState = false;
    QWidget* Parent;
    CheckBox(QString j){
     this->setText(j);
    }
    CheckBox(){}
    CheckBox(CheckState u){
     switch(u){
      case CheckState::Disabled:

      break;
     }
    }
    CheckBox(QWidget* i){
     this->setParent(i);
     Parent = i;
    }
    CheckBox(bool y){
        ThreeState = y;
        this->setTristate(ThreeState);
    }
};

namespace MyFonts {
 const QFont serifFont_B("Times",10,QFont::Bold);
 const QFont serifFont_N("Times",10,QFont::Normal);
}

class Font : public QFont {
public:
    Font(){}
    Font(QFont y = MyFonts::serifFont_N){
      this->setWeight(y.weight());
      this->setFamily(y.family());
    }
};

class Action : public QAction {

};

class Label : public QLabel {
public:
    Label(){}
};

class Size : public QSize {
public:
    Size(int a,int b){
     this->setWidth(a);
     this->setHeight(b);
    }
};

void MsgBox(QString message,QString title){
  QMessageBox i(QMessageBox::Icon::Information,title,message);
  i.show();
}


class Button : public QPushButton {
public:
    Button(QString Text){
      this->setText(Text);
    }
    Button(int Top,int Left){
     this->pos() = QPoint(Left,Top);
    }
    Button(Size t){

    }
    void Attach(QWidget* p){
     this->parent()->setParent(p);

    }
};

class Color : public QColor {
public:
  QColor Aqua = QColor(0,255,255,255);
  QColor White = QColor(255,255,255,255);
  QColor Current;
  int R,G,B;
  Color FromArgb(int r,int g,int b){
    Color i;
    i.R = r;
    i.G = g;
    i.B = b;
    i.Current = QColor(r,g,b,255);
    return i;
  }
  Color FromQCol(QColor i){
   Color u;
   int r,g,b;
   u.Current = i;
   i.getRgb(&r,&g,&b,nullptr);
   u.R = r;
   u.G = g;
   u.B = b;
   return u;
  }
  Color(int r,int g,int b){
   R = r;
   G = g;
   B = b;
   Current = QColor(r,g,b);
  }
  Color(int HEX){

  }
  Color(){}
  QString ToString(){
   return "rgb(" + QString(R) + "," + QString(G) + "," + QString(B) + ")";
  }
};

class Window : public QWindow {
public:
    void Add(Control* i){
     if(i != nullptr){

     }
    }

};

class Slider : public QSlider{
public:
    void Show(){
     this->show();
    }
};

class MMainWindow : public QMainWindow {
public:
    MMainWindow(){}
    MMainWindow(QWidget * t){}
    ~MMainWindow(){}
    void SetBack(Color i){
     this->centralWidget()->setStyleSheet("MainWindow { color: " + i.ToString());
    }
};

void DefClickHandler(Control* i){
  i->setProperty("down",true);
}

void Paint(Color i,Control* j){

}

#endif // BASE_H
