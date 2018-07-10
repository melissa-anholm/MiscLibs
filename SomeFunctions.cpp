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
#include "TColor.h"

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

//#include "SomeFunctions.h"

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
TF1* make_quasi_b_func()
{
	// (1.0 + [0]*(510.9989461/(x+510.9989461)) ) is exact;  divide 'A' by it.
//	TF1* b_func_keV = new TF1("b_func_keV", "(1.0 + [0]*(510.9989461/(x+510.9989461)) )", 0.0, 5100.0);
	
	// divide 'A' by (1+b_func_keV).
	TF1* b_func_keV = new TF1("b_func_keV", "[0]*(510.9989461/(x+510.9989461))", 0.0, 5100.0);
	b_func_keV -> SetParNames("bFierz");
	b_func_keV -> SetParameter("bFierz", 1.0);
	
	return b_func_keV;
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


TH1D * tf1_to_hist_like(TF1 * this_tf1, TH1D * this_hist, int this_color=int(kBlack) )
{
	TH1D * new_hist = (TH1D*)this_hist -> Clone( this_tf1->GetName() );
	new_hist -> Sumw2(kFALSE);
	new_hist -> SetMarkerColor(this_color);
	new_hist -> SetLineColor(this_color);
	
	int n_bins = new_hist->GetNbinsX();
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
		new_hist -> SetBinContent(i, this_tf1->Eval(this_hist->GetBinCenter(i)) );
	}
	return new_hist;
}
TH1D * tf1_to_hist_like(TF1 * this_tf1, TH1D * this_hist, TColor this_color)
{
	Int_t my_color = TColor::GetColor(this_color.GetRed(), this_color.GetGreen(), this_color.GetBlue());
	TH1D * new_th1d = tf1_to_hist_like(this_tf1, this_hist, my_color);
	return new_th1d;
}

/*
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
*/
TH1D* makehist_zeroslike(TH1D* oldhist)
{
	string newname = "tmpname";
	int newcolor = kBlack;
	TH1D * newhist = (TH1D*)oldhist -> Clone(newname.c_str());
	
	newhist -> Sumw2(kFALSE);
	newhist -> SetName(newname.c_str());
	newhist -> SetTitle(newname.c_str());
	newhist -> SetLineColor(newcolor);
	newhist -> SetMarkerColor(newcolor);
	
	int n_bins = newhist->GetNbinsX();
	newhist -> SetBinContent(n_bins,0);
	newhist -> SetBinContent(0,0);
	
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
		newhist -> SetBinContent(i,0);
	}
	return newhist;
}

TH1D* makehist_oneslike(TH1D* oldhist)
{
	string newname = "tmpname";
	int newcolor = kBlack;
	TH1D * newhist = (TH1D*)oldhist -> Clone(newname.c_str());
	
	newhist -> Sumw2(kFALSE);
	newhist -> SetName(newname.c_str());
	newhist -> SetTitle(newname.c_str());
	newhist -> SetLineColor(newcolor);
	newhist -> SetMarkerColor(newcolor);
	
	int n_bins = newhist->GetNbinsX();
	newhist -> SetBinContent(n_bins,0);
	newhist -> SetBinContent(0,0);
	
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
		newhist -> SetBinContent(i,1);
	}
	return newhist;
}

TH1D* makehist_Avoverc_like(TH1D* oldhist)
{
	TF1 * thetf1 = make_Abeta_func();
	thetf1 -> SetParLimits(0, -1.0, 1.0);
	thetf1 -> SetParameter("Abeta", 1.0);

	TH1D* newhist = tf1_to_hist_like(thetf1, oldhist);
	return newhist;
}
/*
double get_chi2_thisbin(double h1_bincontent, double h2_bincontent, double h1_berr=0, double h2_berr=0)
{
	double combined_berr;
	if(h1_berr==0 && h2_berr==0) { combined_berr = 1.0; }
	else { combined_berr = sqrt(h1_berr*h1_berr + h2_berr*h2_berr); }
	
	double this_chi = 0.0;
	this_chi = (h1_bincontent - h2_bincontent)/combined_berr;
	
	return this_chi*this_chi;
}
*/

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
