#include "detector.h"
void printm(Matrix m){
  printf("Matrix:\n%12.6f%12.6f%12.6f\n%12.6f%12.6f%12.6f\n%12.6f%12.6f%12.6f\n\n",
      m.m11,m.m12,m.m13,m.m21,m.m22,m.m23,m.m31,m.m32,m.m33
      );
}
Detector::Detector(QWidget * parent, Qt::WindowFlags f):QDialog( parent, f){
  scene= new QGraphicsScene(0,0,600,600);
  scene->setBackgroundBrush(QBrush(QColor("#e9e9e9")));
  scene->clear ();
  setWindowTitle("Detector");
  QVBoxLayout *lh = new QVBoxLayout(this);
//  QGraphicsItem *itm;
//  itm=
    scene->addRect(5,5,590,590,QColor("#000000"),Qt::NoBrush);
//  itm=scene->addLine(560,-560,560,560,QColor("#e9e9e9"));
  view = new QGraphicsView(scene,this);
  QToolBar *tb=new QToolBar(this);
  tb-> addAction(QIcon(":/zoom-in.png"),"Zoom +",this,SLOT(zoomin())); 
  tb-> addAction(QIcon(":/zoom-out.png"),"Zoom -",this,SLOT(zoomout()));
  thbx=new QDoubleSpinBox(this);
  thbx->setMinimum(-180.0);
  thbx->setMaximum( 180.0);
  thbx->setValue(0.0);
  tb->addWidget(thbx);

  dist=new QDoubleSpinBox(this);
  dist->setMinimum(10.0);
  dist->setMaximum(280.0);
  dist->setValue(100.0);
  tb->addWidget(dist);
  lh->addWidget(view);
  lh->addWidget(tb);
  labels=false;
}
Detector::~Detector(){
  //tidy up some thing...
}
void Detector::pushReflections(QList<Refl> &r){
//  QGraphicsItem *itm;
 // qDebug()<<r.size();
  QStringList texts;
  V3 detn=V3( -dist->value(),0,0);
  V3 detNorth = V3 (0,0,-1);
  V3 detEast  = V3 (0, 1,0);
  double c=cos(thbx->value()/180.0*M_PI),s=sin(thbx->value()/180.0*M_PI);
  Matrix twothetamat=Matrix(c,0,s,0,1,0,-s,0,c);
  detn=detn*twothetamat;
  detNorth=detNorth*twothetamat;
  detEast=detEast*twothetamat;
  printm(twothetamat);

  double minl=1000.0,maxl=-1.0;
  for (int i=0; i<r.size();i++){
  minl=(r.at(i).lambda<minl)?r.at(i).lambda:minl;
  maxl=(r.at(i).lambda>maxl)?r.at(i).lambda:maxl;
  }
  bool laue=(minl!=maxl);
  scene->setBackgroundBrush(QBrush((laue)?QColor("#000000"):QColor("#e9e9e9")));
  scene->clear ();
  QPen pen;
  pen.setColor((laue)?QColor("#ffffff"):QColor("#000000"));
  scene->addRect(5,5,590,590,pen,Qt::NoBrush);
//  scene->addEllipse(150,150,300,300,pen,Qt::NoBrush);  
  double sk=sqrt(Norm(detn));
  V3 detnormal=detn*(1.0/sk);
  for (int i=0; i<r.size();i++){
    if ((detn*r.at(i).xyz)<0)continue;
  

    double th=acos(r.at(i).xyz*detnormal);
    V3 ri=Normalize(r.at(i).xyz%detnormal);
    double al=acos(detNorth*ri),be=acos(detEast*ri);
    al=(be>(M_PI/2.0))?al:-al;
    double dtx=sin(al)*tan(th)*sk;
    double dty=cos(al)*tan(th)*sk;
    printf("%f %f %f %f\n",dtx,dty,th,al);
/*   printf("%g %g %g %12.6f %12.6f %12.6f %f->%f %f  %d\n",
        r.at(i).hkl.x,r.at(i).hkl.y,r.at(i).hkl.z,
        r.at(i).xyz.x,r.at(i).xyz.y,r.at(i).xyz.z,
        r.at(i).dist,
        exp(-1000*r.at(i).dist),
        r.at(i).lambda,
        labels
        );// */
    int alpha=(int)255*exp(-1000*r.at(i).dist);
//    V3 xy=r.at(i).xyz*(sk/(detn*r.at(i).xyz));
//    printf("%12.6f %12.6f %12.6f  %g %g %g \n",xy.x,xy.y,xy.z,r.at(i).hkl.x,r.at(i).hkl.y,r.at(i).hkl.z);
    QColor c=(!laue)?QColor("#0907e6"):Farbverlauf(r.at(i).lambda,minl,maxl);
    c.setAlpha(alpha);   
//    itm=
      scene->addEllipse(300+dtx,300+dty,3.5,3.5,QPen(Qt::NoPen),QBrush(c));
    if (labels){
      QString s=QString("%1 %2 %3").arg(r.at(i).hkl.x,0,'g').arg(r.at(i).hkl.y,0,'g').arg(r.at(i).hkl.z,0,'g');
      if (!texts.contains(s)){
        txt=scene->addText(s,QFont("Helvetica",6));
        txt->moveBy(300+dtx,300+dty);
        if (laue) txt->setDefaultTextColor(QColor("#e9e9e9"));
        texts.append(s);
      }
    }
  }
  update();
  show();
  printf("pushed!\n");
}

QColor Detector::Farbverlauf(double wrt,double min,double max){
  if (min+0.001>=max)max+=0.002;
  double ff[4];
  int lauf=0;
  const double farbe[6][4]={
    {1.0,0.0,1.0,1.0},
    {0.0,0.0,1.0,1.0},
    {0.0,1.0,1.0,1.0},
    {0.0,1.0,0.0,1.0},
    {1.0,1.0,0.0,1.0},
    {1.0,0.0,0.0,1.0}
  };
  double nwrt=(wrt-min)/(max-min);
  nwrt=(nwrt>=1.0)?0.99999:nwrt;
  nwrt=(nwrt<=0.0)?0.00001:nwrt;
  lauf=(int (nwrt/0.2));
  nwrt-=(0.2*lauf);
  nwrt/=(0.2);
  ff[0]=(1.0-nwrt)*farbe[lauf][0]+farbe[lauf+1][0]*nwrt;
  ff[1]=(1.0-nwrt)*farbe[lauf][1]+farbe[lauf+1][1]*nwrt;
  ff[2]=(1.0-nwrt)*farbe[lauf][2]+farbe[lauf+1][2]*nwrt;
  return QColor((int)(ff[0]*255.0),(int)(ff[1]*255.0),(int)(ff[2]*255.0),255);
}

void Detector::zoomin(){
  view->scale(1.05,1.05);
}

void Detector::zoomout(){
  view->scale(0.95,0.95);

}

