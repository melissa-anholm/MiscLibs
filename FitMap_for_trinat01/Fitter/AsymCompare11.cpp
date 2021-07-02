//#include <stdio.h>
//#include <stdlib.h>
//#include <cstdlib>

#include <sstream>
//#include "MakeMap.h"
#include "location.cpp"
#include "GraphExtras.h"

#include "TLatex.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStyle.h"

//#include "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/pizza.h"
#include "pizza.h"
#include "FitResultsStructure.h"
//string output_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";

//using std::itoa;
using std::string;

//string the_datafilepath = "~/Desktop/Anal-Ysis/Abeta2/Get_Abeta2/Files/";
extern string JustPlotA_path;
extern string fitresults_path;

#define __SHORT_FORM_OF_FILE__ \
(strrchr(__FILE__,'/') ? strrchr(__FILE__,'/')+1 : __FILE__ )

string fake_itoa(int the_int)
{
  std::ostringstream o;
  if (!(o << the_int))
    return "";
  return o.str();
}

TH1D* get_simple_residuals(TH1D* h1, TH1D* h2, double ymin=0, double ymax=0)
{
	int verbose = 0;
	if( !( h1 && h2) )
	{
		cout << "h1=" << h1 << ",\th2=" << h2 << endl;
		cout << "So, y'know, that's bad." << endl;
		return h1;
	}
	
	// SetBinError(...) sets the sumw2 to be err^2.

	TH1D * new_hist = (TH1D*)h1 -> Clone( "Residuals" );
//	new_hist -> SetTitle("Residuals");
//	new_hist -> SetTitle("(h1-h2) / sqrt(dh1^2+dh2^2) ");
	new_hist -> SetTitle("h1-h2");
	new_hist -> Sumw2(kFALSE);
	new_hist -> Sumw2();

	int n_bins  = h1->GetNbinsX();
	int n_bins2 = h2->GetNbinsX();

	if(verbose>0)
	{
		cout << "h1 has " << n_bins << " bins;  h2 has " << n_bins2 << " bins." << endl;
	}
	
//	if(n_bins != n_bins2)
//	{
//		cout << "******************************************************" << endl;
//		cout << "ERROR!  h1 and h2 do not have the same number of bins!" << endl;
//		cout << "Returning nonsense." << endl;
//		cout << "******************************************************" << endl;
//		return new_hist;
//	}
	
	double dif;
//	double sig;
//	double res;
//	double e1, e2;
	for (int i =1; i<=n_bins && i<=n_bins2; i++) 
	{
		dif = h1->GetBinContent(i) - h2->GetBinContent(i);
		new_hist -> SetBinContent(i, dif);
		new_hist -> SetBinError(i, 0.000001);
		
	//	cout << "bin i=" << i << ";\txcenter1=" << h1->GetBinCenter(i) << ";\txcenter2=" << h2->GetBinCenter(i);// << endl;
	//	cout << ";\th1=" << h1->GetBinContent(i) << ";\th2=" << h2->GetBinContent(i) << endl;
	//	cout << "\tdif=" << dif << endl;
	}
	
	new_hist -> SetMarkerColor(kBlack);
	new_hist -> SetLineColor(kBlack);
	new_hist -> SetMarkerStyle(20);
//	new_hist -> SetMarkerSize(1.0);
	new_hist -> SetMarkerSize(0.8);
	if(ymin!=ymax)
	{
		new_hist -> GetYaxis() -> SetRangeUser(ymin, ymax);
	}
	return new_hist;  // ...
}

double justgetthedamnchisquared(TH1D * h1, TH1D * h2, /* bool already_weighted=true,*/ double xmin=0, double xmax=0)
{
	bool already_weighted=true;

	int verbose = 0;  // 0,1,2.
	double chi2 = 0.0;
	int nbins = 0;
	
	double h1_bcont = 0.0;
	double h1_berr = 0.0;
	double h2_bcont = 0.0;
	double h2_berr = 0.0;
	double combined_berr = 1.0;
	double chi_contribution = 0.0;
	int bmin, bmax;
	if(xmin==0 && xmax==0)
	{
		bmin = 0;
		bmax = 0;
	}
	else
	{
		bmin = h1->FindBin(xmin);
		bmax = h1->FindBin(xmax);
		
	//	int bmin2 = h2->FindBin(xmin);
	//	int bmax2 = h2->FindBin(xmax);
	//	cout << "Before I get the chi2:  bmin1 = " << bmin << ", bmin2= " << bmin2 << endl;
	//	cout << "Before I get the chi2:  bmax1 = " << bmax << ", bmax2= " << bmax2 << endl;
	}
	if(verbose>0)
	{
		cout << "using bmin=" << bmin << " (eff. xmin=" << h1->GetBinCenter(bmin) - 0.5*h1->GetBinWidth(bmin) << ");\tbmax=" << bmax << " (eff. xmax=" << h1->GetBinCenter(bmax) + 0.5*h1->GetBinWidth(bmax) << ");\tN_binstocount=" << bmax-bmin+1 << endl;
	}
	if(!already_weighted)
	{
		h1 -> Sumw2();
		h2 -> Sumw2();
	}
//	if(HistsHaveSameBinning(h1, h2))
//	{
		nbins = h1 -> GetNbinsX();
		
		if(bmax == 0 || bmax > nbins) { bmax = nbins; }
		if(bmin < 0 || bmin > bmax ) { bmin=0; }
		if(verbose>1)
		{
			cout << "nbins = " << nbins;// << endl;
			cout << "\tbins_in_use = " << bmax - bmin << endl;
			cout << "E_min = " << h1->GetBinCenter(bmin) - 0.5*h1->GetBinWidth(bmin);
			cout << "\tE_max = " << h1->GetBinCenter(bmax) +  0.5*h1->GetBinWidth(bmax) << endl;
			cout << endl;
		}
		
		for (int i = bmin; i <= bmax; i++) 
		{
			if (i < bmin) 
			{
				cout << "i<bmin.  why did this happen?" << endl;
				continue;
			}
			if (i > bmax) 
			{
				continue;
				cout << "i>bmax.  why did this happen?" << endl;
			}
			
			h1_bcont = h1 -> GetBinContent(i);
			h1_berr  = h1 -> GetBinError(i);   // this is the literal size of the error bars.
			h2_bcont = h2 -> GetBinContent(i);
			h2_berr  = h2 -> GetBinError(i);
			
			combined_berr = sqrt(h1_berr*h1_berr + h2_berr*h2_berr);
			if (combined_berr <= 0) 
			{ 
				combined_berr = 1.0; 
				if(verbose>1)
				{
					cout << "* bin " << i << ":  setting combined bin err to 1." << endl;
				}
			}
			
			chi_contribution = (h1_bcont - h2_bcont)/combined_berr;
			chi2 = chi2 + chi_contribution*chi_contribution;
			if(verbose>1)
			{
				cout << "bin " << i;
				cout << " (" << h1->GetBinCenter(i) - 0.5*h1->GetBinWidth(i);
				cout << " - " <<  h1->GetBinCenter(i) +  0.5*h1->GetBinWidth(i);
				cout << ") \t" << "h1_bcont = " << h1_bcont << ", h2_bcont = " << h2_bcont;
				cout << ";\th1_berr = " << h1_berr << ", h2_berr = " << h2_berr;
				cout << ";\tchi2 = " << chi_contribution*chi_contribution << endl;
			}
		}
//	}
//	else
//	{
//		cout << "That's bad.  Hists should really have the same binning..." << endl;
//	}
	if(verbose>0)
	{
		cout << endl;
	}
	if(!already_weighted)
	{
		h1 -> Sumw2(kFALSE);
		h2 -> Sumw2(kFALSE);
	}
	return chi2;  // returns 0 if hists don't have the same binning.
}

double get_CS_from_gS(double g_S)
{
	double C_S = g_S/sqrt(2.0);
	return C_S;
}
double get_xi_from_gA_gS(double g_A, double g_S)
{
	double g_V = 1.0;
	double M_GT = -0.62376;
	
	double xi = (g_V*g_V + g_S*g_S) + M_GT*M_GT*(g_A*g_A);
	
	return xi;
}
double get_Axi_from_gA(double g_A)  // g_S isn't relevant for Axi, as long as g_T=0 and we require the couplings to be real.
{
	double g_V = 1.0;
	double M_GT = -0.62376;
	
	double Axi = (2.0/5.0)*M_GT*M_GT*(g_A*g_A) + 2.0*sqrt(3.0/5.0)*1.0*M_GT*(g_V*g_A);
	
	return Axi;
}
double get_bxi_from_gS(double g_S)  // g_A is irrelevant.
{
	double alpha = 7.2973525693e-3;
	double Z_daughter = 18.0;
	double jtw_gamma = sqrt(1.0-alpha*alpha* Z_daughter*Z_daughter);
	
	double g_V = 1.0;

	double bxi = -2.0*jtw_gamma*(1.0*g_V*g_S);
	
	return bxi;
}
double get_bFierz_from_gA_gS(double g_A, double g_S)
{
	double bxi = get_bxi_from_gS(g_S);
	double xi  = get_xi_from_gA_gS(g_A, g_S);
	
	double bFierz = bxi / xi;
	return bFierz;
}
double get_Abeta_from_gA_gS(double g_A, double g_S)
{
	double Axi = get_Axi_from_gA(g_A);
	double xi  = get_xi_from_gA_gS(g_A, g_S);
	
	double Abeta = Axi / xi;
	return Abeta;
}

double get_Abest(TFitResultPtr r)
{
	string A_parname  = "Abeta";
	int parnumber_A  = r->Index( A_parname);
	if (parnumber_A==-1) { return 0; }
	double A_best = r->Parameter(parnumber_A);
	return A_best;
}
double get_sigmaA(TFitResultPtr r)
{
	string A_parname      = "Abeta";
	string sigmaA_parname = "sigma_A";
	int parnumber_A  = r->Index( A_parname);
	int parnumber_sigmaA = r->Index(sigmaA_parname);
	if (parnumber_A==-1 || parnumber_sigmaA == -1) { return 0; }
	
	double dA_fitminerr = r->ParError(parnumber_A);
	double dA_uponechi2 = r->Parameter(parnumber_sigmaA);
	double A_best = r->Parameter(parnumber_A);
	
	double sigmaA = sqrt(dA_fitminerr*dA_fitminerr + dA_uponechi2*dA_uponechi2);
	return sigmaA;
}
double get_Amin(TFitResultPtr r)
{
	double A_best = get_Abest(r);
	double sigmaA = get_sigmaA(r);
	double Amin   = A_best - sigmaA;
	
	return Amin;
}
double get_Amax(TFitResultPtr r)
{
	double A_best = get_Abest(r);
	double sigmaA = get_sigmaA(r);
	double Amax   = A_best + sigmaA;
	
	return Amax;
}
//
double get_bbest(TFitResultPtr r)
{
	string b_parname  = "bFierz";
	int parnumber_b  = r->Index( b_parname);
	if (parnumber_b==-1) { return 0; }
	double b_best = r->Parameter(parnumber_b);
	return b_best;
}
double get_sigmab(TFitResultPtr r)
{
	string b_parname      = "bFierz";
	string sigmab_parname = "sigma_b";
	int parnumber_b  = r->Index( b_parname);
	int parnumber_sigmab = r->Index(sigmab_parname);
	if (parnumber_b==-1 || parnumber_sigmab == -1) { return 0; }
	
	double db_fitminerr = r->ParError(parnumber_b);
	double db_uponechi2 = r->Parameter(parnumber_sigmab);
	double b_best = r->Parameter(parnumber_b);
	
	double sigmab = sqrt(db_fitminerr*db_fitminerr + db_uponechi2*db_uponechi2);
	return sigmab;
}
//
double get_bmin(TFitResultPtr r)
{
	double b_best = get_bbest(r);
	double sigmab = get_sigmab(r);
	double bmin   = b_best - sigmab;
	
	return bmin;
}
double get_bmax(TFitResultPtr r)
{
	double b_best = get_bbest(r);
	double sigmab = get_sigmab(r);
	double bmax   = b_best + sigmab;
	
	return bmax;
}


// double g_S_G4 =  0.1;
// double g_A_G4 =  0.91210;
// 
// double A_plus  = get_Abeta_from_gA_gS(g_A_G4, g_S_G4);
// double A_0     = get_Abeta_from_gA_gS(g_A_G4, 0.0);
// double A_minus = get_Abeta_from_gA_gS(g_A_G4, -1.0*g_S_G4);
// 
// double b_plus  = get_bFierz_from_gA_gS(g_A_G4, g_S_G4);
// double b_0     = 0.0;
// double b_minus = get_bFierz_from_gA_gS(g_A_G4, -1.0*g_S_G4);
// 
// double A_plus  = -0.563786;
// double A_0     = -0.568045;
// double A_minus = -0.563786;
// double b_plus  = -0.148661;
// double b_0     =  0.0;
// double b_minus =  0.148661;


double get_alpha_from_Abeta_and_bFierz(double Abeta, double bFierz)
{
	double g_S_G4 =  0.1;
	double g_A_G4 =  0.91210;

	double A_plus  = get_Abeta_from_gA_gS(g_A_G4, g_S_G4);
	double A_0     = get_Abeta_from_gA_gS(g_A_G4, 0.0);
//	double A_minus = get_Abeta_from_gA_gS(g_A_G4, -1.0*g_S_G4);
	
	double b_plus  = get_bFierz_from_gA_gS(g_A_G4, g_S_G4);
	double b_0     = 0.0;
//	double b_minus = get_bFierz_from_gA_gS(g_A_G4, -1.0*g_S_G4);
	
	double alpha = (Abeta*b_plus - A_plus*bFierz) / (A_0*b_plus - A_plus*b_0);
	return alpha;
}
double get_beta_from_Abeta_and_bFierz(double Abeta, double bFierz)
{
	double g_S_G4 =  0.1;
	double g_A_G4 =  0.91210;

	double A_plus  = get_Abeta_from_gA_gS(g_A_G4, g_S_G4);
	double A_0     = get_Abeta_from_gA_gS(g_A_G4, 0.0);
//	double A_minus = get_Abeta_from_gA_gS(g_A_G4, -1.0*g_S_G4);
	
	double b_plus  = get_bFierz_from_gA_gS(g_A_G4, g_S_G4);
	double b_0     = 0.0;
//	double b_minus = get_bFierz_from_gA_gS(g_A_G4, -1.0*g_S_G4);
	
	double beta = (bFierz*A_0 - Abeta*b_0) / (A_0*b_plus - A_plus*b_0);
	return beta;
}
double get_Abeta_from_alpha_and_beta(double alpha, double beta)
{
	double g_S_G4 =  0.1;
	double g_A_G4 =  0.91210;

	double A_plus  = get_Abeta_from_gA_gS(g_A_G4, g_S_G4);
	double A_0     = get_Abeta_from_gA_gS(g_A_G4, 0.0);
//	double A_minus = get_Abeta_from_gA_gS(g_A_G4, -1.0*g_S_G4);
	
	double b_plus  = get_bFierz_from_gA_gS(g_A_G4, g_S_G4);
	double b_0     = 0.0;
//	double b_minus = get_bFierz_from_gA_gS(g_A_G4, -1.0*g_S_G4);
	
	double Abeta = alpha*A_0 + beta*A_plus;
	return Abeta;
}
double get_bFierz_from_alpha_and_beta(double alpha, double beta)
{
	double g_S_G4 =  0.1;
	double g_A_G4 =  0.91210;

	double A_plus  = get_Abeta_from_gA_gS(g_A_G4, g_S_G4);
	double A_0     = get_Abeta_from_gA_gS(g_A_G4, 0.0);
//	double A_minus = get_Abeta_from_gA_gS(g_A_G4, -1.0*g_S_G4);
	
	double b_plus  = get_bFierz_from_gA_gS(g_A_G4, g_S_G4);
	double b_0     = 0.0;
//	double b_minus = get_bFierz_from_gA_gS(g_A_G4, -1.0*g_S_G4);

	double bFierz = alpha*b_0 + beta*b_plus;
	return bFierz;
}
double getfast_Abeta_from_alpha_and_beta(double alpha, double beta)
{
	double A_plus  = -0.563786;
	double A_0     = -0.568045;
//	double A_minus = -0.563786;
	double b_plus  = -0.148661;
	double b_0     =  0.0;
//	double b_minus =  0.148661;
	
	double Abeta = alpha*A_0 + beta*A_plus;
	return Abeta;
}
double getfast_bFierz_from_alpha_and_beta(double alpha, double beta)
{
	double A_plus  = -0.563786;
	double A_0     = -0.568045;
//	double A_minus = -0.563786;
	double b_plus  = -0.148661;
	double b_0     =  0.0;
//	double b_minus =  0.148661;

	double bFierz = alpha*b_0 + beta*b_plus;
	return bFierz;
}

string make_asymplot_datafilename(string namestub, string runset, int threshold_index, double sigma_cut, double bb1_maxr)
{
	// JustPlotA_D_ind0_sig3p0_bbr15p5.root
	string thr_string = int_to_string(threshold_index);
//	string the_name = namestub +"_"+runset+"_ind"+thr_string;
	
	string sig_str = convertDouble(sigma_cut);
	if( (sigma_cut-double(int(sigma_cut)))==0.0 ) { sig_str.replace(1, 1, "p0"); }
	else                                          { sig_str.replace(1, 1, "p" ); }
	string bb_str = convertDouble(bb1_maxr);
	if( (bb1_maxr-double(int(bb1_maxr)))==0.0 )   { bb_str.replace(2, 1, "p0"); }
	else                                          { bb_str.replace(2, 1, "p" ); }
	string additional_filename_info = "_ind"+thr_string+"_sig"+sig_str+"_bbr"+bb_str;
	//
	string outfname = string(JustPlotA_path+namestub +"_"+runset+additional_filename_info+".root");
	return outfname;
}
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
int main(int argc, char *argv[]) 
{
	setup_location();
	gStyle->SetErrorX(0);
	gStyle->SetOptStat(0);
//	gStyle->SetPalette(kIsland);
//	gStyle->SetPalette(kDeepSea);
//	gStyle->SetPalette(kOcean);
//	gStyle->SetPalette(kSunset);
//	gStyle->SetPalette(kViridis);
//	gStyle->SetPalette(kSolar);
//	gStyle->SetPalette(kRainBow);
	TColor::InvertPalette();
	
	gStyle->SetNumberContours(255);
	
//	TApplication* rootapp = 0;
//	char ** mychar = NULL;
//	rootapp = new TApplication("blarg",0, mychar);
	
	bool cook_fast = true;
	// Add the ability to call with a set of arguments..
	double topping_bs_scale = 1.0;
	double topping_ss_scale = 1.0;
	double topping_bg_scale = 1.0;
	//
	int threshold_index = 0;
	double sigma_cut    = 3.0;
	double bb1_maxr     = 15.5;
	int bb1_threshold   = 50;
	
//	double A_res = 0.0009375;
//	double b_res = 0.009375;
	double dA = 0.001; // zoomout 97
	double db = 0.01;  // zoomout 97
	int zoomlevel=1;
	
	double the_Amin, the_Amax;
	double the_bmin, the_bmax;

	if(argc>=2)
	{
		zoomlevel = atoi(argv[1]);
		if(zoomlevel==0)  // only for the simplest of code tests.
		{
			the_Amin = -0.62;
			the_Amax = -0.53;
			the_bmin = -0.50;
			the_bmax =  0.40;
			dA = 0.005;
			db = 0.05;
			// N_A = 18
			// N_b = 19
		}
		else if(zoomlevel==1)  // actually zoomed out.
		{
			the_Amin = -0.62;
			the_Amax = -0.53;
			the_bmin = -0.50;
			the_bmax =  0.40;
			dA = 0.001;
			db = 0.01;
			// N_A = 90 ?
			// N_b = 91 ?
		}
		else if(zoomlevel==2)  // actually zoomed out.  probably slow to run.
		{
			the_Amin = -0.62;
			the_Amax = -0.53;
			the_bmin = -0.50;
			the_bmax =  0.40;
			dA = 0.0005;
			db = 0.005;
			// N_A = 180 ?
			// N_b = 182 ?
		}
		else if(zoomlevel==3)  //  <5min
		{
			the_Amin = -0.590;
			the_Amax = -0.555;
			the_bmin = -0.150;
			the_bmax =  0.150;
			dA = 0.0004;
			db = 0.004;
			// N_A = 88
			// N_b = 76
		}
		else if(zoomlevel==4)  // too long.  but should be shorter now.  still 36 min on trinat01.
		{
			the_Amin = -0.590;
			the_Amax = -0.555;
			the_bmin = -0.150;
			the_bmax =  0.150;
			dA = 0.0002;
			db = 0.002;
			// N_A = 175
			// N_b = 151
		}
		else if(zoomlevel==5)
		{
			the_Amin = -0.590;
			the_Amax = -0.555;
			the_bmin = -0.150;
			the_bmax =  0.150;
			dA = 0.0001;
			db = 0.001;
			// N_A = 
			// N_b = 
		}
		else
		{
			cout << "Oh no!" << endl;
			return 0;
		}
	}
	cout << "Using zoomlevel:  " << zoomlevel << endl;
	if(argc>=5)
	{
		topping_bs_scale = atof(argv[2]);
		topping_ss_scale = atof(argv[3]);
		topping_bg_scale = atof(argv[4]);
	}
	if(argc>=9)
	{
		threshold_index = atoi(argv[5]);
		sigma_cut       = atof(argv[6]);
		bb1_maxr        = atof(argv[7]);
		bb1_threshold   = atoi(argv[8]);
	}
	string results_file_namestub = "results";
	if(argc>=10)
	{
		results_file_namestub = argv[9];  // can I do this?
	}
	
	// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
//	int blue_mid=kAzure;
//	int blue_dark=kAzure-6;
//	int blue_light=kAzure+7;
//	
//	int green_mid=kGreen+1;
//	int green_dark=kGreen+3;
//	int green_light=kGreen-6;
//	
//	int orange_mid = kOrange+7;
//	int orange_dark = kOrange+9;
//	int orange_light = kOrange+1;
	
	
	string matched_set;
	
	cout << "Getting pizzas..." << endl;
//	pizza the_pizza = get_pizza("pizzafile.root");
	matched_set     ="B";
	pizza the_pizza_B = get_pizza("pizzafile", matched_set, threshold_index, sigma_cut, bb1_threshold, bb1_maxr);

	matched_set     ="CD";
	pizza the_pizza_CD = get_pizza("pizzafile", matched_set, threshold_index, sigma_cut, bb1_threshold, bb1_maxr);
	//
	
	double topping_A = -0.5707;
	double topping_b = 0.1;
	toppings the_toppings = pick_toppings(topping_A, topping_b, topping_bs_scale, topping_ss_scale, topping_bg_scale);
	//
	
	// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	// Now the data histograms...
	string datafilename_B = make_asymplot_datafilename("JustPlotA", "B", threshold_index, sigma_cut, bb1_maxr);
	string datafilename_C = make_asymplot_datafilename("JustPlotA", "C", threshold_index, sigma_cut, bb1_maxr);
	string datafilename_D = make_asymplot_datafilename("JustPlotA", "D", threshold_index, sigma_cut, bb1_maxr);
	
	TFile * data_file_B = new TFile( (datafilename_B).c_str() );
	TFile * data_file_C = new TFile( (datafilename_C).c_str() );
	TFile * data_file_D = new TFile( (datafilename_D).c_str() );
	
	cout << "Datafilenames:  " << endl;
	cout << "B:  " << datafilename_B << ", file=" << data_file_B << endl;
	cout << "C:  " << datafilename_C << ", file=" << data_file_C << endl;
	cout << "D:  " << datafilename_D << ", file=" << data_file_D << endl;
	
	string sig_str = convertDouble(sigma_cut);
	string bb_str  = convertDouble(bb1_maxr);
	string snr_str = int_to_string(threshold_index);
	
	int data_color = kPink-2;
	int sim_color  = kAzure-3;
	TH1D * h_Abeta_data_B = (TH1D*)data_file_B->Get( (string("Superratio Asymmetry from Data, with TOF Cut ("+int_to_string(bb1_threshold)+"keV)")).c_str() );  // this has already been rebinned... 
	h_Abeta_data_B -> SetTitle( (string("Superratio Asymmetry from Data, with TOF Cut -- Set B (SNR ind.="+int_to_string(threshold_index)+", thr="+int_to_string(bb1_threshold)+"keV, bb1_maxr="+convertDouble(bb1_maxr)+", sig_agr="+ convertDouble(sigma_cut)+")")).c_str() );
	h_Abeta_data_B -> SetName(h_Abeta_data_B->GetTitle());
	h_Abeta_data_B->SetLineColor(data_color);
	h_Abeta_data_B->SetMarkerColor(h_Abeta_data_B->GetLineColor());
	
	
	TH1D * h_Abeta_data_C = (TH1D*)data_file_C->Get( (string("Superratio Asymmetry from Data, with TOF Cut ("+int_to_string(bb1_threshold)+"keV)")).c_str() );
	h_Abeta_data_C -> SetTitle( (string("Superratio Asymmetry from Data, with TOF Cut -- Set C (SNR ind.="+int_to_string(threshold_index)+", thr="+int_to_string(bb1_threshold)+"keV, bb1_maxr="+convertDouble(bb1_maxr)+", sig_agr="+ convertDouble(sigma_cut)+")")).c_str() );
	h_Abeta_data_C -> SetName(h_Abeta_data_C->GetTitle());
	h_Abeta_data_C->SetLineColor(data_color);
	h_Abeta_data_C->SetMarkerColor(h_Abeta_data_C->GetLineColor());

	TH1D * h_Abeta_data_D = (TH1D*)data_file_D->Get( (string("Superratio Asymmetry from Data, with TOF Cut ("+int_to_string(bb1_threshold)+"keV)")).c_str() );
	h_Abeta_data_D -> SetTitle( (string("Superratio Asymmetry from Data, with TOF Cut -- Set D (SNR ind.="+int_to_string(threshold_index)+", thr="+int_to_string(bb1_threshold)+"keV, bb1_maxr="+convertDouble(bb1_maxr)+", sig_agr="+ convertDouble(sigma_cut)+")")).c_str() );
	h_Abeta_data_D -> SetName(h_Abeta_data_D->GetTitle());
	h_Abeta_data_D->SetLineColor(data_color);
	h_Abeta_data_D->SetMarkerColor(h_Abeta_data_D->GetLineColor());
	
	cout << "asymmetries are gotten." << endl;
	
	TText *datalabel = new TText();
	datalabel -> SetNDC();
	datalabel -> SetTextColor(1);
	datalabel -> SetTextSize(0.018);
	TText *datalabel2 = new TText();
	datalabel2 -> SetNDC();
	datalabel2 -> SetTextColor(1);
	datalabel2 -> SetTextSize(0.018*2);
	TText *datalabelr = new TText();
	datalabelr -> SetNDC();
	datalabelr -> SetTextColor(1);
	datalabelr -> SetTextSize(0.018*1.4);
	TText *datalabel_rhs = new TText();
	datalabel_rhs -> SetNDC();
	datalabel_rhs -> SetTextColor(1);
	datalabel_rhs -> SetTextSize(0.018);
	datalabel_rhs->SetTextAlign(31);
	
	
	// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	double xmin = 400.0;
	double xmax = 4800.0-0.001;
	
	int N_A_vals = int( (the_Amax - the_Amin)/dA ) + 1;
	int N_b_vals = int( (the_bmax - the_bmin)/db ) + 1;
	//
	the_Amin = the_Amin-0.5*dA;  // the_Amin and the_bmin are really only approximate.
	the_bmin = the_bmin-0.5*db;  
	the_Amax = the_Amin + double(N_A_vals)*dA;  // 
	the_bmax = the_bmin + double(N_b_vals)*db;  // the_Amax and the_bmax that are baked in above are really only approximate.
	
	cout << "A=[" << the_Amin << "," << the_Amax << "];  dA="   << dA << endl;
	cout << "b=[" << the_bmin << "," << the_bmax << "];\t  db=" << db << endl;

	TH2D * fitmap_B = new TH2D("Runset B Chi^2 Map",   "Runset B Chi^2 Map",   N_b_vals, the_bmin, the_bmax, N_A_vals, the_Amin, the_Amax);
	fitmap_B -> GetXaxis()-> SetTitle("b_Fierz");
	fitmap_B -> GetYaxis()-> SetTitle("A_beta");
	fitmap_B -> GetXaxis()->SetRangeUser(the_bmin+0.5*db, the_bmax-0.5*db);
	fitmap_B -> GetYaxis()->SetRangeUser(the_Amin+0.5*dA, the_Amax-0.5*dA);

	TH2D * fitmap_C = new TH2D("Runset C Chi^2 Map",   "Runset C Chi^2 Map",   N_b_vals, the_bmin, the_bmax, N_A_vals, the_Amin, the_Amax);
	fitmap_C -> GetXaxis()-> SetTitle("b_Fierz");
	fitmap_C -> GetYaxis()-> SetTitle("A_beta");
	fitmap_C -> GetXaxis()->SetRangeUser(the_bmin+0.5*db, the_bmax-0.5*db);
	fitmap_C -> GetYaxis()->SetRangeUser(the_Amin+0.5*dA, the_Amax-0.5*dA);
	
	TH2D * fitmap_D = new TH2D("Runset D Chi^2 Map",   "Runset D Chi^2 Map",   N_b_vals, the_bmin, the_bmax, N_A_vals, the_Amin, the_Amax);
	fitmap_D -> GetXaxis()-> SetTitle("b_Fierz");
	fitmap_D -> GetYaxis()-> SetTitle("A_beta");
	fitmap_D -> GetXaxis()->SetRangeUser(the_bmin+0.5*db, the_bmax-0.5*db);
	fitmap_D -> GetYaxis()->SetRangeUser(the_Amin+0.5*dA, the_Amax-0.5*dA);
	
	TH2D * fitmap_BCD = new TH2D("Runsets BCD Chi^2 Map",   "Runsets BCD Chi^2 Map",   N_b_vals, the_bmin, the_bmax, N_A_vals, the_Amin, the_Amax);
	fitmap_BCD -> GetXaxis()-> SetTitle("b_Fierz");
	fitmap_BCD -> GetYaxis()-> SetTitle("A_beta");
	fitmap_BCD -> GetXaxis()->SetRangeUser(the_bmin+0.5*db, the_bmax-0.5*db);
	fitmap_BCD -> GetYaxis()->SetRangeUser(the_Amin+0.5*dA, the_Amax-0.5*dA);
	
	
	double this_A, this_b;
	double this_alpha, this_beta;
	//
	double this_chi2_B;
	double this_chi2_C;
	double this_chi2_D;
	
	TH1D * h_asym = new TH1D();
	
	TH1D * h_best_B = new TH1D();
	TH1D * h_asym_bmax_B = new TH1D();
	TH1D * h_asym_bmin_B = new TH1D();

	TH1D * h_best_C = new TH1D();
	TH1D * h_asym_bmax_C = new TH1D();
	TH1D * h_asym_bmin_C = new TH1D();

	TH1D * h_best_D = new TH1D();
	TH1D * h_asym_bmax_D = new TH1D();
	TH1D * h_asym_bmin_D = new TH1D();

	double minchi2_B     = 5000;
	double minchi2_C     = 5000;
	double minchi2_D     = 5000;
	double A_best_B, b_best_B;
	double A_best_C, b_best_C;
	double A_best_D, b_best_D;
	
//	double minchi1_fromlastrun = 35.4461;
	/*
//	prev. best:
//	A_best = -0.5681;
//	b_best = -0.0266;
//	values that break it:
	A_best = -0.6;
	b_best = -0.175;
	
	the_toppings = pick_toppings(A_best, b_best, 1.0, 1.0, 0.0);
	h_asym = cook(the_pizza, the_toppings);
	this_chi2 = justgetthedamnchisquared(h_asym, h_Abeta_data, xmin, xmax);
	h_best = (TH1D*)h_asym->Clone("Best Fit");
	cout << "A=" << A_best << ";\tb=" << b_best << ";\tchi^2=" << this_chi2 << endl;
	*/
	
	// Set B:
	cout << "N_A_vals = " << N_A_vals << endl;
	cout << "N_b_vals = " << N_b_vals << endl;
	for(int j=0; j<N_A_vals; j++)
	{
		this_A = the_Amin+0.5*dA + double(j)*dA;
		if(j%10 == 0)  { cout << "j=" << j << ";\tthis_A=" << this_A << endl; }
		//
		for(int i=0; i<N_b_vals; i++)
		{
			this_b = the_bmin+0.5*db + double(i)*db;
			the_toppings = pick_toppings(this_A, this_b, topping_bs_scale, topping_ss_scale, topping_bg_scale);
			
			if(cook_fast)
			{
				h_asym = simple_cook(the_pizza_B, the_toppings);
			}
			else
			{
				h_asym = cook(the_pizza_B, the_toppings);
			}
			//
			this_chi2_B = justgetthedamnchisquared(h_asym, h_Abeta_data_B, xmin, xmax);
			fitmap_B     -> Fill(this_b, this_A, this_chi2_B);
			//
			if(this_chi2_B < minchi2_B)
			{
				minchi2_B= this_chi2_B;
				A_best_B = this_A;
				b_best_B = this_b;
				h_best_B = (TH1D*)h_asym->Clone("Best Fit B");
				h_best_B -> SetLineColor(kAzure+10);
				h_best_B -> SetMarkerColor(kAzure+10);
			}
		}
	}
	cout << "First loop (B) is done." << endl;
	//
	for(int j=0; j<N_A_vals; j++)
	{
		this_A = the_Amin+0.5*dA + double(j)*dA;
		if(j%10 == 0)  { cout << "j=" << j << ";\tthis_A=" << this_A << endl; }
		//
		for(int i=0; i<N_b_vals; i++)
		{
			this_b = the_bmin+0.5*db + double(i)*db;
			the_toppings = pick_toppings(this_A, this_b, topping_bs_scale, topping_ss_scale, topping_bg_scale);
			
			if(cook_fast)
			{
				h_asym = simple_cook(the_pizza_CD, the_toppings);
			}
			else
			{
				h_asym = cook(the_pizza_CD, the_toppings);
			}
			//
			this_chi2_C = justgetthedamnchisquared(h_asym, h_Abeta_data_C, xmin, xmax);
			fitmap_C     -> Fill(this_b, this_A, this_chi2_C);

			this_chi2_D = justgetthedamnchisquared(h_asym, h_Abeta_data_D, xmin, xmax);
			fitmap_D     -> Fill(this_b, this_A, this_chi2_D);
			//
			if(this_chi2_C < minchi2_C)
			{
				minchi2_C= this_chi2_C;
				A_best_C = this_A;
				b_best_C = this_b;
				h_best_C = (TH1D*)h_asym->Clone("Best Fit C");
				h_best_C -> SetTitle(h_best_C->GetName());
				h_best_C -> SetLineColor(kAzure+10);
				h_best_C -> SetMarkerColor(kAzure+10);
			}
			if(this_chi2_D < minchi2_D)
			{
				minchi2_D= this_chi2_D;
				A_best_D = this_A;
				b_best_D = this_b;
				h_best_D = (TH1D*)h_asym->Clone("Best Fit D");
				h_best_D -> SetTitle(h_best_D->GetName());
				h_best_D -> SetLineColor(kAzure+10);
				h_best_D -> SetMarkerColor(kAzure+10);
			}
		}
	}
	cout << "Second loop (CD) is done." << endl;
	//
	fitmap_BCD->Add(fitmap_B);
	fitmap_BCD->Add(fitmap_C);
	fitmap_BCD->Add(fitmap_D);

	double A_best_BCD, b_best_BCD;
	double this_chi2_BCD;
	double minchi2_BCD = 5000;
	
	//
	for(int j=0; j<N_A_vals; j++)  // skip underflow and overflow bins. ... no don't.
	{
		this_A = the_Amin+0.5*dA + double(j)*dA;
		for(int i=0; i<N_b_vals; i++)
		{
			this_b = the_bmin+0.5*db + double(i)*db;
			this_chi2_BCD= fitmap_BCD->GetBinContent(i+1, j+1);
			//
			if(this_chi2_BCD < minchi2_BCD)
			{
				minchi2_BCD= this_chi2_BCD;
				A_best_BCD = this_A;
				b_best_BCD = this_b;
			}
		}
	}
	cout << "Third loop (BCD Combo) is done." << endl;
	
	// OK, just do the whole damn thing twice to get the sigmas.
	double A_min_1s_B =  0.0;
	double A_max_1s_B = -1.0;
	double b_min_1s_B =  0.1;
	double b_max_1s_B = -0.1;
	double A_for_bmin_B =  0.0;
	double A_for_bmax_B = -1.0;
	double b_for_Amin_B = -0.1;
	double b_for_Amax_B =  0.1;
	//
	double A_min_1s_C =  0.0;
	double A_max_1s_C = -1.0;
	double b_min_1s_C =  0.1;
	double b_max_1s_C = -0.1;
	double A_for_bmin_C =  0.0;
	double A_for_bmax_C = -1.0;
	double b_for_Amin_C = -0.1;
	double b_for_Amax_C =  0.1;
	//
	double A_min_1s_D =  0.0;
	double A_max_1s_D = -1.0;
	double b_min_1s_D =  0.1;
	double b_max_1s_D = -0.1;
	double A_for_bmin_D =  0.0;
	double A_for_bmax_D = -1.0;
	double b_for_Amin_D = -0.1;
	double b_for_Amax_D =  0.1;
	//
	double A_min_1s_BCD =  0.0;
	double A_max_1s_BCD = -1.0;
	double b_min_1s_BCD =  0.1;
	double b_max_1s_BCD = -0.1;
	double A_for_bmin_BCD =  0.0;
	double A_for_bmax_BCD = -1.0;
	double b_for_Amin_BCD = -0.1;
	double b_for_Amax_BCD =  0.1;
	//
	double A_min_90_B =  0.0;
	double A_max_90_B = -1.0;
	double b_min_90_B =  0.1;
	double b_max_90_B = -0.1;
	double A_min_90_C =  0.0;
	double A_max_90_C = -1.0;
	double b_min_90_C =  0.1;
	double b_max_90_C = -0.1;
	double A_min_90_D =  0.0;
	double A_max_90_D = -1.0;
	double b_min_90_D =  0.1;
	double b_max_90_D = -0.1;
	double A_min_90_BCD =  0.0;
	double A_max_90_BCD = -1.0;
	double b_min_90_BCD =  0.1;
	double b_max_90_BCD = -0.1;
	//
	double A_min_95_B =  0.0;
	double A_max_95_B = -1.0;
	double b_min_95_B =  0.1;
	double b_max_95_B = -0.1;
	double A_min_95_C =  0.0;
	double A_max_95_C = -1.0;
	double b_min_95_C =  0.1;
	double b_max_95_C = -0.1;
	double A_min_95_D =  0.0;
	double A_max_95_D = -1.0;
	double b_min_95_D =  0.1;
	double b_max_95_D = -0.1;
	double A_min_95_BCD =  0.0;
	double A_max_95_BCD = -1.0;
	double b_min_95_BCD =  0.1;
	double b_max_95_BCD = -0.1;
	//
	
	for(int j=0; j<N_A_vals; j++)  // skip underflow and overflow bins. ... no don't.
	{
		this_A = the_Amin+0.5*dA + double(j)*dA;
		for(int i=0; i<N_b_vals; i++)
		{
			this_b = the_bmin+0.5*db + double(i)*db;
			this_chi2_B     = fitmap_B->GetBinContent(i+1, j+1);
			this_chi2_C     = fitmap_C->GetBinContent(i+1, j+1);
			this_chi2_D     = fitmap_D->GetBinContent(i+1, j+1);
			this_chi2_BCD = fitmap_BCD->GetBinContent(i+1, j+1);
			if(this_chi2_B <= minchi2_B+2.3 )  // 1 sigma
			{
				if(this_A < A_min_1s_B) 
				{
					A_min_1s_B   = this_A; 
					b_for_Amin_B = this_b;
				}
				if(this_A > A_max_1s_B) 
				{
					A_max_1s_B   = this_A;
					b_for_Amax_B = this_b;
				}
				if(this_b < b_min_1s_B) 
				{
					b_min_1s_B   = this_b;
					A_for_bmin_B = this_A;
				}
				if(this_b > b_max_1s_B) 
				{
					b_max_1s_B   = this_b;
					A_for_bmax_B = this_A;
				}
			}
			//
			if(this_chi2_C <= minchi2_C+2.3 )  // 1 sigma
			{
				if(this_A < A_min_1s_C) 
				{
					A_min_1s_C   = this_A; 
					b_for_Amin_C = this_b;
				}
				if(this_A > A_max_1s_C) 
				{
					A_max_1s_C   = this_A;
					b_for_Amax_C = this_b;
				}
				//
				if(this_b < b_min_1s_C) 
				{
					b_min_1s_C   = this_b;
					A_for_bmin_C = this_A;
				}
				if(this_b > b_max_1s_C) 
				{
					b_max_1s_C   = this_b;
					A_for_bmax_C = this_A;
				}
			}
			//
			if(this_chi2_D <= minchi2_D+2.3 )  // 1 sigma
			{
				if(this_A < A_min_1s_D) 
				{
					A_min_1s_D   = this_A; 
					b_for_Amin_D = this_b;
				}
				if(this_A > A_max_1s_D) 
				{
					A_max_1s_D   = this_A;
					b_for_Amax_D = this_b;
				}
				//
				if(this_b < b_min_1s_D) 
				{
					b_min_1s_D   = this_b;
					A_for_bmin_D = this_A;
				}
				if(this_b > b_max_1s_D) 
				{
					b_max_1s_D   = this_b;
					A_for_bmax_D = this_A;
				}
			}
			//
			if(this_chi2_BCD <= minchi2_BCD+2.3 )  // 1 sigma
			{
				if(this_A < A_min_1s_BCD) 
				{
					A_min_1s_BCD   = this_A; 
					b_for_Amin_BCD = this_b;
				}
				if(this_A > A_max_1s_BCD) 
				{
					A_max_1s_BCD   = this_A;
					b_for_Amax_BCD = this_b;
				}
				if(this_b < b_min_1s_BCD) 
				{
					b_min_1s_BCD   = this_b;
					A_for_bmin_BCD = this_A;
				}
				if(this_b > b_max_1s_BCD) 
				{
					b_max_1s_BCD   = this_b;
					A_for_bmax_BCD = this_A;
				}
			}
			// -- * -- //
			if(this_chi2_B <= minchi2_B+4.61 )  // 90% CI
			{
				if(this_A < A_min_90_B)  { A_min_90_B = this_A; }
				if(this_A > A_max_90_B)  { A_max_90_B = this_A; }
				if(this_b < b_min_90_B)  { b_min_90_B = this_b; }
				if(this_b > b_max_90_B)  { b_max_90_B = this_b; }
			}
			if(this_chi2_C <= minchi2_C+4.61 )  // 90% CI
			{
				if(this_A < A_min_90_C)  { A_min_90_C = this_A; }
				if(this_A > A_max_90_C)  { A_max_90_C = this_A; }
				if(this_b < b_min_90_C)  { b_min_90_C = this_b; }
				if(this_b > b_max_90_C)  { b_max_90_C = this_b; }
			}
			if(this_chi2_D <= minchi2_D+4.61 )  // 90% CI
			{
				if(this_A < A_min_90_D)  { A_min_90_D = this_A; }
				if(this_A > A_max_90_D)  { A_max_90_D = this_A; }
				if(this_b < b_min_90_D)  { b_min_90_D = this_b; }
				if(this_b > b_max_90_D)  { b_max_90_D = this_b; }
			}
			if(this_chi2_BCD <= minchi2_BCD+4.61 )  // 90% CI
			{
				if(this_A < A_min_90_BCD)  { A_min_90_BCD = this_A; }
				if(this_A > A_max_90_BCD)  { A_max_90_BCD = this_A; }
				if(this_b < b_min_90_BCD)  { b_min_90_BCD = this_b; }
				if(this_b > b_max_90_BCD)  { b_max_90_BCD = this_b; }
			}
			// -- * -- //
			if(this_chi2_B <= minchi2_B+5.99 )  // 95% CI
			{
				if(this_A < A_min_95_B)  { A_min_95_B = this_A; }
				if(this_A > A_max_95_B)  { A_max_95_B = this_A; }
				if(this_b < b_min_95_B)  { b_min_95_B = this_b; }
				if(this_b > b_max_95_B)  { b_max_95_B = this_b; }
			}
			if(this_chi2_C <= minchi2_C+5.99 )  // 95% CI
			{
				if(this_A < A_min_95_C)  { A_min_95_C = this_A; }
				if(this_A > A_max_95_C)  { A_max_95_C = this_A; }
				if(this_b < b_min_95_C)  { b_min_95_C = this_b; }
				if(this_b > b_max_95_C)  { b_max_95_C = this_b; }
			}
			if(this_chi2_D <= minchi2_D+5.99 )  // 95% CI
			{
				if(this_A < A_min_95_D)  { A_min_95_D = this_A; }
				if(this_A > A_max_95_D)  { A_max_95_D = this_A; }
				if(this_b < b_min_95_D)  { b_min_95_D = this_b; }
				if(this_b > b_max_95_D)  { b_max_95_D = this_b; }
			}
			if(this_chi2_BCD <= minchi2_BCD+5.99 )  // 95% CI
			{
				if(this_A < A_min_95_BCD)  { A_min_95_BCD = this_A; }
				if(this_A > A_max_95_BCD)  { A_max_95_BCD = this_A; }
				if(this_b < b_min_95_BCD)  { b_min_95_BCD = this_b; }
				if(this_b > b_max_95_BCD)  { b_max_95_BCD = this_b; }
			}
			//
		}
	}
	cout << "Fourth and final loop (chi^2) is done." << endl;
	
	cout << "Best Fit Results:  " << endl;
	cout << "\tminchi2_B = " << minchi2_B << endl;
	cout << "\tA_best_B  = " << A_best_B << ", [" << A_min_1s_B << ", " << A_max_1s_B << "]" << endl;
	cout << "\tb_best_B  = " << b_best_B << ", [" << b_min_1s_B << ", " << b_max_1s_B << "]" << endl;
	cout << "\tminchi2_C = " << minchi2_C << endl;
	cout << "\tA_best_C  = " << A_best_C << ", [" << A_min_1s_C << ", " << A_max_1s_C << "]" << endl;
	cout << "\tb_best_C  = " << b_best_C << ", [" << b_min_1s_C << ", " << b_max_1s_C << "]" << endl;
	cout << "\tminchi2_D = " << minchi2_D << endl;
	cout << "\tA_best_D  = " << A_best_D << ", [" << A_min_1s_D << ", " << A_max_1s_D << "]" << endl;
	cout << "\tb_best_D  = " << b_best_D << ", [" << b_min_1s_D << ", " << b_max_1s_D << "]" << endl;
	cout << "\tminchi2_BCD = " << minchi2_BCD << endl;
	cout << "\tA_best_BCD  = " << A_best_BCD << ", [" << A_min_1s_BCD << ", " << A_max_1s_BCD << "]" << endl;
	cout << "\tb_best_BCD  = " << b_best_BCD << ", [" << b_min_1s_BCD << ", " << b_max_1s_BCD << "]" << endl;
	
	
	double epsilon = 0.0001;
	double contours_onesig_B[2];
	contours_onesig_B[0] = minchi2_B + epsilon;
	contours_onesig_B[1] = minchi2_B + 2.30;
	double contours_onesig_C[2];
	contours_onesig_C[0] = minchi2_C + epsilon;
	contours_onesig_C[1] = minchi2_C + 2.30;
	double contours_onesig_D[2];
	contours_onesig_D[0] = minchi2_D + epsilon;
	contours_onesig_D[1] = minchi2_D + 2.30;
	double contours_onesig_BCD[2];
	contours_onesig_BCD[0] = minchi2_BCD + epsilon;
	contours_onesig_BCD[1] = minchi2_BCD + 2.30;

	
	double contours_full_B[4];
	contours_full_B[0] = minchi2_B + epsilon;
	contours_full_B[1] = minchi2_B + 2.30;  // 68.27 %
	contours_full_B[2] = minchi2_B + 4.61;  // 90 %
	contours_full_B[3] = minchi2_B + 5.99;  // 95 %
	double contours_full_C[4];
	contours_full_C[0] = minchi2_C + epsilon;
	contours_full_C[1] = minchi2_C + 2.30;  // 68.27 %
	contours_full_C[2] = minchi2_C + 4.61;  // 90 %
	contours_full_C[3] = minchi2_C + 5.99;  // 95 %
	double contours_full_D[4];
	contours_full_D[0] = minchi2_D + epsilon;
	contours_full_D[1] = minchi2_D + 2.30;  // 68.27 %
	contours_full_D[2] = minchi2_D + 4.61;  // 90 %
	contours_full_D[3] = minchi2_D + 5.99;  // 95 %
	double contours_full_BCD[4];
	contours_full_BCD[0] = minchi2_BCD + epsilon;
	contours_full_BCD[1] = minchi2_BCD + 2.30;  // 68.27 %
	contours_full_BCD[2] = minchi2_BCD + 4.61;  // 90 %
	contours_full_BCD[3] = minchi2_BCD + 5.99;  // 95 %
	
	
	double view_xmin = the_bmin;
	double view_ymin = the_Amin;
	TLine * A_best_line_B = new TLine(view_xmin, A_best_B, b_best_B, A_best_B);
	TLine * b_best_line_B = new TLine(b_best_B, view_ymin, b_best_B, A_best_B);
	A_best_line_B->SetLineColor(kRed);
	b_best_line_B->SetLineColor(kRed);

	TLine * A_min_line_B = new TLine(view_xmin,  A_for_bmax_B, b_max_1s_B, A_for_bmax_B);
	TLine * b_min_line_B = new TLine(b_min_1s_B, view_ymin,    b_min_1s_B, A_for_bmin_B);
	A_min_line_B->SetLineColor(kRed);
	b_min_line_B->SetLineColor(kRed);

	TLine * A_max_line_B = new TLine(view_xmin,  A_for_bmin_B, b_min_1s_B, A_for_bmin_B);
	TLine * b_max_line_B = new TLine(b_max_1s_B, view_ymin,    b_max_1s_B, A_for_bmax_B);
	A_max_line_B->SetLineColor(kRed);
	b_max_line_B->SetLineColor(kRed);
	//
	TLine * A_best_line_C = new TLine(view_xmin, A_best_C, b_best_C, A_best_C);
	TLine * b_best_line_C = new TLine(b_best_C, view_ymin, b_best_C, A_best_C);
	A_best_line_C->SetLineColor(kRed);
	b_best_line_C->SetLineColor(kRed);

	TLine * A_min_line_C = new TLine(view_xmin, A_for_bmax_C, b_max_1s_C, A_for_bmax_C);
	TLine * b_min_line_C = new TLine(b_min_1s_C, view_ymin, b_min_1s_C, A_for_bmin_C);
	A_min_line_C->SetLineColor(kRed);
	b_min_line_C->SetLineColor(kRed);

	TLine * A_max_line_C = new TLine(view_xmin,  A_for_bmin_C, b_min_1s_C, A_for_bmin_C);
	TLine * b_max_line_C = new TLine(b_max_1s_C, view_ymin,    b_max_1s_C, A_for_bmax_C);
	A_max_line_C->SetLineColor(kRed);
	b_max_line_C->SetLineColor(kRed);
	//
	TLine * A_best_line_D = new TLine(view_xmin, A_best_D, b_best_D, A_best_D);
	TLine * b_best_line_D = new TLine(b_best_D, view_ymin, b_best_D, A_best_D);
	A_best_line_D->SetLineColor(kRed);
	b_best_line_D->SetLineColor(kRed);

	TLine * A_min_line_D = new TLine(view_xmin,  A_for_bmax_D, b_max_1s_D, A_for_bmax_D);
	TLine * b_min_line_D = new TLine(b_min_1s_D, view_ymin,    b_min_1s_D, A_for_bmin_D);
	A_min_line_D->SetLineColor(kRed);
	b_min_line_D->SetLineColor(kRed);

	TLine * A_max_line_D = new TLine(view_xmin,  A_for_bmin_D, b_min_1s_D, A_for_bmin_D);
	TLine * b_max_line_D = new TLine(b_max_1s_D, view_ymin,    b_max_1s_D, A_for_bmax_D);
	A_max_line_D->SetLineColor(kRed);
	b_max_line_D->SetLineColor(kRed);
	//
	TLine * A_best_line_BCD = new TLine(view_xmin, A_best_BCD, b_best_BCD, A_best_BCD);
	TLine * b_best_line_BCD = new TLine(b_best_BCD, view_ymin, b_best_BCD, A_best_BCD);
	A_best_line_BCD->SetLineColor(kRed);
	b_best_line_BCD->SetLineColor(kRed);

	TLine * A_min_line_BCD = new TLine(view_xmin,    A_for_bmax_BCD, b_max_1s_BCD, A_for_bmax_BCD);
	TLine * b_min_line_BCD = new TLine(b_min_1s_BCD, view_ymin,      b_min_1s_BCD, A_for_bmin_BCD);
	A_min_line_BCD->SetLineColor(kRed);
	b_min_line_BCD->SetLineColor(kRed);

	TLine * A_max_line_BCD = new TLine(view_xmin,    A_for_bmin_BCD, b_min_1s_BCD, A_for_bmin_BCD);
	TLine * b_max_line_BCD = new TLine(b_max_1s_BCD, view_ymin,      b_max_1s_BCD, A_for_bmax_BCD);
	A_max_line_BCD->SetLineColor(kRed);
	b_max_line_BCD->SetLineColor(kRed);
	//
	double asymmetry_ymin = -0.60;
	double asymmetry_ymax = -0.45;
	TLine * fitrange_min_line = new TLine(400.0,  asymmetry_ymin, 400.0,  asymmetry_ymax);
	TLine * fitrange_max_line = new TLine(4800.0, asymmetry_ymin, 4800.0, asymmetry_ymax);
	fitrange_min_line->SetLineColor(kGray);
	fitrange_max_line->SetLineColor(kGray);
	
	
	//
	TH1D* fitmap_contours_B =(TH1D*)fitmap_B->Clone();
	fitmap_contours_B -> SetContour(2,contours_onesig_B);
	fitmap_contours_B -> SetLineColor(kOrange+10);	
	TH1D* fitmap_contours2_B =(TH1D*)fitmap_B->Clone();
	fitmap_contours2_B -> SetContour(4,contours_full_B);
	fitmap_contours2_B -> SetLineColor(kViolet-2);
	//
	TH1D* fitmap_contours_C =(TH1D*)fitmap_C->Clone();
	fitmap_contours_C -> SetContour(2,contours_onesig_C);
	fitmap_contours_C -> SetLineColor(kOrange+10);	
	TH1D* fitmap_contours2_C =(TH1D*)fitmap_C->Clone();
	fitmap_contours2_C -> SetContour(4,contours_full_C);
	fitmap_contours2_C -> SetLineColor(kViolet-2);
	//
	TH1D* fitmap_contours_D =(TH1D*)fitmap_D->Clone();
	fitmap_contours_D -> SetContour(2,contours_onesig_D);
	fitmap_contours_D -> SetLineColor(kOrange+10);	
	TH1D* fitmap_contours2_D =(TH1D*)fitmap_D->Clone();
	fitmap_contours2_D -> SetContour(4,contours_full_D);
	fitmap_contours2_D -> SetLineColor(kViolet-2);
	//
	TH1D* fitmap_contours_BCD =(TH1D*)fitmap_BCD->Clone();
	fitmap_contours_BCD -> SetContour(2,contours_onesig_BCD);
	fitmap_contours_BCD -> SetLineColor(kOrange+10);	
	TH1D* fitmap_contours2_BCD =(TH1D*)fitmap_BCD->Clone();
	fitmap_contours2_BCD -> SetContour(4,contours_full_BCD);
	fitmap_contours2_BCD -> SetLineColor(kViolet-2);
	//
	
	//
	string title_2D_B = "                SetB, BS="+fake_to_string(the_toppings.BS_scale*100.0)+"\%, SS="+fake_to_string(the_toppings.SS_scale*100.0)+"\%, BG="+fake_to_string(the_toppings.BG_scale*100.0)+"\%";
	string title_2D_C = "                SetC, BS="+fake_to_string(the_toppings.BS_scale*100.0)+"\%, SS="+fake_to_string(the_toppings.SS_scale*100.0)+"\%, BG="+fake_to_string(the_toppings.BG_scale*100.0)+"\%";
	string title_2D_D = "                SetD, BS="+fake_to_string(the_toppings.BS_scale*100.0)+"\%, SS="+fake_to_string(the_toppings.SS_scale*100.0)+"\%, BG="+fake_to_string(the_toppings.BG_scale*100.0)+"\%";
	string title_2D_BCD = "                SetsBCD, BS="+fake_to_string(the_toppings.BS_scale*100.0)+"\%, SS="+fake_to_string(the_toppings.SS_scale*100.0)+"\%, BG="+fake_to_string(the_toppings.BG_scale*100.0)+"\%";
	//
	string A_results_string_B = "A="+fake_to_string(A_best_B)+", ["+fake_to_string(A_min_1s_B)+", "+fake_to_string(A_max_1s_B)+"]";
	string b_results_string_B = "b="+fake_to_string(b_best_B)+", ["+fake_to_string(b_min_1s_B)+", "+fake_to_string(b_max_1s_B)+"]";
	string chimin_string_B    = "min. chi2="+fake_to_string(minchi2_B)+" [NDF=42]";
	
	string A_results_string_C = "A="+fake_to_string(A_best_C)+", ["+fake_to_string(A_min_1s_C)+", "+fake_to_string(A_max_1s_C)+"]";
	string b_results_string_C = "b="+fake_to_string(b_best_C)+", ["+fake_to_string(b_min_1s_C)+", "+fake_to_string(b_max_1s_C)+"]";
	string chimin_string_C    = "min. chi2="+fake_to_string(minchi2_C)+" [NDF=42]";
	
	string A_results_string_D = "A="+fake_to_string(A_best_D)+", ["+fake_to_string(A_min_1s_D)+", "+fake_to_string(A_max_1s_D)+"]";
	string b_results_string_D = "b="+fake_to_string(b_best_D)+", ["+fake_to_string(b_min_1s_D)+", "+fake_to_string(b_max_1s_D)+"]";
	string chimin_string_D    = "min. chi2="+fake_to_string(minchi2_D)+" [NDF=42]";
	
	string N_vals_string      = "pix:  "+fake_to_string(N_b_vals)+" x "+fake_to_string(N_A_vals);
	string res_string         = "res:  dA="+fake_to_string(dA)+",  db="+fake_to_string(db);
	
	string A_results_string_BCD = "A="+fake_to_string(A_best_BCD)+", ["+fake_to_string(A_min_1s_BCD)+", "+fake_to_string(A_max_1s_BCD)+"]";
	string b_results_string_BCD = "b="+fake_to_string(b_best_BCD)+", ["+fake_to_string(b_min_1s_BCD)+", "+fake_to_string(b_max_1s_BCD)+"]";
	string chimin_string_BCD    = "min.chi2="+fake_to_string(minchi2_BCD)+" [NDF=126]";
	//
	
	string outfnamestub = "FitMapBCD_N"+fake_to_string(N_b_vals)+"x"+fake_to_string(N_A_vals);
	string ff_sig_str = convertDouble(sigma_cut);
	if( (sigma_cut-double(int(sigma_cut)))==0.0 ) { ff_sig_str.replace(1, 1, "p0"); }
	else                                          { ff_sig_str.replace(1, 1, "p" ); }
	string bbr_str = convertDouble(bb1_maxr);
	if( (bb1_maxr-double(int(bb1_maxr)))==0.0 )   { bbr_str.replace(2, 1, "p0"); }
	else                                          { bbr_str.replace(2, 1, "p" ); }
	outfnamestub = outfnamestub+"_ind"+int_to_string(threshold_index)+"_sig"+ff_sig_str+"_bbr"+bbr_str+"_bbthr"+int_to_string(bb1_threshold);
	outfnamestub = outfnamestub+"_BS"+fake_to_string(the_toppings.BS_scale*100.0)+"_SS"+fake_to_string(the_toppings.SS_scale*100.0)+"_BG"+fake_to_string(the_toppings.BG_scale*100.0);
	
	//
	outfnamestub=outfnamestub+"_zoom"+int_to_string(zoomlevel);
	if(cook_fast)
	{
		outfnamestub = outfnamestub + "_simple";
	}
	
	double z_min = minchi2_B;
	if(minchi2_C < z_min) { z_min = minchi2_C; }
	if(minchi2_D < z_min) { z_min = minchi2_D; }
	z_min = z_min-epsilon;
	//
	double z_max = minchi2_B;
	if(minchi2_C > z_max) { z_max = minchi2_C; }
	if(minchi2_D > z_max) { z_max = minchi2_D; }
	z_max = z_max+5.99+epsilon;
	//
	fitmap_B -> GetZaxis() -> SetRangeUser(z_min, z_max);
	fitmap_B -> SetName(title_2D_B.c_str());
	fitmap_B -> SetTitle(title_2D_B.c_str());
	
	fitmap_C -> GetZaxis() -> SetRangeUser(z_min, z_max);
	fitmap_C -> SetName(title_2D_C.c_str());
	fitmap_C -> SetTitle(title_2D_C.c_str());
	
	fitmap_D -> GetZaxis() -> SetRangeUser(z_min, z_max);
	fitmap_D -> SetName(title_2D_D.c_str());
	fitmap_D -> SetTitle(title_2D_D.c_str());
	
	fitmap_BCD -> GetZaxis() -> SetRangeUser(minchi2_BCD-epsilon, minchi2_BCD+5.99+epsilon);
	fitmap_BCD -> SetName(title_2D_D.c_str());
	fitmap_BCD -> SetTitle(title_2D_D.c_str());
	//
	
	TCanvas * c3_B = new TCanvas( "c3_B", "c3_B", 100, 0, 900, 700);
	c3_B->cd();
//	fitmap_B -> GetZaxis() -> SetRangeUser(34, 42);
	fitmap_B -> Draw("colz");
//	fitmap_contours_B ->Draw("cont3 same");
	fitmap_contours2_B->Draw("cont3 same");
	
	datalabel -> DrawText(0.10, 0.908, (A_results_string_B).c_str() );
	datalabel -> DrawText(0.10, 0.928, (b_results_string_B).c_str() );
	datalabel -> DrawText(0.10, 0.948, (chimin_string_B).c_str() );
	//
	datalabel_rhs -> DrawText(0.95, 0.948, (N_vals_string).c_str() );
	datalabel_rhs -> DrawText(0.95, 0.928, (res_string).c_str() );
	datalabel_rhs -> DrawText(0.95, 0.908, __SHORT_FORM_OF_FILE__);
	gPad->Update();
	
	A_best_line_B -> Draw("same");
	b_best_line_B -> Draw("same");
	A_min_line_B -> Draw("same");
	b_min_line_B -> Draw("same");
	A_max_line_B -> Draw("same");
	b_max_line_B -> Draw("same");
	c3_B->Modified();
	gPad->Update();
	//
	TCanvas * c3_C = new TCanvas( "c3_C", "c3_C", 100, 0, 900, 700);
	c3_C->cd();
//	fitmap_C -> GetZaxis() -> SetRangeUser(34, 42);
	fitmap_C -> Draw("colz");
//	fitmap_contours_C ->Draw("cont3 same");
	fitmap_contours2_C->Draw("cont3 same");
	
	datalabel -> DrawText(0.10, 0.908, (A_results_string_C).c_str() );
	datalabel -> DrawText(0.10, 0.928, (b_results_string_C).c_str() );
	datalabel -> DrawText(0.10, 0.948, (chimin_string_C).c_str() );
	//
	datalabel_rhs -> DrawText(0.95, 0.948, (N_vals_string).c_str() );
	datalabel_rhs -> DrawText(0.95, 0.928, (res_string).c_str() );
	datalabel_rhs -> DrawText(0.95, 0.908, __SHORT_FORM_OF_FILE__);
	gPad->Update();
	
	A_best_line_C -> Draw("same");
	b_best_line_C -> Draw("same");
	A_min_line_C -> Draw("same");
	b_min_line_C -> Draw("same");
	A_max_line_C -> Draw("same");
	b_max_line_C -> Draw("same");
	c3_C->Modified();
	gPad->Update();
	//
	TCanvas * c3_D = new TCanvas( "c3_D", "c3_D", 100, 0, 900, 700);
	c3_D->cd();
//	fitmap_D -> GetZaxis() -> SetRangeUser(34, 42);
	fitmap_D -> Draw("colz");
	fitmap_contours_D ->Draw("cont3 same");
//	fitmap_contours2_D->Draw("cont3 same");
	
	datalabel -> DrawText(0.10, 0.908, (A_results_string_D).c_str() );
	datalabel -> DrawText(0.10, 0.928, (b_results_string_D).c_str() );
	datalabel -> DrawText(0.10, 0.948, (chimin_string_D).c_str() );
	//
	datalabel_rhs -> DrawText(0.95, 0.948, (N_vals_string).c_str() );
	datalabel_rhs -> DrawText(0.95, 0.928, (res_string).c_str() );
	datalabel_rhs -> DrawText(0.95, 0.908, __SHORT_FORM_OF_FILE__);
	gPad->Update();
	//
	A_best_line_D -> Draw("same");
	b_best_line_D -> Draw("same");
	A_min_line_D -> Draw("same");
	b_min_line_D -> Draw("same");
	A_max_line_D -> Draw("same");
	b_max_line_D -> Draw("same");
	c3_D->Modified();
	gPad->Update();
	//
	TCanvas * c3_BCD = new TCanvas( "c3_BCD", "c3_BCD", 100, 0, 900, 700);
	c3_BCD->cd();
	fitmap_BCD -> Draw("colz");
//	fitmap_contours_BCD ->Draw("cont3 same");
	fitmap_contours2_BCD->Draw("cont3 same");
	
	datalabel -> DrawText(0.10, 0.908, (A_results_string_BCD).c_str() );
	datalabel -> DrawText(0.10, 0.928, (b_results_string_BCD).c_str() );
	datalabel -> DrawText(0.10, 0.948, (chimin_string_BCD).c_str() );
	//
	datalabel_rhs -> DrawText(0.95, 0.948, (N_vals_string).c_str() );
	datalabel_rhs -> DrawText(0.95, 0.928, (res_string).c_str() );
	datalabel_rhs -> DrawText(0.95, 0.908, __SHORT_FORM_OF_FILE__);
	gPad->Update();
	
	A_best_line_BCD -> Draw("same");
	b_best_line_BCD -> Draw("same");
	A_min_line_BCD -> Draw("same");
	b_min_line_BCD -> Draw("same");
	A_max_line_BCD -> Draw("same");
	b_max_line_BCD -> Draw("same");
	c3_B->Modified();
	gPad->Update();
	//
	
	TH1D * residuhist_B = get_residuals(h_best_B, h_Abeta_data_B, 0, 0);
//	cout << "Nbins in h_best_B:  " << h_best_B->GetNbinsX() << endl;
//	cout << "Nbins in h_data_B:  " << h_Abeta_data_B->GetNbinsX() << endl;
	
	TH1D * residuhist_C = get_residuals(h_best_C, h_Abeta_data_C, 0, 0);
//	cout << "Nbins in h_best_C:  " << h_best_C->GetNbinsX() << endl;
//	cout << "Nbins in h_data_C:  " << h_Abeta_data_C->GetNbinsX() << endl;
	
	TH1D * residuhist_D = get_residuals(h_best_D, h_Abeta_data_D, 0, 0);
//	cout << "Nbins in h_best_D:  " << h_best_D->GetNbinsX() << endl;
//	cout << "Nbins in h_data_D:  " << h_Abeta_data_D->GetNbinsX() << endl;
	
	
	TCanvas * c2_B = new TCanvas( "c2_B", "c2_B", 100, 0, 900, 700);
	h_Abeta_data_B->GetYaxis()->SetRangeUser(-0.6, -0.45);
	vector<TPad *> the_residupad1_B = make_residupad(h_Abeta_data_B, residuhist_B, string("E1") );
	the_residupad1_B.at(0) -> cd();
	h_Abeta_data_B -> GetXaxis()->SetNdivisions(10);  // 
	h_best_B->SetLineColor(sim_color);
	h_best_B->SetMarkerColor(h_best_B->GetLineColor());
	h_best_B -> Draw("E1 same");
//	h_Abeta_data_B -> Draw("e1 same");
	gPad->BuildLegend(.34,.80,.98,.92,"");
	fitrange_min_line->Draw("same");
	fitrange_max_line->Draw("same");
	gPad->Update();
	//
	TCanvas * c2_C = new TCanvas( "c2_C", "c2_C", 100, 0, 900, 700);
	h_Abeta_data_C->GetYaxis()->SetRangeUser(-0.6, -0.45);
	vector<TPad *> the_residupad1_C = make_residupad(h_Abeta_data_C, residuhist_C, string("E1") );
	the_residupad1_C.at(0) -> cd();
	h_Abeta_data_C -> GetXaxis()->SetNdivisions(10);  // 
	h_best_C->SetLineColor(sim_color);
	h_best_C->SetMarkerColor(h_best_C->GetLineColor());
	h_best_C -> Draw("E1 same");
//	h_Abeta_data_C -> Draw("e1 same");
	gPad->BuildLegend(.34,.80,.98,.92,"");
	fitrange_min_line->Draw("same");
	fitrange_max_line->Draw("same");
	gPad->Update();
	//
	TCanvas * c2_D = new TCanvas( "c2_D", "c2_D", 100, 0, 900, 700);
	h_Abeta_data_D->GetYaxis()->SetRangeUser(-0.6, -0.45);
	vector<TPad *> the_residupad1_D = make_residupad(h_Abeta_data_D, residuhist_D, string("E1") );
	the_residupad1_D.at(0) -> cd();
	h_Abeta_data_D -> GetXaxis()->SetNdivisions(10);  // 
	h_best_D->SetLineColor(sim_color);
	h_best_D->SetMarkerColor(h_best_D->GetLineColor());
	h_best_D -> Draw("E1 same");
//	h_Abeta_data_D -> Draw("e1 same");
	gPad->BuildLegend(.34,.80,.98,.92,"");
	fitrange_min_line->Draw("same");
	fitrange_max_line->Draw("same");
	gPad->Update();
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	
	string fitresults_string = "([Abest]+[Amin]+[Amax]) + ([bbest]+[bmin]+[bmax])*x +([dA]+[db])*x^2 +([chimin]+[ndf])*x^3";
	
	TF1* frf_1s_B = new TF1("frf_1s_B", fitresults_string.c_str(),  0, 6000.0);
	frf_1s_B->SetParameter("Abest", A_best_B);
	frf_1s_B->SetParameter("Amin",  A_min_1s_B);
	frf_1s_B->SetParameter("Amax",  A_max_1s_B);
	frf_1s_B->SetParameter("bbest", b_best_B);
	frf_1s_B->SetParameter("bmin",  b_min_1s_B);
	frf_1s_B->SetParameter("bmax",  b_max_1s_B);
	frf_1s_B->SetParameter("dA",    dA);
	frf_1s_B->SetParameter("db",    db);
	frf_1s_B->SetParameter("chimin",minchi2_B);
	frf_1s_B->SetParameter("ndf",   42);
	
	TF1* frf_90_B = new TF1("frf_90_B", fitresults_string.c_str(),  0, 6000.0);
	frf_90_B->SetParameter("Abest", A_best_B);
	frf_90_B->SetParameter("Amin",  A_min_90_B);
	frf_90_B->SetParameter("Amax",  A_max_90_B);
	frf_90_B->SetParameter("bbest", b_best_B);
	frf_90_B->SetParameter("bmin",  b_min_90_B);
	frf_90_B->SetParameter("bmax",  b_max_90_B);
	frf_90_B->SetParameter("dA",    dA);
	frf_90_B->SetParameter("db",    db);
	frf_90_B->SetParameter("chimin",minchi2_B);
	frf_90_B->SetParameter("ndf",   42);

	TF1* frf_95_B = new TF1("frf_95_B", fitresults_string.c_str(),  0, 6000.0);
	frf_95_B->SetParameter("Abest", A_best_B);
	frf_95_B->SetParameter("Amin",  A_min_95_B);
	frf_95_B->SetParameter("Amax",  A_max_95_B);
	frf_95_B->SetParameter("bbest", b_best_B);
	frf_95_B->SetParameter("bmin",  b_min_95_B);
	frf_95_B->SetParameter("bmax",  b_max_95_B);
	frf_95_B->SetParameter("dA",    dA);
	frf_95_B->SetParameter("db",    db);
	frf_95_B->SetParameter("chimin",minchi2_B);
	frf_95_B->SetParameter("ndf",   42);
	
	//
	TF1* frf_1s_C = new TF1("frf_1s_C", fitresults_string.c_str(),  0, 6000.0);
	frf_1s_C->SetParameter("Abest", A_best_C);
	frf_1s_C->SetParameter("Amin",  A_min_1s_C);
	frf_1s_C->SetParameter("Amax",  A_max_1s_C);
	frf_1s_C->SetParameter("bbest", b_best_C);
	frf_1s_C->SetParameter("bmin",  b_min_1s_C);
	frf_1s_C->SetParameter("bmax",  b_max_1s_C);
	frf_1s_C->SetParameter("dA",    dA);
	frf_1s_C->SetParameter("db",    db);
	frf_1s_C->SetParameter("chimin",minchi2_C);
	frf_1s_C->SetParameter("ndf",   42);
	
	TF1* frf_90_C = new TF1("frf_90_C", fitresults_string.c_str(),  0, 6000.0);
	frf_90_C->SetParameter("Abest", A_best_C);
	frf_90_C->SetParameter("Amin",  A_min_90_C);
	frf_90_C->SetParameter("Amax",  A_max_90_C);
	frf_90_C->SetParameter("bbest", b_best_C);
	frf_90_C->SetParameter("bmin",  b_min_90_C);
	frf_90_C->SetParameter("bmax",  b_max_90_C);
	frf_90_C->SetParameter("dA",    dA);
	frf_90_C->SetParameter("db",    db);
	frf_90_C->SetParameter("chimin",minchi2_C);
	frf_90_C->SetParameter("ndf",   42);

	TF1* frf_95_C = new TF1("frf_95_C", fitresults_string.c_str(),  0, 6000.0);
	frf_95_C->SetParameter("Abest", A_best_C);
	frf_95_C->SetParameter("Amin",  A_min_95_C);
	frf_95_C->SetParameter("Amax",  A_max_95_C);
	frf_95_C->SetParameter("bbest", b_best_C);
	frf_95_C->SetParameter("bmin",  b_min_95_C);
	frf_95_C->SetParameter("bmax",  b_min_95_C);
	frf_95_C->SetParameter("dA",    dA);
	frf_95_C->SetParameter("db",    db);
	frf_95_C->SetParameter("chimin",minchi2_C);
	frf_95_C->SetParameter("ndf",   42);
	//
	TF1* frf_1s_D = new TF1("frf_1s_D", fitresults_string.c_str(),  0, 6000.0);
	frf_1s_D->SetParameter("Abest", A_best_D);
	frf_1s_D->SetParameter("Amin",  A_min_1s_D);
	frf_1s_D->SetParameter("Amax",  A_max_1s_D);
	frf_1s_D->SetParameter("bbest", b_best_D);
	frf_1s_D->SetParameter("bmin",  b_min_1s_D);
	frf_1s_D->SetParameter("bmax",  b_max_1s_D);
	frf_1s_D->SetParameter("dA",    dA);
	frf_1s_D->SetParameter("db",    db);
	frf_1s_D->SetParameter("chimin",minchi2_D);
	frf_1s_D->SetParameter("ndf",   42);
	
	TF1* frf_90_D = new TF1("frf_90_D", fitresults_string.c_str(),  0, 6000.0);
	frf_90_D->SetParameter("Abest", A_best_D);
	frf_90_D->SetParameter("Amin",  A_min_90_D);
	frf_90_D->SetParameter("Amax",  A_max_90_D);
	frf_90_D->SetParameter("bbest", b_best_D);
	frf_90_D->SetParameter("bmin",  b_min_90_D);
	frf_90_D->SetParameter("bmax",  b_max_90_D);
	frf_90_D->SetParameter("dA",    dA);
	frf_90_D->SetParameter("db",    db);
	frf_90_D->SetParameter("chimin",minchi2_D);
	frf_90_D->SetParameter("ndf",   42);

	TF1* frf_95_D = new TF1("frf_95_D", fitresults_string.c_str(),  0, 6000.0);
	frf_95_D->SetParameter("Abest", A_best_D);
	frf_95_D->SetParameter("Amin",  A_min_95_D);
	frf_95_D->SetParameter("Amax",  A_max_95_D);
	frf_95_D->SetParameter("bbest", b_best_D);
	frf_95_D->SetParameter("bmin",  b_min_95_D);
	frf_95_D->SetParameter("bmax",  b_max_95_D);
	frf_95_D->SetParameter("dA",    dA);
	frf_95_D->SetParameter("db",    db);
	frf_95_D->SetParameter("chimin",minchi2_D);
	frf_95_D->SetParameter("ndf",   42);
	//
	TF1* frf_1s_BCD = new TF1("frf_1s_BCD", fitresults_string.c_str(),  0, 6000.0);
	frf_1s_BCD->SetParameter("Abest", A_best_BCD);
	frf_1s_BCD->SetParameter("Amin",  A_min_1s_BCD);
	frf_1s_BCD->SetParameter("Amax",  A_max_1s_BCD);
	frf_1s_BCD->SetParameter("bbest", b_best_BCD);
	frf_1s_BCD->SetParameter("bmin",  b_min_1s_BCD);
	frf_1s_BCD->SetParameter("bmax",  b_max_1s_BCD);
	frf_1s_BCD->SetParameter("dA",    dA);
	frf_1s_BCD->SetParameter("db",    db);
	frf_1s_BCD->SetParameter("chimin",minchi2_BCD);
	frf_1s_BCD->SetParameter("ndf",   42);
	
	TF1* frf_90_BCD = new TF1("frf_90_BCD", fitresults_string.c_str(),  0, 6000.0);
	frf_90_BCD->SetParameter("Abest", A_best_BCD);
	frf_90_BCD->SetParameter("Amin",  A_min_90_BCD);
	frf_90_BCD->SetParameter("Amax",  A_max_90_BCD);
	frf_90_BCD->SetParameter("bbest", b_best_BCD);
	frf_90_BCD->SetParameter("bmin",  b_min_90_BCD);
	frf_90_BCD->SetParameter("bmax",  b_max_90_BCD);
	frf_90_BCD->SetParameter("dA",    dA);
	frf_90_BCD->SetParameter("db",    db);
	frf_90_BCD->SetParameter("chimin",minchi2_BCD);
	frf_90_BCD->SetParameter("ndf",   42);

	TF1* frf_95_BCD = new TF1("frf_95_BCD", fitresults_string.c_str(),  0, 6000.0);
	frf_95_BCD->SetParameter("Abest", A_best_BCD);
	frf_95_BCD->SetParameter("Amin",  A_min_90_BCD);
	frf_95_BCD->SetParameter("Amax",  A_max_90_BCD);
	frf_95_BCD->SetParameter("bbest", b_best_BCD);
	frf_95_BCD->SetParameter("bmin",  b_min_90_BCD);
	frf_95_BCD->SetParameter("bmax",  b_max_90_BCD);
	frf_95_BCD->SetParameter("dA",    dA);
	frf_95_BCD->SetParameter("db",    db);
	frf_95_BCD->SetParameter("chimin",minchi2_BCD);
	frf_95_BCD->SetParameter("ndf",   42);
	//
	
	
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	fitresults these_results = fitresults();
	these_results.resultsfile       = outfnamestub+".root";
	these_results.orig_pizzafile_B  = pizza_path+the_pizza_B.pizza_stub+".root";
	these_results.orig_pizzafile_CD = pizza_path+the_pizza_CD.pizza_stub+".root";
	
	these_results.orig_datafile_B = datafilename_B;
	these_results.orig_datafile_C = datafilename_C;
	these_results.orig_datafile_D = datafilename_D;
	
	these_results.Amin = the_Amin;
	these_results.Amax = the_Amax;
	these_results.bmin = the_bmin;
	these_results.bmax = the_bmax;
	
	these_results.dA = dA;
	these_results.db = db;
	these_results.Nvals_A = N_A_vals;
	these_results.Nvals_b = N_b_vals;
	these_results.zoomlevel = zoomlevel;
	
	//
	these_results.BS_scale = the_toppings.BS_scale;
	these_results.SS_scale = the_toppings.SS_scale;
	these_results.BG_scale = the_toppings.BG_scale;
	these_results.bb1_snr_thresh = threshold_index;
	these_results.bb1_sigma_agr  = sigma_cut;
	these_results.bb1_maxr       = bb1_maxr;
	these_results.bb1_Ethresh    = bb1_threshold;
	
	these_results.Abest_BCD = A_best_BCD;
	these_results.bbest_BCD = b_best_BCD;
	these_results.Abest_B = A_best_B;
	these_results.bbest_B = b_best_B;
	these_results.Abest_C = A_best_C;
	these_results.bbest_C = b_best_C;
	these_results.Abest_D = A_best_D;
	these_results.bbest_D = b_best_D;
	//
	these_results.Amin1s_BCD = A_min_1s_BCD;
	these_results.Amax1s_BCD = A_max_1s_BCD;
	these_results.bmin1s_BCD = b_min_1s_BCD;
	these_results.bmax1s_BCD = b_max_1s_BCD;
	
	these_results.Amin1s_B = A_min_1s_B;
	these_results.Amax1s_B = A_max_1s_B;
	these_results.bmin1s_B = b_min_1s_B;
	these_results.bmax1s_B = b_max_1s_B;
	
	these_results.Amin1s_C = A_min_1s_C;
	these_results.Amax1s_C = A_max_1s_C;
	these_results.bmin1s_C = b_min_1s_C;
	these_results.bmax1s_C = b_max_1s_C;
	
	these_results.Amin1s_D = A_min_1s_D;
	these_results.Amax1s_D = A_max_1s_D;
	these_results.bmin1s_D = b_min_1s_D;
	these_results.bmax1s_D = b_max_1s_D;
	//
	these_results.Amin90_BCD = A_min_90_BCD;
	these_results.Amax90_BCD = A_max_90_BCD;
	these_results.bmin90_BCD = b_min_90_BCD;
	these_results.bmax90_BCD = b_max_90_BCD;

	these_results.Amin95_BCD = A_min_95_BCD;
	these_results.Amax95_BCD = A_max_95_BCD;
	these_results.bmin95_BCD = b_min_95_BCD;
	these_results.bmax95_BCD = b_max_95_BCD;
	
	
	//
	string results_filename = fitresults_path+results_file_namestub;
	if(cook_fast) { results_filename = results_filename + "_simple"; }
	results_filename = results_filename + ".txt";
	//
	these_results.save_descriptorfile(results_filename);
	
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	c3_B->Print( (fitresults_path+outfnamestub+".pdf[").c_str(), "pdf");  // no print
	//
	c3_B  ->Print( (fitresults_path+outfnamestub+".pdf").c_str(), "pdf");
	c3_C  ->Print( (fitresults_path+outfnamestub+".pdf").c_str(), "pdf");
	c3_D  ->Print( (fitresults_path+outfnamestub+".pdf").c_str(), "pdf");
	c3_BCD->Print( (fitresults_path+outfnamestub+".pdf").c_str(), "pdf");
	c2_B->Print( (fitresults_path+outfnamestub+".pdf").c_str(), "pdf");
	c2_C->Print( (fitresults_path+outfnamestub+".pdf").c_str(), "pdf");
	c2_D->Print( (fitresults_path+outfnamestub+".pdf").c_str(), "pdf");
	//
	c3_B->Print( (fitresults_path+outfnamestub+".pdf]").c_str(), "pdf");  // no print
	
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	TFile * f = new TFile( (fitresults_path+outfnamestub+".root").c_str(), "RECREATE");
	c3_B  -> Write("",TObject::kOverwrite);
	fitmap_B -> Write("",TObject::kOverwrite);
	fitmap_contours_B ->Write("",TObject::kOverwrite);
	fitmap_contours2_B->Write("",TObject::kOverwrite);
	c3_C  -> Write("",TObject::kOverwrite);
	fitmap_C -> Write("",TObject::kOverwrite);
	fitmap_contours_C ->Write("",TObject::kOverwrite);
	fitmap_contours2_C->Write("",TObject::kOverwrite);
	c3_D  -> Write("",TObject::kOverwrite);
	fitmap_D -> Write("",TObject::kOverwrite);
	fitmap_contours_D ->Write("",TObject::kOverwrite);
	fitmap_contours2_D->Write("",TObject::kOverwrite);
	c3_BCD-> Write("",TObject::kOverwrite);
	fitmap_BCD -> Write("",TObject::kOverwrite);
	fitmap_contours_BCD ->Write("",TObject::kOverwrite);
	fitmap_contours2_BCD->Write("",TObject::kOverwrite);
	//
	frf_1s_B -> Write("",TObject::kOverwrite);
	frf_90_B -> Write("",TObject::kOverwrite);
	frf_95_B -> Write("",TObject::kOverwrite);
	
	frf_1s_C -> Write("",TObject::kOverwrite);
	frf_90_C -> Write("",TObject::kOverwrite);
	frf_95_C -> Write("",TObject::kOverwrite);
	
	frf_1s_D -> Write("",TObject::kOverwrite);
	frf_90_D -> Write("",TObject::kOverwrite);
	frf_95_D -> Write("",TObject::kOverwrite);
	
	frf_1s_BCD -> Write("",TObject::kOverwrite);
	frf_90_BCD -> Write("",TObject::kOverwrite);
	frf_95_BCD -> Write("",TObject::kOverwrite);
	//
	c2_B -> Write("",TObject::kOverwrite);
	c2_C -> Write("",TObject::kOverwrite);
	c2_D -> Write("",TObject::kOverwrite);
	f->Close();
	
//	cout << "Running the rootapp." << endl;
//	rootapp->Run();
//	cout << "Done-ish?" << endl;

	return 0;
}
