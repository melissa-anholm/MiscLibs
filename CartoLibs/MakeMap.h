// ==================================================================== //
// Code by Melissa Anholm
// 
// 
// ==================================================================== //
#include <ncurses.h>

#include "TGraph.h"
#include "TGraphErrors.h"

#include "location.cpp"
#include "MetaChain.cpp"

#include "canvasplots.h"  //
#include "MapUtility.h"   // 

#include "FitUtility.h"
#include "make_showing_plots.cpp"
//#include "CountingCanvases.cpp"
#include "AsymmetryCanvasLibs.cpp"
#include "GraphExtras.h"

#include "TStyle.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TRandom3.h"

#include "FitFits.cpp"


#include <algorithm>    // std::max
using std::max;
using std::min;

//using std::vector;
using std::pair;
using std::make_pair;

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#define __SHORT_FORM_OF_FILE__ \
(strrchr(__FILE__,'/') ? strrchr(__FILE__,'/')+1 : __FILE__ )
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

//TF1  * make_superratio(TF1*  f_tp, TF1*  f_tm, TF1*  f_bp, TF1*  f_bm);
TH1D * make_superratio_direct(TH1D* h_tp, TH1D* h_tm, TH1D* h_bp, TH1D* h_bm)
{
	int plotmarkerstyle=7;
	TH1D * h_ratio = make_superratio_histogram(h_tp, h_tm, h_bp, h_bm, string("Superratio"), int(kBlack), plotmarkerstyle);
	h_ratio -> SetOption("E3");
//	h_ratio -> SetFillColor(kGray);
	
	return h_ratio;
}

//TF1  * make_supersum(TF1*  f_tp, TF1*  f_tm, TF1*  f_bp, TF1*  f_bm);
TH1D * make_supersum_direct(TH1D* h_tp, TH1D* h_tm, TH1D* h_bp, TH1D* h_bm)
{
	int plotmarkerstyle=7;
	TH1D * h_sum = make_supersum_histogram(h_tp, h_tm, h_bp, h_bm, string("Supersum"), int(kBlack), plotmarkerstyle);
	h_sum -> SetOption("E3");
//	h_sum -> SetFillColor(kGray);
	
	return h_sum;
}

//TF1  * make_superratioasymmetry(TF1*  f_tp, TF1*  f_tm, TF1*  f_bp, TF1*  f_bm);
TH1D * make_superratioasymmetry_direct(TH1D* h_tp, TH1D* h_tm, TH1D* h_bp, TH1D* h_bm)  // do this to get an asymmetry from the full spectrum.
{
	int plotmarkerstyle=7;
	TH1D * h_asym = make_asymmetry_histogram(h_tp, h_tm, h_bp, h_bm, string("Superratio Asymmetry"), int(kBlack), plotmarkerstyle);
//	h_asym -> SetOption("E3");
//	h_asym -> SetFillColor(kGray);
	h_asym -> GetYaxis()->SetRangeUser(-0.62, -0.455);
	
	return h_asym;
}


vector<energy_specifier> load_energies()
{
	vector<energy_specifier> the_energyset;
	the_energyset.push_back(energy_specifier( "full", -10, -10)); // 
	
	the_energyset.push_back(energy_specifier( "250",  250, 0.25));
	the_energyset.push_back(energy_specifier( "500",  500, 0.5));
	the_energyset.push_back(energy_specifier( "625",  625, 0.625));
	the_energyset.push_back(energy_specifier( "750",  750, 0.75));
	the_energyset.push_back(energy_specifier( "875",  875, 0.875));
	the_energyset.push_back(energy_specifier("1000", 1000, 1.0));
	the_energyset.push_back(energy_specifier("1125", 1125, 1.125));
	the_energyset.push_back(energy_specifier("1250", 1250, 1.25));
	the_energyset.push_back(energy_specifier("1375", 1375, 1.375));
	the_energyset.push_back(energy_specifier("1500", 1500, 1.5));
	the_energyset.push_back(energy_specifier("1750", 1750, 1.75));
	the_energyset.push_back(energy_specifier("2000", 2000, 2.0));
	the_energyset.push_back(energy_specifier("2500", 2500, 2.5));
	the_energyset.push_back(energy_specifier("3000", 3000, 3.0));
	the_energyset.push_back(energy_specifier("3500", 3500, 3.5));
	the_energyset.push_back(energy_specifier("4000", 4000, 4.0));
	the_energyset.push_back(energy_specifier("4500", 4500, 4.5));
	the_energyset.push_back(energy_specifier("5000", 5000, 5.0));
	
	return the_energyset;
}
vector<energy_specifier> the_energyset = load_energies();



// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void makesave_maphist(string namestub, double the_monoenergy)
{
	MapSetup * my_new_map = new MapSetup();
	TChain * TreeChain = get_chain_from_monoenergy(the_monoenergy, true);
	my_new_map -> LoadFromTree(TreeChain);
	my_new_map -> AdjustTheColors();
	//
	my_new_map -> save_to_file( make_mapname_from_monoenergy(namestub, the_monoenergy) );
	delete my_new_map;
}

void makesave_allthemaphists(string namestub, bool skipsome)
{
	for(int i=0; i<the_energyset.size(); i++)
	{
		int energy = the_energyset.at(i).the_int;
		if(skipsome)  // these are the ones we'll skip:
		{
			if( energy==5000 ) { continue; }  // skip this energy
			if( energy==4500 ) { continue; }  // skip this energy
			if( energy==4000 ) { continue; }  // skip this energy
			if( energy==3500 ) { continue; }  // skip this energy  
			if( energy==3000 ) { continue; }  // skip this energy  // 
			if( energy==2500 ) { continue; }  // skip this energy  // 
			if( energy==2000 ) { continue; }  // skip this energy  // 
			if( energy==1750 ) { continue; }  // skip this energy  // 
			if( energy==1500 ) { continue; }  // skip this energy  // 
			if( energy==1375 ) { continue; }  // skip this energy  // 
			if( energy==1250 ) { continue; }  // skip this energy  // 
			if( energy==1125 ) { continue; }  // skip this energy  
			if( energy==1000 ) { continue; }  // skip this energy  // 
			if( energy== 875 ) { continue; }  // skip this energy  
			if( energy== 750 ) { continue; }  // skip this energy  // whatever, I'm not even looking through this now.
		//	if( energy== 625 ) { continue; }  // skip this energy 
			if( energy== 500 ) { continue; }  // skip this energy  // doesn't even converge
			if( energy== 250 ) { continue; }  // skip this energy 
			if( energy== -10 ) { continue; }  // skip this energy  
		}
		makesave_maphist(namestub, the_energyset.at(i).the_double);
	}
//	makesave_fullspectrum_maphist("map_out_full");
}

set_of_fitresult_pointers makefitset(TH1D* htp, TH1D* htm, TH1D* hbp, TH1D* hbm, int monoenergy_int, TF1 &R_tp, TF1 &R_tm, TF1 &R_bp, TF1 &R_bm, int fitmax_algorithm=3, bool finalfit=true)
//	call as, eg:  set_of_fitresult_pointers fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm);  
//	this syntax allows the original TF1s to be modified in place.
{
	if( !htp || !htm || !hbp || !hbm ) // this isn't actually a failure mode...
	{
		cout << "Can't find the histogram(s)!" << endl;  
	}
	
//	double lambda_top    = 1.42;
//	double lambda_bottom = 1.32;
	
	double fitmin =  200.0;
	double fitmax = 5500.0;
	
	set_of_fitresult_pointers fitset;
	//
	if(fitmax_algorithm==0)
	{ 
	//	cout << "fitmax algorithm is zero." << endl;
	}
	else if(fitmax_algorithm==1)
	{
		fitmax = monoenergy_int + 1000;
	}
	else if(fitmax_algorithm==2)
	{
		fitmax = monoenergy_int + 500;
	}
	else if(fitmax_algorithm==3)
	{
		int i_tp, i_tm, i_bp, i_bm, i_max;
		
		i_tp = htp->FindLastBinAbove(0);
		i_tm = htm->FindLastBinAbove(0);
		i_bp = hbp->FindLastBinAbove(0);
		i_bm = hbm->FindLastBinAbove(0);
		
		i_max = max( max(i_tp, i_tm), max(i_bp, i_bm));
		i_max++;  // first bin that we don't have to include.
		fitmax = htp->GetBinCenter(i_max);
		
		cout << "i_max for monoenergy:  " << monoenergy_int << " is:  " << i_max;// << endl;
		cout << ";\tfitmax is:  " << fitmax << endl;
	}
	else
	{
		cout << "fitmax_algorithm = " << fitmax_algorithm << " is not a recognized value." << endl;
		cout << "You fail." << endl;
		assert(0);
		return fitset;
	}
	bool determined = true;	
	// L makes it a likelihood fit (can be combined with "M" and "E").  By default it will probably call Migrad.
	// M calls Hesse.  ('improves fit results', by searching for the best local minimum.)
	// E calls Minos.  (better error estimates)
	// --- * --- // --- * --- // --- * --- //
	cout << "... ... ..." << endl;
	cout << "Fitting Top Plus @ " << monoenergy_int << " keV:  " << endl;
	fitset.pointer_tp = htp->Fit(&R_tp, "S", "", fitmin, fitmax);
	
	if(determined && !(fitset.pointer_tp->IsValid()) )
	{
		cout << "Trying again." << endl;
		fitset.pointer_tp = htp->Fit(&R_tp, "S", "", fitmin, fitmax);
	}
	if(finalfit)
	{
		fitset.pointer_tp = htp->Fit(&R_tp, "ES", "", fitmin, fitmax); 
	}
	
	//
	cout << "..." << endl;
	cout << "Fitting Top Minus @ " << monoenergy_int << " keV:  " << endl;
	fitset.pointer_tm = htm->Fit(&R_tm, "S", "", fitmin, fitmax); 
	
	if(determined && !(fitset.pointer_tm->IsValid()) )
	{
		cout << "Trying again." << endl;
		fitset.pointer_tm = htm->Fit(&R_tm, "S", "", fitmin, fitmax); 
	}
	if(finalfit)
	{
		fitset.pointer_tm = htm->Fit(&R_tm, "ES", "", fitmin, fitmax); 
	}
	
	// --- * --- // --- * --- // --- * --- //
	cout << endl << "... ..." << endl;
	cout << "Fitting Bottom Plus @ " << monoenergy_int << " keV:  " << endl;
	fitset.pointer_bp = hbp->Fit(&R_bp, "S", "", fitmin, fitmax); 
	
	if(determined && !(fitset.pointer_bp->IsValid()) )
	{
		cout << "Trying again." << endl;
		fitset.pointer_bp = hbp->Fit(&R_bp, "S", "", fitmin, fitmax); 
	}
	if(finalfit)
	{
		fitset.pointer_bp = hbp->Fit(&R_bp, "ES", "", fitmin, fitmax); 
	}
	
	//
	cout << "..." << endl;
	cout << "Fitting Bottom Minus @ " << monoenergy_int << " keV:  " << endl;
	fitset.pointer_bm = hbm->Fit(&R_bm, "S", "", fitmin, fitmax); 
	
	if(determined && !(fitset.pointer_bm->IsValid()) )
	{
		cout << "Trying again." << endl;
		fitset.pointer_bm = hbm->Fit(&R_bm, "S", "", fitmin, fitmax); 
	}
	if(finalfit)
	{
		fitset.pointer_bm = hbm->Fit(&R_bm, "ES", "", fitmin, fitmax); 
	}
	return fitset;
}

set_of_fitresult_pointers fit_fitset(MapSetup * my_new_map, int monoenergy_int, bool rcut=false, int fitmax_algorithm=0, int N_rebin=1)
{
	set_of_fitresult_pointers fitset;
	TH1D* htp;
	TH1D* hbp;
	TH1D* htm;
	TH1D* hbm;
	
	if(rcut)
	{
		htp = my_new_map->measured_EnergyT_p_bb1_r155_samedet_one;
		hbp = my_new_map->measured_EnergyB_p_bb1_r155_samedet_one;
		htm = my_new_map->measured_EnergyT_m_bb1_r155_samedet_one;
		hbm = my_new_map->measured_EnergyB_m_bb1_r155_samedet_one;
	}
	else
	{
		htp = my_new_map->measured_EnergyT_p_bb1agree;
		htm = my_new_map->measured_EnergyT_m_bb1agree;
		hbp = my_new_map->measured_EnergyB_p_bb1agree;
		hbm = my_new_map->measured_EnergyB_m_bb1agree;
	}
	
	// bin counts?  the simple ones first...
	TH1D* hnaive_tp = my_new_map->naive_EnergyT_p_hist;
	TH1D* hnaive_bp = my_new_map->naive_EnergyB_p_hist;
	TH1D* hnaive_tm = my_new_map->naive_EnergyT_m_hist;
	TH1D* hnaive_bm = my_new_map->naive_EnergyB_m_hist;
//	double xmin=200;
//	double xmax=6000;
//	double integration_min = 200.0;
//	double integration_max = monoenergy_int + 2000.0;
	double the_binmin = update_binmin(1, (double)monoenergy_int);
	double the_binmax = update_binmax(1, (double)monoenergy_int);
//	double binwidth = 6000.0/(1.0);
	double binwidth = 6000.0 / (6000.0/((double)N_rebin));
	
	cout << endl;
	cout << "mono_energy:  " << monoenergy_int << ";\t binmin = " << the_binmin << ";\tbinmax = " << the_binmax << ";\tbinwidth = " << binwidth << endl;
	cout << "hist limits:  " << the_binmin+binwidth/2.0 << ", " << the_binmax-binwidth/2.0 << endl;
	
	double N_in_tp = hnaive_tp->Integral(hnaive_tp->FindBin(the_binmin+binwidth/2.0),hnaive_tp->FindBin(the_binmax-binwidth/2.0) );
	double N_in_bp = hnaive_bp->Integral(hnaive_bp->FindBin(the_binmin+binwidth/2.0),hnaive_bp->FindBin(the_binmax-binwidth/2.0) );
	double N_in_tm = hnaive_tm->Integral(hnaive_tm->FindBin(the_binmin+binwidth/2.0),hnaive_tm->FindBin(the_binmax-binwidth/2.0) );
	double N_in_bm = hnaive_bm->Integral(hnaive_bm->FindBin(the_binmin+binwidth/2.0),hnaive_bm->FindBin(the_binmax-binwidth/2.0) );
	
	double N_out_tp = htp->Integral(htp->FindBin(the_binmin+binwidth/2.0),htp->FindBin(the_binmax-binwidth/2.0) );
	double N_out_bp = hbp->Integral(hbp->FindBin(the_binmin+binwidth/2.0),hbp->FindBin(the_binmax-binwidth/2.0) );
	double N_out_tm = htm->Integral(htm->FindBin(the_binmin+binwidth/2.0),htm->FindBin(the_binmax-binwidth/2.0) );
	double N_out_bm = hbm->Integral(hbm->FindBin(the_binmin+binwidth/2.0),hbm->FindBin(the_binmax-binwidth/2.0) );
	
	
	cout << " N_in_tp = " << N_in_tp << endl;
	cout << " N_out_tp = " << N_out_tp << endl;
	
	// make the 4 response functions.
	// by default ....
//	double lambda_top    = 1.42;
//	double lambda_bottom = 1.32;
	
	
	// Original Fit Function Setup:
	TF1 * R = make_response_function();
	R->SetLineColor(kBlack);
	R -> SetParameter("00_E0", monoenergy_int-300.0);
//	R -> SetParameter("17_countint", monoenergy_int/1000.0);
	

	TF1 * R_tp = (TF1*)R->Clone();
	TF1 * R_tm = (TF1*)R->Clone();
	TF1 * R_bp = (TF1*)R->Clone();
	TF1 * R_bm = (TF1*)R->Clone();
	// --- * --- // --- * --- // --- * --- //
	

	// --- * --- //
	// 00_E0
	SetParam_E0_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_E0(*R_tp, *R_tm, *R_bp, *R_bm);
	// 01_norm
	SetParam_norm_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);
	// 02_scale
	SetParam_scale_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);  // only two scale values.   Maybe I need 4?
	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	// 03_alpha
	SetParam_alpha_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_alpha(*R_tp, *R_tm, *R_bp, *R_bm);
	// 04_beta
	SetParam_beta_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_beta(*R_tp, *R_tm, *R_bp, *R_bm);
	// 05_gamma
	SetParam_gamma_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int); // this may be a mistake... whatever, we'll fix it later.
	FixParam_gamma(*R_tp, *R_tm, *R_bp, *R_bm); 
	// 06_delta
	SetParam_delta_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);  // this value is kludgy.
	FixParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);
	// 07_W
	SetParam_W_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_W(*R_tp, *R_tm, *R_bp, *R_bm);
	// 08_k
	SetParam_k_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);  // only one 'k' value.  maybe I need two?
	FixParam_k(*R_tp, *R_tm, *R_bp, *R_bm);
	// 09_gfrac
	SetParam_gfrac_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_gfrac(*R_tp, *R_tm, *R_bp, *R_bm);
	// 10_toeres
	SetParam_toeres_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_toeres(*R_tp, *R_tm, *R_bp, *R_bm);
	// 11_lres
	SetParam_lres_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_lres(*R_tp, *R_tm, *R_bp, *R_bm);  
	// 12_gres
	SetParam_gres_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);  // gres is just 1.0.
	FixParam_gres(*R_tp, *R_tm, *R_bp, *R_bm);  
	// 13_DgE
	SetParam_DgE_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);  // DgE is just 0.0.
	FixParam_DgE(*R_tp, *R_tm, *R_bp, *R_bm);
	// 14_dE0
	SetParam_dE0_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);  // what even is this parameter?  -- space between gauss and moyal
	FixParam_dE0(*R_tp, *R_tm, *R_bp, *R_bm);
	//
	R_tp -> SetParameter("15_countin", N_in_tp);
	R_bp -> SetParameter("15_countin", N_in_bp);
	R_tm -> SetParameter("15_countin", N_in_tm);
	R_bm -> SetParameter("15_countin", N_in_bm);
	FixParam_countin(*R_tp, *R_tm, *R_bp, *R_bm);
	//
	R_tp -> SetParameter("16_countout", N_out_tp);
	R_bp -> SetParameter("16_countout", N_out_bp);
	R_tm -> SetParameter("16_countout", N_out_tm);
	R_bm -> SetParameter("16_countout", N_out_bm);
	FixParam_countout(*R_tp, *R_tm, *R_bp, *R_bm);
	//
	FixParam_countint(*R_tp, *R_tm, *R_bp, *R_bm);
//	cout << "Before r1:  " << "R_tp->GetParameter(\"17_countint\") = " << R_tp->GetParameter("17_countint") << endl;
	
	// -- - -- // -- - -- // -- - -- //
	
	
//	ReleaseParam_beta(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("04_beta"), 0.1, 1.5);
//	R_tm -> SetParLimits(R_tm->GetParNumber("04_beta"), 0.1, 1.5);
//	R_bp -> SetParLimits(R_bp->GetParNumber("04_beta"), 0.1, 1.5);
//	R_bm -> SetParLimits(R_bm->GetParNumber("04_beta"), 0.1, 1.5);
	
//	ReleaseParam_k(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("08_k"), 0, 15);
//	R_tm -> SetParLimits(R_tm->GetParNumber("08_k"), 0, 15);
//	R_bp -> SetParLimits(R_bp->GetParNumber("08_k"), 0, 15);
//	R_bm -> SetParLimits(R_bm->GetParNumber("08_k"), 0, 15);
	
//	ReleaseParam_alpha(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("03_alpha"), 20.0, 400);
//	R_tm -> SetParLimits(R_tm->GetParNumber("03_alpha"), 20.0, 400);
//	R_bp -> SetParLimits(R_bp->GetParNumber("03_alpha"), 20.0, 400);
//	R_bm -> SetParLimits(R_bm->GetParNumber("03_alpha"), 20.0, 400);
	
//	ReleaseParam_toeres(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("10_toeres"), 1.0, 8);
//	R_tm -> SetParLimits(R_tm->GetParNumber("10_toeres"), 1.0, 8);
//	R_bp -> SetParLimits(R_bp->GetParNumber("10_toeres"), 1.0, 8);
//	R_bm -> SetParLimits(R_bm->GetParNumber("10_toeres"), 1.0, 8);
	
//	ReleaseParam_W(*R_tp, *R_tm, *R_bp, *R_bm);  
//	R_tp -> SetParLimits(R_tp->GetParNumber("07_W"), 320, 400);
//	R_tm -> SetParLimits(R_tm->GetParNumber("07_W"), 320, 400);
//	R_bp -> SetParLimits(R_bp->GetParNumber("07_W"), 320, 400);
//	R_bm -> SetParLimits(R_bm->GetParNumber("07_W"), 320, 400);
	
//	ReleaseParam_gres(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("12_gres"), 0, 1.2);
//	R_tm -> SetParLimits(R_tm->GetParNumber("12_gres"), 0, 1.2);
//	R_bp -> SetParLimits(R_bp->GetParNumber("12_gres"), 0, 1.2);
//	R_bm -> SetParLimits(R_bm->GetParNumber("12_gres"), 0, 1.2);
	
//	ReleaseParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);  
//	R_tp -> SetParLimits(R_tp->GetParNumber("06_delta"), 0.0, 0.05);
//	R_tm -> SetParLimits(R_tm->GetParNumber("06_delta"), 0.0, 0.05);
//	R_bp -> SetParLimits(R_bp->GetParNumber("06_delta"), 0.0, 0.05);
//	R_bm -> SetParLimits(R_bm->GetParNumber("06_delta"), 0.0, 0.05);
//	ReleaseParam_gamma(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("05_gamma"), 0.001, 0.0014);
//	R_tm -> SetParLimits(R_tm->GetParNumber("05_gamma"), 0.001, 0.0014);
//	R_bp -> SetParLimits(R_bp->GetParNumber("05_gamma"), 0.001, 0.0014);
//	R_bm -> SetParLimits(R_bm->GetParNumber("05_gamma"), 0.001, 0.0014);


//	ReleaseParam_dE0(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("14_dE0"), -20.0, 90.0);
//	R_tm -> SetParLimits(R_tm->GetParNumber("14_dE0"), -20.0, 90.0);
//	R_bp -> SetParLimits(R_bp->GetParNumber("14_dE0"), -20.0, 90.0);
//	R_bm -> SetParLimits(R_bm->GetParNumber("14_dE0"), -20.0, 90.0);
//	ReleaseParam_gfrac(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("09_gfrac"), 0, 1);
//	R_tm -> SetParLimits(R_tm->GetParNumber("09_gfrac"), 0, 1);
//	R_bp -> SetParLimits(R_bp->GetParNumber("09_gfrac"), 0, 1);
//	R_bm -> SetParLimits(R_bm->GetParNumber("09_gfrac"), 0, 1);
//	ReleaseParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);
	
//	ReleaseParam_lres(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("11_lres"), 0.01, 0.2);
//	R_tm -> SetParLimits(R_tm->GetParNumber("11_lres"), 0.01, 0.2);
//	R_bp -> SetParLimits(R_bp->GetParNumber("11_lres"), 0.01, 0.2);
//	R_bm -> SetParLimits(R_bm->GetParNumber("11_lres"), 0.01, 0.2);

	// -- - -- // -- - -- // -- - -- //
	cout << "Fitting Round 1:  " << endl;  
	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);
	// --- * --- //
	
	
	/*
//	ReleaseParam_DgE(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("13_DgE"), -10, 5);
//	R_tm -> SetParLimits(R_tm->GetParNumber("13_DgE"), -10, 5);
//	R_bp -> SetParLimits(R_bp->GetParNumber("13_DgE"), -10, 5);
//	R_bm -> SetParLimits(R_bm->GetParNumber("13_DgE"), -10, 5);
//	ReleaseParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);
//	ReleaseParam_gfrac(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("09_gfrac"), 0.3, 0.95);
//	R_tm -> SetParLimits(R_tm->GetParNumber("09_gfrac"), 0.3, 0.95);
//	R_bp -> SetParLimits(R_bp->GetParNumber("09_gfrac"), 0.3, 0.95);
//	R_bm -> SetParLimits(R_bm->GetParNumber("09_gfrac"), 0.3, 0.95);
	
	
//	ReleaseParam_alpha(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("03_alpha"), 20.0, 400);
//	R_tm -> SetParLimits(R_tm->GetParNumber("03_alpha"), 20.0, 400);
//	R_bp -> SetParLimits(R_bp->GetParNumber("03_alpha"), 20.0, 400);
//	R_bm -> SetParLimits(R_bm->GetParNumber("03_alpha"), 20.0, 400);
	*/
	// --- * --- // --- * --- // --- * --- //
	
	
	
	
	// norm is good now.  
//	FixParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);
	
	// ok, now that norm is fit and fixed and set, what else do we re-try?
	//  ... gfrac, probably. ... fixed.
	// try scale again.  ... fixed.
	// and then .... something else?  lres?
	// before lres, do toeres.
	// toeres is tweaked, but lres is fine.  
	// trying alpha now.  it was still pretty good (though needs tweaks), and it doesn't improve the fits.  I'll leave it alone.
	// trying gres.  ... no change, and it still looks very close.  yay!
	// might need to do something about gamma, particularly at 625 it doesn't look great.  don't think it ns improves fits, but it's better.
	// after gamma, we'd typically try W or delta again.  let's try both at once.  Actually, what if we do all 3 at once?
	
// toeres .... fixed post-norm.
//	ReleaseParam_toeres(*R_tp, *R_tm, *R_bp, *R_bm);
//	R_tp -> SetParLimits(R_tp->GetParNumber("10_toeres"), 1.0, 8);
//	R_tm -> SetParLimits(R_tm->GetParNumber("10_toeres"), 1.0, 8);
//	R_bp -> SetParLimits(R_bp->GetParNumber("10_toeres"), 1.0, 8);
//	R_bm -> SetParLimits(R_bm->GetParNumber("10_toeres"), 1.0, 8);
//	W ... fixed post-norm.
// delta is re-fixed post-norm, with two values rather than one.  (might need four?  idk.)
// gamma is also adjusted post-norm.


	
	
	// should I fix delta up here??
//	SetParam_delta_bb1agree(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
//		FixParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);

	
	// old:  No, toeres is still fine, possibly with the exception of for very low energies?
	// new:  it's a bit different now.  still can't tell if it behaves at low energies though.  -- fixed.
	
//	cout << "Fitting Round 2:  " << endl;
//	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);  
	
	
	
	/*
	// unfix k -- helps some, but not completely.	
	
	// unfixing beta also helps some, but not completely.  
	ReleaseParam_beta(*R_tp, *R_tm, *R_bp, *R_bm);
	R_tp -> SetParLimits(R_tp->GetParNumber("04_beta"), 0.4, 1.5);
	R_tm -> SetParLimits(R_tm->GetParNumber("04_beta"), 0.4, 1.5);
	R_bp -> SetParLimits(R_bp->GetParNumber("04_beta"), 0.4, 1.5);
	R_bm -> SetParLimits(R_bm->GetParNumber("04_beta"), 0.4, 1.5);
	
	// unfixing both beta and k is good, but it doesn't help *enough*...
	*/
	
	
//	// unfixing alpha too has very little effect.  its values don't come out right anymore though, even though it was almost the last one set.
	// what else is worth releasing and re-fixing?
//	ReleaseParam_W(*R_tp, *R_tm, *R_bp, *R_bm);  // only matches at high energies now.
//	R_tp -> SetParLimits(R_tp->GetParNumber("07_W"), 320, 400);
//	R_tm -> SetParLimits(R_tm->GetParNumber("07_W"), 320, 400);
//	R_bp -> SetParLimits(R_bp->GetParNumber("07_W"), 320, 400);
//	R_bm -> SetParLimits(R_bm->GetParNumber("07_W"), 320, 400);
	

//	R_tp -> SetParLimits(R_tp->GetParNumber("00_E0"), (monoenergy_int-300.0)-100, (monoenergy_int-300.0)+40 );
//	R_tm -> SetParLimits(R_tm->GetParNumber("00_E0"), (monoenergy_int-300.0)-100, (monoenergy_int-300.0)+40 );
//	R_bp -> SetParLimits(R_bp->GetParNumber("00_E0"), (monoenergy_int-300.0)-100, (monoenergy_int-300.0)+40 );
//	R_bm -> SetParLimits(R_bm->GetParNumber("00_E0"), (monoenergy_int-300.0)-100, (monoenergy_int-300.0)+40 );

	// could fit or fix beta now.  beta_tmp only captures behaviour at high energy.
	// better just fix it to what it came out to be, if we're going to fix it to anything.  

	
//	cout << "After fitset 2, we find DgE TP has value:  " << 

	/*
	ReleaseAllParams(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);
	
	
	FixParam_gres(*R_tp, *R_tm, *R_bp, *R_bm);  
	FixParam_DgE(*R_tp, *R_tm, *R_bp, *R_bm);
	// fix E0 ?
	FixParam_k(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);

	
	// fix 14_dE0 too ?  
//	// fix W too?
//	FixParam_W(*R_tp, *R_tm, *R_bp, *R_bm);
	
	SetParam_delta_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
	FixParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);
	
	// some params shouldn't vary so far:
	R_tp -> SetParLimits(R_tp->GetParNumber("00_E0"), (monoenergy_int-300.0)-100, (monoenergy_int-300.0)+40 );
	R_tm -> SetParLimits(R_tm->GetParNumber("00_E0"), (monoenergy_int-300.0)-100, (monoenergy_int-300.0)+40 );
	R_bp -> SetParLimits(R_bp->GetParNumber("00_E0"), (monoenergy_int-300.0)-100, (monoenergy_int-300.0)+40 );
	R_bm -> SetParLimits(R_bm->GetParNumber("00_E0"), (monoenergy_int-300.0)-100, (monoenergy_int-300.0)+40 );

//	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm); // leave floating, otherwise lots of things break.  apparently.
//	R_tp -> SetParLimits(R_tp->GetParNumber("02_scale"), 0, 3);
//	R_tm -> SetParLimits(R_tm->GetParNumber("02_scale"), 0, 3);
//	R_bp -> SetParLimits(R_bp->GetParNumber("02_scale"), 0, 3);
//	R_bm -> SetParLimits(R_bm->GetParNumber("02_scale"), 0, 3);
	
	R_tp -> SetParLimits(R_tp->GetParNumber("05_gamma"), 0.0008, 0.0014);
	R_tm -> SetParLimits(R_tm->GetParNumber("05_gamma"), 0.0008, 0.0014);
	R_bp -> SetParLimits(R_bp->GetParNumber("05_gamma"), 0.0008, 0.0014);
	R_bm -> SetParLimits(R_bm->GetParNumber("05_gamma"), 0.0008, 0.0014);


	R_tp -> SetParLimits(R_tp->GetParNumber("07_W"), 340, 410);
	R_tm -> SetParLimits(R_tm->GetParNumber("07_W"), 340, 410);
	R_bp -> SetParLimits(R_bp->GetParNumber("07_W"), 340, 410);
	R_bm -> SetParLimits(R_bm->GetParNumber("07_W"), 340, 410);
	

	
	// 12_gres = 1
	// 13_DgE = 0
	// 14_dE0 = const.  16-ish?
	
	
	cout << "Fitting Round 2:  " << endl;
	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);  
	// scale should be better after this round.  can we release norm too??  .. no, no point.
	// actually, toeres was pretty ugly after this.  Let's fix W...
	// anyway, we got delta.  I think.  does that make our W any better?
	
//	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	
//	SetParam_delta_again(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);
//	FixParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);
	
	cout << "Fitting Round 3:  " << endl;
	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);  
	*/
	
	/*
//	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	
	//	ReleaseParam_toeres(*R_tp, *R_tm, *R_bp, *R_bm);
	// leave toeres fixed, along with norm and scale.  see what happens to lres, W, delta, gamma.
	
	//
	ReleaseParam_E0(*R_tp, *R_tm, *R_bp, *R_bm);

	ReleaseParam_alpha(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_beta(*R_tp, *R_tm, *R_bp, *R_bm);
	
	ReleaseParam_gamma(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_W(*R_tp, *R_tm, *R_bp, *R_bm);
	
	ReleaseParam_gfrac(*R_tp, *R_tm, *R_bp, *R_bm);
	
	ReleaseParam_lres(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_dE0(*R_tp, *R_tm, *R_bp, *R_bm);  // separation between gaussian and moyal
	
//	ReleaseParam_toeres(*R_tp, *R_tm, *R_bp, *R_bm);
	
	// this 'beta' setting makes chi2's worse.
//	SetParam_beta_tmp(*R_tp, *R_tm, *R_bp, *R_bm, monoenergy_int);  // this result changes 'k'..
//	FixParam_beta(*R_tp, *R_tm, *R_bp, *R_bm);
	*/
		
	
//	cout << "Fitting Round 3:  " << endl;
//	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);  
	
	/*
	FixAllParams(*R_tp, *R_tm, *R_bp, *R_bm);
//	ReleaseParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);

//	FixParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);
//	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	*/
	
	/*
	// try to see if norm and scale have improved/changed any...
	FixAllParams(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);

	cout << "Fitting Round 3:  " << endl;
	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);  

	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	*/

	
	/*
	if(monoenergy_int==250)
	{
		FixAllParams(*R_tp, *R_tm, *R_bp, *R_bm);
		ReleaseParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
		
		R_tp -> SetParameter("02_scale", 0.2);
		R_tm -> SetParameter("02_scale", 0.2);
		R_bp -> SetParameter("02_scale", 0.2);
		R_bm -> SetParameter("02_scale", 0.2);
		
		R_tp -> SetParLimits(R_tp->GetParNumber("02_scale"), 1.0e-8, 0.5);
		R_tm -> SetParLimits(R_tm->GetParNumber("02_scale"), 1.0e-8, 0.5);
		R_bp -> SetParLimits(R_bp->GetParNumber("02_scale"), 1.0e-8, 0.5);
		R_bm -> SetParLimits(R_bm->GetParNumber("02_scale"), 1.0e-8, 0.5);
	}
	*/
	
//	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
//	FixParam_lres(*R_tp, *R_tm, *R_bp, *R_bm);  // looks nice and smooth, even though it's not a real function.
	
//	R_tp -> SetParameter("00_E0", monoenergy_int-300.0);
//	R_tm -> SetParameter("00_E0", monoenergy_int-300.0);
//	R_bp -> SetParameter("00_E0", monoenergy_int-300.0);
//	R_bm -> SetParameter("00_E0", monoenergy_int-300.0);
	
	
//	// actually, to lowest order, gres is probably about 1 -- especially if we let toeres float.
//	R_tp -> SetParameter("12_gres", 1.0);
//	R_tm -> SetParameter("12_gres", 1.0);
//	R_bp -> SetParameter("12_gres", 1.0);
//	R_bm -> SetParameter("12_gres", 1.0);
//	FixParam_gres(*R_tp, *R_tm, *R_bp, *R_bm);  
	
	/*
	if(monoenergy_int==250)
	{
		FixAllParams(*R_tp, *R_tm, *R_bp, *R_bm);
		ReleaseParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	}
	if(monoenergy_int==500)
	{
		FixAllParams(*R_tp, *R_tm, *R_bp, *R_bm);
		ReleaseParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	}
	*/
	
	/*
	R_tp -> SetParameter("00_E0", monoenergy_int-300.0);
	R_tm -> SetParameter("00_E0", monoenergy_int-300.0);
	R_bp -> SetParameter("00_E0", monoenergy_int-300.0);
	R_bm -> SetParameter("00_E0", monoenergy_int-300.0);
	FixParam_E0(*R_tp, *R_tm, *R_bp, *R_bm);
	
	FixParam_DgE(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_lres(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_toeres(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_gres(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_W(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_dE0(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_gfrac(*R_tp, *R_tm, *R_bp, *R_bm);  // at firstfit gfrac comes out about the same.
	FixParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);  // at firstfit delta comes out about the same.
	
	cout << "Fitting Round 1:  " << endl;  
	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);  // get norm and scale.
	
	if(monoenergy_int<=500)
	{
		cout << "We're stopping early @ E = " << monoenergy_int << " keV, before we can fuck it up:  " << endl;
		fitset.print_fitconvergence();
		return fitset;
	}
	
	ReleaseAllParams(*R_tp, *R_tm, *R_bp, *R_bm);
	
	FixParam_norm(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_scale(*R_tp, *R_tm, *R_bp, *R_bm);
	
//	FixParam_E0(*R_tp, *R_tm, *R_bp, *R_bm);  // can adjust it later?
	FixParam_DgE(*R_tp, *R_tm, *R_bp, *R_bm);
	*/
	
	/*
	FixParam_alpha(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_beta(*R_tp, *R_tm, *R_bp, *R_bm);
	FixParam_gamma(*R_tp, *R_tm, *R_bp, *R_bm);
//	FixParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);
	
	FixParam_dE0(*R_tp, *R_tm, *R_bp, *R_bm);

	//
	ReleaseParam_gfrac(*R_tp, *R_tm, *R_bp, *R_bm);  
	ReleaseParam_delta(*R_tp, *R_tm, *R_bp, *R_bm);  
	
	ReleaseParam_toeres(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_W(*R_tp, *R_tm, *R_bp, *R_bm);
	ReleaseParam_DgE(*R_tp, *R_tm, *R_bp, *R_bm);

	// 
//	ReleaseParam_W(*R_tp, *R_tm, *R_bp, *R_bm);
	*/
	
	
//	cout << "Fitting Round 2:  " << endl;
//	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);  


	// -- - -- // -- - -- // -- - -- //
	double N_int_tp = R_tp->Integral(the_binmin, the_binmax);
	double N_int_bp = R_bp->Integral(the_binmin, the_binmax);
	double N_int_tm = R_tm->Integral(the_binmin, the_binmax);
	double N_int_bm = R_bm->Integral(the_binmin, the_binmax);
	R_tp -> SetParameter("17_countint", N_int_tp);
	R_bp -> SetParameter("17_countint", N_int_bp);
	R_tm -> SetParameter("17_countint", N_int_tm);
	R_bm -> SetParameter("17_countint", N_int_bm);
	FixParam_countint(*R_tp, *R_tm, *R_bp, *R_bm);

	FixAllParams(*R_tp, *R_tm, *R_bp, *R_bm);  
	cout << "Final Fit -- So The Integral Gets Saved:  " << endl;
	fitset = makefitset(htp, htm, hbp, hbm, monoenergy_int, *R_tp, *R_tm, *R_bp, *R_bm, fitmax_algorithm, false);  

	// -- - -- // -- - -- // -- - -- //
	cout << "Convergence Summary @ E = " << monoenergy_int << " keV:  " << endl;
	fitset.print_fitconvergence();
	
	cout << "Integrated Counts:  " << endl;
	cout << "N_int_tp = " << R_tp -> GetParameter("17_countint") << endl;
	cout << "N_int_bp = " << R_bp -> GetParameter("17_countint") << endl;
	cout << "N_int_tm = " << R_tm -> GetParameter("17_countint") << endl;
	cout << "N_int_bm = " << R_bm -> GetParameter("17_countint") << endl;
	
	return fitset;
}

void fit_em_all(int fitmax_algorithm=0, bool rcut=false, bool skip_theonesthatworked=false, int N_rebin=1)  // overwrites the relevant histogram (many files), with the fit included now too.
{
	double energy;
	MapSetup * my_new_map;
	set_of_fitresult_pointers theset;
	
	for(int i=the_energyset.size()-1; i>=1; i--)  // count down instead.  but skip full spectrum.
	{
		energy = the_energyset.at(i).the_int;
		cout << "i= " << i << ";\tenergy = " << the_energyset.at(i).the_int << endl;
		
		if(skip_theonesthatworked)  // these are the ones we'll skip:
		{
			if( energy==5000 ) { continue; }  // skip this energy
			if( energy==4500 ) { continue; }  // skip this energy
			if( energy==4000 ) { continue; }  // skip this energy
			if( energy==3500 ) { continue; }  // skip this energy  
			if( energy==3000 ) { continue; }  // skip this energy  // 
			if( energy==2500 ) { continue; }  // skip this energy  // 
			if( energy==2000 ) { continue; }  // skip this energy  // 
			if( energy==1750 ) { continue; }  //
			if( energy==1500 ) { continue; }  // skip this energy  // 
			if( energy==1375 ) { continue; }  // skip this energy  // 
			if( energy==1250 ) { continue; }  // skip this energy  // 
			if( energy==1125 ) { continue; }  // skip this energy  
			if( energy==1000 ) { continue; }  // skip this energy  // 
			if( energy== 875 ) { continue; }  // skip this energy  
			if( energy== 750 ) { continue; }  // skip this energy  // whatever, I'm not even looking through this now.
			if( energy== 625 ) { continue; }
		//	if( energy== 500 ) { continue; }  // skip this energy  // doesn't even converge
		//	if( energy== 250 ) { continue; } 
			if( energy== -10 ) { continue; } // this should never be called up anyway.
		}
		
		//
		my_new_map = new MapSetup();
		my_new_map -> LoadFromFile( make_mapname_from_monoenergy(the_energyset.at(i).the_double) );
		//
		theset = fit_fitset(my_new_map, the_energyset.at(i).the_int, rcut, fitmax_algorithm, N_rebin);
		TFile * f = my_new_map->RecreateAndLeaveOpen();  // why are we doing this here?  it's so the histograms will now be saved with their fit functions, that we've *just* created..
		
		f->cd();
		theset.pointer_tp -> Write("",TObject::kOverwrite);
		theset.pointer_tm -> Write("",TObject::kOverwrite);
		theset.pointer_bp -> Write("",TObject::kOverwrite);
		theset.pointer_bm -> Write("",TObject::kOverwrite);
		f->Close();  // close the multi-files when you're done with them.  
	}
}

void makesave_alltheplotcans(TFile * f, int n_rebin=1, bool plot_ff=false)  // function should only be called when we add more data.  call from main().
{
	string extraname;
	for(int i=0; i<the_energyset.size(); i++)
	{
		MapSetup* my_new_map = new MapSetup();
		my_new_map -> LoadFromFile( make_mapname_from_monoenergy(the_energyset.at(i).the_double) );
		
		extraname = int_to_string( the_energyset.at(i).the_int )+"_rebin"+int_to_string(n_rebin);
		
		TCanvas * cp = make_plotcanvas_p(my_new_map, n_rebin, plot_ff, "cp_"+extraname, the_energyset.at(i).the_int ); 
		TCanvas * cm = make_plotcanvas_m(my_new_map, n_rebin, plot_ff, "cm_"+extraname, the_energyset.at(i).the_int );
		
		f -> cd();
		
		cp -> Write("",TObject::kOverwrite);
		cm -> Write("",TObject::kOverwrite);
	}
	cout << "All the plot canvases have been saved to " << f->GetName() << endl;
	return;
}

set_of_fitresult_pointers get_fitresults_fromfile(TFile * f, bool rcut)
{
	set_of_fitresult_pointers my_fits;
	
	if(rcut)
	{
		my_fits.pointer_tp = (TFitResult*)f->Get("TFitResult-Measured ScintT Energy(+) - rBB1<=15.5 (same det, 1hit)-R");
		my_fits.pointer_tm = (TFitResult*)f->Get("TFitResult-Measured ScintT Energy(-) - rBB1<=15.5 (same det, 1hit)-R");
		my_fits.pointer_bp = (TFitResult*)f->Get("TFitResult-Measured ScintB Energy(+) - rBB1<=15.5 (same det, 1hit)-R");
		my_fits.pointer_bm = (TFitResult*)f->Get("TFitResult-Measured ScintB Energy(-) - rBB1<=15.5 (same det, 1hit)-R");
	}
	else
	{
		my_fits.pointer_tp = (TFitResult*)f->Get("TFitResult-Measured ScintT Energy(+) - BB1 Agreement-R");
		my_fits.pointer_tm = (TFitResult*)f->Get("TFitResult-Measured ScintT Energy(-) - BB1 Agreement-R");
		my_fits.pointer_bp = (TFitResult*)f->Get("TFitResult-Measured ScintB Energy(+) - BB1 Agreement-R");
		my_fits.pointer_bm = (TFitResult*)f->Get("TFitResult-Measured ScintB Energy(-) - BB1 Agreement-R");
	}

	return my_fits;
}

set_of_fitresult_pointers get_fitresults(double the_energy_double, bool rcut) // looks in make_mapname_from_monoenergy(the_energy_double).
{
	TFile * f = new TFile( make_mapname_from_monoenergy( the_energy_double ).c_str() );
	set_of_fitresult_pointers my_fits;
	my_fits = get_fitresults_fromfile(f, rcut);
	f->Close();
	return my_fits;
}

void makesave_allthe_residucans(TFile * f, bool rcut=false, int N_rebin=1)
{
	cout << "Called makesave_allthe_residucans(...)" << endl;
	TCanvas * c;
	TH1D* h;
	TH1D* h_mono;
	MapSetup* my_new_map = new MapSetup();

//	cout << "File open?:  " << f->IsOpen() << endl;
//	for(int i=the_energyset.size()-1; i>=0; i--) // count down instead.
	for(int i=the_energyset.size()-1; i>=1; i--)  // count down instead.  but skip full spectrum.
	{
		my_new_map -> LoadFromFile( make_mapname_from_monoenergy(the_energyset.at(i).the_double) );
		
		// check if f is still a good file?? 
	//	cout << "wtf?" << endl;
	//	cout << "i = " << i << ";\tFile still open?:  " << f->IsOpen() << endl;
		
		// Top +
	//	cout << "t+" << endl;
		if(rcut) { h = my_new_map->measured_EnergyT_p_bb1_r155_samedet_one; }
		else     { h = my_new_map->measured_EnergyT_p_bb1agree; }
		h_mono = my_new_map->naive_EnergyT_p_hist;
		c = make_better_residucan(h, h_mono, "Top (+)", the_energyset.at(i), N_rebin);
		
		f->cd();
		c->Write("",TObject::kOverwrite);
		
		// Top -
	//	cout << "t-" << endl;
		if(rcut) { h = my_new_map->measured_EnergyT_m_bb1_r155_samedet_one; }
		else     { h = my_new_map->measured_EnergyT_m_bb1agree; }
		h_mono = my_new_map->naive_EnergyT_m_hist;
		c = make_better_residucan(h, h_mono, "Top (-)", the_energyset.at(i), N_rebin);
		
		f->cd();
		c->Write("",TObject::kOverwrite);
		
		// Bottom +
	//	cout << "b+" << endl;
		if(rcut) { h = my_new_map->measured_EnergyB_p_bb1_r155_samedet_one; }
		else     { h = my_new_map->measured_EnergyB_p_bb1agree; }
		h_mono = my_new_map->naive_EnergyB_p_hist;
		c = make_better_residucan(h, h_mono, "Bottom (+)", the_energyset.at(i), N_rebin);
		
		f->cd();
		c->Write("",TObject::kOverwrite);
		
		// Bottom -
	//	cout << "b-" << endl;
		if(rcut) { h = my_new_map->measured_EnergyB_m_bb1_r155_samedet_one; }
		else     { h = my_new_map->measured_EnergyB_m_bb1agree; }
		h_mono = my_new_map->naive_EnergyB_m_hist;
		c = make_better_residucan(h, h_mono, "Bottom (-)", the_energyset.at(i), N_rebin);
		
		f->cd();
		c->Write("",TObject::kOverwrite);
	}
	cout << "The residucans have been saved to (root) file." << endl;
}

void makesave_normfuncs(TFile * f)  
{
	
	TF1* f_norm_tp = make_relnorm_tp();
	TF1* f_norm_tm = make_relnorm_tm();
	TF1* f_norm_bp = make_relnorm_bp();
	TF1* f_norm_bm = make_relnorm_bm();
	
	f->cd();
	
	f_norm_tp->Write("",TObject::kOverwrite);
	f_norm_tm->Write("",TObject::kOverwrite);
	f_norm_bp->Write("",TObject::kOverwrite);
	f_norm_bm->Write("",TObject::kOverwrite);
	
//	cout << "Finished makesave_normfuncs(...)" << endl;
	return;
}
/*
void makesave_normfuncs_to_mapout_files()
{
	TFile * f;
	for(int i=the_energyset.size()-1; i>=1; i--)  // count down instead.  but skip full spectrum.
	{
		f = new TFile( make_mapname_from_monoenergy(the_energyset.at(i).the_double).c_str() ); 
		makesave_normfuncs(f);
	}
	cout << "makesave_normfuncs_to_mapout_files()" << endl;
}
*/

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// Look through a file and save all canvases into a PDF format.
void saveall_as_pdf(TFile * f, string pdf_outname)
{
	TCanvas * c = new TCanvas();
	bool is_open = false;
	for(auto k : *f->GetListOfKeys()) 
	{
		TKey *key = static_cast<TKey*>(k);
		TClass *cl = gROOT->GetClass(key->GetClassName());
		if (!cl->InheritsFrom("TCanvas")) continue;
		c = key->ReadObject<TCanvas>();
		if(!is_open)
		{
			c -> Print( (pdf_outname+"[").c_str() );
			is_open = true;
		}
		c -> Print( pdf_outname.c_str() );
	}
	c -> Print( (pdf_outname+"]").c_str() );
	
	return;
}
// --- // --- //
// Print all the fit convergence info:
void printall_fitconvergence(bool rcut=false)
{
	set_of_fitresult_pointers the_fitresults;
	for(int i=the_energyset.size()-1; i>=1; i--)  // count down instead.  but skip full spectrum.
	{
		the_fitresults = get_fitresults(the_energyset.at(i).the_double, rcut);
		
		cout << "** ** ** " << endl;
		cout << "Convergence Summary @ E = " << the_energyset.at(i).the_int << " keV:  " << endl;
		the_fitresults.print_fitconvergence();
	}
	
	return;
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
//int main(int argc, char *argv[]) 
//{
	/*
	setup_location();
	bool rcut = true;
	gStyle->SetErrorX(0);

	TApplication* rootapp = 0;
	char ** mychar = NULL;
	TFile * f;
	rootapp = new TApplication("blarg",0, mychar);
	*/
	
	/*
	bool skipsome = false;
	makesave_allthemaphists("map_out", skipsome);  // do this to generate the maps from the ttrees.  saves to many individual mono-files.  Do this when you add new data.
	// Make a set of plot canvases:
	f = new TFile("just_the_hists.root", "RECREATE");
	makesave_alltheplotcans(f, 1, false);  // overwrites individual many-colored plot canvases to canvases.root.  Only need to do this when we add more data.
	f->Close();
	*/
	
	
//	int N_rebin = 60;
	/*
	f = new TFile("canvases.root", "RECREATE");
	// 0 = fit the whole damn thing
	// 1 = monoenergy+1000
	// 2 = monoenergy+500
	// 3 = to last nonzero bin in the four-set of hists.
	int fitmax_algorithm=3;  // 3 will be the new default.
	bool skip_theonesthatworked=false;
	fit_em_all(fitmax_algorithm, rcut, skip_theonesthatworked, N_rebin);
	
	makesave_fitresultgraphs_and_cans(f, rcut);
	
	// ok, the next thing doesn't seem to makesave them quite right..
	makesave_allthe_residucans(f, rcut, 10);  // file, rcut, N_rebin.  ... what if I don't rebin?  Or rebin harder??
	
//	cout << "we're about to call makesave_normfuncs(f)" << endl;
	makesave_normfuncs(f);  // saves to canvases.root
//	makesave_normfuncs_to_mapout_files();
//	makesave_counting_cans(f, rcut);  // I don't think N_rebin can even do anything here...
	f->Close();
	
	cout << "now, we save the plots to the pdf file..." << endl;
	f = new TFile("canvases.root");  // only open for reading now.
	saveall_as_pdf(f, "output_canvases.pdf");
	f->Close();
	printall_fitconvergence(rcut);   // param is rcut.
	*/
	
	/*
	vector<double> mean_abs_tp;
	vector<double> mean_abs_bp;
	vector<double> mean_abs_tm;
	vector<double> mean_abs_bm;
	
	vector<double> E_in;
	
	for(int i=0; i<the_energyset.size(); i++)
	{
		MapSetup * my_new_map = new MapSetup();
		my_new_map -> LoadFromFile( make_mapname_from_monoenergy(the_energyset.at(i).the_double) );
		
		TH1D * h_out_tp = (TH1D*)my_new_map->measured_EnergyT_p_bb1_r155_samedet_one;
		TH1D * h_out_bp = (TH1D*)my_new_map->measured_EnergyB_p_bb1_r155_samedet_one;
		TH1D * h_out_tm = (TH1D*)my_new_map->measured_EnergyT_m_bb1_r155_samedet_one;
		TH1D * h_out_bm = (TH1D*)my_new_map->measured_EnergyB_m_bb1_r155_samedet_one;
		
		double currentmean_tp = h_out_tp -> GetMean(1);
		double currentmean_bp = h_out_bp -> GetMean(1);
		double currentmean_tm = h_out_tm -> GetMean(1);
		double currentmean_bm = h_out_bm -> GetMean(1);
		
		mean_abs_tp.push_back(the_energyset.at(i).the_int - currentmean_tp);
		mean_abs_bp.push_back(the_energyset.at(i).the_int - currentmean_bp);
		mean_abs_tm.push_back(the_energyset.at(i).the_int - currentmean_tm);
		mean_abs_bm.push_back(the_energyset.at(i).the_int - currentmean_bm);
		
		E_in.push_back( (double)the_energyset.at(i).the_int );
	}
	TGraph * g_tp = make_TGraph(E_in, mean_abs_tp, kRed);
	TGraph * g_bp = make_TGraph(E_in, mean_abs_bp, kGreen);
	TGraph * g_tm = make_TGraph(E_in, mean_abs_tm, kMagenta);
	TGraph * g_bm = make_TGraph(E_in, mean_abs_bm, kBlue);
	TCanvas * c_asdf = new TCanvas("absorption canvas", "absorption canvas", 100, 0, 900, 700);
	c_asdf->cd();
	g_tp -> Draw();
	g_bp -> Draw();
	g_tm -> Draw();
	g_bm -> Draw();
	gPad -> Update();
	
	
	cout << "Running the rootapp." << endl;
	rootapp->Run();
	return 0;
	// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	
	
	string filename = "distributions_rebin"+int_to_string(N_rebin)+".root";
	
	set_of_distributions * my_dists = new set_of_distributions(N_rebin);
	my_dists->create();
	my_dists->initialize_distributions();
	my_dists->save_to_file(filename.c_str());
	
	//
//	set_of_distributions * my_dists = new set_of_distributions(N_rebin);
//	my_dists->load_from_file(filename.c_str());
	//
	*/
	
	
	// --- // * // --- // 
	// --- // * // --- // 
		
//	return 0;
//}




























