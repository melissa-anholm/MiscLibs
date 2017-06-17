/* Author: Benjamin Fenker 2015 */
#ifndef INCLUDE_BB1_STRIP_H_
#define INCLUDE_BB1_STRIP_H_
#include <string>
#include <vector>
#include <TRandom.h>
using std::string;
using std::vector;

enum Detector {upper, lower, D_undef};
enum Plane {X, Y, P_undef};

class BB1Strip {
 private :
  int nn;                       /* ntuple number */
  int sn;                  /* strip number */
  double cal, dcal, res, dres;
  double energy;                /* keV */
  double denergy;               /* keV */
  unsigned int maxT;                  /* chan */
  double pos;
 public :
  BB1Strip();
  BB1Strip(int nn_, int sn_, double cal_, double dcal_, double res_,
           double dres_, double* t_, double pos);
  double t[6];                  /* threshold for SN .25, .5, .75, 1, 2, 0.17 */
  double CalcEnergy(double adc);
  double GetResolution() {return res;};
  double GetEnergy() {return energy;};
  double GetThresholdAtIndex(int i);
  int GetStripNumber() {return sn;};
  int GetNtupleNumber() {return nn;};
  double GetCalibration() {return cal;};
  double GetPos() {return pos;};
  void Print();
  void SetStripNumber(int s) {sn = s;};
  void SetNtupleNumber(int n) {nn = n;};
  void SetMaxT(unsigned int t) {maxT = t;}
  unsigned int GetMaxT() {return maxT;}
};

class BB1Detector {
 private:
  vector<BB1Strip> strip;
 public :


  BB1Detector();
  BB1Detector(string fname);
  vector<double> ApplyResolution(vector<double> adc, TRandom *r);
  vector<double> CalcEnergy(vector<double> adc);
  void SetMaxT(vector<unsigned int> tvec);
  vector<double> GetResolution();
  double GetPositionForStrip(int sn);
  BB1Strip GetStripByNtupleN(int nn) {return strip[nn];}
  BB1Strip GetStripByStripN(int sn);
  void SetTDiffSigWithFile(string fname);
  int GetNtupleNumberForStripN(int sn) {return GetStripByStripN(sn).GetNtupleNumber();}
  double tdiff_sig;
  Detector det;
  Plane pl;
};

class BB1Hit {
public :
  double xpos, ypos, energy;
  int nx, ny, smaxx, smaxy;  
  // smaxx, smaxy:  strip with max in x plane, strip with max in y plane.
  bool passE, passT, pass;
  BB1Hit();
};

class BB1Result {
public :
  BB1Hit hit;
  bool twoHits;
  BB1Hit secHit;

  BB1Result();
};

/* 50 keV default threshold */
BB1Result GetResult(BB1Detector xDet, BB1Detector yDet, int t_index,
                    double n_cut = 3.0);
bool CompareXY(double xE, double xR, double xT,
               double yE, double yR, double yT, double ncut, double tdiff_sig);

#endif
