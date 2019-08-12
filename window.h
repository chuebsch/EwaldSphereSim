
#ifndef Window_H
#define Window_H
#include <QtGui>
#include "gl.h"
#include "detector.h"
class MyWindow : public QMainWindow {
     Q_OBJECT
public:
     MyWindow( QMainWindow *parent = 0,
               Qt::WindowFlags flags = 0);
     GL *gl;
     Detector *det;

public slots:
void updateDet();
void toggleLabels();
void pause();

};
#endif
