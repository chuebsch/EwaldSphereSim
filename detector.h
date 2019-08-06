#include <QtGui>
#include "gl.h"
#include <QDialog>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsRectItem>
#include <QToolBar>
//#include <QtWidgets>

class Detector : public QDialog {
Q_OBJECT
  public:
  Detector(QWidget * parent = 0, Qt::WindowFlags f = 0 );
  ~Detector();
  bool labels;
//  public slots:
    void pushReflections(QList<Refl> &xyz);
public slots:
    void zoomin();
    void zoomout();
  private:
    QColor Farbverlauf(double wrt, double min, double max);
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsTextItem *txt;
};

