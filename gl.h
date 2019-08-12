
#ifndef CUBEGL_H
#define CUBEGL_H
#include <QGLWidget>
#include <QtGlobal>
//#include <GL/glu.h>
#include <math.h>
struct V3 {
  double x//! x is the X coordinate 
	 ,y//! y is the Y coordinate 
	 ,z//! z is the Z coordinate
	 ;
//  int rc;
  inline V3( void ){}
  inline V3( const double& _x, const double& _y, const double& _z ) : 
  x(_x), y(_y), z(_z)//!< initializer
       	//,rc(0) 
  {
    ;
  }
  inline V3& operator *= ( const double& d ){
    x *= d;
    y *= d;
    z *= d;
    return *this;
  }//!< The *= operator to scale by a scalar
  inline V3& operator += ( const V3& v ){
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }//!< The += operator to add a V3  
  inline V3& operator += ( const double& v ){
    x += v;
    y += v;
    z += v;
    return *this;
  }//!< The += operator to add a scalar
};
inline V3 operator + ( const V3& v1, const V3& v2 ) {
  V3 t;
  t.x = v1.x + v2.x;
  t.y = v1.y + v2.y;
  t.z = v1.z + v2.z;
  return t;
}//!< The + operator to add two V3
inline V3 operator - ( const V3& v1, const V3& v2 ) {
  V3 t;
  t.x = v1.x - v2.x;
  t.y = v1.y - v2.y;
  t.z = v1.z - v2.z;
  return t;
}//!< The + operator to subtract two V3
inline V3 operator * ( const V3& v, const double& d ) {
  V3 t;
  t.x = v.x*d;
  t.y = v.y*d;
  t.z = v.z*d;
  return t;
}//!< The * to scale a V3
inline V3 operator * ( const double& d, const V3& v ) {
  V3 t;
  t.x = v.x*d;
  t.y = v.y*d;
  t.z = v.z*d;
  return t;
}//!< The * to scale a V3
inline V3 operator % ( const V3& v1, const V3& v2 ) {
  V3 t;
  t.x = v1.y*v2.z - v2.y*v1.z;
  t.y = v1.z*v2.x - v2.z*v1.x;
  t.z = v1.x*v2.y - v2.x*v1.y;
  return t;
}//!< The % operator the cross product of two V3
inline double operator * ( const V3& v1, const V3& v2 ) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}//!< The * operator the scalar product of two V3
inline double Norm( const V3& v ) {
  return v.x*v.x + v.y*v.y + v.z*v.z;
}//!< The squared lenght of a V3
inline double Distance( const V3& v1, const V3& v2 ) {
  return Norm(v1 - v2);
}//!< The squared distance between two V3
inline bool operator == (const V3& v1, const V3& v2 ) {
  //  return ((v1.x==v2.x)&&(v1.y==v2.y)&&(v1.z==v2.z));
  return (Distance(v1,v2)<0.001);
}
inline V3& Normalize( V3 v ) {
static V3 erg=V3(1,0,0);
  if (Norm(v))  erg= (v * (1.0/sqrt(Norm(v)))); 
  return erg; 
}
//! Matrix is a 3 x 3 Matrix with all needed operators
struct Matrix{
double m11, m21, m31, m12, m22, m32, m13, m23, m33;
 inline Matrix(void){}
 inline Matrix( const V3 &a, const V3 &b, const V3 &c):
	 m11(a.x), m21(b.x), m31(c.x),
	 m12(a.y), m22(b.y), m32(c.y),
	 m13(a.z), m23(b.z), m33(c.z){;}
 inline Matrix( const double& x11, const double& x21, const double& x31,
                const double& x12, const double& x22, const double& x32,
                const double& x13, const double& x23, const double& x33):
	 m11(x11), m21(x21), m31(x31),
	 m12(x12), m22(x22), m32(x32),
	 m13(x13), m23(x23), m33(x33){;}

};
 inline Matrix transponse (Matrix a){//transponse
    return Matrix(
		  a.m11, a.m12, a.m13,
		  a.m21, a.m22, a.m23,
		  a.m31, a.m32, a.m33);
 }
inline double determinant (Matrix a){
return a.m11*a.m22*a.m33 - a.m11*a.m23*a.m32 - a.m12*a.m21*a.m33 + a.m12*a.m23*a.m31 +a.m13*a.m21*a.m32 -a.m13*a.m22*a.m31;
}
inline Matrix inverse (Matrix A){
  double D = determinant(A);
  if (D==0) return A;
  D=1.0/D;
  return Matrix(
          D*(A.m22*A.m33-A.m23*A.m32),//x11
          D*(A.m13*A.m32-A.m12*A.m33),//x21
          D*(A.m21*A.m23-A.m13*A.m22),//x31
          D*(A.m23*A.m31-A.m21*A.m33),//x12
          D*(A.m11*A.m33-A.m13*A.m31),//x22
          D*(A.m13*A.m21-A.m11*A.m23),//x32
          D*(A.m21*A.m32-A.m22*A.m31),//x13
          D*(A.m12*A.m31-A.m11*A.m32),//x23
          D*(A.m11*A.m22-A.m12*A.m21)//x33
          );
}
 inline bool operator == (const Matrix &a,const Matrix &b){
     return ((a.m11 == b.m11)&&(a.m21 == b.m21)&&(a.m31 == b.m31)&&
     (a.m12 == b.m12)&&(a.m22 == b.m22)&&(a.m23 == b.m23)&&
     (a.m13 == b.m13)&&(a.m32 == b.m32)&&(a.m33 == b.m33));
 }
inline Matrix operator * (const Matrix &a,const Matrix &b){
  Matrix erg;
  erg.m11 = a.m11 * b.m11 + a.m21 * b.m12 + a.m31 * b.m13;
  erg.m21 = a.m11 * b.m21 + a.m21 * b.m22 + a.m31 * b.m23;
  erg.m31 = a.m11 * b.m31 + a.m21 * b.m32 + a.m31 * b.m33;

  erg.m12 = a.m12 * b.m11 + a.m22 * b.m12 + a.m32 * b.m13;
  erg.m22 = a.m12 * b.m21 + a.m22 * b.m22 + a.m32 * b.m23;
  erg.m32 = a.m12 * b.m31 + a.m22 * b.m32 + a.m32 * b.m33;

  erg.m13 = a.m13 * b.m11 + a.m23 * b.m12 + a.m33 * b.m13;
  erg.m23 = a.m13 * b.m21 + a.m23 * b.m22 + a.m33 * b.m23;
  erg.m33 = a.m13 * b.m31 + a.m23 * b.m32 + a.m33 * b.m33;
  return erg;
}
inline V3 operator * (const Matrix &a, const V3 &b){
  V3 erg;
  erg.x = a.m11*b.x + a.m21*b.y + a.m31*b.z;
  erg.y = a.m12*b.x + a.m22*b.y + a.m32*b.z;
  erg.z = a.m13*b.x + a.m23*b.y + a.m33*b.z;
  return erg;
}
inline V3 operator * (const V3 &a, const Matrix &b){
  V3 erg;
  erg.x = b.m11*a.x + b.m12*a.y + b.m13*a.z;
  erg.y = b.m21*a.x + b.m22*a.y + b.m23*a.z;
  erg.z = b.m31*a.x + b.m32*a.y + b.m33*a.z;
  return erg;
}
inline Matrix operator * (const Matrix &a,const double &b){
  Matrix erg;
  erg.m11 = a.m11*b;
  erg.m21 = a.m21*b;
  erg.m31 = a.m31*b;

  erg.m12 = a.m12*b;
  erg.m22 = a.m22*b;
  erg.m32 = a.m32*b;

  erg.m13 = a.m13*b;
  erg.m23 = a.m23*b;
  erg.m33 = a.m33*b;
  return erg;
}
struct Refl{
  V3 xyz,hkl;
  double lambda,dist;
};

class GL : public QGLWidget {
Q_OBJECT
public:
  GL(double om,double ch,double ph,QWidget *parent = 0);

  QFont myFont;
  double lambda,a,b,c,alpha,beta,gamma,V;
  double ra,rb,rc,ralpha,rbeta,rgama,rV;
  V3 A,B,C,RA,RB,RC;
  GLfloat MM[16];
  GLclampf bgCR, bgCG, bgCB, bgCA;
  GLclampf tCR, tCG, tCB, tCA;
//  QList<V3> HKL,XYZ;
  QList<Refl> R;
  int _win_width,_win_height;
  bool rezi,ewald,dirlat,rlat;
  bool SCANPHI,SCANOMEGA,LAU,pause;
  // void setMole(molekul m);
  void zoom(double speed);
  void rotY(double speed);
  void rotX(double speed);
  void moveX(double speed);
  void moveY(double speed);
  //  INP *xdinpC;
  // molekul mokl;
  bool recording;
public slots:
  void scanPhi(double start,double end);
  void scanOmega(double start,double end);
  void scanLambda(double start,double end);
  void toggl_rezi();
  void toggl_ewald();
  void toggl_dirlat();
  void toggl_rlat();
  void dolaue();
  void scanphi();

signals:
  void detect();  
protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  void keyPressEvent(QKeyEvent *event);
private:
  void mySphere(int bal);
  void draw();
  void cryrot();
  GLint vp[4];
  Matrix orient,rm;
  double chi,omega,phi,rbal;
  QPoint lastPos;
  double cosal, cosbe, cosga, singa, cphi, tau;
  bool centeric,centered;
  QList<Matrix> symmops;
  QList<V3> trans,HKL2;
  double winkel(V3 a, V3 b);
  bool decodeSymmCard(QString symmCard); 
  void applyLatticeCentro(int gitter);
  int absent(int h,int k, int l);
  void printm(Matrix m);
};
#endif

