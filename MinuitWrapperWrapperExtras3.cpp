// ==================================================================== //
// Code by Melissa Anholm
// 28.9.2017 - 3.7.2018
//
// 'Extras' is where I put the functions that go with SuperMinuit 
// but do actual physics that one might want to change.  
// Otherwise, SuperMinuit itself just gets messy.
// ==================================================================== //


#include "MinuitWrapperWrapper3.cpp"
// 'MinuitWrapperWrapperExtras3.cpp' holds all the physics of the SuperMinuit fit functions.
//     'MinuitWrapperWrapperExtras3.cpp' includes 'MinuitWrapperWrapper3.cpp'
// 'MinuitWrapperWrapper3.cpp' holds the base class structure of the SuperMinuit.
//     'MinuitWrapperWrapper3.cpp' includes 'MinuitWrapperWrapperFitExtensions3.cpp'
// 'MinuitWrapperWrapperFitExtensions3.cpp' holds the combo_histfitter and superasym_histfitter classes.
//     'MinuitWrapperWrapperFitExtensions3.cpp' includes 'MinuitWrapperWrapper_fitparam.h'
// 'MinuitWrapperWrapper_fitparam.h' holds the FitParameter class.  It's safe to include anywhere.


SuperMinuit          * global_minuit;  // Create a global instance.
//combo_histfitter     * global_chf;
//superasym_histfitter * global_sahf;

enum 
{
	uninitialized   = 0,
	original        = 1,
	histfitter      = 2,
	histfitter_A    = 3,
	histfitter_Ab   = 4,
	histfitter_asym = 5
};


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
double get_chi2_thisbin(double h1_bincontent, double h2_bincontent, double h1_berr=0, double h2_berr=0)
{
	double combined_berr;
	if(h1_berr==0 && h2_berr==0) { combined_berr = 1.0; }
	else { combined_berr = sqrt(h1_berr*h1_berr + h2_berr*h2_berr); }
	
	double this_chi = 0.0;
	this_chi = (h1_bincontent - h2_bincontent)/combined_berr;
	
	return this_chi*this_chi;
}
void NonMemberFitFunction(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_)
{
	global_minuit -> MakeTheMinimizationGo(n_params_, gin_, result_to_minimize_, parameters_, ierflg_);
	
	global_minuit -> n_params_i         = n_params_;
	global_minuit -> gin                = gin_;
	global_minuit -> result_to_minimize = result_to_minimize_;
	global_minuit -> parameters         = parameters_;  // this doesn't really work??
	global_minuit -> ierflg             = ierflg_;
	
	return;
}

int SuperMinuit::MakeTheMinimizationGo(Int_t &n_params_, Double_t *gin_, Double_t &result_to_minimize_, Double_t *parameters_, Int_t ierflg_)
{
	
	result_to_minimize_ = 0;
	if(which_thing == uninitialized) {return 0;}
	if(which_thing == original && n_params==1)  // chi^2 fit to A_beta*v/c.  this is probably broken now...
	{
		/*
		double this_Abeta = parameters_[0];
		// hist1 is the original.
		// hist2 is the A*v/c histogram where we vary A.
	//	TH1D* this_h2 = makehist_A_v_over_c_like(this_Abeta, hist1);
		
		double tmp_result = 0.0;
	//	int n_bins = hist1->GetNbinsX();
	//	fit_bmin
		for(int i=fit_bmin; i<=fit_bmax; i++)
		{
	////		tmp_result =  get_chi2_thisbin(hist1->GetBinContent(i), parameters_[0]*hist2->GetBinContent(i), hist1->GetBinError(i), 0);
			result_to_minimize_ = result_to_minimize_ + tmp_result;
		}
		*/
		return 0;
	}
	
	if(which_thing == histfitter || (which_thing == histfitter_A && n_params==1) )
	{
		result_to_minimize_ = 0;
		double tmp_result = 0.0;
		double tmp_bincontent = 0.0;
		double tmp_binerror = 0.0;
		for(int i=fit_bmin; i<=fit_bmax; i++)
		{
			tmp_bincontent = 0.0;
			tmp_binerror = 0.0;
			for(int j=0; j<n_params; j++)
			{
				tmp_bincontent = tmp_bincontent + parameters_[j]*(the_chf->histvect.at(j)->GetBinContent(i));
			}
			tmp_result = get_chi2_thisbin( the_chf->get_FitHist()->GetBinContent(i), 
				tmp_bincontent, the_chf->get_FitHist()->GetBinError(i), tmp_binerror);
			
			result_to_minimize_ = result_to_minimize_ + tmp_result;
		}
	}
	
	if(which_thing == histfitter_Ab)
	{
		double tmp_result = 0.0;
		double tmp_bincontent = 0.0;
		double tmp_binerror = 0.0;
		result_to_minimize_ = 0;
		
	//	int Abeta_num  = the_chf->paramnumber_by_name(string("Abeta"));
	//	int bFierz_num = the_chf->paramnumber_by_name(string("bFierz"));
		int Abeta_num  = get_variable_paramnumber_by_name(string("Abeta"));
		int bFierz_num = get_variable_paramnumber_by_name(string("bFierz"));
		if( Abeta_num == -1 || bFierz_num == -1 )
		{
			cout << "BAD!!  Abeta_num = " << Abeta_num << ",\tbFierz_num = " << bFierz_num << endl;
			return 0;
		}
		for(int i=fit_bmin; i<=fit_bmax; i++)
		{
			tmp_binerror = 0.0;
			tmp_bincontent = parameters_[Abeta_num]*(the_chf->histvect.at(Abeta_num)->GetBinContent(i)) / (1.0 + parameters_[bFierz_num]*(the_chf->histvect.at(bFierz_num)->GetBinContent(i)) );
			
			tmp_result = get_chi2_thisbin( the_chf->get_FitHist()->GetBinContent(i), 
				tmp_bincontent, the_chf->get_FitHist()->GetBinError(i), tmp_binerror);
			result_to_minimize_ = result_to_minimize_ + tmp_result;
		}
	}
	
	if(which_thing == histfitter_asym) //  'n_params' gives the number of *non-fixed* params.
	{
	//	cout << "Calling the fitter we wanted..." << endl;
		
		double tmp_result = 0.0;
		result_to_minimize_ = 0;
		
		/*
		int One_num    = get_variable_paramnumber_by_name("One");  // this doesn't work if the parameter is fixed!!!  
		int Abeta_num  = get_variable_paramnumber_by_name("Abeta");
		int bFierz_num = get_variable_paramnumber_by_name("bFierz");
		
		double Abeta_value = parameters_[Abeta_num];
		double bFierz_value = parameters_[bFierz_num];
		double One_value = parameters_[One_num];
		*/
		
		DumpCurrentFitParams();
		double Abeta_value = get_parameter_byname("Abeta").fit_val;
		double bFierz_value = get_parameter_byname("bFierz").fit_val;
		double One_value = get_parameter_byname("One").fit_val;
		
	//	FitParameter this_fitparameter;
	//	if(fitpar.is_fixed)
		
	//	cout << "One_num    = " << One_num << endl;
	//	cout << "Abeta_num  = " << Abeta_num << endl;
	//	cout << "bFierz_num = " << bFierz_num << endl;
		
		// build a new asym hist using the current parameter values...
	//	the_sahf->the_chs->make_all_Ehists(parameters_[One_num], parameters_[Abeta_num], parameters_[bFierz_num]);
	//	the_sahf->the_chs->make_all_Ehists(1.0, parameters_[Abeta_num], parameters_[bFierz_num]);
		
		the_sahf->the_chs->make_all_Ehists(1.0, Abeta_value, bFierz_value);
		
	//	cout << "hists created with:  " << endl;
	//	cout << "\tparameters_[One_num]    = " << parameters_[One_num]    << endl;
	//	cout << "\tparameters_[Abeta_num]  = " << parameters_[Abeta_num]  << endl;
	//	cout << "\tparameters_[bFierz_num] = " << parameters_[bFierz_num] << endl;
		
		for(int i=fit_bmin; i<=fit_bmax; i++)
		{
			tmp_result = get_chi2_thisbin( 
				the_sahf->get_FitHist()->GetBinContent(i), 
				the_sahf->the_chs->h_asym->GetBinContent(i), 
				the_sahf->get_FitHist()->GetBinError(i), 
				the_sahf->the_chs->h_asym->GetBinError(i) );
				
		//	cout << "\ttmp_result=" << tmp_result << endl;
			result_to_minimize_ = result_to_minimize_ + tmp_result;
		//	parameters_[Abeta_num]*(the_chf->histvect.at(Abeta_num)->GetBinContent(i)) / (1.0 + parameters_[bFierz_num]*(the_chf->histvect.at(bFierz_num)->GetBinContent(i)) );
		}
	//	cout << "result_to_minimize_ = " << result_to_minimize_ << endl;
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


void SuperMinuit::SetupFCN_original(TH1D* hist_to_fit)
{
	which_thing = original;
	// this fit function basically doesn't do anything..
	// this -> TMinuit::SetFCN( NonMemberFitFunction ); 
};
void SuperMinuit::SetupFCN_histfitter( combo_histfitter * chf )
{
	cout << "Setting up the histfitter...." << endl;
	cout << "Initially, n_params (in chf) = " << chf->get_n_params() << endl;
	
	which_thing = histfitter;
	the_chf = chf;
	the_chf -> SetupTheFitter_LinearCombo(); // returns true if hists all have the same binning.
	
	for(int i=0; i<chf->get_n_params(); i++)
	{
		this -> SetupParam(i, chf->paramvect.at(i) );
	}
	cout << "At the end, n_params (in chf) = " << chf->get_n_params() << endl;
};
void SuperMinuit::SetupFCN_histfitter_A( combo_histfitter * chf )
{
	SetupFCN_histfitter(chf);
	which_thing = histfitter_A;
}
void SuperMinuit::SetupFCN_histfitter_Ab( combo_histfitter * chf )
{
	SetupFCN_histfitter(chf);
	which_thing = histfitter_Ab;
}

void SuperMinuit::SetupFCN_histfitter_superasym(superasym_histfitter * sahf)
{
	which_thing = histfitter_asym;
	the_sahf = sahf;
	// check if they have the same binning?
	bool same_binning = HistsHaveSameBinning(the_sahf->the_chs->make_all_Ehists(), the_sahf->get_FitHist() );
	
	if(!same_binning) 
	{
		cout << "ERROR!!  HISTS DON'T HAVE SAME BINNING!  (this is very bad...)" << endl;
	}
	
	cout << "Setting up the superasym_histfitter...." << endl;
	cout << "Initially, n_params = " << this->Get_n_params() << endl;

	this -> SetupParam(0, the_sahf->param_one);
	this -> SetupParam(1, the_sahf->param_A);
	this -> SetupParam(2, the_sahf->param_b);
	
	cout << "At the end, n_params  = " << this->Get_n_params() << endl;
}


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
