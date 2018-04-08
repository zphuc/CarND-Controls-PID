#include "PID.h"

//#include <iostream>
//#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

/*
* TODO: Complete the PID class.
*/

#define nP 3
#define iP 0 //index of Kp
#define iI 1 //index of Ki
#define iD 2 //index of Kd

//PID::PID() {}
PID::PID():
fname_(""),
nst_(1),
nen_(24000),
nn_(2000),
iX_(0),
mX_(1),
ic_(1),
p_({0,0,0}),
dp_({0,0,0}),
cte_({0,0,0}),
v0_(0.3),
err_(0.0),
best_err_(1.0e+10),
flagW_(false) 
{}

void PID::Init(double v0, double Kp, double Ki, double Kd) {
  
  flagW_ = false; // no tunning
  v0_ = v0;
  p_[iP] = Kp; 
  p_[iI] = Ki; 
  p_[iD] = Kd; 

  printf("%f %f %f %f\n",v0_,p_[iP],p_[iI],p_[iD]);
  printf("Running------\n");
}


void PID::Init(const char* fname)
{
  flagW_ = true; // do tunning
  strcpy(fname_,fname);

  FILE *fp;
  fp = fopen(fname_,"r");

  char tmp[100];
  fscanf(fp,"%s",tmp);  //skip 1st line
  printf("%s\n",tmp);

  double err;
  while (
     fscanf(fp,"%lf %d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",
                 &v0_,
                 &iX_,&mX_,
                 &p_[iP],&p_[iI],&p_[iD],
                 &dp_[iP],&dp_[iI],&dp_[iD],
                 &best_err_,&err) != EOF ) {
     printf("%f %f %f %f\n",v0_,p_[iP],p_[iI],p_[iD]);
  }

  fclose(fp);
 
  mX_++;
  if (mX_> 2) {
    iX_ = ((iX_+1) % nP);
    mX_ = 1;
  }

  printf("Tunning with twiddle------\n");

}

PID::~PID() {}
//////////////////////////////////////////////////////////////
void PID::UpdateError(double cte) {

  if (ic_> 1) { 
    cte_[iD] = cte - cte_[iP];
  } else {
    cte_[iD] = 0.0;
  }
  cte_[iP] = cte;
  cte_[iI] += cte; 

  if (flagW_ && (ic_>= nst_) && (ic_ < nen_)) {
    err_ += pow(cte, 2);
  }

  ic_++;
}

//////////////////////////////////////////////////////////////
double PID::TotalError() {

  //Kp*p_error + Ki*i_error + Kd*d_error;
  double terr = 0.0;
  for (int i=0;i<nP;i++) {
     terr -= p_[i]*cte_[i];
  }

  int ii = ic_% nn_;

  if (flagW_ && (ic_<= nen_) && (ic_>nst_) && (ii==0)) {
    err_ /= (nn_);

    if (err_ < best_err_) {
       best_err_ = err_;
       dp_[iX_]  *= 1.1;
    } else {
       if (mX_==1) {
          p_[iX_] -= 2* dp_[iX_];
       } else {
          p_[iX_] += dp_[iX_];
          dp_[iX_] *= 0.9;
       }
    }

    FILE *fp;
    fp = fopen(fname_,"a");
    
    fprintf(fp,"%5.3f %d %d %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.5f %6.5f\n",
                 v0_,
                 iX_,mX_,
                 p_[iP],p_[iI],p_[iD],
                 dp_[iP],dp_[iI],dp_[iD],
                 best_err_,err_); 
    fclose(fp);

    err_ = 0.0;
    mX_++;
    if (mX_> 2) {
      iX_ = ((iX_+1) % nP);
      mX_ = 1;
    }

    while (dp_[iX_]==0) {
       mX_++;
       if (mX_> 2) {
          iX_ = ((iX_+1) % nP);
          mX_ = 1;
       }
    }
  }

  return (v0_ + terr);

} 

int PID::getIC() {
   return ic_;
}



#undef nP
#undef iP
#undef iI
#undef iD
