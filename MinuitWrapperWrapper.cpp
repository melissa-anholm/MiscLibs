// ==================================================================== //
// Code by Melissa Anholm
// 28.9.2017
// This one is *not* imported from hello_fit.cpp
// ==================================================================== //

// Standard includes:
#include <iostream>  // cout, endl;
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

// ROOT includes:
#include <TFile.h>
#include <TH1.h>
#include <TMinuit.h>
#include <TString.h>


struct FitParameter
{
public:
	FitParameter();
	FitParameter(TString name_, double start_, double step_, double min_, double max_, bool is_fixed_);
	
	double initial_val;
	double stepsize;
	double min_val;
	double max_val;
	bool is_fixed;
	
	bool fit_completed;
	double fit_val;
	double fit_err;
	
	Double_t xlo, xup; // ??
	Int_t iint;        // ??
	
	TString GetName() {return name; };
	void SetName(TString newname) { name = TString(newname); };
	TString name;
//private:
};

FitParameter::FitParameter()
{
	TString name_       = "";
	double initial_val_ = 0.0;
	double step_        = 0.1;
	double min_         = 0.0;
	double max_         = 0.0;
	bool is_fixed_      = false;
	FitParameter(name_, initial_val_, step_, min_, max_, is_fixed_);
}
FitParameter::FitParameter(TString name_, double initial_val_, double step_, double min_, double max_, bool is_fixed_=false)
{
	name         = name_;
	initial_val  = initial_val_;
	stepsize     = step_;
	min_val      = min_;
	max_val      = max_;
	is_fixed     = is_fixed_;
	
	fit_completed = false;
	fit_val = 0;
	fit_err = 0;
}


/*
// Fit function.
void GetChi2(Int_t &n_parameters, Double_t *gin, Double_t &result_to_minimize, Double_t *parameters, Int_t iflag) 
{
	double asym = parameters[0];
	double bg   = parameters[1];
	
	hist3 = (TH1D*)hist2->Clone("Current Fit");
	hist3 -> SetTitle("Current Fit");
	hist3 -> Scale(asym);
	
	double chi2 = 0.0;
	for (int i = bmin; i <= bmax; i++) 
	{
		if (i < bmin) 
		{
			continue;
		}
		if (i > bmax) 
		{
			continue;
		}
		
		double sim = hist3 -> GetBinContent(i);
		double exp = hist1 -> GetBinContent(i);
		double err = hist1 -> GetBinError(i);
		
		double t = exp - sim;
		if (err > 0.0) 
		{
			t = t / (err);
		}
		t = t * t;
		chi2 = chi2 + t;
	}

//	cout.flush();
	if (nfunc % 10 == 0) 
	{
		cout << "*  ";// << endl;
		cout << "nfunc=" << nfunc << "\t" << "chi2=" << chi2 << "\t";
		cout << "asym=" << asym << "\t" << "bg=" << bg << endl;
	}
	nfunc++;
	
	result_to_minimize = chi2;  // "result" is the thing that gets not-returned.
	
	result_to_minimize = 0.0;
}

*/

// A Class to deal with fitting histograms using TMinuit:
class histfitter
{
public:
	histfitter();
	histfitter(TH1D* hist1, TH1D* hist2);

	void update_h1(TH1D * new_h1) {h1 = new_h1;};
	void update_h2(TH1D * new_h2) {h2 = new_h2;};
	void update_h3(TH1D * new_h3) {h3 = new_h3;};
	
	TH1D* get_h1() {return h1; };
	TH1D* get_h2() {return h2; };
	TH1D* get_h3() {return h3; };

	TH1D* h1;
	TH1D* h2;
	TH1D* h3; // junk, just for the memory space.
	
	int CheckBinning();
	bool SetFitLimits_ByBin(int minbin, int maxbin);
	bool SetFitLimits_ByValue(double x_min, double x_max);

private:
	int bin_min;
	int bin_max;
//	TH1D* h1;
//	TH1D* h2;
//	TH1D* h3; // junk, just for the memory space.
};

histfitter::histfitter()
{ // hist1 = data, hist2 = sim.
	TH1D* hist1 = new TH1D();
	TH1D* hist2 = new TH1D();
	histfitter(hist1, hist2);
}
histfitter::histfitter(TH1D* hist1, TH1D* hist2)
{ // hist1 = data, hist2 = sim.
	h1 = hist1;
	h2 = hist2;
	
	TH1D* hist3 = new TH1D();
	h3 = hist3;
}


// Utility function(s):
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

int histfitter::CheckBinning()
{
	bool same_binning = HistsHaveSameBinning(h1, h2);
	int n_bins = 0;
	if(same_binning)
	{
		n_bins = h1 -> GetNbinsX();
	}
	
	if(bin_min < 1)      { bin_min=1; }
	if(bin_max > n_bins) { bin_max=n_bins; }
	
	return n_bins;
}

bool histfitter::SetFitLimits_ByBin(int minbin, int maxbin)
{
	bin_min = minbin;
	if(bin_min < 1) { bin_min=1; }
	bin_max = maxbin;
	
	return true;
}
bool histfitter::SetFitLimits_ByValue(double x_min, double x_max)
{
	// this doesn't work yet.
	return true;
}

// A Class to deal with TMinuit itself:
class SuperMinuit : virtual public TMinuit
{
public:
//	SuperMinuit() {};
	SuperMinuit():n_params(25), TMinuit(25) 
		{ init(); };
	SuperMinuit(int n):n_params(n), TMinuit(n) 
		{ init(); };
	SuperMinuit(histfitter * a_histfitter):n_params(0),TMinuit() 
		{this_histfitter = a_histfitter; init(); };
	
	void AssociateHistograms(histfitter * a_histfitter) {this_histfitter = a_histfitter;};
	histfitter * get_this_histfitter() {return this_histfitter; };

	int SetupParam(int c_paramnumber, FitParameter   fitpar);
	int SetupParam(int c_paramnumber, FitParameter * fitpar);
	
//	int CountParams();
	
	void Help()
		{ cout << "\"Help()\":  Not yet implemented." << endl; };
	
	int execute_simplex();
	int execute_simplex(int maxcalls, double err_est);
	int execute_migrad();
	int execute_migrad(int maxcalls, double err_est);
	int execute_hesse();
	int execute_hesse(int maxcalls, double err_est);
	int execute_minos();
	int execute_minos(int maxcalls, double err_est);
	
	double get_fmin_best();
	int get_errorest_best();
	int get_npar_varied_best();
	int get_errmatrix_quality_best();
	

	
//	void SetFitFunction(void (*fcn)(Int_t &, Double_t *, Double_t &, Double_t *, Int_t)) 
//		{ this -> SetFCN(fcn); }; 
	int get_n_params() 
	{
		n_params = this->GetNumPars();
		return n_params;
	};
	void init();
	void Setup_FitStopParams(bool);
	void Setup_FitStopParams(int, double);
	void SetMaxCalls(int maxcalls);
	void SetAcceptableErr(double err_est);
	
//	void * TheFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize, Double_t *parameters_, Int_t iflag_);
	void TheFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize, Double_t *parameters_, Int_t iflag_);
	
	/*
	void SetFCN_() 
	{ 
	//	this -> TMinuit::SetFCN( TheFitFunction(this->n_params, this->gin, this->result_to_minimize, this->parameters, this->ierflg) ); 
	//	// this works as long as it's a void*.
		this -> TMinuit::SetFCN( SuperMinuit::TheFitFunction(this->n_params, this->gin, this->result_to_minimize, this->parameters, this->ierflg) ); 
	};  // bake the fcn into the class.
	*/
	
// Below:  this doesn't work unless you still supply all the params from the call.	
//	void SetFCN(void (*fcn)(Int_t &, Double_t *, Double_t &, Double_t *, Int_t) ) 
//		{ this -> TMinuit::SetFCN(fcn); };  // set your own fcn.
	
//	vector<FitParameter*> fit_parameters;
	vector<FitParameter> fit_parameters;
	void DumpFitParams();
	
private:
	histfitter * this_histfitter;
	
	// best fit params:
	double fmin_val;  
	double fedm_val;
	double errdef_val;    // for memory space only?
	int npari_val;        // for memory space only?
	int nparx_val;
	int istat_val;
//	bool params_used[25];
	
	int n_params;                // mostly for memory space, but also returnable by get_n_params().
	Double_t * gin;              // mem space???  am I doing this right??
	Double_t result_to_minimize; // memspace?
	Double_t * parameters;       // memspace?
	Int_t ierflg;                // for memory space only?
	
	Double_t arglist[10];  // for memory space only?

	int length_of_arglist;
	Int_t n_maxcalls;
	Double_t est_distance_to_min;
};

// Fit function.
//void * SuperMinuit::TheFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize, Double_t *parameters_, Int_t iflag_)
void SuperMinuit::TheFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize, Double_t *parameters_, Int_t iflag_)
{
//	void * thisthing;
	
//	n_params = these_n_parameters;
//	gin = this_gin;
//	result_to_minimize = this_result_to_minimize;
//	parameters = these_parameters;
//	ierflg = this_iflag;
	
//	double asym = parameters[0];
//	double bg   = parameters[1];
//	result_to_minimize = chi2;  // "result" is the thing that gets not-returned.

	double result = 0.0;
	cout << "n_params_ = " << n_params_ << endl;
	double asym = parameters_[0];
	double bg   = parameters_[1];
//	cout << "ok?" << endl;
	
	if(n_params >= 2)
	{
	//	cout << "ok!" << endl;
	//	result_to_minimize = (parameters_[0] - 0.5)*(parameters_[1] - 0.4);
		result = (asym - 0.5)*(bg - 0.4);
	//	cout << "So... yeah?" << endl;
		cout << "result_to_minimize = " <<  result << ";\tparameters_[0] = " << parameters_[0] << ";\tparameters_[1] = " << parameters_[1] << endl;
	}
	
	result_to_minimize = result;
//	result_to_minimize = result_to_minimize_;
//	return thisthing;
	return;
}

void TheFitFunction_(Int_t &n_params, Double_t *gin, Double_t &result_to_minimize, Double_t *parameters, Int_t iflag)
{
	double result = 0.0;
	cout << "n_params = " << n_params << endl;
	double asym = parameters[0];
	double bg   = parameters[1];
	
	if(n_params >= 2)
	{
		result = (asym - 0.5)*(bg - 0.4);
		cout << "result_to_minimize = " <<  result << ";\tparameters[0] = " << parameters[0] << ";\tparameters[1] = " << parameters[1] << endl;
	}
	
	result_to_minimize = result;
	return;
}


void SuperMinuit::DumpFitParams()
{
	n_params = this->GetNumPars();  // gets the number of params that were *varied*.
	TString paramname = "";
	Double_t val, err, xlo, xup;
	Int_t iint;
	
//	fit_parameters = vector<FitParameter>(n_params);
	for(int i=0; i<n_params; i++)
	{
		this -> mnpout(i, paramname, val, err, xlo, xup, iint);
		this->fit_parameters[i].name = paramname;
		fit_parameters.at(i).fit_val = val;
		this->fit_parameters[i].fit_err = err;
		this->fit_parameters[i].xlo     = xlo;
		this->fit_parameters[i].xup     = xup;
		this->fit_parameters[i].iint    = iint;
	}
}

/*
void * TheFitFunction(Int_t &n_parameters, Double_t *this_gin, Double_t &result_to_minimize, Double_t *parameters, Int_t iflag) 
{
	void * thisthing;
//	double asym = parameters[0];
//	double bg   = parameters[1];

//	result_to_minimize = chi2;  // "result" is the thing that gets not-returned.
	gin = this_gin;
	result_to_minimize = 0.0;
	
	return thisthing;
}
*/

void SuperMinuit::init()
{
	parameters = new Double_t[25];
	gin = new Double_t[25];
	fit_parameters = vector<FitParameter>(25);

	length_of_arglist = 0;
	n_maxcalls = 500;
	est_distance_to_min = 100.0;  // what does this even mean?!
}

int SuperMinuit::SetupParam(int c_paramnumber, FitParameter fitpar)
{
	this -> mnparm(c_paramnumber, fitpar.name, fitpar.initial_val, fitpar.stepsize, fitpar.min_val, fitpar.max_val, ierflg);
	if(fitpar.is_fixed)
	{
		this -> FixParameter(c_paramnumber);
	}
	return ierflg;
}
int SuperMinuit::SetupParam(int c_paramnumber, FitParameter * fitpar)
{
	this -> mnparm(c_paramnumber, fitpar->name, fitpar->initial_val, fitpar->stepsize, fitpar->min_val, fitpar->max_val, ierflg);
	if(fitpar->is_fixed)
	{
		this -> FixParameter(c_paramnumber);
	}
	return ierflg;
}

/*
int SuperMinuit::SetupParam(FitParameter fitpar,   int c_paramnumber)
	{ return SetupParam(c_paramnumber, fitpar); }
int SuperMinuit::SetupParam(FitParameter * fitpar, int c_paramnumber)
	{ return SetupParam(c_paramnumber, fitpar); }
*/

/*
//minuit->GetNumPars()
int SuperMinuit::CountParams()
{
	int n = 0;
	for(int i=0; i<25; i++)
	{
		if(params_used[i] == true)
		{
			n++;
		}
		else
		{
			break;
		}
	}
	n_params = n;
	return n_params;
}
*/

void SuperMinuit::SetMaxCalls(int maxcalls)
{
	n_maxcalls = maxcalls;
	arglist[0] = n_maxcalls;
	
	if(length_of_arglist<1)
	{
		length_of_arglist=1;
	}
}
void SuperMinuit::SetAcceptableErr(double err_est)
{
	est_distance_to_min = err_est;
	arglist[1] = est_distance_to_min;
	
	if(length_of_arglist<2)
	{
		length_of_arglist=2;
	}
}


void SuperMinuit::Setup_FitStopParams(bool use_them)
{
	if(use_them)
	{
		length_of_arglist = 2;
	}
	else
	{
		length_of_arglist = 0;
	}
}
void SuperMinuit::Setup_FitStopParams(int maxcalls, double err_est)
{
	length_of_arglist = 2;
	n_maxcalls = maxcalls;
	est_distance_to_min = err_est;
	
	arglist[0] = n_maxcalls;
	arglist[1] = est_distance_to_min;
}

int SuperMinuit::execute_simplex() 
{
	this -> mnexcm("SIMPLEX", arglist, length_of_arglist, ierflg);  // err:  0
	return ierflg;
}
int SuperMinuit::execute_simplex(int maxcalls, double err_est) 
{
	Setup_FitStopParams(maxcalls, err_est);
	return execute_simplex();
}

int SuperMinuit::execute_migrad() 
{
	this -> mnexcm("MIGRAD", arglist, length_of_arglist, ierflg);  // err:  0
	return ierflg;
}
int SuperMinuit::execute_migrad(int maxcalls, double err_est) 
{
	Setup_FitStopParams(maxcalls, err_est);
	return execute_migrad();
}

int SuperMinuit::execute_hesse() 
{
	this -> mnexcm("HESSE", arglist, length_of_arglist, ierflg);  // err:  0
	return ierflg;
}
int SuperMinuit::execute_hesse(int maxcalls, double err_est) 
{
	Setup_FitStopParams(maxcalls, err_est);
	return execute_hesse();
}

int SuperMinuit::execute_minos() 
{
	this -> mnexcm("HESSE", arglist, length_of_arglist, ierflg);  // err:  0
	return ierflg;
}
int SuperMinuit::execute_minos(int maxcalls, double err_est) 
{
	Setup_FitStopParams(maxcalls, err_est);
	return execute_minos();
}


double SuperMinuit::get_fmin_best()
{
	this -> mnstat(fmin_val, fedm_val, errdef_val, npari_val, nparx_val, istat_val);
	return fmin_val;
}
int SuperMinuit::get_errorest_best()
{
	this -> mnstat(fmin_val, fedm_val, errdef_val, npari_val, nparx_val, istat_val);
	return fedm_val;
}
int SuperMinuit::get_npar_varied_best()
{
	this -> mnstat(fmin_val, fedm_val, errdef_val, npari_val, nparx_val, istat_val);
	return nparx_val;
}
int SuperMinuit::get_errmatrix_quality_best()
{
	this -> mnstat(fmin_val, fedm_val, errdef_val, npari_val, nparx_val, istat_val);
	return istat_val;
}


// 






