// ==================================================================== //
// Code by Melissa Anholm
// 28.9.2017
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


//
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
	double get_fedm_best();
	double get_errdef_best();
	int get_npar_varied_best();
	int get_npar_defined_best();
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
	
	static void TheFitFunction(Int_t &n_params_, Double_t *gin, Double_t &result_to_minimize, Double_t *parameters, Int_t iflag_);
	
	void SetFCN() 
	{ 
		this -> TMinuit::SetFCN( this -> TheFitFunction ); 
	};  // bake the fcn into the class.
//	this_minuit -> SetFCN(TheFitFunction_);              
//	this_minuit -> SetFCN(this_minuit -> TheFitFunction);
	
	vector<FitParameter> fit_parameters;
	void DumpBestFitParams();
	void DumpCurrentFitParams();
	
	void SetupOutput(string, string, int);
	void DumpToOutput();

	void ClosedownOutput() { logfilestream.close(); };
	
private:
	histfitter * this_histfitter;
	
//	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	double fmin_val;        //
	double fedm;            // Huh?  some sort of fit error.
	double errdef;          // Huh?  some sort of fit error.
	int npar_varied;        // 
	int npar_defined;       //
	int errmatrix_quality;  //
	
	int n_params;          // used.
	Int_t ierflg;          // for memory space only?
	
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
};

void SuperMinuit::SetupOutput(string o_type, string o_fname=string("fitoutput.txt"), int o_verbosity=1)
{
	output_type      = o_type;
	// Possible types:  
	//		none
	//		cout
	//		file
	//		both
	output_fname     = o_fname;
	output_verbosity = o_verbosity;
	
	// 
	output_type_to_file = false;
	output_type_to_cout = false;
	if( output_type == string("none") )
	{
		cout << " ... none." << endl;
	//	output_type_to_file = false;
	//	output_type_to_cout = false;
	}
	else if( output_type == string("cout") )
	{
		cout << " ... cout." << endl;
	//	output_type_to_file = false;
		output_type_to_cout = true;
	}
	else if( output_type == string("file") )
	{
		output_type_to_file = true;
	//	output_type_to_cout = false;
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
	}
	
	if(output_type_to_file==true)
	{
		cout << "output_fname set to:  " << output_fname << endl;
		if(logfilestream.is_open())
		{
			// close it.
			cout << "logfilestream is already open.  Closing it..." << endl;
			logfilestream.close();
			// open it.
			logfilestream.open( output_fname.c_str(), std::ios::out);
			cout << "Opening logfilestream for output." << endl;
		}
		else
		{
			// open it.
			logfilestream.open( output_fname.c_str(), std::ios::out);
			cout << "Opening logfilestream for output." << endl;
			// Clear it.
		}
	}
}
void SuperMinuit::DumpToOutput()
{
	cout << "SuperMinuit::DumpToOutput() doesn't work yet." << endl;
	DumpCurrentFitParams();
	/*
	this -> mnpout(i, paramname, val, err, xlo, xup, iint);
	this->fit_parameters[i].name    = paramname;
	this->fit_parameters[i].fit_val = val;
	this->fit_parameters[i].fit_err = err;
	this->fit_parameters[i].xlo     = xlo;
	this->fit_parameters[i].xup     = xup;
	this->fit_parameters[i].iint    = iint;
	*/
	if(output_type_to_file)
	{
		if(n_calls == 0)
		{
			for (int i = 0; i < n_params; i++) 
			{
				// First dump the param summary for all params.
				logfilestream << "Parameter " << setw(2) << i << ":\t";
				logfilestream << fit_parameters[i].name << "\t" << endl ;
				logfilestream << "\tRange     = (";
				logfilestream << fit_parameters[i].xlo << ", ";
				logfilestream << fit_parameters[i].xup << ")." << endl;
				logfilestream << "\tGuess     = " << fit_parameters[i].fit_val << endl;
				logfilestream << "\tPrecision = " << fit_parameters[i].fit_err << endl;
			}
			logfilestream << "Fit type:  " << current_fittype << endl;
			logfilestream << "Verbosity: " << output_verbosity << endl;
			logfilestream << "n_fits:    " << n_fits << endl;
		//	logfilestream << "n_calls:   " << n_calls << endl;
			logfilestream << endl;
			// 
			logfilestream << setw(7) << "n_calls" << "\t";
			for (int i = 0; i < n_params; i++) 
			{
				// param value.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+1);
				logfilestream << setw(current_columnwidth) << fit_parameters[i].name << "\t";
				
				// param err.
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+4+1);
				logfilestream << setw(current_columnwidth) << fit_parameters[i].name << "_err\t";
			}
			logfilestream << setw(20) << "chi2" << endl;
		}
		else
		{
			logfilestream << setw(7) << n_calls << "\t";
			for (int i = 0; i < n_params; i++) 
			{
				// how wide do we make this column ?
				current_columnwidth = fmax(orig_columnwidth, fit_parameters[i].name.Length()+1);
				logfilestream << setw(current_columnwidth) << fit_parameters[i].fit_val << "\t";
			}
			logfilestream << setw(20) << "chi2" << endl;
			
		}
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
//	cout << "SuperMinuit::DumpToOutput() doesn't work yet." << endl;
	n_calls++;  // 
}

void SuperMinuit::init()
{
	fit_parameters = vector<FitParameter>(25);
	
	length_of_arglist = 0;
	n_maxcalls = 500;
	est_distance_to_min = 100.0;  // what does this even mean?!
	
	n_fits = 0;
	n_calls = 0;
	
	output_type_to_file = false;
	output_type_to_cout = true;
	current_fittype = string("");

//	this -> TMinuit::SetFCN( this -> TheFitFunction ); 
}


// Fit function.
void SuperMinuit::TheFitFunction(Int_t &n_params_, Double_t *gin, Double_t &result_to_minimize, Double_t *parameters, Int_t iflag_)
{
	double result = 0.0;
	cout << "n_params_ = " << n_params_ << endl;
	double asym = parameters[0];
	double bg   = parameters[1];
	
	if(n_params_ >= 2)
	{
		result = fabs( (asym - 0.5)*(bg - 0.4) + (asym - 0.5) + (bg - 0.4) );
		cout << "result_to_minimize = " <<  result << ";\tparameters[0] = " << parameters[0] << ";\tparameters[1] = " << parameters[1] << endl;
	}
	
	result_to_minimize = result;
	// dump param (etc) values from fit to ... something.
	return;
}

void SuperMinuit::DumpBestFitParams()
{
	// Below:  this actually gets you the *current* values of the parameters.
	/*
	// For the overall fit;
	n_params = this->GetNumPars();  // gets the number of params that were *varied*.
	
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	// error est ?
	
	// For the individual parameters:
	TString paramname = "";
	Double_t val, err, xlo, xup;
	Int_t f_pnum;  // f_pnum = fortran param number.
	
	for(int i=0; i<n_params; i++)
	{
		this -> mnpout(i, paramname, val, err, xlo, xup, f_pnum);
		this->fit_parameters[i].name    = paramname;
		this->fit_parameters[i].fit_val = val;
		this->fit_parameters[i].fit_err = err;
		this->fit_parameters[i].xlo     = xlo;
		this->fit_parameters[i].xup     = xup;
		this->fit_parameters[i].f_pnum  = f_pnum;
	}
	*/
	return;
}

void SuperMinuit::DumpCurrentFitParams()
{
	// For the overall fit;
	n_params = this->GetNumPars();  // gets the number of params that were *varied*.
	
	this -> mnstat(fmin_val, fedm, errdef, npar_varied, npar_defined, errmatrix_quality);
	// error est ?
	
	// For the individual parameters:
	TString paramname = "";
	Double_t val, err, xlo, xup;
	Int_t f_pnum;  // f_pnum = fortran param number.
	
	for(int i=0; i<n_params; i++)
	{
		this -> mnpout(i, paramname, val, err, xlo, xup, f_pnum);
		this->fit_parameters[i].name    = paramname;
		this->fit_parameters[i].fit_val = val;
		this->fit_parameters[i].fit_err = err;
		this->fit_parameters[i].xlo     = xlo;
		this->fit_parameters[i].xup     = xup;
		this->fit_parameters[i].f_pnum  = f_pnum;
	}
	return;
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
	
	n_fits++;
	n_calls = 0;
	// set each parameter's "fit_completed" to "false" ??
	current_fittype = string("SIMPLEX");	
	this -> mnexcm("SIMPLEX", arglist, length_of_arglist, ierflg);  // err:  0
	is_finished = true;
	return ierflg;
}
int SuperMinuit::execute_migrad() 
{
	is_finished = false;
	
	n_fits++;
	n_calls = 0;
	current_fittype = string("MIGRAD");
	this -> mnexcm("MIGRAD", arglist, length_of_arglist, ierflg);  // err:  0
	is_finished = true;
	return ierflg;
}
int SuperMinuit::execute_hesse() 
{
	// Hesse doesn't update the best fit parameters?  
	// It does vary them though.
	is_finished = false;
	
	n_fits++;
	n_calls = 0;
	current_fittype = string("HESSE");
	this -> mnexcm("HESSE", arglist, length_of_arglist, ierflg);  // err:  0
	is_finished = true;
	return ierflg;
}
int SuperMinuit::execute_minos() 
{
	is_finished = false;
	
	n_fits++;
	n_calls = 0;
	current_fittype = string("MINOS");
	this -> mnexcm("MINOS", arglist, length_of_arglist, ierflg);  // err:  0
	is_finished = true;
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






