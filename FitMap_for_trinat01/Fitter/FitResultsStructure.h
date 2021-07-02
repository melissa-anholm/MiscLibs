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
		bmax95_BCD(0)
	{
		
	};
	
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
	
	
	
	/*
	string make_string_ofresults()
	{
		string the_string;
		the_string = "FitParams:  N_A x N_b = "+int_to_string(Nvals_A)+" x "+int_to_string(Nvals_b)+", Arange=["+fake_to_string(Amin)+", "+fake_to_string(Amax)"], brange=["+fake_to_string(bmin)+", "+fake_to_string(bmax)+"], zoomout="+int_to_string(zoomout);
		the_string = the_string+", \tAbest_BCD="+fake_to_string(Abest_BCD)+"["+fake_to_string(Amin1s_BCD)+", "+fake_to_string(Amax1s_BCD)+"], Abest_B="+fake_to_string(Abest_B)+"["+fake_to_string(Amin1s_B)+", "+fake_to_string(Amax1s_B)+"], Abest_C="+fake_to_string(Abest_C)+"["+fake_to_string(Amin1s_C)+", "+fake_to_string(Amax1s_C)+"], Abest_D="+fake_to_string(Abest_D)+"["+fake_to_string(Amin1s_D)+", "+fake_to_string(Amax1s_D)+"]";
		the_string = the_string+", \tbb1_snr_thresh="+fake_to_string(bb1_snr_thresh)+", bb1_sigma_agr="+fake_to_string(bb1_sigma_agr)+", bb1_maxr="+fake_to_string(bb1_maxr)+", bb1_Ethresh="+bb1_Ethresh;
		the_string = the_string+", \tBS_scale="+fake_to_string(BS_scale)+", SS_scale="+fake_to_string(SS_scale)+", BG_scale="+fake_to_string(BG_scale);
		return the_string;
	};
	*/
//	void print_header(string resultsfilename);
//	void SaveMetaData(string resultsfilename);
	
	void save_descriptorfile(string resultsfilename ="tmp.txt", string path="")
	{
	//	string path="/Users/anholm/Desktop/Anal-Ysis/Sim_to_Asym/Output/";
	//	string resultsfilename ="tmp.txt";
		
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
		fprintf(io_file, "%s",                 "\n");
		fprintf(io_file, "%s",                 "Fit Range:  \n");
		fprintf(io_file, "%s%f%s%f%s%f%s%i%s", "A_beta:  [", Amin, ", ", Amax, "], \tdA=", dA, "(N_A=", Nvals_A, ")\n");
		fprintf(io_file, "%s%f%s%f%s%f%s%i%s", "b_Fierz: [", bmin, ", ", bmax, "], \tdb=", db, "(N_b=", Nvals_b, ")\n");
		fprintf(io_file, "%s%i%s",             "nominal_zoomlevel:  ", zoomlevel, "\n");
		
		fprintf(io_file, "%s",    "\n");
		fprintf(io_file, "%s",    "Parameter Settings:  \n");
		fprintf(io_file, "%s%f%s","BS_scale:  ", BS_scale*100.0, "\%\n");
		fprintf(io_file, "%s%f%s","SS_scale:  ", SS_scale*100.0, "\%\n");
		fprintf(io_file, "%s%f%s","BG_scale:  ", BG_scale*100.0, "\%\n");
		fprintf(io_file, "%s%i%s","BB1 SNR threshold index: ", bb1_snr_thresh, "\n");
		fprintf(io_file, "%s%f%s","BB1 sigma E agreement:   ", bb1_sigma_agr, "\n");
		fprintf(io_file, "%s%f%s","BB1 max. radius:         ", bb1_maxr, "\n");
		fprintf(io_file, "%s%f%s","BB1 Energy Threshold:    ", bb1_Ethresh, "\n");
		
		fprintf(io_file, "%s",            "\n");
		fprintf(io_file, "%s",            "Merged Results (One Sigma Interval):  \n");
		fprintf(io_file, "%s%f%s%f%s%f%s","A_best=", Abest_BCD, " \t[", Amin1s_BCD, ", ", Amax1s_BCD,"]\t(Sets BCD, one sigma)\n");
		fprintf(io_file, "%s%f%s%f%s%f%s","b_best=", bbest_BCD, " \t[", bmin1s_BCD, ", ", bmax1s_BCD,"]\t(Sets BCD, one sigma)\n");
		fprintf(io_file, "%s",            "\n");
		fprintf(io_file, "%s",            "Merged Results (90\% Confidence Interval):  \n");
		fprintf(io_file, "%s%f%s%f%s%f%s","A_best=", Abest_BCD, " \t[", Amin90_BCD, ", ", Amax90_BCD,"]\t(Sets BCD, 90\% CI)\n");
		fprintf(io_file, "%s%f%s%f%s%f%s","b_best=", bbest_BCD, " \t[", bmin90_BCD, ", ", bmax90_BCD,"]\t(Sets BCD, 90\% CI)\n");
		fprintf(io_file, "%s",            "Merged Results (95\% Confidence Interval):  \n");
		fprintf(io_file, "%s%f%s%f%s%f%s","A_best=", Abest_BCD, " \t[", Amin95_BCD, ", ", Amax95_BCD,"]\t(Sets BCD, 95\% CI)\n");
		fprintf(io_file, "%s%f%s%f%s%f%s","b_best=", bbest_BCD, " \t[", bmin95_BCD, ", ", bmax95_BCD,"]\t(Sets BCD, 95\% CI)\n");
		
		fprintf(io_file, "%s",            "\n");
		fprintf(io_file, "%s",            "Individual Results (One Sigma Interval):  \n");
		fprintf(io_file, "%s%f%s%f%s%f%s","   A_B=", Abest_B,   " \t[", Amin1s_B,   ", ", Amax1s_B,  "]\t(Set B, one sigma)\n");
		fprintf(io_file, "%s%f%s%f%s%f%s","   b_B=", bbest_B,   " \t[", bmin1s_B,   ", ", bmax1s_B,  "]\t(Set B, one sigma)\n");
		fprintf(io_file, "%s",            "\n");
		fprintf(io_file, "%s%f%s%f%s%f%s","   A_C=", Abest_C,   " \t[", Amin1s_C,   ", ", Amax1s_C,  "]\t(Set C, one sigma)\n");
		fprintf(io_file, "%s%f%s%f%s%f%s","   b_C=", bbest_C,   " \t[", bmin1s_C,   ", ", bmax1s_C,  "]\t(Set C, one sigma)\n");
		fprintf(io_file, "%s",            "\n");
		fprintf(io_file, "%s%f%s%f%s%f%s","   A_D=", Abest_D,   " \t[", Amin1s_D,   ", ", Amax1s_D,  "]\t(Set D, one sigma)\n");
		fprintf(io_file, "%s%f%s%f%s%f%s","   b_D=", bbest_D,   " \t[", bmin1s_D,   ", ", bmax1s_D,  "]\t(Set D, one sigma)\n");
		fprintf(io_file, "%s",            "\n");

		//
		fprintf(io_file, "%s",    "\n");
		fclose(io_file);
		//
		cout << "Fit Results Summary has been saved to:  " << path+resultsfilename << endl;
	};
};
/*
void fitresults::print_header(string resultsfilename)
{
	cout << "Called GlobalAggregator::PrintMetaDataHeader()." << endl;
	
	FILE *io_file;
	io_file = fopen(resultsfilename.c_str(), "a+");
	if(io_file == NULL)
	{
		cout << "Couldn't open the MetaData file, for some reason..." << endl;
		cout << "resultsfilename:  " << resultsfilename << endl;
		return;
	}
	
//	fprintf(io_file, "%s", "Run/I:");
	fprintf(io_file, "%s", "resultsfile/C:");
	fprintf(io_file, "%s", "orig_pizzafile/C:");
	fprintf(io_file, "%s", "orig_datafile/C:");
	
	fprintf(io_file, "%s", "has_been_summed/I:");  // formerly "BadFlag"
	fprintf(io_file, "%s", "is_a_sum/I:");  // formerly "is_summed"
	
	fprintf(io_file, "%s", "SaveEventTypes/C:");
	fprintf(io_file, "%s", "matches_runset/C:");
	
	fprintf(io_file, "%s", "EventsGenerated/I:");
	fprintf(io_file, "%s", "EventsSaved/I:");
	fprintf(io_file, "%s", "MinCosTheta/D:");
	//
	fprintf(io_file, "%s", "MonoEnergy_MeV/D");

	fprintf(io_file, "\n");
	fclose(io_file);
}

void fitresults::SaveMetaData(string resultsfilename)
{
	cout << "Called GlobalAggregator::SaveMetaData()." << endl;
	//
	FILE *io_file;
	io_file = fopen(resultsfilename.c_str(), "a+");  // create the file if it doesn't exist.
	if(io_file==NULL)
	{
		cout << "ERROR:  " << resultsfilename << " could not be opened." << endl;
		return;
	}
	// otherwise, do stuff.
	
	fprintf(io_file, "%i\t",   filenumber);
	fprintf(io_file, "%s\t",   GetGlobalMiniName().c_str() );
	fprintf(io_file, "%i\t",   0);  // "has_been_summed/I:"
	fprintf(io_file, "%i\t",   0);  //    "is_a_sum/I:"
	
	fprintf(io_file, "%s\t",   (the_runaction->Get_AcceptanceTypesString()).c_str() ); 
	fprintf(io_file, "%s\t",   (the_atomic_setup->GetMatchedRunsetLetter()).c_str() );     // 
	
	fprintf(io_file, "%i\t",   the_runaction->Get_Nevents_total() );      // number of events generated.
	fprintf(io_file, "%i\t",   the_runaction->Get_Naccepted_total() );    // number of events accepted.
	
	fprintf(io_file, "%f\t",   PGA_mincostheta );  //
	
	fprintf(io_file, "%f\t",   (Efield->GetConstantFieldValue())/(volt/cm) );
	//
	fprintf(io_file, "%f\t",   PGA_monoenergy/MeV );
	
	
	fprintf(io_file, "\n");
	//
	fclose(io_file);
	cout << "Done with GlobalAggregator::SaveMetaData()." << endl;
}
*/
