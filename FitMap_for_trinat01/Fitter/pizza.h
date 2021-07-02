// pizza.h

#include "/Users/anholm/Packages/MiscLibs/HistExtras.cpp"
//extern setup_location();

//string pizza_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";
extern string pizza_path;

string fake_to_string(double thisnumber)  // This thing super doesn't work!!!  :(  ...wait, what?  It's fine.
{
	string mynumberstring;// = "";
	
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << thisnumber;
	
	mynumberstring = ss.str();
	return mynumberstring;
}
//
string make_additional_filename_info(int threshold_index, double sigma_cut, double bb1_maxr)  // runsets BCD.
{
	string sig_str = convertDouble(sigma_cut);
	if( (sigma_cut-double(int(sigma_cut)))==0.0 ) { sig_str.replace(1, 1, "p0"); }
	else                                          { sig_str.replace(1, 1, "p" ); }
	string bb_str = convertDouble(bb1_maxr);
	if( (bb1_maxr-double(int(bb1_maxr)))==0.0 )   { bb_str.replace(2, 1, "p0"); }
	else                                          { bb_str.replace(2, 1, "p" ); }
	string additional_filename_info = "_BCD_ind"+int_to_string(threshold_index)+"_sig"+sig_str+"_bbr"+bb_str;
//	string outfilename = string(output_path+namestub+additional_filename_info+".root");
	return additional_filename_info;
}

double getfast_alpha_from_Abeta_and_bFierz(double Abeta, double bFierz)
{
	double A_plus  = -0.563786;
	double A_0     = -0.568045;
	double A_minus = -0.563786;
	double b_plus  = -0.148661;
	double b_0     =  0.0;
	double b_minus =  0.148661;
	
	double b_bsm = b_0;
	if(bFierz<=0) { b_bsm=b_plus; }
	else          { b_bsm=b_minus; }
	
	double alpha = (Abeta*b_bsm - A_plus*bFierz) / (A_0*b_bsm - A_plus*b_0);
	return alpha;
}
double getfast_beta_from_Abeta_and_bFierz(double Abeta, double bFierz)
{
	double A_plus  = -0.563786;
	double A_0     = -0.568045;
	double A_minus = -0.563786;
	double b_plus  = -0.148661;
	double b_0     =  0.0;
	double b_minus =  0.148661;
	
	double b_bsm = b_0;
	if(bFierz<=0) { b_bsm=b_plus; }
	else          { b_bsm=b_minus; }
	
	double beta = (bFierz*A_0 - Abeta*b_0) / (A_0*b_bsm - A_plus*b_0);
	return beta;
}

/*
double getfast_alpha_from_Abeta_and_bFierz(double Abeta, double bFierz)
{
	double A_plus  = -0.563786;
	double A_0     = -0.568045;
//	double A_minus = -0.563786;
	double b_plus  = -0.148661;
	double b_0     =  0.0;
	double b_minus =  0.148661;
	
	double b_bsm = b_0;
	if(bFierz<=0)
	{
		b_bsm=b_plus
	}
	else
	{
		b_bsm=b_minus
	}
	
	double alpha = (Abeta*b_plus - A_plus*bFierz) / (A_0*b_plus - A_plus*b_0);
	return alpha;
}
double getfast_beta_from_Abeta_and_bFierz(double Abeta, double bFierz)
{
	double A_plus  = -0.563786;
	double A_0     = -0.568045;
//	double A_minus = -0.563786;
	double b_plus  = -0.148661;
	double b_0     =  0.0;
//	double b_minus =  0.148661;
	
	double b_bsm = b_0;
	if(bFierz<=0) { b_bsm=b_plus; }
	else          { b_bsm=b_minus; }
	
	double beta = (bFierz*A_0 - Abeta*b_0) / (A_0*b_plus - A_plus*b_0);
	return beta;
}
*/

string getmake_pizzastub(string namestub, string matched_set, int threshold_index, double sigma_cut, int bb1_threshold, double bb1_maxr)
{
	string additional_filename_info = make_additional_filename_info(threshold_index, sigma_cut, bb1_maxr);
	string bb_str = int_to_string(bb1_threshold);
	string pizza_stub = namestub+additional_filename_info+"_Set"+matched_set+"_thr"+bb_str;
	return pizza_stub;
}

struct toppings
{
	double Abeta;
	double bFierz;
	double BS_scale;
	double SS_scale;
	double BG_scale;
};
toppings pick_toppings(double A, double b, double bs_scale=1.0, double ss_scale=1.0, double bg_scale=1.0)
{
	toppings the_toppings;
	the_toppings.Abeta = A;
	the_toppings.bFierz = b;
	the_toppings.BS_scale = bs_scale;
	the_toppings.SS_scale = ss_scale;
	the_toppings.BG_scale = bg_scale;
	
	return the_toppings;
}

struct pizza  // this is not a very good pizza...
{
	string pizza_stub;
//	string pizza_path;
	//
	string matched_set;
	int threshold_index;
	double sigma_cut;
	int bb1_threshold;
	double bb1_maxr;

	TH1D * h_sm_direct_tp;
	TH1D * h_sm_direct_tm;
	TH1D * h_sm_direct_bp;
	TH1D * h_sm_direct_bm;

	TH1D * h_sm_fss_tp;
	TH1D * h_sm_fss_tm;
	TH1D * h_sm_fss_bp;
	TH1D * h_sm_fss_bm;

	TH1D * h_sm_bss_tp;
	TH1D * h_sm_bss_tm;
	TH1D * h_sm_bss_bp;
	TH1D * h_sm_bss_bm;
	
	TH1D * h_sm_bs_tp;
	TH1D * h_sm_bs_tm;
	TH1D * h_sm_bs_bp;
	TH1D * h_sm_bs_bm;
	
	//
	TH1D * h_bsmp_direct_tp;
	TH1D * h_bsmp_direct_tm;
	TH1D * h_bsmp_direct_bp;
	TH1D * h_bsmp_direct_bm;

	TH1D * h_bsmp_fss_tp;
	TH1D * h_bsmp_fss_tm;
	TH1D * h_bsmp_fss_bp;
	TH1D * h_bsmp_fss_bm;

	TH1D * h_bsmp_bss_tp;
	TH1D * h_bsmp_bss_tm;
	TH1D * h_bsmp_bss_bp;
	TH1D * h_bsmp_bss_bm;
	
	TH1D * h_bsmp_bs_tp;
	TH1D * h_bsmp_bs_tm;
	TH1D * h_bsmp_bs_bp;
	TH1D * h_bsmp_bs_bm;
	
	//
	TH1D * h_bsmm_direct_tp;
	TH1D * h_bsmm_direct_tm;
	TH1D * h_bsmm_direct_bp;
	TH1D * h_bsmm_direct_bm;

	TH1D * h_bsmm_fss_tp;
	TH1D * h_bsmm_fss_tm;
	TH1D * h_bsmm_fss_bp;
	TH1D * h_bsmm_fss_bm;

	TH1D * h_bsmm_bss_tp;
	TH1D * h_bsmm_bss_tm;
	TH1D * h_bsmm_bss_bp;
	TH1D * h_bsmm_bss_bm;
	
	TH1D * h_bsmm_bs_tp;
	TH1D * h_bsmm_bs_tm;
	TH1D * h_bsmm_bs_bp;
	TH1D * h_bsmm_bs_bm;
	//
	
	TH1D * h_bg_tp;
	TH1D * h_bg_tm;
	TH1D * h_bg_bp;
	TH1D * h_bg_bm;
	
	/*
	string getmake_pizzaminifname()
	{
	//	string output_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";
	//	pizza_path = output_path;
	
		string namestub = "pizzafile";
		string additional_filename_info = make_additional_filename_info(threshold_index, sigma_cut, bb1_maxr);
		string bb_str = int_to_string(bb1_threshold);

		pizza_stub = namestub+additional_filename_info+"_Set"+matched_set+"_thr"+bb_str;
	//	pizza_stub = namestub+additional_filename_info+"_Set"+matched_set+"_thr"+bb_str+".root";
	//	string fname = pizza_path+pizza_stub;
		return pizza_stub;
	};
	*/
};

pizza make_pizza(string sm_minifname, string bsmp_minifname, string bsmm_minifname, string bg_minifname, string matched_set, int threshold_index, double sigma_cut, int bb1_threshold, double bb1_maxr)
{
//	int N_rebin = 10;  // rebin everything.  everything!
	string harder_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";  // doesn't have to be the same as the pizzapath.
	string additional_filename_info = make_additional_filename_info(threshold_index, sigma_cut, bb1_maxr);
	
//	string fname_sm   = harder_path+sm_minifname;
//	string fname_bsmp = harder_path+bsmp_minifname;
//	string fname_bsmm = harder_path+bsmm_minifname;
	string fname_sm   = harder_path+sm_minifname+additional_filename_info+".root";
	string fname_bsmp = harder_path+bsmp_minifname+additional_filename_info+".root";
	string fname_bsmm = harder_path+bsmm_minifname+additional_filename_info+".root";
	string fname_bg   = harder_path+bg_minifname;
	
	TFile * f_sm   = new TFile( (fname_sm).c_str() );
	TFile * f_bsmp = new TFile( (fname_bsmp).c_str() );
	TFile * f_bsmm = new TFile( (fname_bsmm).c_str() );
	TFile * f_bg   = new TFile( (fname_bg).c_str() );
	
	cout << "f_sm:   " << fname_sm << " (" << f_sm << ")" << endl;
	cout << "f_bsmp: " << fname_bsmp << " (" << f_bsmp << ")" << endl;
	cout << "f_bsmm: " << fname_bsmm << " (" << f_bsmm << ")" << endl;
	cout << "f_bg:   " << fname_bg << " (" << f_bg << ")" << endl;
	
	pizza the_pizza;
	
	the_pizza.matched_set     = matched_set;
	the_pizza.threshold_index = threshold_index;
	the_pizza.sigma_cut       = sigma_cut;
	the_pizza.bb1_threshold   = bb1_threshold;
	the_pizza.bb1_maxr        = bb1_maxr;
	the_pizza.pizza_stub      = getmake_pizzastub("pizzafile", matched_set, threshold_index, sigma_cut, bb1_threshold, bb1_maxr);
	
	string bb_str = int_to_string(bb1_threshold);

	/*
	//	string sig_str = convertDouble(sigma_cut);
	//	if( (sigma_cut-double(int(sigma_cut)))==0.0 ) { sig_str.replace(1, 1, "p0"); }
	//	else                                          { sig_str.replace(1, 1, "p" ); }
	//	string bb_str = convertDouble(bb1_maxr);
	//	if( (bb1_maxr-double(int(bb1_maxr)))==0.0 )   { bb_str.replace(2, 1, "p0"); }
	//	else                                          { bb_str.replace(2, 1, "p" ); }
	//	string additional_filename_info = "_BCD_ind"+int_to_string(threshold_index)+"_sig"+sig_str+"_bbr"+bb_str;
	//	string outfilename = string(output_path+namestub+additional_filename_info+".root");
	
	string input_namestub = "MakeHarderMulti_SM";
	string input_minifname = string(input_namestub+additional_filename_info+".root");
	
	string output_namestub = "GenMultiEnergyAsym_SM";
	string out_minifname  = string(output_namestub+additional_filename_info+".root");
	
	string fname_sm   = output_path+sm_minifname+additional_filename_info+".root";
	*/
	
	//
	
	//
	the_pizza.h_sm_direct_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_sm_direct_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_sm_direct_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_sm_direct_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_sm_fss_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_sm_fss_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_sm_fss_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_sm_fss_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_sm_bss_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_sm_bss_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_sm_bss_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_sm_bss_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_sm_bs_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_sm_bs_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_sm_bs_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_sm_bs_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	
	/*
	cout << "SM stuff's got gotten, I guess." << endl;
	cout << the_pizza.h_sm_direct_tp << endl;
	cout << "\tattempted name of h_sm_direct_tp:  " << string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T+)").c_str() << endl;
	// 60keV SetB(DirectHit Lev+0eV TOFcut), (T+)
	
	// 20keV SetB E_scint (DirectHit Lev+0eV TOFcut), (T+)
//	f_sm->ls();
	cout << the_pizza.h_sm_direct_tm << endl;
	cout << the_pizza.h_sm_direct_bp << endl;
	cout << the_pizza.h_sm_direct_bm << endl;
	cout << the_pizza.h_sm_fss_tp << endl;
	cout << the_pizza.h_sm_fss_tm << endl;
	cout << the_pizza.h_sm_fss_bp << endl;
	cout << the_pizza.h_sm_fss_bm << endl;
	cout << the_pizza.h_sm_bss_tp << endl;
	cout << the_pizza.h_sm_bss_tm << endl;
	cout << the_pizza.h_sm_bss_bp << endl;
	cout << the_pizza.h_sm_bss_bm << endl;
	cout << the_pizza.h_sm_bs_tp << endl;
	cout << the_pizza.h_sm_bs_tm << endl;
	cout << the_pizza.h_sm_bs_bp << endl;
	cout << the_pizza.h_sm_bs_bm << endl;
	*/
	
//	cout << "guess I'll rename those now." << endl;
	the_pizza.h_sm_direct_tp -> SetName("E_Scint SM DirectHit Lev0eV (T+)");
	the_pizza.h_sm_direct_tm -> SetName("E_Scint SM DirectHit Lev0eV (T-)");
	the_pizza.h_sm_direct_bp -> SetName("E_Scint SM DirectHit Lev0eV (B+)");
	the_pizza.h_sm_direct_bm -> SetName("E_Scint SM DirectHit Lev0eV (B-)");
	the_pizza.h_sm_direct_tp -> SetTitle("E_Scint SM DirectHit Lev0eV (T+)");
	the_pizza.h_sm_direct_tm -> SetTitle("E_Scint SM DirectHit Lev0eV (T-)");
	the_pizza.h_sm_direct_bp -> SetTitle("E_Scint SM DirectHit Lev0eV (B+)");
	the_pizza.h_sm_direct_bm -> SetTitle("E_Scint SM DirectHit Lev0eV (B-)");
	
	the_pizza.h_sm_fss_tp -> SetName("E_Scint SM ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_fss_tm -> SetName("E_Scint SM ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_fss_bp -> SetName("E_Scint SM ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_fss_bm -> SetName("E_Scint SM ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_sm_fss_tp -> SetTitle("E_Scint SM ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_fss_tm -> SetTitle("E_Scint SM ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_fss_bp -> SetTitle("E_Scint SM ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_fss_bm -> SetTitle("E_Scint SM ForwardSideScatter Lev0eV (B-)");

	the_pizza.h_sm_bss_tp -> SetName("E_Scint SM BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_bss_tm -> SetName("E_Scint SM BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_bss_bp -> SetName("E_Scint SM BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_bss_bm -> SetName("E_Scint SM BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_sm_bss_tp -> SetTitle("E_Scint SM BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_bss_tm -> SetTitle("E_Scint SM BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_bss_bp -> SetTitle("E_Scint SM BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_bss_bm -> SetTitle("E_Scint SM BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_sm_bs_tp -> SetName("E_Scint SM BackScatter Lev0eV (T+)");
	the_pizza.h_sm_bs_tm -> SetName("E_Scint SM BackScatter Lev0eV (T-)");
	the_pizza.h_sm_bs_bp -> SetName("E_Scint SM BackScatter Lev0eV (B+)");
	the_pizza.h_sm_bs_bm -> SetName("E_Scint SM BackScatter Lev0eV (B-)");
	the_pizza.h_sm_bs_tp -> SetTitle("E_Scint SM BackScatter Lev0eV (T+)");
	the_pizza.h_sm_bs_tm -> SetTitle("E_Scint SM BackScatter Lev0eV (T-)");
	the_pizza.h_sm_bs_bp -> SetTitle("E_Scint SM BackScatter Lev0eV (B+)");
	the_pizza.h_sm_bs_bm -> SetTitle("E_Scint SM BackScatter Lev0eV (B-)");
	
//	cout << "and now SM stuff has been renamed." << endl;
	
	//
	/*
	the_pizza.h_bsmp_direct_tp = (TH1D*)f_bsmp->Get("E_Scint DirectHit Lev0eV (T+)");
	the_pizza.h_bsmp_direct_tm = (TH1D*)f_bsmp->Get("E_Scint DirectHit Lev0eV (T-)");
	the_pizza.h_bsmp_direct_bp = (TH1D*)f_bsmp->Get("E_Scint DirectHit Lev0eV (B+)");
	the_pizza.h_bsmp_direct_bm = (TH1D*)f_bsmp->Get("E_Scint DirectHit Lev0eV (B-)");
	the_pizza.h_bsmp_fss_tp = (TH1D*)f_bsmp->Get("E_Scint ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_fss_tm = (TH1D*)f_bsmp->Get("E_Scint ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_fss_bp = (TH1D*)f_bsmp->Get("E_Scint ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_fss_bm = (TH1D*)f_bsmp->Get("E_Scint ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmp_bss_tp = (TH1D*)f_bsmp->Get("E_Scint BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bss_tm = (TH1D*)f_bsmp->Get("E_Scint BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bss_bp = (TH1D*)f_bsmp->Get("E_Scint BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bss_bm = (TH1D*)f_bsmp->Get("E_Scint BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmp_bs_tp = (TH1D*)f_bsmp->Get("E_Scint BackScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bs_tm = (TH1D*)f_bsmp->Get("E_Scint BackScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bs_bp = (TH1D*)f_bsmp->Get("E_Scint BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm = (TH1D*)f_bsmp->Get("E_Scint BackScatter Lev0eV (B-)");
	*/
	
	//
	the_pizza.h_bsmp_direct_tp = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmp_direct_tm = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmp_direct_bp = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmp_direct_bm = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmp_fss_tp = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmp_fss_tm = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmp_fss_bp = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmp_fss_bm = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmp_bss_tp = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmp_bss_tm = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmp_bss_bp = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmp_bss_bm = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmp_bs_tp = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmp_bs_tm = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmp_bs_bp = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmp_bs_bm = (TH1D*)f_bsmp->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	
	the_pizza.h_bsmp_direct_tp -> SetName("E_Scint BSM+ DirectHit Lev0eV (T+)");
	the_pizza.h_bsmp_direct_tm -> SetName("E_Scint BSM+ DirectHit Lev0eV (T-)");
	the_pizza.h_bsmp_direct_bp -> SetName("E_Scint BSM+ DirectHit Lev0eV (B+)");
	the_pizza.h_bsmp_direct_bm -> SetName("E_Scint BSM+ DirectHit Lev0eV (B-)");
	the_pizza.h_bsmp_direct_tp -> SetTitle("E_Scint BSM+ DirectHit Lev0eV (T+)");
	the_pizza.h_bsmp_direct_tm -> SetTitle("E_Scint BSM+ DirectHit Lev0eV (T-)");
	the_pizza.h_bsmp_direct_bp -> SetTitle("E_Scint BSM+ DirectHit Lev0eV (B+)");
	the_pizza.h_bsmp_direct_bm -> SetTitle("E_Scint BSM+ DirectHit Lev0eV (B-)");
	
	the_pizza.h_bsmp_fss_tp -> SetName("E_Scint BSM+ ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_fss_tm -> SetName("E_Scint BSM+ ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_fss_bp -> SetName("E_Scint BSM+ ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_fss_bm -> SetName("E_Scint BSM+ ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmp_fss_tp -> SetTitle("E_Scint BSM+ ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_fss_tm -> SetTitle("E_Scint BSM+ ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_fss_bp -> SetTitle("E_Scint BSM+ ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_fss_bm -> SetTitle("E_Scint BSM+ ForwardSideScatter Lev0eV (B-)");

	the_pizza.h_bsmp_bss_tp -> SetName("E_Scint BSM+ BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bss_tm -> SetName("E_Scint BSM+ BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bss_bp -> SetName("E_Scint BSM+ BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bss_bm -> SetName("E_Scint BSM+ BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmp_bss_tp -> SetTitle("E_Scint BSM+ BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bss_tm -> SetTitle("E_Scint BSM+ BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bss_bp -> SetTitle("E_Scint BSM+ BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bss_bm -> SetTitle("E_Scint BSM+ BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmp_bs_tp -> SetName("E_Scint BSM+ BackScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bs_tm -> SetName("E_Scint BSM+ BackScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bs_bp -> SetName("E_Scint BSM+ BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm -> SetName("E_Scint BSM+ BackScatter Lev0eV (B-)");
	the_pizza.h_bsmp_bs_tp -> SetTitle("E_Scint BSM+ BackScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bs_tm -> SetTitle("E_Scint BSM+ BackScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bs_bp -> SetTitle("E_Scint BSM+ BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm -> SetTitle("E_Scint BSM+ BackScatter Lev0eV (B-)");
	
	cout << "BSMP stuff is gotten and modified." << endl;
	//
	/*
	the_pizza.h_bsmm_direct_tp = (TH1D*)f_bsmm->Get("E_Scint DirectHit Lev0eV (T+)");
	the_pizza.h_bsmm_direct_tm = (TH1D*)f_bsmm->Get("E_Scint DirectHit Lev0eV (T-)");
	the_pizza.h_bsmm_direct_bp = (TH1D*)f_bsmm->Get("E_Scint DirectHit Lev0eV (B+)");
	the_pizza.h_bsmm_direct_bm = (TH1D*)f_bsmm->Get("E_Scint DirectHit Lev0eV (B-)");
	the_pizza.h_bsmm_fss_tp = (TH1D*)f_bsmm->Get("E_Scint ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_fss_tm = (TH1D*)f_bsmm->Get("E_Scint ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_fss_bp = (TH1D*)f_bsmm->Get("E_Scint ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_fss_bm = (TH1D*)f_bsmm->Get("E_Scint ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmm_bss_tp = (TH1D*)f_bsmm->Get("E_Scint BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bss_tm = (TH1D*)f_bsmm->Get("E_Scint BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bss_bp = (TH1D*)f_bsmm->Get("E_Scint BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bss_bm = (TH1D*)f_bsmm->Get("E_Scint BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmm_bs_tp = (TH1D*)f_bsmm->Get("E_Scint BackScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bs_tm = (TH1D*)f_bsmm->Get("E_Scint BackScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bs_bp = (TH1D*)f_bsmm->Get("E_Scint BackScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bs_bm = (TH1D*)f_bsmm->Get("E_Scint BackScatter Lev0eV (B-)");
	*/
	
	the_pizza.h_bsmm_direct_tp = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmm_direct_tm = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmm_direct_bp = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmm_direct_bm = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmm_fss_tp = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmm_fss_tm = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmm_fss_bp = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmm_fss_bm = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmm_bss_tp = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmm_bss_tm = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmm_bss_bp = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmm_bss_bm = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmm_bs_tp = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmm_bs_tm = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmm_bs_bp = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmm_bs_bm = (TH1D*)f_bsmm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	
	the_pizza.h_bsmm_direct_tp -> SetName("E_Scint BSM- DirectHit Lev0eV (T+)");
	the_pizza.h_bsmm_direct_tm -> SetName("E_Scint BSM- DirectHit Lev0eV (T-)");
	the_pizza.h_bsmm_direct_bp -> SetName("E_Scint BSM- DirectHit Lev0eV (B+)");
	the_pizza.h_bsmm_direct_bm -> SetName("E_Scint BSM- DirectHit Lev0eV (B-)");
	the_pizza.h_bsmm_direct_tp -> SetTitle("E_Scint BSM- DirectHit Lev0eV (T+)");
	the_pizza.h_bsmm_direct_tm -> SetTitle("E_Scint BSM- DirectHit Lev0eV (T-)");
	the_pizza.h_bsmm_direct_bp -> SetTitle("E_Scint BSM- DirectHit Lev0eV (B+)");
	the_pizza.h_bsmm_direct_bm -> SetTitle("E_Scint BSM- DirectHit Lev0eV (B-)");
	
	the_pizza.h_bsmm_fss_tp -> SetName("E_Scint BSM- ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_fss_tm -> SetName("E_Scint BSM- ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_fss_bp -> SetName("E_Scint BSM- ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_fss_bm -> SetName("E_Scint BSM- ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmm_fss_tp -> SetTitle("E_Scint BSM- ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_fss_tm -> SetTitle("E_Scint BSM- ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_fss_bp -> SetTitle("E_Scint BSM- ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_fss_bm -> SetTitle("E_Scint BSM- ForwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmm_bss_tp -> SetName("E_Scint BSM- BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bss_tm -> SetName("E_Scint BSM- BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bss_bp -> SetName("E_Scint BSM- BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bss_bm -> SetName("E_Scint BSM- BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmm_bss_tp -> SetTitle("E_Scint BSM- BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bss_tm -> SetTitle("E_Scint BSM- BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bss_bp -> SetTitle("E_Scint BSM- BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bss_bm -> SetTitle("E_Scint BSM- BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmm_bs_tp -> SetName("E_Scint BSM- BackScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bs_tm -> SetName("E_Scint BSM- BackScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bs_bp -> SetName("E_Scint BSM- BackScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bs_bm -> SetName("E_Scint BSM- BackScatter Lev0eV (B-)");
	the_pizza.h_bsmm_bs_tp -> SetTitle("E_Scint BSM- BackScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bs_tm -> SetTitle("E_Scint BSM- BackScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bs_bp -> SetTitle("E_Scint BSM- BackScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bs_bm -> SetTitle("E_Scint BSM- BackScatter Lev0eV (B-)");
	
	cout << "BSMM spectra are got and modded." << endl;
	
	/*
	int N_rebin = 10;
	cout << "Rebinning..." << endl;
	the_pizza.h_sm_direct_tp -> Rebin(N_rebin);
	the_pizza.h_sm_direct_tm -> Rebin(N_rebin);
	the_pizza.h_sm_direct_bp -> Rebin(N_rebin);
	the_pizza.h_sm_direct_bm -> Rebin(N_rebin);
	the_pizza.h_sm_fss_tp -> Rebin(N_rebin);
	the_pizza.h_sm_fss_tm -> Rebin(N_rebin);
	the_pizza.h_sm_fss_bp -> Rebin(N_rebin);
	the_pizza.h_sm_fss_bm -> Rebin(N_rebin);
	the_pizza.h_sm_bss_tp -> Rebin(N_rebin);
	the_pizza.h_sm_bss_tm -> Rebin(N_rebin);
	the_pizza.h_sm_bss_bp -> Rebin(N_rebin);
	the_pizza.h_sm_bss_bm -> Rebin(N_rebin);
	the_pizza.h_sm_bs_tp -> Rebin(N_rebin);
	the_pizza.h_sm_bs_tm -> Rebin(N_rebin);
	the_pizza.h_sm_bs_bp -> Rebin(N_rebin);
	the_pizza.h_sm_bs_bm -> Rebin(N_rebin);
	//
	the_pizza.h_bsmp_direct_tp -> Rebin(N_rebin);
	the_pizza.h_bsmp_direct_tm -> Rebin(N_rebin);
	the_pizza.h_bsmp_direct_bp -> Rebin(N_rebin);
	the_pizza.h_bsmp_direct_bm -> Rebin(N_rebin);
	the_pizza.h_bsmp_fss_tp -> Rebin(N_rebin);
	the_pizza.h_bsmp_fss_tm -> Rebin(N_rebin);
	the_pizza.h_bsmp_fss_bp -> Rebin(N_rebin);
	the_pizza.h_bsmp_fss_bm -> Rebin(N_rebin);
	the_pizza.h_bsmp_bss_tp -> Rebin(N_rebin);
	the_pizza.h_bsmp_bss_tm -> Rebin(N_rebin);
	the_pizza.h_bsmp_bss_bp -> Rebin(N_rebin);
	the_pizza.h_bsmp_bss_bm -> Rebin(N_rebin);
	the_pizza.h_bsmp_bs_tp -> Rebin(N_rebin);
	the_pizza.h_bsmp_bs_tm -> Rebin(N_rebin);
	the_pizza.h_bsmp_bs_bp -> Rebin(N_rebin);
	the_pizza.h_bsmp_bs_bm -> Rebin(N_rebin);
	//
	the_pizza.h_bsmm_direct_tp -> Rebin(N_rebin);
	the_pizza.h_bsmm_direct_tm -> Rebin(N_rebin);
	the_pizza.h_bsmm_direct_bp -> Rebin(N_rebin);
	the_pizza.h_bsmm_direct_bm -> Rebin(N_rebin);
	the_pizza.h_bsmm_fss_tp -> Rebin(N_rebin);
	the_pizza.h_bsmm_fss_tm -> Rebin(N_rebin);
	the_pizza.h_bsmm_fss_bp -> Rebin(N_rebin);
	the_pizza.h_bsmm_fss_bm -> Rebin(N_rebin);
	the_pizza.h_bsmm_bss_tp -> Rebin(N_rebin);
	the_pizza.h_bsmm_bss_tm -> Rebin(N_rebin);
	the_pizza.h_bsmm_bss_bp -> Rebin(N_rebin);
	the_pizza.h_bsmm_bss_bm -> Rebin(N_rebin);
	the_pizza.h_bsmm_bs_tp -> Rebin(N_rebin);
	the_pizza.h_bsmm_bs_tm -> Rebin(N_rebin);
	the_pizza.h_bsmm_bs_bp -> Rebin(N_rebin);
	the_pizza.h_bsmm_bs_bm -> Rebin(N_rebin);
	*/
	
	
	// Background spectra:
	the_pizza.h_bg_tp = (TH1D*)f_bg->Get("E_Scint Sum:  All BG, Face+Edge, (T+)");
	the_pizza.h_bg_tm = (TH1D*)f_bg->Get("E_Scint Sum:  All BG, Face+Edge, (T-)");
	the_pizza.h_bg_bp = (TH1D*)f_bg->Get("E_Scint Sum:  All BG, Face+Edge, (B+)");
	the_pizza.h_bg_bm = (TH1D*)f_bg->Get("E_Scint Sum:  All BG, Face+Edge, (B-)");
	the_pizza.h_bg_tp -> SetName("E_Scint Summed BG in TOF region (T+)");
	the_pizza.h_bg_tm -> SetName("E_Scint Summed BG in TOF region (T-)");
	the_pizza.h_bg_bp -> SetName("E_Scint Summed BG in TOF region (B+)");
	the_pizza.h_bg_bm -> SetName("E_Scint Summed BG in TOF region (B-)");
	the_pizza.h_bg_tp -> SetTitle("E_Scint Summed BG in TOF region (T+)");
	the_pizza.h_bg_tm -> SetTitle("E_Scint Summed BG in TOF region (T-)");
	the_pizza.h_bg_bp -> SetTitle("E_Scint Summed BG in TOF region (B+)");
	the_pizza.h_bg_bm -> SetTitle("E_Scint Summed BG in TOF region (B-)");
	
	cout << "bg spectra has been gotten and tweaked." << endl;
	// Normalization:  BG is normalized to match Data Set B.  
	// BG has Integral=351.456 in range.
	// What I really want to know for this is, what is the normalization for the total number of events in the TOF window of the simulation, given this normalization of the bg spectra.
	
	double the_G4_integral = 0.0;
	the_G4_integral += the_pizza.h_sm_direct_tp->Integral();
	the_G4_integral += the_pizza.h_sm_direct_tm->Integral();
	the_G4_integral += the_pizza.h_sm_direct_bp->Integral();
	the_G4_integral += the_pizza.h_sm_direct_bm->Integral();
	the_G4_integral += the_pizza.h_sm_fss_tp->Integral();
	the_G4_integral += the_pizza.h_sm_fss_tm->Integral();
	the_G4_integral += the_pizza.h_sm_fss_bp->Integral();
	the_G4_integral += the_pizza.h_sm_fss_bm->Integral();
	the_G4_integral += the_pizza.h_sm_bss_tp->Integral();
	the_G4_integral += the_pizza.h_sm_bss_tm->Integral();
	the_G4_integral += the_pizza.h_sm_bss_bp->Integral();
	the_G4_integral += the_pizza.h_sm_bss_bm->Integral();
	the_G4_integral += the_pizza.h_sm_bs_tp->Integral();
	the_G4_integral += the_pizza.h_sm_bs_tm->Integral();
	the_G4_integral += the_pizza.h_sm_bs_bp->Integral();
	the_G4_integral += the_pizza.h_sm_bs_bm->Integral();
	cout << "the_G4_integral = " << the_G4_integral << endl;
	
	double the_orig_BG_integral = 0.0;
	the_orig_BG_integral += the_pizza.h_bg_tp->Integral();
	the_orig_BG_integral += the_pizza.h_bg_tm->Integral();
	the_orig_BG_integral += the_pizza.h_bg_bp->Integral();
	the_orig_BG_integral += the_pizza.h_bg_bm->Integral();
	cout << "the_orig_BG_integral = " << the_orig_BG_integral << endl;
	
	// scale it so that  new_bg_integral/(the_G4_integral+new_bg_integral) = 351.456/366217.0
	// ie:  the_G4_integral*(351.456/366217.0) + new_bg_integral*(351.456/366217.0) = new_bg_integral
	//   :  (1.0 - (351.456/366217.0))*new_bg_integral = the_G4_integral*(351.456/366217.0)
	//   :  new_bg_integral = the_G4_integral * (351.456/366217.0)/(1.0 - (351.456/366217.0))
	double scale_B = (351.456/366217.0);
	double new_bg_integral = the_G4_integral*(scale_B)/(1.0 - scale_B);
	cout << "new_bg_integral = " << new_bg_integral << endl;
	
	the_pizza.h_bg_tp -> Scale(new_bg_integral / the_orig_BG_integral);
	the_pizza.h_bg_tm -> Scale(new_bg_integral / the_orig_BG_integral);
	the_pizza.h_bg_bp -> Scale(new_bg_integral / the_orig_BG_integral);
	the_pizza.h_bg_bm -> Scale(new_bg_integral / the_orig_BG_integral);
	the_pizza.h_bg_tp -> ResetStats();
	the_pizza.h_bg_tm -> ResetStats();
	the_pizza.h_bg_bp -> ResetStats();
	the_pizza.h_bg_bm -> ResetStats();
	
	cout << "Pizza created." << endl;
	//
	return the_pizza;
}

pizza get_pizza(string namestub, string matched_set, int threshold_index, double sigma_cut, int bb1_threshold, double bb1_maxr)
{
	/*
//	string output_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";	
	string additional_filename_info = make_additional_filename_info(threshold_index, sigma_cut, bb1_maxr);
	string bb_str = int_to_string(bb1_threshold);
	string the_pizza_stub = mini_pizzafname+additional_filename_info+"_Set"+matched_set+"_thr"+bb_str;
//	string pizza_fname=the_pizza_stub+".root";
//	pizza_path = output_path;
//	string the_pizza_fname = getmake_pizzaminifname();
	
//	string fname = output_path+mini_pizzafname+additional_filename_info+"_Set"+matched_set+"_thr"+bb_str+".root";
	*/
	//
	string the_pizza_stub = getmake_pizzastub(namestub, matched_set, threshold_index, sigma_cut, bb1_threshold, bb1_maxr);
	string fname = pizza_path+the_pizza_stub+".root";
	
	
	TFile * pizzafile   = new TFile( (fname).c_str() );
	pizza the_pizza;
	
	the_pizza.pizza_stub  = the_pizza_stub;
	
	the_pizza.matched_set     = matched_set;
	the_pizza.threshold_index = threshold_index;
	the_pizza.sigma_cut       = sigma_cut;
	the_pizza.bb1_threshold   = bb1_threshold;
	the_pizza.bb1_maxr        = bb1_maxr;
	
	//
	the_pizza.h_sm_direct_tp = (TH1D*)pizzafile->Get("E_Scint SM DirectHit Lev0eV (T+)");
	the_pizza.h_sm_direct_tm = (TH1D*)pizzafile->Get("E_Scint SM DirectHit Lev0eV (T-)");
	the_pizza.h_sm_direct_bp = (TH1D*)pizzafile->Get("E_Scint SM DirectHit Lev0eV (B+)");
	the_pizza.h_sm_direct_bm = (TH1D*)pizzafile->Get("E_Scint SM DirectHit Lev0eV (B-)");
	
	the_pizza.h_sm_fss_tp = (TH1D*)pizzafile->Get("E_Scint SM ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_fss_tm = (TH1D*)pizzafile->Get("E_Scint SM ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_fss_bp = (TH1D*)pizzafile->Get("E_Scint SM ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_fss_bm = (TH1D*)pizzafile->Get("E_Scint SM ForwardSideScatter Lev0eV (B-)");

	the_pizza.h_sm_bss_tp = (TH1D*)pizzafile->Get("E_Scint SM BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_bss_tm = (TH1D*)pizzafile->Get("E_Scint SM BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_bss_bp = (TH1D*)pizzafile->Get("E_Scint SM BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_bss_bm = (TH1D*)pizzafile->Get("E_Scint SM BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_sm_bs_tp = (TH1D*)pizzafile->Get("E_Scint SM BackScatter Lev0eV (T+)");
	the_pizza.h_sm_bs_tm = (TH1D*)pizzafile->Get("E_Scint SM BackScatter Lev0eV (T-)");
	the_pizza.h_sm_bs_bp = (TH1D*)pizzafile->Get("E_Scint SM BackScatter Lev0eV (B+)");
	the_pizza.h_sm_bs_bm = (TH1D*)pizzafile->Get("E_Scint SM BackScatter Lev0eV (B-)");
	
	//
	the_pizza.h_bsmp_direct_tp = (TH1D*)pizzafile->Get("E_Scint BSM+ DirectHit Lev0eV (T+)");
	the_pizza.h_bsmp_direct_tm = (TH1D*)pizzafile->Get("E_Scint BSM+ DirectHit Lev0eV (T-)");
	the_pizza.h_bsmp_direct_bp = (TH1D*)pizzafile->Get("E_Scint BSM+ DirectHit Lev0eV (B+)");
	the_pizza.h_bsmp_direct_bm = (TH1D*)pizzafile->Get("E_Scint BSM+ DirectHit Lev0eV (B-)");
	
	the_pizza.h_bsmp_fss_tp = (TH1D*)pizzafile->Get("E_Scint BSM+ ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_fss_tm = (TH1D*)pizzafile->Get("E_Scint BSM+ ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_fss_bp = (TH1D*)pizzafile->Get("E_Scint BSM+ ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_fss_bm = (TH1D*)pizzafile->Get("E_Scint BSM+ ForwardSideScatter Lev0eV (B-)");

	the_pizza.h_bsmp_bss_tp = (TH1D*)pizzafile->Get("E_Scint BSM+ BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bss_tm = (TH1D*)pizzafile->Get("E_Scint BSM+ BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bss_bp = (TH1D*)pizzafile->Get("E_Scint BSM+ BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bss_bm = (TH1D*)pizzafile->Get("E_Scint BSM+ BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmp_bs_tp = (TH1D*)pizzafile->Get("E_Scint BSM+ BackScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bs_tm = (TH1D*)pizzafile->Get("E_Scint BSM+ BackScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bs_bp = (TH1D*)pizzafile->Get("E_Scint BSM+ BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm = (TH1D*)pizzafile->Get("E_Scint BSM+ BackScatter Lev0eV (B-)");
	
	
	//
	the_pizza.h_bsmm_direct_tp = (TH1D*)pizzafile->Get("E_Scint BSM- DirectHit Lev0eV (T+)");
	the_pizza.h_bsmm_direct_tm = (TH1D*)pizzafile->Get("E_Scint BSM- DirectHit Lev0eV (T-)");
	the_pizza.h_bsmm_direct_bp = (TH1D*)pizzafile->Get("E_Scint BSM- DirectHit Lev0eV (B+)");
	the_pizza.h_bsmm_direct_bm = (TH1D*)pizzafile->Get("E_Scint BSM- DirectHit Lev0eV (B-)");
	
	the_pizza.h_bsmm_fss_tp = (TH1D*)pizzafile->Get("E_Scint BSM- ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_fss_tm = (TH1D*)pizzafile->Get("E_Scint BSM- ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_fss_bp = (TH1D*)pizzafile->Get("E_Scint BSM- ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_fss_bm = (TH1D*)pizzafile->Get("E_Scint BSM- ForwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmm_bss_tp = (TH1D*)pizzafile->Get("E_Scint BSM- BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bss_tm = (TH1D*)pizzafile->Get("E_Scint BSM- BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bss_bp = (TH1D*)pizzafile->Get("E_Scint BSM- BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bss_bm = (TH1D*)pizzafile->Get("E_Scint BSM- BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmm_bs_tp = (TH1D*)pizzafile->Get("E_Scint BSM- BackScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bs_tm = (TH1D*)pizzafile->Get("E_Scint BSM- BackScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bs_bp = (TH1D*)pizzafile->Get("E_Scint BSM- BackScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bs_bm = (TH1D*)pizzafile->Get("E_Scint BSM- BackScatter Lev0eV (B-)");

//	cout << "h_bsmm_bs_tp=" << the_pizza.h_bsmm_bs_tp << endl;
//	cout << "h_bsmm_bs_tm=" << the_pizza.h_bsmm_bs_tm << endl;
//	cout << "h_bsmm_bs_bp=" << the_pizza.h_bsmm_bs_bp << endl;
//	cout << "h_bsmm_bs_bm=" << the_pizza.h_bsmm_bs_bm << endl;
	
	//
	the_pizza.h_bg_tp = (TH1D*)pizzafile->Get("E_Scint Summed BG in TOF region (T+)");
	the_pizza.h_bg_tm = (TH1D*)pizzafile->Get("E_Scint Summed BG in TOF region (T-)");
	the_pizza.h_bg_bp = (TH1D*)pizzafile->Get("E_Scint Summed BG in TOF region (B+)");
	the_pizza.h_bg_bm = (TH1D*)pizzafile->Get("E_Scint Summed BG in TOF region (B-)");
	
	cout << "Pizza acquired!" << endl;
	return the_pizza;
}

pizza clone_pizza(pizza the_original)
{
	pizza the_new_pizza;
	
	the_new_pizza.pizza_stub = the_original.pizza_stub;
//	the_new_pizza.pizza_path  = the_original.pizza_path;
	//
	
	the_new_pizza.matched_set     = the_original.matched_set;
	the_new_pizza.threshold_index = the_original.threshold_index;
	the_new_pizza.sigma_cut       = the_original.sigma_cut;
	the_new_pizza.bb1_threshold   = the_original.bb1_threshold;
	the_new_pizza.bb1_maxr        = the_original.bb1_maxr;
	
	the_new_pizza.h_sm_direct_tp = (TH1D*)the_original.h_sm_direct_tp->Clone();
	the_new_pizza.h_sm_direct_tm = (TH1D*)the_original.h_sm_direct_tm->Clone();
	the_new_pizza.h_sm_direct_bp = (TH1D*)the_original.h_sm_direct_bp->Clone();
	the_new_pizza.h_sm_direct_bm = (TH1D*)the_original.h_sm_direct_bm->Clone();

	the_new_pizza.h_sm_fss_tp = (TH1D*)the_original.h_sm_fss_tp->Clone();
	the_new_pizza.h_sm_fss_tm = (TH1D*)the_original.h_sm_fss_tm->Clone();
	the_new_pizza.h_sm_fss_bp = (TH1D*)the_original.h_sm_fss_bp->Clone();
	the_new_pizza.h_sm_fss_bm = (TH1D*)the_original.h_sm_fss_bm->Clone();

	the_new_pizza.h_sm_bss_tp = (TH1D*)the_original.h_sm_bss_tp->Clone();
	the_new_pizza.h_sm_bss_tm = (TH1D*)the_original.h_sm_bss_tm->Clone();
	the_new_pizza.h_sm_bss_bp = (TH1D*)the_original.h_sm_bss_bp->Clone();
	the_new_pizza.h_sm_bss_bm = (TH1D*)the_original.h_sm_bss_bm->Clone();
	
	the_new_pizza.h_sm_bs_tp = (TH1D*)the_original.h_sm_bs_tp->Clone();
	the_new_pizza.h_sm_bs_tm = (TH1D*)the_original.h_sm_bs_tm->Clone();
	the_new_pizza.h_sm_bs_bp = (TH1D*)the_original.h_sm_bs_bp->Clone();
	the_new_pizza.h_sm_bs_bm = (TH1D*)the_original.h_sm_bs_bm->Clone();
	
	//
	the_new_pizza.h_bsmp_direct_tp = (TH1D*)the_original.h_bsmp_direct_tp->Clone();
	the_new_pizza.h_bsmp_direct_tm = (TH1D*)the_original.h_bsmp_direct_tm->Clone();
	the_new_pizza.h_bsmp_direct_bp = (TH1D*)the_original.h_bsmp_direct_bp->Clone();
	the_new_pizza.h_bsmp_direct_bm = (TH1D*)the_original.h_bsmp_direct_bm->Clone();

	the_new_pizza.h_bsmp_fss_tp = (TH1D*)the_original.h_bsmp_fss_tp->Clone();
	the_new_pizza.h_bsmp_fss_tm = (TH1D*)the_original.h_bsmp_fss_tm->Clone();
	the_new_pizza.h_bsmp_fss_bp = (TH1D*)the_original.h_bsmp_fss_bp->Clone();
	the_new_pizza.h_bsmp_fss_bm = (TH1D*)the_original.h_bsmp_fss_bm->Clone();

	the_new_pizza.h_bsmp_bss_tp = (TH1D*)the_original.h_bsmp_bss_tp->Clone();
	the_new_pizza.h_bsmp_bss_tm = (TH1D*)the_original.h_bsmp_bss_tm->Clone();
	the_new_pizza.h_bsmp_bss_bp = (TH1D*)the_original.h_bsmp_bss_bp->Clone();
	the_new_pizza.h_bsmp_bss_bm = (TH1D*)the_original.h_bsmp_bss_bm->Clone();
	
	the_new_pizza.h_bsmp_bs_tp = (TH1D*)the_original.h_bsmp_bs_tp->Clone();
	the_new_pizza.h_bsmp_bs_tm = (TH1D*)the_original.h_bsmp_bs_tm->Clone();
	the_new_pizza.h_bsmp_bs_bp = (TH1D*)the_original.h_bsmp_bs_bp->Clone();
	the_new_pizza.h_bsmp_bs_bm = (TH1D*)the_original.h_bsmp_bs_bm->Clone();
	
	//
	the_new_pizza.h_bsmm_direct_tp = (TH1D*)the_original.h_bsmm_direct_tp->Clone();
	the_new_pizza.h_bsmm_direct_tm = (TH1D*)the_original.h_bsmm_direct_tm->Clone();
	the_new_pizza.h_bsmm_direct_bp = (TH1D*)the_original.h_bsmm_direct_bp->Clone();
	the_new_pizza.h_bsmm_direct_bm = (TH1D*)the_original.h_bsmm_direct_bm->Clone();

	the_new_pizza.h_bsmm_fss_tp = (TH1D*)the_original.h_bsmm_fss_tp->Clone();
	the_new_pizza.h_bsmm_fss_tm = (TH1D*)the_original.h_bsmm_fss_tm->Clone();
	the_new_pizza.h_bsmm_fss_bp = (TH1D*)the_original.h_bsmm_fss_bp->Clone();
	the_new_pizza.h_bsmm_fss_bm = (TH1D*)the_original.h_bsmm_fss_bm->Clone();

	the_new_pizza.h_bsmm_bss_tp = (TH1D*)the_original.h_bsmm_bss_tp->Clone();
	the_new_pizza.h_bsmm_bss_tm = (TH1D*)the_original.h_bsmm_bss_tm->Clone();
	the_new_pizza.h_bsmm_bss_bp = (TH1D*)the_original.h_bsmm_bss_bp->Clone();
	the_new_pizza.h_bsmm_bss_bm = (TH1D*)the_original.h_bsmm_bss_bm->Clone();
	
	the_new_pizza.h_bsmm_bs_tp = (TH1D*)the_original.h_bsmm_bs_tp->Clone();
	the_new_pizza.h_bsmm_bs_tm = (TH1D*)the_original.h_bsmm_bs_tm->Clone();
	the_new_pizza.h_bsmm_bs_bp = (TH1D*)the_original.h_bsmm_bs_bp->Clone();
	the_new_pizza.h_bsmm_bs_bm = (TH1D*)the_original.h_bsmm_bs_bm->Clone();
	//
	
	the_new_pizza.h_bg_tp = (TH1D*)the_original.h_bg_tp->Clone();
	the_new_pizza.h_bg_tm = (TH1D*)the_original.h_bg_tm->Clone();
	the_new_pizza.h_bg_bp = (TH1D*)the_original.h_bg_bp->Clone();
	the_new_pizza.h_bg_bm = (TH1D*)the_original.h_bg_bm->Clone();
	
	return the_new_pizza;
}

void store_pizza(pizza the_pizza, string namestub)
{
	
//	string output_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";
//	pizza_path = output_path;
	
//	string additional_filename_info = make_additional_filename_info(the_pizza.threshold_index, the_pizza.sigma_cut, the_pizza.bb1_maxr);
//	string bb_str = int_to_string(the_pizza.bb1_threshold);
//	string pizza_stub = mini_pizzafname+additional_filename_info+"_Set"+the_pizza.matched_set+"_thr"+bb_str;//+".root";
	
	//
	string the_pizza_stub = getmake_pizzastub(namestub, the_pizza.matched_set, the_pizza.threshold_index, the_pizza.sigma_cut, the_pizza.bb1_threshold, the_pizza.bb1_maxr);
	string fname = pizza_path+the_pizza_stub+".root";
	
	TFile * pizzafile   = new TFile( (fname).c_str(), "RECREATE");
	pizzafile->cd();

	the_pizza.h_sm_direct_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_direct_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_direct_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_direct_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_fss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_fss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_fss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_fss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bs_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bs_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bs_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bs_bm -> Write("",TObject::kOverwrite);
	
	the_pizza.h_bsmp_direct_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_direct_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_direct_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_direct_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_fss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_fss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_fss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_fss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_bm -> Write("",TObject::kOverwrite);
	
	the_pizza.h_bsmm_direct_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_direct_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_direct_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_direct_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_fss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_fss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_fss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_fss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bs_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bs_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bs_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bs_bm -> Write("",TObject::kOverwrite);
	
	the_pizza.h_bg_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bg_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bg_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bg_bm -> Write("",TObject::kOverwrite);
	
	pizzafile -> Close();
	cout << "Pizza is stored in file: " << fname << endl;  // GenEnergyAsym.root
	return;
}


TH1D * cook(pizza the_pizza, toppings the_toppings)  // it's still a pizza with the default amount of scattering!  :(
{
	double A_0     = -0.568045;
	double opposite_A = A_0 - (the_toppings.Abeta - A_0);
	
	double alpha          = getfast_alpha_from_Abeta_and_bFierz(the_toppings.Abeta, the_toppings.bFierz);
	double beta           = getfast_beta_from_Abeta_and_bFierz(the_toppings.Abeta,  the_toppings.bFierz);
	double opposite_alpha = getfast_alpha_from_Abeta_and_bFierz(opposite_A,         the_toppings.bFierz);
	double opposite_beta  = getfast_beta_from_Abeta_and_bFierz(opposite_A,          the_toppings.bFierz);
	string asym_title     = "Superratio Asymmetry (simple cook):  A="+fake_to_string(the_toppings.Abeta)+", b="+fake_to_string(the_toppings.bFierz)+";  BS="+fake_to_string(the_toppings.BS_scale*100.0)+"\%, SS="+fake_to_string(the_toppings.SS_scale*100.0)+"\%, BG="+fake_to_string(the_toppings.BG_scale*100.0)+"\%";
	double dA  = A_0*(alpha - 1.0);
	double dbA = A_0*(beta  - 1.0);
	
	double costheta_avg_dir;
	double costheta_avg_fss;
	double costheta_avg_bss;
	double costheta_avg_bs;
	
	bool is_plus;
	if(the_toppings.bFierz<=0)
	{
		is_plus = true;
	}
	else
	{
		is_plus = false;
	}
	
	// the new asymmetry be like:  alpha*A_SM + beta*A_BSM.
	// for the individual spectra?  .. scale them all like that.  Or, scale at least the direct ones like that?
	// ....Nope.
	
	//	double get_asymmetry(double r1p, double r1m, double r2p, double r2m)
	//	A = ( sqrt(r1p*r2m) - sqrt(r1m*r2p) ) / ( sqrt(r1p*r2m) + sqrt(r1m*r2p) );
	//    ~ (R_tp - R_bp) / (R_tp + R_bp)
	//    ~ ((1+AcosT) - (1-AcosT)) / ((1+AcosT) + (1-AcosT))
	//    = 2AcosT / 2 
	//    = AcosT
	// eg, R_tp = R*(1+AcosT)
	//     R_tp-->R*(1+(A+dA)cosT)
	// for A-->A+dA, 
	// A+dA ~ ((1+(A+dA)cosT) - (1-(A+dA)cosT)) / ((1+(A+dA)cosT) + (1-(A+dA)cosT))
	//      = (2*(A+dA)) / (2)
	// A_spect = alpha*A_SM_spect + beta*A_BSM_spect
	//
	// Suppose:
	// A_spect = alpha*A_SM_spect
	// A_val   = A_val_orig + dA_val
	// Take:  R_tp --> R_tp*(1+dA/A)  ?????
	// Do this.  R_tp --> R_tp*(1+(A+dA)cosT)/(1+AcosT) 
	//                 =  R_tp*( 1 + dA/(1+AcosT) )
	//           R_tp --> R_tp + dA  (srsly?)
	//           R_tp->Scale(1.0+dA/(1+A_0))
	//           +/- dA is what happens when you want to scale the SM asymmetry by alpha.
	// 		really, I did +/- dA wrong..  I should use alpha to make it happen, not the raw difference in Abeta.
	// 		A_0_SM*alpha = A_0_SM + dA
	// 		dA = A_0_SM*alpha - A_0_SM
	// 		dA = A_0_SM*(alpha - 1.0);
	// Try this too, for the BSM ones?:
	//           R_tp --> R_tp + dbA
	//           R_tp->Scale(1.0+dbA/(1+A_0))
	// but what's dbA?  Presumably it's the thing that happens when you want to scale the BSM asymmetry by beta.
	// 
	
	
	
	// --- *** --- //
	// Load histograms:
	TH1D *h_sm_tp, *h_bsm_tp;
	TH1D *h_sm_tm, *h_bsm_tm;
	TH1D *h_sm_bp, *h_bsm_bp;
	TH1D *h_sm_bm, *h_bsm_bm;
	
	// Direct:
	TH1D *h_sm_dir_tp, *h_bsm_dir_tp;
	TH1D *h_sm_dir_tm, *h_bsm_dir_tm;
	TH1D *h_sm_dir_bp, *h_bsm_dir_bp;
	TH1D *h_sm_dir_bm, *h_bsm_dir_bm;
	// forward side-scatter
	TH1D *h_sm_fss_tp, *h_bsm_fss_tp;
	TH1D *h_sm_fss_tm, *h_bsm_fss_tm;
	TH1D *h_sm_fss_bp, *h_bsm_fss_bp;
	TH1D *h_sm_fss_bm, *h_bsm_fss_bm;
	// backward side-scatter
	TH1D *h_sm_bss_tp, *h_bsm_bss_tp;
	TH1D *h_sm_bss_tm, *h_bsm_bss_tm;
	TH1D *h_sm_bss_bp, *h_bsm_bss_bp;
	TH1D *h_sm_bss_bm, *h_bsm_bss_bm;
	// backscatter
	TH1D *h_sm_bs_tp, *h_bsm_bs_tp;
	TH1D *h_sm_bs_tm, *h_bsm_bs_tm;
	TH1D *h_sm_bs_bp, *h_bsm_bs_bp;
	TH1D *h_sm_bs_bm, *h_bsm_bs_bm;
	// background
	TH1D * h_bg_tp;
	TH1D * h_bg_tm;
	TH1D * h_bg_bp;
	TH1D * h_bg_bm;
	
	// --- *** --- //
	// Direct:
	h_sm_dir_tp = the_pizza.h_sm_direct_tp;
	h_sm_dir_tm = the_pizza.h_sm_direct_tm;
	h_sm_dir_bp = the_pizza.h_sm_direct_bp;
	h_sm_dir_bm = the_pizza.h_sm_direct_bm;
	// forward side-scatter
	h_sm_fss_tp = the_pizza.h_sm_fss_tp;
	h_sm_fss_tm = the_pizza.h_sm_fss_tm;
	h_sm_fss_bp = the_pizza.h_sm_fss_bp;
	h_sm_fss_bm = the_pizza.h_sm_fss_bm;
	// backward side-scatter
	h_sm_bss_tp = the_pizza.h_sm_bss_tp;
	h_sm_bss_tm = the_pizza.h_sm_bss_tm;
	h_sm_bss_bp = the_pizza.h_sm_bss_bp;
	h_sm_bss_bm = the_pizza.h_sm_bss_bm;
	// backscatter
	h_sm_bs_tp = the_pizza.h_sm_bs_tp;
	h_sm_bs_tm = the_pizza.h_sm_bs_tm;
	h_sm_bs_bp = the_pizza.h_sm_bs_bp;
	h_sm_bs_bm = the_pizza.h_sm_bs_bm;
	// Background:
	h_bg_tp = the_pizza.h_bg_tp;
	h_bg_tm = the_pizza.h_bg_tm;
	h_bg_bp = the_pizza.h_bg_bp;
	h_bg_bm = the_pizza.h_bg_bm;
	
	if(is_plus)
	{
		h_bsm_dir_tp = the_pizza.h_bsmp_direct_tp;
		h_bsm_dir_tm = the_pizza.h_bsmp_direct_tm;
		h_bsm_dir_bp = the_pizza.h_bsmp_direct_bp;
		h_bsm_dir_bm = the_pizza.h_bsmp_direct_bm;
		
		h_bsm_fss_tp = the_pizza.h_bsmp_fss_tp;
		h_bsm_fss_tm = the_pizza.h_bsmp_fss_tm;
		h_bsm_fss_bp = the_pizza.h_bsmp_fss_bp;
		h_bsm_fss_bm = the_pizza.h_bsmp_fss_bm;
		
		h_bsm_bss_tp = the_pizza.h_bsmp_bss_tp;
		h_bsm_bss_tm = the_pizza.h_bsmp_bss_tm;
		h_bsm_bss_bp = the_pizza.h_bsmp_bss_bp;
		h_bsm_bss_bm = the_pizza.h_bsmp_bss_bm;
		
		h_bsm_bs_tp = the_pizza.h_bsmp_bs_tp;
		h_bsm_bs_tm = the_pizza.h_bsmp_bs_tm;
		h_bsm_bs_bp = the_pizza.h_bsmp_bs_bp;
		h_bsm_bs_bm = the_pizza.h_bsmp_bs_bm;
	}
	else
	{
		h_bsm_dir_tp = the_pizza.h_bsmm_direct_tp;
		h_bsm_dir_tm = the_pizza.h_bsmm_direct_tm;
		h_bsm_dir_bp = the_pizza.h_bsmm_direct_bp;
		h_bsm_dir_bm = the_pizza.h_bsmm_direct_bm;
		
		h_bsm_fss_tp = the_pizza.h_bsmm_fss_tp;
		h_bsm_fss_tm = the_pizza.h_bsmm_fss_tm;
		h_bsm_fss_bp = the_pizza.h_bsmm_fss_bp;
		h_bsm_fss_bm = the_pizza.h_bsmm_fss_bm;
		
		h_bsm_bss_tp = the_pizza.h_bsmm_bss_tp;
		h_bsm_bss_tm = the_pizza.h_bsmm_bss_tm;
		h_bsm_bss_bp = the_pizza.h_bsmm_bss_bp;
		h_bsm_bss_bm = the_pizza.h_bsmm_bss_bm;
		
		h_bsm_bs_tp = the_pizza.h_bsmm_bs_tp;
		h_bsm_bs_tm = the_pizza.h_bsmm_bs_tm;
		h_bsm_bs_bp = the_pizza.h_bsmm_bs_bp;
		h_bsm_bs_bm = the_pizza.h_bsmm_bs_bm;
	}
	
	// Direct:
	h_sm_tp = (TH1D*)h_sm_dir_tp -> Clone("SM E_scint (T+)");
	h_sm_tm = (TH1D*)h_sm_dir_tm -> Clone("SM E_scint (T-)");
	h_sm_bp = (TH1D*)h_sm_dir_bp -> Clone("SM E_scint (B+)");
	h_sm_bm = (TH1D*)h_sm_dir_bm -> Clone("SM E_scint (B-)");
//	
	h_bsm_tp = (TH1D*)h_bsm_dir_tp -> Clone("BSM E_scint (T+)");
	h_bsm_tm = (TH1D*)h_bsm_dir_tm -> Clone("BSM E_scint (T-)");
	h_bsm_bp = (TH1D*)h_bsm_dir_bp -> Clone("BSM E_scint (B+)");
	h_bsm_bm = (TH1D*)h_bsm_dir_bm -> Clone("BSM E_scint (B-)");
	
	
	
	// forward side-scatter
	h_sm_tp ->Add(h_sm_fss_tp, the_toppings.SS_scale);
	h_sm_tm ->Add(h_sm_fss_tm, the_toppings.SS_scale);
	h_sm_bp ->Add(h_sm_fss_bp, the_toppings.SS_scale);
	h_sm_bm ->Add(h_sm_fss_bm, the_toppings.SS_scale);

	h_bsm_tp->Add(h_bsm_fss_tp, the_toppings.SS_scale);
	h_bsm_tm->Add(h_bsm_fss_tm, the_toppings.SS_scale);
	h_bsm_bp->Add(h_bsm_fss_bp, the_toppings.SS_scale);
	h_bsm_bm->Add(h_bsm_fss_bm, the_toppings.SS_scale);
		
	// backward side-scatter
	h_sm_tp ->Add(h_sm_bss_tp, the_toppings.SS_scale);
	h_sm_tm ->Add(h_sm_bss_tm, the_toppings.SS_scale);
	h_sm_bp ->Add(h_sm_bss_bp, the_toppings.SS_scale);
	h_sm_bm ->Add(h_sm_bss_bm, the_toppings.SS_scale);
	
	h_bsm_tp->Add(h_bsm_bss_tp, the_toppings.SS_scale);
	h_bsm_tm->Add(h_bsm_bss_tm, the_toppings.SS_scale);
	h_bsm_bp->Add(h_bsm_bss_bp, the_toppings.SS_scale);
	h_bsm_bm->Add(h_bsm_bss_bm, the_toppings.SS_scale);
	
	// backscatter
	h_sm_tp ->Add(h_sm_bs_tp,  the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bs_tm,  the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bs_bp,  the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bs_bm,  the_toppings.BS_scale);
	h_bsm_tp->Add(h_bsm_bs_tp, the_toppings.BS_scale);
	h_bsm_tm->Add(h_bsm_bs_tm, the_toppings.BS_scale);
	h_bsm_bp->Add(h_bsm_bs_bp, the_toppings.BS_scale);
	h_bsm_bm->Add(h_bsm_bs_bm, the_toppings.BS_scale);
	
	// Background:
	// Debug:  skip the background.
//	cout << "Nbins in h_sm_tp:  " << h_sm_tp->GetNbinsX() << ";\tNbins in h_bg_tp:  " << h_bg_tp->GetNbinsX() << endl;
	h_sm_tp ->Add(h_bg_tp,  the_toppings.BG_scale);
	h_sm_tm ->Add(h_bg_tm,  the_toppings.BG_scale);
	h_sm_bp ->Add(h_bg_bp,  the_toppings.BG_scale);
	h_sm_bm ->Add(h_bg_bm,  the_toppings.BG_scale);
	
	h_bsm_tp->Add(h_bg_tp, the_toppings.BG_scale);
	h_bsm_tm->Add(h_bg_tm, the_toppings.BG_scale);
	h_bsm_bp->Add(h_bg_bp, the_toppings.BG_scale);
	h_bsm_bm->Add(h_bg_bm, the_toppings.BG_scale);
//	cout << "But not here." << endl;
	
	// Assemble:
	h_sm_tp -> ResetStats();
	h_sm_tm -> ResetStats();
	h_sm_bp -> ResetStats();
	h_sm_bm -> ResetStats();
	TH1D * h_asym_sm  = make_superratioasymmetry_direct(h_sm_tp, h_sm_tm, h_sm_bp, h_sm_bm);
//	h_asym_sm->SetBit(TH1::kIsAverage);
	h_asym_sm->Scale(alpha);
//	TH1D* h_asym_sm_tmp = (TH1D*)h_asym_sm->Clone();
	
	h_bsm_tp -> ResetStats();
	h_bsm_tm -> ResetStats();
	h_bsm_bp -> ResetStats();
	h_bsm_bm -> ResetStats();
	TH1D * h_asym_bsm  = make_superratioasymmetry_direct(h_bsm_tp, h_bsm_tm, h_bsm_bp, h_bsm_bm);
	
//	h_asym_bsm->SetBit(TH1::kIsAverage);
	h_asym_bsm->Scale(beta);
	
	TH1D * h_asym = h_asym_sm;// (TH1D*)h_asym_sm->Clone();
	h_asym -> Add(h_asym_bsm);
	h_asym -> SetName(asym_title.c_str());
	h_asym -> SetTitle(asym_title.c_str());
	
	/*
	TCanvas * c0 = new TCanvas();
	c0->Divide(2,2);
	c0->cd(1);
	h_bsm_tp->Draw();
	c0->cd(2);
	h_bsm_tm->Draw();
	c0->cd(3);
	h_bsm_bp->Draw();
	c0->cd(4);
	h_bsm_bm->Draw();
	*/
//	TCanvas * c1 = new TCanvas();
//	h_asym_bsm->Draw();
//	TCanvas * c2 = new TCanvas();
//	h_asym_sm_tmp->Draw();
	
	
	delete h_sm_tp;
	delete h_sm_tm;
	delete h_sm_bp;
	delete h_sm_bm;

	delete h_bsm_tp;
	delete h_bsm_tm;
	delete h_bsm_bp;
	delete h_bsm_bm;
	
//	delete h_asym_sm;
	delete h_asym_bsm;
	
	
	return h_asym;
} // cook(...)

TH1D * simple_cook(pizza the_pizza, toppings the_toppings)  // it's still a pizza with the default amount of scattering!  :(
{
//	cout << "Got here though." << endl;
	
	double A_0     = -0.568045;
//	double dA = (the_toppings.Abeta - A_0);
//	double opposite_A = A_0 - dA;
	double opposite_A = A_0 - (the_toppings.Abeta - A_0);
	
	double alpha          = getfast_alpha_from_Abeta_and_bFierz(the_toppings.Abeta, the_toppings.bFierz);
	double beta           = getfast_beta_from_Abeta_and_bFierz(the_toppings.Abeta,  the_toppings.bFierz);
	double opposite_alpha = getfast_alpha_from_Abeta_and_bFierz(opposite_A,         the_toppings.bFierz);
	double opposite_beta  = getfast_beta_from_Abeta_and_bFierz(opposite_A,          the_toppings.bFierz);
	string asym_title     = "Superratio Asymmetry (simple cook):  A="+fake_to_string(the_toppings.Abeta)+", b="+fake_to_string(the_toppings.bFierz)+";  BS="+fake_to_string(the_toppings.BS_scale*100.0)+"\%, SS="+fake_to_string(the_toppings.SS_scale*100.0)+"\%, BG="+fake_to_string(the_toppings.BG_scale*100.0)+"\%";
//	double dA = the_toppings.Abeta - A_0;
	double dA  = A_0*(alpha - 1.0);
	double dbA = A_0*(beta  - 1.0);
	
	
//	cout << "bFierz=" << the_toppings.bFierz << ";\tbeta =" << beta  << ";\topposite_beta ="<< opposite_beta << endl;
	bool is_plus;
	if(the_toppings.bFierz<=0)
	{
		is_plus = true;
	}
	else
	{
		is_plus = false;
	//	beta=abs(beta);
	//	opposite_beta=abs(opposite_beta);
	}
//	cout << "Abeta =" << the_toppings.Abeta  << ";\talpha=" << alpha << ";\topposite_alpha="<< opposite_alpha << endl;
//	cout << "bFierz=" << the_toppings.bFierz << ";\tbeta =" << beta  << ";\topposite_beta ="<< opposite_beta << ";\tis_plus=" << is_plus << endl;
//	cout << "*" << endl;
	
	// the new asymmetry be like:  alpha*A_SM + beta*A_BSM.
	// for the individual spectra?  .. scale them all like that.  Or, scale at least the direct ones like that?
	// ....Nope.
	
	//	double get_asymmetry(double r1p, double r1m, double r2p, double r2m)
	//	A = ( sqrt(r1p*r2m) - sqrt(r1m*r2p) ) / ( sqrt(r1p*r2m) + sqrt(r1m*r2p) );
	//    ~ (R_tp - R_bp) / (R_tp + R_bp)
	//    ~ ((1+AcosT) - (1-AcosT)) / ((1+AcosT) + (1-AcosT))
	//    = 2AcosT / 2 
	//    = AcosT
	// eg, R_tp = R*(1+AcosT)
	//     R_tp-->R*(1+(A+dA)cosT)
	// for A-->A+dA, 
	// A+dA ~ ((1+(A+dA)cosT) - (1-(A+dA)cosT)) / ((1+(A+dA)cosT) + (1-(A+dA)cosT))
	//      = (2*(A+dA)) / (2)
	// A_spect = alpha*A_SM_spect + beta*A_BSM_spect
	//
	// Suppose:
	// A_spect = alpha*A_SM_spect
	// A_val   = A_val_orig + dA_val
	// Take:  R_tp --> R_tp*(1+dA/A)  ?????
	// Do this.  R_tp --> R_tp*(1+(A+dA)cosT)/(1+AcosT) 
	//                 =  R_tp*( 1 + dA/(1+AcosT) )
	//           R_tp --> R_tp + dA  (srsly?)
	//           R_tp->Scale(1.0+dA/(1+A_0))
	//           +/- dA is what happens when you want to scale the SM asymmetry by alpha.
	// 		really, I did +/- dA wrong..  I should use alpha to make it happen, not the raw difference in Abeta.
	// 		A_0_SM*alpha = A_0_SM + dA
	// 		dA = A_0_SM*alpha - A_0_SM
	// 		dA = A_0_SM*(alpha - 1.0);
	// Try this too, for the BSM ones?:
	//           R_tp --> R_tp + dbA
	//           R_tp->Scale(1.0+dbA/(1+A_0))
	// but what's dbA?  Presumably it's the thing that happens when you want to scale the BSM asymmetry by beta.
	// 
	
	
	
	// --- *** --- //
	// Load histograms:
	TH1D *h_sm_tp, *h_bsm_tp;
	TH1D *h_sm_tm, *h_bsm_tm;
	TH1D *h_sm_bp, *h_bsm_bp;
	TH1D *h_sm_bm, *h_bsm_bm;
	
	// Direct:
	TH1D *h_sm_dir_tp, *h_bsm_dir_tp;
	TH1D *h_sm_dir_tm, *h_bsm_dir_tm;
	TH1D *h_sm_dir_bp, *h_bsm_dir_bp;
	TH1D *h_sm_dir_bm, *h_bsm_dir_bm;
	// forward side-scatter
	TH1D *h_sm_fss_tp, *h_bsm_fss_tp;
	TH1D *h_sm_fss_tm, *h_bsm_fss_tm;
	TH1D *h_sm_fss_bp, *h_bsm_fss_bp;
	TH1D *h_sm_fss_bm, *h_bsm_fss_bm;
	// backward side-scatter
	TH1D *h_sm_bss_tp, *h_bsm_bss_tp;
	TH1D *h_sm_bss_tm, *h_bsm_bss_tm;
	TH1D *h_sm_bss_bp, *h_bsm_bss_bp;
	TH1D *h_sm_bss_bm, *h_bsm_bss_bm;
	// backscatter
	TH1D *h_sm_bs_tp, *h_bsm_bs_tp;
	TH1D *h_sm_bs_tm, *h_bsm_bs_tm;
	TH1D *h_sm_bs_bp, *h_bsm_bs_bp;
	TH1D *h_sm_bs_bm, *h_bsm_bs_bm;
	// background
	TH1D * h_bg_tp;
	TH1D * h_bg_tm;
	TH1D * h_bg_bp;
	TH1D * h_bg_bm;
	
	// --- *** --- //
	// Direct:
	h_sm_dir_tp = the_pizza.h_sm_direct_tp;
	h_sm_dir_tm = the_pizza.h_sm_direct_tm;
	h_sm_dir_bp = the_pizza.h_sm_direct_bp;
	h_sm_dir_bm = the_pizza.h_sm_direct_bm;
	// forward side-scatter
	h_sm_fss_tp = the_pizza.h_sm_fss_tp;
	h_sm_fss_tm = the_pizza.h_sm_fss_tm;
	h_sm_fss_bp = the_pizza.h_sm_fss_bp;
	h_sm_fss_bm = the_pizza.h_sm_fss_bm;
	// backward side-scatter
	h_sm_bss_tp = the_pizza.h_sm_bss_tp;
	h_sm_bss_tm = the_pizza.h_sm_bss_tm;
	h_sm_bss_bp = the_pizza.h_sm_bss_bp;
	h_sm_bss_bm = the_pizza.h_sm_bss_bm;
	// backscatter
	h_sm_bs_tp = the_pizza.h_sm_bs_tp;
	h_sm_bs_tm = the_pizza.h_sm_bs_tm;
	h_sm_bs_bp = the_pizza.h_sm_bs_bp;
	h_sm_bs_bm = the_pizza.h_sm_bs_bm;
	// Background:
	h_bg_tp = the_pizza.h_bg_tp;
	h_bg_tm = the_pizza.h_bg_tm;
	h_bg_bp = the_pizza.h_bg_bp;
	h_bg_bm = the_pizza.h_bg_bm;
	
	if(is_plus)
	{
		h_bsm_dir_tp = the_pizza.h_bsmp_direct_tp;
		h_bsm_dir_tm = the_pizza.h_bsmp_direct_tm;
		h_bsm_dir_bp = the_pizza.h_bsmp_direct_bp;
		h_bsm_dir_bm = the_pizza.h_bsmp_direct_bm;
		
		h_bsm_fss_tp = the_pizza.h_bsmp_fss_tp;
		h_bsm_fss_tm = the_pizza.h_bsmp_fss_tm;
		h_bsm_fss_bp = the_pizza.h_bsmp_fss_bp;
		h_bsm_fss_bm = the_pizza.h_bsmp_fss_bm;
		
		h_bsm_bss_tp = the_pizza.h_bsmp_bss_tp;
		h_bsm_bss_tm = the_pizza.h_bsmp_bss_tm;
		h_bsm_bss_bp = the_pizza.h_bsmp_bss_bp;
		h_bsm_bss_bm = the_pizza.h_bsmp_bss_bm;
		
		h_bsm_bs_tp = the_pizza.h_bsmp_bs_tp;
		h_bsm_bs_tm = the_pizza.h_bsmp_bs_tm;
		h_bsm_bs_bp = the_pizza.h_bsmp_bs_bp;
		h_bsm_bs_bm = the_pizza.h_bsmp_bs_bm;
	}
	else
	{
		h_bsm_dir_tp = the_pizza.h_bsmm_direct_tp;
		h_bsm_dir_tm = the_pizza.h_bsmm_direct_tm;
		h_bsm_dir_bp = the_pizza.h_bsmm_direct_bp;
		h_bsm_dir_bm = the_pizza.h_bsmm_direct_bm;
		
		h_bsm_fss_tp = the_pizza.h_bsmm_fss_tp;
		h_bsm_fss_tm = the_pizza.h_bsmm_fss_tm;
		h_bsm_fss_bp = the_pizza.h_bsmm_fss_bp;
		h_bsm_fss_bm = the_pizza.h_bsmm_fss_bm;
		
		h_bsm_bss_tp = the_pizza.h_bsmm_bss_tp;
		h_bsm_bss_tm = the_pizza.h_bsmm_bss_tm;
		h_bsm_bss_bp = the_pizza.h_bsmm_bss_bp;
		h_bsm_bss_bm = the_pizza.h_bsmm_bss_bm;
		
		h_bsm_bs_tp = the_pizza.h_bsmm_bs_tp;
		h_bsm_bs_tm = the_pizza.h_bsmm_bs_tm;
		h_bsm_bs_bp = the_pizza.h_bsmm_bs_bp;
		h_bsm_bs_bm = the_pizza.h_bsmm_bs_bm;
	}
	
	// Direct:
	h_sm_tp = (TH1D*)h_sm_dir_tp -> Clone("SM E_scint (T+)");
	h_sm_tm = (TH1D*)h_sm_dir_tm -> Clone("SM E_scint (T-)");
	h_sm_bp = (TH1D*)h_sm_dir_bp -> Clone("SM E_scint (B+)");
	h_sm_bm = (TH1D*)h_sm_dir_bm -> Clone("SM E_scint (B-)");
//	
	h_bsm_tp = (TH1D*)h_bsm_dir_tp -> Clone("BSM E_scint (T+)");
	h_bsm_tm = (TH1D*)h_bsm_dir_tm -> Clone("BSM E_scint (T-)");
	h_bsm_bp = (TH1D*)h_bsm_dir_bp -> Clone("BSM E_scint (B+)");
	h_bsm_bm = (TH1D*)h_bsm_dir_bm -> Clone("BSM E_scint (B-)");
	
	
	
	// forward side-scatter
	h_sm_tp ->Add(h_sm_fss_tp, the_toppings.SS_scale);
	h_sm_tm ->Add(h_sm_fss_tm, the_toppings.SS_scale);
	h_sm_bp ->Add(h_sm_fss_bp, the_toppings.SS_scale);
	h_sm_bm ->Add(h_sm_fss_bm, the_toppings.SS_scale);

	h_bsm_tp->Add(h_bsm_fss_tp, the_toppings.SS_scale);
	h_bsm_tm->Add(h_bsm_fss_tm, the_toppings.SS_scale);
	h_bsm_bp->Add(h_bsm_fss_bp, the_toppings.SS_scale);
	h_bsm_bm->Add(h_bsm_fss_bm, the_toppings.SS_scale);
		
	// backward side-scatter
	h_sm_tp ->Add(h_sm_bss_tp, the_toppings.SS_scale);
	h_sm_tm ->Add(h_sm_bss_tm, the_toppings.SS_scale);
	h_sm_bp ->Add(h_sm_bss_bp, the_toppings.SS_scale);
	h_sm_bm ->Add(h_sm_bss_bm, the_toppings.SS_scale);
	
	h_bsm_tp->Add(h_bsm_bss_tp, the_toppings.SS_scale);
	h_bsm_tm->Add(h_bsm_bss_tm, the_toppings.SS_scale);
	h_bsm_bp->Add(h_bsm_bss_bp, the_toppings.SS_scale);
	h_bsm_bm->Add(h_bsm_bss_bm, the_toppings.SS_scale);
	
	// backscatter
	h_sm_tp ->Add(h_sm_bs_tp,  the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bs_tm,  the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bs_bp,  the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bs_bm,  the_toppings.BS_scale);
	h_bsm_tp->Add(h_bsm_bs_tp, the_toppings.BS_scale);
	h_bsm_tm->Add(h_bsm_bs_tm, the_toppings.BS_scale);
	h_bsm_bp->Add(h_bsm_bs_bp, the_toppings.BS_scale);
	h_bsm_bm->Add(h_bsm_bs_bm, the_toppings.BS_scale);
	
	// Background:
	// Debug:  skip the background.
//	cout << "Nbins in h_sm_tp:  " << h_sm_tp->GetNbinsX() << ";\tNbins in h_bg_tp:  " << h_bg_tp->GetNbinsX() << endl;
	h_sm_tp ->Add(h_bg_tp,  the_toppings.BG_scale);
	h_sm_tm ->Add(h_bg_tm,  the_toppings.BG_scale);
	h_sm_bp ->Add(h_bg_bp,  the_toppings.BG_scale);
	h_sm_bm ->Add(h_bg_bm,  the_toppings.BG_scale);
	
	h_bsm_tp->Add(h_bg_tp, the_toppings.BG_scale);
	h_bsm_tm->Add(h_bg_tm, the_toppings.BG_scale);
	h_bsm_bp->Add(h_bg_bp, the_toppings.BG_scale);
	h_bsm_bm->Add(h_bg_bm, the_toppings.BG_scale);
//	cout << "But not here." << endl;
	
	// Assemble:
	h_sm_tp -> ResetStats();
	h_sm_tm -> ResetStats();
	h_sm_bp -> ResetStats();
	h_sm_bm -> ResetStats();
	TH1D * h_asym_sm  = make_superratioasymmetry_direct(h_sm_tp, h_sm_tm, h_sm_bp, h_sm_bm);
//	h_asym_sm->SetBit(TH1::kIsAverage);
	h_asym_sm->Scale(alpha);
//	TH1D* h_asym_sm_tmp = (TH1D*)h_asym_sm->Clone();
	
	h_bsm_tp -> ResetStats();
	h_bsm_tm -> ResetStats();
	h_bsm_bp -> ResetStats();
	h_bsm_bm -> ResetStats();
	TH1D * h_asym_bsm  = make_superratioasymmetry_direct(h_bsm_tp, h_bsm_tm, h_bsm_bp, h_bsm_bm);
	
//	h_asym_bsm->SetBit(TH1::kIsAverage);
	h_asym_bsm->Scale(beta);
	
	TH1D * h_asym = h_asym_sm;// (TH1D*)h_asym_sm->Clone();
	h_asym -> Add(h_asym_bsm);
	h_asym -> SetName(asym_title.c_str());
	h_asym -> SetTitle(asym_title.c_str());
	
	/*
	TCanvas * c0 = new TCanvas();
	c0->Divide(2,2);
	c0->cd(1);
	h_bsm_tp->Draw();
	c0->cd(2);
	h_bsm_tm->Draw();
	c0->cd(3);
	h_bsm_bp->Draw();
	c0->cd(4);
	h_bsm_bm->Draw();
	*/
//	TCanvas * c1 = new TCanvas();
//	h_asym_bsm->Draw();
//	TCanvas * c2 = new TCanvas();
//	h_asym_sm_tmp->Draw();
	
	
	delete h_sm_tp;
	delete h_sm_tm;
	delete h_sm_bp;
	delete h_sm_bm;

	delete h_bsm_tp;
	delete h_bsm_tm;
	delete h_bsm_bp;
	delete h_bsm_bm;
	
//	delete h_asym_sm;
	delete h_asym_bsm;
	
	
	return h_asym;
} // simple_cook(...)


//
TH1D * slow_cook(pizza the_pizza, toppings the_toppings)  // it's still a pizza with the default amount of scattering!  :(
{
	double A_0     = -0.568045;
//	double dA = (the_toppings.Abeta - A_0);
//	double opposite_A = A_0 - dA;
	double opposite_A = A_0 - (the_toppings.Abeta - A_0);
	
	double alpha          = getfast_alpha_from_Abeta_and_bFierz(the_toppings.Abeta, the_toppings.bFierz);
	double beta           = getfast_beta_from_Abeta_and_bFierz(the_toppings.Abeta,  the_toppings.bFierz);
	double opposite_alpha = getfast_alpha_from_Abeta_and_bFierz(opposite_A,         the_toppings.bFierz);
	double opposite_beta  = getfast_beta_from_Abeta_and_bFierz(opposite_A,          the_toppings.bFierz);
	string asym_title     = "Superratio Asymmetry (normal cook):  A="+fake_to_string(the_toppings.Abeta)+", b="+fake_to_string(the_toppings.bFierz)+";  BS="+fake_to_string(the_toppings.BS_scale*100.0)+"\%, SS="+fake_to_string(the_toppings.SS_scale*100.0)+"\%, BG="+fake_to_string(the_toppings.BG_scale*100.0)+"\%";
//	double dA = the_toppings.Abeta - A_0;
	double dA  = A_0*(alpha - 1.0);
	double dbA = A_0*(beta  - 1.0);
	
	// the new asymmetry be like:  alpha*A_SM + beta*A_BSM.
	// for the individual spectra?  .. scale them all like that.  Or, scale at least the direct ones like that?
	// ....Nope.
	
	//	double get_asymmetry(double r1p, double r1m, double r2p, double r2m)
	//	A = ( sqrt(r1p*r2m) - sqrt(r1m*r2p) ) / ( sqrt(r1p*r2m) + sqrt(r1m*r2p) );
	//    ~ (R_tp - R_bp) / (R_tp + R_bp)
	//    ~ ((1+AcosT) - (1-AcosT)) / ((1+AcosT) + (1-AcosT))
	//    = 2AcosT / 2 
	//    = AcosT
	// eg, R_tp = R*(1+AcosT)
	//     R_tp-->R*(1+(A+dA)cosT)
	// for A-->A+dA, 
	// A+dA ~ ((1+(A+dA)cosT) - (1-(A+dA)cosT)) / ((1+(A+dA)cosT) + (1-(A+dA)cosT))
	//      = (2*(A+dA)) / (2)
	// A_spect = alpha*A_SM_spect + beta*A_BSM_spect
	//
	// Suppose:
	// A_spect = alpha*A_SM_spect
	// A_val   = A_val_orig + dA_val
	// Take:  R_tp --> R_tp*(1+dA/A)  ?????
	// Do this.  R_tp --> R_tp*(1+(A+dA)cosT)/(1+AcosT) 
	//                 =  R_tp*( 1 + dA/(1+AcosT) )
	//           R_tp --> R_tp + dA  (srsly?)
	//           R_tp->Scale(1.0+dA/(1+A_0))
	//           +/- dA is what happens when you want to scale the SM asymmetry by alpha.
	// 		really, I did +/- dA wrong..  I should use alpha to make it happen, not the raw difference in Abeta.
	// 		A_0_SM*alpha = A_0_SM + dA
	// 		dA = A_0_SM*alpha - A_0_SM
	// 		dA = A_0_SM*(alpha - 1.0);
	// Try this too, for the BSM ones?:
	//           R_tp --> R_tp + dbA
	//           R_tp->Scale(1.0+dbA/(1+A_0))
	// but what's dbA?  Presumably it's the thing that happens when you want to scale the BSM asymmetry by beta.
	// 
	
	
	bool is_plus = true;
	
	// --- *** --- //
	// Load histograms:
	
	// Direct:
	TH1D *h_sm_tp, *h_bsm_tp;
	TH1D *h_sm_tm, *h_bsm_tm;
	TH1D *h_sm_bp, *h_bsm_bp;
	TH1D *h_sm_bm, *h_bsm_bm;
	// forward side-scatter
	TH1D *h_sm_fss_tp, *h_bsm_fss_tp;
	TH1D *h_sm_fss_tm, *h_bsm_fss_tm;
	TH1D *h_sm_fss_bp, *h_bsm_fss_bp;
	TH1D *h_sm_fss_bm, *h_bsm_fss_bm;
	// backward side-scatter
	TH1D *h_sm_bss_tp, *h_bsm_bss_tp;
	TH1D *h_sm_bss_tm, *h_bsm_bss_tm;
	TH1D *h_sm_bss_bp, *h_bsm_bss_bp;
	TH1D *h_sm_bss_bm, *h_bsm_bss_bm;
	// backscatter
	TH1D *h_sm_bs_tp, *h_bsm_bs_tp;
	TH1D *h_sm_bs_tm, *h_bsm_bs_tm;
	TH1D *h_sm_bs_bp, *h_bsm_bs_bp;
	TH1D *h_sm_bs_bm, *h_bsm_bs_bm;
	// Background:
	TH1D * h_bg_tp = (TH1D*)the_pizza.h_bg_tp -> Clone();
	TH1D * h_bg_tm = (TH1D*)the_pizza.h_bg_tm -> Clone();
	TH1D * h_bg_bp = (TH1D*)the_pizza.h_bg_bp -> Clone();
	TH1D * h_bg_bm = (TH1D*)the_pizza.h_bg_bm -> Clone();
	
	
	// --- *** --- //
	// Direct:
	h_sm_tp = (TH1D*)the_pizza.h_sm_direct_tp -> Clone("SM E_scint (T+)");
	h_sm_tm = (TH1D*)the_pizza.h_sm_direct_tm -> Clone("SM E_scint (T-)");
	h_sm_bp = (TH1D*)the_pizza.h_sm_direct_bp -> Clone("SM E_scint (B+)");
	h_sm_bm = (TH1D*)the_pizza.h_sm_direct_bm -> Clone("SM E_scint (B-)");
	// forward side-scatter
	h_sm_fss_tp = (TH1D*)the_pizza.h_sm_fss_tp -> Clone();
	h_sm_fss_tm = (TH1D*)the_pizza.h_sm_fss_tm -> Clone();
	h_sm_fss_bp = (TH1D*)the_pizza.h_sm_fss_bp -> Clone();
	h_sm_fss_bm = (TH1D*)the_pizza.h_sm_fss_bm -> Clone();
	// backward side-scatter
	h_sm_bss_tp = (TH1D*)the_pizza.h_sm_bss_tp -> Clone();
	h_sm_bss_tm = (TH1D*)the_pizza.h_sm_bss_tm -> Clone();
	h_sm_bss_bp = (TH1D*)the_pizza.h_sm_bss_bp -> Clone();
	h_sm_bss_bm = (TH1D*)the_pizza.h_sm_bss_bm -> Clone();
	// backscatter
	h_sm_bs_tp = (TH1D*)the_pizza.h_sm_bs_tp -> Clone();
	h_sm_bs_tm = (TH1D*)the_pizza.h_sm_bs_tm -> Clone();
	h_sm_bs_bp = (TH1D*)the_pizza.h_sm_bs_bp -> Clone();
	h_sm_bs_bm = (TH1D*)the_pizza.h_sm_bs_bm -> Clone();

	if(is_plus)
	{
		h_bsm_tp = (TH1D*)the_pizza.h_bsmp_direct_tp -> Clone("BSM E_scint (T+)");
		h_bsm_tm = (TH1D*)the_pizza.h_bsmp_direct_tm -> Clone("BSM E_scint (T-)");
		h_bsm_bp = (TH1D*)the_pizza.h_bsmp_direct_bp -> Clone("BSM E_scint (B+)");
		h_bsm_bm = (TH1D*)the_pizza.h_bsmp_direct_bm -> Clone("BSM E_scint (B-)");
		
		h_bsm_fss_tp = (TH1D*)the_pizza.h_bsmp_fss_tp -> Clone();
		h_bsm_fss_tm = (TH1D*)the_pizza.h_bsmp_fss_tm -> Clone();
		h_bsm_fss_bp = (TH1D*)the_pizza.h_bsmp_fss_bp -> Clone();
		h_bsm_fss_bm = (TH1D*)the_pizza.h_bsmp_fss_bm -> Clone();
		
		h_bsm_bss_tp = (TH1D*)the_pizza.h_bsmp_bss_tp -> Clone();
		h_bsm_bss_tm = (TH1D*)the_pizza.h_bsmp_bss_tm -> Clone();
		h_bsm_bss_bp = (TH1D*)the_pizza.h_bsmp_bss_bp -> Clone();
		h_bsm_bss_bm = (TH1D*)the_pizza.h_bsmp_bss_bm -> Clone();
		
		h_bsm_bs_tp = (TH1D*)the_pizza.h_bsmp_bs_tp -> Clone();
		h_bsm_bs_tm = (TH1D*)the_pizza.h_bsmp_bs_tm -> Clone();
		h_bsm_bs_bp = (TH1D*)the_pizza.h_bsmp_bs_bp -> Clone();
		h_bsm_bs_bm = (TH1D*)the_pizza.h_bsmp_bs_bm -> Clone();
	}
	else
	{
		h_bsm_tp = (TH1D*)the_pizza.h_bsmm_direct_tp -> Clone("BSM E_scint (T+)");
		h_bsm_tm = (TH1D*)the_pizza.h_bsmm_direct_tm -> Clone("BSM E_scint (T-)");
		h_bsm_bp = (TH1D*)the_pizza.h_bsmm_direct_bp -> Clone("BSM E_scint (B+)");
		h_bsm_bm = (TH1D*)the_pizza.h_bsmm_direct_bm -> Clone("BSM E_scint (B-)");
		
		h_bsm_fss_tp = (TH1D*)the_pizza.h_bsmm_fss_tp -> Clone();
		h_bsm_fss_tm = (TH1D*)the_pizza.h_bsmm_fss_tm -> Clone();
		h_bsm_fss_bp = (TH1D*)the_pizza.h_bsmm_fss_bp -> Clone();
		h_bsm_fss_bm = (TH1D*)the_pizza.h_bsmm_fss_bm -> Clone();
		
		h_bsm_bss_tp = (TH1D*)the_pizza.h_bsmm_bss_tp -> Clone();
		h_bsm_bss_tm = (TH1D*)the_pizza.h_bsmm_bss_tm -> Clone();
		h_bsm_bss_bp = (TH1D*)the_pizza.h_bsmm_bss_bp -> Clone();
		h_bsm_bss_bm = (TH1D*)the_pizza.h_bsmm_bss_bm -> Clone();
		
		h_bsm_bs_tp = (TH1D*)the_pizza.h_bsmm_bs_tp -> Clone();
		h_bsm_bs_tm = (TH1D*)the_pizza.h_bsmm_bs_tm -> Clone();
		h_bsm_bs_bp = (TH1D*)the_pizza.h_bsmm_bs_bp -> Clone();
		h_bsm_bs_bm = (TH1D*)the_pizza.h_bsmm_bs_bm -> Clone();
	}
	
	// Direct:
	/*
	h_sm_tp -> Scale(alpha);
	h_sm_tm -> Scale(alpha);
	h_sm_bp -> Scale(alpha);
	h_sm_bm -> Scale(alpha);

	h_bsm_tp -> Scale(beta);
	h_bsm_tm -> Scale(beta);
	h_bsm_bp -> Scale(beta);
	h_bsm_bm -> Scale(beta);
	*/
	//           R_tp->Scale(1.0+dA/(1+A_0))
	h_sm_tp -> Scale(1.0+dA/(1.0+A_0));
	h_sm_tm -> Scale(1.0-dA/(1.0+A_0));
	h_sm_bp -> Scale(1.0+dA/(1.0+A_0));
	h_sm_bm -> Scale(1.0-dA/(1.0+A_0));

	h_bsm_tp -> Scale(1.0+dbA/(1.0+A_0));
	h_bsm_tm -> Scale(1.0-dbA/(1.0+A_0));
	h_bsm_bp -> Scale(1.0+dbA/(1.0+A_0));
	h_bsm_bm -> Scale(1.0-dbA/(1.0+A_0));
	
	// forward side-scatter
	/*
	h_sm_tp ->Add(h_sm_fss_tp, alpha);
	h_sm_tm ->Add(h_sm_fss_tm, alpha);
	h_sm_bp ->Add(h_sm_fss_bp, alpha);
	h_sm_bm ->Add(h_sm_fss_bm, alpha);

	h_bsm_tp->Add(h_bsm_fss_tp, beta);
	h_bsm_tm->Add(h_bsm_fss_tm, beta);
	h_bsm_bp->Add(h_bsm_fss_bp, beta);
	h_bsm_bm->Add(h_bsm_fss_bm, beta);
	*/
	h_sm_tp ->Add(h_sm_fss_tp,  (1.0+dA/(1.0+A_0))*the_toppings.SS_scale );
	h_sm_tm ->Add(h_sm_fss_tm,  (1.0-dA/(1.0+A_0))*the_toppings.SS_scale );
	h_sm_bp ->Add(h_sm_fss_bp,  (1.0+dA/(1.0+A_0))*the_toppings.SS_scale );
	h_sm_bm ->Add(h_sm_fss_bm,  (1.0-dA/(1.0+A_0))*the_toppings.SS_scale );
	
	h_bsm_tp->Add(h_bsm_fss_tp, (1.0+dbA/(1.0+A_0))*the_toppings.SS_scale );
	h_bsm_tm->Add(h_bsm_fss_tm, (1.0-dbA/(1.0+A_0))*the_toppings.SS_scale );
	h_bsm_bp->Add(h_bsm_fss_bp, (1.0+dbA/(1.0+A_0))*the_toppings.SS_scale );
	h_bsm_bm->Add(h_bsm_fss_bm, (1.0-dbA/(1.0+A_0))*the_toppings.SS_scale );
	
	// backward side-scatter
	/*
	h_sm_tp ->Add(h_sm_bss_tp, opposite_alpha*the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bss_tm, opposite_alpha*the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bss_bp, opposite_alpha*the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bss_bm, opposite_alpha*the_toppings.BS_scale);

	h_bsm_tp->Add(h_bsm_bss_tp, opposite_beta*the_toppings.BS_scale);
	h_bsm_tm->Add(h_bsm_bss_tm, opposite_beta*the_toppings.BS_scale);
	h_bsm_bp->Add(h_bsm_bss_bp, opposite_beta*the_toppings.BS_scale);
	h_bsm_bm->Add(h_bsm_bss_bm, opposite_beta*the_toppings.BS_scale);
	*/
	h_sm_tp ->Add(h_sm_bss_tp, (1.0-dA/(1.0+A_0))*the_toppings.SS_scale);
	h_sm_tm ->Add(h_sm_bss_tm, (1.0+dA/(1.0+A_0))*the_toppings.SS_scale);
	h_sm_bp ->Add(h_sm_bss_bp, (1.0-dA/(1.0+A_0))*the_toppings.SS_scale);
	h_sm_bm ->Add(h_sm_bss_bm, (1.0+dA/(1.0+A_0))*the_toppings.SS_scale);
	
	h_bsm_tp->Add(h_bsm_bss_tp, (1.0-dbA/(1.0+A_0))*the_toppings.SS_scale);
	h_bsm_tm->Add(h_bsm_bss_tm, (1.0+dbA/(1.0+A_0))*the_toppings.SS_scale);
	h_bsm_bp->Add(h_bsm_bss_bp, (1.0-dbA/(1.0+A_0))*the_toppings.SS_scale);
	h_bsm_bm->Add(h_bsm_bss_bm, (1.0+dbA/(1.0+A_0))*the_toppings.SS_scale);
	
	// backscatter
	/*
	h_sm_tp ->Add(h_sm_bs_tp,  opposite_alpha*the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bs_tm,  opposite_alpha*the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bs_bp,  opposite_alpha*the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bs_bm,  opposite_alpha*the_toppings.BS_scale);

	h_bsm_tp->Add(h_bsm_bs_tp, opposite_beta*the_toppings.BS_scale);
	h_bsm_tm->Add(h_bsm_bs_tm, opposite_beta*the_toppings.BS_scale);
	h_bsm_bp->Add(h_bsm_bs_bp, opposite_beta*the_toppings.BS_scale);
	h_bsm_bm->Add(h_bsm_bs_bm, opposite_beta*the_toppings.BS_scale);
	*/
	h_sm_tp ->Add(h_sm_bs_tp,  (1.0-dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bs_tm,  (1.0+dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bs_bp,  (1.0-dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bs_bm,  (1.0+dA/(1.0+A_0))*the_toppings.BS_scale);
	
	h_bsm_tp->Add(h_bsm_bs_tp, (1.0-dbA/(1.0+A_0))*the_toppings.BS_scale);
	h_bsm_tm->Add(h_bsm_bs_tm, (1.0+dbA/(1.0+A_0))*the_toppings.BS_scale);
	h_bsm_bp->Add(h_bsm_bs_bp, (1.0-dbA/(1.0+A_0))*the_toppings.BS_scale);
	h_bsm_bm->Add(h_bsm_bs_bm, (1.0+dbA/(1.0+A_0))*the_toppings.BS_scale);
	
	
	// Background:
	/*
	h_bg_tp -> Scale( the_toppings.BG_scale*(alpha+beta) );
	h_bg_tm -> Scale( the_toppings.BG_scale*(alpha+beta) );
	h_bg_bp -> Scale( the_toppings.BG_scale*(alpha+beta) );
	h_bg_bm -> Scale( the_toppings.BG_scale*(alpha+beta) );
	*/
	h_bg_tp -> Scale( the_toppings.BG_scale*( (1.0+dA/(1.0+A_0)) + (1.0+dbA/(1.0+A_0)) ) );
	h_bg_tm -> Scale( the_toppings.BG_scale*( (1.0-dA/(1.0+A_0)) + (1.0-dbA/(1.0+A_0)) ) );
	h_bg_bp -> Scale( the_toppings.BG_scale*( (1.0+dA/(1.0+A_0)) + (1.0+dbA/(1.0+A_0)) ) );
	h_bg_bm -> Scale( the_toppings.BG_scale*( (1.0-dA/(1.0+A_0)) + (1.0-dbA/(1.0+A_0)) ) );
	
	h_sm_tp ->Add(h_bg_tp);
	h_sm_tm ->Add(h_bg_tm);
	h_sm_bp ->Add(h_bg_bp);
	h_sm_bm ->Add(h_bg_bm);
	
	h_bsm_tp->Add(h_bg_tp);
	h_bsm_tm->Add(h_bg_tm);
	h_bsm_bp->Add(h_bg_bp);
	h_bsm_bm->Add(h_bg_bm);
	
	// so that it has the right weighting.
	h_sm_tp ->Scale(alpha);
	h_sm_tm ->Scale(alpha);
	h_sm_bp ->Scale(alpha);
	h_sm_bm ->Scale(alpha);
	h_bsm_tp->Scale(beta);
	h_bsm_tm->Scale(beta);
	h_bsm_bp->Scale(beta);
	h_bsm_bm->Scale(beta);
	h_sm_tp ->ResetStats();
	h_sm_tm ->ResetStats();
	h_sm_bp ->ResetStats();
	h_sm_bm ->ResetStats();
	h_bsm_tp->ResetStats();
	h_bsm_tm->ResetStats();
	h_bsm_bp->ResetStats();
	h_bsm_bm->ResetStats();
	
	// Assemble:
	TH1D * h_tp = (TH1D*)h_sm_tp -> Clone("Full E_scint (T+)");
	TH1D * h_tm = (TH1D*)h_sm_tm -> Clone("Full E_scint (T-)");
	TH1D * h_bp = (TH1D*)h_sm_bp -> Clone("Full E_scint (B+)");
	TH1D * h_bm = (TH1D*)h_sm_bm -> Clone("Full E_scint (B-)");
	h_tp -> SetTitle(h_tp->GetName());
	h_tm -> SetTitle(h_tm->GetName());
	h_bp -> SetTitle(h_bp->GetName());
	h_bm -> SetTitle(h_bm->GetName());
	
	h_tp->Add(h_bsm_tp);
	h_tm->Add(h_bsm_tm);
	h_bp->Add(h_bsm_bp);
	h_bm->Add(h_bsm_bm);

//	h_tp->Add(h_bg_tp);
//	h_tm->Add(h_bg_tm);
//	h_bp->Add(h_bg_bp);
//	h_bm->Add(h_bg_bm);
	
//	double get_asymmetry(double r1p, double r1m, double r2p, double r2m)
//	A = ( sqrt(r1p*r2m) - sqrt(r1m*r2p) ) / ( sqrt(r1p*r2m) + sqrt(r1m*r2p) );
	TH1D * h_asym  = make_superratioasymmetry_direct(h_tp, h_tm, h_bp, h_bm);
	h_asym -> SetName(asym_title.c_str());
	h_asym -> SetTitle(asym_title.c_str());
	
	return h_asym;
}
TH1D * really_old_cook(pizza the_pizza, toppings the_toppings)  // it's still a pizza with the default amount of scattering!  :(
{
	double A_0     = -0.568045;
//	double dA = (the_toppings.Abeta - A_0);
//	double opposite_A = A_0 - dA;
	double opposite_A = A_0 - (the_toppings.Abeta - A_0);
	
	double alpha          = getfast_alpha_from_Abeta_and_bFierz(the_toppings.Abeta, the_toppings.bFierz);
	double beta           = getfast_beta_from_Abeta_and_bFierz(the_toppings.Abeta,  the_toppings.bFierz);
	double opposite_alpha = getfast_alpha_from_Abeta_and_bFierz(opposite_A,         the_toppings.bFierz);
	double opposite_beta  = getfast_beta_from_Abeta_and_bFierz(opposite_A,          the_toppings.bFierz);
	string asym_title     = "Superratio Asymmetry:  A="+fake_to_string(the_toppings.Abeta)+", b="+fake_to_string(the_toppings.bFierz)+";  BS="+fake_to_string(the_toppings.BS_scale*100.0)+"\%, SS="+fake_to_string(the_toppings.SS_scale*100.0)+"\%";
	double dA = the_toppings.Abeta - A_0;
	
	// the new asymmetry be like:  alpha*A_SM + beta*A_BSM.
	// for the individual spectra?  .. scale them all like that.  Or, scale at least the direct ones like that?
	// ....Nope.
	
	//	double get_asymmetry(double r1p, double r1m, double r2p, double r2m)
	//	A = ( sqrt(r1p*r2m) - sqrt(r1m*r2p) ) / ( sqrt(r1p*r2m) + sqrt(r1m*r2p) );
	//    ~ (R_tp - R_bp) / (R_tp + R_bp)
	//    ~ ((1+AcosT) - (1-AcosT)) / ((1+AcosT) + (1-AcosT))
	//    = 2AcosT / 2 
	//    = AcosT
	// eg, R_tp = R*(1+AcosT)
	//     R_tp-->R*(1+(A+dA)cosT)
	// for A-->A+dA, 
	// A+dA ~ ((1+(A+dA)cosT) - (1-(A+dA)cosT)) / ((1+(A+dA)cosT) + (1-(A+dA)cosT))
	//      = (2*(A+dA)) / (2)
	// A_spect = alpha*A_SM_spect + beta*A_BSM_spect
	//
	// Suppose:
	// A_spect = alpha*A_SM_spect
	// A_val   = A_val_orig + dA_val
	// Take:  R_tp --> R_tp*(1+dA/A)  ?????
	// Do this.  R_tp --> R_tp*(1+(A+dA)cosT)/(1+AcosT) 
	//                 =  R_tp*( 1 + dA/(1+AcosT) )
	//           R_tp --> R_tp + dA  (srsly?)
	//           R_tp->Scale(1.0+dA/(1+A_0))
	
	bool is_plus = true;
	
	// --- *** --- //
	// Load histograms:
	
	// Direct:
	TH1D *h_sm_tp, *h_bsm_tp;
	TH1D *h_sm_tm, *h_bsm_tm;
	TH1D *h_sm_bp, *h_bsm_bp;
	TH1D *h_sm_bm, *h_bsm_bm;
	// forward side-scatter
	TH1D *h_sm_fss_tp, *h_bsm_fss_tp;
	TH1D *h_sm_fss_tm, *h_bsm_fss_tm;
	TH1D *h_sm_fss_bp, *h_bsm_fss_bp;
	TH1D *h_sm_fss_bm, *h_bsm_fss_bm;
	// backward side-scatter
	TH1D *h_sm_bss_tp, *h_bsm_bss_tp;
	TH1D *h_sm_bss_tm, *h_bsm_bss_tm;
	TH1D *h_sm_bss_bp, *h_bsm_bss_bp;
	TH1D *h_sm_bss_bm, *h_bsm_bss_bm;
	// backscatter
	TH1D *h_sm_bs_tp, *h_bsm_bs_tp;
	TH1D *h_sm_bs_tm, *h_bsm_bs_tm;
	TH1D *h_sm_bs_bp, *h_bsm_bs_bp;
	TH1D *h_sm_bs_bm, *h_bsm_bs_bm;
	// Background:
	TH1D * h_bg_tp = (TH1D*)the_pizza.h_bg_tp -> Clone();
	TH1D * h_bg_tm = (TH1D*)the_pizza.h_bg_tm -> Clone();
	TH1D * h_bg_bp = (TH1D*)the_pizza.h_bg_bp -> Clone();
	TH1D * h_bg_bm = (TH1D*)the_pizza.h_bg_bm -> Clone();
	
	
	// --- *** --- //
	// Direct:
	h_sm_tp = (TH1D*)the_pizza.h_sm_direct_tp -> Clone("SM E_scint (T+)");
	h_sm_tm = (TH1D*)the_pizza.h_sm_direct_tm -> Clone("SM E_scint (T-)");
	h_sm_bp = (TH1D*)the_pizza.h_sm_direct_bp -> Clone("SM E_scint (B+)");
	h_sm_bm = (TH1D*)the_pizza.h_sm_direct_bm -> Clone("SM E_scint (B-)");
	// forward side-scatter
	h_sm_fss_tp = (TH1D*)the_pizza.h_sm_fss_tp -> Clone();
	h_sm_fss_tm = (TH1D*)the_pizza.h_sm_fss_tm -> Clone();
	h_sm_fss_bp = (TH1D*)the_pizza.h_sm_fss_bp -> Clone();
	h_sm_fss_bm = (TH1D*)the_pizza.h_sm_fss_bm -> Clone();
	// backward side-scatter
	h_sm_bss_tp = (TH1D*)the_pizza.h_sm_bss_tp -> Clone();
	h_sm_bss_tm = (TH1D*)the_pizza.h_sm_bss_tm -> Clone();
	h_sm_bss_bp = (TH1D*)the_pizza.h_sm_bss_bp -> Clone();
	h_sm_bss_bm = (TH1D*)the_pizza.h_sm_bss_bm -> Clone();
	// backscatter
	h_sm_bs_tp = (TH1D*)the_pizza.h_sm_bs_tp -> Clone();
	h_sm_bs_tm = (TH1D*)the_pizza.h_sm_bs_tm -> Clone();
	h_sm_bs_bp = (TH1D*)the_pizza.h_sm_bs_bp -> Clone();
	h_sm_bs_bm = (TH1D*)the_pizza.h_sm_bs_bm -> Clone();

	if(is_plus)
	{
		h_bsm_tp = (TH1D*)the_pizza.h_bsmp_direct_tp -> Clone("BSM E_scint (T+)");
		h_bsm_tm = (TH1D*)the_pizza.h_bsmp_direct_tm -> Clone("BSM E_scint (T-)");
		h_bsm_bp = (TH1D*)the_pizza.h_bsmp_direct_bp -> Clone("BSM E_scint (B+)");
		h_bsm_bm = (TH1D*)the_pizza.h_bsmp_direct_bm -> Clone("BSM E_scint (B-)");
		
		h_bsm_fss_tp = (TH1D*)the_pizza.h_bsmp_fss_tp -> Clone();
		h_bsm_fss_tm = (TH1D*)the_pizza.h_bsmp_fss_tm -> Clone();
		h_bsm_fss_bp = (TH1D*)the_pizza.h_bsmp_fss_bp -> Clone();
		h_bsm_fss_bm = (TH1D*)the_pizza.h_bsmp_fss_bm -> Clone();
		
		h_bsm_bss_tp = (TH1D*)the_pizza.h_bsmp_bss_tp -> Clone();
		h_bsm_bss_tm = (TH1D*)the_pizza.h_bsmp_bss_tm -> Clone();
		h_bsm_bss_bp = (TH1D*)the_pizza.h_bsmp_bss_bp -> Clone();
		h_bsm_bss_bm = (TH1D*)the_pizza.h_bsmp_bss_bm -> Clone();
		
		h_bsm_bs_tp = (TH1D*)the_pizza.h_bsmp_bs_tp -> Clone();
		h_bsm_bs_tm = (TH1D*)the_pizza.h_bsmp_bs_tm -> Clone();
		h_bsm_bs_bp = (TH1D*)the_pizza.h_bsmp_bs_bp -> Clone();
		h_bsm_bs_bm = (TH1D*)the_pizza.h_bsmp_bs_bm -> Clone();
	}
	else
	{
		h_bsm_tp = (TH1D*)the_pizza.h_bsmm_direct_tp -> Clone("BSM E_scint (T+)");
		h_bsm_tm = (TH1D*)the_pizza.h_bsmm_direct_tm -> Clone("BSM E_scint (T-)");
		h_bsm_bp = (TH1D*)the_pizza.h_bsmm_direct_bp -> Clone("BSM E_scint (B+)");
		h_bsm_bm = (TH1D*)the_pizza.h_bsmm_direct_bm -> Clone("BSM E_scint (B-)");
		
		h_bsm_fss_tp = (TH1D*)the_pizza.h_bsmm_fss_tp -> Clone();
		h_bsm_fss_tm = (TH1D*)the_pizza.h_bsmm_fss_tm -> Clone();
		h_bsm_fss_bp = (TH1D*)the_pizza.h_bsmm_fss_bp -> Clone();
		h_bsm_fss_bm = (TH1D*)the_pizza.h_bsmm_fss_bm -> Clone();
		
		h_bsm_bss_tp = (TH1D*)the_pizza.h_bsmm_bss_tp -> Clone();
		h_bsm_bss_tm = (TH1D*)the_pizza.h_bsmm_bss_tm -> Clone();
		h_bsm_bss_bp = (TH1D*)the_pizza.h_bsmm_bss_bp -> Clone();
		h_bsm_bss_bm = (TH1D*)the_pizza.h_bsmm_bss_bm -> Clone();
		
		h_bsm_bs_tp = (TH1D*)the_pizza.h_bsmm_bs_tp -> Clone();
		h_bsm_bs_tm = (TH1D*)the_pizza.h_bsmm_bs_tm -> Clone();
		h_bsm_bs_bp = (TH1D*)the_pizza.h_bsmm_bs_bp -> Clone();
		h_bsm_bs_bm = (TH1D*)the_pizza.h_bsmm_bs_bm -> Clone();
	}
	
	// Direct:
	/*
	h_sm_tp -> Scale(alpha);
	h_sm_tm -> Scale(alpha);
	h_sm_bp -> Scale(alpha);
	h_sm_bm -> Scale(alpha);
	*/
	//           R_tp->Scale(1.0+dA/(1+A_0))
	h_sm_tp -> Scale(1.0+dA/(1.0+A_0));
	h_sm_tm -> Scale(1.0-dA/(1.0+A_0));
	h_sm_bp -> Scale(1.0+dA/(1.0+A_0));
	h_sm_bm -> Scale(1.0-dA/(1.0+A_0));

	h_bsm_tp -> Scale(beta);
	h_bsm_tm -> Scale(beta);
	h_bsm_bp -> Scale(beta);
	h_bsm_bm -> Scale(beta);
	
	// forward side-scatter
	/*
	h_sm_tp ->Add(h_sm_fss_tp, alpha);
	h_sm_tm ->Add(h_sm_fss_tm, alpha);
	h_sm_bp ->Add(h_sm_fss_bp, alpha);
	h_sm_bm ->Add(h_sm_fss_bm, alpha);
	*/
	h_sm_tp ->Add(h_sm_fss_tp, 1.0+dA/(1.0+A_0));
	h_sm_tm ->Add(h_sm_fss_tm, 1.0-dA/(1.0+A_0));
	h_sm_bp ->Add(h_sm_fss_bp, 1.0+dA/(1.0+A_0));
	h_sm_bm ->Add(h_sm_fss_bm, 1.0-dA/(1.0+A_0));
	
	h_bsm_tp->Add(h_bsm_fss_tp, beta);
	h_bsm_tm->Add(h_bsm_fss_tm, beta);
	h_bsm_bp->Add(h_bsm_fss_bp, beta);
	h_bsm_bm->Add(h_bsm_fss_bm, beta);
	
	// backward side-scatter
	/*
	h_sm_tp ->Add(h_sm_bss_tp, opposite_alpha*the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bss_tm, opposite_alpha*the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bss_bp, opposite_alpha*the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bss_bm, opposite_alpha*the_toppings.BS_scale);
	*/
	h_sm_tp ->Add(h_sm_bss_tp, (1.0-dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bss_tm, (1.0+dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bss_bp, (1.0-dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bss_bm, (1.0+dA/(1.0+A_0))*the_toppings.BS_scale);
	
	h_bsm_tp->Add(h_bsm_bss_tp, opposite_beta*the_toppings.BS_scale);
	h_bsm_tm->Add(h_bsm_bss_tm, opposite_beta*the_toppings.BS_scale);
	h_bsm_bp->Add(h_bsm_bss_bp, opposite_beta*the_toppings.BS_scale);
	h_bsm_bm->Add(h_bsm_bss_bm, opposite_beta*the_toppings.BS_scale);
	
	// backscatter
	/*
	h_sm_tp ->Add(h_sm_bs_tp,  opposite_alpha*the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bs_tm,  opposite_alpha*the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bs_bp,  opposite_alpha*the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bs_bm,  opposite_alpha*the_toppings.BS_scale);
	*/
	h_sm_tp ->Add(h_sm_bs_tp,  (1.0-dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_tm ->Add(h_sm_bs_tm,  (1.0+dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_bp ->Add(h_sm_bs_bp,  (1.0-dA/(1.0+A_0))*the_toppings.BS_scale);
	h_sm_bm ->Add(h_sm_bs_bm,  (1.0+dA/(1.0+A_0))*the_toppings.BS_scale);
	
	h_bsm_tp->Add(h_bsm_bs_tp, opposite_beta*the_toppings.BS_scale);
	h_bsm_tm->Add(h_bsm_bs_tm, opposite_beta*the_toppings.BS_scale);
	h_bsm_bp->Add(h_bsm_bs_bp, opposite_beta*the_toppings.BS_scale);
	h_bsm_bm->Add(h_bsm_bs_bm, opposite_beta*the_toppings.BS_scale);
	
	
	// Background:
	h_bg_tp -> Scale( the_toppings.BG_scale*(alpha+beta) );
	h_bg_tm -> Scale( the_toppings.BG_scale*(alpha+beta) );
	h_bg_bp -> Scale( the_toppings.BG_scale*(alpha+beta) );
	h_bg_bm -> Scale( the_toppings.BG_scale*(alpha+beta) );
	
	
	// Assemble:
	TH1D * h_tp = (TH1D*)h_sm_tp -> Clone("Full E_scint (T+)");
	TH1D * h_tm = (TH1D*)h_sm_tm -> Clone("Full E_scint (T-)");
	TH1D * h_bp = (TH1D*)h_sm_bp -> Clone("Full E_scint (B+)");
	TH1D * h_bm = (TH1D*)h_sm_bm -> Clone("Full E_scint (B-)");
	h_tp -> SetTitle(h_tp->GetName());
	h_tm -> SetTitle(h_tm->GetName());
	h_bp -> SetTitle(h_bp->GetName());
	h_bm -> SetTitle(h_bm->GetName());
	
	h_tp->Add(h_bsm_tp);
	h_tm->Add(h_bsm_tm);
	h_bp->Add(h_bsm_bp);
	h_bm->Add(h_bsm_bm);

	h_tp->Add(h_bg_tp);
	h_tm->Add(h_bg_tm);
	h_bp->Add(h_bg_bp);
	h_bm->Add(h_bg_bm);
	
//	double get_asymmetry(double r1p, double r1m, double r2p, double r2m)
//	A = ( sqrt(r1p*r2m) - sqrt(r1m*r2p) ) / ( sqrt(r1p*r2m) + sqrt(r1m*r2p) );
	TH1D * h_asym  = make_superratioasymmetry_direct(h_tp, h_tm, h_bp, h_bm);
	h_asym -> SetName(asym_title.c_str());
	h_asym -> SetTitle(asym_title.c_str());
	
	return h_asym;
}
//

/*
pizza make_pizza_old(string sm_minifname, string bsmp_minifname, string bsmm_minifname, string bg_minifname)
{
//	int N_rebin = 10;  // rebin everything.  everything!
	cout << "Trying to make the (old) pizza." << endl;
	
	string output_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";
	
	string fname_sm   = output_path+sm_minifname;
	string fname_bsmp = output_path+bsmp_minifname;
	string fname_bsmm = output_path+bsmm_minifname;
	string fname_bg   = output_path+bg_minifname;
	
	TFile * f_sm   = new TFile( (fname_sm).c_str() );
	TFile * f_bsmp = new TFile( (fname_bsmp).c_str() );
	TFile * f_bsmm = new TFile( (fname_bsmm).c_str() );
	TFile * f_bg   = new TFile( (fname_bg).c_str() );
	
	cout << "f_sm:   " << fname_sm << " (" << f_sm << ")" << endl;
	cout << "f_bsmp: " << fname_bsmp << " (" << f_bsmp << ")" << endl;
	cout << "f_bsmm: " << fname_bsmm << " (" << f_bsmm << ")" << endl;
	cout << "f_bg:   " << fname_bg << " (" << f_bg << ")" << endl;
	
	pizza the_pizza;
	
	string matched_set="B";
	int threshold_index=0;
	double sigma_cut=3.0;
	int bb1_threshold=60.0;
	double bb1_maxr=15.5;

	the_pizza.matched_set     = matched_set;
	the_pizza.threshold_index = threshold_index;
	the_pizza.sigma_cut       = sigma_cut;
	the_pizza.bb1_threshold   = bb1_threshold;
	the_pizza.bb1_maxr        = bb1_maxr;
	string bb_str = int_to_string(bb1_threshold);
//	string additional_filename_info = make_additional_filename_info(threshold_index, sigma_cut, bb1_maxr);
//	additional_filename_info=additional_filename_info+"_old";
	cout << "Old pizza parameters are set." << endl;
	
	//
	the_pizza.h_sm_direct_tp = (TH1D*)f_sm->Get( string("E_scint DirectHit Lev+0eV TOFcut (T+)").c_str() );
	the_pizza.h_sm_direct_tm = (TH1D*)f_sm->Get( string("E_scint DirectHit Lev+0eV TOFcut (T-)").c_str() );
	the_pizza.h_sm_direct_bp = (TH1D*)f_sm->Get( string("E_scint DirectHit Lev+0eV TOFcut (B+)").c_str() );
	the_pizza.h_sm_direct_bm = (TH1D*)f_sm->Get( string("E_scint DirectHit Lev+0eV TOFcut (B-)").c_str() );
	the_pizza.h_sm_fss_tp = (TH1D*)f_sm->Get( string("E_scint ForwardSideScatter Lev+0eV TOFcut (T+)").c_str() );
	the_pizza.h_sm_fss_tm = (TH1D*)f_sm->Get( string("E_scint ForwardSideScatter Lev+0eV TOFcut (T-)").c_str() );
	the_pizza.h_sm_fss_bp = (TH1D*)f_sm->Get( string("E_scint ForwardSideScatter Lev+0eV TOFcut (B+)").c_str() );
	the_pizza.h_sm_fss_bm = (TH1D*)f_sm->Get( string("E_scint ForwardSideScatter Lev+0eV TOFcut (B-)").c_str() );
	the_pizza.h_sm_bss_tp = (TH1D*)f_sm->Get( string("E_scint BackwardSideScatter Lev+0eV TOFcut (T+)").c_str() );
	the_pizza.h_sm_bss_tm = (TH1D*)f_sm->Get( string("E_scint BackwardSideScatter Lev+0eV TOFcut (T-)").c_str() );
	the_pizza.h_sm_bss_bp = (TH1D*)f_sm->Get( string("E_scint BackwardSideScatter Lev+0eV TOFcut (B+)").c_str() );
	the_pizza.h_sm_bss_bm = (TH1D*)f_sm->Get( string("E_scint BackwardSideScatter Lev+0eV TOFcut (B-)").c_str() );
	the_pizza.h_sm_bs_tp = (TH1D*)f_sm->Get( string("E_scint BackScatter Lev+0eV TOFcut (T+)").c_str() );
	the_pizza.h_sm_bs_tm = (TH1D*)f_sm->Get( string("E_scint BackScatter Lev+0eV TOFcut (T-)").c_str() );
	the_pizza.h_sm_bs_bp = (TH1D*)f_sm->Get( string("E_scint BackScatter Lev+0eV TOFcut (B+)").c_str() );
	the_pizza.h_sm_bs_bm = (TH1D*)f_sm->Get( string("E_scint BackScatter Lev+0eV TOFcut (B-)").c_str() );
	
	
	cout << "SM stuff's got gotten, I guess." << endl;
	cout << the_pizza.h_sm_direct_tp << endl;
	cout << "\tattempted name of h_sm_direct_tp:  " << string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T+)").c_str() << endl;
	// 60keV SetB(DirectHit Lev+0eV TOFcut), (T+)
	
	// 20keV SetB E_scint (DirectHit Lev+0eV TOFcut), (T+)
//	f_sm->ls();
	cout << the_pizza.h_sm_direct_tm << endl;
	cout << the_pizza.h_sm_direct_bp << endl;
	cout << the_pizza.h_sm_direct_bm << endl;
	cout << the_pizza.h_sm_fss_tp << endl;
	cout << the_pizza.h_sm_fss_tm << endl;
	cout << the_pizza.h_sm_fss_bp << endl;
	cout << the_pizza.h_sm_fss_bm << endl;
	cout << the_pizza.h_sm_bss_tp << endl;
	cout << the_pizza.h_sm_bss_tm << endl;
	cout << the_pizza.h_sm_bss_bp << endl;
	cout << the_pizza.h_sm_bss_bm << endl;
	cout << the_pizza.h_sm_bs_tp << endl;
	cout << the_pizza.h_sm_bs_tm << endl;
	cout << the_pizza.h_sm_bs_bp << endl;
	cout << the_pizza.h_sm_bs_bm << endl;
	
	
	cout << "guess I'll rename those now." << endl;
	the_pizza.h_sm_direct_tp -> SetName("E_Scint SM DirectHit Lev0eV (T+)");
	the_pizza.h_sm_direct_tm -> SetName("E_Scint SM DirectHit Lev0eV (T-)");
	the_pizza.h_sm_direct_bp -> SetName("E_Scint SM DirectHit Lev0eV (B+)");
	the_pizza.h_sm_direct_bm -> SetName("E_Scint SM DirectHit Lev0eV (B-)");
	the_pizza.h_sm_direct_tp -> SetTitle("E_Scint SM DirectHit Lev0eV (T+)");
	the_pizza.h_sm_direct_tm -> SetTitle("E_Scint SM DirectHit Lev0eV (T-)");
	the_pizza.h_sm_direct_bp -> SetTitle("E_Scint SM DirectHit Lev0eV (B+)");
	the_pizza.h_sm_direct_bm -> SetTitle("E_Scint SM DirectHit Lev0eV (B-)");
	
	the_pizza.h_sm_fss_tp -> SetName("E_Scint SM ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_fss_tm -> SetName("E_Scint SM ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_fss_bp -> SetName("E_Scint SM ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_fss_bm -> SetName("E_Scint SM ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_sm_fss_tp -> SetTitle("E_Scint SM ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_fss_tm -> SetTitle("E_Scint SM ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_fss_bp -> SetTitle("E_Scint SM ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_fss_bm -> SetTitle("E_Scint SM ForwardSideScatter Lev0eV (B-)");

	the_pizza.h_sm_bss_tp -> SetName("E_Scint SM BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_bss_tm -> SetName("E_Scint SM BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_bss_bp -> SetName("E_Scint SM BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_bss_bm -> SetName("E_Scint SM BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_sm_bss_tp -> SetTitle("E_Scint SM BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_bss_tm -> SetTitle("E_Scint SM BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_bss_bp -> SetTitle("E_Scint SM BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_bss_bm -> SetTitle("E_Scint SM BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_sm_bs_tp -> SetName("E_Scint SM BackScatter Lev0eV (T+)");
	the_pizza.h_sm_bs_tm -> SetName("E_Scint SM BackScatter Lev0eV (T-)");
	the_pizza.h_sm_bs_bp -> SetName("E_Scint SM BackScatter Lev0eV (B+)");
	the_pizza.h_sm_bs_bm -> SetName("E_Scint SM BackScatter Lev0eV (B-)");
	the_pizza.h_sm_bs_tp -> SetTitle("E_Scint SM BackScatter Lev0eV (T+)");
	the_pizza.h_sm_bs_tm -> SetTitle("E_Scint SM BackScatter Lev0eV (T-)");
	the_pizza.h_sm_bs_bp -> SetTitle("E_Scint SM BackScatter Lev0eV (B+)");
	the_pizza.h_sm_bs_bm -> SetTitle("E_Scint SM BackScatter Lev0eV (B-)");
	
//	cout << "and now SM stuff has been renamed." << endl;
	
	//
	
	the_pizza.h_bsmp_direct_tp = (TH1D*)f_bsmp->Get("E_Scint DirectHit Lev0eV (T+)");
	the_pizza.h_bsmp_direct_tm = (TH1D*)f_bsmp->Get("E_Scint DirectHit Lev0eV (T-)");
	the_pizza.h_bsmp_direct_bp = (TH1D*)f_bsmp->Get("E_Scint DirectHit Lev0eV (B+)");
	the_pizza.h_bsmp_direct_bm = (TH1D*)f_bsmp->Get("E_Scint DirectHit Lev0eV (B-)");
	the_pizza.h_bsmp_fss_tp = (TH1D*)f_bsmp->Get("E_Scint ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_fss_tm = (TH1D*)f_bsmp->Get("E_Scint ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_fss_bp = (TH1D*)f_bsmp->Get("E_Scint ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_fss_bm = (TH1D*)f_bsmp->Get("E_Scint ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmp_bss_tp = (TH1D*)f_bsmp->Get("E_Scint BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bss_tm = (TH1D*)f_bsmp->Get("E_Scint BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bss_bp = (TH1D*)f_bsmp->Get("E_Scint BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bss_bm = (TH1D*)f_bsmp->Get("E_Scint BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmp_bs_tp = (TH1D*)f_bsmp->Get("E_Scint BackScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bs_tm = (TH1D*)f_bsmp->Get("E_Scint BackScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bs_bp = (TH1D*)f_bsmp->Get("E_Scint BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm = (TH1D*)f_bsmp->Get("E_Scint BackScatter Lev0eV (B-)");
	
	*/
	//
	/*
	the_pizza.h_bsmp_direct_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmp_direct_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmp_direct_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmp_direct_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmp_fss_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmp_fss_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmp_fss_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmp_fss_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmp_bss_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmp_bss_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmp_bss_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmp_bss_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmp_bs_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmp_bs_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmp_bs_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmp_bs_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	*/
	
	/*
	the_pizza.h_bsmp_direct_tp -> SetName("E_Scint BSM+ DirectHit Lev0eV (T+)");
	the_pizza.h_bsmp_direct_tm -> SetName("E_Scint BSM+ DirectHit Lev0eV (T-)");
	the_pizza.h_bsmp_direct_bp -> SetName("E_Scint BSM+ DirectHit Lev0eV (B+)");
	the_pizza.h_bsmp_direct_bm -> SetName("E_Scint BSM+ DirectHit Lev0eV (B-)");
	the_pizza.h_bsmp_direct_tp -> SetTitle("E_Scint BSM+ DirectHit Lev0eV (T+)");
	the_pizza.h_bsmp_direct_tm -> SetTitle("E_Scint BSM+ DirectHit Lev0eV (T-)");
	the_pizza.h_bsmp_direct_bp -> SetTitle("E_Scint BSM+ DirectHit Lev0eV (B+)");
	the_pizza.h_bsmp_direct_bm -> SetTitle("E_Scint BSM+ DirectHit Lev0eV (B-)");
	
	the_pizza.h_bsmp_fss_tp -> SetName("E_Scint BSM+ ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_fss_tm -> SetName("E_Scint BSM+ ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_fss_bp -> SetName("E_Scint BSM+ ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_fss_bm -> SetName("E_Scint BSM+ ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmp_fss_tp -> SetTitle("E_Scint BSM+ ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_fss_tm -> SetTitle("E_Scint BSM+ ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_fss_bp -> SetTitle("E_Scint BSM+ ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_fss_bm -> SetTitle("E_Scint BSM+ ForwardSideScatter Lev0eV (B-)");

	the_pizza.h_bsmp_bss_tp -> SetName("E_Scint BSM+ BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bss_tm -> SetName("E_Scint BSM+ BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bss_bp -> SetName("E_Scint BSM+ BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bss_bm -> SetName("E_Scint BSM+ BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmp_bss_tp -> SetTitle("E_Scint BSM+ BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bss_tm -> SetTitle("E_Scint BSM+ BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bss_bp -> SetTitle("E_Scint BSM+ BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bss_bm -> SetTitle("E_Scint BSM+ BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmp_bs_tp -> SetName("E_Scint BSM+ BackScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bs_tm -> SetName("E_Scint BSM+ BackScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bs_bp -> SetName("E_Scint BSM+ BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm -> SetName("E_Scint BSM+ BackScatter Lev0eV (B-)");
	the_pizza.h_bsmp_bs_tp -> SetTitle("E_Scint BSM+ BackScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bs_tm -> SetTitle("E_Scint BSM+ BackScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bs_bp -> SetTitle("E_Scint BSM+ BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm -> SetTitle("E_Scint BSM+ BackScatter Lev0eV (B-)");
	
	cout << "BSMP stuff is gotten and modified." << endl;
	//
	
	the_pizza.h_bsmm_direct_tp = (TH1D*)f_bsmm->Get("E_Scint DirectHit Lev0eV (T+)");
	the_pizza.h_bsmm_direct_tm = (TH1D*)f_bsmm->Get("E_Scint DirectHit Lev0eV (T-)");
	the_pizza.h_bsmm_direct_bp = (TH1D*)f_bsmm->Get("E_Scint DirectHit Lev0eV (B+)");
	the_pizza.h_bsmm_direct_bm = (TH1D*)f_bsmm->Get("E_Scint DirectHit Lev0eV (B-)");
	the_pizza.h_bsmm_fss_tp = (TH1D*)f_bsmm->Get("E_Scint ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_fss_tm = (TH1D*)f_bsmm->Get("E_Scint ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_fss_bp = (TH1D*)f_bsmm->Get("E_Scint ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_fss_bm = (TH1D*)f_bsmm->Get("E_Scint ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmm_bss_tp = (TH1D*)f_bsmm->Get("E_Scint BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bss_tm = (TH1D*)f_bsmm->Get("E_Scint BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bss_bp = (TH1D*)f_bsmm->Get("E_Scint BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bss_bm = (TH1D*)f_bsmm->Get("E_Scint BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmm_bs_tp = (TH1D*)f_bsmm->Get("E_Scint BackScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bs_tm = (TH1D*)f_bsmm->Get("E_Scint BackScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bs_bp = (TH1D*)f_bsmm->Get("E_Scint BackScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bs_bm = (TH1D*)f_bsmm->Get("E_Scint BackScatter Lev0eV (B-)");
	*/
	/*
	the_pizza.h_bsmm_direct_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmm_direct_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmm_direct_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmm_direct_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (DirectHit Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmm_fss_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmm_fss_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmm_fss_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmm_fss_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (ForwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmm_bss_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmm_bss_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmm_bss_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmm_bss_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackwardSideScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	the_pizza.h_bsmm_bs_tp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T+)").c_str() );
	the_pizza.h_bsmm_bs_tm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(T-)").c_str() );
	the_pizza.h_bsmm_bs_bp = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B+)").c_str() );
	the_pizza.h_bsmm_bs_bm = (TH1D*)f_sm->Get( string(bb_str+"keV Set"+matched_set+" E_scint (BackScatter Lev+0eV TOFcut), "+"(B-)").c_str() );
	*/
	/*
	the_pizza.h_bsmm_direct_tp -> SetName("E_Scint BSM- DirectHit Lev0eV (T+)");
	the_pizza.h_bsmm_direct_tm -> SetName("E_Scint BSM- DirectHit Lev0eV (T-)");
	the_pizza.h_bsmm_direct_bp -> SetName("E_Scint BSM- DirectHit Lev0eV (B+)");
	the_pizza.h_bsmm_direct_bm -> SetName("E_Scint BSM- DirectHit Lev0eV (B-)");
	the_pizza.h_bsmm_direct_tp -> SetTitle("E_Scint BSM- DirectHit Lev0eV (T+)");
	the_pizza.h_bsmm_direct_tm -> SetTitle("E_Scint BSM- DirectHit Lev0eV (T-)");
	the_pizza.h_bsmm_direct_bp -> SetTitle("E_Scint BSM- DirectHit Lev0eV (B+)");
	the_pizza.h_bsmm_direct_bm -> SetTitle("E_Scint BSM- DirectHit Lev0eV (B-)");
	
	the_pizza.h_bsmm_fss_tp -> SetName("E_Scint BSM- ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_fss_tm -> SetName("E_Scint BSM- ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_fss_bp -> SetName("E_Scint BSM- ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_fss_bm -> SetName("E_Scint BSM- ForwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmm_fss_tp -> SetTitle("E_Scint BSM- ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_fss_tm -> SetTitle("E_Scint BSM- ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_fss_bp -> SetTitle("E_Scint BSM- ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_fss_bm -> SetTitle("E_Scint BSM- ForwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmm_bss_tp -> SetName("E_Scint BSM- BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bss_tm -> SetName("E_Scint BSM- BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bss_bp -> SetName("E_Scint BSM- BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bss_bm -> SetName("E_Scint BSM- BackwardSideScatter Lev0eV (B-)");
	the_pizza.h_bsmm_bss_tp -> SetTitle("E_Scint BSM- BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bss_tm -> SetTitle("E_Scint BSM- BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bss_bp -> SetTitle("E_Scint BSM- BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bss_bm -> SetTitle("E_Scint BSM- BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmm_bs_tp -> SetName("E_Scint BSM- BackScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bs_tm -> SetName("E_Scint BSM- BackScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bs_bp -> SetName("E_Scint BSM- BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm -> SetName("E_Scint BSM- BackScatter Lev0eV (B-)");
	the_pizza.h_bsmm_bs_tp -> SetTitle("E_Scint BSM- BackScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bs_tm -> SetTitle("E_Scint BSM- BackScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bs_bp -> SetTitle("E_Scint BSM- BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm -> SetTitle("E_Scint BSM- BackScatter Lev0eV (B-)");
	
	cout << "BSMM spectra are got and modded." << endl;
	*/
	/*
	int N_rebin = 10;
	cout << "Rebinning..." << endl;
	the_pizza.h_sm_direct_tp -> Rebin(N_rebin);
	the_pizza.h_sm_direct_tm -> Rebin(N_rebin);
	the_pizza.h_sm_direct_bp -> Rebin(N_rebin);
	the_pizza.h_sm_direct_bm -> Rebin(N_rebin);
	the_pizza.h_sm_fss_tp -> Rebin(N_rebin);
	the_pizza.h_sm_fss_tm -> Rebin(N_rebin);
	the_pizza.h_sm_fss_bp -> Rebin(N_rebin);
	the_pizza.h_sm_fss_bm -> Rebin(N_rebin);
	the_pizza.h_sm_bss_tp -> Rebin(N_rebin);
	the_pizza.h_sm_bss_tm -> Rebin(N_rebin);
	the_pizza.h_sm_bss_bp -> Rebin(N_rebin);
	the_pizza.h_sm_bss_bm -> Rebin(N_rebin);
	the_pizza.h_sm_bs_tp -> Rebin(N_rebin);
	the_pizza.h_sm_bs_tm -> Rebin(N_rebin);
	the_pizza.h_sm_bs_bp -> Rebin(N_rebin);
	the_pizza.h_sm_bs_bm -> Rebin(N_rebin);
	//
	the_pizza.h_bsmp_direct_tp -> Rebin(N_rebin);
	the_pizza.h_bsmp_direct_tm -> Rebin(N_rebin);
	the_pizza.h_bsmp_direct_bp -> Rebin(N_rebin);
	the_pizza.h_bsmp_direct_bm -> Rebin(N_rebin);
	the_pizza.h_bsmp_fss_tp -> Rebin(N_rebin);
	the_pizza.h_bsmp_fss_tm -> Rebin(N_rebin);
	the_pizza.h_bsmp_fss_bp -> Rebin(N_rebin);
	the_pizza.h_bsmp_fss_bm -> Rebin(N_rebin);
	the_pizza.h_bsmp_bss_tp -> Rebin(N_rebin);
	the_pizza.h_bsmp_bss_tm -> Rebin(N_rebin);
	the_pizza.h_bsmp_bss_bp -> Rebin(N_rebin);
	the_pizza.h_bsmp_bss_bm -> Rebin(N_rebin);
	the_pizza.h_bsmp_bs_tp -> Rebin(N_rebin);
	the_pizza.h_bsmp_bs_tm -> Rebin(N_rebin);
	the_pizza.h_bsmp_bs_bp -> Rebin(N_rebin);
	the_pizza.h_bsmp_bs_bm -> Rebin(N_rebin);
	//
	the_pizza.h_bsmm_direct_tp -> Rebin(N_rebin);
	the_pizza.h_bsmm_direct_tm -> Rebin(N_rebin);
	the_pizza.h_bsmm_direct_bp -> Rebin(N_rebin);
	the_pizza.h_bsmm_direct_bm -> Rebin(N_rebin);
	the_pizza.h_bsmm_fss_tp -> Rebin(N_rebin);
	the_pizza.h_bsmm_fss_tm -> Rebin(N_rebin);
	the_pizza.h_bsmm_fss_bp -> Rebin(N_rebin);
	the_pizza.h_bsmm_fss_bm -> Rebin(N_rebin);
	the_pizza.h_bsmm_bss_tp -> Rebin(N_rebin);
	the_pizza.h_bsmm_bss_tm -> Rebin(N_rebin);
	the_pizza.h_bsmm_bss_bp -> Rebin(N_rebin);
	the_pizza.h_bsmm_bss_bm -> Rebin(N_rebin);
	the_pizza.h_bsmm_bs_tp -> Rebin(N_rebin);
	the_pizza.h_bsmm_bs_tm -> Rebin(N_rebin);
	the_pizza.h_bsmm_bs_bp -> Rebin(N_rebin);
	the_pizza.h_bsmm_bs_bm -> Rebin(N_rebin);
	*/
	
	/*
	// Background spectra:
	the_pizza.h_bg_tp = (TH1D*)f_bg->Get("E_Scint Sum:  All BG, Face+Edge, (T+)");
	the_pizza.h_bg_tm = (TH1D*)f_bg->Get("E_Scint Sum:  All BG, Face+Edge, (T-)");
	the_pizza.h_bg_bp = (TH1D*)f_bg->Get("E_Scint Sum:  All BG, Face+Edge, (B+)");
	the_pizza.h_bg_bm = (TH1D*)f_bg->Get("E_Scint Sum:  All BG, Face+Edge, (B-)");
	the_pizza.h_bg_tp -> SetName("E_Scint Summed BG in TOF region (T+)");
	the_pizza.h_bg_tm -> SetName("E_Scint Summed BG in TOF region (T-)");
	the_pizza.h_bg_bp -> SetName("E_Scint Summed BG in TOF region (B+)");
	the_pizza.h_bg_bm -> SetName("E_Scint Summed BG in TOF region (B-)");
	the_pizza.h_bg_tp -> SetTitle("E_Scint Summed BG in TOF region (T+)");
	the_pizza.h_bg_tm -> SetTitle("E_Scint Summed BG in TOF region (T-)");
	the_pizza.h_bg_bp -> SetTitle("E_Scint Summed BG in TOF region (B+)");
	the_pizza.h_bg_bm -> SetTitle("E_Scint Summed BG in TOF region (B-)");
	
	cout << "bg spectra has been gotten and tweaked." << endl;
	// Normalization:  BG is normalized to match Data Set B.  
	// BG has Integral=351.456 in range.
	// What I really want to know for this is, what is the normalization for the total number of events in the TOF window of the simulation, given this normalization of the bg spectra.
	
	double the_G4_integral = 0.0;
	the_G4_integral += the_pizza.h_sm_direct_tp->Integral();
	the_G4_integral += the_pizza.h_sm_direct_tm->Integral();
	the_G4_integral += the_pizza.h_sm_direct_bp->Integral();
	the_G4_integral += the_pizza.h_sm_direct_bm->Integral();
	the_G4_integral += the_pizza.h_sm_fss_tp->Integral();
	the_G4_integral += the_pizza.h_sm_fss_tm->Integral();
	the_G4_integral += the_pizza.h_sm_fss_bp->Integral();
	the_G4_integral += the_pizza.h_sm_fss_bm->Integral();
	the_G4_integral += the_pizza.h_sm_bss_tp->Integral();
	the_G4_integral += the_pizza.h_sm_bss_tm->Integral();
	the_G4_integral += the_pizza.h_sm_bss_bp->Integral();
	the_G4_integral += the_pizza.h_sm_bss_bm->Integral();
	the_G4_integral += the_pizza.h_sm_bs_tp->Integral();
	the_G4_integral += the_pizza.h_sm_bs_tm->Integral();
	the_G4_integral += the_pizza.h_sm_bs_bp->Integral();
	the_G4_integral += the_pizza.h_sm_bs_bm->Integral();
	cout << "the_G4_integral = " << the_G4_integral << endl;
	
	double the_orig_BG_integral = 0.0;
	the_orig_BG_integral += the_pizza.h_bg_tp->Integral();
	the_orig_BG_integral += the_pizza.h_bg_tm->Integral();
	the_orig_BG_integral += the_pizza.h_bg_bp->Integral();
	the_orig_BG_integral += the_pizza.h_bg_bm->Integral();
	cout << "the_orig_BG_integral = " << the_orig_BG_integral << endl;
	
	// scale it so that  new_bg_integral/(the_G4_integral+new_bg_integral) = 351.456/366217.0
	// ie:  the_G4_integral*(351.456/366217.0) + new_bg_integral*(351.456/366217.0) = new_bg_integral
	//   :  (1.0 - (351.456/366217.0))*new_bg_integral = the_G4_integral*(351.456/366217.0)
	//   :  new_bg_integral = the_G4_integral * (351.456/366217.0)/(1.0 - (351.456/366217.0))
	double scale_B = (351.456/366217.0);
	double new_bg_integral = the_G4_integral*(scale_B)/(1.0 - scale_B);
	cout << "new_bg_integral = " << new_bg_integral << endl;
	
	the_pizza.h_bg_tp -> Scale(new_bg_integral / the_orig_BG_integral);
	the_pizza.h_bg_tm -> Scale(new_bg_integral / the_orig_BG_integral);
	the_pizza.h_bg_bp -> Scale(new_bg_integral / the_orig_BG_integral);
	the_pizza.h_bg_bm -> Scale(new_bg_integral / the_orig_BG_integral);
	the_pizza.h_bg_tp -> ResetStats();
	the_pizza.h_bg_tm -> ResetStats();
	the_pizza.h_bg_bp -> ResetStats();
	the_pizza.h_bg_bm -> ResetStats();
	
	cout << "Pizza created." << endl;
	//
	return the_pizza;
}
pizza get_pizza_old(string mini_pizzafname)
{
	string output_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";
//	string fname   = output_path+mini_pizzafname;
	string matched_set="B";
	int threshold_index=0;
	double sigma_cut=3.0;
	int bb1_threshold=60.0;
	double bb1_maxr=15.5;
	
	
	string additional_filename_info = make_additional_filename_info(threshold_index, sigma_cut, bb1_maxr);
	additional_filename_info=additional_filename_info+"_old";
	string bb_str = int_to_string(bb1_threshold);
	
	string fname = output_path+mini_pizzafname+additional_filename_info+"_Set"+matched_set+"_thr"+bb_str+".root";
	cout << "get_pizza_old is looking for file:  " << fname << endl;
	
	TFile * pizzafile   = new TFile( (fname).c_str() );
	pizza the_pizza;
	
	the_pizza.matched_set     = matched_set;
	the_pizza.threshold_index = threshold_index;
	the_pizza.sigma_cut       = sigma_cut;
	the_pizza.bb1_threshold   = bb1_threshold;
	the_pizza.bb1_maxr        = bb1_maxr;
	
	//
	the_pizza.h_sm_direct_tp = (TH1D*)pizzafile->Get("E_Scint SM DirectHit Lev0eV (T+)");
	the_pizza.h_sm_direct_tm = (TH1D*)pizzafile->Get("E_Scint SM DirectHit Lev0eV (T-)");
	the_pizza.h_sm_direct_bp = (TH1D*)pizzafile->Get("E_Scint SM DirectHit Lev0eV (B+)");
	the_pizza.h_sm_direct_bm = (TH1D*)pizzafile->Get("E_Scint SM DirectHit Lev0eV (B-)");
	
	the_pizza.h_sm_fss_tp = (TH1D*)pizzafile->Get("E_Scint SM ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_fss_tm = (TH1D*)pizzafile->Get("E_Scint SM ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_fss_bp = (TH1D*)pizzafile->Get("E_Scint SM ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_fss_bm = (TH1D*)pizzafile->Get("E_Scint SM ForwardSideScatter Lev0eV (B-)");

	the_pizza.h_sm_bss_tp = (TH1D*)pizzafile->Get("E_Scint SM BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_sm_bss_tm = (TH1D*)pizzafile->Get("E_Scint SM BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_sm_bss_bp = (TH1D*)pizzafile->Get("E_Scint SM BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_sm_bss_bm = (TH1D*)pizzafile->Get("E_Scint SM BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_sm_bs_tp = (TH1D*)pizzafile->Get("E_Scint SM BackScatter Lev0eV (T+)");
	the_pizza.h_sm_bs_tm = (TH1D*)pizzafile->Get("E_Scint SM BackScatter Lev0eV (T-)");
	the_pizza.h_sm_bs_bp = (TH1D*)pizzafile->Get("E_Scint SM BackScatter Lev0eV (B+)");
	the_pizza.h_sm_bs_bm = (TH1D*)pizzafile->Get("E_Scint SM BackScatter Lev0eV (B-)");
	
	//
	the_pizza.h_bsmp_direct_tp = (TH1D*)pizzafile->Get("E_Scint BSM+ DirectHit Lev0eV (T+)");
	the_pizza.h_bsmp_direct_tm = (TH1D*)pizzafile->Get("E_Scint BSM+ DirectHit Lev0eV (T-)");
	the_pizza.h_bsmp_direct_bp = (TH1D*)pizzafile->Get("E_Scint BSM+ DirectHit Lev0eV (B+)");
	the_pizza.h_bsmp_direct_bm = (TH1D*)pizzafile->Get("E_Scint BSM+ DirectHit Lev0eV (B-)");
	
	the_pizza.h_bsmp_fss_tp = (TH1D*)pizzafile->Get("E_Scint BSM+ ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_fss_tm = (TH1D*)pizzafile->Get("E_Scint BSM+ ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_fss_bp = (TH1D*)pizzafile->Get("E_Scint BSM+ ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_fss_bm = (TH1D*)pizzafile->Get("E_Scint BSM+ ForwardSideScatter Lev0eV (B-)");

	the_pizza.h_bsmp_bss_tp = (TH1D*)pizzafile->Get("E_Scint BSM+ BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bss_tm = (TH1D*)pizzafile->Get("E_Scint BSM+ BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bss_bp = (TH1D*)pizzafile->Get("E_Scint BSM+ BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bss_bm = (TH1D*)pizzafile->Get("E_Scint BSM+ BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmp_bs_tp = (TH1D*)pizzafile->Get("E_Scint BSM+ BackScatter Lev0eV (T+)");
	the_pizza.h_bsmp_bs_tm = (TH1D*)pizzafile->Get("E_Scint BSM+ BackScatter Lev0eV (T-)");
	the_pizza.h_bsmp_bs_bp = (TH1D*)pizzafile->Get("E_Scint BSM+ BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm = (TH1D*)pizzafile->Get("E_Scint BSM+ BackScatter Lev0eV (B-)");
	
	
	//
	the_pizza.h_bsmm_direct_tp = (TH1D*)pizzafile->Get("E_Scint BSM- DirectHit Lev0eV (T+)");
	the_pizza.h_bsmm_direct_tm = (TH1D*)pizzafile->Get("E_Scint BSM- DirectHit Lev0eV (T-)");
	the_pizza.h_bsmm_direct_bp = (TH1D*)pizzafile->Get("E_Scint BSM- DirectHit Lev0eV (B+)");
	the_pizza.h_bsmm_direct_bm = (TH1D*)pizzafile->Get("E_Scint BSM- DirectHit Lev0eV (B-)");
	
	the_pizza.h_bsmm_fss_tp = (TH1D*)pizzafile->Get("E_Scint BSM- ForwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_fss_tm = (TH1D*)pizzafile->Get("E_Scint BSM- ForwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_fss_bp = (TH1D*)pizzafile->Get("E_Scint BSM- ForwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_fss_bm = (TH1D*)pizzafile->Get("E_Scint BSM- ForwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmm_bss_tp = (TH1D*)pizzafile->Get("E_Scint BSM- BackwardSideScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bss_tm = (TH1D*)pizzafile->Get("E_Scint BSM- BackwardSideScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bss_bp = (TH1D*)pizzafile->Get("E_Scint BSM- BackwardSideScatter Lev0eV (B+)");
	the_pizza.h_bsmm_bss_bm = (TH1D*)pizzafile->Get("E_Scint BSM- BackwardSideScatter Lev0eV (B-)");
	
	the_pizza.h_bsmm_bs_tp = (TH1D*)pizzafile->Get("E_Scint BSM- BackScatter Lev0eV (T+)");
	the_pizza.h_bsmm_bs_tm = (TH1D*)pizzafile->Get("E_Scint BSM- BackScatter Lev0eV (T-)");
	the_pizza.h_bsmm_bs_bp = (TH1D*)pizzafile->Get("E_Scint BSM- BackScatter Lev0eV (B+)");
	the_pizza.h_bsmp_bs_bm = (TH1D*)pizzafile->Get("E_Scint BSM- BackScatter Lev0eV (B-)");

//	cout << "h_bsmm_bs_tp=" << the_pizza.h_bsmm_bs_tp << endl;
//	cout << "h_bsmm_bs_tm=" << the_pizza.h_bsmm_bs_tm << endl;
//	cout << "h_bsmm_bs_bp=" << the_pizza.h_bsmm_bs_bp << endl;
//	cout << "h_bsmm_bs_bm=" << the_pizza.h_bsmm_bs_bm << endl;
	
	//
	the_pizza.h_bg_tp = (TH1D*)pizzafile->Get("E_Scint Summed BG in TOF region (T+)");
	the_pizza.h_bg_tm = (TH1D*)pizzafile->Get("E_Scint Summed BG in TOF region (T-)");
	the_pizza.h_bg_bp = (TH1D*)pizzafile->Get("E_Scint Summed BG in TOF region (B+)");
	the_pizza.h_bg_bm = (TH1D*)pizzafile->Get("E_Scint Summed BG in TOF region (B-)");
	
	cout << "Pizza acquired!" << endl;
	return the_pizza;
}
void store_pizza_old(pizza the_pizza, string mini_pizzafname)
{
//	matched_set = matched_set_;
//	threshold_index = threshold_index_;
//	sigma_cut = sigma_cut_;
//	bb1_threshold = bb1_threshold_;
//	bb1_maxr = bb1_maxr_;
	string additional_filename_info = make_additional_filename_info(the_pizza.threshold_index, the_pizza.sigma_cut, the_pizza.bb1_maxr);
	additional_filename_info=additional_filename_info+"_old";
	string bb_str = int_to_string(the_pizza.bb1_threshold);

	string output_path = "/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";
	string pizza_fname = output_path+mini_pizzafname+additional_filename_info+"_Set"+the_pizza.matched_set+"_thr"+bb_str+".root";   // MELISSA OMG FIX THIS! ...or maybe it's fine?  ugh.
	
	TFile * pizzafile   = new TFile( (pizza_fname).c_str(), "RECREATE");
	pizzafile->cd();

	the_pizza.h_sm_direct_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_direct_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_direct_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_direct_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_fss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_fss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_fss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_fss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bs_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bs_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bs_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_sm_bs_bm -> Write("",TObject::kOverwrite);
	
	the_pizza.h_bsmp_direct_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_direct_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_direct_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_direct_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_fss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_fss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_fss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_fss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_bm -> Write("",TObject::kOverwrite);
	
	the_pizza.h_bsmm_direct_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_direct_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_direct_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_direct_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_fss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_fss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_fss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_fss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bss_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bss_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bss_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bss_bm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bs_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bs_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmm_bs_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bsmp_bs_bm -> Write("",TObject::kOverwrite);
	
	the_pizza.h_bg_tp -> Write("",TObject::kOverwrite);
	the_pizza.h_bg_tm -> Write("",TObject::kOverwrite);
	the_pizza.h_bg_bp -> Write("",TObject::kOverwrite);
	the_pizza.h_bg_bm -> Write("",TObject::kOverwrite);
	
	pizzafile -> Close();
	cout << "Pizza is stored in file: " << pizza_fname << endl;  // GenEnergyAsym.root
	return;
}
*/
