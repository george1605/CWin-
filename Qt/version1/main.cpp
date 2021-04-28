#include "mainwindow.h"
#include "CWin++.h"
#include <QApplication>

int main(int argc, char *argv[])
{//or char** argv
    QApplication a(argc, argv);
    MainWindow w;
    w.setVisible(true);
    Button i("well...");
    i.Attach(&w);
    return a.exec();
}
