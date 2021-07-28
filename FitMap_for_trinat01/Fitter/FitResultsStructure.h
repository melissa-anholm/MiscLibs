using std::string;

extern string fake_itoa(int);
extern string fake_to_string(double);
//extern setup_location();

extern string fitresults_path;

struct fitresults
{
	fitresults():
		resultsfile("."), orig_pizzafile_B("."), orig_pizzafile_CD("."), 
		orig_datafile_B("."), orig_datafile_C("."), orig_datafile_D("."), 
		Amin(0), Amax(0), bmin(0), bmax(0), dA(0), db(0), Nvals_A(0), Nvals_b(0), zoomlevel(-1), 
		BS_scale(0), SS_scale(0), BG_scale(0), 
		bb1_snr_thresh(0), bb1_sigma_agr(0), bb1_maxr(0), bb1_Ethresh(0),
		min_chi2(0), min_chi2_B(0), min_chi2_C(0), min_chi2_D(0),
		Abest_B(0), Abest_C(0), Abest_D(0), Abest_BCD(0), bbest_B(0), bbest_C(0), bbest_D(0), bbest_BCD(0), 
		Amin1s_B(0), Amin1s_C(0), Amin1s_D(0), 
		Amin1s_BCD(0), 
		Amax1s_B(0), Amax1s_C(0), Amax1s_D(0), 
		Amax1s_BCD(0), 
		Amin90_BCD(0), 
		Amax90_BCD(0), 
		Amin95_BCD(0), 
		Amax95_BCD(0), 
		bmin1s_B(0), bmin1s_C(0), bmin1s_D(0), 
		bmin1s_BCD(0), 
		bmax1s_B(0), bmax1s_C(0), bmax1s_D(0), 
		bmax1s_BCD(0), 
		bmin90_BCD(0), 
		bmax90_BCD(0), 
		bmin95_BCD(0), 
		bmax95_BCD(0), 
		bbest_BCD_v2(0), bmin1s_BCD_v2(0), bmax1s_BCD_v2(0), Abest_BCD_v2(0), Amin1s_BCD_v2(0), Amax1s_BCD_v2(0), min_chi2_v2(0), 
		Emin(400), Emax(1800)
	{};
	
	// Relevant Filenames:
	string resultsfile;
	string orig_pizzafile_B;
	string orig_pizzafile_CD;
	string orig_datafile_B;
	string orig_datafile_C;
	string orig_datafile_D;


	// fit ranges:
	double Amin, Amax;
	double bmin, bmax;
	double dA, db;
	int Nvals_A, Nvals_b; 
	int zoomlevel;
	
	// Parameters:
	double BS_scale;
	double SS_scale;
	double BG_scale;
	
	int bb1_snr_thresh;
	double bb1_sigma_agr;
	double bb1_maxr;
	double bb1_Ethresh;
	
	
	// fit results:
	double min_chi2, min_chi2_B, min_chi2_C, min_chi2_D;
	
	double Abest_B, Abest_C, Abest_D, Abest_BCD;
	double bbest_B, bbest_C, bbest_D, bbest_BCD;
	
	double Amin1s_B, Amin1s_C, Amin1s_D, Amin1s_BCD;
	double Amax1s_B, Amax1s_C, Amax1s_D, Amax1s_BCD;
	
	double bmin1s_B, bmin1s_C, bmin1s_D, bmin1s_BCD;
	double bmax1s_B, bmax1s_C, bmax1s_D, bmax1s_BCD;

	double Amin90_BCD;
	double Amax90_BCD;

	double bmin90_BCD;
	double bmax90_BCD;

	double Amin95_BCD;
	double Amax95_BCD;

	double bmin95_BCD;
	double bmax95_BCD;
	//
	double bbest_BCD_v2, bmin1s_BCD_v2, bmax1s_BCD_v2;
	double Abest_BCD_v2, Amin1s_BCD_v2, Amax1s_BCD_v2;
	double min_chi2_v2;
	
	double Emin, Emax;
	
	void save_descriptorfile(string resultsfilename ="tmp.txt", string path="")
	{
		bool use_v2=false;
		use_v2=true;
		
		if(bbest_BCD_v2 || bmin1s_BCD_v2 || bmax1s_BCD_v2 || Abest_BCD_v2 || Amin1s_BCD_v2 || Amax1s_BCD_v2)
		{
			use_v2=true;
		}
		
		FILE *io_file;
		io_file = fopen( (path+resultsfilename).c_str(), "a+");  // create the file if it doesn't exist.
		if(io_file==NULL)
		{
			cout << "ERROR:  " << path+resultsfilename << " could not be opened." << endl;
			return;
		}
		fprintf(io_file, "%s",     "///  --- *** ---  ///  --- *** ---  ///\n");
		fprintf(io_file, "%s",     "Fit Results Summary:  \n\n");
		//
		fprintf(io_file, "%s",     "Using Files:  \n");
	//	fprintf(io_file, "%s%s%s", "resultsfile:    ", resultsfile.c_str(),    "\n");
		fprintf(io_file, "%s%s%s", "pizza_B:  ", orig_pizzafile_B.c_str(),  "\n");
		fprintf(io_file, "%s%s%s", "pizza_CD: ", orig_pizzafile_CD.c_str(), "\n");
		fprintf(io_file, "%s%s%s", "data_B:  ",  orig_datafile_B.c_str(),   "\n");
		fprintf(io_file, "%s%s%s", "data_C:  ",  orig_datafile_C.c_str(),   "\n");
		fprintf(io_file, "%s%s%s", "data_D:  ",  orig_datafile_D.c_str(),   "\n");
		//
		fprintf(io_file, "%s",    "\n");
		fprintf(io_file, "%s",    "Parameter Settings:  \n");
		fprintf(io_file, "%s%f%s","BS_scale:  ", BS_scale*100.0, "\%\n");
		fprintf(io_file, "%s%f%s","SS_scale:  ", SS_scale*100.0, "\%\n");
		fprintf(io_file, "%s%f%s","BG_scale:  ", BG_scale*100.0, "\%\n");
		fprintf(io_file, "%s%i%s","BB1 SNR threshold index: ", bb1_snr_thresh, "\n");
		fprintf(io_file, "%s%f%s","BB1 sigma E agreement:   ", bb1_sigma_agr, "\n");
		fprintf(io_file, "%s%f%s","BB1 max. radius:         ", bb1_maxr, "\n");
		fprintf(io_file, "%s%f%s","BB1 Energy Threshold:    ", bb1_Ethresh, "\n");
		fprintf(io_file, "%s%f%s%f%s","E_scint Range: \t[", Emin, ", ", Emax,"]\n");
		//
		fprintf(io_file, "%s",                 "\n");
		fprintf(io_file, "%s%i%s",             "Nominal Zoomlevel:  ", zoomlevel, "\n");
		fprintf(io_file, "%s",                 "Fit Range:  \n");
		if(use_v2)
		{
			fprintf(io_file, "%s%f%s%f%s%f%s%i%s", "A_beta:  [", Amin, ", ", Amax, "], \tdA=", dA, " (N_A=", Nvals_A, ") -- v2 refinement\n");
			fprintf(io_file, "%s%f%s%f%s%f%s%i%s", "b_Fierz: [", bmin, ", ", bmax, "], \tdb=", db, " (N_b=", Nvals_b, ") -- v2 refinement\n");
		}
		else
		{
			fprintf(io_file, "%s%f%s%f%s%f%s%i%s", "A_beta:  [", Amin, ", ", Amax, "], \tdA=", dA, " (N_A=", Nvals_A, ")\n");
			fprintf(io_file, "%s%f%s%f%s%f%s%i%s", "b_Fierz: [", bmin, ", ", bmax, "], \tdb=", db, " (N_b=", Nvals_b, ")\n");
		}
		//
		
		//
		fprintf(io_file, "%s",            "\n");
		fprintf(io_file, "%s",            "Merged Results (One Sigma Interval):  \n");
	//	if(use_v2)
	//	{
	//		fprintf(io_file, "%s",            "*  Best Parameters:  \n");
	//		fprintf(io_file, "%s%f%s%f%s%f%s","*  b_best=", bbest_BCD_v2, " \t[", bmin1s_BCD_v2, ", ", bmax1s_BCD_v2,"]\t(Sets BCD, one sigma)\n");
	//		fprintf(io_file, "%s%f%s%f%s%f%s","*  A_best=", Abest_BCD_v2, " \t[", Amin1s_BCD_v2, ", ", Amax1s_BCD_v2,"]\t(Sets BCD, one sigma)\n");
	//		fprintf(io_file, "%s%f%s",        "*  Min. chi^2:  ", min_chi2_v2, "\n");
	//		//
	//		fprintf(io_file, "%s",            "\n");
	//		fprintf(io_file, "%s",            "Previous Parameters:  \n");
	//		fprintf(io_file, "%s%f%s%f%s%f%s","( b_best=", bbest_BCD, " \t[", bmin1s_BCD, ", ", bmax1s_BCD,"] )\n");
	//		fprintf(io_file, "%s%f%s%f%s%f%s","( A_best=", Abest_BCD, " \t[", Amin1s_BCD, ", ", Amax1s_BCD,"] )\n");
	//		fprintf(io_file, "%s%f%s",        "( Min. chi^2:  ", min_chi2, " )\n");
	//	}
	//	else
	//	{
			fprintf(io_file, "%s%f%s%f%s%f%s","*  b_best=", bbest_BCD, "  \t[", bmin1s_BCD, ", ", bmax1s_BCD,"] \t(Sets BCD, one sigma)\n");
			fprintf(io_file, "%s%f%s%f%s%f%s","*  A_best=", Abest_BCD, "  \t[", Amin1s_BCD, ", ", Amax1s_BCD,"] \t(Sets BCD, one sigma)\n");
			fprintf(io_file, "%s%f%s",        "*  Min. chi^2:  ", min_chi2, "\n");
	//	}
		//
		if(!use_v2)  // just don't even bother with this if we're zooming in extra.
		{
			fprintf(io_file, "%s",            "\n");
			fprintf(io_file, "%s",            "Merged Results (90\% Confidence Interval):  \n");
			fprintf(io_file, "%s%f%s%f%s%f%s","b_best=", bbest_BCD, " \t[", bmin90_BCD, ", ", bmax90_BCD,"]\t(Sets BCD, 90\% CI)\n");
			fprintf(io_file, "%s%f%s%f%s%f%s","A_best=", Abest_BCD, " \t[", Amin90_BCD, ", ", Amax90_BCD,"]\t(Sets BCD, 90\% CI)\n");
			fprintf(io_file, "%s",            "Merged Results (95\% Confidence Interval):  \n");
			fprintf(io_file, "%s%f%s%f%s%f%s","b_best=", bbest_BCD, " \t[", bmin95_BCD, ", ", bmax95_BCD,"]\t(Sets BCD, 95\% CI)\n");
			fprintf(io_file, "%s%f%s%f%s%f%s","A_best=", Abest_BCD, " \t[", Amin95_BCD, ", ", Amax95_BCD,"]\t(Sets BCD, 95\% CI)\n");
			
			fprintf(io_file, "%s",            "\n");
			fprintf(io_file, "%s",            "Individual Results (One Sigma Interval):  \n");
			fprintf(io_file, "%s%f%s%f%s%f%s","   b_B=", bbest_B,   " \t[", bmin1s_B,   ", ", bmax1s_B,  "]\t(Set B, one sigma)\n");
			fprintf(io_file, "%s%f%s%f%s%f%s","   A_B=", Abest_B,   " \t[", Amin1s_B,   ", ", Amax1s_B,  "]\t(Set B, one sigma)\n");
			fprintf(io_file, "%s",            "\n");
			fprintf(io_file, "%s%f%s%f%s%f%s","   b_C=", bbest_C,   " \t[", bmin1s_C,   ", ", bmax1s_C,  "]\t(Set C, one sigma)\n");
			fprintf(io_file, "%s%f%s%f%s%f%s","   A_C=", Abest_C,   " \t[", Amin1s_C,   ", ", Amax1s_C,  "]\t(Set C, one sigma)\n");
			fprintf(io_file, "%s",            "\n");
			fprintf(io_file, "%s%f%s%f%s%f%s","   b_D=", bbest_D,   " \t[", bmin1s_D,   ", ", bmax1s_D,  "]\t(Set D, one sigma)\n");
			fprintf(io_file, "%s%f%s%f%s%f%s","   A_D=", Abest_D,   " \t[", Amin1s_D,   ", ", Amax1s_D,  "]\t(Set D, one sigma)\n");
			fprintf(io_file, "%s",            "\n");
		}
		//
		//
		fprintf(io_file, "%s",    "\n");
		fclose(io_file);
		//
		cout << "Fit Results Summary has been saved to:  " << path+resultsfilename << endl;
	};
};
