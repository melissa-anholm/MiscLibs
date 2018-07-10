// ==================================================================== //
// Code by Melissa Anholm
// 28.9.2017 - 3.7.2018
// 
// ==================================================================== //

#include "MinuitWrapperWrapper3.cpp"

SuperMinuit      * global_minuit;  // Create a global instance.
combo_histfitter * global_histfitter;
TH1D             * global_thing1hist;

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
				tmp_bincontent = tmp_bincontent + parameters_[j]*(global_histfitter->histvect.at(j)->GetBinContent(i));
			}
			tmp_result = get_chi2_thisbin( global_histfitter->get_FitHist()->GetBinContent(i), 
				tmp_bincontent, global_histfitter->get_FitHist()->GetBinError(i), tmp_binerror);
			
			result_to_minimize_ = result_to_minimize_ + tmp_result;
		}
	}
	
	if(which_thing == histfitter_Ab)
	{
		double tmp_result = 0.0;
		double tmp_bincontent = 0.0;
		double tmp_binerror = 0.0;
		result_to_minimize_ = 0;
		
		int Abeta_num  = global_histfitter->get_paramnumber_by_name(string("Abeta"));
		int bFierz_num = global_histfitter->get_paramnumber_by_name(string("bFierz"));
		if( Abeta_num == -1 || bFierz_num == -1 )
		{
			cout << "BAD!!  Abeta_num = " << Abeta_num << ",\tbFierz_num = " << bFierz_num << endl;
			return 0;
		}
		for(int i=fit_bmin; i<=fit_bmax; i++)
		{
			tmp_binerror = 0.0;
			tmp_bincontent = parameters_[Abeta_num]*(global_histfitter->histvect.at(Abeta_num)->GetBinContent(i)) / (1.0 + parameters_[bFierz_num]*(global_histfitter->histvect.at(bFierz_num)->GetBinContent(i)) );
			
			tmp_result = get_chi2_thisbin( global_histfitter->get_FitHist()->GetBinContent(i), 
				tmp_bincontent, global_histfitter->get_FitHist()->GetBinError(i), tmp_binerror);
			result_to_minimize_ = result_to_minimize_ + tmp_result;
		}
	}
	
	if(which_thing == histfitter_asym && n_params==3)
	{
		int one_num    = global_histfitter->get_paramnumber_by_name(string("One"));
		int Abeta_num  = global_histfitter->get_paramnumber_by_name(string("Abeta"));
		int bFierz_num = global_histfitter->get_paramnumber_by_name(string("bFierz"));
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
	cout << "Initially, n_params = " << chf->get_n_params() << endl;
	
	which_thing = histfitter;
	chf -> SetupTheFitter_LinearCombo(); // returns true if hists all have the same binning.
	global_histfitter = chf;
	
	for(int i=0; i<chf->get_n_params(); i++)
	{
		this -> SetupParam(i, chf->paramvect.at(i) );
	}
	cout << "At the end, n_params = " << chf->get_n_params() << endl;
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
	sahf -> SetupTheFitter_Asym(); //
	which_thing = histfitter_asym;
}


// --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- // / // --- * --- //
