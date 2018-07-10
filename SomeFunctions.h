// ==================================================================== //
// Code by Melissa Anholm
// June 2017 - 
// 
// consolidate_and_fit.cpp inherits from consolidate_results3.cpp
// LineshapeConvolution.h inherits from RhoLibs.h
// ==================================================================== //
#ifndef INCLUDE_SOMEFUNCTIONS_H
#define INCLUDE_SOMEFUNCTIONS_H 1


#include <string>

#include <TH1D.h>
#include "TF1.h"
#include "TColor.h"

using std::string;


TF1 * make_Abeta_func();
TF1 * make_Abeta_b_func();
TF1 * make_quasi_b_func();
TF1 * make_parabola_func();

extern string double_to_string(double thisnumber, int thisprecision=3);  // 
// ---- // ---- // ---- // ---- // ---- // ---- //

bool HistsHaveSameBinning(TH1D *a, TH1D *b, bool verbose=false);
double justgetthedamnchisquared(TH1D * h1, TH1D * h2, bool already_weighted=true, int bmin=0, int bmax=0);

TH1D * tf1_to_hist_like(TF1 * this_tf1, TH1D * this_hist, int this_color=int(kBlack) );
TH1D * tf1_to_hist_like(TF1 * this_tf1, TH1D * this_hist, TColor this_color);
TH1D * make_th1_from_tf1(TF1* this_tf1, string hist_type, int this_color=int(kBlack), string new_hist_title=string(""));
TH1D * makehist_zeroslike(TH1D* oldhist);
TH1D * makehist_oneslike(TH1D* oldhist);
TH1D * makehist_Avoverc_like(TH1D* oldhist);


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#endif
