#include "window.h"

MyWindow::MyWindow( QMainWindow *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)  {
  gl =new GL;
  gl->setMinimumSize(700,700);
  setCentralWidget(gl);
  gl->setFocus(Qt::OtherFocusReason);
  det =new Detector(this);
  det->open();
  connect(gl,SIGNAL(detect()),this,SLOT(updateDet()));
  QToolBar *tb=new QToolBar(this);
  QAction *a=tb->addAction("labels",this,SLOT(toggleLabels()));
  a->setCheckable(true);
  a->setChecked(det->labels);
  a=tb->addAction("pause",this,SLOT(pause()));
  a->setCheckable(true);
  a->setChecked(gl->pause);
  a=tb->addAction("lattice points",gl,SLOT(toggl_rezi()));
  a->setCheckable(true);
  a->setChecked(gl->rezi);
  a=tb->addAction("Ewald shpere",gl,SLOT(toggl_ewald()));
  a->setCheckable(true);
  a->setChecked(gl->ewald);
  a=tb->addAction("direct cell",gl,SLOT(toggl_dirlat()));
  a->setCheckable(true);
  a->setChecked(gl->dirlat);
  a=tb->addAction("recip. axes",gl,SLOT(toggl_rlat()));
  a->setCheckable(true);
  a->setChecked(gl->rlat);

  addToolBar(Qt::BottomToolBarArea,tb);
}
void MyWindow::updateDet(){
  printf("push\n");
  det->pushReflections(gl->R);
  det->update();
}

void MyWindow::toggleLabels(){
  det->labels=!det->labels;
//  det->pushReflections(gl->R);
  det->update();
}
void MyWindow::pause(){
  gl->pause=!gl->pause;
  qDebug()<<"pause"<<gl->pause;
  gl->updateGL();
  update();
}
