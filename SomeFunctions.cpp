// ==================================================================== //
// Code by Melissa Anholm
// June 2017 - 
// 
// consolidate_and_fit.cpp inherits from consolidate_results3.cpp
// LineshapeConvolution.h inherits from RhoLibs.h
// ==================================================================== //
#ifndef INCLUDE_SOMEFUNCTIONS
#define INCLUDE_SOMEFUNCTIONS 1


#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>   // do I need this?
#include <algorithm> // to manipulate strings.
#include <sstream> 
#include <utility>
#include <map>

//#include <TBranch.h>
#include <TFile.h>
//#include <TTree.h>

#include <TH1D.h>
//#include <TH2D.h>
//#include "TPad.h"
//#include <TCanvas.h>
//#include <TLegend.h>
//#include "TPaveText.h"
//#include "TPaveStats.h"

#include "TF1.h"
//#include "TArrayD.h"

//#include <TMinuit.h>
#include <TObject.h>

//#include <TLine.h>
//#include <TFitResult.h>
//#include <TFitResultPtr.h>
//#include <TCutG.h>
//#include <TMultiGraph.h>
//#include <TStyle.h>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::make_pair;
using std::pair;
using std::min;
using std::fixed;
using std::setprecision;


TF1* make_Abeta_func()
{
//	TF1* Abeta_func_keV = new TF1("Abeta_func_keV", "[0]*sqrt(1.0 - 510.9989461^2/(x+510.9989461)^2)", 0.0, 5100.0);
	TF1* Abeta_func_keV = new TF1("Abeta_func_keV", "[0]*sqrt(x^2 + 2*x*510.9989461) / (x+510.9989461)", 0.0, 5100.0);
	//double m_e = 510.9989461;
	Abeta_func_keV -> SetParNames("Abeta");
	Abeta_func_keV -> SetParameter("Abeta", -0.56);
	Abeta_func_keV -> SetParLimits(0, -0.59, -0.52);
	
	return Abeta_func_keV;
}
TF1* make_Abeta_b_func()
{
//	TF1* Abeta_b_func_keV = new TF1("Abeta_b_func_keV", "[0]*sqrt(1.0 - 510.9989461^2/(x+510.9989461)^2) * ( 1.0 + [1]*(510.9989461/x) )", 0.0, 5100.0);
	TF1* Abeta_b_func_keV = new TF1("Abeta_b_func_keV", "[0]*( sqrt(x^2 + 2*x*510.9989461) / (x+510.9989461) ) / (1.0 + [1]*(510.9989461/(x+510.9989461)) )", 0.0, 5100.0);
	Abeta_b_func_keV -> SetParNames("Abeta", "bFierz");
	Abeta_b_func_keV -> SetParameter("Abeta", -0.56);
	Abeta_b_func_keV -> SetParameter("bFierz", 0.0);
	
	return Abeta_b_func_keV;
}
TF1 * make_parabola_func()
{
	//double xmin = -0.590;
	//double xmax = -0.535;
	TF1* parabola = new TF1("parabola", "[1]^(-2)*(x-[0])^2 + [2]", -0.590, -0.535);
	parabola -> SetParNames("Abeta", "partial_dA", "vertex_y");
	parabola -> SetParameter("Abeta",      -0.56);
	parabola -> SetParameter("partial_dA",  0.0025);
	parabola -> SetParameter("vertex_y",   32.0);
	parabola -> SetLineColor(kGray);
	
	return parabola;
}

string double_to_string(double thisnumber, int thisprecision=3)  // 
{
	std::ostringstream oss;
	oss << fixed << setprecision(3) << thisnumber;
	
	string mynumberstring = oss.str();
	return mynumberstring;
}

// ---- // ---- // ---- // ---- // ---- // ---- //
bool HistsHaveSameBinning(TH1D *a, TH1D *b, bool verbose=false) 
{
	bool same = true;
	if (!a || !b) 
	{
		cout << "ERROR:  Histogram doesn't exist" << endl;
		cout << "a=" << a << ", b=" << b << endl;
		same = false;
	//	return same;
	}
	else if ( a -> GetNbinsX() != b -> GetNbinsX() ) 
	{
		cout << "ERROR:  Histograms have different numbers of bins." << endl;
		same = false;
	//	return same;
	}
	double eps = 1.E-3;
	if (same) 
	{
		for (int i = 1; i <= a -> GetNbinsX(); i++) 
		{
			if (fabs(a->GetBinCenter(i) - b->GetBinCenter(i)) > eps)
			{
				same = false;
			}
		}
	}
	//
	if(same && verbose)
	{
		cout << "Histograms " << a->GetName() << " and ";
		cout << b->GetName() << " have the same binning." << endl;
	}
	else if(!same)
	{
		cout << "ERROR:  bin centres are different." << endl;
	}
	return same;
}

double justgetthedamnchisquared(TH1D * h1, TH1D * h2, bool already_weighted=true, int bmin=0, int bmax=0)
{
	int verbose = 0;
	double chi2 = 0.0;
	int nbins = 0;
	
	double h1_bcont = 0.0;
	double h1_berr = 0.0;
	double h2_bcont = 0.0;
	double h2_berr = 0.0;
	double combined_berr = 1.0;
	double chi_contribution = 0.0;
	
	if(!already_weighted)
	{
		h1 -> Sumw2();
		h2 -> Sumw2();
	}
	if(HistsHaveSameBinning(h1, h2))
	{
		nbins = h1 -> GetNbinsX();
		
		if(bmax == 0 || bmax > nbins) { bmax = nbins; }
		if(bmin < 0 || bmin > bmax ) { bmin=0; }
		if(verbose>0)
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
				if(verbose>0)
				{
					cout << "* bin " << i << ":  setting combined bin err to 1." << endl;
				}
			}
			
			chi_contribution = (h1_bcont - h2_bcont)/combined_berr;
			chi2 = chi2 + chi_contribution*chi_contribution;
			if(verbose>0)
			{
				cout << "bin " << i;
				cout << " (" << h1->GetBinCenter(i) - 0.5*h1->GetBinWidth(i);
				cout << " - " <<  h1->GetBinCenter(i) +  0.5*h1->GetBinWidth(i);
				cout << ") \t" << "h1_bcont = " << h1_bcont << ", h2_bcont = " << h2_bcont;
				cout << ";\th1_berr = " << h1_berr << ", h2_berr = " << h2_berr;
				cout << ";\tchi2 = " << chi_contribution*chi_contribution << endl;
			}
		}
	}
	else
	{
		cout << "That's bad.  Hists should really have the same binning..." << endl;
	}
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


/*
void dump_parameter_to_terminal(int this_paramnumber)
{
	FitParameter this_param = global_minuit -> get_FitParameter(this_paramnumber);
	
	cout << "param:  " << this_paramnumber << "  ";
	cout << this_param.GetName() << ":  ";
	cout << this_param.fit_val << " +/- " << this_param.fit_err;
	cout << ", f_pnum = " << cout << this_param.f_pnum << endl;
}
void dump_fitresults()
{
	cout << "fmin_best    = " << global_minuit -> get_fmin_best() << endl;
	cout << "fedm_best    = " << global_minuit -> get_fedm_best() << endl;
	cout << "errdef_best  = " << global_minuit -> get_errdef_best() << endl;
}
*/
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
/*
void printresults_A(TFitResultPtr thisfit)
{
	cout << thisfit -> GetName() << ":   " << endl;
	cout << "A = " << thisfit->Parameter( thisfit->Index("Abeta") );
	cout << " +/- " << thisfit->ParError( thisfit->Index("Abeta") );
	cout << "\tchi^2/NDF = " << thisfit->Chi2() / double(thisfit->Ndf() - thisfit->NFreeParameters());
	cout << "\t[chi^2 = " << thisfit->Chi2() << ", NDF = " << thisfit->Ndf() - thisfit->NFreeParameters() << "]" << endl;
}
void printresults_A_b(TFitResultPtr thisfit)
{
//	cout << thisfit -> GetName() << ":   " << endl;
	cout << thisfit -> GetName() << ":   " << endl;
	cout << "A = " << thisfit->Parameter( thisfit->Index("Abeta") );
	cout << " +/- " << thisfit->ParError( thisfit->Index("Abeta") );
	cout << "\tb = " << thisfit->Parameter( thisfit->Index("bFierz") );
	cout << " +/- " << thisfit->ParError( thisfit->Index("bFierz") );
	cout << "\tchi^2/NDF = " << thisfit->Chi2() / double(thisfit->Ndf() - thisfit->NFreeParameters());
	cout << "\t[chi^2 = " << thisfit->Chi2() << ", NDF = " << thisfit->Ndf() - thisfit->NFreeParameters() << "]" << endl;
}

TH1D* make_th1_from_tf1(TF1* this_tf1, string hist_type, int this_color=int(kBlack), string new_hist_title=string(""))
{ // rescale so Abeta is 1 before calling this, maybe?
	int rebin_factor=1;
	if(new_hist_title==string("")) { new_hist_title = string(this_tf1->GetTitle()); }
	TH1D * new_hist = CreateHist(new_hist_title, hist_type, this_color, rebin_factor);
	int n_bins = new_hist->GetNbinsX();
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
		new_hist -> SetBinContent(i, this_tf1->Eval(new_hist->GetBinCenter(i)) );
	}
	return new_hist;
}

void fit_hist_to_hist(TH1D * h1, TH1D * h2, int bmin, int bmax)
{
	bool already_weighted=true;
//	(TH1D * h1, TH1D * h2, bool already_weighted=true, int bmin=0, int bmax=0)
	double chi2 = justgetthedamnchisquared(h1, h2, already_weighted, bmin, bmax);
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
*/
// --- // --- //
/*
double get_Abest(TFitResultPtr r)
{
	string A_parname  = "Abeta";
	int parnumber_A  = r->Index( A_parname);
	if (parnumber_A==-1) { return 0; }
	double A_best = r->Parameter(parnumber_A);
	return A_best;
}
double get_dA(TFitResultPtr r)
{
	string A_parname  = "Abeta";
	string dA_parname = "partial_dA";
	int parnumber_A  = r->Index( A_parname);
	int parnumber_dA = r->Index(dA_parname);
	if (parnumber_A==-1 || parnumber_dA == -1) { return 0; }
	
	double dA_fitminerr = r->ParError(parnumber_A);
	double dA_uponechi2 = r->Parameter(parnumber_dA);
	double A_best = r->Parameter(parnumber_A);
	
	double dA = sqrt(dA_fitminerr*dA_fitminerr + dA_uponechi2*dA_uponechi2);
	return dA;
}
double get_Amin(TFitResultPtr r)
{
	double A_best = get_Abest(r);
	double dA     = get_dA(r);
	double Amin = A_best - dA;
	
	return Amin;
}
double get_Amax(TFitResultPtr r)
{
	double A_best = get_Abest(r);
	double dA     = get_dA(r);
	double Amax = A_best + dA;
	
	return Amax;
}
*/
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#endif
