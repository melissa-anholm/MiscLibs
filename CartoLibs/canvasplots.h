#ifndef INCLUDE_CANVASPLOTS_H
#define INCLUDE_CANVASPLOTS_H 1


#include <vector>
#include <string>
#include <iostream>  // cout
#include <math.h>

#include "TCanvas.h"
#include <TFile.h>
#include "TPaveText.h"
#include "TStyle.h"
#include "TLegend.h"
#include <TH1.h>
#include "TGraph.h"
#include <TF1.h>

#include <TFitResult.h>
#include <TFitResultPtr.h>

#include "MapUtility.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

string int_to_string(int);


//#define __SHORT_FORM_OF_FILE__ \
// (strrchr(__FILE__,'/') ? strrchr(__FILE__,'/')+1 : __FILE__ )
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //


class energy_specifier
{
public:
	energy_specifier(string ts, int ti, double td)
	{
		the_string = ts;
		the_double = td;
		the_int    = ti;
	};
	
	string the_string;
	double the_double;
	int the_int;
};
struct set_of_fitvectors  // size of the vectors is the number of monoenergies we've fit to.
{
	int color;
	
	vector<double> fcn_vec;
	vector<double> chi2_vec;
	vector<double> redchi2_vec;
	vector<double> ndf_vec;
	vector<int>    convergence_vec;
	vector<double> P_vec;
	//
	vector<double> E0_vec;     // 00_E0
	vector<double> E0_err;
	vector<double> W_vec;      // 07_W
	vector<double> W_err;
	vector<double> alpha_vec;  // 03_alpha
	vector<double> alpha_err;
	vector<double> beta_vec;   // 04_beta
	vector<double> beta_err;
	vector<double> delta_vec;  // 06_delta
	vector<double> delta_err;
	vector<double> gamma_vec;  // 05_gamma
	vector<double> gamma_err;
	vector<double> k_vec;      // 08_k
	vector<double> k_err;
	vector<double> lres_vec;   // 11_lres
	vector<double> lres_err;
	vector<double> norm_vec;   // 01_norm
	vector<double> norm_err;
	// 
	vector<double> scale_vec;  // 02_scale
	vector<double> scale_err;   
	
	vector<double> gsize_vec;  // 09_gfrac
	vector<double> gsize_err;

	vector<double> gres_vec;   // 12_gres
	vector<double> gres_err;

//	vector<double> lambda_b_vec;
//	vector<double> lambda_b_err;
	
	vector<double> toeres_vec;  // 10_toeres
	vector<double> toeres_err;
	
	vector<double> dE_vec;      // 14_dE0
	vector<double> dE_err;

	vector<double> Dgcenter_vec;  // 13_DgE
	vector<double> Dgcenter_err;
	
	//
	vector<double> countin_vec;   // 15_countin
	vector<double> countout_vec;  // 16_countout
	vector<double> countint_vec;  // 17_countint
	
	vector<double> relnorm_vec;   // ~ 17_countint / 15_countin
	
	//
	void clear_all()
	{
		color = kBlack;
		fcn_vec.clear();
		chi2_vec.clear();
		redchi2_vec.clear();
		ndf_vec.clear();
		convergence_vec.clear();
		P_vec.clear();
		//
		E0_vec.clear();     // E0, param0.
		E0_err.clear();
		W_vec.clear();      // W, param1.
		W_err.clear();
		alpha_vec.clear();  // alpha, param2.
		alpha_err.clear();
		beta_vec.clear();   // beta, param3.
		beta_err.clear();
		delta_vec.clear();  // delta, param4.
		delta_err.clear();
		gamma_vec.clear();  // gamma, param5.
		gamma_err.clear();
		k_vec.clear();      // k, param6.
		k_err.clear();
		lres_vec.clear();   // 11_lres.
		lres_err.clear();
		norm_vec.clear();   // norm, param8.
		norm_err.clear();
		
		scale_vec.clear();
		scale_err.clear();
		
		toeres_vec.clear();
		toeres_err.clear();
		
		dE_vec.clear();
		dE_err.clear();
		
		gsize_vec.clear();
		gsize_err.clear();

		gres_vec.clear();
		gres_err.clear();
		
		Dgcenter_vec.clear(); // 14_Dgcenter
		Dgcenter_err.clear();
		
		//
		countin_vec.clear();
		countout_vec.clear();
		countint_vec.clear();
		
		relnorm_vec.clear();
		//
		
	};
	void increment_withfitpointer(TFitResultPtr fr)  // doesn't do anything to the x's.  how do I get fcn and chi2 ?
	{
		fcn_vec.push_back( fr->MinFcnValue() );
		chi2_vec.push_back( fr->Chi2() );
		redchi2_vec.push_back( fr->Chi2()/((double)fr->Ndf()) );
		ndf_vec.push_back( (double)fr->Ndf() );
		convergence_vec.push_back( fr->IsValid() );
		
		double Pval = 0.5*(1.0 - TMath::Erf( sqrt(2.0*(fr->Chi2())) - sqrt(2.0*(fr->Ndf())-1.0) ));
		P_vec.push_back( Pval );
		
		E0_vec.push_back(fr->Parameter(0));       // 00_E0
		E0_err.push_back(fr->Error(0));
		
		norm_vec.push_back(fr->Parameter(1));     // 01_norm
		norm_err.push_back(fr->Error(1));
		
		scale_vec.push_back(fr->Parameter(2));    // 02_scale
		scale_err.push_back(fr->Error(2));
		
		// 03_alpha
		alpha_vec.push_back(fr->Parameter(3));    // 03_alpha
		alpha_err.push_back(fr->Error(3));
		// 04_beta
		beta_vec.push_back(fr->Parameter(4));     // 04_beta
		beta_err.push_back(fr->Error(4));
		
		gamma_vec.push_back(fr->Parameter(5));    // 05_gamma
		gamma_err.push_back(fr->Error(5));
		
		delta_vec.push_back(fr->Parameter(6));    // 06_delta
		delta_err.push_back(fr->Error(6));
		
		W_vec.push_back(fr->Parameter(7));        // 07_W
		W_err.push_back(fr->Error(7));
		
		// 08_k
		k_vec.push_back(fr->Parameter(8)); 
		k_err.push_back(fr->Error(8));

		gsize_vec.push_back(fr->Parameter(9));     // 09_gfrac
		gsize_err.push_back(fr->Error(9));
		
		toeres_vec.push_back(fr->Parameter(10));   // 10_toeres
		toeres_err.push_back(fr->Error(10));
		
		lres_vec.push_back(fr->Parameter(11));     // 11_lres
		lres_err.push_back(fr->Error(11));
		
		gres_vec.push_back(fr->Parameter(12));     // 12_gres
		gres_err.push_back(fr->Error(12));
		
		Dgcenter_vec.push_back(fr->Parameter(13)); // 13_DgE
		Dgcenter_err.push_back(fr->Error(13));

		dE_vec.push_back(fr->Parameter(14));        // 14_dE0
		dE_err.push_back(fr->Error(14)); 
		
		// 
		countin_vec.push_back(fr->Parameter(15));   // 15_countin
		countout_vec.push_back(fr->Parameter(16));  // 16_countout
		countint_vec.push_back(fr->Parameter(17));  // 17_countint
		
		relnorm_vec.push_back( fr->Parameter(17) / fr->Parameter(15) );
	};
};
struct set_of_fitresult_pointers
{
	TFitResultPtr pointer_tp;
	TFitResultPtr pointer_tm;
	TFitResultPtr pointer_bp;
	TFitResultPtr pointer_bm;
	
	void print_fitconvergence()
	{
	//	cout << "Convergence Summary @ E = " << monoenergy_int << " keV:  " << endl;
		cout << "Top (+):    IsValid = " << pointer_tp -> IsValid() << ";\tStatus = " << pointer_tp -> Status() << endl;
		cout << "Top (-):    IsValid = " << pointer_tm -> IsValid() << ";\tStatus = " << pointer_tm -> Status() << endl;
		cout << "Bottom (+): IsValid = " << pointer_bp -> IsValid() << ";\tStatus = " << pointer_bp -> Status() << endl;
		cout << "Bottom (-): IsValid = " << pointer_bm -> IsValid() << ";\tStatus = " << pointer_bm -> Status() << endl;
	};
};


extern set_of_fitresult_pointers get_fitresults(double the_energy_double, bool rcut);

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
TCanvas * make_better_residucan(TH1D * datahist, TH1D * monohist, string namestub, energy_specifier the_monospecifier, int N_rebin=1);

//
void makesave_E0ResiduCan(TFile * f, set_of_fitvectors, set_of_fitvectors, set_of_fitvectors, set_of_fitvectors, vector<double>, vector<double>);
void makesave_PseudoE0_Can(TFile * f, set_of_fitvectors, set_of_fitvectors, set_of_fitvectors, set_of_fitvectors, vector<double>, vector<double>);

void makesave_fitresultgraphs_and_cans(TFile * f, bool rcut);
void makesave_fitresultgraphs_and_cans(TFile * f, set_of_fitvectors, set_of_fitvectors, set_of_fitvectors, set_of_fitvectors, vector<double>, vector<double>);

//
TCanvas * make_plotcanvas_p(MapSetup * the_map, int N_rebin=1, bool plot_fitfuncs=false, string canvasname="cp", int monoenergy_int=0);
TCanvas * make_plotcanvas_m(MapSetup * the_map, int N_rebin=1, bool plot_fitfuncs=false, string canvasname="cm", int monoenergy_int=0);
TCanvas * make_bb1agreecanvas_p(MapSetup * the_map, int N_rebin=1, bool plot_fitfuncs=true, string canvasname="pfc_p", int monoenergy_int=0);
TCanvas * make_bb1agreecanvas_m(MapSetup * the_map, int N_rebin=1, bool plot_fitfuncs=true, string canvasname="pfc_m", int monoenergy_int=0);

#endif
