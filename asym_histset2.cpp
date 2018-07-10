// ==================================================================== //
// Code by Melissa Anholm
// June 2017 - 
// 
// asym_histset2.cpp gets some of the more experimental class things, 
// for Wtilde analysis.
//
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
//#include "SomeFunctions.cpp"
#include "AsymmetryCanvasLibs.cpp"

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
string double_to_string(double thisnumber, int thisprecision=3)  // 
{
	std::ostringstream oss;
	oss << fixed << setprecision(3) << thisnumber;
	
	string mynumberstring = oss.str();
	return mynumberstring;
}
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


class asym_histset : public TObject
{
public:
	double rho;
	string descriptor;
	TH1D* h_asym;
	TH1D* h_counts;
	
	void init_empty_counthists();
	bool init_counthists(string filename, string name_tp, string name_tm, string name_bp, string name_bm);
	bool init_counthists(TH1D* h_tp_, TH1D* h_tm_, TH1D* h_bp_, TH1D* h_bm_);
	bool has_counthists;
	TH1D* h_tp;
	TH1D* h_tm;
	TH1D* h_bp;
	TH1D* h_bm;
	
	void set_htp(TH1D* hist) { h_tp = hist; };
	void set_htm(TH1D* hist) { h_tm = hist; };
	void set_hbp(TH1D* hist) { h_bp = hist; };
	void set_hbm(TH1D* hist) { h_bm = hist; };
	void set_hascounthists(bool hasthem) { has_counthists = hasthem; };
	
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
bool asym_histset::init_counthists(string filename, string name_tp, string name_tm, string name_bp, string name_bm)
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

bool asym_histset::init_counthists(TH1D* h_tp_, TH1D* h_tm_, TH1D* h_bp_, TH1D* h_bm_)
{
	has_counthists = true;
	h_tp = (TH1D*)h_tp_->Clone();
	h_tm = (TH1D*)h_tm_->Clone();
	h_bp = (TH1D*)h_bp_->Clone();
	h_bm = (TH1D*)h_bm_->Clone();
	
	has_counthists = h_tp && h_tm  && h_bp && h_bm;
	if( has_counthists == false)
	{
		cout << "ERROR!  Some of the counthists aren't real hists.";
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
	
	pair_benB->init_counthists(ben_fname_B, 
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
	
	pair_benC->init_counthists(ben_fname_C, 
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
	
	pair_benD->init_counthists(ben_fname_D, 
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

class created_histset : public asym_histset
{
public:
	created_histset();
	created_histset(TH1D*, TH1D*, TH1D*);
	
	TH1D* Wtilde_1_hist;
	TH1D* Wtilde_A_hist;
	TH1D* Wtilde_b_hist;
	
	TH1D* make_an_Ehist_more(double, double, double);
	TH1D* make_an_Ehist_less(double, double, double);
//	void make_all_the_Ehists();
	void make_raw_Ehists();
	void make_raw_Ehists(double, double, double);
	TH1D* assemble_AsymmetrySuperratio();
	
	double size_one;
	double size_A;
	double size_b;
	
	double top_efficiency;
	double bottom_efficiency;
	double N_plus_rel;
	double N_minus_rel;
	double pol_plus;
	double pol_minus;
	double avg_costheta;
	
	void setup_counts(double n_total_counts, double, double, double, double);
	double N_total_counts;
	double N_tp;
	double N_tm;
	double N_bp;
	double N_bm;
//	void rescale_theoretical_fromraw();
	void rescale_empirical_fromraw();
	void rescale_statistics_by_N(double n_total_counts);
	void rescale_statistics_by_fraction(double scale_factor);
	void rescale_statistics_by_empirical();

	void setup_counts_like_B();
	void setup_counts_like_C();
	void setup_counts_like_D();
	
};

created_histset::created_histset()
{
	asym_histset();
	
	Wtilde_1_hist = new TH1D();
	Wtilde_A_hist = new TH1D();
	Wtilde_b_hist = new TH1D();

	size_one = 1.0;
	size_A   = -0.5723;
	size_b   = 0.0;
	
	top_efficiency      = 1.0;
	bottom_efficiency   = 1.0;
	N_plus_rel          = 1.0;
	N_minus_rel         = 1.0;
	
	pol_plus       = 0.9913;    // +0.9913(6)
	pol_minus      = 0.9912;    // -0.9912(7)
	avg_costheta   = 0.994484;  // or less.
	
	N_total_counts = 0.0;
	N_tp = 0;
	N_tm = 0;
	N_bp = 0;
	N_bm = 0;
}

created_histset::created_histset(TH1D* w1, TH1D* wA, TH1D* wb)
{
	asym_histset();
	
	Wtilde_1_hist = w1;
	Wtilde_A_hist = wA;
	Wtilde_b_hist = wb;
	
	size_one = 1.0;
	size_A   = -0.5723;
	size_b   = 0.0;
	
	top_efficiency      = 1.0;
	bottom_efficiency   = 1.0;
	N_plus_rel          = 1.0;
	N_minus_rel         = 1.0;
	
	pol_plus       = 0.9913;    // +0.9913(6)
	pol_minus      = 0.9912;    // -0.9912(7)
	avg_costheta   = 0.994484;  // or less.
	
	N_total_counts = 0.0;
	N_tp = 0;
	N_tm = 0;
	N_bp = 0;
	N_bm = 0;
}

TH1D* created_histset::make_an_Ehist_more(double parsize_one=1.0, double parsize_A=-0.5723, double parsize_b=0.0)
{
	TH1D* tmp_Ehist = makehist_zeroslike(Wtilde_1_hist);
	tmp_Ehist -> Add(Wtilde_1_hist, parsize_one);
	tmp_Ehist -> Add(Wtilde_A_hist, 1.0*parsize_A);
	tmp_Ehist -> Add(Wtilde_b_hist, parsize_b);
	
	return tmp_Ehist;
}
TH1D* created_histset::make_an_Ehist_less(double parsize_one=1.0, double parsize_A=-0.5723, double parsize_b=0.0)
{
	TH1D* tmp_Ehist = makehist_zeroslike(Wtilde_1_hist);
	tmp_Ehist -> Add(Wtilde_1_hist, parsize_one);
	tmp_Ehist -> Add(Wtilde_A_hist, -1.0*parsize_A);
	tmp_Ehist -> Add(Wtilde_b_hist, parsize_b);
	
	return tmp_Ehist;
}

/*
void created_histset::make_all_the_Ehists()
{
	h_tp = make_an_Ehist_less(size_one, size_A*pol_plus*avg_costheta,  size_b);
//	h_tp -> Scale(top_efficiency*N_plus_rel);
	h_tm = make_an_Ehist_more(size_one, size_A*pol_minus*avg_costheta, size_b);
//	h_tm -> Scale(top_efficiency*N_minus_rel);
	
	h_bp = make_an_Ehist_more(size_one, size_A*pol_plus*avg_costheta,  size_b);
//	h_bp -> Scale(bottom_efficiency*N_plus_rel);
	h_bm = make_an_Ehist_less(size_one, size_A*pol_minus*avg_costheta, size_b);
//	h_bm -> Scale(bottom_efficiency*N_minus_rel);
	
	// rescale them all by N_total_counts.  but only if N!=0.
}
*/
void created_histset::make_raw_Ehists()
{
	h_tp = make_an_Ehist_less(size_one, size_A*pol_plus *avg_costheta, size_b);
	h_tm = make_an_Ehist_more(size_one, size_A*pol_minus*avg_costheta, size_b);
	h_bp = make_an_Ehist_more(size_one, size_A*pol_plus *avg_costheta, size_b);
	h_bm = make_an_Ehist_less(size_one, size_A*pol_minus*avg_costheta, size_b);
}

void created_histset::make_raw_Ehists(double this_size_one, double this_size_A, double this_size_b)
{
	size_one = this_size_one;
	size_A   = this_size_A;
	size_b   = this_size_b;
	make_raw_Ehists();
}

void created_histset::rescale_empirical_fromraw()
{
	if(N_tp!=0 && N_tm!=0 && N_bp!=0 && N_bm!=0)
	{
		cout << "Warning:  an empirical rescale doesn't appropriately take into account ";
		cout << "the difference between detector efficiency and trap location.  "; 
		cout << "This will be implemented later.  Maybe." << endl;
		
		h_tp -> Scale(N_tp);
		h_tm -> Scale(N_tm);
		h_bp -> Scale(N_bp);
		h_bm -> Scale(N_bm);
	}
	else
	{
		cout << "ERROR!  Could not rescale histograms.  Everything will be wrong!" << endl;
	}
}

/*
void created_histset::rescale_theoretical_fromraw()
{
	h_tp -> Scale(top_efficiency*N_plus_rel);
	h_tm -> Scale(top_efficiency*N_minus_rel);
	h_bp -> Scale(bottom_efficiency*N_plus_rel);
	h_bm -> Scale(bottom_efficiency*N_minus_rel);
	
	if(N_total_counts!=0)
	{
		double tmp_N = 0;
		tmp_N = h_tp->Integral() + h_tm->Integral() + h_bp->Integral() + h_bm->Integral();
		
		h_tp -> Scale(N_total_counts/tmp_N);
		h_tm -> Scale(N_total_counts/tmp_N);
		h_bp -> Scale(N_total_counts/tmp_N);
		h_bm -> Scale(N_total_counts/tmp_N);
	}
}
*/

void created_histset::rescale_statistics_by_N(double n_total_counts)
{ 
	// * if this is called before rescale_empirical_fromraw(), then 
	//   rescale_empirical_fromraw() still must be called afterwards.
	// * if this is called after rescale_empirical_fromraw(), then 
	//   all the histograms should come out scaled correctly.
	// **The important thing is that both must be called.
	double scale_factor = n_total_counts/N_total_counts;
	h_tp -> Scale(scale_factor);
	h_tm -> Scale(scale_factor);
	h_bp -> Scale(scale_factor);
	h_bm -> Scale(scale_factor);
	
	N_total_counts = n_total_counts;
	N_tp = N_tp*scale_factor;
	N_tm = N_tm*scale_factor;
	N_bp = N_bp*scale_factor;
	N_bm = N_bm*scale_factor;
}

void created_histset::rescale_statistics_by_empirical()
{ 
	// Use this one if you've already set up all the N_countses for the 4 sub-hists.
	h_tp -> Scale( N_tp/(h_tp->Integral()) );
	h_tm -> Scale( N_tm/(h_tm->Integral()) );
	h_bp -> Scale( N_bp/(h_bp->Integral()) );
	h_bm -> Scale( N_bm/(h_bm->Integral()) );
}

void created_histset::rescale_statistics_by_fraction(double scale_factor)
{ 
	double n_total_counts = scale_factor*N_total_counts;
	rescale_statistics_by_N(n_total_counts);
}

void created_histset::setup_counts(double n_total_counts, double n_tp=0, double n_tm=0, double n_bp=0, double n_bm=0)
{
	N_total_counts = n_total_counts;
	N_tp = n_tp;
	N_tm = n_tm;
	N_bp = n_bp;
	N_bm = n_bm;
}


void created_histset::setup_counts_like_B()
{
	double n_total_counts = 196395;
	double n_tp = 21843;
	double n_tm = 72759;
	double n_bp = 78222;
	double n_bm = 23571;
	setup_counts(n_total_counts, n_tp, n_tm, n_bp, n_bm);
}
void created_histset::setup_counts_like_C()
{
	double n_total_counts = 20177;
	double n_tp = 2208;
	double n_tm = 7426;
	double n_bp = 8183;
	double n_bm = 2360;
	setup_counts(n_total_counts, n_tp, n_tm, n_bp, n_bm);
}
void created_histset::setup_counts_like_D()
{
	double n_total_counts = 232061;
	double n_tp = 25493;
	double n_tm = 85603;
	double n_bp = 92961;
	double n_bm = 28004;
	setup_counts(n_total_counts, n_tp, n_tm, n_bp, n_bm);
}

TH1D* created_histset::assemble_AsymmetrySuperratio()
{
	// they must all be correctly scaled, already.
	h_asym = make_asymmetry_histogram(h_tp, h_tm, h_bp, h_bm, string("A_super"), int(kBlack));
	h_counts = make_asymcounts_histogram(h_tp, h_tm, h_bp, h_bm, string("Counts for A_super"), int(kGray));
	
	return h_asym;
}

TH1D* assemble_AsymmetrySuperratio(created_histset * chf)
{
	TH1D* this_h_asym = make_asymmetry_histogram(chf->h_tp, chf->h_tm, chf->h_bp, chf->h_bm, string("A_super"), int(kBlack));
	return this_h_asym;
}


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
