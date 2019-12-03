// ==================================================================== //
// Code by Melissa Anholm
// June 2017 - 
// 
// asym_histset2.cpp gets some of the more experimental class things, 
// for Wtilde analysis.
//
// ==================================================================== //
#ifndef INCLUDE_ASYMHISTSET
#define INCLUDE_ASYMHISTSET 1


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
	oss << fixed << setprecision(thisprecision) << thisnumber;
	
	string mynumberstring = oss.str();
	return mynumberstring;
}
TH1D* makehist_zeroslike(TH1D* oldhist, string newname)
{
//	string newname = "tmpname";
//	int newcolor = kBlack;
	int newcolor = int(oldhist->GetLineColor());
//	TH1D * newhist = new TH1D(*oldhist);  // doesn't copy the list of functions.
	TH1D * newhist = (TH1D*)oldhist -> Clone(newname.c_str());
//	newhist = new TH1D(*newhist);
	
	// delete the list of functions...
	
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
bool HistsHaveSameBinning2(TH1D *a, TH1D *b, bool verbose=false) 
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

TH1D* make_th1_from_tf1(TF1* this_tf1, string hist_type, string new_hist_title, int this_color=int(kBlack))
{ // rescale so Abeta is 1 before calling this, maybe?
	double x_min = this_tf1->GetXmin();
	double x_max = this_tf1->GetXmax();
	int rebin_factor=1;
	TH1D * new_hist = CreateHist(new_hist_title, hist_type, this_color, rebin_factor);
	int n_bins = new_hist->GetNbinsX();
	
	for (int i=1; i<n_bins; i++)  // Bins i=0, i=n_bins are the underflow and overflow?
	{
		if( new_hist->GetBinCenter(i) - 0.5*(new_hist->GetBinWidth(i)) >= x_min 
		 && new_hist->GetBinCenter(i) + 0.5*(new_hist->GetBinWidth(i)) <= x_max )
		{
			new_hist -> SetBinContent(i, this_tf1->Eval(new_hist->GetBinCenter(i)) );
		}
		else
		{
			new_hist -> SetBinContent(i, 0.0 );
		}
	}
//	new_hist->GetListOfFunctions()->Add(this_tf1);
	return new_hist;
}

TH1D * evaluate_gaussian(string hist_type, double center, double lambda, double counts_total)
{
	TH1D* gaussian_hist = new TH1D();
	TH1D * tmp_hist = CreateHist("tmp", hist_type, int(kBlack), 1);
	if(counts_total > 0)
	{
		double sigma = sqrt(lambda*center);
		double area = counts_total*tmp_hist->GetBinWidth(1);
	
		TF1* gaussian_func = make_gaussian_func(center, sigma, area);
		gaussian_hist = make_th1_from_tf1(gaussian_func, hist_type, "tmp_gaussian");
		double scale_factor = counts_total/gaussian_hist->Integral();
		gaussian_hist -> Scale( scale_factor );
	}
	else
	{
		gaussian_hist = makehist_zeroslike(tmp_hist, "tmp_gaussian");
	}
	tmp_hist -> Delete();
	
	return gaussian_hist;
}


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //


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
	asym_histset(TH1D* this_htp, TH1D* this_htm, TH1D* this_hbp, TH1D* this_hbm);  // make it from data (or wev) hists.
	
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
asym_histset::asym_histset(TH1D* this_htp, TH1D* this_htm, TH1D* this_hbp, TH1D* this_hbm)
{
	rho = 0.0;
	if( !this_htp || !this_htm || !this_hbp || !this_hbm)
	{
		cout << "Nope.  asym_histset creation failed." << endl;
		return;
	}
	init_counthists(this_htp, this_htm, this_hbp, this_hbm);  // also sets h_tp, h_tm, h_bp, h_bm.
	// ok, but now create h_asym and h_counts from this?
	
	// create h_asym, h_superratio, h_supersum, ...
	h_asym   = make_asymmetry_histogram (this_htp, this_htm, this_hbp, this_hbm);
	h_counts = make_asymcounts_histogram(this_htp, this_htm, this_hbp, this_hbm);
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
//	h_tp -> Sumw2();
//	h_tm -> Sumw2();
//	h_bp -> Sumw2();
//	h_bm -> Sumw2();
	
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
	
	
	if( h_tp ) { h_tp->SetMarkerColor(newcolor);  h_tp->SetLineColor(newcolor); }
	if( h_tm ) { h_tm->SetMarkerColor(newcolor);  h_tm->SetLineColor(newcolor); }
	if( h_bp ) { h_bp->SetMarkerColor(newcolor);  h_bp->SetLineColor(newcolor); }
	if( h_bm ) { h_bm->SetMarkerColor(newcolor);  h_bm->SetLineColor(newcolor); }
}
void asym_histset::set_range_user(double lowlimit, double highlimit)
{
	h_asym   -> GetXaxis() -> SetRangeUser(lowlimit, highlimit);
	h_counts -> GetXaxis() -> SetRangeUser(lowlimit, highlimit);
	
	if( h_tp ) { h_tp->GetXaxis() -> SetRangeUser(lowlimit, highlimit); }
	if( h_tm ) { h_tm->GetXaxis() -> SetRangeUser(lowlimit, highlimit); }
	if( h_bp ) { h_bp->GetXaxis() -> SetRangeUser(lowlimit, highlimit); }
	if( h_bm ) { h_bm->GetXaxis() -> SetRangeUser(lowlimit, highlimit); }
}
void asym_histset::set_xaxis_name(string newname)
{
	h_asym   -> GetXaxis() -> SetName(newname.c_str());
	h_counts -> GetXaxis() -> SetName(newname.c_str());
	
	if( h_tp ) { h_tp->GetXaxis() -> SetName(newname.c_str()); }
	if( h_tm ) { h_tm->GetXaxis() -> SetName(newname.c_str()); }
	if( h_bp ) { h_bp->GetXaxis() -> SetName(newname.c_str()); }
	if( h_bm ) { h_bm->GetXaxis() -> SetName(newname.c_str()); }
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
	
	// bool asym_histset::init_counthists(string filename, string name_tp, string name_tm, string name_bp, string name_bm)
	pair_benB->init_counthists(ben_fname_B, 
		name_tp_default, name_tm_default, name_bp_default, name_bm_default);
	if(pair_benB->has_counthists && verbose) { cout << "Successfully initialized counthists for Set B." << endl; }
	else if (verbose) { cout << "Could not initialize counthists for Set B." << endl; }
	
	if(pair_benB->has_counthists)
	{
		pair_benB->h_tp -> SetName( (string("Ben Set B:  ")+string(pair_benB->h_tp->GetName())).c_str() );
		pair_benB->h_tm -> SetName( (string("Ben Set B:  ")+string(pair_benB->h_tm->GetName())).c_str() );
		pair_benB->h_bp -> SetName( (string("Ben Set B:  ")+string(pair_benB->h_bp->GetName())).c_str() );
		pair_benB->h_bm -> SetName( (string("Ben Set B:  ")+string(pair_benB->h_bm->GetName())).c_str() );
	}
	
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

	if(pair_benC->has_counthists)
	{
		pair_benC->h_tp -> SetName( (string("Ben Set C:  ")+string(pair_benC->h_tp->GetName())).c_str() );
		pair_benC->h_tm -> SetName( (string("Ben Set C:  ")+string(pair_benC->h_tm->GetName())).c_str() );
		pair_benC->h_bp -> SetName( (string("Ben Set C:  ")+string(pair_benC->h_bp->GetName())).c_str() );
		pair_benC->h_bm -> SetName( (string("Ben Set C:  ")+string(pair_benC->h_bm->GetName())).c_str() );
	}

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

	if(pair_benD->has_counthists)
	{
		pair_benD->h_tp -> SetName( (string("Ben Set D:  ")+string(pair_benD->h_tp->GetName())).c_str() );
		pair_benD->h_tm -> SetName( (string("Ben Set D:  ")+string(pair_benD->h_tm->GetName())).c_str() );
		pair_benD->h_bp -> SetName( (string("Ben Set D:  ")+string(pair_benD->h_bp->GetName())).c_str() );
		pair_benD->h_bm -> SetName( (string("Ben Set D:  ")+string(pair_benD->h_bm->GetName())).c_str() );
	}

	return pair_benD;
}
asym_histset * get_pair_ben_all()
{
	asym_histset * pair_all = get_pair_ben_B();
//	asym_histset * pairB = get_pair_ben_B();

	asym_histset * pairC = get_pair_ben_C();
	pair_all->h_tp -> Add( pairC->h_tp );
	pair_all->h_tm -> Add( pairC->h_tm );
	pair_all->h_bp -> Add( pairC->h_bp );
	pair_all->h_bm -> Add( pairC->h_bm );

	asym_histset * pairD = get_pair_ben_D();
	pair_all->h_tp -> Add( pairD->h_tp );
	pair_all->h_tm -> Add( pairD->h_tm );
	pair_all->h_bp -> Add( pairD->h_bp );
	pair_all->h_bm -> Add( pairD->h_bm );
	
	pair_all -> set_name("Sets BCD" );
	pair_all->h_tp -> SetName( (string("Sets BCD:  ")+string(pair_all->h_tp->GetName())).c_str() );
	pair_all->h_tm -> SetName( (string("Sets BCD:  ")+string(pair_all->h_tm->GetName())).c_str() );
	pair_all->h_bp -> SetName( (string("Sets BCD:  ")+string(pair_all->h_bp->GetName())).c_str() );
	pair_all->h_bm -> SetName( (string("Sets BCD:  ")+string(pair_all->h_bm->GetName())).c_str() );

	pair_all->h_counts->Add( pairC->h_counts);
	pair_all->h_counts->Add( pairD->h_counts);
//	pair_all->h_counts->Set
//	asym_histset::init_counthists(TH1D* h_tp_, TH1D* h_tm_, TH1D* h_bp_, TH1D* h_bm_)

	
	// ok, now construct the asymmetry...
	
	TH1D * h = make_asymmetry_histogram(pair_all->h_tp, pair_all->h_tm, pair_all->h_bp, pair_all->h_bm, string("Sets BCD:  A_beta"));
	pair_all->h_asym = h;
	
	return pair_all;
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
	
	bool set_hists(TH1D*, TH1D*, TH1D*);
	TH1D* Wtilde_1_hist;
	TH1D* Wtilde_A_hist;
	TH1D* Wtilde_b_hist;
	bool has_wtilde_hists;
	
	void set_physics_params(double par_one=1.0, double par_A=-0.5723, double par_b=0.0);
	double parsize_one;
	double parsize_A;
	double parsize_b;

	bool setup_counts(double n_total_counts, double, double, double, double);
	double N_total_counts;
	double N_tp;
	double N_tm;
	double N_bp;
	double N_bm;
	void setup_counts_like_B();
	void setup_counts_like_C();
	void setup_counts_like_D();
	void setup_counts_like_all();
	void scale_stats(double n_total_counts);
	void set_use_errs(bool this_use_errs) { use_errs = this_use_errs; };
	
	TH1D* make_all_Ehists();
	TH1D* make_all_Ehists(double, double, double);
//	TH1D* assemble_AsymmetrySuperratio();
	
	void set_efficiency_by_N();
	double top_efficiency;
	double bottom_efficiency;
	void rescale_by_efficiency();
	
	double N_plus_rel;
	double N_minus_rel;
	double pol_plus;
	double pol_minus;
	double avg_costheta;
	
	void rescale_by_stats();
//	void rescale_empirical();
//	void rescale_all(double n_total_counts);
	
//	void rescale_statistics_by_fraction(double scale_factor);
//	void rescale_theoretical_fromraw();
//	void rescale_statistics_by_empirical();
	
	void setup_convolution_params(double, double, double);
	double lambda_top;
	double lambda_bottom;
	double fraction_flat;
	
	void set_do_convolution(bool doitornot) { do_numerical_convolution=doitornot; }
	void convolute_hists();
	void do_the_flat_bit();
	void do_the_gaussian_bit();
	
	void setup_plotstyle();
	
private:
	bool do_numerical_convolution;
	bool use_errs;
	TH1D* make_an_Ehist_more(double, double, double, string);
	TH1D* make_an_Ehist_less(double, double, double, string);
};

created_histset::created_histset()
{
	TH1D* w1 = new TH1D();
	TH1D* wA = new TH1D();
	TH1D* wb = new TH1D();
	created_histset(w1, wA, wb);
	has_wtilde_hists = false;
}
created_histset::created_histset(TH1D* w1, TH1D* wA, TH1D* wb)
{
	asym_histset();
	set_hists(w1, wA, wb);
	set_physics_params();
	setup_convolution_params(0.10, 1.4, 1.4);

	use_errs = true;
	do_numerical_convolution = false;
//	double lambda_top    = 0.0;
//	double lambda_bottom = 0.0;
//	double fraction_flat = 0.0;
	
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
bool created_histset::set_hists(TH1D* w1, TH1D* wA, TH1D* wb)
{
	Wtilde_1_hist = w1;
	Wtilde_A_hist = wA;
	Wtilde_b_hist = wb;
	has_wtilde_hists = Wtilde_1_hist && Wtilde_A_hist && Wtilde_b_hist; 
	bool same_binning = false;
	
	same_binning = HistsHaveSameBinning2(Wtilde_1_hist, Wtilde_A_hist) 
		&& HistsHaveSameBinning2(Wtilde_1_hist, Wtilde_b_hist);
	
//	cout << "I just checked the binning..." << endl;
	if (!same_binning)
	{
		cout << "You fail at assigning histograms to this class." << endl;
	}
	// re-rescale things now?
	
	return same_binning;
}
void created_histset::set_physics_params(double par_one, double par_A, double par_b)
{
	parsize_one = par_one;
	parsize_A   = par_A;
	parsize_b   = par_b;
}
bool created_histset::setup_counts(double n_total_counts, double n_tp=0, double n_tm=0, double n_bp=0, double n_bm=0)
{
	N_total_counts = n_total_counts;
	N_tp = n_tp;
	N_tm = n_tm;
	N_bp = n_bp;
	N_bm = n_bm;
	
	bool setup_good = true;
	if( N_total_counts != N_tp + N_tm + N_bp + N_bm )
	{
		cout << "ERROR!  Mismatched counts!" << endl;
		cout << "N_total_counts = " << N_total_counts << endl;
		cout << "N_tp + N_tm + N_bp + N_bm = " << N_tp + N_tm + N_bp + N_bm << endl;
		setup_good = false;
	}
	
	set_efficiency_by_N();
	return setup_good;
}
void created_histset::setup_counts_like_B()
{
	double n_total_counts = 196395;
	double n_tp = 21843;
	double n_tm = 72759;
	double n_bp = 78222;
	double n_bm = 23571;
	setup_counts(n_total_counts, n_tp, n_tm, n_bp, n_bm);
	
	lambda_top    = 1.55;
	lambda_bottom = 1.28;
}
void created_histset::setup_counts_like_C()
{
	double n_total_counts = 20177;
	double n_tp = 2208;
	double n_tm = 7426;
	double n_bp = 8183;
	double n_bm = 2360;
	setup_counts(n_total_counts, n_tp, n_tm, n_bp, n_bm);
	
	lambda_top    = 1.42;
	lambda_bottom = 1.32;
}
void created_histset::setup_counts_like_D()
{
	double n_total_counts = 232061;
	double n_tp = 25493;
	double n_tm = 85603;
	double n_bp = 92961;
	double n_bm = 28004;
	setup_counts(n_total_counts, n_tp, n_tm, n_bp, n_bm);
	
	lambda_top    = 1.42;
	lambda_bottom = 1.32;
}

void created_histset::setup_counts_like_all()  // APPROXIMATE SCALING ONLY!
{
	double n_total_counts = 196395+20177+232061;
	double n_tp = 21843+2208+25493;
	double n_tm = 72759+7426+85603;
	double n_bp = 78222+8183+92961;
	double n_bm = 23571+2360+28004;
	setup_counts(n_total_counts, n_tp, n_tm, n_bp, n_bm);
	
	lambda_top    = (1.55+1.42)/2.0;
	lambda_bottom = (1.28+1.32)/2.0;
}



void created_histset::setup_convolution_params(double flatfrac, double lambda_t, double lambda_b)
{
	fraction_flat = flatfrac;
	lambda_top    = lambda_t;
	lambda_bottom = lambda_b;
}

void created_histset::set_efficiency_by_N()
{
	if(N_tp==0 || N_tm==0 || N_bp==0 || N_bm==0)
	{
		cout << "Nope.  Can't set the efficiencies." << endl;
		return;
	}
	double n_top    = N_tp+N_tm;
	double n_bottom = N_bp+N_bm;
	
	top_efficiency = 1.0;
	bottom_efficiency = n_bottom/n_top;
}

void created_histset::scale_stats(double n_total_counts)
{
	// originally:
	// B:  196395
	// C:   20177
	// D:  232061
	double scale_factor = n_total_counts/N_total_counts;
	
	N_total_counts = N_total_counts*scale_factor;
	N_tp = N_tp * scale_factor;
	N_tm = N_tm * scale_factor;
	N_bp = N_bp * scale_factor;
	N_bm = N_bm * scale_factor;
}

TH1D* created_histset::make_an_Ehist_more(double par_one, double par_A, double par_b, string name="")
{
	TH1D* tmp_Ehist = makehist_zeroslike(Wtilde_1_hist, name);
	tmp_Ehist -> Add(Wtilde_1_hist, par_one);
	tmp_Ehist -> Add(Wtilde_A_hist, -1.0*par_A);
	tmp_Ehist -> Add(Wtilde_b_hist, par_b);
//	tmp_Ehist -> SetName(name.c_str());
//	tmp_Ehist -> SetTitle(name.c_str());

	return tmp_Ehist;
}
TH1D* created_histset::make_an_Ehist_less(double par_one, double par_A, double par_b, string name="")
{
	TH1D* tmp_Ehist = makehist_zeroslike(Wtilde_1_hist, name);
	tmp_Ehist -> Add(Wtilde_1_hist, par_one);
	tmp_Ehist -> Add(Wtilde_A_hist, 1.0*par_A);
	tmp_Ehist -> Add(Wtilde_b_hist, par_b);
//	tmp_Ehist -> SetName(name.c_str());
//	tmp_Ehist -> SetTitle(name.c_str());
	
	return tmp_Ehist;
}
TH1D* created_histset::make_all_Ehists()  // gets its binning from internal Wtilde_1_hist.
{
	if(h_tp==0 || h_tm==0 || h_bp==0 || h_bm==0)
	{
		cout << "Uh-oh..." << endl;
	}
	
	h_tp = make_an_Ehist_less(parsize_one, parsize_A*pol_plus *avg_costheta, parsize_b, "TP");
	h_tm = make_an_Ehist_more(parsize_one, parsize_A*pol_minus*avg_costheta, parsize_b, "TM");
	h_bp = make_an_Ehist_more(parsize_one, parsize_A*pol_plus *avg_costheta, parsize_b, "BP");
	h_bm = make_an_Ehist_less(parsize_one, parsize_A*pol_minus*avg_costheta, parsize_b, "BM");
//	rescale_empirical(); // everything is scaled correctly now, so h_asym and h_counts will be too.
	rescale_by_stats();
	rescale_by_efficiency();
	
	if(do_numerical_convolution)
	{
		convolute_hists();
	}
	
	if( !HistsHaveSameBinning2(h_tp, h_tm) || !HistsHaveSameBinning2(h_bp, h_bm) || !HistsHaveSameBinning2(h_tp, h_bp) )
	{
		cout << "Trying to create all the Ehists, but binning is all wrong." << endl;
	}
	
	h_asym = make_asymmetry_histogram(h_tp, h_tm, h_bp, h_bm, string("A_super"));
	h_counts = make_asymcounts_histogram(h_tp, h_tm, h_bp, h_bm, string("Counts for A_super"));

// TH1D * make_asymmetry_histogram(TH1D * r1p_hist, TH1D * r1m_hist, TH1D * r2p_hist, TH1D * r2m_hist, string hist_title = string("A_beta"), int color=int(kBlack), int plotmarkerstyle=20)
	
	h_asym->SetOption("E1L");
	h_asym->SetMarkerSize(0.5);
	if(!use_errs) { h_asym->Sumw2(false); }
//	else { h_asym->SetOption("E1L"); }
	
	return h_asym;
}
TH1D* created_histset::make_all_Ehists(double this_size_one, double this_size_A, double this_size_b)
{
	set_physics_params(this_size_one, this_size_A, this_size_b);
	make_all_Ehists();
	return h_asym;
}
/*
void created_histset::rescale_empirical() // turns out this function is a fucking disaster.
{
	if(N_tp!=0 && N_tm!=0 && N_bp!=0 && N_bm!=0)
	{
//		cout << "Warning:  an empirical rescale doesn't appropriately take into account ";
//		cout << "the difference between detector efficiency and trap location.  "; 
//		cout << "This will be implemented later.  Maybe." << endl;
		
		h_tp -> Scale( N_tp / (h_tp->Integral()) );
		h_tm -> Scale( N_tm / (h_tm->Integral()) );
		h_bp -> Scale( N_bp / (h_bp->Integral()) );
		h_bm -> Scale( N_bm / (h_bm->Integral()) );
	}
	else
	{
		cout << "ERROR!  Could not rescale histograms.  Everything will be wrong!" << endl;
	}
}
*/
void created_histset::rescale_by_stats()
{
	double N_total_counts_current = h_tp->Integral() + h_tm->Integral() + h_bp->Integral() + h_bm->Integral();
	double scale_factor = N_total_counts/N_total_counts_current;
	
	h_tp -> Scale(scale_factor);
	h_tm -> Scale(scale_factor);
	h_bp -> Scale(scale_factor);
	h_bm -> Scale(scale_factor);
}

void created_histset::rescale_by_efficiency()
{
	h_tp -> Scale( top_efficiency );
	h_tm -> Scale( top_efficiency );
	h_bp -> Scale( bottom_efficiency );
	h_bm -> Scale( bottom_efficiency );
}

void created_histset::convolute_hists()
{
	do_the_flat_bit();
	do_the_gaussian_bit();
}

TH1D* do_the_gaussian_bit_individual(TH1D* oldhist, double lambda)
{
	TH1D * this_hist = oldhist;
	int n_bins = oldhist -> GetNbinsX();
	const char * thename = oldhist -> GetName();
	TH1D* this_clone = makehist_zeroslike(oldhist, "tmp3");
	
//	TH1D * this_hist;
//	TH1D * this_clone;
	TH1D * res_gauss;
	double this_lambda;
	
	double old_bincount;
	double old_clonecontent;
	double new_bincount;
	double total_counts = 0;
	
//	this_hist  = oldhist;
//	this_clone = newhist;
	this_lambda = lambda;
	for (int i=1; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		res_gauss = evaluate_gaussian("Ben_Ebeta", this_hist->GetBinCenter(i), this_lambda, old_bincount);
		for (int j=1; j<n_bins; j++)
		{
			this_clone -> SetBinContent(j, this_clone->GetBinContent(j) + res_gauss->GetBinContent(j) );
		}
		res_gauss -> Delete();
	}
	
	oldhist = (TH1D*) this_clone -> Clone(thename);
	oldhist -> SetName(thename);
	oldhist -> SetTitle(thename);
	
	return oldhist;
}

void created_histset::do_the_gaussian_bit()
{
	h_tp = do_the_gaussian_bit_individual(h_tp, lambda_top);
	h_tm = do_the_gaussian_bit_individual(h_tm, lambda_top);
	h_bp = do_the_gaussian_bit_individual(h_bp, lambda_bottom);
	h_bm = do_the_gaussian_bit_individual(h_bm, lambda_bottom);
}


/*
void created_histset::do_the_gaussian_bit()
{
	int verbose = 0;
//	cout << "Called gaussian bit." << endl;
	
	if(h_tp==0 || h_tm==0 || h_bp==0 || h_bm==0) { cout << "Uh-oh..." << endl; return; }
	int n_bins = h_tp -> GetNbinsX();
	
	TH1D* h_tp_tmp = makehist_zeroslike(h_tp, "tmp2_tp");
	TH1D* h_tm_tmp = makehist_zeroslike(h_tm, "tmp2_tm");
	TH1D* h_bp_tmp = makehist_zeroslike(h_bp, "tmp2_bp");
	TH1D* h_bm_tmp = makehist_zeroslike(h_bm, "tmp2_bm");
	if(h_tp_tmp==0 || h_tm_tmp==0 || h_bp_tmp==0 || h_bm_tmp==0) { cout << "There are no clones!!" << endl; return; }
	
	TH1D * this_hist;
	TH1D * this_clone;
	TH1D * res_gauss;
	double this_lambda;
	
	double old_bincount;
	double old_clonecontent;
	double new_bincount;
	double total_counts = 0;
	//
	this_hist  = h_tp;
	this_clone = h_tp_tmp;
	this_lambda = lambda_top;
	for (int i=1; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		res_gauss = evaluate_gaussian("Ben_Ebeta", this_hist->GetBinCenter(i), this_lambda, old_bincount);
		for (int j=1; j<n_bins; j++)
		{
			this_clone -> SetBinContent(j, this_clone->GetBinContent(j) + res_gauss->GetBinContent(j) );
		}
		res_gauss -> Delete();
	}
	if(verbose>0)
	{
		total_counts = this_hist -> Integral();
		cout << "TP:  original:  N=" << total_counts << ",\tafterward:  N=" << this_clone -> Integral();// << endl;
		cout << "diff = " << total_counts - this_clone -> Integral() << endl;
	}
	this_hist  = h_tm;
	this_clone = h_tm_tmp;
	this_lambda = lambda_top;
//	total_counts = this_hist -> Integral();
	for (int i=1; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		res_gauss = evaluate_gaussian("Ben_Ebeta", this_hist->GetBinCenter(i), this_lambda, old_bincount);
		for (int j=1; j<n_bins; j++)
		{
			this_clone -> SetBinContent(j, this_clone->GetBinContent(j) + res_gauss->GetBinContent(j) );
		}
		res_gauss -> Delete();
	}
	if(verbose>0)
	{
		total_counts = this_hist -> Integral();
		cout << "TM:  original:  N=" << total_counts << ",\tafterward:  N=" << this_clone -> Integral();// << endl;
		cout << "diff = " << total_counts - this_clone -> Integral() << endl;
	}

	this_hist  = h_bp;
	this_clone = h_bp_tmp;
	this_lambda = lambda_bottom;
//	total_counts = this_hist -> Integral();
	for (int i=1; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		res_gauss = evaluate_gaussian("Ben_Ebeta", this_hist->GetBinCenter(i), this_lambda, old_bincount);
		for (int j=1; j<n_bins; j++)
		{
			this_clone -> SetBinContent(j, this_clone->GetBinContent(j) + res_gauss->GetBinContent(j) );
		}
		res_gauss -> Delete();
	}
	if(verbose>0)
	{
		total_counts = this_hist -> Integral();
		cout << "BP:  original:  N=" << total_counts << ",\tafterward:  N=" << this_clone -> Integral();// << endl;
		cout << "diff = " << total_counts - this_clone -> Integral() << endl;
	}
	
	this_hist  = h_bm;
	this_clone = h_bm_tmp;
	this_lambda = lambda_bottom;
//	total_counts = this_hist -> Integral();
	for (int i=1; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		res_gauss = evaluate_gaussian("Ben_Ebeta", this_hist->GetBinCenter(i), this_lambda, old_bincount);
		for (int j=1; j<n_bins; j++)
		{
			this_clone -> SetBinContent(j, this_clone->GetBinContent(j) + res_gauss->GetBinContent(j) );
		}
		res_gauss -> Delete();
	}
	if(verbose>0)
	{
		total_counts = this_hist -> Integral();
		cout << "BM:  original:  N=" << total_counts << ",\tafterward:  N=" << this_clone -> Integral();// << endl;
		cout << "diff = " << total_counts - this_clone -> Integral() << endl;
	}
	
	
	h_tp = (TH1D*)h_tp_tmp -> Clone();
	h_tm = (TH1D*)h_tm_tmp -> Clone();
	h_bp = (TH1D*)h_bp_tmp -> Clone();
	h_bm = (TH1D*)h_bm_tmp -> Clone();
	
	h_tp -> SetName("TP");
	h_tp -> SetTitle("TP");
	h_tm -> SetName("TM");
	h_tm -> SetTitle("TM");
	h_bp -> SetName("BP");
	h_bp -> SetTitle("BP");
	h_bm -> SetName("BM");
	h_bm -> SetTitle("BM");
}

*/

TH1D* do_the_flat_bit_individual(TH1D* oldhist, double flatfrac)
{
	TH1D * this_hist  = oldhist;
	const char * thename = oldhist -> GetName();
	int n_bins = oldhist -> GetNbinsX();
	TH1D * this_clone = makehist_zeroslike(oldhist, "tmp_4");

	double fraction_flat = flatfrac;
	double old_bincount;
	double new_bincount;
	double flatbit_for_each_bin;
//	double total_counts;
	double n_bins_to_use;
	for (int i=0; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		this_clone -> SetBinContent(i, old_bincount);
		if(i>=2)
		{
			n_bins_to_use = double(i) - 1.0;
			flatbit_for_each_bin = (fraction_flat*old_bincount)/n_bins_to_use;
			
			this_clone -> SetBinContent(i, (1.0-fraction_flat)*old_bincount );
			for(int j=i-1; j>0; j--)
			{
				this_clone -> SetBinContent(j, this_clone->GetBinContent(j)+flatbit_for_each_bin );
			}
		}
	}
	oldhist = (TH1D*) this_clone -> Clone(thename);
	oldhist -> SetName(thename);
	oldhist -> SetTitle(thename);
	
	return oldhist;
}

void created_histset::do_the_flat_bit()
{
	h_tp = do_the_flat_bit_individual(h_tp, fraction_flat);
	h_tm = do_the_flat_bit_individual(h_tm, fraction_flat);
	h_bp = do_the_flat_bit_individual(h_bp, fraction_flat);
	h_bm = do_the_flat_bit_individual(h_bm, fraction_flat);
}


/*
void created_histset::do_the_flat_bit()
{
	int verbose = 0;
//	fraction_flat = 0.10;
	
	TH1D * this_hist;
	TH1D * this_clone;

	int n_bins = h_tp -> GetNbinsX();
	if(h_tp==0 || h_tm==0 || h_bp==0 || h_bm==0)
	{
		cout << "Uh-oh..." << endl;
	}

	TH1D* h_tp_tmp = makehist_zeroslike(h_tp, "tmp_tp");
	TH1D* h_tm_tmp = makehist_zeroslike(h_tm, "tmp_tm");
	TH1D* h_bp_tmp = makehist_zeroslike(h_bp, "tmp_bp");
	TH1D* h_bm_tmp = makehist_zeroslike(h_bm, "tmp_bm");
	if(h_tp==0 || h_tm==0 || h_bp==0 || h_bm==0)
	{
		cout << "Hists went away.  :(" << endl;
	}
	if(h_tp_tmp==0 || h_tm_tmp==0 || h_bp_tmp==0 || h_bm_tmp==0)
	{
		cout << "There are no clones!!" << endl;
	}
	
	double old_bincount;
	double new_bincount;
	double flatbit_for_each_bin;
	double total_counts;
	double n_bins_to_use;
	// Flatbit first...

	this_hist  = h_tp;
	this_clone = h_tp_tmp;
	total_counts = this_hist -> Integral();
	for (int i=0; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		this_clone -> SetBinContent(i, old_bincount);
		if(i>=2)
		{
			n_bins_to_use = double(i) - 1.0;
			flatbit_for_each_bin = (fraction_flat*old_bincount)/n_bins_to_use;
			
			this_clone -> SetBinContent(i, (1.0-fraction_flat)*old_bincount );
			for(int j=i-1; j>0; j--)
			{
				this_clone -> SetBinContent(j, this_clone->GetBinContent(j)+flatbit_for_each_bin );
			}
		}
	}
	if(verbose>=1)
	{
		cout << "original:  N=" << total_counts << ",\tafterward:  N=" << this_clone -> Integral() << endl;
	}
	
	this_hist  = h_tm;
	this_clone = h_tm_tmp;
	total_counts = this_hist -> Integral();
	
	for (int i=0; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		this_clone -> SetBinContent(i, old_bincount);
		if(i>=2)
		{
			n_bins_to_use = double(i) - 1.0;
			flatbit_for_each_bin = (fraction_flat*old_bincount)/n_bins_to_use;
			this_clone -> SetBinContent(i, (1.0-fraction_flat)*old_bincount );
			
			for(int j=i-1; j>0; j--)
			{
			//	this_clone -> SetBinContent(i, this_clone->GetBinContent(j)+flatbit_for_each_bin );
				this_clone -> SetBinContent(j, this_clone->GetBinContent(j)+flatbit_for_each_bin );
			}
		}
	}
	if(verbose>=1)
	{
		cout << "original:  N=" << total_counts << ",\tafterward:  N=" << this_clone -> Integral() << endl;
	}
	
	this_hist  = h_bp;
	this_clone = h_bp_tmp;
	total_counts = this_hist -> Integral();
	for (int i=0; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		this_clone -> SetBinContent(i, old_bincount);
		if(i>=2)
		{
			n_bins_to_use = double(i) - 1.0;
			flatbit_for_each_bin = (fraction_flat*old_bincount)/n_bins_to_use;
			
			this_clone -> SetBinContent(i, (1.0-fraction_flat)*old_bincount );
			for(int j=i-1; j>0; j--)
			{
				this_clone -> SetBinContent(j, this_clone->GetBinContent(j)+flatbit_for_each_bin );
			}
		}
	}
	if(verbose>=1)
	{
		cout << "original:  N=" << total_counts << ",\tafterward:  N=" << this_clone -> Integral() << endl;
	}

	this_hist  = h_bm;
	this_clone = h_bm_tmp;
	total_counts = this_hist -> Integral();
	for (int i=0; i<n_bins; i++)
	{
		old_bincount = this_hist->GetBinContent(i);
		this_clone -> SetBinContent(i, old_bincount);
		if(i>=2)
		{
			n_bins_to_use = double(i) - 1.0;
			flatbit_for_each_bin = (fraction_flat*old_bincount)/n_bins_to_use;
			
			this_clone -> SetBinContent(i, (1.0-fraction_flat)*old_bincount );
			for(int j=i-1; j>0; j--)
			{
				this_clone -> SetBinContent(j, this_clone->GetBinContent(j)+flatbit_for_each_bin );
			}
		}
	}
	if(verbose>=1)
	{
		cout << "original:  N=" << total_counts << ",\tafterward:  N=" << this_clone -> Integral() << endl;
	}
	h_tp = (TH1D*)h_tp_tmp -> Clone();
	h_tm = (TH1D*)h_tm_tmp -> Clone();
	h_bp = (TH1D*)h_bp_tmp -> Clone();
	h_bm = (TH1D*)h_bm_tmp -> Clone();
	
	h_tp -> SetName("TP");
	h_tp -> SetTitle("TP");
	h_tm -> SetName("TM");
	h_tm -> SetTitle("TM");
	h_bp -> SetName("BP");
	h_bp -> SetTitle("BP");
	h_bm -> SetName("BM");
	h_bm -> SetTitle("BM");
}
*/

void created_histset::setup_plotstyle()
{
	h_tp -> SetName("TP");
	h_tp -> SetTitle("TP");
	h_tp -> SetLineColor(kGreen);
	h_tp -> SetMarkerColor(kGreen);
	
	h_tm -> SetName("TM");
	h_tm -> SetTitle("TM");
	h_tm -> SetLineColor(mOrange);
	h_tm -> SetMarkerColor(mOrange);
	
	h_bp -> SetName("BP");
	h_bp -> SetTitle("BP");
	h_bp -> SetLineColor(kBlue);
	h_bp -> SetMarkerColor(kBlue);

	h_bm -> SetName("BM");
	h_bm -> SetTitle("BM");
	h_bm -> SetLineColor(kMagenta);
	h_bm -> SetMarkerColor(kMagenta);
	
	h_asym -> SetLineColor(kAzure);
	h_asym -> SetMarkerColor(kAzure);
	h_asym -> SetMarkerStyle(24);
}


/*
void created_histset::scale_hists(double n_total_counts)
{ 
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
*/

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

/*
void created_histset::rescale_statistics_by_empirical()
{ 
	// Use this one if you've already set up all the N_countses for the 4 sub-hists.
	h_tp -> Scale( N_tp/(h_tp->Integral()) );
	h_tm -> Scale( N_tm/(h_tm->Integral()) );
	h_bp -> Scale( N_bp/(h_bp->Integral()) );
	h_bm -> Scale( N_bm/(h_bm->Integral()) );
}
*/
/*
void created_histset::rescale_statistics_by_fraction(double scale_factor)
{ 
	double n_total_counts = scale_factor*N_total_counts;
	rescale_statistics_by_N(n_total_counts);
}
*/

/*
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
*/

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#endif
