#ifndef INCLUDE_FITFITS
#define INCLUDE_FITFITS 1

#include "canvasplots.h"  // energy_specifier, set_of_fitvectors, set_of_fitresult_pointers.
#include "FitUtility.h"   // plotfuncs, relnorm_funcs.  (parameter settings)

extern TF1 * make_response_function();

double update_binmin(int N_rebin)  
// i=1 is the bin that starts at 0.  
// Set integration lower limit at the first bin border above 200.  This gives us the normalization for the whole PDF.  
{
	double binwidth = 6000.0 / (6000.0/((double)N_rebin));
	double integration_min = 200.0;
	double binmin = 200.0;
	for(int i=1; i<6000+1; i++)
	{
		if( (double)i * binwidth >= integration_min)
		{
			binmin = (double)i * binwidth;
			return binmin;
		}
	}
	cout << "ERROR!  binmin search failed!!" << endl;
	binmin = integration_min;
	return binmin;
};
double update_binmin(int N_rebin, double mono_int_keV)
{
	return update_binmin(N_rebin);
};
double update_binmax(int N_rebin, double mono_int_keV)
{
	double binwidth = 6000.0 / (6000.0/((double)N_rebin));
	
	double integration_max = mono_int_keV + 2000.0;
	double binmax = 1.0* binwidth;
	for(int i=1; i<6000+1; i++)
	{
		if( (double)i * binwidth > integration_max)  // this quits the for-loop.  check if we're out of bounds.
		{
			return binmax;
		}
		binmax = (double)i * binwidth;
		if(binmax>=6000.0)
		{
			binmax = 6000.0; 
			return binmax;
		}
	}
	cout << "ERROR!  binmax search failed!!  (binmax=" << binmax << ")" << endl;
	return binmax;
};


class set_of_fitfits
{
public:
	int color_tp = int(mOrange);
	int color_bp = int(mGreen);
	int color_tm = int(kAzure);
	int color_bm = int(kMagenta);
	
	double binmin;  // lower bound of the minimum bin that we use to integrate over.
	double binmax;  // upper bound of the maximum bin that we use to integrate over.  
	int N_rebin;
	double binwidth;
	
	void initialize()
	{
		initialize(1);
	};
	void initialize(int N)
	{
		integration_min = 200.0;
		integration_max = 6000.0;
		
		N_rebin = N;
		binwidth = 6000.0 / (6000.0/((double)N_rebin));
		
		update_integrallimits();
		
		fitparams_tp = plotfuncs();
		fitparams_bp = plotfuncs();
		fitparams_tm = plotfuncs();
		fitparams_bm = plotfuncs();
		fitparams_tp.load_default_parameters();
		fitparams_bp.load_default_parameters();
		fitparams_tm.load_default_parameters();
		fitparams_bm.load_default_parameters();
		
		normparams_tp = plotfuncs();
		normparams_bp = plotfuncs();
		normparams_tm = plotfuncs();
		normparams_bm = plotfuncs();
		normparams_tp.load_default_parameters();
		normparams_bp.load_default_parameters();
		normparams_tm.load_default_parameters();
		normparams_bm.load_default_parameters();

		TF1 * f = new TF1();
		f = make_relnorm_tp();
		relnorm_tp_ = *f;
		f = make_relnorm_tm();
		relnorm_tm_ = *f;
		f = make_relnorm_bp();
		relnorm_bp_ = *f;
		f = make_relnorm_bm();
		relnorm_bm_ = *f;
		//
		f = makedist_E0();
		f_E0_ = *f;
		f = makedist_W();
		f_W_ = *f;
		f = makedist_gres();
		f_gres_ = *f;
		f = makedist_DgE();
		f_DgE_ = *f;
		f = makedist_gamma_more();
		f_gamma_more_ = *f;
		f = makedist_gamma_less();
		f_gamma_less_ = *f;
		f = makedist_dE0();
		f_dE0_ = *f;
		f = makedist_k();
		f_k_ = *f;
		f = makedist_toeres_top();
		f_toeres_top_ = *f;
		f = makedist_toeres_bottom();
		f_toeres_bottom_ = *f;
		f = makedist_delta_top();
		f_delta_top_ = *f;
		f = makedist_delta_bottom();
		f_delta_bottom_ = *f;
		f = makedist_scale_more();
		f_scale_more_ = *f;
		f = makedist_scale_less();
		f_scale_less_ = *f;
		f = makedist_lres();
		f_lres_ = *f;
		f = makedist_gfrac();
		f_gfrac_ = *f;
		f = makedist_beta();
		f_beta_ = *f;
		f = makedist_alpha_more();
		f_alpha_more_ = *f;
		f = makedist_alpha_less();
		f_alpha_less_ = *f;
		f = makedist_norm_tp();
		f_norm_tp_ = *f;
		f = makedist_norm_bp();
		f_norm_bp_ = *f;
		f = makedist_norm_tm();
		f_norm_tm_ = *f;
		f = makedist_norm_bm();
		f_norm_bm_ = *f;
	};
	void generate_R_tp(double mono_int_keV)  // generate from the parameters we've loaded
	{
		update_integrallimits(mono_int_keV);
		
		double _00_E0, _01_norm, _02_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint;
		
		// tp:
		_00_E0     = f_E0_.Eval( mono_int_keV ) + mono_int_keV-300.0;
		_01_norm   = f_norm_tp_.Eval( mono_int_keV );       //
		_02_scale  = f_scale_less_.Eval( mono_int_keV );    //
		_03_alpha  = f_alpha_less_.Eval( mono_int_keV );    //
		_04_beta   = f_beta_.Eval( mono_int_keV );
		_05_gamma  = f_gamma_less_.Eval( mono_int_keV );
		_06_delta  = f_delta_top_.Eval( mono_int_keV );     //
		_07_W      = f_W_.Eval( mono_int_keV );
		_08_k      = f_k_.Eval( mono_int_keV );
		_09_gfrac  = f_gfrac_.Eval( mono_int_keV );
		_10_toeres = f_toeres_top_.Eval( mono_int_keV );    // 
		_11_lres   = f_lres_.Eval( mono_int_keV );
		_12_gres   = f_gres_.Eval( mono_int_keV );
		_13_DgE    = f_DgE_.Eval( mono_int_keV );
		_14_dE0    = f_dE0_.Eval( mono_int_keV );
		_15_countin  = 1;
		_16_countout = 1;
		_17_countint = 1;
		
		if( _01_norm <0 ) { _01_norm  = 0; }
		if( _06_delta<=0) { _06_delta = 1.0e-12; }
		
		fitparams_tp.set_params_and_propagate(_00_E0, _01_norm, _02_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint);

		double R_integral = fitparams_tp.R_total()->Integral(binmin, binmax);
		double new_norm  = _01_norm  * relnorm_tp_.Eval( mono_int_keV ) / R_integral;
		double new_scale = _02_scale * relnorm_tp_.Eval( mono_int_keV ) / R_integral;
		
	//	cout << "R_integral (tp) = " << R_integral << endl;
		normparams_tp.set_params_and_propagate(_00_E0, new_norm, new_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint);
		
		
		bool verbose=false;
		if(verbose)
		{
			cout << "generate_Rnorm_tp(mono_int_keV=" << mono_int_keV << "):  " << endl;
			cout << "\t_00_E0     = " << _00_E0 << endl;
			cout << "\t_01_norm   = " << _01_norm << endl;
			cout << "\t_02_scale  = " << _02_scale << endl;
			cout << "\t_03_alpha  = " << _03_alpha << endl;
			cout << "\t_04_beta   = " << _04_beta << endl;
			cout << "\t_05_gamma  = " << _05_gamma << endl;
			cout << "\t_06_delta  = " << _06_delta << endl;
			cout << "\t_07_W      = " << _07_W << endl;
			cout << "\t_08_k      = " << _08_k << endl;
			cout << "\t_09_gfrac  = " << _09_gfrac << endl;
			cout << "\t_10_toeres = " << _10_toeres << endl;
			cout << "\t_11_lres   = " << _11_lres << endl;
			cout << "\t_12_gres   = " << _12_gres << endl;
			cout << "\t_13_DgE    = " << _13_DgE << endl;
			cout << "\t_14_dE0    = " << _14_dE0 << endl;
		
			cout << "\t_15_countin  = " << _15_countin << endl;
			cout << "\t_16_countout = " << _16_countout << endl;
			cout << "\t_17_countint = " << _17_countint << endl;
		
			cout << "\tnew_norm  = " << new_norm << endl;
			cout << "\tnew_scale = " << new_scale << endl;
			cout << "\trelnorm   = " << relnorm_tp_.Eval( mono_int_keV ) << endl;
		}
	};
	void generate_R_bp(double mono_int_keV)  //
	{
		update_integrallimits(mono_int_keV);
		
		double _00_E0, _01_norm, _02_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint;
		
		// bp:
		_00_E0     = f_E0_.Eval( mono_int_keV ) + mono_int_keV-300.0;
		_01_norm   = f_norm_bp_.Eval( mono_int_keV );       //
		_02_scale  = f_scale_more_.Eval( mono_int_keV );    //
		_03_alpha  = f_alpha_more_.Eval( mono_int_keV );    //
		_04_beta   = f_beta_.Eval( mono_int_keV );
		_05_gamma  = f_gamma_more_.Eval( mono_int_keV );
		_06_delta  = f_delta_bottom_.Eval( mono_int_keV );     //
		_07_W      = f_W_.Eval( mono_int_keV );
		_08_k      = f_k_.Eval( mono_int_keV );
		_09_gfrac  = f_gfrac_.Eval( mono_int_keV );
		_10_toeres = f_toeres_bottom_.Eval( mono_int_keV );    // 
		_11_lres   = f_lres_.Eval( mono_int_keV );
		_12_gres   = f_gres_.Eval( mono_int_keV );
		_13_DgE    = f_DgE_.Eval( mono_int_keV );
		_14_dE0    = f_dE0_.Eval( mono_int_keV );
		_15_countin  = 1;
		_16_countout = 1;
		_17_countint = 1;
		
		if( _01_norm<0 )  { _01_norm=0; }
		if( _06_delta<=0) { _06_delta = 1.0e-12; }

		fitparams_bp.set_params_and_propagate(_00_E0, _01_norm, _02_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint);
		
		double R_integral = fitparams_bp.R_total()->Integral(binmin, binmax);
		double new_norm  = _01_norm  * relnorm_bp_.Eval( mono_int_keV ) / R_integral;
		double new_scale = _02_scale * relnorm_bp_.Eval( mono_int_keV ) / R_integral;
		
		normparams_bp.set_params_and_propagate(_00_E0, new_norm, new_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint);
	};
	void generate_R_tm(double mono_int_keV)  // 
	{
	//	integration_max = mono_int_keV + 2000.0;
		update_integrallimits(mono_int_keV);
		
		double _00_E0, _01_norm, _02_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint;
		
		// tm
		_00_E0     = f_E0_.Eval( mono_int_keV ) + mono_int_keV-300.0;
		_01_norm   = f_norm_tm_.Eval( mono_int_keV );       //
		_02_scale  = f_scale_more_.Eval( mono_int_keV );    //
		_03_alpha  = f_alpha_more_.Eval( mono_int_keV );    //
		_04_beta   = f_beta_.Eval( mono_int_keV );
	//	_05_gamma  = f_gamma_.Eval( mono_int_keV );
		_05_gamma  = f_gamma_more_.Eval( mono_int_keV );
		_06_delta  = f_delta_top_.Eval( mono_int_keV );     //
		_07_W      = f_W_.Eval( mono_int_keV );
		_08_k      = f_k_.Eval( mono_int_keV );
		_09_gfrac  = f_gfrac_.Eval( mono_int_keV );
		_10_toeres = f_toeres_top_.Eval( mono_int_keV );    // 
		_11_lres   = f_lres_.Eval( mono_int_keV );
		_12_gres   = f_gres_.Eval( mono_int_keV );
		_13_DgE    = f_DgE_.Eval( mono_int_keV );
		_14_dE0    = f_dE0_.Eval( mono_int_keV );
		_15_countin  = 1;
		_16_countout = 1;
		_17_countint = 1;
		
		if( _01_norm<0 )  { _01_norm=0; }
		if( _06_delta<=0) { _06_delta = 1.0e-12; }
		
		fitparams_tm.set_params_and_propagate(_00_E0, _01_norm, _02_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint);
		
		double R_integral = fitparams_tm.R_total()->Integral(binmin, binmax);
		double new_norm  = _01_norm  * relnorm_tm_.Eval( mono_int_keV ) / R_integral;
		double new_scale = _02_scale * relnorm_tm_.Eval( mono_int_keV ) / R_integral;
		
	//	cout << "R_integral (tm) = " << R_integral << endl;
		normparams_tm.set_params_and_propagate(_00_E0, new_norm, new_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint);
	};
	void generate_R_bm(double mono_int_keV)  // 
	{
	//	integration_max = mono_int_keV + 2000.0;
		update_integrallimits(mono_int_keV);
		
		double _00_E0, _01_norm, _02_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint;
		
		// bm
		_00_E0     = f_E0_.Eval( mono_int_keV ) + mono_int_keV-300.0;
		_01_norm   = f_norm_bm_.Eval( mono_int_keV );       //
		_02_scale  = f_scale_less_.Eval( mono_int_keV );    //
		_03_alpha  = f_alpha_less_.Eval( mono_int_keV );    //
		_04_beta   = f_beta_.Eval( mono_int_keV );
	//	_05_gamma  = f_gamma_.Eval( mono_int_keV );
		_05_gamma  = f_gamma_less_.Eval( mono_int_keV );
		_06_delta  = f_delta_bottom_.Eval( mono_int_keV );     //
		_07_W      = f_W_.Eval( mono_int_keV );
		_08_k      = f_k_.Eval( mono_int_keV );
		_09_gfrac  = f_gfrac_.Eval( mono_int_keV );
		_10_toeres = f_toeres_bottom_.Eval( mono_int_keV );    // 
		_11_lres   = f_lres_.Eval( mono_int_keV );
		_12_gres   = f_gres_.Eval( mono_int_keV );
		_13_DgE    = f_DgE_.Eval( mono_int_keV );
		_14_dE0    = f_dE0_.Eval( mono_int_keV );
		_15_countin  = 1;
		_16_countout = 1;
		_17_countint = 1;
		
		if( _01_norm<0 )  { _01_norm=0; }
		if( _06_delta<=0) { _06_delta = 1.0e-12; }
		
		fitparams_bm.set_params_and_propagate(_00_E0, _01_norm, _02_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint);
		
		double R_integral = fitparams_bm.R_total()->Integral(binmin, binmax);
		double new_norm  = _01_norm  * relnorm_bm_.Eval( mono_int_keV ) / R_integral;
		double new_scale = _02_scale * relnorm_bm_.Eval( mono_int_keV ) / R_integral;
		
	//	cout << "R_integral (bm) = " << R_integral << endl;
		normparams_bm.set_params_and_propagate(_00_E0, new_norm, new_scale, _03_alpha, _04_beta, _05_gamma, _06_delta, \
		_07_W, _08_k, _09_gfrac, _10_toeres, _11_lres, _12_gres, _13_DgE, _14_dE0, _15_countin, _16_countout, _17_countint);
		
		/*
		cout << "generate_Rnorm_bm(mono_int_keV=" << mono_int_keV << "):  " << endl;
		cout << "\t_00_E0     = " << _00_E0 << endl;
		cout << "\t_01_norm   = " << _01_norm << endl;
		cout << "\t_02_scale  = " << _02_scale << endl;
		cout << "\t_03_alpha  = " << _03_alpha << endl;
		cout << "\t_04_beta   = " << _04_beta << endl;
		cout << "\t_05_gamma  = " << _05_gamma << endl;
		cout << "\t_06_delta  = " << _06_delta << endl;
		cout << "\t_07_W      = " << _07_W << endl;
		cout << "\t_08_k      = " << _08_k << endl;
		cout << "\t_09_gfrac  = " << _09_gfrac << endl;
		cout << "\t_10_toeres = " << _10_toeres << endl;
		cout << "\t_11_lres   = " << _11_lres << endl;
		cout << "\t_12_gres   = " << _12_gres << endl;
		cout << "\t_13_DgE    = " << _13_DgE << endl;
		cout << "\t_14_dE0    = " << _14_dE0 << endl;
		
		cout << "\t_15_countin  = " << _15_countin << endl;
		cout << "\t_16_countout = " << _16_countout << endl;
		cout << "\t_17_countint = " << _17_countint << endl;
		
		cout << "\tnew_norm  = " << new_norm << endl;
		cout << "\tnew_scale = " << new_scale << endl;
		*/
	};
	void make_R_funcs(double mono_int_keV)
	{
		TF1 * f = new TF1();
		
		generate_R_tp(mono_int_keV);
		generate_R_bp(mono_int_keV);
		generate_R_tm(mono_int_keV);
		generate_R_bm(mono_int_keV);
		return;
	};
	
	plotfuncs fitparams_tp;
	plotfuncs fitparams_bp;
	plotfuncs fitparams_tm;
	plotfuncs fitparams_bm;

	plotfuncs normparams_tp;
	plotfuncs normparams_bp;
	plotfuncs normparams_tm;
	plotfuncs normparams_bm;
	
public:
	TF1 * relnorm_tp() { return &relnorm_tp_; };
	TF1 * relnorm_bp() { return &relnorm_bp_; };
	TF1 * relnorm_tm() { return &relnorm_tm_; };
	TF1 * relnorm_bm() { return &relnorm_bm_; };

	TF1 * Rnorm_tp() { return normparams_tp.R_total(); };
	TF1 * Rnorm_bp() { return normparams_bp.R_total(); };
	TF1 * Rnorm_tm() { return normparams_tm.R_total(); };
	TF1 * Rnorm_bm() { return normparams_bm.R_total(); };

	TF1 * R_tp() { return fitparams_tp.R_total(); };
	TF1 * R_bp() { return fitparams_bp.R_total(); };
	TF1 * R_tm() { return fitparams_tm.R_total(); };
	TF1 * R_bm() { return fitparams_bm.R_total(); };
	
	void update_integrallimits(double mono_int_keV)
	{
		integration_max = mono_int_keV + 2000.0;
		update_integrallimits();
	};
	void update_integrallimits()  // assumes integration_min, integration_max, and N_rebin are all set to nominal values already.  and binwidth.
	{
		// zeroth try:
		binmin = integration_min;
		binmax = integration_max;
		
		// we want to use all of the first bin with its centroid above (or at) integration_min.
		this->update_binmin();
		// use all of the last bin with its centroid below (or at) integration_max.
		this->update_binmax();
	};
	double update_binmax()
	{
		// integration_max = mono_int_keV + 2000.0;
		binmax = 1.0* binwidth;// + 0.5*binwidth;
		for(int i=1; i<6000+1; i++)
		{
			if( (double)i * binwidth > integration_max)  // this quits the for-loop.  check if we're out of bounds.
			{
				return binmax;
			}
			binmax = (double)i * binwidth;// + 0.5*binwidth;
			if(binmax>6000.0)
			{
				binmax = 6000.0; 
				return binmax;
			}
		}
		cout << "ERROR!  binmax search failed!!  (binmax=" << binmax << ")" << endl;
		return binmax;
	}
	double update_binmin()
	{
		for(int i=1; i<6000+1; i++)
		{
			if( (double)i * binwidth >= integration_min)
			{
				binmin = (double)i * binwidth;// - 0.5*binwidth;
				return binmin;
			}
		}
		cout << "ERROR!  binmin search failed!!" << endl;
		binmin = integration_min;
		return binmin;
	};
private:
	TF1 relnorm_tp_;
	TF1 relnorm_tm_;
	TF1 relnorm_bp_;
	TF1 relnorm_bm_;
	
	TF1 f_E0_;
	TF1 f_norm_tp_;
	TF1 f_norm_bp_;
	TF1 f_norm_tm_;
	TF1 f_norm_bm_;
	TF1 f_scale_more_;
	TF1 f_scale_less_;
	TF1 f_alpha_more_;
	TF1 f_alpha_less_;
	TF1 f_beta_;
	TF1 f_gamma_more_;
	TF1 f_gamma_less_;
	TF1 f_delta_top_;
	TF1 f_delta_bottom_;

	TF1 f_W_;
	TF1 f_k_;
	TF1 f_gfrac_;
	TF1 f_toeres_top_;
	TF1 f_toeres_bottom_;
	TF1 f_lres_;
	TF1 f_gres_;
	
	TF1 f_DgE_;
	TF1 f_dE0_;
	
	double integration_min;
	double integration_max;
};






// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
class set_of_distributions
{
public:
	set_of_distributions( int N )
	{
		kludge_dE = 350.0;
		//
		N_rebin = N;
		//
		h_tmp = CreateHist( string("tmp hist"), string("Mapping_Ebeta"), int(kBlack), N_rebin);
		N_bins = h_tmp -> GetNbinsX();
		binwidth = h_tmp->GetBinCenter(6) - h_tmp->GetBinCenter(5);
		//
		h_pdf_out_color  = kRed;
		h_cdf_out_color  = kBlue;
		ih_cdf_out_color = int(mGreen);
		reE_hcdf_out_color = kOrange;
	};
	/*
	~set_of_distributions()
	{
		delete h_reconstructed_pdf_tp;
		delete h_reconstructed_h_cdf_tp;
		delete h_reconstructed_ih_cdf_tp;
	
		delete h_reconstructed_pdf_bp;
		delete h_reconstructed_h_cdf_bp;
		delete h_reconstructed_ih_cdf_bp;
	
		delete h_reconstructed_pdf_tm;
		delete h_reconstructed_h_cdf_tm;
		delete h_reconstructed_ih_cdf_tm;

		delete h_reconstructed_pdf_bm;
		delete h_reconstructed_h_cdf_bm;
		delete h_reconstructed_ih_cdf_bm;
	
		delete h_rewithE_hcdf_tp;
		delete h_rewithE_hcdf_bp;
		delete h_rewithE_hcdf_tm;
		delete h_rewithE_hcdf_bm;
		
		delete h_tmp;
		
		for(int i=0; i<f_pdf_tp.size(); i++)
		{
			delete f_pdf_tp.at(i);
			delete f_pdf_bp.at(i);
			delete f_pdf_tm.at(i);
			delete f_pdf_bm.at(i);

			delete h_pdf_tp.at(i);
			delete h_pdf_bp.at(i);
			delete h_pdf_tm.at(i);
			delete h_pdf_bm.at(i);
			
			delete ih_cdf_tp.at(i);
			delete ih_cdf_bp.at(i);
			delete ih_cdf_tm.at(i);
			delete ih_cdf_bm.at(i);
			
			delete h_cdf_tp.at(i);
			delete h_cdf_bp.at(i);
			delete h_cdf_tm.at(i);
			delete h_cdf_bm.at(i);
		}
		
		f_pdf_tp.clear();
		f_pdf_bp.clear();
		f_pdf_tm.clear();
		f_pdf_bm.clear();
	
		h_pdf_tp.clear();
		h_pdf_bp.clear();
		h_pdf_tm.clear();
		h_pdf_bm.clear();
	
		ih_cdf_tp.clear();
		ih_cdf_bp.clear();
		ih_cdf_tm.clear();
		ih_cdf_bm.clear();
	
		h_cdf_tp.clear();
		h_cdf_bp.clear();
		h_cdf_tm.clear();
		h_cdf_bm.clear();
		
		file_we_loaded_from->Close();
		delete file_we_loaded_from;
	};
	*/
	
	void create()
	{
		// delete the histograms and tf1s if they're already in existence?
		f_pdf_tp.clear();
		f_pdf_bp.clear();
		f_pdf_tm.clear();
		f_pdf_bm.clear();
		
		h_pdf_tp.clear();
		h_pdf_bp.clear();
		h_pdf_tm.clear();
		h_pdf_bm.clear();
		
		f_pdf_tp.clear();
		f_pdf_bp.clear();
		f_pdf_tm.clear();
		f_pdf_bm.clear();
		
		ih_cdf_tp.clear();
		ih_cdf_bp.clear();
		ih_cdf_tm.clear();
		ih_cdf_bm.clear();
		
		// Now create a new set with the correct length.
		for (int i=0; i<N_bins+1; i++)
		{
			int the_E_bin = h_tmp->GetBinCenter(i);
			string E_string = int_to_string(the_E_bin);
			
			h_pdf_tp.push_back( CreateHist( string("hist PDF (T+), E=")+E_string, string("Mapping_Ebeta"), h_pdf_out_color, N_rebin) );
			h_pdf_bp.push_back( CreateHist( string("hist PDF (B+), E=")+E_string, string("Mapping_Ebeta"), h_pdf_out_color, N_rebin) );
			h_pdf_tm.push_back( CreateHist( string("hist PDF (T-), E=")+E_string, string("Mapping_Ebeta"), h_pdf_out_color, N_rebin) );
			h_pdf_bm.push_back( CreateHist( string("hist PDF (B-), E=")+E_string, string("Mapping_Ebeta"), h_pdf_out_color, N_rebin) );
		
			h_cdf_tp.push_back( CreateHist( string("hist CDF (T+), E=")+E_string, string("Mapping_Ebeta"), h_cdf_out_color, N_rebin) );
			h_cdf_bp.push_back( CreateHist( string("hist CDF (B+), E=")+E_string, string("Mapping_Ebeta"), h_cdf_out_color, N_rebin) );
			h_cdf_tm.push_back( CreateHist( string("hist CDF (T-), E=")+E_string, string("Mapping_Ebeta"), h_cdf_out_color, N_rebin) );
			h_cdf_bm.push_back( CreateHist( string("hist CDF (B-), E=")+E_string, string("Mapping_Ebeta"), h_cdf_out_color, N_rebin) );
		
			f_pdf_tp.push_back( new TF1() );  // gets a new name later anyway.
			f_pdf_bp.push_back( new TF1() );
			f_pdf_tm.push_back( new TF1() );
			f_pdf_bm.push_back( new TF1() );
			
			ih_cdf_tp.push_back( CreateHist( string("Direct CDF (T+), E=")+E_string, string("Mapping_Ebeta"), ih_cdf_out_color, N_rebin) );
			ih_cdf_bp.push_back( CreateHist( string("Direct CDF (B+), E=")+E_string, string("Mapping_Ebeta"), ih_cdf_out_color, N_rebin) );
			ih_cdf_tm.push_back( CreateHist( string("Direct CDF (T-), E=")+E_string, string("Mapping_Ebeta"), ih_cdf_out_color, N_rebin) );
			ih_cdf_bm.push_back( CreateHist( string("Direct CDF (B-), E=")+E_string, string("Mapping_Ebeta"), ih_cdf_out_color, N_rebin) );
		}
		
		// Probably don't need to create new individual histograms, but I guess we might as well...
		h_reconstructed_pdf_tp    = new TH1D();
		h_reconstructed_h_cdf_tp  = new TH1D();
		h_reconstructed_ih_cdf_tp = new TH1D();
		
		h_reconstructed_pdf_bp    = new TH1D();
		h_reconstructed_h_cdf_bp  = new TH1D();
		h_reconstructed_ih_cdf_bp = new TH1D();
		
		h_reconstructed_pdf_tm    = new TH1D();
		h_reconstructed_h_cdf_tm  = new TH1D();
		h_reconstructed_ih_cdf_tm = new TH1D();
		
		h_reconstructed_pdf_bm    = new TH1D();
		h_reconstructed_h_cdf_bm  = new TH1D();
		h_reconstructed_ih_cdf_bm = new TH1D();
		
		TH1D* h_rewithE_hcdf_tp = CreateHist( string("CDF Reconstructed Re-Added Upper Energy(+)"), string("Mapping_Ebeta"), int(reE_hcdf_out_color), N_rebin);
		TH1D* h_rewithE_hcdf_bp = CreateHist( string("CDF Reconstructed Re-Added Lower Energy(+)"), string("Mapping_Ebeta"), int(reE_hcdf_out_color), N_rebin);
		TH1D* h_rewithE_hcdf_tm = CreateHist( string("CDF Reconstructed Re-Added Upper Energy(-)"), string("Mapping_Ebeta"), int(reE_hcdf_out_color), N_rebin);
		TH1D* h_rewithE_hcdf_bm = CreateHist( string("CDF Reconstructed Re-Added Lower Energy(-)"), string("Mapping_Ebeta"), int(reE_hcdf_out_color), N_rebin);
	};
	void initialize_distributions()
	{
		set_of_fitfits * my_fitfits = new set_of_fitfits();
		my_fitfits -> initialize(N_rebin);
		
		double E_in_keV;
		double other_E; 
		for (int i=1; i<N_bins+1; i++)  // just skip the zeroth bin.
		{
			E_in_keV = h_tmp->GetBinCenter(i);
			string E_string = int_to_string(E_in_keV);
			my_fitfits->make_R_funcs(E_in_keV);  // re-evaluates integration limits too.
			
			TF1 * Rnorm_tp = my_fitfits->Rnorm_tp();
			TF1 * Rnorm_bp = my_fitfits->Rnorm_bp();
			TF1 * Rnorm_tm = my_fitfits->Rnorm_tm();
			TF1 * Rnorm_bm = my_fitfits->Rnorm_bm();
			
			f_pdf_tp.at(i) = (TF1*)Rnorm_tp->Clone( (string("PDF (T+), E=")+E_string).c_str() );
			f_pdf_bp.at(i) = (TF1*)Rnorm_bp->Clone( (string("PDF (B+), E=")+E_string).c_str() );
			f_pdf_tm.at(i) = (TF1*)Rnorm_tm->Clone( (string("PDF (T-), E=")+E_string).c_str() );
			f_pdf_bm.at(i) = (TF1*)Rnorm_bm->Clone( (string("PDF (B-), E=")+E_string).c_str() );
			
			f_pdf_tp.at(i) -> SetTitle( f_pdf_tp.at(i)->GetName() );
			f_pdf_bp.at(i) -> SetTitle( f_pdf_bp.at(i)->GetName() );
			f_pdf_tm.at(i) -> SetTitle( f_pdf_tm.at(i)->GetName() );
			f_pdf_bm.at(i) -> SetTitle( f_pdf_bm.at(i)->GetName() );
		}
		// f_pdfs are loaded...
		
		// load the pdf hists too:
		for (int i=1; i<N_bins+1; i++)  // just skip the zeroth bin.
		{
			E_in_keV = h_tmp->GetBinCenter(i);
			for (int j=1; j<N_bins+1; j++)
			{
				other_E = h_tmp->GetBinCenter(j);
				if(other_E > E_in_keV+2000) { continue; }
				
				h_pdf_tp.at(i) -> Fill(other_E, f_pdf_tp.at(i)->Eval(other_E) * binwidth);
				h_pdf_bp.at(i) -> Fill(other_E, f_pdf_bp.at(i)->Eval(other_E) * binwidth);
				h_pdf_tm.at(i) -> Fill(other_E, f_pdf_tm.at(i)->Eval(other_E) * binwidth);
				h_pdf_bm.at(i) -> Fill(other_E, f_pdf_bm.at(i)->Eval(other_E) * binwidth);
			}
		}
		// load the CDFs (from the hist PDFs):
		double prev_tp, prev_bp, prev_tm, prev_bm;
		for (int i=1; i<N_bins+1; i++)
		{
			E_in_keV = h_tmp->GetBinCenter(i);
			for (int j=1; j<N_bins+1; j++)
			{
				double other_E = h_tmp->GetBinCenter(j);
				
				prev_tp = h_pdf_tp.at(i)->GetBinContent(j);
				prev_bp = h_pdf_bp.at(i)->GetBinContent(j);
				prev_tm = h_pdf_tm.at(i)->GetBinContent(j);
				prev_bm = h_pdf_bm.at(i)->GetBinContent(j);
			
				h_cdf_tp.at(i) -> Fill(other_E, prev_tp + h_cdf_tp.at(i)->GetBinContent(j-1) );
				h_cdf_bp.at(i) -> Fill(other_E, prev_bp + h_cdf_bp.at(i)->GetBinContent(j-1) );
				h_cdf_tm.at(i) -> Fill(other_E, prev_tm + h_cdf_tm.at(i)->GetBinContent(j-1) );
				h_cdf_bm.at(i) -> Fill(other_E, prev_bm + h_cdf_bm.at(i)->GetBinContent(j-1) );
			}
		}
		
		// load the integrated CDFs?
		double integral_tp, integral_bp, integral_tm, integral_bm;
		double xmin = 0;
		double xmax;
		for (int i=1; i<N_bins+1; i++)
		{
			E_in_keV = h_tmp->GetBinCenter(i);
			for (int j=1; j<N_bins+1; j++)
			{
				double other_E = h_tmp->GetBinCenter(j);
				if(other_E <= xmin)
				{
					integral_tp = 0;
					integral_bp = 0;
					integral_tm = 0;
					integral_bm = 0;
				}
				else
				{
					integral_tp = f_pdf_tp.at(i)->Integral(xmin, other_E);
					integral_bp = f_pdf_bp.at(i)->Integral(xmin, other_E);
					integral_tm = f_pdf_tm.at(i)->Integral(xmin, other_E);
					integral_bm = f_pdf_bm.at(i)->Integral(xmin, other_E);
				}
				//
				ih_cdf_tp.at(i) -> Fill(other_E, integral_tp);
				ih_cdf_bp.at(i) -> Fill(other_E, integral_bp);
				ih_cdf_tm.at(i) -> Fill(other_E, integral_tm);
				ih_cdf_bm.at(i) -> Fill(other_E, integral_bm);
			}
		}
		
		// tweak appearances..
		tweak_colors();
	};
	
	void tweak_colors()
	{
		for (int i=1; i<N_bins+1; i++)
		{
			h_pdf_tp.at(i)->SetMarkerColor(h_pdf_out_color);
			h_pdf_bp.at(i)->SetMarkerColor(h_pdf_out_color);
			h_pdf_tm.at(i)->SetMarkerColor(h_pdf_out_color);
			h_pdf_bm.at(i)->SetMarkerColor(h_pdf_out_color);
			h_pdf_tp.at(i)->SetLineColor(h_pdf_out_color);
			h_pdf_bp.at(i)->SetLineColor(h_pdf_out_color);
			h_pdf_tm.at(i)->SetLineColor(h_pdf_out_color);
			h_pdf_bm.at(i)->SetLineColor(h_pdf_out_color);
			
			h_cdf_tp.at(i)->SetMarkerColor(h_cdf_out_color);
			h_cdf_bp.at(i)->SetMarkerColor(h_cdf_out_color);
			h_cdf_tm.at(i)->SetMarkerColor(h_cdf_out_color);
			h_cdf_bm.at(i)->SetMarkerColor(h_cdf_out_color);
			h_cdf_tp.at(i)->SetLineColor(h_cdf_out_color);
			h_cdf_bp.at(i)->SetLineColor(h_cdf_out_color);
			h_cdf_tm.at(i)->SetLineColor(h_cdf_out_color);
			h_cdf_bm.at(i)->SetLineColor(h_cdf_out_color);
			
			ih_cdf_tp.at(i)->SetMarkerColor(ih_cdf_out_color);
			ih_cdf_bp.at(i)->SetMarkerColor(ih_cdf_out_color);
			ih_cdf_tm.at(i)->SetMarkerColor(ih_cdf_out_color);
			ih_cdf_bm.at(i)->SetMarkerColor(ih_cdf_out_color);
			ih_cdf_tp.at(i)->SetLineColor(ih_cdf_out_color);
			ih_cdf_bp.at(i)->SetLineColor(ih_cdf_out_color);
			ih_cdf_tm.at(i)->SetLineColor(ih_cdf_out_color);
			ih_cdf_bm.at(i)->SetLineColor(ih_cdf_out_color);
		}
		// turn off sumw2's !
		for(int i=0; i<N_bins+1; i++)
		{
			h_pdf_tp.at(i)->Sumw2(false);
			h_pdf_bp.at(i)->Sumw2(false);
			h_pdf_tm.at(i)->Sumw2(false);
			h_pdf_bm.at(i)->Sumw2(false);
			
			h_cdf_tp.at(i)->Sumw2(false);
			h_cdf_bp.at(i)->Sumw2(false);
			h_cdf_tm.at(i)->Sumw2(false);
			h_cdf_bm.at(i)->Sumw2(false);
			
			ih_cdf_tp.at(i)->Sumw2(false);
			ih_cdf_bp.at(i)->Sumw2(false);
			ih_cdf_tm.at(i)->Sumw2(false);
			ih_cdf_bm.at(i)->Sumw2(false);
		}
	};
	void do_reconstructions_tp(TH1D * h_naive)
	{
		h_reconstructed_pdf_tp    = reconstruct_tp_from_pdf(h_naive);
		reconstruct_tp_from_cdf(h_naive);
	//	cout << "adding back TP energy:" << endl;
	//	addback_E_tp_from_hcdf();  // currently it's a kludge.
	};
	void do_reconstructions_bp(TH1D * h_naive)
	{
		h_reconstructed_pdf_bp    = reconstruct_bp_from_pdf(h_naive);
		reconstruct_bp_from_cdf(h_naive);
	//	addback_E_bp_from_hcdf();  // currently it's a kludge.
	};
	void do_reconstructions_tm(TH1D * h_naive)
	{
		h_reconstructed_pdf_tm    = reconstruct_tm_from_pdf(h_naive);
		reconstruct_tm_from_cdf(h_naive);
	//	addback_E_tm_from_hcdf();  // currently it's a kludge.
	};
	void do_reconstructions_bm(TH1D * h_naive)
	{
		h_reconstructed_pdf_bm    = reconstruct_bm_from_pdf(h_naive);
		reconstruct_bm_from_cdf(h_naive);
	//	addback_E_bm_from_hcdf();  // currently it's a kludge.
	};
	
	void save_to_file(string filename) 
	{
		TFile * f = new TFile(filename.c_str(), "RECREATE");
		f->cd();
		for (int i=0; i<N_bins+1; i++)
		{
			f_pdf_tp.at(i) -> Write("",TObject::kOverwrite);
			f_pdf_bp.at(i) -> Write("",TObject::kOverwrite);
			f_pdf_tm.at(i) -> Write("",TObject::kOverwrite);
			f_pdf_bm.at(i) -> Write("",TObject::kOverwrite);
		}
		for (int i=0; i<N_bins+1; i++)
		{
			h_pdf_tp.at(i) -> Write("",TObject::kOverwrite);
			h_pdf_bp.at(i) -> Write("",TObject::kOverwrite);
			h_pdf_tm.at(i) -> Write("",TObject::kOverwrite);
			h_pdf_bm.at(i) -> Write("",TObject::kOverwrite);
		}
		for (int i=0; i<N_bins+1; i++)
		{
			h_cdf_tp.at(i) -> Write("",TObject::kOverwrite);
			h_cdf_bp.at(i) -> Write("",TObject::kOverwrite);
			h_cdf_tm.at(i) -> Write("",TObject::kOverwrite);
			h_cdf_bm.at(i) -> Write("",TObject::kOverwrite);
		}
		for (int i=0; i<N_bins+1; i++)
		{
			ih_cdf_tp.at(i) -> Write("",TObject::kOverwrite);
			ih_cdf_bp.at(i) -> Write("",TObject::kOverwrite);
			ih_cdf_tm.at(i) -> Write("",TObject::kOverwrite);
			ih_cdf_bm.at(i) -> Write("",TObject::kOverwrite);
		}
		f->Close();
	};
	void load_from_file(string filename) // must have already called set_of_distributions( int N ) to set N_rebin.
	{
		this->create();
		// get binning info right out of the file.  I think.
		
		// int N_rebin; 
		// double binwidth;
		// int N_bins;

		
		file_we_loaded_from = new TFile(filename.c_str());
		if(file_we_loaded_from->IsZombie())
		{
			cout << "No TFile " << filename << " could be found." << endl;
			cout << "Hard kill." << endl;
			assert(0);
		}
		string E_string;
		string name_tp, name_bp, name_tm, name_bm;
		double E_in_keV;
		// hist pdfs:
		for (int i=0; i<N_bins+1; i++)
		{
			E_in_keV = h_tmp->GetBinCenter(i);
			E_string = int_to_string(E_in_keV);
			//
			name_tp = string("hist PDF (T+), E=")+E_string;
			name_bp = string("hist PDF (B+), E=")+E_string;
			name_tm = string("hist PDF (T-), E=")+E_string;
			name_bm = string("hist PDF (B-), E=")+E_string;
			
			h_pdf_tp.at(i) = (TH1D*)file_we_loaded_from->Get( name_tp.c_str() );
			h_pdf_bp.at(i) = (TH1D*)file_we_loaded_from->Get( name_bp.c_str() );
			h_pdf_tm.at(i) = (TH1D*)file_we_loaded_from->Get( name_tm.c_str() );
			h_pdf_bm.at(i) = (TH1D*)file_we_loaded_from->Get( name_bm.c_str() );
		}
		// hist cdfs:
		for (int i=0; i<N_bins+1; i++)
		{
			E_in_keV = h_tmp->GetBinCenter(i);
			E_string = int_to_string(E_in_keV);
			//
			name_tp = string("hist CDF (T+), E=")+E_string;
			name_bp = string("hist CDF (B+), E=")+E_string;
			name_tm = string("hist CDF (T-), E=")+E_string;
			name_bm = string("hist CDF (B-), E=")+E_string;
			
			h_cdf_tp.at(i) = (TH1D*)file_we_loaded_from->Get( name_tp.c_str() );
			h_cdf_bp.at(i) = (TH1D*)file_we_loaded_from->Get( name_bp.c_str() );
			h_cdf_tm.at(i) = (TH1D*)file_we_loaded_from->Get( name_tm.c_str() );
			h_cdf_bm.at(i) = (TH1D*)file_we_loaded_from->Get( name_bm.c_str() );
		}
		// analytic pdfs:
		for (int i=0; i<N_bins+1; i++)
		{
			E_in_keV = h_tmp->GetBinCenter(i);
			E_string = int_to_string(E_in_keV);
			//
			name_tp = string("PDF (T+), E=")+E_string;
			name_bp = string("PDF (B+), E=")+E_string;
			name_tm = string("PDF (T-), E=")+E_string;
			name_bm = string("PDF (B-), E=")+E_string;
			
			f_pdf_tp.at(i) = (TF1*)file_we_loaded_from->Get( name_tp.c_str() );
			f_pdf_bp.at(i) = (TF1*)file_we_loaded_from->Get( name_bp.c_str() );
			f_pdf_tm.at(i) = (TF1*)file_we_loaded_from->Get( name_tm.c_str() );
			f_pdf_bm.at(i) = (TF1*)file_we_loaded_from->Get( name_bm.c_str() );
		}
		
		// direct-integrated cdfs:
		for (int i=0; i<N_bins+1; i++)
		{
			E_in_keV = h_tmp->GetBinCenter(i);
			E_string = int_to_string(E_in_keV);
			//
			name_tp = string("Direct CDF (T+), E=")+E_string;
			name_bp = string("Direct CDF (B+), E=")+E_string;
			name_tm = string("Direct CDF (T-), E=")+E_string;
			name_bm = string("Direct CDF (B-), E=")+E_string;
			
			ih_cdf_tp.at(i) = (TH1D*)file_we_loaded_from->Get( name_tp.c_str() );
			ih_cdf_bp.at(i) = (TH1D*)file_we_loaded_from->Get( name_bp.c_str() );
			ih_cdf_tm.at(i) = (TH1D*)file_we_loaded_from->Get( name_tm.c_str() );
			ih_cdf_bm.at(i) = (TH1D*)file_we_loaded_from->Get( name_bm.c_str() );
		}
		
		// can I close f again, or should I just not?  ... don't, I think.
	};
	
	//  -- * --  //
	TH1D * reconstruct_tp_from_pdf(TH1D * h_naive)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("PDF Reconstructed Upper Energy(+)"), string("Mapping_Ebeta"), int(h_pdf_out_color), N_rebin);
		
		double E_in_keV, other_E;
		double countin;
		for (int i=1; i<N_bins+1; i++)
		{
			E_in_keV = h_in->GetBinCenter(i);
			countin = h_in->GetBinContent(i);
			for (int j=1; j<N_bins+1; j++)
			{
				other_E = h_in->GetBinCenter(j);
				if(other_E > E_in_keV+2000) { continue; }
				if(other_E > E_in_keV+2000) { cout << "should never get here." << endl; }
				h_reconstructed -> Fill(other_E, countin * this->f_pdf_tp.at(i)->Eval(other_E) * binwidth );
			}
		}
		return h_reconstructed;  // tp
	};
	TH1D * reconstruct_bp_from_pdf(TH1D * h_naive)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("PDF Reconstructed Lower Energy(+)"), string("Mapping_Ebeta"), int(h_pdf_out_color), N_rebin);
		
		double E_in_keV, other_E;
		double countin;
		for (int i=1; i<N_bins+1; i++)
		{
			E_in_keV = h_in->GetBinCenter(i);
			countin = h_in->GetBinContent(i);
			for (int j=1; j<N_bins+1; j++)
			{
				other_E = h_in->GetBinCenter(j);
				if(other_E > E_in_keV+2000) { continue; }
				if(other_E > E_in_keV+2000) { cout << "should never get here." << endl; }
				h_reconstructed -> Fill(other_E, countin * this->f_pdf_bp.at(i)->Eval(other_E) * binwidth );
			}
		}
		return h_reconstructed;  // bp
	};
	TH1D * reconstruct_tm_from_pdf(TH1D * h_naive)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("PDF Reconstructed Upper Energy(-)"), string("Mapping_Ebeta"), int(h_pdf_out_color), N_rebin);
		
		double E_in_keV, other_E;
		double countin;
		for (int i=1; i<N_bins+1; i++)
		{
			E_in_keV = h_in->GetBinCenter(i);
			countin = h_in->GetBinContent(i);
			for (int j=1; j<N_bins+1; j++)
			{
				other_E = h_in->GetBinCenter(j);
				if(other_E > E_in_keV+2000) { continue; }
				if(other_E > E_in_keV+2000) { cout << "should never get here." << endl; }
				h_reconstructed -> Fill(other_E, countin * this->f_pdf_tm.at(i)->Eval(other_E) * binwidth );
			}
		}
		return h_reconstructed;  // tm
	};
	TH1D * reconstruct_bm_from_pdf(TH1D * h_naive)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("PDF Reconstructed Lower Energy(-)"), string("Mapping_Ebeta"), int(h_pdf_out_color), N_rebin);
		
		double E_in_keV, other_E;
		double countin;
		for (int i=1; i<N_bins+1; i++)
		{
			E_in_keV = h_in->GetBinCenter(i);
			countin = h_in->GetBinContent(i);
			for (int j=1; j<N_bins+1; j++)
			{
				other_E = h_in->GetBinCenter(j);
				if(other_E > E_in_keV+2000) { continue; }
				if(other_E > E_in_keV+2000) { cout << "should never get here." << endl; }
				h_reconstructed -> Fill(other_E, countin * this->f_pdf_bm.at(i)->Eval(other_E) * binwidth );
			}
		}
		return h_reconstructed;  // bm
	};
	
	void reconstruct_tp_from_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed_h = CreateHist( string("CDF Reconstructed Upper Energy(+)"), string("Mapping_Ebeta"), int(h_cdf_out_color), N_rebin);
		TH1D* h_reconstructed_ih = CreateHist( string("DirectIntegrated CDF Reconstructed Upper Energy(+)"), string("Mapping_Ebeta"), int(ih_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found_h = false;
		bool is_found_ih = false;
		int countin;
		double the_random;
		double bincontent_h, bincontent_ih;
		double the_E_h, the_E_ih;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found_h = false;
				is_found_ih = false;
				for (int j=1; j<N_bins+1; j++)
				{
					if( !is_found_h )
					{
						bincontent_h = h_cdf_tp.at(i)->GetBinContent(j);
						if( bincontent_h >= the_random )
						{
							is_found_h = true;
							the_E_h = h_in->GetBinCenter(j);
						}
					}
					if( !is_found_ih )
					{
						bincontent_ih = ih_cdf_tp.at(i)->GetBinContent(j);
						if( bincontent_ih >= the_random )
						{
							is_found_ih = true;
							the_E_ih = h_in->GetBinCenter(j);
						}
					}
				}
				if(is_found_h)
				{
					h_reconstructed_h -> Fill(the_E_h);
				}
				if(is_found_ih)
				{
					h_reconstructed_ih -> Fill(the_E_ih);
				}
			}
		}
		h_reconstructed_h_cdf_tp  = h_reconstructed_h;
		h_reconstructed_ih_cdf_tp = h_reconstructed_ih;
	};
	void reconstruct_tm_from_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed_h = CreateHist( string("CDF Reconstructed Upper Energy(-)"), string("Mapping_Ebeta"), int(h_cdf_out_color), N_rebin);
		TH1D* h_reconstructed_ih = CreateHist( string("DirectIntegrated CDF Reconstructed Upper Energy(-)"), string("Mapping_Ebeta"), int(ih_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found_h = false;
		bool is_found_ih = false;
		int countin;
		double the_random;
		double bincontent_h, bincontent_ih;
		double the_E_h, the_E_ih;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found_h = false;
				is_found_ih = false;
				for (int j=1; j<N_bins+1; j++)
				{
					if( !is_found_h )
					{
						bincontent_h = h_cdf_tm.at(i)->GetBinContent(j);
						if( bincontent_h >= the_random )
						{
							is_found_h = true;
							the_E_h = h_in->GetBinCenter(j);
						}
					}
					if( !is_found_ih )
					{
						bincontent_ih = ih_cdf_tm.at(i)->GetBinContent(j);
						if( bincontent_ih >= the_random )
						{
							is_found_ih = true;
							the_E_ih = h_in->GetBinCenter(j);
						}
					}
				}
				if(is_found_h)
				{
					h_reconstructed_h -> Fill(the_E_h);
				}
				if(is_found_ih)
				{
					h_reconstructed_ih -> Fill(the_E_ih);
				}
			}
		}
		h_reconstructed_h_cdf_tm  = h_reconstructed_h;
		h_reconstructed_ih_cdf_tm = h_reconstructed_ih;
	};
	void reconstruct_bp_from_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed_h = CreateHist( string("CDF Reconstructed Lower Energy(+)"), string("Mapping_Ebeta"), int(h_cdf_out_color), N_rebin);
		TH1D* h_reconstructed_ih = CreateHist( string("DirectIntegrated CDF Reconstructed Lower Energy(+)"), string("Mapping_Ebeta"), int(ih_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found_h = false;
		bool is_found_ih = false;
		int countin;
		double the_random;
		double bincontent_h, bincontent_ih;
		double the_E_h, the_E_ih;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found_h = false;
				is_found_ih = false;
				for (int j=1; j<N_bins+1; j++)
				{
					if( !is_found_h )
					{
						bincontent_h = h_cdf_bp.at(i)->GetBinContent(j);
						if( bincontent_h >= the_random )
						{
							is_found_h = true;
							the_E_h = h_in->GetBinCenter(j);
						}
					}
					if( !is_found_ih )
					{
						bincontent_ih = ih_cdf_bp.at(i)->GetBinContent(j);
						if( bincontent_ih >= the_random )
						{
							is_found_ih = true;
							the_E_ih = h_in->GetBinCenter(j);
						}
					}
				}
				if(is_found_h)
				{
					h_reconstructed_h -> Fill(the_E_h);
				}
				if(is_found_ih)
				{
					h_reconstructed_ih -> Fill(the_E_ih);
				}
			}
		}
		h_reconstructed_h_cdf_bp  = h_reconstructed_h;
		h_reconstructed_ih_cdf_bp = h_reconstructed_ih;
	};
	void reconstruct_bm_from_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed_h = CreateHist( string("CDF Reconstructed Lower Energy(-)"), string("Mapping_Ebeta"), int(h_cdf_out_color), N_rebin);
		TH1D* h_reconstructed_ih = CreateHist( string("DirectIntegrated CDF Reconstructed Lower Energy(-)"), string("Mapping_Ebeta"), int(ih_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found_h = false;
		bool is_found_ih = false;
		int countin;
		double the_random;
		double bincontent_h, bincontent_ih;
		double the_E_h, the_E_ih;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found_h = false;
				is_found_ih = false;
				for (int j=1; j<N_bins+1; j++)
				{
					if( !is_found_h )
					{
						bincontent_h = h_cdf_bm.at(i)->GetBinContent(j);
						if( bincontent_h >= the_random )
						{
							is_found_h = true;
							the_E_h = h_in->GetBinCenter(j);
						}
					}
					if( !is_found_ih )
					{
						bincontent_ih = ih_cdf_bm.at(i)->GetBinContent(j);
						if( bincontent_ih >= the_random )
						{
							is_found_ih = true;
							the_E_ih = h_in->GetBinCenter(j);
						}
					}
				}
				if(is_found_h)
				{
					h_reconstructed_h -> Fill(the_E_h);
				}
				if(is_found_ih)
				{
					h_reconstructed_ih -> Fill(the_E_ih);
				}
			}
		}
		h_reconstructed_h_cdf_bm  = h_reconstructed_h;
		h_reconstructed_ih_cdf_bm = h_reconstructed_ih;
	};
	
	/*
	void addback_E_tp_from_hcdf(int use_seed=0)  // h_reconstructed_h_cdf_tp must already be filled.
	{
		TH1D* h_in = h_reconstructed_h_cdf_tp;
		TH1D* h_out = h_rewithE_hcdf_tp;
		TH1D* h_line;
		
		double E_in;
		double the_dE;
		int countin;
		double the_random;
		bool is_found=false;
		double cdf_bincontent;
		
		//
		for (int i=1; i<N_bins+1; i++)
		{
			// which lineshape histogram are we looking at for this E_scint bin?
			h_line = h_cdf_tp.at(i);
			// how many events in this E_scint bin?
			countin = h_in->GetBinContent(i);
			// what's the central energy for the E_scint bin we're looking at?
			E_in = h_in->GetBinCenter(i);
			
			//
			for(int k=0; k<countin; k++)
			{
				is_found = false;
				while( !is_found )  // pick random numbers until we find one that works.
				{
					the_random = my_rng->Uniform(); 
					for (int j=1; j<N_bins+1; j++)   // for each bin in the CDF, check if the cum. prob. is higher than the random number.
					{
						cdf_bincontent = h_line->GetBinContent(j);  // h_line is the i-th hist.
						if( cdf_bincontent >= the_random )
						{
							is_found = true;
							the_dE = h_in->GetBinCenter(j);
						}
					}
				}
				h_out->Fill(E_in+the_dE);
			}
		}
		h_rewithE_hcdf_tp = h_out;
		return;
	}
	*/
	void addback_E_tp_from_hcdf(int use_seed=0)  // kludge
	{
		TH1D* h_in = h_reconstructed_h_cdf_tp;
	//	TH1D* h_out = h_rewithE_hcdf_tp;
		
		int countin;
		double E_center;
		int the_new_bin;
		for (int i=1; i<N_bins; i++)
		{
			cout << "i = " << i << endl;
			countin = h_in->GetBinContent(i);
			cout << "countin=" << countin << endl;
			E_center = h_in->GetBinCenter(i);
			cout << "E_center=" << E_center << endl;
			cout << "kludge_dE= " << kludge_dE << endl;
			the_new_bin = h_in->FindBin(E_center+kludge_dE);
			cout << "the_new_bin = " << the_new_bin << endl;
			//
		//	h_out->Fill(E_center+kludge_dE, countin);
			h_rewithE_hcdf_tp->SetBinContent(the_new_bin, countin);
		}
	//	h_rewithE_hcdf_tp = h_out;
		return;
	}
	void addback_E_bp_from_hcdf(int use_seed=0)  // kludge
	{
		TH1D* h_in = h_reconstructed_h_cdf_bp;
		TH1D* h_out = h_rewithE_hcdf_bp;
		
		int countin;
		double E_center;
		for (int i=1; i<N_bins; i++)
		{
			countin = h_in->GetBinContent(i);
			E_center = h_in->GetBinCenter(i);
			h_out->Fill(E_center+kludge_dE, countin);
		}
		h_rewithE_hcdf_bp = h_out;
		return;
	}
	void addback_E_tm_from_hcdf(int use_seed=0)  // kludge
	{
		TH1D* h_in = h_reconstructed_h_cdf_tm;
		TH1D* h_out = h_rewithE_hcdf_tm;
		
		int countin;
		double E_center;
		for (int i=1; i<N_bins; i++)
		{
			countin = h_in->GetBinContent(i);
			E_center = h_in->GetBinCenter(i);
			h_out->Fill(E_center+kludge_dE, countin);
		}
		h_rewithE_hcdf_tm = h_out;
		return;
	}
	void addback_E_bm_from_hcdf(int use_seed=0)  // kludge
	{
		TH1D* h_in = h_reconstructed_h_cdf_bm;
		TH1D* h_out = h_rewithE_hcdf_bm;
		
		int countin;
		double E_center;
		for (int i=1; i<N_bins; i++)
		{
			countin = h_in->GetBinContent(i);
			E_center = h_in->GetBinCenter(i);
			h_out->Fill(E_center+kludge_dE, countin);
		}
		h_rewithE_hcdf_bm = h_out;
		return;
	}
	
	/*
	void addback_E_tp_from_hcdf(int use_seed=0)  // h_reconstructed_h_cdf_tp must already be filled.
	{
		TH1D * oldhist = (TH1D*)h_reconstructed_h_cdf_tp->Clone();
		TH1D * newhist = CreateHist( string("CDF Reconstructed Upper Energy(+), with Re-Added Energy"), string("Mapping_Ebeta"), int(reE_hcdf_out_color), N_rebin);
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		double the_random;
		double the_oldbincontent, bincontent_response;
		double newE_thisevent;
		double the_E_h;
		bool is_found_h;
		for (int i=1; i<N_bins+1; i++)  // i = bins in the original hist.
		{
			the_oldbincontent = oldhist->GetBinContent(i);
			for(int k=0; k<the_oldbincontent; k++) // k = entries in the i-th bin in the original hist.
			{
				is_found_h = false;
				while( !is_found_h )  // do it until you get a result.  
				{
					for (int j=1; j<N_bins+1; j++) // j = bins in the response function.
					{
						the_random = my_rng->Uniform();
						bincontent_response = h_cdf_tp.at(i)->GetBinContent(j);
						if( bincontent_response >= the_random )
						{
							is_found_h = true;
							the_E_h = oldhist->GetBinCenter(j); // E that you get out after subtracting some.
						}
					}
				}
				newE_thisevent = oldhist->GetBinCenter(i) + (oldhist->GetBinCenter(i) - the_E_h);
				newhist -> Fill(newE_thisevent);
				cout << "i=" << i << ";\tE_bincenter=" << oldhist->GetBinCenter(i) << ";\tthe_E_h= "<< the_E_h << ";\tnewE=" << newE_thisevent << endl;
			}
		}
		h_rewithE_hcdf_tp = newhist;
	}
	void addback_E_bp_from_hcdf(int use_seed=0)  //
	{
		TH1D * oldhist = (TH1D*)h_reconstructed_h_cdf_bp->Clone();
		TH1D * newhist = CreateHist( string("CDF Reconstructed Lower Energy(+), with Re-Added Energy"), string("Mapping_Ebeta"), int(reE_hcdf_out_color), N_rebin);
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		double the_random;
		double the_oldbincontent, bincontent_response;
		double newE_thisevent;
		double the_E_h;
		bool is_found_h;
		for (int i=1; i<N_bins+1; i++)
		{
			the_oldbincontent = oldhist->GetBinContent(i);
			for(int k=0; k<the_oldbincontent; k++)
			{
				for (int j=1; j<N_bins+1; j++) // look through bins in the response function.
				{
					the_random = my_rng->Uniform();
					is_found_h = false;
					while( !is_found_h )  // do it until you get a result.  
					{
						bincontent_response = h_cdf_bp.at(i)->GetBinContent(j);
						if( bincontent_response >= the_random )
						{
							is_found_h = true;
							the_E_h = oldhist->GetBinCenter(j); // E that you get out after subtracting some.
						}
					}
				}
				newE_thisevent = oldhist->GetBinCenter(k) + (oldhist->GetBinCenter(k) - the_E_h);
				newhist -> Fill(newE_thisevent);
			}
		}
		h_rewithE_hcdf_bp = newhist;
	}
	void addback_E_tm_from_hcdf(int use_seed=0)  //
	{
		TH1D * oldhist = (TH1D*)h_reconstructed_h_cdf_bp->Clone();
		TH1D * newhist = CreateHist( string("CDF Reconstructed Upper Energy(-), with Re-Added Energy"), string("Mapping_Ebeta"), int(reE_hcdf_out_color), N_rebin);
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		double the_random;
		double the_oldbincontent, bincontent_response;
		double newE_thisevent;
		double the_E_h;
		bool is_found_h;
		for (int i=1; i<N_bins+1; i++)
		{
			the_oldbincontent = oldhist->GetBinContent(i);
			for(int k=0; k<the_oldbincontent; k++)
			{
				for (int j=1; j<N_bins+1; j++) // look through bins in the response function.
				{
					the_random = my_rng->Uniform();
					is_found_h = false;
					while( !is_found_h )  // do it until you get a result.  
					{
						bincontent_response = h_cdf_tm.at(i)->GetBinContent(j);
						if( bincontent_response >= the_random )
						{
							is_found_h = true;
							the_E_h = oldhist->GetBinCenter(j); // E that you get out after subtracting some.
						}
					}
				}
				newE_thisevent = oldhist->GetBinCenter(k) + (oldhist->GetBinCenter(k) - the_E_h);
				newhist -> Fill(newE_thisevent);
			}
		}
		h_rewithE_hcdf_tm = newhist;
	}
	void addback_E_bm_from_hcdf(int use_seed=0)  //
	{
		TH1D * oldhist = (TH1D*)h_reconstructed_h_cdf_bp->Clone();
		TH1D * newhist = CreateHist( string("CDF Reconstructed Lower Energy(-), with Re-Added Energy"), string("Mapping_Ebeta"), int(reE_hcdf_out_color), N_rebin);
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		double the_random;
		double the_oldbincontent, bincontent_response;
		double newE_thisevent;
		double the_E_h;
		bool is_found_h;
		for (int i=1; i<N_bins+1; i++)
		{
			the_oldbincontent = oldhist->GetBinContent(i);
			for(int k=0; k<the_oldbincontent; k++)
			{
				for (int j=1; j<N_bins+1; j++) // look through bins in the response function.
				{
					the_random = my_rng->Uniform();
					is_found_h = false;
					while( !is_found_h )  // do it until you get a result.  
					{
						bincontent_response = h_cdf_bm.at(i)->GetBinContent(j);
						if( bincontent_response >= the_random )
						{
							is_found_h = true;
							the_E_h = oldhist->GetBinCenter(j); // E that you get out after subtracting some.
						}
					}
				}
				newE_thisevent = oldhist->GetBinCenter(k) + (oldhist->GetBinCenter(k) - the_E_h);
				newhist -> Fill(newE_thisevent);
			}
		}
		h_rewithE_hcdf_bm = newhist;
	}
	*/

	/*
	TH1D * reconstruct_tp_from_hist_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("CDF Reconstructed Upper Energy(+)"), string("Mapping_Ebeta"), int(h_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found = false;
		int countin;
		double the_random;
		double bincontent;
		double the_E;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found = false;
				for (int j=1; j<N_bins+1 && !is_found; j++)
				{
					bincontent = h_cdf_tp.at(i)->GetBinContent(j);
					if( bincontent >= the_random )
					{
						is_found = true;
						the_E = h_in->GetBinCenter(j);
					}
				}
				if(is_found)
				{
					h_reconstructed -> Fill(the_E);
				}
			}
		}
		return h_reconstructed;
	};
	TH1D * reconstruct_bp_from_hist_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("CDF Reconstructed Lower Energy(+)"), string("Mapping_Ebeta"), int(h_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found = false;
		int countin;
		double the_random;
		double bincontent;
		double the_E;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found = false;
				for (int j=1; j<N_bins+1 && !is_found; j++)
				{
					bincontent = h_cdf_bp.at(i)->GetBinContent(j);
					if( bincontent >= the_random )
					{
						is_found = true;
						the_E = h_in->GetBinCenter(j);
					}
				}
				if(is_found)
				{
					h_reconstructed -> Fill(the_E);
				}
			}
		}
		return h_reconstructed;
	};
	TH1D * reconstruct_tm_from_hist_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("CDF Reconstructed Upper Energy(-)"), string("Mapping_Ebeta"), int(h_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found = false;
		int countin;
		double the_random;
		double bincontent;
		double the_E;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found = false;
				for (int j=1; j<N_bins+1 && !is_found; j++)
				{
					bincontent = h_cdf_tm.at(i)->GetBinContent(j);
					if( bincontent >= the_random )
					{
						is_found = true;
						the_E = h_in->GetBinCenter(j);
					}
				}
				if(is_found)
				{
					h_reconstructed -> Fill(the_E);
				}
			}
		}
		return h_reconstructed;
	};
	TH1D * reconstruct_bm_from_hist_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("CDF Reconstructed Lower Energy(-)"), string("Mapping_Ebeta"), int(h_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found = false;
		int countin;
		double the_random;
		double bincontent;
		double the_E;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found = false;
				for (int j=1; j<N_bins+1 && !is_found; j++)
				{
					bincontent = h_cdf_bm.at(i)->GetBinContent(j);
					if( bincontent >= the_random )
					{
						is_found = true;
						the_E = h_in->GetBinCenter(j);
					}
				}
				if(is_found)
				{
					h_reconstructed -> Fill(the_E);
				}
			}
		}
		return h_reconstructed;
	};
	TH1D * reconstruct_tp_from_ih_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("DirectIntegrated CDF Reconstructed Upper Energy(+)"), string("Mapping_Ebeta"), int(ih_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found = false;
		int countin;
		double the_random;
		double bincontent;
		double the_E;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found = false;
				for (int j=1; j<N_bins+1 && !is_found; j++)
				{
					bincontent = ih_cdf_tp.at(i)->GetBinContent(j);
					if( bincontent >= the_random )
					{
						is_found = true;
						the_E = h_in->GetBinCenter(j);
					}
				}
				if(is_found)
				{
					h_reconstructed -> Fill(the_E);
				}
			}
		}
		return h_reconstructed;
	};
	TH1D * reconstruct_bp_from_ih_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("DirectIntegrated CDF Reconstructed Lower Energy(+)"), string("Mapping_Ebeta"), int(ih_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found = false;
		int countin;
		double the_random;
		double bincontent;
		double the_E;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found = false;
				for (int j=1; j<N_bins+1 && !is_found; j++)
				{
					bincontent = ih_cdf_bp.at(i)->GetBinContent(j);
					if( bincontent >= the_random )
					{
						is_found = true;
						the_E = h_in->GetBinCenter(j);
					}
				}
				if(is_found)
				{
					h_reconstructed -> Fill(the_E);
				}
			}
		}
		return h_reconstructed;
	};
	TH1D * reconstruct_tm_from_ih_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("DirectIntegrated CDF Reconstructed Upper Energy(-)"), string("Mapping_Ebeta"), int(ih_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found = false;
		int countin;
		double the_random;
		double bincontent;
		double the_E;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found = false;
				for (int j=1; j<N_bins+1 && !is_found; j++)
				{
					bincontent = ih_cdf_tm.at(i)->GetBinContent(j);
					if( bincontent >= the_random )
					{
						is_found = true;
						the_E = h_in->GetBinCenter(j);
					}
				}
				if(is_found)
				{
					h_reconstructed -> Fill(the_E);
				}
			}
		}
		return h_reconstructed;
	};
	TH1D * reconstruct_bm_from_ih_cdf(TH1D * h_naive, int use_seed=0)  // put in the histogram un-rebinned !
	{
		TH1D * h_in = (TH1D*)h_naive->Clone();
		h_in->Rebin(N_rebin);
		TH1D* h_reconstructed = CreateHist( string("DirectIntegrated CDF Reconstructed Lower Energy(-)"), string("Mapping_Ebeta"), int(ih_cdf_out_color), N_rebin);
		
		TRandom3 * my_rng = new TRandom3(use_seed);
		
		bool is_found = false;
		int countin;
		double the_random;
		double bincontent;
		double the_E;
		for (int i=1; i<N_bins+1; i++)
		{
			countin = h_in->GetBinContent(i);
			for(int k=0; k<countin; k++)
			{
				the_random = my_rng->Uniform();
				is_found = false;
				for (int j=1; j<N_bins+1 && !is_found; j++)
				{
					bincontent = ih_cdf_bm.at(i)->GetBinContent(j);
					if( bincontent >= the_random )
					{
						is_found = true;
						the_E = h_in->GetBinCenter(j);
					}
				}
				if(is_found)
				{
					h_reconstructed -> Fill(the_E);
				}
			}
		}
		return h_reconstructed;
	};
	*/
	
	
public:
	int N_rebin; 
	double binwidth;
	int N_bins;
	
	int h_pdf_out_color;
	int h_cdf_out_color;
	int ih_cdf_out_color;
	int reE_hcdf_out_color;
	
	double kludge_dE;
	
	vector<TF1*> f_pdf_tp;
	vector<TF1*> f_pdf_bp;
	vector<TF1*> f_pdf_tm;
	vector<TF1*> f_pdf_bm;
	
	vector<TH1D*> h_pdf_tp;
	vector<TH1D*> h_pdf_bp;
	vector<TH1D*> h_pdf_tm;
	vector<TH1D*> h_pdf_bm;
	
	vector<TH1D*> ih_cdf_tp;
	vector<TH1D*> ih_cdf_bp;
	vector<TH1D*> ih_cdf_tm;
	vector<TH1D*> ih_cdf_bm;
	
	vector<TH1D*> h_cdf_tp;
	vector<TH1D*> h_cdf_bp;
	vector<TH1D*> h_cdf_tm;
	vector<TH1D*> h_cdf_bm;
	
public:
	TH1D * h_reconstructed_pdf_tp;
	TH1D * h_reconstructed_h_cdf_tp;
	TH1D * h_reconstructed_ih_cdf_tp;
	
	TH1D * h_reconstructed_pdf_bp;
	TH1D * h_reconstructed_h_cdf_bp;
	TH1D * h_reconstructed_ih_cdf_bp;
	
	TH1D * h_reconstructed_pdf_tm;
	TH1D * h_reconstructed_h_cdf_tm;
	TH1D * h_reconstructed_ih_cdf_tm;

	TH1D * h_reconstructed_pdf_bm;
	TH1D * h_reconstructed_h_cdf_bm;
	TH1D * h_reconstructed_ih_cdf_bm;
	
	TH1D * h_rewithE_hcdf_tp;
	TH1D * h_rewithE_hcdf_bp;
	TH1D * h_rewithE_hcdf_tm;
	TH1D * h_rewithE_hcdf_bm;
	
private:
	TH1D* h_tmp;
	TFile * file_we_loaded_from;
};
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#endif
