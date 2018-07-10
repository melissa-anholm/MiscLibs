// ==================================================================== //
// Code by Melissa Anholm
// 28.9.2017 - 3.7.2018
// 
// ==================================================================== //

#include <iostream>  // cout, endl;
#include <iomanip>   // setw
#include <vector>

using std::cout;    // already in .h
using std::endl;    // already in .h
using std::setw;    
using std::vector;
using std::string;  // already in MinuitWrapperWrapper3.h, but it doesn't matter.


#include "MinuitFitterExtensions3.h"  // combo_histfitter, superasym_histfitter

//extern bool HistsHaveSameBinning(TH1D *a, TH1D *b, bool verbose);
//extern double get_chi2_thisbin(double h1_bincontent, double h2_bincontent, double h1_berr=0, double h2_berr=0);
extern void NonMemberFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_);


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
// A Class to deal with TMinuit itself:
class SuperMinuit : virtual public TMinuit
{
public:
//	SuperMinuit() {};
	SuperMinuit():n_params(25), TMinuit(25) 
		{ init(); };
//	SuperMinuit(int n):n_params(n), TMinuit(n) 
//		{ init(); };

	int SetupParam(int c_paramnumber, FitParameter   fitpar);
	int SetupParam(int c_paramnumber, FitParameter * fitpar);
	FitParameter get_FitParameter(int n) { return this->fit_parameters.at(n); };
	
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
	
	int MakeTheMinimizationGo(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_);
	
	int which_thing;
	void SetupFCN_original      (TH1D* hist_to_fit);
	void SetupFCN_histfitter    ( combo_histfitter * chf );
	void SetupFCN_histfitter_A  ( combo_histfitter * chf );
	void SetupFCN_histfitter_Ab ( combo_histfitter * chf );
	void SetupFCN_histfitter_superasym( superasym_histfitter * sahf );
	
	void set_bmin(int newbin) { fit_bmin = newbin; }
	void set_bmax(int newbin) { fit_bmax = newbin; }
	int get_bmin() {return fit_bmin; }
	int get_bmax() {return fit_bmax; }
	int set_xmin(double xmin, TH1D*h)
	{
		int binno = h->GetXaxis()->FindBin(xmin);
		double bin_minx = h->GetBinCenter(binno) - 0.5*h->GetBinWidth(binno);
		if(bin_minx<xmin)
		{
			binno++;
		}
		set_bmin(binno);
		return binno;
	}
	int set_xmax(double xmax, TH1D*h)
	{
		int binno = h->GetXaxis()->FindBin(xmax);
		double bin_maxx = h->GetBinCenter(binno) + 0.5*h->GetBinWidth(binno);
		if(bin_maxx>xmax)
		{
			binno--;
		}
		set_bmax(binno);
		return binno;
	}
//	double get_xmin()
//	{
//		
//	}
//	double get_xmax()
//	{
//		
//	}
	void set_fitrange(int xmin, int xmax, TH1D*h)
	{
		set_xmin(xmin, h);
		set_xmax(xmax, h);
	}
	
	int get_n_fitbins() { return fit_bmax - fit_bmin + 1; }
	void increment_n_calls() { n_calls++; }
	
private:
	vector<FitParameter> fit_parameters;  // just for memory space...
	
	int fit_bmin;
	int fit_bmax;
	
	void SetupOutputType(string);  // private!
	void OutputHeader();           // private?
	void OutputHeader1();          // private?
	void OutputHeader2();          // private?
	void DumpFitResults();         // ...private??

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
	
	std::ofstream         logfilestream;
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
// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //

void SuperMinuit::init()
{
	fit_parameters = vector<FitParameter>(25);
	
	length_of_arglist = 0;
	n_maxcalls = 500;
	est_distance_to_min = 100.0;  // what does this even mean?!
	
	n_fits = 0;
	n_calls = 0;
	which_thing = 0; // 0 == uninitialized.
	
	is_finished = false;
	SetupOutput("file");
	
	current_fittype = string("");
	
	Int_t n_params = 25;
	Double_t *gin  = new Double_t(25);
	Double_t *parameters = new Double_t(25);
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
			logfilestream.open( output_fname.c_str(), std::ios::out);  // std::ios::out will overwrite.
		} // it's open now.
	
		// do I need to do this?  .. yes, n_params (etc) probably aren't set yet.
		DumpCurrentFitParams();
		
		logfilestream << "Fit type:       " << current_fittype << endl;
		logfilestream << "Verbosity:      " << output_verbosity << endl;
		logfilestream << "n_fits:         " << n_fits << endl;
		logfilestream << "n_params:       " << n_params << endl;
		logfilestream << "n_fixed_params: " << n_fixed_params << endl;
		logfilestream << "n_free_params:  " << n_free_params << endl;
		logfilestream << "n_bins:         " << fit_bmax - fit_bmin + 1 << endl;
		logfilestream << "fit_bmin:       " << fit_bmin << endl;
		logfilestream << "fit_bmax:       " << fit_bmax << endl;
	//	logfilestream << "hist title:      " << hist1->GetTitle() << endl;  // GLOBAL hist1
		
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
	OutputHeader2();
	DumpToOutput();
	
	int this_paramnumber = 0;
	this_paramnumber = 0;
	for(int this_paramnumber = 0; this_paramnumber < n_params; this_paramnumber++)
	{
		logfilestream << "param:  " << this_paramnumber << "  " << (fit_parameters.at(this_paramnumber) ).GetName() << ":  ";
		logfilestream << fit_parameters[this_paramnumber].fit_val << " +/- " << fit_parameters[this_paramnumber].fit_err << ", f_pnum = " << fit_parameters[this_paramnumber].f_pnum << endl;
	}
	
	logfilestream << "fmin_best    = " << this -> get_fmin_best() << endl;
	logfilestream << "fedm_best    = " << this -> get_fedm_best() << endl;
	logfilestream << "errdef_best  = " << this -> get_errdef_best() << endl;

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


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //


