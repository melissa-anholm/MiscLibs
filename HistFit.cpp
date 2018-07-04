// ==================================================================== //
// Code by Melissa Anholm
// 28.9.2017 - 3.7.2018
// 
// ==================================================================== //

// Standard includes:
#include <iostream>  // cout, endl;
#include <string>
#include <fstream>   // I think this probably helps...
#include <iomanip>   // setw

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::ofstream;
using std::setw;

// ROOT includes:
#include <TFile.h>
#include <TH1.h>
#include <TMinuit.h>
#include <TString.h>

//
#include "SomeFunctions.cpp"
#include "MinuitWrapperWrapper_fitparam.cpp"
#include "MinuitWrapperWrapper3.h"  // 


extern TH1D * hist1;
extern TH1D * hist2;

class SuperMinuit;
extern SuperMinuit * global_minuit;  // Create a global instance.

// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
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

class combo_histfitter
{
public:
	combo_histfitter(int n=0);
	void AddFitHist(TH1D* newfithist) { FitHist = newfithist; }
	void AddHistWithParam(TH1D*, FitParameter);
	void AddHistWithParam(FitParameter thisparam, TH1D* thishist) { AddHistWithParam(thishist, thisparam); }
	void RemoveHistAndParam(string);
	bool SetupTheFitter();
	TH1D * assemble_new_histogram();
	
private:
	vector<TH1D*> histvect;
	vector<FitParameter> paramvect;
	int n_params;
	TH1D * tmp_hist;
	TH1D * FitHist;
};

combo_histfitter::combo_histfitter(int n)
{
	n_params = n;
	tmp_hist = new TH1D();
	FitHist  = new TH1D();
}
void combo_histfitter::AddHistWithParam(TH1D* thishist, FitParameter thisparam)
{
	histvect.push_back(thishist);
	paramvect.push_back(thisparam);
	n_params++;
}
void combo_histfitter::RemoveHistAndParam(string paramname)
{
	bool found=false;
	int this_paramnum = -1;
	for(int i=0; i<n_params; i++)
	{
		if( paramvect.at(i).GetName().EqualTo( paramname.c_str() ) )
		{
			found=true;
			this_paramnum=i;
			break;
		}
	}
	
	if(found) 
	{ 
		vector<TH1D*> tmp_histvect;
		vector<FitParameter> tmp_paramvect;
		for(int j=0; j<n_params; j++)
		{
			if(j!=this_paramnum)
			{
				tmp_histvect.push_back(histvect.at(j));
				tmp_paramvect.push_back(paramvect.at(j));
			}
		}
		histvect = tmp_histvect;
		paramvect = tmp_paramvect;
		n_params--; 
	}
}


TH1D* combo_histfitter::assemble_new_histogram()
{
	tmp_hist = makehist_zeroslike(FitHist);
	for(int i=0; i<n_params; i++)
	{
		tmp_hist -> Add( histvect.at(i), paramvect.at(i).fit_val ); // might need to reference param from inside the superminuit.
	}
	return tmp_hist;
}

// Call SetupTheFitter() when hists and params are all loaded up and you're about to fit.
bool combo_histfitter::SetupTheFitter() // returns true if hists all have the same binning.
{
	bool samebinning = true;
	for(int i=0; i<n_params; i++)
	{
		samebinning = samebinning && HistsHaveSameBinning(FitHist, histvect.at(i));
	}
	if( !samebinning ) 
	{ 
		cout << "ERROR!  Hists don't have the same binning!  Fitter is not set up." << endl;
		return samebinning; 
	}
//	tmp_hist = makehist_zeroslike(FitHist); // must set up the temp histogram.
	assemble_new_histogram();  // do I need to do this here??
	
//	global_minuit = new SuperMinuit();
//	for(int i=0; i<n_params; i++)
//	{
//		global_minuit -> SetupParam(i, paramvect.at(i) );
//	}
	
//	global_minuit -> chf = this;
//	this -> TMinuit::SetFCN( NonMemberFitFunction ); // this line only works if it's a *static* void...
	
	return true;
}



