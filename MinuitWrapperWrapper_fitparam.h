#ifndef INCLUDE_FITPARAM
#define INCLUDE_FITPARAM 1


#include <TString.h>

//
class FitParameter
{
public:
	FitParameter()
	{
		TString name_       = "";
		double initial_val_ = 0.0;
		double step_        = 0.1;
		double min_         = 0.0;
		double max_         = 0.0;
		bool is_fixed_      = false;
		FitParameter(name_, initial_val_, step_, min_, max_, is_fixed_);
	};
	FitParameter(TString name_, double initial_val_, double step_)
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
	};
	FitParameter(TString name_, double initial_val_, double step_, double min_, double max_, bool is_fixed_=false)
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
	};
	
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
	
	TString name;
	TString GetName() {return name; };
	void SetName(TString newname) { name = TString(newname); };
	bool is_named(string possiblename) { return (bool)name.EqualTo(possiblename.c_str()); };
};

#endif
