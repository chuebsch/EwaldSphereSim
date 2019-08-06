#include "detector.h"
Detector::Detector(QWidget * parent, Qt::WindowFlags f){
  scene= new QGraphicsScene(0,0,600,600);
  scene->setBackgroundBrush(QBrush(QColor("#e9e9e9")));
  scene->clear ();
  QVBoxLayout *lh = new QVBoxLayout(this);
  QGraphicsItem *itm;
  itm=scene->addRect(5,5,590,590,Qt::DashLine,QBrush(QColor("#e9e9e9")));
//  itm=scene->addLine(560,-560,560,560,QColor("#e9e9e9"));
  view = new QGraphicsView(scene,this);
  QToolBar *tb=new QToolBar(this);
  tb-> addAction(QIcon(":/zoom-in.png"),"Zoom +",this,SLOT(zoomin())); 
  tb-> addAction(QIcon(":/zoom-out.png"),"Zoom -",this,SLOT(zoomout()));
  lh->addWidget(view);
  lh->addWidget(tb);
  labels=false;
}
Detector::~Detector(){
  //tidy up some thing...
}
void Detector::pushReflections(QList<Refl> &r){
  QGraphicsItem *itm;
  qDebug()<<r.size();
  QStringList texts;
  V3 detn=V3( -100,0,0);
  double minl=1000.0,maxl=-1.0;
  for (int i=0; i<r.size();i++){
  minl=(r.at(i).lambda<minl)?r.at(i).lambda:minl;
  maxl=(r.at(i).lambda>maxl)?r.at(i).lambda:maxl;
  }
  bool laue=(minl!=maxl);
  scene->setBackgroundBrush(QBrush((laue)?QColor("#000000"):QColor("#e9e9e9")));
  scene->clear ();
  itm=scene->addRect(5,5,590,590,Qt::DashLine,QBrush((laue)?QColor("#000000"):QColor("#e9e9e9")));
  
  double sk=Norm(detn);
  for (int i=0; i<r.size();i++){
    if ((detn*r.at(i).xyz)<0)continue;
/*    printf("%g %g %g %12.6f %12.6f %12.6f %f->%f %f  %d\n",
        r.at(i).hkl.x,r.at(i).hkl.y,r.at(i).hkl.z,
        r.at(i).xyz.x,r.at(i).xyz.y,r.at(i).xyz.z,
        r.at(i).dist,
        exp(-1000*r.at(i).dist),
        r.at(i).lambda,
        labels
        );// */
    int alpha=(int)255*exp(-1000*r.at(i).dist);
    V3 xy=r.at(i).xyz*(sk/(detn*r.at(i).xyz));
//    printf("%12.6f %12.6f %12.6f\n",xy.x,xy.y,xy.z);
    QColor c=(!laue)?QColor("#0907e6"):Farbverlauf(r.at(i).lambda,minl,maxl);
    c.setAlpha(alpha);   
    itm=scene->addEllipse(300+xy.z,300+xy.y,1.5,1.5,QPen(Qt::NoPen),QBrush(c));
    if (labels){
      QString s=QString("%1 %2 %3").arg(r.at(i).hkl.x,0,'g').arg(r.at(i).hkl.y,0,'g').arg(r.at(i).hkl.z,0,'g');
      if (!texts.contains(s)){
        txt=scene->addText(s,QFont("Helvetica",4));
        txt->moveBy(300+xy.z,300+xy.y);
        if (laue) txt->setDefaultTextColor(QColor("#e9e9e9"));
        texts.append(s);
      }
    }
  }
  update();
  printf("pushed!\n");
}

QColor Detector::Farbverlauf(double wrt,double min,double max){
  if (min+0.001>=max)max+=0.002;
  double ff[4];
  int lauf=0;
  const double farbe[6][4]={{1.0,0.0,0.0,1.0},
    {1.0,1.0,0.0,1.0},
    {0.0,1.0,0.0,1.0},
    {0.0,1.0,1.0,1.0},
    {0.0,0.0,1.0,1.0},
    {1.0,0.0,1.0,1.0}};
  double nwrt=(wrt-min)/(max-min);
  nwrt=(nwrt>=1.0)?0.99999:nwrt;
  nwrt=(nwrt<=0.0)?0.00001:nwrt;
  lauf=(int (nwrt/0.2));
  nwrt-=(0.2*lauf);
  nwrt/=(0.2);
  ff[0]=(1.0-nwrt)*farbe[lauf][0]+farbe[lauf+1][0]*nwrt;
  ff[1]=(1.0-nwrt)*farbe[lauf][1]+farbe[lauf+1][1]*nwrt;
  ff[2]=(1.0-nwrt)*farbe[lauf][2]+farbe[lauf+1][2]*nwrt;
  ff[3]=1.0;  
  return QColor((int)(ff[0]*255.0),(int)(ff[1]*255.0),(int)(ff[2]*255.0),255);
}

void Detector::zoomin(){
  view->scale(1.05,1.05);
}

void Detector::zoomout(){
  view->scale(0.95,0.95);

}

