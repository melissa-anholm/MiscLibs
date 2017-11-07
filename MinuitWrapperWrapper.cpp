// ==================================================================== //
// Code by Melissa Anholm
// 28.9.2017 - 31.10.2017
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

/*
struct FitResult
{
public:
	FitResult();
	void Load(double fit_min_, double fedm_, double errdef_, int npar_varied_, int npar_defined_, int errmatrix_quality_);
	void Load(TMinuit a_minuit);
	void Load(TMinuit * a_minuit);
	
//	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);

	TString fit_type;
	double  fit_min;
	double  fedm;
	double  errdef;
	int     npar_varied;
	int     npar_defined;
	int     errmatrix_quality;

//	for errmatrix_quality:  
//		0 = Not calculated at all
//		1 = Diagonal approximation only, not accurate
//		2 = Full matrix, but forced positive-definite
//		3 = Full accurate covariance matrix (After MIGRAD, this is the indication of normal convergence.)
};
FitResult::FitResult() 
{
	fit_type          = "";
	fit_min           = 0.0;
	fedm              = 0.0;
	errdef            = 0.0;
	npar_varied       = 0;
	npar_defined      = 0;
	errmatrix_quality = 0;
}
void FitResult::Load(double fit_min_, double fedm_, double errdef_, int npar_varied_, int npar_defined_, int errmatrix_quality_)
{
//	fit_type          = fit_type_;
	fit_min           = fit_min_;
	fedm              = fedm_;
	errdef            = errdef_;
	npar_varied       = npar_varied_;
	npar_defined      = npar_defined_;
	errmatrix_quality = errmatrix_quality_;
}
void FitResult::Load(TMinuit a_minuit)
{
	fit_type = "";
	a_minuit.mnstat(fit_min, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
}
void FitResult::Load(TMinuit * a_minuit)
{
	fit_type = "";
	a_minuit -> mnstat(fit_min, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
}
*/

//
class FitParameter
{
public:
	FitParameter();
	FitParameter(TString name_, double start_, double step_);
	FitParameter(TString name_, double start_, double step_, double min_, double max_, bool is_fixed_);
	
	double initial_val;
	double stepsize;
	double min_val;
	double max_val;
	bool is_fixed;
//	bool is_bounded;
	
	bool   fit_completed;
	double fit_val;
	double fit_err;  // ?
	
	double eplus;
	double eminus;
	double eparab;
	double globcc;
	
	void Unbound()
	{
		min_val = 0.0;
		max_val = 0.0;
	};
	void SetLimits(double min_val_, double max_val_)
	{
		min_val = min_val_;
		max_val = max_val_;
	};
	
	Double_t xlo, xup; // ??
	Int_t f_pnum;  // f_pnum = fortran_paramnumber
	
	TString GetName() {return name; };
	void SetName(TString newname) { name = TString(newname); };
	TString name;
};

FitParameter::FitParameter()
{
	TString name_       = "";
	double initial_val_ = 0.0;
	double step_        = 0.1;
	double min_         = 0.0;
	double max_         = 0.0;
	bool is_fixed_      = false;
//	FitParameter(name_, initial_val_, step_, min_, max_, is_fixed_);
	FitParameter(TString(""), 0.0, 0.1, 0.0, 0.0, false);
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
	
	eplus  = 0;
	eminus = 0;
	eparab = 0;
	globcc = 0;
//	cout << "Original FitParameter is called for param:  " << name_ << endl;
}

FitParameter::FitParameter(TString name_, double initial_val_, double step_)
{
	name         = name_;
	initial_val  = initial_val_;
	stepsize     = step_;
	min_val      = 0.0;
	max_val      = 0.0;
	is_fixed     = false;
	
	fit_completed = false;
	fit_val = 0;
	fit_err = 0;
	
	eplus  = 0;
	eminus = 0;
	eparab = 0;
	globcc = 0;
	
//	cout << "calling FitParameter from inside FitParameter for " << name_ << "..." << endl;
// 	FitParameter(name_, initial_val_, step_, 0.0, 0.0, false);  // BAD!  This DOES NOT WORK!
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
	double get_fedm_best();
	double get_errdef_best();
	int get_npar_varied_best();
	int get_npar_defined_best();
	int get_errmatrix_quality_best();
	
	Int_t Get_n_params() 
	{
		n_params = this->GetNumPars();  // gets the number of params that were *varied*.
		return n_params; 
	};
	Int_t Get_n_fixed_params() 
	{
		n_fixed_params = this->GetNumFixedPars();  //
		return n_fixed_params; 
	};
	Int_t Get_n_free_params() 
	{
		n_free_params = this->GetNumFreePars();  //
		return n_free_params; 
	};
	
//	int FixParameter(int internal_paramnumber) // must be an int to match the original..
//		{ return this -> TMinuit::FixParameter(internal_paramnumber); };
	void FreeParameter(int internal_paramnumber) 
		{ this -> TMinuit::mnfree(-1*internal_paramnumber); };
	
	Double_t * Get_gin() {return gin; }; 
	Double_t Get_result_to_minimize() {return result_to_minimize; };
	Double_t * Get_parameters() {return parameters; };
	Int_t Get_ierflg() {return ierflg; };

	void init();
	void Setup_FitStopParams(bool);
	void Setup_FitStopParams(int, double);
	void SetMaxCalls(int maxcalls);
	void SetAcceptableErr(double err_est);
	
	vector<FitParameter> fit_parameters;
	void DumpBestFitParams();
	void DumpCurrentFitParams();
	
	void SetupOutput(string o_type, string o_fname=string("fitoutput.txt"), int o_verbosity=1);

	void SetOutputVerbosity(int o_verbosity=1)
	{ 
		output_verbosity = o_verbosity; 
		cout << "Set output verbosity to:  " << output_verbosity << endl;
	};
	void DumpToOutput();

	void ClosedownOutput() { logfilestream.close(); };
	
	int DoTheThing(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_);
	int which_thing;
	
private:
	void SetupOutputType(string);  // private!
	void OutputHeader();  // private?
	void OutputHeader1();  // private?
	void OutputHeader2();  // private?
	void DumpFitResults();  // ...private??

	histfitter * this_histfitter;
	
//	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	double fmin_val;        //
	double fedm;            // Huh?  some sort of fit error.
	double errdef;          // Huh?  some sort of fit error.
	int npar_varied;        // 
	int npar_defined;       //
	int errmatrix_quality;  //
	
	Double_t arglist[10];  // for memory space only?

	int length_of_arglist;
	Int_t n_maxcalls;
	Double_t est_distance_to_min;
	
	int n_fits;
	int n_calls;
	bool is_finished;
	
	string output_type;
	string output_fname;
	string current_fittype;
	
	int     output_verbosity;
	bool    output_type_to_file;
	bool    output_type_to_cout;
	
	ofstream         logfilestream;
	const static int orig_columnwidth = 12;
	int              current_columnwidth;
	
	Int_t n_fixed_params;
	Int_t n_free_params;
	Int_t n_params;        // used for something real.
	
public:
//	Int_t n_params;           // used, for something real.  but also for something fake.
	Int_t n_params_i;  // used for something fake.
	Double_t *gin;
	Double_t result_to_minimize;  // don't need it?
	Double_t *parameters;  // don't need it?
	Int_t ierflg;
	
};
SuperMinuit * global_minuit = new SuperMinuit();  // Create a global instance.
//SuperMinuit * global_minuit = 0;  // Create a global instance.

void NonMemberFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_)
{
//	double asym = parameters_[0];
//	double bg   = parameters_[1];
//	result_to_minimize_ = fabs( (asym - 0.5)*(bg - 0.4) + (asym - 0.5) + (bg - 0.4) );
	
	global_minuit -> DoTheThing(n_params_, gin_, result_to_minimize_, parameters_, ierflg_);

//	double val0 = 0.5;
//	double val1 = 0.4;
//	result_to_minimize_ = pow((parameters_[0] - val0), 2) + pow((parameters_[1] - val1), 2);
	
	/*
	n_params_           = global_minuit -> Get_n_params();
	gin_                = global_minuit -> Get_gin();
	result_to_minimize_ = global_minuit -> Get_result_to_minimize();
	parameters_         = global_minuit -> Get_parameters();
	ierflg_             = global_minuit -> Get_ierflg();
	*/
	
//	global_minuit -> DumpToOutput();
	global_minuit -> n_params_i         = n_params_;
	global_minuit -> gin                = gin_;
	global_minuit -> result_to_minimize = result_to_minimize_;
	global_minuit -> parameters         = parameters_;
	global_minuit -> ierflg             = ierflg_;
	
	return;
}

int SuperMinuit::DoTheThing(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_)
{
	/*
	n_params           = n_params_;
	gin                = gin_;
	result_to_minimize = result_to_minimize_;
	parameters         = parameters_;
	ierflg             = ierflg_;
	*/
	
	double val0 = 0.5;
	double val1 = 0.4;
	
	// Which thing?
	if( !(which_thing==1) )
	{
		cout << "ERROR:  which_thing = " << which_thing << endl;
		return 0;
	}
	if(n_params_ >= 2)
	{
	//	double asym = parameters[0];
	//	double bg   = parameters[1];
	//	result_to_minimize = fabs( (asym - 0.5)*(bg - 0.4) + (asym - 0.5) + (bg - 0.4) );
	//	result_to_minimize = pow( (asym - 0.5)*(bg - 0.4) + (asym - 0.5) + (bg - 0.4), 2);
	//	result_to_minimize = pow( (asym - 0.5)*(bg - 0.4), 2);
	//	result_to_minimize = pow( (asym - 0.5), 2) + pow( (bg - 0.4), 2);
	//	result_to_minimize = pow((parameters_[0] - val0), 2) + pow((parameters_[1] - val1), 2);
	//	result_to_minimize = fabs( (asym - 0.5)*(bg - 0.4) );
		
		result_to_minimize_ = pow((parameters_[0] - val0), 2) + pow((parameters_[1] - val1), 2);
	//	result_to_minimize_ = pow((parameters_[0] - val0)/parameters_[0], 2) + pow((parameters_[1] - val1)/parameters_[1], 2);
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

void SuperMinuit::init()
{
	fit_parameters = vector<FitParameter>(25);
	
	length_of_arglist = 0;
	n_maxcalls = 500;
	est_distance_to_min = 100.0;  // what does this even mean?!
	
	n_fits = 0;
	n_calls = 0;
	which_thing = 1;

	is_finished = false;
	SetupOutput("file");
	
	current_fittype = string("");
	
	Int_t n_params = 25;
	Double_t *gin  = new Double_t(25);
//	Double_t result_to_minimize;
	Double_t *parameters = new Double_t(25);  
//	Int_t ierflg;

	this -> TMinuit::SetFCN( NonMemberFitFunction ); // this line only works if it's a *static* void...
}

void SuperMinuit::SetupOutputType(string o_type)
{
	output_type      = o_type;
	cout << "Setting output_type:  " << output_type << endl;
	// Possible types:  
	//		none
	//		cout
	//		file
	//		both
	
	if( output_type == string("none") )
	{
		output_type_to_file = false;
		output_type_to_cout = false;
	}
	else if( output_type == string("cout") )
	{
		output_type_to_file = false;
		output_type_to_cout = true;
	}
	else if( output_type == string("file") )
	{
		output_type_to_file = true;
		output_type_to_cout = false;
	}
	else if( output_type == string("both") )
	{
		output_type_to_file = true;
		output_type_to_cout = true;
	}
	else
	{
		cout << "Output type not recognized:  " << o_type << endl;
		cout << "Setting output type to none."  << endl;
		output_type = string("none");
		output_type_to_file = false;
		output_type_to_cout = false;
	}
	
	if(output_type_to_file == false)
	{
		if( logfilestream.is_open() )
		{
			// close it.
			cout << "File output has been disabled.  Closing logfilestream now..." << endl;
			logfilestream.close();
		}
	}
}

void SuperMinuit::SetupOutput(string o_type, string o_fname, int o_verbosity)
{
	SetupOutputType(o_type);
	
	output_fname     = o_fname;
	cout << "output_fname set to:      " << output_fname << endl;
	output_verbosity = o_verbosity;
	cout << "output_verbosity set to:  " << output_verbosity << endl;
}


void SuperMinuit::OutputHeader()
{
	OutputHeader1();
	OutputHeader2();
}

void SuperMinuit::OutputHeader1()
{
	if(output_type_to_file==true)
	{
		if(!logfilestream.is_open())
		{
			cout << "Opening file " << output_fname << " for log output." << endl;
			logfilestream.open( output_fname.c_str(), std::ios::out);
		} // it's open now.
	
		// do I need to do this?  .. yes, n_params (etc) probably aren't set yet.
		DumpCurrentFitParams();
		
		logfilestream << "Fit type:       " << current_fittype << endl;
		logfilestream << "Verbosity:      " << output_verbosity << endl;
		logfilestream << "n_fits:         " << n_fits << endl;
		logfilestream << "n_params:       " << n_params << endl;
		logfilestream << "n_fixed_params: " << n_fixed_params << endl;
		logfilestream << "n_free_params:  " << n_free_params << endl;
		for (int i = 0; i < n_params; i++) 
		{
			// First dump the param summary for all params.
			logfilestream << "Parameter " << setw(2) << i << ":  ";
			logfilestream << fit_parameters[i].name << endl;
			logfilestream << "\tRange = (";
			logfilestream << fit_parameters[i].xlo << ", ";
			logfilestream << fit_parameters[i].xup << ")" << endl;
			logfilestream << "\tGuess = " << fit_parameters[i].fit_val << endl;
			logfilestream << "\tStep  = " << fit_parameters[i].stepsize << endl;
		}
	}
	else
	{
		cout << "File output is disabled." << endl;
	}
	return;
}
void SuperMinuit::OutputHeader2()
{
//	logfilestream << "n_params = " << n_params << endl;
	if(output_type_to_file==true)
	{
		// 
		logfilestream << setw(7) << std::right << "n_calls";
		logfilestream << setw(4) << std::left << "\t";
		logfilestream << setw(orig_columnwidth) << std::left << "chi2" << "\t";
		for (int i = 0; i < n_params; i++) 
		{
			// param value.
			current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length());
			logfilestream << setw(current_columnwidth) << std::left << fit_parameters[i].name;
			logfilestream << "\t";
		}
		for (int i = 0; i < n_params; i++) 
		{
			if(output_verbosity>=2)
			{
				// param err.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+4);
				logfilestream << setw(current_columnwidth) << std::left << fit_parameters[i].name+"_err";
				logfilestream << "\t";
			}
		}
		for (int i = 0; i < n_params; i++) 
		{
			if(output_verbosity>=3)
			{
				// param eparab.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+7);
				logfilestream << setw(current_columnwidth) << std::left << fit_parameters[i].name+"_eparab";
				logfilestream << "\t";
			}
		}
		for (int i = 0; i < n_params; i++) 
		{
			if(output_verbosity>=4)
			{
				// param err_p.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+6);
				logfilestream << setw(current_columnwidth) << std::left << fit_parameters[i].name+"_err_p";
				logfilestream << "\t";
				// param err_m.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+6);
				logfilestream << setw(current_columnwidth) << std::left << fit_parameters[i].name+"_err_m";
				logfilestream << "\t";
			}
		}
		logfilestream << endl;
	}
}

void SuperMinuit::DumpToOutput()
{
//	logfilestream << "n_params = " << n_params << ", \t";
	DumpCurrentFitParams();
//	logfilestream << "n_params = " << n_params << endl;
	if(output_type_to_file)
	{
		logfilestream << setw(7) << std::right << n_calls;// << "\t";
		logfilestream << setw(4) << std::left << "\t";
		logfilestream << setw(orig_columnwidth) << std::left << result_to_minimize << "\t";
		for (int i = 0; i < n_params; i++) 
		{
			// param value.
			current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length());
			logfilestream << setw(orig_columnwidth) << std::left << fit_parameters[i].fit_val;
			logfilestream << "\t";
		}
		for (int i = 0; i < n_params; i++) 
		{
			if(output_verbosity>=2)
			{
				// param err.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+4);
				logfilestream << setw(orig_columnwidth) << std::left << fit_parameters[i].fit_err;
				logfilestream << "\t";
			}
		}
		for (int i = 0; i < n_params; i++) 
		{
			if(output_verbosity>=3)
			{
				// param eparab.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+7);
				logfilestream << setw(current_columnwidth) << std::left << fit_parameters[i].eparab;
				logfilestream << "\t";
			}
		}
		for (int i = 0; i < n_params; i++) 
		{
			if(output_verbosity>=4)
			{
				// param err_p.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+6);
				logfilestream << setw(current_columnwidth) << std::left << fit_parameters[i].eplus;
				logfilestream << "\t";
				// param err_m.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+6);
				logfilestream << setw(current_columnwidth) << std::left << fit_parameters[i].eminus;
				logfilestream << "\t";
			}
		}
		logfilestream << endl;
		
		if(is_finished==true)
		{
			// Output the final result.
		}
	}
	if(output_type_to_cout)
	{
		//
		cout << "Output to cout does not work yet." << endl;
	}
}

void SuperMinuit::DumpFitResults()
{
	logfilestream << endl;
	logfilestream << "Final results from " << current_fittype << endl;
//	logfilestream << "n_params = " << n_params << endl;
	OutputHeader2();
//	logfilestream << "n_params = " << n_params << endl;
	DumpToOutput();
//	logfilestream << "n_params = " << n_params << endl;
	
	int this_paramnumber = 0;
	this_paramnumber = 0;
	for(int this_paramnumber = 0; this_paramnumber < n_params; this_paramnumber++)
	{
		logfilestream << "param:  " << this_paramnumber << "  " << (fit_parameters.at(this_paramnumber) ).GetName() << ":  ";
		logfilestream << fit_parameters[this_paramnumber].fit_val << " +/- " << fit_parameters[this_paramnumber].fit_err << ", f_pnum = " << fit_parameters[this_paramnumber].f_pnum << endl;
	}
	
	logfilestream << "fmin_best    = " << global_minuit -> get_fmin_best() << endl;
	logfilestream << "fedm_best    = " << global_minuit -> get_fedm_best() << endl;
	logfilestream << "errdef_best  = " << global_minuit -> get_errdef_best() << endl;

	logfilestream << endl;
	logfilestream << endl;
}

void SuperMinuit::DumpCurrentFitParams()
{
	// For the overall fit;
	n_params = this->GetNumPars();  // gets the number of params that were *varied*.
	n_fixed_params = this->GetNumFixedPars();  // 
	n_free_params = this->GetNumFreePars();  // 
	
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	// error est ?
	
	// For the individual parameters:
	TString paramname = "";
	Double_t val, err, xlo, xup;
	Int_t f_pnum;  // f_pnum = fortran param number.
	
	Double_t eplus, eminus, eparab, globcc;
	
	for(int i=0; i<n_params; i++)
	{
		this -> mnpout(i, paramname, val, err, xlo, xup, f_pnum);
		this->fit_parameters[i].name      = paramname;
		this->fit_parameters[i].fit_val   = val;
		this->fit_parameters[i].fit_err   = err;
		this->fit_parameters[i].stepsize  = err;
		this->fit_parameters[i].xlo       = xlo;
		this->fit_parameters[i].xup       = xup;
		this->fit_parameters[i].f_pnum    = f_pnum;
		
		this -> mnerrs(i, eplus, eminus, eparab, globcc);
		this->fit_parameters[i].eplus     = eplus;
		this->fit_parameters[i].eminus    = eminus;
		this->fit_parameters[i].eparab    = eparab;
		this->fit_parameters[i].globcc    = globcc;
	}
	return;
//	fit_min
}

int SuperMinuit::SetupParam(int c_paramnumber, FitParameter fitpar)
{
	cout << "Setting up parameter:  " << c_paramnumber << endl;
	cout << "\t" << fitpar.name << endl;
	cout << "\t" << fitpar.initial_val << endl;
	cout << "\t" << fitpar.stepsize << endl;
	cout << "\t" << fitpar.min_val << endl;
	cout << "\t" << fitpar.max_val << endl;
	cout << "\t" << "ierflg = " << ierflg << endl;
	
	this -> mnparm(c_paramnumber, fitpar.name, fitpar.initial_val, fitpar.stepsize, fitpar.min_val, fitpar.max_val, ierflg);
	if(fitpar.is_fixed)
	{
		cout << "Fixing param:  " << fitpar.name << endl;
		this -> FixParameter(c_paramnumber);
	}
	cout << "\t" << "ierflg = " << ierflg << endl;
	return ierflg;
}
int SuperMinuit::SetupParam(int c_paramnumber, FitParameter * fitpar)
{
	cout << "Setting up parameter:  " << c_paramnumber << endl;
	cout << "\t" << fitpar->name << endl;
	cout << "\t" << fitpar->initial_val << endl;
	cout << "\t" << fitpar->stepsize << endl;
	cout << "\t" << fitpar->min_val << endl;
	cout << "\t" << fitpar->max_val << endl;
	cout << "\t" << "ierflg = " << ierflg << endl;

	this -> mnparm(c_paramnumber, fitpar->name, fitpar->initial_val, fitpar->stepsize, fitpar->min_val, fitpar->max_val, ierflg);
	if(fitpar->is_fixed)
	{
		cout << "Fixing param:  " << fitpar->name << endl;
		this -> FixParameter(c_paramnumber);
	}
	cout << "\t" << "ierflg = " << ierflg << endl;
	return ierflg;
}

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
	is_finished = false;
	cout << "Calling SIMPLEX ..." << endl;
	n_fits++;
	n_calls = 0;
	current_fittype = string("SIMPLEX");
//	DumpCurrentFitParams();
	OutputHeader();

	this -> mnexcm("SIMPLEX", arglist, length_of_arglist, ierflg);  // err:  0
	
	is_finished = true;
//	logfilestream << "n_params = " << n_params << endl;
	DumpFitResults();
	return ierflg;
}
int SuperMinuit::execute_migrad() 
{
	is_finished = false;
	cout << "Calling MIGRAD ..." << endl;
	n_fits++;
	n_calls = 0;
	current_fittype = string("MIGRAD");
//	DumpCurrentFitParams();
	OutputHeader();
	
	this -> mnexcm("MIGRAD", arglist, length_of_arglist, ierflg);  // err:  0
	
	is_finished = true;
//	logfilestream << "n_params = " << n_params << endl;
	DumpFitResults();
	return ierflg;
}
int SuperMinuit::execute_hesse() 
{
	// Hesse doesn't update the best fit parameters?  
	// It does vary them though.
	is_finished = false;
	cout << "Calling HESSE ..." << endl;
	n_fits++;
	n_calls = 0;
	current_fittype = string("HESSE");
//	DumpCurrentFitParams();
	OutputHeader();
	
	this -> mnexcm("HESSE", arglist, length_of_arglist, ierflg);  // err:  0
	
	is_finished = true;
//	logfilestream << "n_params = " << n_params << endl;
	DumpFitResults();
	return ierflg;
}
int SuperMinuit::execute_minos() 
{
	is_finished = false;
	cout << "Calling MINOS ..." << endl;
	n_fits++;
	n_calls = 0;
	current_fittype = string("MINOS");
//	DumpCurrentFitParams();
	OutputHeader();
	
	this -> mnexcm("MINOS", arglist, length_of_arglist, ierflg);  // err:  0

	is_finished = true;
//	logfilestream << "n_params = " << n_params << endl;
	DumpFitResults();
	return ierflg;
}

int SuperMinuit::execute_simplex(int maxcalls, double err_est) 
{
	Setup_FitStopParams(maxcalls, err_est);
	return execute_simplex();
}
int SuperMinuit::execute_migrad(int maxcalls, double err_est) 
{
	Setup_FitStopParams(maxcalls, err_est);
	return execute_migrad();
}
int SuperMinuit::execute_hesse(int maxcalls, double err_est) 
{
	Setup_FitStopParams(maxcalls, err_est);
	return execute_hesse();
}
int SuperMinuit::execute_minos(int maxcalls, double err_est) 
{
	Setup_FitStopParams(maxcalls, err_est);
	return execute_minos();
}
//
double SuperMinuit::get_fmin_best()
{
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	return fmin_val;
}
double SuperMinuit::get_fedm_best()
{
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	return fedm;
}
double SuperMinuit::get_errdef_best()
{
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	return errdef;
}
int SuperMinuit::get_npar_varied_best()
{
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	return npar_varied;
}
int SuperMinuit::get_npar_defined_best()
{
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	return npar_defined;
}
int SuperMinuit::get_errmatrix_quality_best()
{
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	return errmatrix_quality;
}


// 

/*
struct OAS // OverArchingStructure
{
	OAS(SuperMinuit*);
	SuperMinuit * instance_of_superminuit;
	void this_fcn(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_);
	void set_the_goddamn_fitfunction();
};

OAS::OAS(SuperMinuit *this_instance_of_minuit)
{
	instance_of_superminuit = new SuperMinuit();
	instance_of_superminuit = this_instance_of_minuit;
}

void OAS::set_the_goddamn_fitfunction()
{
// this -> TMinuit::SetFCN( NonMemberFitFunction );
	instance_of_superminuit -> TMinuit::SetFCN(this_fcn);
}

void OAS::this_fcn(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_)
{
	double asym = parameters_[0];
	double bg   = parameters_[1];
	result_to_minimize_ = fabs( (asym - 0.5)*(bg - 0.4) + (asym - 0.5) + (bg - 0.4) );
	
	
	// Do this at the end...
//	this_minuit -> AssignPrivateFitParamsAndShit(n_params_, result_to_minimize_, parameters_, ierflg_);
	
	return;
}
*/

