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


//extern TH1D * hist1;
//extern TH1D * hist2;

//class SuperMinuit;
extern SuperMinuit * global_minuit;  // Create a global instance.


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
/*
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
*/

void NonMember_HistFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_)
{
//	global_minuit -> DoThe_HistFitThing(n_params_, gin_, result_to_minimize_, parameters_, ierflg_);
	global_minuit -> DoTheThing(n_params_, gin_, result_to_minimize_, parameters_, ierflg_);
	
	global_minuit -> n_params_i         = n_params_;
	global_minuit -> gin                = gin_;
	global_minuit -> result_to_minimize = result_to_minimize_;
	global_minuit -> parameters         = parameters_;  // this doesn't really work??
	global_minuit -> ierflg             = ierflg_;
	
	return;
}


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
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

	int DoThe_HistFitThing(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_);
	
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
	
//	this -> SuperMinuit::init();
//	global_minuit = this;
//	this -> TMinuit::SetFCN( NonMember_HistFitFunction ); 
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
	
	global_minuit = new SuperMinuit();
	for(int i=0; i<n_params; i++)
	{
//		this -> SuperMinuit::SetupParam(i, paramvect.at(i) );
		global_minuit -> SuperMinuit::SetupParam(i, paramvect.at(i) );
	}
	
//	global_minuit -> chf = this;
//	this -> TMinuit::SetFCN( NonMemberFitFunction ); // this line only works if it's a *static* void...
	
	return true;
}

/*
int combo_histfitter::DoThe_HistFitThing(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_)
{
	result_to_minimize_ = 0;
	double this_Abeta = parameters_[0];
	
	// Which thing?
	if(n_params==1)  // chi^2 fit to A_beta*v/c
	{
		// hist1 is the original.
		// hist2 is the A*v/c histogram where we vary A.
	//	TH1D* this_h2 = makehist_A_v_over_c_like(this_Abeta, hist1);
		
		double tmp_result = 0.0;
	//	int n_bins = hist1->GetNbinsX();
	//	fit_bmin
		for(int i=fit_bmin; i<=fit_bmax; i++)
		{
	////		tmp_result =  get_chi2_thisbin(hist1->GetBinContent(i), parameters_[0]*hist2->GetBinContent(i), hist1->GetBinError(i), 0);
			result_to_minimize_ = result_to_minimize_ + tmp_result;
		}
	}
	
	n_params_i         = n_params_;  // really? 
	gin                = gin_;
	result_to_minimize = result_to_minimize_;
	parameters         = parameters_;
	ierflg             = ierflg_;
	
	this -> DumpToOutput();
	n_calls++;
	
	return 1;
}
*/

