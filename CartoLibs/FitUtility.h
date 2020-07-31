#ifndef INCLUDE_FITUTILITY
#define INCLUDE_FITUTILITY 1


#include "TF1.h"
#include "TRandom3.h"

#include <string>
using std::string;

//#include "TCanvas.h"
#include "/Users/anholm/Packages/MiscLibs/ColorExtras.h"

#include <iostream>  // cout
using std::string;
using std::cout;
using std::endl;


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

TF1 * makedist_norm_tp();
TF1 * makedist_norm_bp();
TF1 * makedist_norm_tm();
TF1 * makedist_norm_bm();
TF1 * makedist_alpha_more();
TF1 * makedist_alpha_less();
TF1 * makedist_beta();
TF1 * makedist_gfrac();
TF1 * makedist_lres();
TF1 * makedist_scale_more();
TF1 * makedist_scale_less();
TF1 * makedist_delta_top();
TF1 * makedist_delta_bottom();
TF1 * makedist_toeres_top();
TF1 * makedist_toeres_bottom();
TF1 * makedist_k();
TF1 * makedist_dE0();
//TF1 * makedist_gamma();
TF1 * makedist_gamma_more();
TF1 * makedist_gamma_less();
TF1 * makedist_DgE();
TF1 * makedist_gres();
TF1 * makedist_W();
TF1 * makedist_E0();




void SetParam_E0_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_norm_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_scale_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_alpha_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_beta_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
//void SetParam_beta_tmp(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_gamma_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_delta_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_W_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_k_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_gfrac_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_toeres_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_lres_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_gres_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_DgE_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_dE0_again(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void SetParamsLike_BB1Agree_highenergy(TF1 &R);

void SetParam_E0_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_norm_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_scale_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_alpha_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_beta_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_gamma_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_delta_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_W_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_k_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_gfrac_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_toeres_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_lres_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_gres_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_DgE_tmp_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);
void SetParam_dE0_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);

void SetAllParams_like_bb1agree(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int mono_int);

//
void FixParam_E0(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_norm(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_scale(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_alpha(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_beta(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_gamma(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_delta(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_W(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_k(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_gfrac(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_toeres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_lres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_gres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_DgE(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_dE0(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);

void FixParam_countin(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_countout(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixParam_countint(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);

void FixCountParams(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void FixAllParams(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);


void ReleaseParam_E0(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_norm(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_scale(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_alpha(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_beta(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_gamma(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_delta(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_W(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_k(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_gfrac(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_toeres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_lres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_gres(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_DgE(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_dE0(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);

void ReleaseParam_countin(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_countout(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);
void ReleaseParam_countint(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);

void ReleaseAllParams(TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm);


TF1* make_response_function();

TF1* make_relnorm(string);
TF1* make_relnorm_tp();
TF1* make_relnorm_tm();
TF1* make_relnorm_bp();
TF1* make_relnorm_bm();

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
class relnorm_funcs
{
public:
	void initialize()
	{
		TF1 * f = new TF1();
		
		f = make_relnorm_tp();
		relnorm_tp_ = *f;
		f = make_relnorm_tm();
		relnorm_tm_ = *f;
		f = make_relnorm_bp();
		relnorm_bp_ = *f;
		f = make_relnorm_bm();
		relnorm_bm_ = *f;
		
		relnorm_tp_.SetLineColor(int(mOrange));
		relnorm_bp_.SetLineColor(int(mGreen));
		relnorm_tm_.SetLineColor(int(kAzure));
		relnorm_bm_.SetLineColor(int(kMagenta));
	};

//	void draw()
//	{
//		TCanvas * c = new TCanvas("c", "c", 100, 0, 900, 700);
//		relnorm_bp_.Draw();
//		relnorm_tm_.Draw("same");
///		relnorm_tp_.Draw("same");
//		relnorm_bm_.Draw("same");
//		gPad->Update();
//	}
	
public:
	TF1 * relnorm_tp() { return &relnorm_tp_; };
	TF1 * relnorm_tm() { return &relnorm_tm_; };
	TF1 * relnorm_bp() { return &relnorm_bp_; };
	TF1 * relnorm_bm() { return &relnorm_bm_; };
	
private:
	TF1 relnorm_tp_;
	TF1 relnorm_tm_;
	TF1 relnorm_bp_;
	TF1 relnorm_bm_;
};

class plotfuncs
{
public:
	plotfuncs();
	void load_parameters(TF1 * R_in)
	{
		TF1 * R_tmp = (TF1*)R_in->Clone();
		R_total_ = *R_tmp;
		propagate_through();
	};
	void load_default_parameters()
	{
		TF1 * R_tmp = make_response_function();
		R_total_ = *R_tmp;
		propagate_through();
	};
	void set_params_and_propagate( double _00_E0, double _01_norm, double _02_scale, \
		double _03_alpha, double _04_beta, double _05_gamma, double _06_delta, double _07_W, \
		double _08_k, double _09_gfrac, double _10_toeres, double _11_lres, double _12_gres, \
		double _13_DgE, double _14_dE0, double _15_countin, double _16_countout, double _17_countint)
	{
		R_total_. SetParameter("00_E0",       _00_E0);
		R_total_. SetParameter("01_norm",     _01_norm);
		R_total_. SetParameter("02_scale",    _02_scale);
		R_total_. SetParameter("03_alpha",    _03_alpha);
		R_total_. SetParameter("04_beta",     _04_beta);
		R_total_. SetParameter("05_gamma",    _05_gamma);
		R_total_. SetParameter("06_delta",    _06_delta);
		R_total_. SetParameter("07_W",        _07_W);
		R_total_. SetParameter("08_k",        _08_k);
		R_total_. SetParameter("09_gfrac",    _09_gfrac);
		R_total_. SetParameter("10_toeres",   _10_toeres);
		R_total_. SetParameter("11_lres",     _11_lres);
		R_total_. SetParameter("12_gres",     _12_gres);
		R_total_. SetParameter("13_DgE",      _13_DgE);
		R_total_. SetParameter("14_dE0",      _14_dE0);
		//
		R_total_. SetParameter("15_countin",  _15_countin);
		R_total_. SetParameter("16_countout", _16_countout);
		R_total_. SetParameter("17_countint", _17_countint);
		//
		propagate_through();
	}
	void adjust_norm( double norm_adjustment )  // this will take "relative norm".
	{
		double new_norm  = R_total_.GetParameter("01_norm")  * norm_adjustment;
		double new_scale = R_total_.GetParameter("02_scale") * norm_adjustment; 
		R_norm_ . SetParameter("01_norm",         new_norm);
		R_norm_ . SetParameter("02_scale",        new_scale);
	};
	void propagate_through()
	{
		f_landau_ . SetParameter("00_E0",     R_total_.GetParameter("00_E0"));
		f_landau_ . SetParameter("01_norm",   R_total_.GetParameter("01_norm"));
		f_landau_ . SetParameter("06_delta",  R_total_.GetParameter("06_delta"));
		f_landau_ . SetParameter("05_gamma",  R_total_.GetParameter("05_gamma"));
		f_landau_ . SetParameter("03_alpha",  R_total_.GetParameter("03_alpha"));
		f_landau_ . SetParameter("04_beta",   R_total_.GetParameter("04_beta"));
		f_landau_ . SetParameter("09_gfrac",  R_total_.GetParameter("09_gfrac"));
		f_landau_ . SetParameter("10_toeres", R_total_.GetParameter("10_toeres"));
		f_landau_ . SetParameter("11_lres",   R_total_.GetParameter("11_lres"));
		f_landau_ . SetParameter("12_gres",   R_total_.GetParameter("12_gres"));
		
		f_landau_ . SetParameter("14_dE0",    R_total_.GetParameter("14_dE0"));
		f_landau_ . SetParameter("13_DgE",    R_total_.GetParameter("13_DgE"));
		f_landau_ . SetParameter("07_W",      R_total_.GetParameter("07_W"));
		
		// f1 is the gaussian.
		f1_clifford_ . SetParameter("00_E0",     R_total_.GetParameter("00_E0"));
		f1_clifford_ . SetParameter("01_norm",   R_total_.GetParameter("01_norm"));
		f1_clifford_ . SetParameter("11_lres",   R_total_.GetParameter("11_lres"));
		f1_clifford_ . SetParameter("06_delta",  R_total_.GetParameter("06_delta"));
		f1_clifford_ . SetParameter("05_gamma",  R_total_.GetParameter("05_gamma"));
		f1_clifford_ . SetParameter("03_alpha",  R_total_.GetParameter("03_alpha"));
		f1_clifford_ . SetParameter("04_beta",   R_total_.GetParameter("04_beta"));
		f1_clifford_ . SetParameter("09_gfrac",  R_total_.GetParameter("09_gfrac"));
		f1_clifford_ . SetParameter("10_toeres", R_total_.GetParameter("10_toeres"));
		f1_clifford_ . SetParameter("12_gres",   R_total_.GetParameter("12_gres"));
		f1_clifford_ . SetParameter("13_DgE",    R_total_.GetParameter("13_DgE"));
		f1_clifford_ . SetParameter("07_W",      R_total_.GetParameter("07_W"));
		
		f2_clifford_ . SetParameter("00_E0",     R_total_.GetParameter("00_E0"));
		f2_clifford_ . SetParameter("01_norm",   R_total_.GetParameter("01_norm"));
		f2_clifford_ . SetParameter("03_alpha",  R_total_.GetParameter("03_alpha"));
		f2_clifford_ . SetParameter("10_toeres", R_total_.GetParameter("10_toeres"));
	
		f3_clifford_ . SetParameter("00_E0",     R_total_.GetParameter("00_E0"));
		f3_clifford_ . SetParameter("01_norm",   R_total_.GetParameter("01_norm"));
		f3_clifford_ . SetParameter("10_toeres", R_total_.GetParameter("10_toeres"));
		f3_clifford_ . SetParameter("04_beta",   R_total_.GetParameter("04_beta"));
		f3_clifford_ . SetParameter("08_k",      R_total_.GetParameter("08_k"));
	
		f4_clifford_ . SetParameter("00_E0",     R_total_.GetParameter("00_E0"));
		f4_clifford_ . SetParameter("01_norm",   R_total_.GetParameter("01_norm"));
		f4_clifford_ . SetParameter("10_toeres", R_total_.GetParameter("10_toeres"));
		f4_clifford_ . SetParameter("05_gamma",  R_total_.GetParameter("05_gamma"));
		f4_clifford_ . SetParameter("07_W",      R_total_.GetParameter("07_W"));

		f5_clifford_ . SetParameter("00_E0",     R_total_.GetParameter("00_E0"));
		f5_clifford_ . SetParameter("01_norm",   R_total_.GetParameter("01_norm"));
		f5_clifford_ . SetParameter("10_toeres", R_total_.GetParameter("10_toeres"));
		f5_clifford_ . SetParameter("06_delta",  R_total_.GetParameter("06_delta"));
		f5_clifford_ . SetParameter("07_W",      R_total_.GetParameter("07_W"));
		f5_clifford_ . SetParameter("05_gamma",  R_total_.GetParameter("05_gamma"));
	
		f_511_       . SetParameter("02_scale", R_total_.GetParameter("02_scale"));
		
		f_countin_   . SetParameter("15_countin",  R_total_.GetParameter("15_countin"));
		f_countout_  . SetParameter("16_countout", R_total_.GetParameter("16_countout"));
		f_countint_  . SetParameter("17_countint", R_total_.GetParameter("17_countint"));
		
		double new_norm  = R_total_.GetParameter("01_norm")  * R_total_.GetParameter("17_countint") / R_total_.GetParameter("15_countin");
		double new_scale = R_total_.GetParameter("02_scale") * R_total_.GetParameter("17_countint") / R_total_.GetParameter("15_countin");
		
		R_norm_ . SetParameter("00_E0",           R_total_.GetParameter("00_E0"));
//		R_norm_ . SetParameter("01_norm",         R_total_.GetParameter("01_norm"));
		R_norm_ . SetParameter("01_norm",         new_norm);
//		R_norm_ . SetParameter("02_scale",        R_total_.GetParameter("02_scale"));
		R_norm_ . SetParameter("02_scale",        new_scale);
		R_norm_ . SetParameter("03_alpha",        R_total_.GetParameter("03_alpha"));
		R_norm_ . SetParameter("04_beta",         R_total_.GetParameter("04_beta"));
		R_norm_ . SetParameter("05_gamma",        R_total_.GetParameter("05_gamma"));
		R_norm_ . SetParameter("06_delta",        R_total_.GetParameter("06_delta"));
		R_norm_ . SetParameter("07_W",            R_total_.GetParameter("07_W"));
		R_norm_ . SetParameter("08_k",            R_total_.GetParameter("08_k"));
		R_norm_ . SetParameter("09_gfrac",        R_total_.GetParameter("09_gfrac"));
		R_norm_ . SetParameter("10_toeres",       R_total_.GetParameter("10_toeres"));
		R_norm_ . SetParameter("11_lres",         R_total_.GetParameter("11_lres"));
		R_norm_ . SetParameter("12_gres",         R_total_.GetParameter("12_gres"));
		R_norm_ . SetParameter("13_DgE",          R_total_.GetParameter("13_DgE"));
		R_norm_ . SetParameter("14_dE0",          R_total_.GetParameter("14_dE0"));
		R_norm_ . SetParameter("15_countin",      R_total_.GetParameter("15_countin"));
		R_norm_ . SetParameter("16_countout",     R_total_.GetParameter("16_countout"));
		R_norm_ . SetParameter("17_countint",     R_total_.GetParameter("17_countint"));
	};
	
	TF1 * R_total()     { return &R_total_; };
	TF1 * R_norm()      { return &R_norm_; };
	TF1 * f_landau()    { return &f_landau_; };
	TF1 * f1_clifford() { return &f1_clifford_; };
	TF1 * f2_clifford() { return &f2_clifford_; };
	TF1 * f3_clifford() { return &f3_clifford_; };
	TF1 * f4_clifford() { return &f4_clifford_; };
	TF1 * f5_clifford() { return &f5_clifford_; };
	TF1 * f_511()       { return &f_511_; };
	
	TF1 * f_countin()   { return &f_countin_;  };
	TF1 * f_countout()  { return &f_countout_; };
	TF1 * f_countint()  { return &f_countint_; };
	
	
private:
	TF1 R_total_;
	TF1 R_norm_;
	TF1 f_landau_;  // or moyal
	TF1 f1_clifford_;
	TF1 f2_clifford_;
	TF1 f3_clifford_;
	TF1 f4_clifford_;
	TF1 f5_clifford_;
	TF1 f_511_;
	
	TF1 f_countin_; 
	TF1 f_countout_;
	TF1 f_countint_;

	int n_plot;
};



// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#endif














































