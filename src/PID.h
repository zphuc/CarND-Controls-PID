#ifndef PID_H
#define PID_H

#include <vector>

using namespace std;

class PID {

private:
  char fname_[100];
  int nst_;
  int nen_;
  int nn_;
  int iX_;
  int mX_;
  int ic_;
  std::vector<double> p_;
  std::vector<double> dp_;
  std::vector<double> cte_;

  double v0_;
  double err_;
  double best_err_;
  bool flagW_;

public:
  /*
  * Constructor
  */
  PID();
  /*
  * Initialize PID.
  */
  void Init(double v0,double Kp, double Ki, double Kd);
  void Init(const char* fname);

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
  int getIC();
};

#endif /* PID_H */
