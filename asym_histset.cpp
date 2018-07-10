// ==================================================================== //
// Code by Melissa Anholm
// June 2017 - 
// 
// consolidate_and_fit.cpp inherits from consolidate_results3.cpp
// LineshapeConvolution.h inherits from RhoLibs.h
// ==================================================================== //
#include <stdlib.h>
#include <string>
#include <sstream> 
//#include <iostream>
//#include <iomanip>   // do I need this?
//#include <algorithm> // to manipulate strings.
//#include <utility>

#include <TObject.h>
#include <TFile.h>
#include <TH1D.h>
#include "TF1.h"
#include <TStyle.h>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

#include "HistExtras.cpp"         // in AsymmetryCanvasLibs
#include "MetaChain.cpp"
#include "SomeFunctions.cpp"

string asymhist_file_path = "~/Desktop/Anal-Ysis/Abeta2/Files/";
string ben_fname_B        = "Abeta_bencompare_B.root";
string ben_fname_C        = "Abeta_bencompare_C.root";
string ben_fname_D        = "Abeta_bencompare_D.root";
string name_tp_default    = "Scint Energy (top&&bb1, sigma+)";
string name_tm_default    = "Scint Energy (top&&bb1, sigma-)";
string name_bp_default    = "Scint Energy (bottom&&bb1, sigma+)";
string name_bm_default    = "Scint Energy (bottom&&bb1, sigma-)";

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

class asym_histset : public TObject
{
public:
	double rho;
	string descriptor;
	TH1D* h_asym;
	TH1D* h_counts;
	
	void init_empty_counthists();
	bool init_actual_counthists(string filename, string name_tp, string name_tm, string name_bp, string name_bm);
	bool has_counthists;
	TH1D* h_tp;
	TH1D* h_tm;
	TH1D* h_bp;
	TH1D* h_bm;
	
	asym_histset();
	asym_histset(TH1D* this_h_asym, TH1D* this_h_counts, double);
	asym_histset(string filename, string hname_asym, string hname_counts, double);

	void set_name(string);
	void set_color(int);
	void set_range_user(double, double);
	void set_xaxis_name(string);
	void set_rho(double);
};

asym_histset::asym_histset()
{
	rho = 0.0;
	h_asym   = new TH1D();
	h_counts = new TH1D();
	
	init_empty_counthists();
}
asym_histset::asym_histset(TH1D* this_h_asym, TH1D* this_h_counts, double this_rho=0)
{
	rho = this_rho;
	h_asym   = new TH1D();
	h_counts = new TH1D();
	
	h_asym = (TH1D*)this_h_asym -> Clone();
	h_counts = (TH1D*)this_h_counts -> Clone();
	
	init_empty_counthists();
}
asym_histset::asym_histset(string filename, string hname_asym, string hname_counts, double this_rho=0)
{
	cout << "Looking for a file at:  " << asymhist_file_path+filename << endl;
	rho = this_rho;
	TFile * file = new TFile( (asymhist_file_path+filename).c_str() );
	
	h_asym   = (TH1D*)file->Get( hname_asym.c_str());    // 
	h_counts = (TH1D*)file->Get( hname_counts.c_str());  // 
//	if(h_asym && h_counts) {cout << "Found 2 hists!" << endl;}
//	else {cout << "couldn't find those 2 hists." << endl; }
//	file -> Close();  // if I close it, it breaks.  :(
	init_empty_counthists();
}
void asym_histset::init_empty_counthists()
{
	has_counthists = false;
	h_tp = new TH1D();
	h_tm = new TH1D();
	h_bp = new TH1D();
	h_bm = new TH1D();
//	return has_counthists;
}
bool asym_histset::init_actual_counthists(string filename, string name_tp, string name_tm, string name_bp, string name_bm)
{
	TFile * file = new TFile( (asymhist_file_path+filename).c_str() );
	
	has_counthists = true;
	h_tp = (TH1D*)file->Get( name_tp.c_str());
	h_tm = (TH1D*)file->Get( name_tm.c_str());
	h_bp = (TH1D*)file->Get( name_bp.c_str());
	h_bm = (TH1D*)file->Get( name_bm.c_str());
//	file -> Close();  // this crashes it.  For some reason.
	
	has_counthists = h_tp && h_tm  && h_bp && h_bm;
	if( has_counthists == false)
	{
		cout << "ERROR!  Couldn't find all the counthists in file: ";
		cout << asymhist_file_path+filename << endl;
		init_empty_counthists();
	}
	return has_counthists;
}


void asym_histset::set_name(string newname)
{
	descriptor = newname;
	h_asym -> SetTitle(newname.c_str() );
	h_asym -> SetName( newname.c_str() );

	h_counts -> SetTitle( (string("N_Events for ") + newname).c_str() );
	h_counts -> SetName(  (string("N_Events for ") + newname).c_str() );
}
void asym_histset::set_color(int newcolor)
{
	h_asym -> SetMarkerColor(newcolor);
	h_asym -> SetLineColor(newcolor);
	
	h_counts -> SetMarkerColor(newcolor);
	h_counts -> SetLineColor(newcolor);
}
void asym_histset::set_range_user(double lowlimit, double highlimit)
{
	h_asym   -> GetXaxis() -> SetRangeUser(lowlimit, highlimit);
	h_counts -> GetXaxis() -> SetRangeUser(lowlimit, highlimit);
}
void asym_histset::set_xaxis_name(string newname)
{
	h_asym   -> GetXaxis() -> SetName(newname.c_str());
	h_counts -> GetXaxis() -> SetName(newname.c_str());
}
string make_g4_asymhistpair_filename(double rho, string setletter)
{
	string rhostring = double_to_string(rho);
	rhostring.erase( std::remove(rhostring.begin(), rhostring.end(), '.'), rhostring.end() );
	
	stringstream ss;
	ss << "Abeta_g4_rho" << rhostring << setletter << ".root";
	string filename = ss.str();
	return filename;
}
string make_g4_asymhistpair_title(double rho, string setletter)
{
	string rhostring = double_to_string(rho);
	stringstream ss;
	ss << "G4 A_beta (rho=" << rhostring << ", Set " << setletter << ")";
	string title = ss.str();
	return title;
}


// --- // --- //

asym_histset * get_pair_ben_B()
{
	bool verbose = false;
	if(verbose) { cout << "Initializing set B." << endl; }
	
	string string_asym   = "Ben HEXpos recombination";
	string string_counts = "Ncounts- Ben HEXpos recombination";
	
	asym_histset * pair_benB  = new asym_histset(ben_fname_B, string_asym, string_counts);
	pair_benB -> set_name("Ben Set B" );
	pair_benB -> set_xaxis_name("Scintillator Energy [keV]");
	pair_benB -> set_range_user(0.0, 5100.0);
	pair_benB->h_asym -> GetYaxis() -> SetRangeUser(-0.63, -0.42);
	pair_benB->h_asym -> GetListOfFunctions() -> Delete();
	pair_benB -> set_color(kBlack);
	pair_benB->h_asym -> SetMarkerStyle(33);
	pair_benB->h_asym -> SetMarkerSize(1.5);
	pair_benB->h_asym -> SetLineWidth(1);
	
	pair_benB->init_actual_counthists(ben_fname_B, 
		name_tp_default, name_tm_default, name_bp_default, name_bm_default);
	if(pair_benB->has_counthists && verbose) { cout << "Successfully initialized counthists for Set B." << endl; }
	else if (verbose) { cout << "Could not initialize counthists for Set B." << endl; }

	return pair_benB;
}
asym_histset * get_pair_ben_C()
{
	bool verbose = false;
	if(verbose) { cout << "Initializing set C." << endl; }
	
	string string_asym   = "Ben HEXpos recombination";
	string string_counts = "Ncounts- Ben HEXpos recombination";
	
	asym_histset * pair_benC  = new asym_histset(ben_fname_C,  string_asym, string_counts);
	pair_benC -> set_name("Ben Set C" );
	pair_benC -> set_xaxis_name("Scintillator Energy [keV]");
	pair_benC -> set_range_user(0.0, 5100.0);
	pair_benC->h_asym -> GetYaxis() -> SetRangeUser(-0.63, -0.42);
	pair_benC->h_asym -> GetListOfFunctions() -> Delete();
	pair_benC -> set_color(kBlack);
	pair_benC->h_asym -> SetMarkerStyle(33);
	pair_benC->h_asym -> SetMarkerSize(1.5);
	pair_benC->h_asym -> SetLineWidth(1);
	
	pair_benC->init_actual_counthists(ben_fname_C, 
		name_tp_default, name_tm_default, name_bp_default, name_bm_default);
	if(pair_benC->has_counthists && verbose) { cout << "Successfully initialized counthists for Set C." << endl; }
	else if (verbose) { cout << "Could not initialize counthists for Set C." << endl; }

	return pair_benC;
}
asym_histset * get_pair_ben_D()
{
	bool verbose = false;
	if(verbose) { cout << "Initializing set D." << endl; }
	
	string string_asym   = "Ben HEXpos recombination";
	string string_counts = "Ncounts- Ben HEXpos recombination";
	
	asym_histset * pair_benD  = new asym_histset(ben_fname_D,  string_asym, string_counts);
	pair_benD -> set_name("Ben Set D" );
	pair_benD -> set_xaxis_name("Scintillator Energy [keV]");
	pair_benD -> set_range_user(0.0, 5100.0);
	pair_benD->h_asym -> GetYaxis() -> SetRangeUser(-0.63, -0.42);
	pair_benD->h_asym -> GetListOfFunctions() -> Delete();
	pair_benD -> set_color(kBlack);
	pair_benD->h_asym -> SetMarkerStyle(33);
	pair_benD->h_asym -> SetMarkerSize(1.5);
	pair_benD->h_asym -> SetLineWidth(1);
	
	pair_benD->init_actual_counthists(ben_fname_D, 
		name_tp_default, name_tm_default, name_bp_default, name_bm_default);
	if(pair_benD->has_counthists && verbose) { cout << "Successfully initialized counthists for Set D." << endl; }
	else if (verbose) { cout << "Could not initialize counthists for Set D." << endl; }
	return pair_benD;
}
asym_histset * get_g4_asymhist_pair(double rho, string setletter)
{
	string string_asym = "Super-er A_beta (from E_scint)";
	string string_counts = "N_Events for A_beta";
	string filename = make_g4_asymhistpair_filename(rho, setletter);
	asym_histset * this_pair = new asym_histset(filename, string_asym, string_counts, rho);
	string pair_title = make_g4_asymhistpair_title(rho, setletter);
	this_pair -> set_name( pair_title );

	return this_pair;
}


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
