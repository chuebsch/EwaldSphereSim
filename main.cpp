// beispiele/qteditor/main.cpp
#include <QApplication>
#include <QTranslator>
#include "window.h"
int main(int argc, char *argv[])  {
   QApplication app(argc, argv);
   MyWindow *window = new MyWindow();
   window->show();
   return app.exec();
}

