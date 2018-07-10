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
// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //


#endif