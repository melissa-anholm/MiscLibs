#include "TF1.h"
#include "TMath.h"
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TROOT.h>

//#include "MapUtility.cpp"
//#include "MakeSomeTF1s.cpp"

#include "FitUtility.h"

#include <string>
#include <iostream>  // cout
using std::string;
using std::cout;
using std::endl;


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
TF1* make_relnorm(string thename="f_relnorm")
{
	TF1 * f_relnorm = new TF1(thename.c_str(), "0.5*(1-TMath::Erf((x-[xe])/[se]))* [A1]*exp(x/[tau1]) + 0.5*(1+TMath::Erf((x-[xe])/[se]))*[A2]*(1-exp(-(x-[x0])/[tau2])) + [m]*x + [b]", 0, 6000.0);
	f_relnorm -> SetLineColor(kBlack);
	f_relnorm -> SetLineWidth(1);
	f_relnorm -> SetNpx(500);
	return f_relnorm;
}

TF1* make_relnorm_tp()
{
	TF1 * f_relnorm = make_relnorm("f_relnorm_tp");
	
	f_relnorm -> SetParameter("A1",      0.000347615);
	f_relnorm -> SetParameter("A2",      0.379864);
	f_relnorm -> SetParameter("b",       0);
	f_relnorm -> SetParameter("m",       5.63574e-06);
	f_relnorm -> SetParameter("se",      187.614);
	f_relnorm -> SetParameter("tau1",    156.583);
	f_relnorm -> SetParameter("tau2",    670.677);
	f_relnorm -> SetParameter("x0",      549.676);
	f_relnorm -> SetParameter("xe",      689.091);
	
	return f_relnorm;
}
TF1* make_relnorm_tm()
{
	TF1 * f_relnorm = make_relnorm("f_relnorm_tm");

	f_relnorm -> SetParameter("A1",      0.000351555);
	f_relnorm -> SetParameter("A2",      0.389504);
	f_relnorm -> SetParameter("b",       0);
	f_relnorm -> SetParameter("m",       5.49564e-06);
	f_relnorm -> SetParameter("se",      199.123);
	f_relnorm -> SetParameter("tau1",    159.921);
	f_relnorm -> SetParameter("tau2",    698.494);
	f_relnorm -> SetParameter("x0",      536.535);
	f_relnorm -> SetParameter("xe",      688.141);
	
	return f_relnorm;
}
TF1* make_relnorm_bp()
{
	TF1 * f_relnorm = make_relnorm("f_relnorm_bp");
	
	f_relnorm -> SetParameter("A1",      0.000316087);
	f_relnorm -> SetParameter("A2",      0.421939);
	f_relnorm -> SetParameter("b",       0);
	f_relnorm -> SetParameter("m",       6.38493e-06);
	f_relnorm -> SetParameter("se",      179.02);
	f_relnorm -> SetParameter("tau1",    150.42);
	f_relnorm -> SetParameter("tau2",    681.787);
	f_relnorm -> SetParameter("x0",      553.463);
	f_relnorm -> SetParameter("xe",      681.219);
	
	return f_relnorm;
}
TF1* make_relnorm_bm()
{
	TF1 * f_relnorm = make_relnorm("f_relnorm_bm");
	
	f_relnorm -> SetParameter("A1",      0.000324752);
	f_relnorm -> SetParameter("A2",      0.415921);
	f_relnorm -> SetParameter("b",       0);
	f_relnorm -> SetParameter("m",       5.41815e-06);
	f_relnorm -> SetParameter("se",      177.245);
	f_relnorm -> SetParameter("tau1",    150.211);
	f_relnorm -> SetParameter("tau2",    672.626);
	f_relnorm -> SetParameter("x0",      555.174);
	f_relnorm -> SetParameter("xe",      683.555);
	
	return f_relnorm;
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
TF1 * makedist_E0()  // actually, it makes pseudoE0.
{
	TF1 * f_E0 = new TF1("f_E0", "([Ae]*(1-exp(-(x-[x0])/[tau]))+[m]*x +[k])*0.5*(1+TMath::Erf((x-[x0e])/[sigma] )) + [k] +[zz]", 0.0, 6000.0);
	f_E0 -> SetLineColor(kBlack);
	f_E0 -> SetLineWidth(1);
	
	f_E0->SetParameter("Ae",       9.48525e+01);
	f_E0->SetParameter("k",        5.69909e+01);
	f_E0->SetParameter("m",       -3.16184e-03);
	f_E0->SetParameter("sigma",    6.14420e+02);
	f_E0->SetParameter("tau",      7.10119e+02);
	f_E0->SetParameter("x0",       500.0);
	f_E0->SetParameter("x0e",      5.10329e+02);
	f_E0->SetParameter("zz",      -200.0);
	
	return f_E0;
}
void SetParam_E0_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  // KLUDGE!  DON'T LET E0==0 !
{
	TF1 * f_E0 = makedist_E0();
	double thenew_E0 = f_E0->Eval( (double)mono_int );
	double the_newer_E0 = mono_int-300.0 + thenew_E0;
	if( abs(the_newer_E0)<1.0 ) { the_newer_E0=1.0; }
	
	R_tp.SetParameter("00_E0", the_newer_E0);
	R_tm.SetParameter("00_E0", the_newer_E0);
	R_bp.SetParameter("00_E0", the_newer_E0);
	R_bm.SetParameter("00_E0", the_newer_E0);
}
TF1 * makedist_W()
{
//	TF1 * f_W = new TF1("f_W", "gaus(0)", 0.0, 6000.0);
//	f_W -> SetParameter("Constant",   4.00092e+02);
//	f_W -> SetParameter("Mean",       8.18778e+03);
//	f_W -> SetParameter("Sigma",      2.28319e+04);

	TF1 * f_W = new TF1("f_W", "[A]*(1-exp(-(x-[x0])/[tau])) + [m]*x + [b]", 0.0, 6000.0);
	f_W -> SetLineColor(kBlack);
	f_W -> SetLineWidth(1);
	
	f_W -> SetParameter("A",     10.3273);
	f_W -> SetParameter("b",     377.637);
	f_W -> SetParameter("m",     0.00114857);
	f_W -> SetParameter("tau",   909.883);
	f_W -> SetParameter("x0",    1000.0);
	
	return f_W;
}
void SetParam_W_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)
{
	TF1 * f_W = makedist_W();
	double thenew_W = f_W->Eval( (double)mono_int );
	R_tp.SetParameter("07_W", thenew_W);
	R_tm.SetParameter("07_W", thenew_W);
	R_bp.SetParameter("07_W", thenew_W);
	R_bm.SetParameter("07_W", thenew_W);
	
	return;
}
TF1 * makedist_gres()
{
	TF1 * f_gres = new TF1("f_gres", "1.0", 0.0, 6000.0);
	f_gres -> SetLineColor(kBlack);
	f_gres -> SetLineWidth(1);
	return f_gres;
}
void SetParam_gres_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  // 1.0
{
	TF1 * f_gres = makedist_gres();
	double thenew_gres = f_gres->Eval( (double)mono_int );

	R_tp.SetParameter("12_gres", thenew_gres);
	R_tm.SetParameter("12_gres", thenew_gres);
	R_bp.SetParameter("12_gres", thenew_gres);
	R_bm.SetParameter("12_gres", thenew_gres);
}
TF1 * makedist_DgE()
{
	TF1 * f_DgE = new TF1("f_DgE", "0.0", 0.0, 6000.0);
	f_DgE -> SetLineColor(kBlack);
	f_DgE -> SetLineWidth(1);
	return f_DgE;
}
void SetParam_DgE_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  // 0.0
{
	TF1 * f_DgE = makedist_DgE();
	double thenew_DgE = f_DgE->Eval( (double)mono_int );
	
	R_tp.SetParameter("13_DgE", thenew_DgE);
	R_tm.SetParameter("13_DgE", thenew_DgE);
	R_bp.SetParameter("13_DgE", thenew_DgE);
	R_bm.SetParameter("13_DgE", thenew_DgE);
}
TF1 * makedist_gamma_more()
{
	TF1 * f_gamma_more = new TF1("f_gamma_more", "[A]*(1-exp(-(x-[x0])/[tau])) + [m]*x + [b]", 0.0, 6000.0);
	f_gamma_more -> SetLineColor(kBlack);
	f_gamma_more -> SetLineWidth(1);
	f_gamma_more->SetParameter("A",       0.0000517265);  // tm, bp
	f_gamma_more->SetParameter("b",       0.00116423);
	f_gamma_more->SetParameter("m",      -2.22692e-08);
	f_gamma_more->SetParameter("tau",     6.99255e+02);
	f_gamma_more->SetParameter("x0",      1000);
	
	return f_gamma_more;
}
TF1 * makedist_gamma_less()
{
	TF1 * f_gamma_less = new TF1("f_gamma_less", "[A]*(1-exp(-(x-[x0])/[tau])) + [m]*x + [b]", 0.0, 6000.0);
	f_gamma_less -> SetLineColor(kBlack);
	f_gamma_less -> SetLineWidth(1);
	f_gamma_less->SetParameter("A",       0.0000328339);  // tp bm
	f_gamma_less->SetParameter("b",       0.00117988);
	f_gamma_less->SetParameter("m",      -1.79271e-08);
	f_gamma_less->SetParameter("tau",     6.99255e+02);
	f_gamma_less->SetParameter("x0",      1000);
	
	return f_gamma_less;
}

void SetParam_gamma_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  // this might need four?  it was split in two earlier, but now it's not clear which two would be better...  welp.  it's split into two again.  
{
	TF1 * f_gamma_more = makedist_gamma_more();
	TF1 * f_gamma_less = makedist_gamma_less();
	double thenew_gamma_more = f_gamma_more->Eval( (double)mono_int);
	double thenew_gamma_less = f_gamma_less->Eval( (double)mono_int);
	
	R_tp.SetParameter("05_gamma", thenew_gamma_less);
	R_tm.SetParameter("05_gamma", thenew_gamma_more);
	R_bp.SetParameter("05_gamma", thenew_gamma_more);
	R_bm.SetParameter("05_gamma", thenew_gamma_less);
}
TF1 * makedist_dE0()
{
	TF1 * f_dE0 = new TF1("f_dE0", "([Ae]*(1-exp(-(x-[x0])/[tau]))+[m]*x +[k])*0.5*(1+TMath::Erf((x-[x0e])/[sigma] )) + [k] +[zz]", 0, 6000.0);
	f_dE0 -> SetLineColor(kBlack);
	f_dE0 -> SetLineWidth(1);
	
	f_dE0->SetParameter("Ae",       1.21925e+08);
	f_dE0->SetParameter("k",        206.476);
	f_dE0->SetParameter("m",       -1.18936);
	f_dE0->SetParameter("sigma",    1006.67);
	f_dE0->SetParameter("tau",      1.02711e+08);
	f_dE0->SetParameter("x0",       500.0);
	f_dE0->SetParameter("x0e",     -34.0212);
	f_dE0->SetParameter("zz",       200.0);
	
	return f_dE0;
}
void SetParam_dE0_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  // space between gauss and moyal.
{
	TF1 * f_dE0 = makedist_dE0();
	double thenew_dE0 = f_dE0->Eval( (double)mono_int );

	R_tp.SetParameter("14_dE0", thenew_dE0);
	R_tm.SetParameter("14_dE0", thenew_dE0);
	R_bp.SetParameter("14_dE0", thenew_dE0);
	R_bm.SetParameter("14_dE0", thenew_dE0);
}
//
TF1 * makedist_k()
{
	TF1 * f_k = new TF1("f_k", "[m]*x+[b] + [A1]*exp(-((x-[x1])/[s1])^2)", 0, 6000.0);
	f_k -> SetLineColor(kBlack);
	f_k -> SetLineWidth(1);
	f_k -> SetNpx(500);
	
	f_k -> SetParameter("A1",    0.324309);
	f_k -> SetParameter("b",     2.74734);
	f_k -> SetParameter("m",     0.000238217);
	f_k -> SetParameter("s1",    1107.8);
	f_k -> SetParameter("x1",   -662.466);

	return f_k;
}
void SetParam_k_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)
{
	TF1 * f_k = makedist_k();
	double thenew_k = f_k->Eval( (double)mono_int);
	
	R_tp.SetParameter("08_k", thenew_k);
	R_tm.SetParameter("08_k", thenew_k);
	R_bp.SetParameter("08_k", thenew_k);
	R_bm.SetParameter("08_k", thenew_k);
}

TF1 * makedist_toeres_top()
{
	TF1 * f_toeres_top = new TF1("f_toeres_top", "[A]*(1+TMath::Erf( (x-[x0])/[sigma] )) + [A]*(1-TMath::Erf( (x-[x0])/[sigma] ))*(exp( -(x-[x1])/[tau] ) + [m]*x + [b]) + [zag]*exp(-((x-[x0])/[zsg])^2)", 0, 6000.0);
	f_toeres_top -> SetLineColor(kBlack);
	f_toeres_top -> SetLineWidth(1);
	f_toeres_top -> SetNpx(500);

	f_toeres_top->SetParameter("A",      1.02123);  // tp only ...and tm
	f_toeres_top->SetParameter("b",      1.02393);
	f_toeres_top->SetParameter("m",     -0.0000105528);
	f_toeres_top->SetParameter("sigma", -2.09544e+02);
	f_toeres_top->SetParameter("tau",    1114.66);
	f_toeres_top->SetParameter("x0",     640.);
	f_toeres_top->SetParameter("x1",     1000.0);
	f_toeres_top->SetParameter("zag",    2.19024);
	f_toeres_top->SetParameter("zsg",    3.50149e+02);
	
	return f_toeres_top;
}
TF1 * makedist_toeres_bottom()
{
	TF1 * f_toeres_bottom = new TF1("f_toeres_bottom", "[A]*(1+TMath::Erf( (x-[x0])/[sigma] )) + [A]*(1-TMath::Erf( (x-[x0])/[sigma] ))*(exp( -(x-[x1])/[tau] ) + [m]*x + [b]) + [zag]*exp(-((x-[x0])/[zsg])^2)", 0, 6000.0);
	f_toeres_bottom -> SetLineColor(kBlack);
	f_toeres_bottom -> SetLineWidth(1);
	f_toeres_bottom -> SetNpx(500);

	f_toeres_bottom->SetParameter("A",      1.08458);  // bp && bm
	f_toeres_bottom->SetParameter("b",      0.824265);
	f_toeres_bottom->SetParameter("m",      6.22652e-06);
	f_toeres_bottom->SetParameter("sigma", -2.09544e+02);
	f_toeres_bottom->SetParameter("tau",    1199.58);
	f_toeres_bottom->SetParameter("x0",     640.);
	f_toeres_bottom->SetParameter("x1",     1000.0);
	f_toeres_bottom->SetParameter("zag",    2.32502);
	f_toeres_bottom->SetParameter("zsg",    3.50149e+02);
	
	return f_toeres_bottom;
}
void SetParam_toeres_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  //this doesn't look right at low E.
{
	TF1 * f_toeres_top    = makedist_toeres_top();
	TF1 * f_toeres_bottom = makedist_toeres_bottom();
	
	double thenew_toeres_top    = f_toeres_top   ->Eval( (double)mono_int);
	double thenew_toeres_bottom = f_toeres_bottom->Eval( (double)mono_int);
	
	R_tp.SetParameter("10_toeres", thenew_toeres_top);
	R_tm.SetParameter("10_toeres", thenew_toeres_top);
	R_bp.SetParameter("10_toeres", thenew_toeres_bottom);
	R_bm.SetParameter("10_toeres", thenew_toeres_bottom);
	
	return;
}

// MELISSA LOOK HERE!  FIX DELTA!!!!!!!  ...ok, it's fixed.
TF1 * makedist_delta_top()
{
	TF1 * tmp = makedist_delta_bottom();
	TF1 * f_delta_top = (TF1*)tmp->Clone("f_delta_top");
	
	return f_delta_top;
}
TF1 * makedist_delta_bottom()
{
	TF1 * f_delta_bottom = new TF1("f_delta_bottom4", "(gaus(0)+[m]*x+[b])*0.5*(1+TMath::Erf((x-[xe])/[se]))+[b2]", 0, 6000.0);

	f_delta_bottom -> SetLineColor(kBlack);
	f_delta_bottom -> SetLineWidth(1);
	
	//	bottom: 
	f_delta_bottom -> SetParameter("p0",      0.926353);  // const
	f_delta_bottom -> SetParameter("p1",      4120.9);    // mean
	f_delta_bottom -> SetParameter("p2",      15000.0);   // sigma
	f_delta_bottom -> SetParameter("b",      -0.900806);
	f_delta_bottom -> SetParameter("b2",      0.0005);
	f_delta_bottom -> SetParameter("m",       1.9893e-06);
	f_delta_bottom -> SetParameter("se",      215.263);
	f_delta_bottom -> SetParameter("xe",      686.422);
	
	
	return f_delta_bottom;
}
void SetParam_delta_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  // might want to separate out into less and more, later.
{
	TF1 * f_delta_top    = makedist_delta_top();
	TF1 * f_delta_bottom = makedist_delta_bottom();
	
	double thenew_delta_top    = f_delta_top   ->Eval( (double)mono_int);
	double thenew_delta_bottom = f_delta_bottom->Eval( (double)mono_int);
	
	if( thenew_delta_top<=0 )    { thenew_delta_top= 1.0e-12; }
	if( thenew_delta_bottom<=0 ) { thenew_delta_bottom=1.0e-12; }

	R_tp.SetParameter("06_delta", thenew_delta_top);
	R_tm.SetParameter("06_delta", thenew_delta_top);
	R_bp.SetParameter("06_delta", thenew_delta_bottom);
	R_bm.SetParameter("06_delta", thenew_delta_bottom);
}
TF1 * makedist_scale_more()
{
	TF1 * f_scale_more = new TF1("f_scale_more", "[A]*0.5*(1+TMath::Erf((x-[x0])/[sigma]))*exp(-(x-[x1])/[tau]) +[zAg]*exp(-(x-[zxg])^2/[zsg]^2)", 0, 6000.0);
	f_scale_more -> SetLineColor(kBlack);
	f_scale_more -> SetLineWidth(1);
	f_scale_more -> SetNpx(500);
	
	f_scale_more -> SetParameter("A",       9.01323e-01);  // fit to blue (T-)
	f_scale_more -> SetParameter("sigma",   1.43649e+02);
	f_scale_more -> SetParameter("tau",     3.39774e+02);
	f_scale_more -> SetParameter("x0",      5.52682e+02);
	f_scale_more -> SetParameter("x1",      1000.0);
	f_scale_more -> SetParameter("zAg",     7.60951e-01);
	f_scale_more -> SetParameter("zsg",     1.18927e+02);
	f_scale_more -> SetParameter("zxg",     7.53364e+02);
	
	return f_scale_more;
}
TF1 * makedist_scale_less()
{
	TF1 * f_scale_less = new TF1("f_scale_less", "[A]*0.5*(1+TMath::Erf((x-[x0])/[sigma]))*exp(-(x-[x1])/[tau]) +[zAg]*exp(-(x-[zxg])^2/[zsg]^2)", 0, 6000.0);
	f_scale_less -> SetLineColor(kBlack);
	f_scale_less -> SetLineWidth(1);
	f_scale_less -> SetNpx(500);

	f_scale_less -> SetParameter("A",       2.99792e-01);  // fit to orange (T-)
	f_scale_less -> SetParameter("sigma",   9.26651e+01);
	f_scale_less -> SetParameter("tau",     2.97734e+02);
	f_scale_less -> SetParameter("x0",      6.62628e+02);
	f_scale_less -> SetParameter("x1",      1000.0);
	f_scale_less -> SetParameter("zAg",     5.03255e-01);
	f_scale_less -> SetParameter("zsg",     1.73095e+02);
	f_scale_less -> SetParameter("zxg",     5.89858e+02);
	
	return f_scale_less;
}
void SetParam_scale_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  // I'll maybe need 4, eventually.  this is only temporary..
{
	TF1 * f_scale_more = makedist_scale_more();
	TF1 * f_scale_less = makedist_scale_less();
	//
	double thenew_scale_more = f_scale_more->Eval( (double)mono_int);
	double thenew_scale_less = f_scale_less->Eval( (double)mono_int);
	
	R_tp.SetParameter("02_scale", thenew_scale_less);
	R_tm.SetParameter("02_scale", thenew_scale_more);
	R_bp.SetParameter("02_scale", thenew_scale_more);
	R_bm.SetParameter("02_scale", thenew_scale_less);
}
TF1 * makedist_lres()
{
	TF1 * f_lres = new TF1("f_lres", "[A]*(exp(-(x-[x0])/[tau])) + [m]*x + [b] + [ag]*exp(-((x-[xg])/[sg])^2)", 0, 6000.0);
	f_lres -> SetLineColor(kBlack);
	f_lres -> SetLineWidth(1);
	
	f_lres -> SetParameter("A",     0.0459696);
	f_lres -> SetParameter("ag",    0.0741505);
	f_lres -> SetParameter("b",     0.0308825);
	f_lres -> SetParameter("m",    -2.56988e-06);
	f_lres -> SetParameter("sg",    565.774);
	f_lres -> SetParameter("tau",   566.959);
	f_lres -> SetParameter("x0",    1000.0);
	f_lres -> SetParameter("xg",    346.381);
	
	return f_lres;
}
void SetParam_lres_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)
{
	TF1 * f_lres = makedist_lres();
	double thenew_lres= f_lres->Eval( (double)mono_int);
	
	R_tp.SetParameter("11_lres", thenew_lres);
	R_tm.SetParameter("11_lres", thenew_lres);
	R_bp.SetParameter("11_lres", thenew_lres);
	R_bm.SetParameter("11_lres", thenew_lres);
}
TF1 * makedist_gfrac()
{
	TF1 * f_gfrac = new TF1("f_gfrac", "[m]*x + [b] + [ag]*exp(-((x-[xg])/[sg])^2)", 0, 6000.0);
	f_gfrac -> SetLineColor(kBlack);
	f_gfrac -> SetLineWidth(1);
	
	f_gfrac -> SetParameter("ag",   -0.0745695);  // tp
	f_gfrac -> SetParameter("b",     0.892039);
	f_gfrac -> SetParameter("m",     4.63657e-06);
	f_gfrac -> SetParameter("sg",    1920.0);
	f_gfrac -> SetParameter("xg",    200);
	
	return f_gfrac;
}
void SetParam_gfrac_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)
{
	TF1 * f_gfrac = makedist_gfrac();
	double thenew_gfrac = f_gfrac->Eval( (double)mono_int);
	
	R_tp.SetParameter("09_gfrac", thenew_gfrac);
	R_tm.SetParameter("09_gfrac", thenew_gfrac);
	R_bp.SetParameter("09_gfrac", thenew_gfrac);
	R_bm.SetParameter("09_gfrac", thenew_gfrac);
}

TF1 * makedist_beta()
{
	TF1 * f_beta = new TF1("f_beta", "[A]*(exp(-(x-[x0])/[tau])) + [m]*x + [b] + [ag]*exp(-((x-[xg])/[sg])^2)", 0, 6000.0);

	f_beta -> SetLineColor(kBlack);
	f_beta -> SetLineWidth(1);
	f_beta -> SetNpx(500);
	
	f_beta -> SetParameter("A",      24.9513);
	f_beta -> SetParameter("ag",     0.334304);
	f_beta -> SetParameter("b",     -25.5278);
	f_beta -> SetParameter("m",      0.00115525);
	f_beta -> SetParameter("sg",     908.53);
	f_beta -> SetParameter("tau",    23796.5);
	f_beta -> SetParameter("x0",     1000);
	f_beta -> SetParameter("xg",     350.9);
	
	return f_beta;
}
void SetParam_beta_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)  // might need two?  unclear.
{
	TF1 * f_beta = makedist_beta();
	double thenew_beta = f_beta->Eval( (double)mono_int);
	
	R_tp.SetParameter("04_beta", thenew_beta);
	R_tm.SetParameter("04_beta", thenew_beta);
	R_bp.SetParameter("04_beta", thenew_beta);
	R_bm.SetParameter("04_beta", thenew_beta);
}

TF1 * makedist_alpha_more()
{
	TF1 * f_alpha_more = new TF1("f_alpha_more", "[b]+[m]*x + [A1]*exp(-((x-[x1])/[s1])^2) + [Ae]*(exp((x-[xe])/[tau]))", 0, 6000.0);
	f_alpha_more -> SetLineColor(kBlack);
	f_alpha_more -> SetLineWidth(1);

	f_alpha_more -> SetParameter("A1",     5.31924e+01);
	f_alpha_more -> SetParameter("Ae",     69.8445);
	f_alpha_more -> SetParameter("b",      0);
	f_alpha_more -> SetParameter("m",      0);
	f_alpha_more -> SetParameter("s1",     1.03433e+03);
	f_alpha_more -> SetParameter("tau",    2411.87);
	f_alpha_more -> SetParameter("x1",     1400.0);
	f_alpha_more -> SetParameter("xe",     1000.0);
	
	return f_alpha_more;
}
TF1 * makedist_alpha_less()
{
	TF1 * f_alpha_less = new TF1("f_alpha_less", "[b]+[m]*x + [A1]*exp(-((x-[x1])/[s1])^2) + [Ae]*(exp((x-[xe])/[tau]))", 0, 6000.0);
	f_alpha_less -> SetLineColor(kBlack);
	f_alpha_less -> SetLineWidth(1);
	
	f_alpha_less -> SetParameter("A1",     5.31924e+01);
	f_alpha_less -> SetParameter("Ae",     47.844);
	f_alpha_less -> SetParameter("b",      18);
	f_alpha_less -> SetParameter("m",      0);
	f_alpha_less -> SetParameter("s1",     1.03433e+03);
	f_alpha_less -> SetParameter("tau",    2302.3);
	f_alpha_less -> SetParameter("x1",     1500.0);
	f_alpha_less -> SetParameter("xe",     1000.0);
	
	return f_alpha_less;
}
void SetParam_alpha_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)
{
	TF1 * f_alpha_more = makedist_alpha_more();
	TF1 * f_alpha_less = makedist_alpha_less();
	
	double thenew_alpha_more = f_alpha_more->Eval( (double)mono_int);
	double thenew_alpha_less = f_alpha_less->Eval( (double)mono_int);
	
	R_tp.SetParameter("03_alpha", thenew_alpha_less);
	R_tm.SetParameter("03_alpha", thenew_alpha_more);
	R_bp.SetParameter("03_alpha", thenew_alpha_more);
	R_bm.SetParameter("03_alpha", thenew_alpha_less);
}

// MELISSA LOOK HERE!!!  YOU HAVE TO FIX NORM TOO!!!
TF1 * makedist_norm_tp()
{
	TF1 * f_norm_tp = new TF1("f_norm_tp", "[A]*(1-exp(-(x-[x0])/[tau])) + [m]*x + [b]", 0, 6000.0);
	f_norm_tp -> SetLineColor(kBlack);
	f_norm_tp -> SetLineWidth(1);

	// tp:
	f_norm_tp -> SetParameter("A",      81007.1);  // tp
	f_norm_tp -> SetParameter("b",      72614.6);
	f_norm_tp -> SetParameter("m",     -0.830193);
	f_norm_tp -> SetParameter("tau",    639.861);
	f_norm_tp -> SetParameter("x0",     1000.0);
	
	return f_norm_tp;
}
TF1 * makedist_norm_bp()
{
	TF1 * f_norm_bp = new TF1("f_norm_bp", "[A]*(1-exp(-(x-[x0])/[tau])) + [m]*x + [b]", 0, 6000.0);

	f_norm_bp -> SetLineColor(kBlack);
	f_norm_bp -> SetLineWidth(1);
	// bp:
	f_norm_bp -> SetParameter("A",      344609);  // bp
	f_norm_bp -> SetParameter("b",      258793);
	f_norm_bp -> SetParameter("m",     -3.57134);
	f_norm_bp -> SetParameter("tau",    740.548);
	f_norm_bp -> SetParameter("x0",     1000.0);

//	TF1 * f_norm_bp = new TF1("f_norm_bp", "0.5*(1-TMath::Erf((x-[xe])/[se]))*[A1]*exp(x/[tau1]) + 0.5*(1+TMath::Erf((x-[xe])/[se]))*[A2]*(1-exp(-(x-[x0])/[tau2])) + [m]*x + [b]", 0, 6000.0);
//	f_norm_bp -> SetParameter("A1",      3232.23);
//	f_norm_bp -> SetParameter("A2",      654027);
//	f_norm_bp -> SetParameter("b",       50);
//	f_norm_bp -> SetParameter("m",      -13.8672);
//	f_norm_bp -> SetParameter("se",      215.454);
//	f_norm_bp -> SetParameter("tau1",    550);
//	f_norm_bp -> SetParameter("tau2",    896.309);
//	f_norm_bp -> SetParameter("x0",      500);
//	f_norm_bp -> SetParameter("xe",      655.94);


	return f_norm_bp;
}
TF1 * makedist_norm_tm()
{
	TF1 * f_norm_tm = new TF1("f_norm_tm", "[A]*(1-exp(-(x-[x0])/[tau])) + [m]*x + [b]", 0, 6000.0);
	f_norm_tm -> SetLineColor(kBlack);
	f_norm_tm -> SetLineWidth(1);
	// tm:
	f_norm_tm -> SetParameter("A",      314843);  // tm
	f_norm_tm -> SetParameter("b",      237137);
	f_norm_tm -> SetParameter("m",     -2.88768);
	f_norm_tm -> SetParameter("tau",    731.353);
	f_norm_tm -> SetParameter("x0",     1000.0);
	
	return f_norm_tm;
}
TF1 * makedist_norm_bm()
{
	TF1 * f_norm_bm = new TF1("f_norm_bm", "[A]*(1-exp(-(x-[x0])/[tau])) + [m]*x + [b]", 0, 6000.0);
	f_norm_bm -> SetLineColor(kBlack);
	f_norm_bm -> SetLineWidth(1);
	// bm:
	f_norm_bm -> SetParameter("A",      90081.8);  // bm
	f_norm_bm -> SetParameter("b",      79733.1);
	f_norm_bm -> SetParameter("m",     -1.6509);
	f_norm_bm -> SetParameter("tau",    656.659);
	f_norm_bm -> SetParameter("x0",     1000.0);
	
	return f_norm_bm;
}
void SetParam_norm_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int)
{
	TF1 * f_norm_tp = makedist_norm_tp();
	TF1 * f_norm_bp = makedist_norm_bp();
	TF1 * f_norm_tm = makedist_norm_tm();
	TF1 * f_norm_bm = makedist_norm_bm();
	
	double thenew_norm_tp = f_norm_tp->Eval( (double)mono_int);
	double thenew_norm_bp = f_norm_bp->Eval( (double)mono_int);
	double thenew_norm_tm = f_norm_tm->Eval( (double)mono_int);
	double thenew_norm_bm = f_norm_bm->Eval( (double)mono_int);
	
	if( thenew_norm_tp<0 ) { thenew_norm_tp=0.0; }
	if( thenew_norm_bp<0 ) { thenew_norm_bp=0.0; }
	if( thenew_norm_tm<0 ) { thenew_norm_tm=0.0; }
	if( thenew_norm_bm<0 ) { thenew_norm_bm=0.0; }
	
	R_tp.SetParameter("01_norm", thenew_norm_tp);
	R_tm.SetParameter("01_norm", thenew_norm_tm);
	R_bp.SetParameter("01_norm", thenew_norm_bp);
	R_bm.SetParameter("01_norm", thenew_norm_bm);
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void FixAllParams(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	FixParam_E0(R_tp, R_tm, R_bp, R_bm);
	FixParam_norm(R_tp, R_tm, R_bp, R_bm);
	FixParam_scale(R_tp, R_tm, R_bp, R_bm);
	FixParam_alpha(R_tp, R_tm, R_bp, R_bm);
	FixParam_beta(R_tp, R_tm, R_bp, R_bm);
	FixParam_gamma(R_tp, R_tm, R_bp, R_bm);
	FixParam_delta(R_tp, R_tm, R_bp, R_bm);
	FixParam_W(R_tp, R_tm, R_bp, R_bm);
	FixParam_k(R_tp, R_tm, R_bp, R_bm);
	FixParam_gfrac(R_tp, R_tm, R_bp, R_bm);
	FixParam_toeres(R_tp, R_tm, R_bp, R_bm);
	FixParam_lres(R_tp, R_tm, R_bp, R_bm);
	FixParam_gres(R_tp, R_tm, R_bp, R_bm);  
	FixParam_DgE(R_tp, R_tm, R_bp, R_bm);  // should be zero.
	FixParam_dE0(R_tp, R_tm, R_bp, R_bm);
	// extra:
	FixParam_countin(R_tp, R_tm, R_bp, R_bm);
	FixParam_countout(R_tp, R_tm, R_bp, R_bm);
	FixParam_countint(R_tp, R_tm, R_bp, R_bm);
}
void ReleaseAllParams(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	ReleaseParam_E0(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_norm(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_scale(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_alpha(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_beta(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_gamma(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_delta(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_W(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_k(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_gfrac(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_toeres(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_lres(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_gres(R_tp, R_tm, R_bp, R_bm); 
	ReleaseParam_DgE(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_dE0(R_tp, R_tm, R_bp, R_bm);

	ReleaseParam_countin(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_countout(R_tp, R_tm, R_bp, R_bm);
	ReleaseParam_countint(R_tp, R_tm, R_bp, R_bm);
}
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void FixParam_E0(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("00_E0"), R_tp.GetParameter("00_E0") );
	R_tm . FixParameter(R_tm.GetParNumber("00_E0"), R_tm.GetParameter("00_E0") );
	R_bp . FixParameter(R_bp.GetParNumber("00_E0"), R_bp.GetParameter("00_E0") );
	R_bm . FixParameter(R_bm.GetParNumber("00_E0"), R_bm.GetParameter("00_E0") );
}
void FixParam_norm(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("01_norm"), R_tp.GetParameter("01_norm") );
	R_tm . FixParameter(R_tm.GetParNumber("01_norm"), R_tm.GetParameter("01_norm") );
	R_bp . FixParameter(R_bp.GetParNumber("01_norm"), R_bp.GetParameter("01_norm") );
	R_bm . FixParameter(R_bm.GetParNumber("01_norm"), R_bm.GetParameter("01_norm") );
}
void FixParam_scale(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("02_scale"), R_tp.GetParameter("02_scale") );
	R_tm . FixParameter(R_tm.GetParNumber("02_scale"), R_tm.GetParameter("02_scale") );
	R_bp . FixParameter(R_bp.GetParNumber("02_scale"), R_bp.GetParameter("02_scale") );
	R_bm . FixParameter(R_bm.GetParNumber("02_scale"), R_bm.GetParameter("02_scale") );
}
void FixParam_alpha(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("03_alpha"), R_tp.GetParameter("03_alpha") );
	R_tm . FixParameter(R_tm.GetParNumber("03_alpha"), R_tm.GetParameter("03_alpha") );
	R_bp . FixParameter(R_bp.GetParNumber("03_alpha"), R_bp.GetParameter("03_alpha") );
	R_bm . FixParameter(R_bm.GetParNumber("03_alpha"), R_bm.GetParameter("03_alpha") );
}
void FixParam_beta(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("04_beta"), R_tp.GetParameter("04_beta") );
	R_tm . FixParameter(R_tm.GetParNumber("04_beta"), R_tm.GetParameter("04_beta") );
	R_bp . FixParameter(R_bp.GetParNumber("04_beta"), R_bp.GetParameter("04_beta") );
	R_bm . FixParameter(R_bm.GetParNumber("04_beta"), R_bm.GetParameter("04_beta") );
}
void FixParam_gamma(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("05_gamma"), R_tp.GetParameter("05_gamma") );
	R_tm . FixParameter(R_tm.GetParNumber("05_gamma"), R_tm.GetParameter("05_gamma") );
	R_bp . FixParameter(R_bp.GetParNumber("05_gamma"), R_bp.GetParameter("05_gamma") );
	R_bm . FixParameter(R_bm.GetParNumber("05_gamma"), R_bm.GetParameter("05_gamma") );
}
void FixParam_delta(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("06_delta"), R_tp.GetParameter("06_delta") );
	R_tm . FixParameter(R_tm.GetParNumber("06_delta"), R_tm.GetParameter("06_delta") );
	R_bp . FixParameter(R_bp.GetParNumber("06_delta"), R_bp.GetParameter("06_delta") );
	R_bm . FixParameter(R_bm.GetParNumber("06_delta"), R_bm.GetParameter("06_delta") );
}
void FixParam_W(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("07_W"), R_tp.GetParameter("07_W") );
	R_tm . FixParameter(R_tm.GetParNumber("07_W"), R_tm.GetParameter("07_W") );
	R_bp . FixParameter(R_bp.GetParNumber("07_W"), R_bp.GetParameter("07_W") );
	R_bm . FixParameter(R_bm.GetParNumber("07_W"), R_bm.GetParameter("07_W") );
}
void FixParam_k(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("08_k"), R_tp.GetParameter("08_k") );
	R_tm . FixParameter(R_tm.GetParNumber("08_k"), R_tm.GetParameter("08_k") );
	R_bp . FixParameter(R_bp.GetParNumber("08_k"), R_bp.GetParameter("08_k") );
	R_bm . FixParameter(R_bm.GetParNumber("08_k"), R_bm.GetParameter("08_k") );
}
void FixParam_gfrac(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("09_gfrac"), R_tp.GetParameter("09_gfrac") );
	R_tm . FixParameter(R_tm.GetParNumber("09_gfrac"), R_tm.GetParameter("09_gfrac") );
	R_bp . FixParameter(R_bp.GetParNumber("09_gfrac"), R_bp.GetParameter("09_gfrac") );
	R_bm . FixParameter(R_bm.GetParNumber("09_gfrac"), R_bm.GetParameter("09_gfrac") );
}
void FixParam_toeres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("10_toeres"), R_tp.GetParameter("10_toeres") );
	R_tm . FixParameter(R_tm.GetParNumber("10_toeres"), R_tm.GetParameter("10_toeres") );
	R_bp . FixParameter(R_bp.GetParNumber("10_toeres"), R_bp.GetParameter("10_toeres") );
	R_bm . FixParameter(R_bm.GetParNumber("10_toeres"), R_bm.GetParameter("10_toeres") );
}
void FixParam_lres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("11_lres"), R_tp.GetParameter("11_lres") );
	R_tm . FixParameter(R_tm.GetParNumber("11_lres"), R_tm.GetParameter("11_lres") );
	R_bp . FixParameter(R_bp.GetParNumber("11_lres"), R_bp.GetParameter("11_lres") );
	R_bm . FixParameter(R_bm.GetParNumber("11_lres"), R_bm.GetParameter("11_lres") );
}
void FixParam_gres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("12_gres"), R_tp.GetParameter("12_gres") );
	R_tm . FixParameter(R_tm.GetParNumber("12_gres"), R_tm.GetParameter("12_gres") );
	R_bp . FixParameter(R_bp.GetParNumber("12_gres"), R_bp.GetParameter("12_gres") );
	R_bm . FixParameter(R_bm.GetParNumber("12_gres"), R_bm.GetParameter("12_gres") );
}
void FixParam_DgE(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("13_DgE"), R_tp.GetParameter("13_DgE") );
	R_tm . FixParameter(R_tm.GetParNumber("13_DgE"), R_tm.GetParameter("13_DgE") );
	R_bp . FixParameter(R_bp.GetParNumber("13_DgE"), R_bp.GetParameter("13_DgE") );
	R_bm . FixParameter(R_bm.GetParNumber("13_DgE"), R_bm.GetParameter("13_DgE") );
}
void FixParam_dE0(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("14_dE0"), R_tp.GetParameter("14_dE0") );
	R_tm . FixParameter(R_tm.GetParNumber("14_dE0"), R_tm.GetParameter("14_dE0") );
	R_bp . FixParameter(R_bp.GetParNumber("14_dE0"), R_bp.GetParameter("14_dE0") );
	R_bm . FixParameter(R_bm.GetParNumber("14_dE0"), R_bm.GetParameter("14_dE0") );
}
void FixParam_countin(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("15_countin"), R_tp.GetParameter("15_countin") );
	R_bp . FixParameter(R_bp.GetParNumber("15_countin"), R_bp.GetParameter("15_countin") );
	R_tm . FixParameter(R_tm.GetParNumber("15_countin"), R_tm.GetParameter("15_countin") );
	R_bm . FixParameter(R_bm.GetParNumber("15_countin"), R_bm.GetParameter("15_countin") );
}
void FixParam_countout(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("16_countout"), R_tp.GetParameter("16_countout") );
	R_bp . FixParameter(R_bp.GetParNumber("16_countout"), R_bp.GetParameter("16_countout") );
	R_tm . FixParameter(R_tm.GetParNumber("16_countout"), R_tm.GetParameter("16_countout") );
	R_bm . FixParameter(R_bm.GetParNumber("16_countout"), R_bm.GetParameter("16_countout") );
}
void FixParam_countint(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . FixParameter(R_tp.GetParNumber("17_countint"), R_tp.GetParameter("17_countint") );
	R_bp . FixParameter(R_bp.GetParNumber("17_countint"), R_bp.GetParameter("17_countint") );
	R_tm . FixParameter(R_tm.GetParNumber("17_countint"), R_tm.GetParameter("17_countint") );
	R_bm . FixParameter(R_bm.GetParNumber("17_countint"), R_bm.GetParameter("17_countint") );
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void ReleaseParam_E0(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("00_E0") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("00_E0") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("00_E0") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("00_E0") );
}
void ReleaseParam_norm(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("01_norm") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("01_norm") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("01_norm") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("01_norm") );
}
void ReleaseParam_scale(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("02_scale") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("02_scale") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("02_scale") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("02_scale") );
}
void ReleaseParam_alpha(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("03_alpha") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("03_alpha") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("03_alpha") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("03_alpha") );
}
void ReleaseParam_beta(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("04_beta") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("04_beta") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("04_beta") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("04_beta") );
}
void ReleaseParam_gamma(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("05_gamma") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("05_gamma") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("05_gamma") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("05_gamma") );
}
void ReleaseParam_delta(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("06_delta") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("06_delta") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("06_delta") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("06_delta") );
}
void ReleaseParam_W(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("07_W") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("07_W") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("07_W") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("07_W") );
}
void ReleaseParam_k(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("08_k") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("08_k") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("08_k") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("08_k") );
}
void ReleaseParam_gfrac(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("09_gfrac") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("09_gfrac") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("09_gfrac") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("09_gfrac") );
}
void ReleaseParam_toeres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("10_toeres") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("10_toeres") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("10_toeres") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("10_toeres") );
}
void ReleaseParam_lres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("11_lres") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("11_lres") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("11_lres") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("11_lres") );
}
void ReleaseParam_gres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("12_gres") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("12_gres") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("12_gres") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("12_gres") );
}
void ReleaseParam_DgE(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("13_DgE") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("13_DgE") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("13_DgE") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("13_DgE") );
	
//	cout << "Parameters released:  DgE.  Current value TP:  " << R_tp.GetParameter("13_DgE") << endl;
}
void ReleaseParam_dE0(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("14_dE0") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("14_dE0") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("14_dE0") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("14_dE0") );
}

void ReleaseParam_countin(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("15_countin") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("15_countin") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("15_countin") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("15_countin") );
}
void ReleaseParam_countout(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("16_countout") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("16_countout") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("16_countout") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("16_countout") );
}
void ReleaseParam_countint(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm)
{
	R_tp . ReleaseParameter(R_tp.GetParNumber("17_countint") );
	R_tm . ReleaseParameter(R_tm.GetParNumber("17_countint") );
	R_bp . ReleaseParameter(R_bp.GetParNumber("17_countint") );
	R_bm . ReleaseParameter(R_bm.GetParNumber("17_countint") );
}
