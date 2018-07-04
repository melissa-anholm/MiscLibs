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
#include "MinuitWrapperWrapper_fitparam.cpp"


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
// A Class to deal with TMinuit itself:
class SuperMinuit : virtual public TMinuit
{
public:
//	SuperMinuit() {};
	SuperMinuit():n_params(25), TMinuit(25) 
		{ init(); };
	SuperMinuit(int n):n_params(n), TMinuit(n) 
		{ init(); };

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
//	int histfitter_MemberFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_);
//	int which_thing;
	
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
	
//	combo_histfitter chf;
private:
	vector<FitParameter> fit_parameters;  // just for memory space...
	
	int fit_bmin;
	int fit_bmax;
//	int n_fitbins;
	
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

#endif