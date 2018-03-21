// ==================================================================== //
// Code by Melissa Anholm
// June 2017
// 
// 
// ==================================================================== //
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream> 
#include <utility>

#include <TH1D.h>
#include "TPad.h"
#include <TCanvas.h>
#include <TLegend.h>
#include "TPaveText.h"
#include "TPaveStats.h"

#include "TF1.h"
#include "TArrayD.h" // for reading out Sumw2 ?
#include "TColor.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

#include "/Users/spiffyzha/Packages/MiscLibs/HistExtras.cpp"
#include "/Users/spiffyzha/Packages/MiscLibs/ColorExtras.cpp"
#include "/Users/spiffyzha/Packages/MiscLibs/treeql_replacement.cpp"


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

TH1D * get_hist_sqrt(TH1D* input_hist, string title = string("Histogram") )
{
	TH1D * manip_hist = (TH1D*)input_hist -> Clone( title.c_str() );

	int n_bins = manip_hist->GetNbinsX();
	manip_hist -> Sumw2(kFALSE);  // 
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
		if( input_hist->GetBinContent(i) < 0 )
		{
			cout << "BAD!!  Bin " << i << " has contents:  " << input_hist->GetBinContent(i) << endl;
			cout << "Returning original histogram." << endl;
			return input_hist;
		}
		else
		{
			manip_hist -> SetBinContent(i, sqrt(input_hist->GetBinContent(i)) );
		}
	}
	manip_hist -> Sumw2();        // 
	return manip_hist;
}

void readout_sumw2(TH1D* hist)
{
	cout << "Reading:  " << hist -> GetTitle() << endl;
	TArrayD* errs = hist->GetSumw2();
	int n_bins = hist->GetNbinsX();
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
		cout << "Bin " << i << " at " << hist->GetBinCenter(i) << " has contents:  " << hist->GetBinContent(i) << " and sumw2:  " << errs -> At(i) << endl;
	}
	return;
}

void readout_bincenters(TH1D * hist)
{
	cout << "Reading:  " << hist -> GetTitle() << endl;
	int n_bins = hist->GetNbinsX();
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
		cout << "Bin " << i << " at " << hist->GetBinCenter(i) << endl; // << " has contents:  " << hist->GetBinContent(i) << " and sumw2:  " << errs -> At(i) << endl;
	}
	return;
}

double get_asymmetry(double r1p, double r1m, double r2p, double r2m)
{
	double A;
	if(r1p==0 && (r1m==0 || r2p==0) )
	{
		A = 0.0;
	}
	else if(r2m==0 && (r1m==0 || r2p==0) )
	{
		A = 0.0;
	}
	else
	{
		A = ( sqrt(r1p*r2m) - sqrt(r1m*r2p) ) / ( sqrt(r1p*r2m) + sqrt(r1m*r2p) );
	}
	return A;
}

double get_asymmetry_err(double r1p, double r1m, double r2p, double r2m)
{
	/*
	double t1 = sqrt(r1p*r2m);
	double t2 = sqrt(r1m*r2p);
	double dAdt1 =  2.0*t2/( (t1+t2)*(t1+t2) );
	double dAdt2 = -2.0*t1/( (t1+t2)*(t1+t2) );

	double DA2;
	if(t1==0)
	{
		dAdt2 = 0.0;
	}
	if(t2==0)
	{
		dAdt1 = 0.0;
	}
	DA2 = dAdt1*dAdt1*( 0.25*r2m + 0.25*r1p ) + dAdt2*dAdt2*( 0.25*r2p + 0.25*r1m );
	return sqrt(DA2);
	*/
	/*
	double s;
	double DA2 = 0.0;
	if(r1p!=0.0 && r1m!=0.0 && r2p!=0.0 && r2m!=0.0)
	{
		s = sqrt( (r1m*r2p) / (r1p*r2m) );
		DA2 = ( s*s / ((1.0+s)*(1.0+s)) ) * (1.0/r1p + 1.0/r1m + 1.0/r2p + 1.0/r2m);
	}
	return sqrt(DA2);
	*/
	/*
	double s;
	double DA2 = 0.0;
	if(r1p!=0.0 && r1m!=0.0 && r2p!=0.0 && r2m!=0.0)
	{
		s = sqrt( (r1m*r2p) / (r1p*r2m) );
		DA2 = 1.0/4.0 * pow( (s-1.0)/(s+1.0), 2) * s*s * (1.0/r1p + 1.0/r1m + 1.0/r2p + 1.0/r2m);
	}
	return sqrt(DA2);
	*/
	double s;
	double DA2 = 1.0;
	if(r1p!=0.0 && r1m!=0.0 && r2p!=0.0 && r2m!=0.0)
	{
		s = sqrt( (r1m*r2p) / (r1p*r2m) );
		DA2 = pow( -2.0/( (1.0+s)*(1.0+s) ), 2) * 1.0/4.0 * s*s * (1.0/r1p + 1.0/r1m + 1.0/r2p + 1.0/r2m);
	}
	else
	{
		cout << "r1p=" << r1p << "\tr1m=" << r1m << "\tr2p=" << r2p << "\tr2m=" << r2m << endl;
		if(r1p==0.0) {r1p=1.0;}
		if(r1m==0.0) {r1m=1.0;}
		if(r2p==0.0) {r2p=1.0;}
		if(r2m==0.0) {r2m=1.0;}
		s = sqrt( (r1m*r2p) / (r1p*r2m) );
		DA2 = pow( -2.0/( (1.0+s)*(1.0+s) ), 2) * 1.0/4.0 * s*s * (1.0/r1p + 1.0/r1m + 1.0/r2p + 1.0/r2m);
	}
	return sqrt(DA2);
}

TH1D * make_asymmetry_histogram(TH1D * r1p_hist, TH1D * r1m_hist, TH1D * r2p_hist, TH1D * r2m_hist, string hist_title = string("A_beta"), int color=int(kBlack), int plotmarkerstyle=20)
{
	int N_bins = 0;
	N_bins = r1p_hist->GetNbinsX();

	TH1D * superratio = (TH1D*)r1p_hist -> Clone( hist_title.c_str() );
	superratio -> SetName(hist_title.c_str());
	superratio -> SetTitle(hist_title.c_str());
	superratio -> SetLineColor(color);
	superratio -> SetMarkerColor(color);
	superratio -> SetLineWidth(2);
	superratio -> Sumw2(kFALSE);
	superratio -> Sumw2();
	
	double r1p, r1m, r2p, r2m;
	double bin_content, bin_err;
	
	if( r1p_hist->GetNbinsX()==r1m_hist->GetNbinsX() && r2p_hist->GetNbinsX()==r2m_hist->GetNbinsX() && r1p_hist->GetNbinsX()==r2p_hist->GetNbinsX() )
	{
		for (int i=1; i<N_bins; i++)  // Bins i=0, i=N_bins are the underflow and overflow?
		{
			superratio -> SetBinContent(i, 0.0);
			r1p = r1p_hist -> GetBinContent(i);
			r1m = r1m_hist -> GetBinContent(i);
			r2p = r2p_hist -> GetBinContent(i);
			r2m = r2m_hist -> GetBinContent(i);
			
		//	bin_content = get_asymmetry(r1p, r1m, r2p, r2m);
		//	bin_err = get_asymmetry_err(r1p, r1m, r2p, r2m);
	//		cout << "bin " << i << ":\t";
	//		cout << bin_content << " +/- " << bin_err << endl;
			
			superratio -> SetBinContent(i, get_asymmetry(r1p, r1m, r2p, r2m) );
			superratio -> SetBinError(i, get_asymmetry_err(r1p, r1m, r2p, r2m) );
			// SetBinError(...) sets the sumw2 to be err^2.
		}
	}
	else
	{
		cout << "Must use histograms with the same number of bins!" << endl;
	}
	
	superratio -> SetMarkerStyle(plotmarkerstyle);
//	superratio -> SetMarkerStyle(20);  // 20:  big circles.
//	superratio -> SetMarkerStyle(22);  // 22:  solid up-triangles.

	return superratio;
}

vector<TPad *> make_residupad(TH1D* top_hist, TH1D* bottom_hist, string top_draw_option=string("") )
{
	TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);
	pad2->SetTopMargin(0.00001);
	pad2->SetBottomMargin(0.3);
	pad2->SetBorderMode(0);
	pad2->SetGrid();
	pad2->Draw();

	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
	pad1->SetBottomMargin(0.00001);
	pad1->SetBorderMode(0);
	pad1->Draw();
	
	pad1 -> cd();
	
	top_hist -> GetYaxis() -> SetTitle( top_hist->GetTitle() );
//	top_hist -> SetTitle("");
	
	top_hist -> GetYaxis() -> SetTitleFont(63);
	top_hist -> GetYaxis() -> SetTitleSize(16);
	top_hist -> GetYaxis() -> SetTitleOffset(1.4);

	top_hist -> GetYaxis() -> SetLabelFont(63);
	top_hist -> GetYaxis() -> SetLabelSize(16);
	top_hist -> GetXaxis() -> SetLabelFont(63);
	top_hist -> GetXaxis() -> SetLabelSize(20);

//	top_hist -> GetXaxis() -> SetTickLength(0.09);
	top_hist -> GetYaxis() -> SetTickLength(0.04);

	top_hist -> Draw(top_draw_option.c_str());
	top_hist -> SetStats(0);
//	gPad->BuildLegend(.76,.90,.98,.98,"");
//	datalabelr -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	datalabelr -> DrawText(0.10, 0.930, (runlist).c_str());
	gPad -> Update();
	
	pad2 -> cd();

	bottom_hist -> GetYaxis() -> SetTitle( bottom_hist->GetTitle() );
//	bottom_hist -> GetYaxis() -> SetRangeUser( -2.0, 2.0 );

	bottom_hist -> SetTitle("");
	bottom_hist -> GetXaxis() -> SetRangeUser( pad1->GetUxmin(), pad1->GetUxmax() );
	
	bottom_hist -> GetYaxis() -> SetTitleFont(63);
	bottom_hist -> GetYaxis() -> SetTitleSize(14);
	bottom_hist -> GetYaxis() -> SetTitleOffset(1.7);

	bottom_hist -> GetXaxis() -> SetTitleOffset(3);
	bottom_hist -> GetXaxis() -> SetTitleFont(63);
	bottom_hist -> GetXaxis() -> SetTitleSize(16);
	bottom_hist -> GetXaxis() -> SetLabelFont(63);
	bottom_hist -> GetXaxis() -> SetLabelSize(14);
	
	bottom_hist -> GetYaxis() -> SetLabelFont(63);
	bottom_hist -> GetYaxis() -> SetLabelSize(16);

//	possibly SetTickSize and SetTickLength are the same?
//	bottom_hist -> GetXaxis() -> SetTickSize(0.2);  
	bottom_hist -> GetXaxis() -> SetTickLength(0.09);
	bottom_hist -> GetYaxis() -> SetTickLength(0.05);

	bottom_hist -> GetYaxis()->SetNdivisions(6);  // 
	bottom_hist -> GetXaxis()->SetNdivisions(10);  // 
	bottom_hist -> Draw();
	bottom_hist -> SetStats(0);
	gPad -> Update();
	
	vector<TPad *> pad_vector;
	pad_vector.push_back(pad1);
	pad_vector.push_back(pad2);
	return pad_vector;
}


TH1D * tf1_to_hist_like(TF1 * this_tf1, TH1D * this_hist, int this_color=int(kBlack) )
{
	TH1D * new_hist = (TH1D*)this_hist -> Clone( this_tf1->GetName() );
	new_hist -> Sumw2(kFALSE);
	new_hist -> SetMarkerColor(this_color);
	new_hist -> SetLineColor(this_color);
	
	int n_bins = new_hist->GetNbinsX();
	// well shit, it probably needs some parameters or something.
	
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
	//	this_tf1->Eval(this_hist->GetBinCenter(i));
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

double justgetthedamnchisquared(TH1D * h1, TH1D * h2, bool use_weighted, int bmin=0, int bmax=0)
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
			if(use_weighted)
			{
		//		cout << "bin " << i << ":\tBinError1 = " << h1_berr << ",\tBinError2 = " << h2_berr;// << endl;
		//		cout << ";\tBinContent1 = " << h1_bcont << ",  \tBinContent2 = " << h2_bcont << endl;
				// The sumw2 are squared, but the bin error is not.
			
				combined_berr = sqrt(h1_berr*h1_berr + h2_berr*h2_berr);
			
				// or ... is it???
			//	combined_berr = sqrt(h1_berr + h2_berr);
				
				if (combined_berr <= 0) { combined_berr = 1.0; }
		//		cout << "i=" << i << "\th1-h2=" << h1_bcont - h2_bcont << "\tcombined_berr = " << combined_berr << "\th1_berr = " << h1_berr << "\th2_berr = " << h2_berr << endl;
			}
			chi_contribution = (h1_bcont - h2_bcont)/combined_berr;
			chi2 = chi2 + chi_contribution*chi_contribution;
			
		//	cout << "i=" << i << "\th1-h2 = " << h1_bcont - h2_bcont << "  \tcombined_err = " << combined_berr << "  \tdchi2 = " << chi_contribution*chi_contribution << endl;
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
	return chi2;  // returns 0 if hists don't have the same binning.
}

TH1D* get_residuals(TH1D* h1, TH1D* h2)
{
//	// Residuals for BB1s, from Ben:
//	r = (exp - sim) / sqrt(sim);
//	residuals -> SetBinContent(i, r);
//	residuals -> SetBinError(i, 0.);
	
	// SetBinError(...) sets the sumw2 to be err^2.

	TH1D * new_hist = (TH1D*)h1 -> Clone( "Residuals" );
//	new_hist -> SetTitle("Residuals");
	new_hist -> SetTitle("(h1-h2) / sqrt(dh1^2+dh2^2) ");
	new_hist -> Sumw2(kFALSE);
	new_hist -> Sumw2();

	int n_bins  = h1->GetNbinsX();
	int n_bins2 = h2->GetNbinsX();
	
	TArrayD* errs1_2 = h1->GetSumw2(); // these are actually the *squares* of the weights.
	TArrayD* errs2_2 = h2->GetSumw2(); // GetSumw2 is the squares, GetBinError is the unsquared weights.
	
//	TArrayD* errs_new = new_hist->GetSumw2();
	
	if(n_bins != n_bins2)
	{
		cout << "******************************************************" << endl;
		cout << "ERROR!  h1 and h2 do not have the same number of bins!" << endl;
		cout << "Returning nonsense." << endl;
		cout << "******************************************************" << endl;
		return new_hist;
	}
	
	double dif;
	double sig;
	double res;
	for (int i =1; i<=n_bins; i++) 
	{
		dif = h1->GetBinContent(i) - h2->GetBinContent(i);
	//	sig = sqrt( h1->At(i) * h1->At(i) + h2->At(i) * h2->At(i) );
	//	sig = sqrt( h1->At(i) + h2->At(i) );
	//	sig = 1.0;
	//	cout << "i=" << i << ",\terrs1_2->At(i) = " << errs1_2->At(i) << ",\th1->GetBinError(i) = " << h1->GetBinError(i) << endl;
		sig = sqrt( errs1_2->At(i) + errs2_2->At(i) );
		if(sig==0)
		{
			cout << "bin " << i << " (" << h1->GetBinCenter(i) <<"):\tsig=" << sig << ", dif=" << dif << " -- setting sig=1:  res=" << dif << endl;
			sig = 1.0;
		}
		res = dif / sig;
		
		new_hist -> SetBinContent(i, res);
		new_hist -> SetBinError(i, 0.000001);
	}
	
	new_hist -> SetMarkerColor(kBlack);
	new_hist -> SetLineColor(kBlack);
	new_hist -> SetMarkerStyle(20);
//	new_hist -> GetYaxis() -> SetRangeUser(-0.08, 0.08);
	new_hist -> GetYaxis() -> SetRangeUser(-2.0, 2.0);
//	new_hist -> GetYaxis() -> SetRange(-1, -1);
	return new_hist;  // ...
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
