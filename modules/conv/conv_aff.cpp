#include "conv_aff.h"
#include "err/err.h"

// Diagonalize (N+1)xN matrix
#define AN(x,y) a[(x)+(y)*(N+1)]
int mdiag(int N, double *a){
  int i,j,k,l;
  double tmp;

  for (k=0; k<N; ++k){  // go from the top line to the bottom one
    // rotate lines k .. N-1 to have non-zero element on (k,k)
    for (i=k; i<N; ++i){ if (AN(k,k)!=0) break;
      for (j=0;j<N+1;++j){  // all columns
        tmp=AN(j,k);        // save upper values
        for (l=k; l<N-1; ++l) AN(j,l)=AN(j,l+1); // shift up all lines
        AN(j,N-1)=tmp; // move old upper value to the bottom line
      }
    }
    // If it is not possible to have non-zero (k,k) element, then
    // matrix can not be diagonilized
    if (AN(k,k)==0) return 1;
    // Divide the line k by (k,k) element (note thet 0..k-1 are zeros)
    for (j=N; j>=k; --j) AN(j,k)=AN(j,k)/AN(k,k);
    // Subtract this line from all others with a factor A(k,*)
    for (i=0; i<N; ++i) if (i!=k) for (j=N; j>=k; --j) AN(j,i)-=AN(k,i)*AN(j,k);
  }
  return 0;
}


void ConvAff2D::bck_recalc(){
  double D = det();
  if (D==0) throw Err() << "ConvAff2D: can't calculate matrix for backward conversion.";

  k_bck[0] =   k_frw[4] / D;
  k_bck[1] = - k_frw[1] / D;
  k_bck[2] = (k_frw[1] * k_frw[5] - k_frw[2] * k_frw[4]) / D;

  k_bck[3] = - k_frw[3] / D;
  k_bck[4] =   k_frw[0] / D;
  k_bck[5] = (- k_frw[0] * k_frw[5] + k_frw[3] * k_frw[2]) / D;
}

void
ConvAff2D::reset(){
  k_frw.resize(6);
  k_bck.resize(6);
  for (int i=0; i<6; i++) k_frw[i]=k_bck[i]=0;
  k_frw[0] = k_bck[0] = 1.0;
  k_frw[4] = k_bck[4] = 1.0;
}

void
ConvAff2D::reset(const std::map<dPoint, dPoint> & ref){
/*
Transformation is:
  a x + b y + c = X
  d x + e y + f = Y

To find least square fit we should solve these equations:
 d/d(a..f) SUMi(  a xi + b yi + c - Xi )^2 +
    SUMi(  d xi + e yi + f - Yi )^2 = 0

 a Sxx + b Sxy + c Sx - SXx = 0
 a Sxy + b Syy + c Sy - SXy = 0
 a Sx  + b Sy  + c S  - SX  = 0
 d Sxx + e Sxy + f Sx - SYx = 0
 d Sxy + e Syy + f Sy - SYy = 0
 d Sx  + e Sy  + f S  - SY  = 0

 Sxx Sxy Sx 0   0   0  = SXx
 Sxy Syy Sy 0   0   0  = SXy
 Sx  Sy  S  0   0   0  = SX
 0   0   0  Sxx Sxy Sx = SYx
 0   0   0  Sxy Syy Sy = SYy
 0   0   0  Sx  Sy  S  = SY
*/

#define A7(x,y) a[(x)+(y)*7]
  double a[6*7];
  for (int i=0; i<6*7; i++) a[i]=0;

  std::map<dPoint, dPoint>::const_iterator pp;
  for (pp=ref.begin(); pp!=ref.end(); pp++){
    double x  = pp->first.x;
    double y  = pp->first.y;
    double xc = pp->second.x;
    double yc = pp->second.y;

    A7(0,0)+=x*x; A7(3,3)+=x*x;
    A7(1,0)+=x*y; A7(4,3)+=x*y;
    A7(2,0)+=x;   A7(5,3)+=x;
    A7(0,1)+=x*y; A7(3,4)+=x*y;
    A7(1,1)+=y*y; A7(4,4)+=y*y;
    A7(2,1)+=y;   A7(5,4)+=y;
    A7(0,2)+=x;   A7(3,5)+=x;
    A7(1,2)+=y;   A7(4,5)+=y;
    A7(2,2)+=1;   A7(5,5)+=1;
    A7(6,0)+=xc*x; A7(6,1)+=xc*y; A7(6,2)+=xc;
    A7(6,3)+=yc*x; A7(6,4)+=yc*y; A7(6,5)+=yc;
  }
  if (mdiag (6, a) != 0) throw Err() << "ConvAff2D: can't calculate conversion matrix.";

  k_frw.resize(6);
  k_bck.resize(6);
  for (int i=0; i<6; i++) k_frw[i] = A7(6,i);
  bck_recalc();
}

void
ConvAff2D::shift_src(const dPoint & p){
  k_frw[2] += p.x*k_frw[0] + p.y*k_frw[1];
  k_frw[5] += p.x*k_frw[3] + p.y*k_frw[4];
  bck_recalc();
}

void
ConvAff2D::shift_dst(const dPoint & p){
  k_frw[2] += p.x;
  k_frw[5] += p.y;
  bck_recalc();
}

void
ConvAff2D::rescale_src(const double kx, const double ky){
  k_frw[0]*=kx; k_frw[3]*=kx;
  k_frw[1]*=ky; k_frw[4]*=ky;
  bck_recalc();
}

void
ConvAff2D::rescale_dst(const double kx, const double ky){
  for (int i=0; i<3; i++) k_frw[i]*=kx;
  for (int i=3; i<6; i++) k_frw[i]*=ky;
  bck_recalc();
}
