// ==================================================================== //
// Code by Melissa Anholm
// 28.9.2017 - 3.7.2018
// 
// ==================================================================== //

#ifndef INCLUDE_SUPERMINUIT
#define INCLUDE_SUPERMINUIT 1


// Standard includes:
#include <iostream>  // cout, endl;
#include <string>
#include <vector>
#include <fstream>   // I think this probably helps...

using std::cout;
using std::endl;
using std::string;
using std::vector;

// ROOT includes:
#include <TH1.h>
#include <TMinuit.h>

// Mine:
#include "MinuitWrapperWrapper_fitparam.h"
//#include "HistFit.cpp"
//#include "SomeFunctions.cpp"
//#include "SomeFunctions.h"
//#include "asym_histset2.cpp"


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


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //

class superasym_histfitter
{
public:
	superasym_histfitter(created_histset* this_chs);
	created_histset * chs;
	bool SetupTheFitter_Asym();
};

superasym_histfitter::superasym_histfitter(created_histset* this_chs)
{
	chs = this_chs;
}

bool superasym_histfitter::SetupTheFitter_Asym()
{
	return true;
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
	bool SetupTheFitter_LinearCombo();
	bool SetupTheFitter_Asym();
	TH1D * assemble_new_histogram();
	
	int get_paramnumber_by_name(string thename);
	
	int get_n_params() { return n_params; }
	TH1D* get_FitHist() { return FitHist; }
	vector<TH1D*> histvect;
	vector<FitParameter> paramvect;
private:
	int n_params;
	TH1D * tmp_hist;
	TH1D * FitHist;
};

// --- * --- // 
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

/*
TH1D* combo_histfitter::adjust_histogram()
{
	for(int i=0; i<n_params; i++)
	{
		tmp_hist -> Add( histvect.at(i), paramvect.at(i).fit_val );
	}
	return tmp_hist;
}
*/

// SetupTheFitter_LinearCombo() is called by SuperMinuit's SetupFCN_histfitter(...) method.
// SetupTheFitter_LinearCombo() clears out the SuperMinuit.
// Call SetupTheFitter_LinearCombo() when hists and params are all loaded up and you're about to fit.
bool combo_histfitter::SetupTheFitter_LinearCombo() // returns true if hists all have the same binning.
{
	bool samebinning = true;
	for(int i=0; i<n_params; i++)
	{
		samebinning = samebinning && HistsHaveSameBinning(FitHist, histvect.at(i), false);
	}
	if( !samebinning ) 
	{ 
		cout << "ERROR!  Hists don't have the same binning!  Fitter is not set up." << endl;
		return samebinning; 
	}
//	tmp_hist = makehist_zeroslike(FitHist); // must set up the temp histogram.
	assemble_new_histogram();  // do I need to do this here??
	/*
	global_minuit = new SuperMinuit();
	for(int i=0; i<n_params; i++)
	{
//		this -> SuperMinuit::SetupParam(i, paramvect.at(i) );
		global_minuit -> SuperMinuit::SetupParam(i, paramvect.at(i) );
	}
	*/
//	global_minuit -> chf = this;
//	this -> TMinuit::SetFCN( NonMemberFitFunction ); // this line only works if it's a *static* void...
	
	return true;
}

bool combo_histfitter::SetupTheFitter_Asym() // returns true if hists all have the same binning.
{
	bool samebinning = true;
	for(int i=0; i<n_params; i++)
	{
		samebinning = samebinning && HistsHaveSameBinning(FitHist, histvect.at(i), false);
	}
	if( !samebinning ) 
	{ 
		cout << "ERROR!  Hists don't have the same binning!  Fitter is not set up." << endl;
		return samebinning; 
	}
//	assemble_new_histogram();  // do I need to do this here??
	// assemble_new_histogram() assumes a lin. combo.  make a new function to do the asymhists.
	return true;
}


int combo_histfitter::get_paramnumber_by_name(string thename)
{
	int the_paramnumber = -1;
	for(int i=0; i<n_params; i++)
	{
		if( paramvect.at(i).is_named(thename) )
		{
			the_paramnumber = i;
			break;
		}
	}
	
	return the_paramnumber;
}
// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //


#endif