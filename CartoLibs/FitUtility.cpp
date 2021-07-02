#include "TF1.h"
#include "TMath.h"
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TROOT.h>

//#include "MapUtility.cpp"
//#include "MakeSomeTF1s.cpp"

#include "FitUtility.h"
#include "ColorExtras.h"

#include <string>
#include <iostream>  // cout
using std::string;
using std::cout;
using std::endl;


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#include "FitParam_Extras.cpp"

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void SetParamsLike_BB1Agree_highenergy(TF1 &R)
{
	// call as:  SetParamsLike_BB1Agree_highenergy(*R);
	// R.SetParameter("norm",  1.0);
	
	// --- // --- //
	R.SetParameter("00_E0",    4.19232e+03);  // just set it to a value.  I'll pre-set it according to monoenergy later.
	R.SetParLimits(R.GetParNumber("00_E0"),     0.0,   5.5e3);  // the broadest of possible physical limits.  we'll give it better limits later.

	R.SetParameter("01_norm",  1.0e6);
	R.SetParLimits(R.GetParNumber("01_norm"),   0.0,   1e7);  // norm can't go negative.
	
	R.SetParameter("02_scale", 1.5);
	R.SetParLimits(R.GetParNumber("02_scale"), 0, 3.0);  // scale can't go negative either.
		
	R.SetParameter("03_alpha", 300.0);
	R.SetParLimits(R.GetParNumber("03_alpha"),  100,   500.0);
	
	R.SetParameter("04_beta", 0.0);
	R.SetParLimits(R.GetParNumber("04_beta"), 0.0,   5);

	R.SetParameter("05_gamma", 0.00100);
//	R.SetParLimits(R.GetParNumber("05_gamma"),  0.00090,  0.00120);
	
	R.SetParameter("06_delta", 7.06801e-02);
//	R.SetParLimits(R.GetParNumber("06_delta"),  0.0,  0.03); 

	R.SetParameter("07_W",     380.0);
	R.SetParLimits(R.GetParNumber("07_W"),      200.0, 450.0);

	// 08_k
	R.SetParameter("08_k", 4.3);
	R.SetParLimits(R.GetParNumber("08_k"), 3.5,   12);  // 
	
	// 09_gfrac
	R.SetParameter("09_gfrac", 0.9);
	R.SetParLimits(R.GetParNumber("09_gfrac"), 0.6,   1.0);

	R.SetParameter("10_toeres", 1.0);  // set it according to top and bottom detector later.  But keep it fixed initially.
	R.SetParLimits(R.GetParNumber("10_toeres"),      0.5,   13.0);
	
	R.SetParameter("11_lres", 0.006);  // as a fraction of toeres
	R.SetParLimits(R.GetParNumber("11_lres"), 0.01,   0.1);  //
	
	// 12_gres
	R.SetParameter("12_gres", 1.0);
	R.SetParLimits(R.GetParNumber("12_gres"), 1.0e-4,   8.0);
	
	R.SetParameter("13_DgE", 0.0);
	R.SetParLimits(R.GetParNumber("13_DgE"),    -10.0,  15.0);
	
	R.SetParameter("14_dE0",      0.0);  
	R.SetParLimits(R.GetParNumber("14_dE0"),  -150.0,  50.0);

	//
	//
	R.SetParameter("15_countin",      -1.0);  
	R.SetParameter("16_countout",     -1.0);  
	R.SetParameter("17_countint",     -1.0);  
	
	return;
}


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
plotfuncs::plotfuncs()
{
	n_plot = 500;
// sigma^2 = lambda*E0

	string fmoyal_string = "(1.0-[03_alpha]/abs([00_E0])-[04_beta]/abs([00_E0])-[06_delta]/([05_gamma]*[07_W])-([05_gamma]*[07_W]))*(1-[09_gfrac])*((1.0/ sqrt(2.0*pi*([11_lres]*[12_gres])*abs([00_E0]-0.5*[14_dE0]+[13_DgE])) )*exp( -0.5*( -(x-([00_E0]-0.5*[14_dE0]+[13_DgE]) )/ (([11_lres]*[12_gres])*abs([00_E0]-0.5*[14_dE0]+[13_DgE]) ) )) * exp(-0.5*exp((x-([00_E0]-0.5*[14_dE0]+[13_DgE]) )/ (([11_lres]*[12_gres])*abs([00_E0]-0.5*[14_dE0]+[13_DgE])) )))";  // the "reversed" moyal function
	
	
	string flandau_string = "(1.0-[03_alpha]/abs([00_E0])-[04_beta]/abs([00_E0])-[06_delta]/([05_gamma]*[07_W])-([05_gamma]*[07_W]))*(1-[09_gfrac])*TMath::Landau(-1.0*x, -1.0*abs([00_E0]-0.5*[14_dE0]), sqrt(([11_lres]*[10_toeres])*abs([00_E0]-0.5*[14_dE0])) )*(pi/2.0)/sqrt(([11_lres]*[10_toeres])*abs([00_E0]-0.5*[14_dE0]))";  // built-in (reversed) landau distribution.  It now scales correctly with 'norm', and *probably* has the right overall value.  the pi/2 was a "guess", since it was close to the empirical measured value.
	
	string f1_string = "(1.0-[03_alpha]/abs([00_E0])-[04_beta]/abs([00_E0])-[06_delta]/([05_gamma]*[07_W])-([05_gamma]*[07_W]))*[09_gfrac]*( exp(-1.0*(x-([00_E0]+[13_DgE]+0.5*[14_dE0]))^2/(2.0*[12_gres]*[10_toeres]*abs([00_E0]+[13_DgE]+0.5*[14_dE0])) ) / ( sqrt(2.0*pi*[12_gres]*[10_toeres]*abs([00_E0]+[13_DgE]+0.5*[14_dE0])) ) )";  // the primary peak, clifford style.  A gaussian.
	
	string f2_string = "[03_alpha]/abs([00_E0])*( (1.0-TMath::Erf( (x-abs([00_E0]))/(sqrt(2.0*[10_toeres]*abs([00_E0]))) ) ) / ( 2.0*abs([00_E0])) )";  // long, flat, low energy tail

//	string f3_string = "( x<5900 || [00_E0]>300 ) * [04_beta]/abs([00_E0])*( (1.0-TMath::Erf( (x-[00_E0]+[10_toeres]*[08_k] )/sqrt(2.0*[10_toeres]*abs([00_E0])) )) * exp( [08_k]/abs([00_E0])*(x-[00_E0]) )*exp( [10_toeres]*[08_k]*[08_k]/abs([00_E0])/2.0 ) / (2.0*(1-exp(-1.0*[08_k]) )) )";
//	string f3_string = "[04_beta]/abs([00_E0])*( (1.0-TMath::Erf( (x-[00_E0]+[10_toeres]*[08_k] )/sqrt(2.0*[10_toeres]*abs([00_E0])) )) * exp( [08_k]/abs([00_E0])*(x-[00_E0]) )*exp( [10_toeres]*[08_k]*[08_k]/abs([00_E0])/2.0 ) / (2.0*(1-exp(-1.0*[08_k]) )) )";  // low-energy exponential tail.  now with k/E0.  ...simplified.
	
	string f3_string = "[04_beta]/abs([00_E0])*( (1.0-TMath::Erf( (x-[00_E0]+[10_toeres]*[08_k] )/sqrt(2.0*[10_toeres]*abs([00_E0])) )) * exp( [08_k]/abs([00_E0])*(x-[00_E0]) )*exp( [10_toeres]*[08_k]*[08_k]/abs([00_E0])/2.0 ) / (2.0*(1-exp(-1.0*[08_k]) )) )";  // low-energy exponential tail.  now with k/E0.  ...simplified.
	
	string f4_string = "([05_gamma]*[07_W])*(TMath::Erf( (x-[00_E0])/(sqrt(2.0*[10_toeres]*abs([00_E0]))) )/(2.0*[07_W]) - TMath::Erf( (x-[00_E0]-[07_W])/(sqrt(2.0*[10_toeres]*abs([00_E0]+[07_W]) )) )/(2.0*[07_W]) )";  // shelf

	string f5a_string = "( (x-[00_E0])*( TMath::Erf( (x-[00_E0])/(sqrt(2.0*[10_toeres]*abs([00_E0])))) - 2.0*TMath::Erf( (x-[00_E0]-[07_W])/(sqrt(2.0*[10_toeres]*abs([00_E0]+[07_W]) ))) + TMath::Erf( (x-[00_E0]-2.0*[07_W])/(sqrt(2.0*[10_toeres]*abs([00_E0]+2.0*[07_W]) ))) ) )";
	string f5b_string = "(2.0*[07_W])*( TMath::Erf( (x-[00_E0]-[07_W])/(sqrt(2.0*[10_toeres]*abs([00_E0]+[07_W]) ))) - TMath::Erf( (x-[00_E0]-2.0*[07_W])/(sqrt(2.0*[10_toeres]*abs([00_E0]+2.0*[07_W]) ))) )";
	string f5c_string = "(2.0*[10_toeres]*abs([00_E0]))*( (exp(-1.0*(x-[00_E0])^2/(4.0*[10_toeres]*abs([00_E0]))) / ( sqrt(2.0*pi*[10_toeres]*abs([00_E0])))) -2.0*(exp(-1.0*(x-[00_E0]-[07_W])^2/(4.0*[10_toeres]*abs([00_E0]+[07_W]) )) / ( sqrt(2.0*pi*[10_toeres]*abs([00_E0]+[07_W]) ))) + ( exp(-1.0*(x-[00_E0]-2.0*[07_W])^2/(4.0*[10_toeres]*abs([00_E0]+2.0*[07_W]) )) / ( sqrt(2.0*pi*[10_toeres]*abs([00_E0]+2.0*[07_W]) )) ) )";
	
	string f5_string = "[06_delta]/([05_gamma]*[07_W])*("+f5a_string+" + "+f5b_string+" + "+f5c_string+")/(2.0*[07_W]*[07_W])";  // triangle
	//
	
	string f_511string1 = "( 195.0*sqrt(2.0/pi) / 17.0 * exp(-(1.0/578.0)*(-308.0 + x)^2) )";// + 
	string f_511string2 = "(80.0 + 1.0/450.0 * (-210.0 + x)^2) * (0.5 - 0.5*TMath::Erf(1.0/30.0*(-334.0 + x)) )";// + 
	string f_511string3 = "1.0/360.0 * (-505.0 + x)^2 * (0.5 - 0.5* TMath::Erf(1.0/30.0*(-505.0 + x)) ) * (0.5 + 0.5* TMath::Erf(1.0/30.0*(-334.0 + x)) )"; 
	string f511_string = "sqrt([02_scale]*[02_scale]) * ("+f_511string1+" + "+f_511string2+" + "+f_511string3+")";
	
	
	string f_countin_string  = "[15_countin]";
	string f_countout_string = "[16_countout]";
	string f_countint_string = "[17_countint]";
	
	string f_extra_string = "0.0*("+f_countin_string+" + "+f_countout_string+" + "+f_countint_string+")";

	//								          1                    09_gfrac       03_alpha        04_beta/08_k    05_gamma        06_delta         02_scale
	string complete_ff_string = "[01_norm]*("+fmoyal_string+" + "+f1_string+" + "+f2_string+" + "+f3_string+" + "+f4_string+" + "+f5_string+") + "+f511_string+" + "+f_extra_string;  // landau primary + modified compton toe + 511s.
	
	
	// --- // 
	R_total_      = TF1("R",           (complete_ff_string).c_str(),         0.0, 6000.0);
	R_norm_       = TF1("R_norm",      (complete_ff_string).c_str(),         0.0, 6000.0);
	
	f_landau_     = TF1("f_landau",    ("[01_norm]*"+fmoyal_string).c_str(), 0.0, 6000.0);  // 00_E0 01_norm 06_delta 05_gamma 03_alpha 04_beta 09_gfrac 10_toeres 11_lres 14_dE0 13_DgE
	f1_clifford_  = TF1("f1_clifford", ("[01_norm]*"+f1_string).c_str(),     0.0, 6000.0);  // 00_E0 01_norm 06_delta 05_gamma 03_alpha 04_beta 09_gfrac 10_toeres 12_gres 13_DgE
	
	f2_clifford_  = TF1("f2_clifford", ("[01_norm]*"+f2_string).c_str(),     0.0, 6000.0);  // 00_E0 01_norm 10_toeres 03_alpha
	f3_clifford_  = TF1("f3_clifford", ("[01_norm]*"+f3_string).c_str(),     0.0, 6000.0);  // 00_E0 01_norm 10_toeres 04_beta 08_k
	f4_clifford_  = TF1("f4_clifford", ("[01_norm]*"+f4_string).c_str(),     0.0, 6000.0);  // 00_E0 01_norm 10_toeres 05_gamma 07_W 
	f5_clifford_  = TF1("f5_clifford", ("[01_norm]*"+f5_string).c_str(),     0.0, 6000.0);  // 00_E0 01_norm 10_toeres 06_delta 07_W 05_gamma
	
	f_511_        = TF1("f_511",       (f511_string).c_str(),                0.0, 6000.0);  // 02_scale
	
//	f_extra_      = TF1("f_extra",     (f_extra_string).c_str(),             0.0, 6000.0);  // 15_countin 16_countout 17_countint

	f_countin_   = TF1("f_extra_in_",     (f_countin_string).c_str(),        0.0, 6000.0);
	f_countout_  = TF1("f_extra_out_",    (f_countout_string).c_str(),       0.0, 6000.0);
	f_countint_  = TF1("f_extra_int_",    (f_countint_string).c_str(),       0.0, 6000.0);
	
	// some default parameters so we don't segfault later:
	SetParamsLike_BB1Agree_highenergy(R_total_);
	// Propagate them through...
	propagate_through();
	
	// set up fineness of plots:
	R_total_.SetNpx(n_plot);
	R_norm_.SetNpx(n_plot);
	f_landau_.SetNpx(n_plot);
	f1_clifford_.SetNpx(n_plot);
	f2_clifford_.SetNpx(n_plot);
	f3_clifford_.SetNpx(n_plot);
	f4_clifford_.SetNpx(n_plot);
	f5_clifford_.SetNpx(n_plot);
	f_511_.SetNpx(n_plot);
//	f_extra_.SetNpx(n_plot);
	f_countin_.SetNpx(n_plot);
	f_countout_.SetNpx(n_plot);
	f_countint_.SetNpx(n_plot);
	
	// Set up some colors ....
	R_total_.SetLineColor(kBlack);
	R_norm_.SetLineColor(kBlack);
	f_landau_.SetLineColor(kGreen);
	f1_clifford_.SetLineColor(kRed);      // gaussian
	f2_clifford_.SetLineColor(kMagenta);  // flat tail
	f3_clifford_.SetLineColor(kOrange);   // exponential tail
	f4_clifford_.SetLineColor(kBlue);     // shelf
	f5_clifford_.SetLineColor(kGray);     // triangle
	f_511_.SetLineColor(kCyan);
	
	f_countin_.SetLineColor(int(mOrange));
	f_countout_.SetLineColor(int(kBlue));
	f_countint_.SetLineColor(int(mLavender));
}



// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
TF1* make_response_function()
{
	plotfuncs * the_tmp_plotfuncs = new plotfuncs();
	TF1 * R = the_tmp_plotfuncs->R_total();
	return R;
	
}























































