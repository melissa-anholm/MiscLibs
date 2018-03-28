// Author: Benjamin Fenker 2015
#include <fstream>
#include <iostream>
#include <math.h>

#include <TFile.h>
#include <TRandom3.h>

#include "bb1_strip.h"

using std::cin;
using std::cout;
using std::endl;

string bb1_strip_version = "4.27.2";

BB1Result::BB1Result() {
  hit = BB1Hit();
  twoHits = false;
  secHit = BB1Hit();
}

BB1Strip::BB1Strip() {
  nn = 0;
  sn = 0;
  cal = 0.;
  dcal = 0.;
  res = 0.;
  dres = 0.;
  energy = 0.;
  denergy = 0.;
  maxT = 0;
  noise = 0;
  for (int i = 0; i < 5; i++) t[i] = 0.;
}

BB1Strip::BB1Strip(int nn_, int sn_, double cal_, double dcal_, double res_,
                   double dres_, double* t_, double pos_) {
  nn = nn_;
  sn = sn_;
  cal = cal_;
  dcal = dcal_;
  res = res_;
  dres = dres_;
  maxT = 0;
  pos = pos_;
  noise = 0;
  for (int i = 0; i < 5; i++) t[i] = 0.5*t_[i];
}

double BB1Strip::CalcEnergy(double adc) {
  if (cal > 0.) {
    energy = adc / cal;
    denergy  = (energy / cal) * dcal;
    return energy;
  } else return 0.0;
}

void BB1Strip::Print() {
  cout << "Strip number: " << sn << "\tNutuple number: " << nn << endl;
  cout << "Calibration: " << cal << " +/- " << dcal << " ch/keV" << endl;
  cout << "Resolution: " << res << " +/- " << dres << " keV" << endl;
  cout << "Thresholds for S/N: ";
  for (int i = 0; i < 4; i++) cout << t[i] << "   ";
  cout << " keV" << endl;
}

double BB1Strip::GetThresholdAtIndex(int i) {
  if (i > 4 || i < 0) {
    cout << "WARNING: Must supply index 0->4" << endl;
    cout << "Assuming you meant i = 4" << endl;
    i = 4;
  }
  return t[i];
  //  return 60.0;                          // keV
}


BB1Detector::BB1Detector() {
  double t[5] = {0, 0, 0, 0, 0};
  strip = vector<BB1Strip>(40, BB1Strip());
  det = D_undef;
  pl = P_undef;
  tdiff_sig = 0.0;
}

BB1Detector::BB1Detector(string fname) : det(D_undef), pl(P_undef) {
  cout << "Reading file " << fname << endl;
  cout << "Initializing BB1Detector version " << bb1_strip_version << endl;
  double t[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
  strip = vector<BB1Strip>(40, BB1Strip());
  for (unsigned int i = 0; i < 40; i++) strip[i].SetNtupleNumber(i);
  std::ifstream ifs(fname.c_str(), std::ifstream::in);
  if (!ifs.is_open()) {
    cout << "Error reading " << fname << endl;
    cout << "Enter any number to confirm" << endl;
    int j;
    cin >> j;
  }

  string line;
  vector<string> word;
  int sn, nn;
  double cal, dcal, res, dres;
  double pos;
  int count = 0;
  while (ifs >> nn >> sn >> pos >> cal >> dcal >> res >> dres >> t[0] >> t[1] >> t[2] >>
         t[3] >> t[4] && !ifs.eof() && ifs.good() && count < 40) {
    count++;
    strip[nn] = BB1Strip(nn, sn, cal, dcal, res, dres, t, pos);
    for (int i = 0; i < 5; i++) 
      {
        strip[nn].t[i] = t[i];

      }
    //    cout << "Strip " << nn << " cal = " << cal << endl;
    //    cout << t[2] << "\t" << strip[nn].t[2] << endl;
    
    //    strip[nn].Print();
  }
  cout << "Read " << count << " strips from " << fname << endl;
  ifs.close();
  double_hit_threshold = 40.;           // keV
}

void BB1Detector::SetTDiffSigWithFile(string fname) {
  std::ifstream ifs(fname.c_str(), std::ifstream::in);
  if (!ifs.is_open()) {
    cout << "Error reading " << fname << endl;
    cout << "Enter any number to confirm" << endl;
    int j;
    cin >> j;
  }
  double s;
  ifs >> tdiff_sig;
  cout << "Read tdiff_sig = " << tdiff_sig << endl;
  ifs.close();  
}

vector<double> BB1Detector::ApplyResolution(vector<double> adc, TRandom *r,
                                            bool doEmpirical) {
  // This function is always called for Geant only, therefore adc == energy
  vector<double> adc_with_res(adc.size(), 0.0);

  for (unsigned int i = 0; i < adc.size(); i++) {
    double orig = adc[i];
    double g = -1;
    
    int signedi = static_cast<int>(i);
    BB1Strip strip = GetStripByStripN(i);
    if (strip.GetNtupleNumber() != signedi && strip.GetCalibration() > 0.) {
      cout << "WARNING in ApplyResolution. Possible misalignment" << endl;
      cout << "Det = " << det << "\tPlane = " << pl << "\ti = " << i << "\tS = "
           << strip.GetStripNumber() << "\tN = " << strip.GetNtupleNumber()
           << endl;
    }

    TH1D *n = strip.GetNoise();

    if (doEmpirical && orig < 5.0 && n) { // E = 0, draw from noise
      //      cout << "Throwing empirical noise" << endl;
      g = n -> GetRandom();
    } else {                    //  E > 0, apply gaussian resolution
      // Since this is only used for Geant, Strip# == Ntuple# so either is okay
      double sigm = strip.GetResolution();
      
      if (sigm < 0.001) {
        g = 0.0;
      } else {
        while (g < 0) {
          g = r -> Gaus(orig, sigm);
        }
      }
    }

    adc_with_res[i] = g;
  }
  return adc_with_res;
}

vector<double> BB1Detector::CalcEnergy(vector<double> adc) {
  vector<double> energy(strip.size(), 0.0);
  if (adc.size() != strip.size()) {
    cout << "BB1Detector::CalcEnergy Size mismatch!" << endl;
  } else {
    for (unsigned int i = 0; i < adc.size(); i++) {
      energy[i] = strip[i].CalcEnergy(adc[i]); // Also sets strip energy
      // cout << "Ntuple #: " << strip[i].GetNtupleNumber() << "\tStrip #: "
      //      << strip[i].GetStripNumber() << "\tADC: " << adc[i] << "\tEnergy: "
      //      << energy[i] << endl;
    }
  }
  return energy;
}

void BB1Detector::SetMaxT(vector<unsigned int> tvec) {
  if (tvec.size() != 40) {
    cout << "BB1Detector::SetMaxT Size mismatch!" << endl;
  } else {
    for (unsigned int i = 0; i < tvec.size(); i++) {
      strip[i].SetMaxT(tvec[i]);
    }
  }
}

vector<double> BB1Detector::GetResolution() {
  vector<double> res = vector<double>(strip.size(), 0.0);
  for (unsigned int i = 0; i < res.size(); i++) {
    res[i] = strip[i].GetResolution();
  }
  return res;
}

double BB1Detector::GetPositionForStrip(int sn) {
  return GetStripByStripN(sn).GetPos();
  // double pos = 0.0;
  // if (det == upper && pl == Y) {
  //   pos = 19.5 - double(sn);
  // } else {
  //   pos = -19.5 + double(sn);
  // }
  // return pos;
}

BB1Strip BB1Detector::GetStripByStripN(int sn) {
  for (unsigned int i = 0; i < strip.size(); i++) {
    if (strip[i].GetStripNumber() == sn) return strip[i]; // Pass by value
  }
  //  cout << "Strip number " << sn << " not found!" << endl;
  return BB1Strip();
}

BB1Strip *BB1Detector::GetStripByStripN_ptr(int sn) {
  for (unsigned int i = 0; i < strip.size(); i++) {
    if (strip[i].GetStripNumber() == sn) return &(strip[i]); // Pas by reference
  }
  //  cout << "Strip number " << sn << " not found!" << endl;
  return new BB1Strip();
}

void BB1Detector::SetupNoiseFromFile(string fname) {
  //  TFile *nfile = new TFile(fname.c_str());
  TFile *nfile = TFile::Open(fname.c_str());
  string uORl = "";
  if (det == upper) {
    uORl = "U";
  } else if (det == lower) {
    uORl = "L";
  } else {
    cout << "Detector " << det << " not found" << endl;
  }
  string xORy = "";
  if (pl == X) {
    xORy = "X";
  } else if (pl == Y) {
    xORy = "Y";
  } else {
    cout << "Plane " << pl << " not found" << endl;
  }
    
  TH1D *noise_clone;
  for (int i = 0; i < 40; i++) {
    BB1Strip *strip = GetStripByStripN_ptr(i);

    char hname[1024];
    sprintf(hname, "%s%s_S%d_noise", uORl.c_str(), xORy.c_str(), i);
    cout << hname << endl;
    TH1D *noise = (TH1D*)nfile -> Get(hname);
    if (noise) {
      //      cout << noise << endl;
      noise_clone = (TH1D*)noise -> Clone();
      //      cout << noise_clone << endl;
      //      cout << "Got noise with name " << noise -> GetName() << endl;
      noise_clone -> SetDirectory(0);
      //      cout << "Strip at " << strip << " setting noise" << endl;
      strip -> SetNoise(noise_clone);
      //      cout << strip -> GetNoise() << endl;
      strip -> Print(); 
    } else {
      cout << "No noise" << endl; 
    }    
  }
  nfile -> Close();
}


BB1Hit::BB1Hit() : xpos(0.), ypos(0.), energy(0.), nx(0), ny(0), smaxx(0),
                   smaxy(0), passE(false), passT(false), pass(false) {}

BB1Result GetResult(BB1Detector xDet, BB1Detector yDet,
                    int t_index, double n_cut) {
  BB1Result r = BB1Result();
  BB1Hit h = r.hit;
  int verbose = 0;
  if (xDet.pl == yDet.pl) {
    cout << "BOTH PLANES ARE LABELLED " << xDet.pl << endl;
    cout << "POSITION WILL BE WRONG" << endl;
  }
  if (xDet.pl != X || yDet.pl != Y) {
    cout << "INCORRECT ORDERING OF PLANES (X, Y)" << endl;
    cout << "POSITION WILL BE WRONG" << endl;
  }
  if (xDet.det != yDet.det) {
    cout << "PLANES ARE NOT ON SAME DETECTOR" << endl;
    cout << "NONSENSE!" << endl;
  }
  if (fabs(xDet.tdiff_sig-yDet.tdiff_sig) > 1E-6) {
    cout << "PLANES DO NOT HAVE SAME TDIFF_SIG" << endl;
    cout << "WILL USE X-DIRECTION, BUT SOMETHING IS WRONG" << endl;
  }
  if (fabs(xDet.GetDoubleHitThreshold() -
           yDet.GetDoubleHitThreshold()) > 1E-6) {
    cout << "WARNING: PLANES DO NOT HAVE SAME X2 HIT THRESHOLD" << endl;
    cout << "WILL USE X-DIRECTION X2 HIT THRESHOLD = "
         << xDet.GetDoubleHitThreshold() << endl;
  }
  // vector<BB1Strip> x = xDet.strip;
  // vector<BB1Strip> y = yDet.strip;

  int nx = 0, ny = 0, smaxx = -1, smaxy = -1;
  double maxx = -1.0, maxy = -1.0;

  vector<int> xstrips(0, 0);
  vector<int> ystrips(0, 0);
  for (unsigned int i = 0; i < 40; i++) {
    BB1Strip x = xDet.GetStripByNtupleN(i);
    BB1Strip y = yDet.GetStripByNtupleN(i);
    
    if (x.GetNtupleNumber() != int(i)) {
      cout << "ERROR IN X STRIP NUMBERS" << endl;
    }
    if (y.GetNtupleNumber() != int(i)) {
      cout << "ERROR IN Y STRIP NUMBERS" << endl;
      cout << y.GetNtupleNumber() << " != " << i << endl;
    }

    if (x.GetThresholdAtIndex(t_index) > 0.) {
      if (x.GetEnergy() > x.GetThresholdAtIndex(t_index)) {
        nx++;
        xstrips.push_back(x.GetStripNumber());
        // cout << "Above threshold for n#: " << i << " ie s# "
        //      << x.GetStripNumber() << " E = " << x.GetEnergy()
        //      << " > " << x.GetThresholdAtIndex(t_index) << endl;
        
        
        
      }
      if (x.GetEnergy() > maxx) {
        maxx = x.GetEnergy();
        smaxx = x.GetStripNumber();
      }
    }

    if (y.GetThresholdAtIndex(t_index) > 0.) {
      if (y.GetEnergy() > y.GetThresholdAtIndex(t_index)) {
        ny++;
        ystrips.push_back(y.GetStripNumber());
      }
      if (y.GetEnergy() > maxy) {
        maxy = y.GetEnergy();
        smaxy = y.GetStripNumber();
      }
    }
  }
  h.nx = nx;
  h.ny = ny;
  h.smaxx = smaxx;
  h.smaxy = smaxy;
  if (nx == 0 || ny == 0) {
    h.energy = 0.;
    r.hit = h;
    return r;
  }
  vector<int> xstrips_sorted(0, 0.0);
  vector<int> ystrips_sorted(0, 0.0);
  do {
    double emax = 0.0;
    double zmax = 0;
    for (unsigned int z = 0; z < xstrips.size(); z++) {
      double e = xDet.GetStripByStripN(xstrips[z]).GetEnergy();
      if (e > emax) {
        emax = e;
        zmax = z;
      }
    }
    xstrips_sorted.push_back(xstrips[zmax]);
    xstrips.erase(xstrips.begin()+zmax);
  } while (xstrips.size() > 0);
  do {
    double emax = 0.0;
    double zmax = 0;
    for (unsigned int z = 0; z < ystrips.size(); z++) {
      double e = yDet.GetStripByStripN(ystrips[z]).GetEnergy();
      if (e > emax) {
        emax = e;
        zmax = z;
      }
    }
    ystrips_sorted.push_back(ystrips[zmax]);
    ystrips.erase(ystrips.begin()+zmax);
  } while (ystrips.size() > 0);

  if (verbose > 0) {
    for (int ii = 0; ii < 20; ii++) cout << "bb1";
    cout << endl;
    cout << "X-strips above individual thresholds:" << endl;
    cout << "ntuple#, strip#, energy, resolution" << endl;
    for (unsigned int ii = 0; ii < xstrips_sorted.size(); ii++) {
      BB1Strip x = xDet.GetStripByStripN(xstrips_sorted[ii]);
      cout << x.GetNtupleNumber() << "\t"
           << x.GetStripNumber() << "\t"
           << x.GetEnergy() << "\t"
           << x.GetResolution() << endl;
    }
    cout << "Y-strips above individual thresholds:" << endl;
    cout << "ntuple#, strip#, energy, resolution" << endl;
    for (unsigned int ii = 0; ii < ystrips_sorted.size(); ii++) {
      BB1Strip y = yDet.GetStripByStripN(ystrips_sorted[ii]);
      cout << y.GetNtupleNumber() << "\t"
           << y.GetStripNumber() << "\t"
           << y.GetEnergy() << "\t"
           << y.GetResolution() << endl;        
    }

  }
  // Check for double hits
  if (nx >= 2 && ny >= 2) {
    //    cout << "Both >= 2!" << endl;
    BB1Strip xs[2] = {xDet.GetStripByStripN(xstrips_sorted[0]),
                      xDet.GetStripByStripN(xstrips_sorted[1])};
    BB1Strip ys[2] = {yDet.GetStripByStripN(ystrips_sorted[0]),
                      yDet.GetStripByStripN(ystrips_sorted[1])};
    bool pass = true;
    for (int i = 0; i < 2; i++) {
      double r = sqrt(pow(xs[i].GetResolution(), 2) +
                      pow(ys[i].GetResolution(), 2));
      double d = fabs(xs[i].GetEnergy() - ys[i].GetEnergy());
      double e = 0.5*(xs[1].GetEnergy() + ys[i].GetEnergy());
      double t = fabs(static_cast<double>(xs[i].GetMaxT()) -
                      static_cast<double>(ys[i].GetMaxT()));      
      pass = pass
          && d < n_cut*r
          && t < n_cut*xDet.tdiff_sig
          && e > xDet.GetDoubleHitThreshold();
    }
    
    if (pass) {
      if (verbose) {
        cout << "Event satisfies multi-hit criteria" << endl;
      }
      r.twoHits = true;

      h.xpos = xDet.GetPositionForStrip(xstrips_sorted[0]);
      h.ypos = yDet.GetPositionForStrip(ystrips_sorted[0]);
      double enext = xs[0].GetEnergy();
      double eneyt = ys[0].GetEnergy();
      double resxt = xs[0].GetResolution();
      double resyt = ys[0].GetResolution();
      double nt = (enext/(resxt*resxt)) + (eneyt/(resyt*resyt));
      double dt = (1.0 /(resxt*resxt)) + (1.0 /(resyt*resyt));
      h.time_ch = 0.5*static_cast<double>(xs[0].GetMaxT() + ys[0].GetMaxT());
      h.energy = nt / dt;
      h.nx = 1;
      h.ny = 1;
      h.smaxx = smaxx;
      h.smaxy = smaxy;
      h.passE = true;
      h.passT = true;
      h.pass = true;
      
      r.hit = h;
          
      r.secHit.xpos = xDet.GetPositionForStrip(xstrips_sorted[1]);
      r.secHit.ypos = yDet.GetPositionForStrip(ystrips_sorted[1]);
      enext = xs[1].GetEnergy();
      eneyt = ys[1].GetEnergy();
      resxt = xs[1].GetResolution();
      resyt = ys[1].GetResolution();
      nt = (enext/(resxt*resxt)) + (eneyt/(resyt*resyt));
      dt = (1.0 /(resxt*resxt)) + (1.0 /(resyt*resyt));
      r.secHit.energy = nt/dt;
      r.secHit.nx = 1;
      r.secHit.ny = 1;
      r.secHit.smaxx = xstrips_sorted[1];
      r.secHit.smaxy = ystrips_sorted[1];
      r.secHit.time_ch = 0.5*static_cast<double>(xs[1].GetMaxT() +
                                                 ys[1].GetMaxT());
      r.secHit.passE = true;
      r.secHit.passT = true;
      r.secHit.pass = true;
      
      return r;
    }
  }

  double resx = -10., enex = -10.;
  double resy = -10., eney = -10.;
  double posx = -100.0, posy = -100.0;
  bool xadjacent = false, yadjacent = false;
  double xT = 0., yT = 0.;
  if (nx == 1) {
    resx = xDet.GetStripByStripN(smaxx).GetResolution();
    enex = xDet.GetStripByStripN(smaxx).GetEnergy();
    posx = xDet.GetPositionForStrip(smaxx);
    xT = static_cast<double>(xDet.GetStripByStripN(smaxx).GetMaxT());
    //    } else if (nx == 2 && abs(xstrips_sorted[1]-xstrips_sorted[0]) == 1) {
  } else {
    xadjacent = false;
    unsigned int s1 = 0, s2 = 1;
    for (s1 = 0; s1 < xstrips_sorted.size() && !xadjacent; s1++) {
      for (s2 = s1 + 1; s2 < xstrips_sorted.size() && !xadjacent; s2++) {
        if (abs(xstrips_sorted[s1] - xstrips_sorted[s2]) == 1) {
          xadjacent = true;
        }
      }
    }
    s1--;
    s2--;
    if (xadjacent) {
      if (verbose > 0) {
        cout << "Event appears to have 2 adjacent x-strips" << endl;
      }
      BB1Strip temp[2] = {xDet.GetStripByStripN(xstrips_sorted[s1]),
                          xDet.GetStripByStripN(xstrips_sorted[s2])};
      resx = 0.5*(temp[0].GetResolution() + temp[1].GetResolution());
      enex = temp[0].GetEnergy() + temp[1].GetEnergy();

      posx = (temp[0].GetEnergy()*xDet.GetPositionForStrip(xstrips_sorted[0])) +
          (temp[1].GetEnergy()*xDet.GetPositionForStrip(xstrips_sorted[1]));
      posx = posx / enex;
      double t1 = static_cast<double>(temp[0].GetMaxT());
      double t2 = static_cast<double>(temp[1].GetMaxT());
      xT = (t1 + t2) / 2.0;
      
    } else {//if (xDet.GetStripByStripN(smaxx).GetEnergy() > 40.0) {
      enex = xDet.GetStripByStripN(smaxx).GetEnergy();
      resx = xDet.GetStripByStripN(smaxx).GetResolution();
      posx = xDet.GetPositionForStrip(smaxx); 
      xT = static_cast<double>(xDet.GetStripByStripN(smaxx).GetMaxT());
    } // else {
    //   enex = -10.0;
    //   xT = -10.0;
    // }
  }

  if (ny == 1) {
    //    h.pass = true;
    resy = yDet.GetStripByStripN(smaxy).GetResolution();
    eney = yDet.GetStripByStripN(smaxy).GetEnergy();
    posy = yDet.GetPositionForStrip(smaxy);
    yT = static_cast<double>(yDet.GetStripByStripN(smaxy).GetMaxT());
    //    } else if (ny == 2 && abs(ystrips_sorted[1]-ystrips_sorted[0]) == 1) {
  } else {
    yadjacent = false;
    unsigned int s1 = 0, s2 = 1;
    for (s1 = 0; s1 < ystrips_sorted.size() && !yadjacent; s1++) {
      for (s2 = s1 + 1; s2 < ystrips_sorted.size() && !yadjacent; s2++) {
        if (abs(ystrips_sorted[s1] - ystrips_sorted[s2]) == 1) {
          yadjacent = true;
        }
      }
    }
    s1--;
    s2--;
    if (yadjacent) {
      if (verbose > 0) {
        cout << "Event appears to have 2 adjacent y-strips" << endl;
        if (verbose > 1) {
          cout << "S1 = " << s1 << endl;
          cout << "S2 = " << s2 << endl;
        }
      }
      BB1Strip temp[2] = {yDet.GetStripByStripN(ystrips_sorted[s1]),
                          yDet.GetStripByStripN(ystrips_sorted[s2])};
      resy = 0.5*(temp[0].GetResolution() + temp[1].GetResolution());
      eney = temp[0].GetEnergy() + temp[1].GetEnergy();

      posy = (temp[0].GetEnergy()*yDet.GetPositionForStrip(ystrips_sorted[0])) +
          (temp[1].GetEnergy()*yDet.GetPositionForStrip(ystrips_sorted[1]));
      posy = posy / eney;
      double t1 = static_cast<double>(temp[0].GetMaxT());
      double t2 = static_cast<double>(temp[1].GetMaxT());
      yT = (t1 + t2) / 2.0;
    } else {//if (yDet.GetStripByStripN(smaxy).GetEnergy() > 40.0) {
      eney = yDet.GetStripByStripN(smaxy).GetEnergy();
      resy = yDet.GetStripByStripN(smaxy).GetResolution();
      posy = yDet.GetPositionForStrip(smaxy);
      yT = static_cast<double>(yDet.GetStripByStripN(smaxy).GetMaxT());
    // } else {
    //   eney = -10.0;
    //   yT = -10.0;
    }
  }

  double res_tot = sqrt(pow(resx, 2.0) + pow(resy, 2.0));
  double ediff = fabs(enex - eney);
  //    cout << "Ediff = " << ediff << "\t total res = " << res_tot << endl;

  // double n = (enex/(resx*resx)) + (eney/(resy*resy));
  // double d = (1.0 /(resx*resx)) + (1.0 /(resy*resy));

  bool finalPass = false;
  if (verbose > 1) {
    cout << "Comparing energies for A: " << enex << " and " << eney << endl;
  }
  if (CompareXY(enex, resx, xT, eney, resy, yT, n_cut, xDet.tdiff_sig)) {
    double n = (enex/(resx*resx)) + (eney/(resy*resy));
    double d = (1.0 /(resx*resx)) + (1.0 /(resy*resy));
    h.energy = n / d;
    h.xpos = posx;
    h.ypos = posy;
    finalPass = true;
    h.passE = true;
    h.passT = true;
    h.time_ch = 0.5*(xT + yT);
    if (verbose > 0) cout << "Passed A (all adjacent strips, if any)" << endl;
  }

  if (!finalPass) {
    double xE = xDet.GetStripByStripN(smaxx).GetEnergy();
    double xR = xDet.GetStripByStripN(smaxx).GetResolution();
    double xT_try = static_cast<double>(xDet.GetStripByStripN(smaxx).GetMaxT());
    if (verbose > 1) {
      cout << "Comparing energies for B: " << xE << " and " << eney << endl;
    }
    if (CompareXY(xE, xR, xT_try, eney, resy, yT, n_cut, xDet.tdiff_sig)) {
      double n = (xE/(xR*xR)) + (eney/(resy*resy));
      double d = (1.0 /(xR*xR)) + (1.0 /(resy*resy));
      h.energy = n / d;
      h.xpos = xDet.GetPositionForStrip(smaxx);
      h.ypos = posy;
      finalPass = true;
      h.passE = true;
      h.passT = true;
      h.time_ch = 0.5*(xT_try + yT);
      if (verbose > 0) cout << "Passed B (only y-adjacent strips, if any)"
                            << endl;
    }
  }

  if (!finalPass) {
    double yE = yDet.GetStripByStripN(smaxy).GetEnergy();
    double yR = yDet.GetStripByStripN(smaxy).GetResolution();
    double yT_try = static_cast<double>(yDet.GetStripByStripN(smaxy).GetMaxT());
    if (verbose > 1) {
      cout << "Comparing energies for C: " << enex << " and " << yE << endl;
    }
    if (CompareXY(enex, resx, xT, yE, yR, yT_try, n_cut, xDet.tdiff_sig)) {
      double n = (enex/(resx*resx)) + (yE/(yR*yR));
      double d = (1.0 /(resx*resx)) + (1.0 /(yR*yR));
      h.energy = n / d;
      h.xpos = posx;
      h.ypos = yDet.GetPositionForStrip(smaxy);
      finalPass = true;
      h.passE = true;
      h.passT = true;
      h.time_ch = 0.5*(xT + yT_try);
      if (verbose > 0) cout << "Passed C (only x-adjacent strips, if any)"
                            << endl;
    }
  }
  if (!finalPass) {
    double xE = xDet.GetStripByStripN(smaxx).GetEnergy();
    double xR = xDet.GetStripByStripN(smaxx).GetResolution();
    double xT_try = static_cast<double>(xDet.GetStripByStripN(smaxx).GetMaxT());
    double yE = yDet.GetStripByStripN(smaxy).GetEnergy();
    double yR = yDet.GetStripByStripN(smaxy).GetResolution();
    double yT_try = static_cast<double>(yDet.GetStripByStripN(smaxy).GetMaxT());

    res_tot = sqrt(pow(xR, 2.0) + pow(yR, 2.0));
    ediff = fabs(xE - yE);
    double n = (xE/(xR*xR)) + (yE/(yR*yR));
    double d = (1./(xR*xR)) + (1./(yR*yR));
    h.energy = n / d;
    h.xpos = posx;
    h.ypos = posy;
    h.passE = (ediff < res_tot*n_cut && xE > 0. && yE > 0.);
    h.passT = (fabs(xT_try - yT_try) < n_cut*xDet.tdiff_sig);
    h.time_ch = 0.5*(xT_try + yT_try);
    if (h.passE && h.passT && verbose > 0) {
      cout << "Passed D (no adjacent strips)" << endl;
    }
  }
  h.pass = h.passE && h.passT;
  r.hit = h;
  return r;
}

bool CompareXY(double xE, double xR, double xT,
               double yE, double yR, double yT, double n_cut, double tdiff_sig) {
  int verbose = 0;
  double res_tot = sqrt(pow(xR, 2.0) + pow(yR, 2.0));
  double ediff = fabs(xE - yE);
  double tdiff = fabs(xT - yT);
  bool p = (ediff < res_tot*n_cut && xE > 0.0 &&
            yE > 0.0 && tdiff < n_cut*tdiff_sig);

  if (verbose > 0) {
    cout << "X: " << xE << "\t" << xR << "\t" << xT << endl;
    cout << "Y: " << yE << "\t" << yR << "\t" << yT << endl;
    cout << "sigma_tot = " << res_tot << endl;
    cout << "ediff = " << ediff << " = " << ediff/res_tot << " sigmas"
         << endl;
    cout << "n_cut = " << n_cut << endl;
  }

  return p;
}
