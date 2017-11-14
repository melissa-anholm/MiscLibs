// ==================================================================== //
// Author:  Melissa Anholm, 2016 - 
// 
// ==================================================================== //
// 
// This will add branches that need to be added to a friendtuple file.
// (original) all_okay, accyclecount, is_pol, is_unpol, is_ac.
// 
// updated to v2, 3.May.2016.
// use 100 mus time to polarize and new scint.E calibrations 
// (upper_scint_E, lower_scint_E).  also, upper_scint_E_res, 
// lower_scint_E_res.
// also-also, dl_x_pos, dl_z_pos.
// 
// 22.8.2017:  Not sure what happened to version 4, but version 5
// features include a one-parameter *thing* to switch between my 
// own computer and trinatdaq, as well as the functionality to 
// properly friendtuple the G4 data.  
// ==================================================================== //
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream> 
#include <utility>

//#include <TApplication.h>
#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>
//#include <TChain.h>
#include <TObjString.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::make_pair;
using std::pair;
using std::min;

#define on_trinatdaq 1

bool is_blinded = false;
bool is_g4      = false;

int version = 6;

#define XSTR(x) #x
#define STR(x) XSTR(x)


//
#ifdef on_trinatdaq
//
	#define path_to_libs /home/trinat/anholm/MiscLibs

	#include STR(path_to_libs/MetaChain.cpp)
	#include STR(path_to_libs/treeql_replacement.cpp)
	#include STR(path_to_libs/BB1/bb1_strip.h)
	#include STR(path_to_libs/mini_cal_maker.cpp)
	
	string blind_r_path = "/data/trinat/S1188_2014_blinded/";
	string blind_e_path = "/data/trinat/S1188_2014_blinded/";
	string blind_o_path = "/home/trinat/anholm/Friends/";

	string unblind_r_path = "/home/trinat/online/analyzedFiles_2014/";
	string unblind_e_path = "/home/trinat/online/analyzedFiles_2014/";
	string unblind_o_path = "/home/trinat/anholm/Friends/";

	string g4_tree_path     = "/home/trinat/anholm/Trinat_Geant/build/Output/";
	string g4_friend_path   = "/home/trinat/anholm/Trinat_Geant/build/Output/Friends/";
	string metadatafilename = "/home/trinat/anholm/Trinat_Geant/build/Output/MetaData.txt";

	string bb1_prefix = "/home/trinat/anholm/MiscLibs/BB1/";

//
#else  // NOT on trinatdaq.
//
	#define path_to_libs /Users/spiffyzha/Packages/MiscLibs

	#include STR(path_to_libs/MetaChain.cpp)
	#include STR(path_to_libs/treeql_replacement.cpp)
	#include STR(path_to_libs/BB1/bb1_strip.h)
	#include STR(path_to_libs/mini_cal_maker.cpp)

	string blind_r_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_2014/";
	string blind_e_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
	string blind_o_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Friends/";

	string unblind_r_path = "/Users/spiffyzha/Desktop/Anal-Ysis/";
	string unblind_e_path = "/Users/spiffyzha/Desktop/Anal-Ysis/";
	string unblind_o_path = "/Users/spiffyzha/Desktop/Anal-Ysis/";

	string g4_tree_path     = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";
	string g4_friend_path   = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/Friends/";
	string metadatafilename = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/MetaData.txt";

	string bb1_prefix = "/Users/spiffyzha/Packages/MiscLibs/BB1/";  // 

//
#endif
//


string make_rootfilename(string name, int parameter, string name2=string(""))
{
	string rootfname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << name << parameter << name2 << ".root";
	rootfname = ss.str();
	return rootfname;
}
string int_to_string(int thisint)
{
	string outstring;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	ss << thisint;
	outstring = ss.str();
	return outstring;
}
TObjString * make_tstring(string descriptor, int value)
{
	string tmp_string = descriptor+int_to_string(value);
	TObjString * string_obj = new TObjString(tmp_string.c_str());
	return string_obj;
}

// Bad Times:
vector<pair<UInt_t, UInt_t> >  ProcessAllOkayForRunsInFile(int runno) 
{
//	cout << "Run:  " << runno << endl;
  string btimes_file = "badTimes.txt";

  vector<pair<UInt_t, UInt_t> > time;
  std::ifstream tf;
  tf.open(btimes_file.c_str(), std::ifstream::in);
  UInt_t r, t1, t2;
  while (tf >> r >> t1 >> t2) 
  {
    if (r == runno) 
    {
      cout << "Something to do for run " << r << endl;
      time.push_back(make_pair(t1, t2));
    }
  }

  for (unsigned int i = 0; i < time.size(); i++) 
  {
    cout << runno << "\t" << time[i].first << "\t" << time[i].second << endl;
  }
  return time;
}
Bool_t get_all_okay_for_event(UInt_t unix_time, vector<pair<UInt_t, UInt_t> > badtimesforrun, int *b, int *skipped)
{
	Bool_t all_okay = kTRUE;
	if ( (*b) < badtimesforrun.size()) 
	{
		if (unix_time >= badtimesforrun[*b].first && unix_time <= badtimesforrun[*b].second) 
		{
		  all_okay = kFALSE;
		  (*skipped)++;
		}
		if (unix_time > badtimesforrun[*b].second) 
		{
		  cout << "Skipped " << (*skipped) << " events during "
		       << badtimesforrun[*b].second-badtimesforrun[*b].first << " seconds" << endl;
		  (*skipped) = 0;
		  (*b)++;
		}
	}
	return all_okay;
}
// Scint. Energy:
Double_t get_upper_E(double qdc, int run, bool g4data=false)
{
	double offset;
	double slope;
	double E;
	if(!g4data)
	{
		if (run < 450) 
		{
			offset = 110.0;  // +/- 0.3
			slope  = 398.5;  // +/- 0.4
		} 
		else 
		{
			offset = 110.7;  // +/- 0.2
			slope  = 388.3;  // +/- 0.4
		}
		E = (qdc - offset) / (slope*1000.0);
	}
	else
	{
		E = qdc;
	}
	
//	if (run <= 449) 
//	{
//		offset = 109.7;  // +/- 0.3
//		slope = 0.3991;  // +/- 0.0004
//	}
//	else // if (run <= 513) 
//	{
//		offset = 110.5;  // +/- 0.3
//		slope = 0.3890;  // +/- 0.0004
//	}
//	double E = (qdc - offset) / slope;
//
//	//	double offset = 107.48;
//	//	double slope;
//	//	if (run < 450) 
//	//	{
//	//		slope = 395.0;
//	//	} 
//	//	else if (run < 483) 
//	//	{
//	//		slope = 386.0;
//	//	} 
//	//	else 
//	//	{
//	//		slope = 385.0;
//	//	}
//	//	double E = 1000.0*(qdc - offset) / slope;
	return E;
}
Double_t get_lower_E(double qdc, int run, bool g4data=false)
{
	double offset;
	double slope;
	double E;
	if(!g4data)
	{
		if (run < 450) 
		{
			offset = 142.0;  // +/- 0.3
			slope  = 423.4;  // +/- 0.4
		} 
		else 
		{
			offset = 143.0;  // +/- 0.3
			slope  = 413.2;  // +/- 0.4
		}
		E = (qdc - offset) / (slope*1000.0);
	}
	else
	{
		E = qdc;
	}
//	if (run <= 449) 
//	{
//		offset = 141.8;  // +/- 0.3
//		slope = 0.4242;  // +/- 0.0005
//	}
//	else // if (run <= 513) 
//	{
//		offset = 142.7;  // +/- 0.3
//		slope = 0.4139;  // +/- 0.0004
//	}
//	double E = (qdc - offset) / slope;
//
//	//	double offset = 143.4;
//	//	double slope;
//	//	if (run < 450) 
//	//	{
//	//		slope = 419.2;
//	//	} 
//	//	else if (run < 483) 
//	//	{
//	//		slope = 410.8;
//	//	} 
//	//	else 
//	//	{
//	//		slope = 409.1;
//	//	}
//	//	double E = 1000.0*(qdc - offset) / slope;
	return E;
}

Double_t get_upper_E_res(double upper_E, int run, bool g4data=false)
{ // E is in units of keV.
	double lambda;
	double E_res;
	if(!g4data)
	{
		if (run < 450) 
		{
			lambda = 1.55;  // +/- 0.09
		}
		else 
		{
			lambda = 1.42;  // +/- 0.08
		}
		E_res = sqrt(lambda*upper_E);
	}
	else
	{
		E_res = 0.0;
	}
	return E_res;
	
//	double res;
//	if (run <= 449) 
//	{
//		res = 1.61;  // +/- 0.09
//	}
//	else // if (run <= 513) 
//	{
//		res = 1.45;  // +/- 0.08
//	}
//	double sigma = sqrt(res*E);
//	return sigma;
}
Double_t get_lower_E_res(double lower_E, int run, bool g4data=false)  
{ // E is in units of keV.
	double lambda;
	double E_res;
	if(!g4data)
	{
		if (run < 450) 
		{
			lambda = 1.28;  // +/- 0.08
		}
		else 
		{
			lambda = 1.32;  // +/- 0.08
		}
		E_res = sqrt(lambda*lower_E);
	}
	else
	{
		E_res = 0.0;
	}
	return E_res;

//	double res;
//	if (run <= 449) 
//	{
//		res = 1.32;  // +/- 0.08
//	}
//	else // if (run <= 513) 
//	{
//		res = 1.33;  // +/- 0.07
//	}
//	double sigma = sqrt(res*E);
//	return sigma;
}


// Cycle Counter:
double time_to_polarize = 100.0;   // ns.
long rollover_count = 1073741824;  // acmot/op seems to loop at ~1038e6.  "Usually."
int ac_cyclelength = 97260;
//int dc_slop = int(12000/50); // events read out 12 microseconds?  I think that's right...
class my_prev_event
{
// kludge to deep copy timestamps.
private:
	int timestamp;
	int timestamp_prev;
public:
	bool started;  // if I were a better human being, I would make this private.
	my_prev_event();
	void update(int);
	int ts();
	int ts_prev();
};
my_prev_event::my_prev_event()
{
	timestamp=0;
	timestamp_prev=0;
	started=false;
}
void my_prev_event::update(int new_timestamp)
{
	started=true;
	timestamp_prev=timestamp;
	timestamp=new_timestamp;
}
int my_prev_event::ts()
{
	return timestamp;
}
int my_prev_event::ts_prev()
{
	return timestamp_prev;
}
// AC/Pol classification:
bool check_pol(int acmottime, double op_delay)
{
	int ac_cycle_mus = 97260*50/1000; // in microseconds.  4863 mus.
	int actime = 2956;
//	double time_to_polarize = 100.0;
	double time_anomaly = 32.0;  // it's, what, 200?  33?  ...32.
	double timing_jitter = 5.0;  // 
	
	bool polarized = false;
	if( ( double(acmottime)/1000.0 > actime + time_to_polarize + op_delay + time_anomaly
			&& double(acmottime)/1000.0 < ac_cycle_mus + time_anomaly - timing_jitter) 
		|| double(acmottime)/1000.0 < time_anomaly-timing_jitter  )
	{ 
		polarized = true;
	}
	return polarized;
}
bool check_unpol(int acmottime, double op_delay)
{
	int actime = 2956;
	double time_anomaly = 32.0;  // it's, what, 200?  33?
	double timing_jitter = 5.0;
	
	bool unpol = false;
	if( (double(acmottime)/1000.0 < actime + op_delay - timing_jitter)    // for all unpolarized data.
		&& (double(acmottime)/1000.0 > time_anomaly + timing_jitter) ) 
	{
		unpol = true; 
	}
	return unpol;
}
bool check_ac(int acmottime, double op_delay)
{
	int actime = 2956;
	double time_anomaly = 32.0;  // it's, what, 200?  33?
	double timing_jitter = 5.0;
	
	bool is_ac = false;
	if( (double(acmottime)/1000.0 < actime + time_anomaly - timing_jitter)            // for true AC-MOT times.
		&& (double(acmottime)/1000.0 > time_anomaly + timing_jitter) ) 
	{
		is_ac = true; 
	}
	return is_ac;
}


enum detector_position
{
	t = 1,
	b = 0
};
enum bb1_axis
{
	x = 0,
	y = 1
};
double sigma_cut = 3.0;
int threshold_index = 0;

//
int main(int argc, char *argv[]) 
{
//	bool is_blinded = true;
	int runno;
	if(argc==2)
	{
		runno = atoi(argv[1]);
	}
	else
	{
		cout << "Must call with a run number." << endl;
		return 0;
	}
	TObjString * version_string = make_tstring(string("ReTuple version "), version);
	cout << "ReTuple version " << version << endl;
	
	if(is_g4)
	{ 
		is_blinded = false; 
	}
	
	string fname;
	string friend_fname;
	double this_opdelay;
	if(!is_g4)
	{
		// polarization classification overhead:
		set_of_runs runs;  // Do NOT use a '*'.  It breaks.  Dunno why.
		this_opdelay = runs.OP_Delay[runno];
		if(is_blinded)
		{
			cout << "Running retuple on blinded datasets." << endl;
			if( runs.good_recoil[runno]==true )
			{
				cout << "Run " << runno << " is a recoil run." << endl;
				fname  = make_rootfilename(blind_r_path+"output00", runno, "_blinded");
				cout << "Using file:  " << fname << endl;
			}
			else if(runs.good_electron[runno]==true)
			{
				cout << "Run " << runno << " is an electron run." << endl;
				fname  = make_rootfilename(blind_e_path+"output00", runno, "_blinded");
				cout << "Using file:  " << fname << endl;
			}
			else
			{
				cout << "BAD.  Check run number." << endl;
				return 0;
			}
			friend_fname = make_rootfilename(blind_o_path+"friend00",runno, "_blinded");
		}
		else
		{
			if( runs.good_recoil[runno]==true )
			{
				cout << "Run " << runno << " is a recoil run." << endl;
				fname  = make_rootfilename(unblind_r_path+"output00", runno);
				cout << "Using file:  " << fname << endl;
			}
			else if(runs.good_electron[runno]==true)
			{
				cout << "Run " << runno << " is an electron run." << endl;
				fname  = make_rootfilename(unblind_e_path+"output00", runno);
				cout << "Using file:  " << fname << endl;
			}
			else
			{
				cout << "BAD.  Check run number." << endl;
				return 0;
			}
			friend_fname = make_rootfilename(unblind_o_path+"friend00",runno);
		}
	}
	else
	{
	//	TTree * MetaTree = load_metadata_tree(metadatafilename);
		this_opdelay = 0.0;
		
		// FIX THESE.
		fname  = make_rootfilename(g4_tree_path+"output_", runno);
		friend_fname = make_rootfilename(g4_friend_path+"friend_", runno);
	}
	
	// Original Tree:
	TFile *f = new TFile(fname.c_str(), "READ");
	if(!(f->IsOpen()))
	{
		cout << "File can't be opened." << endl;
		return 0;
	}
//	cout << "File is open." << endl;
	TTree *tree = new TTree;  // Fix this.  This needs to be a TChain now?  ... No, no it doesn't.
	tree = (TTree*)f->Get("ntuple");
//	cout << "Tree is loaded." << endl;
	
	UInt_t upper_qdc_int;
	UInt_t lower_qdc_int;
	UInt_t unix_time;
	double upper_qdc_d;
	double lower_qdc_d;
	
	UInt_t acmot_last = 0;                 //	TNIM_ACMOT_ON;
	UInt_t dcmot_last = 0;                 //	TNIM_DCMOT_OFF;
	vector<unsigned int> * acmot_all = 0;  //	TNIM_ACMOT_ON_all;
	vector<unsigned int> * dcmot_all = 0;  //	TNIM_DCMOT_OFF_all;
	UInt_t eventtime = 0;
//	cout << "Setting some branch addresses." << endl;
	if(!is_g4)
	{
		tree -> SetBranchAddress("QDC_UpperPMT", &upper_qdc_int);
		tree -> SetBranchAddress("QDC_LowerPMT", &lower_qdc_int);
		tree -> SetBranchAddress("Unix_time", &unix_time);
		
		tree -> SetBranchAddress("TNIM_ACMOT_ON", &acmot_last);
		tree -> SetBranchAddress("TNIM_DCMOT_OFF",&dcmot_last);
		tree -> SetBranchAddress("TNIM_ACMOT_ON_all", &acmot_all);
		tree -> SetBranchAddress("TNIM_DCMOT_OFF_all",&dcmot_all);
		tree -> SetBranchAddress("TNIM_TIMESTAMP", &eventtime);  // 
	}
	else
	{
		tree -> SetBranchAddress("QDC_UpperPMT", &upper_qdc_d);
		tree -> SetBranchAddress("QDC_LowerPMT", &lower_qdc_d);
	}
	int acmot_count;
	int dcmot_count;
	
//	cout << "Setting some more branch addresses." << endl;
	vector<double> *ion_events = 0;
	tree -> SetBranchAddress("TDC_ION_MCP", &ion_events);
	int ion_count = 0;

	vector<double> *x1_dla = 0;
	vector<double> *x2_dla = 0;
	vector<double> *z1_dla = 0;
	vector<double> *z2_dla = 0;
	vector<double> *prev_dlx = 0;
	vector<double> *prev_dlz = 0;
	if(!is_g4)
	{
		tree -> SetBranchAddress("TDC_DL_X1",&x1_dla);
		tree -> SetBranchAddress("TDC_DL_X2",&x2_dla);
		tree -> SetBranchAddress("TDC_DL_Z1",&z1_dla);
		tree -> SetBranchAddress("TDC_DL_Z2",&z2_dla);
	}
	else
	{
		// branches for the x, z position info that already exists.
		tree -> SetBranchAddress("DL_X_Pos",&prev_dlx);
		tree -> SetBranchAddress("DL_Z_Pos",&prev_dlz);
	}
	int x1_count = 0;
	int x2_count = 0;
	int z1_count = 0;
	int z2_count = 0;
	int x_count = 0;
	int z_count = 0;
	
	// BB1s:  
	UInt_t led_count = 0;
	UInt_t photodiode_count = 0;
//	cout << "Now, additional branches." << endl;
	if(!is_g4)
	{
		tree -> SetBranchAddress("TDC_PULSER_LED_Count", &led_count);
		tree -> SetBranchAddress("TDC_PHOTO_DIODE_Count", &photodiode_count);
	}
	vector<double> * scint_time_t = 0;
	vector<double> * scint_time_b = 0;
	tree -> SetBranchAddress("TDC_SCINT_TOP",    &scint_time_t);  
	tree -> SetBranchAddress("TDC_SCINT_BOTTOM", &scint_time_b);  
//	vector<double> *emcp_t = 0;
//	vector<double> *imcp_t = 0;
//	tree -> SetBranchAddress("TDC_ELECTRON_MCP", &emcp_t);
//	tree -> SetBranchAddress("TDC_ION_MCP",      &imcp_t);
	
//	cout << "These branches will be strip detector branches." << endl;
	BB1Detector stripdetector[2][2];
//	cout << "made an array of BB1s." << endl;
	string tdiff_file[2] = {bb1_prefix+"bb1_u_tdiff.dat", bb1_prefix+"bb1_l_tdiff.dat"};  // WHAT DOES THIS SHIT EVEN DO FOR G4 DATA?  ... I think it's fine, because I'll just set everything to be the same.
	if(!is_g4)
	{
		stripdetector[t][x] = BB1Detector(bb1_prefix + "bb1_calibration_" + "UX.dat");
		stripdetector[t][y] = BB1Detector(bb1_prefix + "bb1_calibration_" + "UY.dat");
		stripdetector[b][x] = BB1Detector(bb1_prefix + "bb1_calibration_" + "LX.dat");
		stripdetector[b][y] = BB1Detector(bb1_prefix + "bb1_calibration_" + "LY.dat");
	}
	else
	{
		stripdetector[t][x] = BB1Detector(bb1_prefix + "g4_calibration_" + "UX.dat");
		stripdetector[t][y] = BB1Detector(bb1_prefix + "g4_calibration_" + "UY.dat");
		stripdetector[b][x] = BB1Detector(bb1_prefix + "g4_calibration_" + "LX.dat");
		stripdetector[b][y] = BB1Detector(bb1_prefix + "g4_calibration_" + "LY.dat");
	}
	
	vector<double> *strip_E[2][2];        // strip detector amplitudes.
	vector<unsigned int> *strip_T[2][2];  // strip detector peak times.
	for (int detector = 0; detector<=1; detector++)
	{ // loop over detectors.
		for (int axis = 0; axis<=1; axis++) 
		{ // loop over bb1 axes.
			strip_E[detector][axis] = 0;
			strip_T[detector][axis] = 0;
			stripdetector[detector][axis].SetTDiffSigWithFile(tdiff_file[detector]);
			stripdetector[detector][axis].det = Detector(detector);  // t or b.
			stripdetector[detector][axis].pl = Plane(axis);          // x or y.
		}
	}
	tree -> SetBranchAddress("BB1_AMPLITUDE_UX", &strip_E[t][x]);
	tree -> SetBranchAddress("BB1_AMPLITUDE_UY", &strip_E[t][y]);
	tree -> SetBranchAddress("BB1_AMPLITUDE_LX", &strip_E[b][x]);
	tree -> SetBranchAddress("BB1_AMPLITUDE_LY", &strip_E[b][y]);
	
	if(!is_g4)
	{
		tree -> SetBranchAddress("BB1_UX_PEAKTIME", &strip_T[t][x]);
		tree -> SetBranchAddress("BB1_UY_PEAKTIME", &strip_T[t][y]);
		tree -> SetBranchAddress("BB1_LX_PEAKTIME", &strip_T[b][x]);
		tree -> SetBranchAddress("BB1_LY_PEAKTIME", &strip_T[b][y]);
	}

	// 
	// Friend Tree:
	cout << "There.  Onwards to the friend tree." << endl;
	TFile *friendfile = new TFile(friend_fname.c_str(), "RECREATE");
	TTree *friend_tree = new TTree("friendtuple", "friendtuple");
	
	Bool_t all_okay = kTRUE;
	Bool_t is_polarized = kFALSE;
	Bool_t is_unpolarized = kFALSE;
	Bool_t is_ac = kFALSE;
	TBranch *all_okay_b = friend_tree -> Branch("all_okay", &all_okay);  
	TBranch *is_polarized_b = friend_tree -> Branch("is_polarized", &is_polarized);  
	TBranch *is_unpolarized_b = friend_tree -> Branch("is_unpolarized", &is_unpolarized);  
	TBranch *is_ac_b = friend_tree -> Branch("is_ac", &is_ac);  
	int cyclecount = 50;
	TBranch * cyclecount_branch = friend_tree -> Branch("AC_CycleCount", &cyclecount);  
	
	Double_t upper_E;
	Double_t lower_E;
	Double_t upper_E_res;
	Double_t lower_E_res;
	TBranch *upper_e_b = friend_tree -> Branch("upper_scint_E", &upper_E);
	TBranch *lower_e_b = friend_tree -> Branch("lower_scint_E", &lower_E);
	TBranch *upper_e_res_branch = friend_tree -> Branch("upper_scint_E_res", &upper_E_res);
	TBranch *lower_e_res_branch = friend_tree -> Branch("lower_scint_E_res", &lower_E_res);


	vector<double> *dl_x_pos = 0;
	vector<double> *dl_z_pos = 0;
	TBranch *x_branch = friend_tree -> Branch("dl_x_pos", &dl_x_pos);
	TBranch *z_branch = friend_tree -> Branch("dl_z_pos", &dl_z_pos);
	dl_x_pos -> clear();
	dl_z_pos -> clear();
	
	// new bb1 things:
	vector<double> * bb1_t_x = 0;
	vector<double> * bb1_t_y = 0;
	vector<double> * bb1_t_E = 0;
	vector<double> * bb1_b_x = 0;
	vector<double> * bb1_b_y = 0;
	vector<double> * bb1_b_E = 0;
	TBranch *bb1_t_x_branch = friend_tree -> Branch("bb1_top_x", &bb1_t_x);
	TBranch *bb1_t_y_branch = friend_tree -> Branch("bb1_top_y", &bb1_t_y);
	TBranch *bb1_t_E_branch = friend_tree -> Branch("bb1_top_E", &bb1_t_E);
	TBranch *bb1_b_x_branch = friend_tree -> Branch("bb1_bottom_x", &bb1_b_x);
	TBranch *bb1_b_y_branch = friend_tree -> Branch("bb1_bottom_y", &bb1_b_y);
	TBranch *bb1_b_E_branch = friend_tree -> Branch("bb1_bottom_E", &bb1_b_E);
	bb1_t_x -> clear();
	bb1_t_y -> clear();
	bb1_t_E -> clear();
	bb1_b_x -> clear();
	bb1_b_y -> clear();
	bb1_b_E -> clear();
	Bool_t bb1_t_pass = kFALSE;
	Bool_t bb1_b_pass = kFALSE;
	TBranch *bb1_t_pass_b = friend_tree -> Branch("bb1_t_pass", &bb1_t_pass);  
	TBranch *bb1_b_pass_b = friend_tree -> Branch("bb1_b_pass", &bb1_b_pass);  
	
	
	if(is_g4)
	{
		TBranch * bb1_ux_time = friend_tree -> Branch("BB1_UX_PEAKTIME", &strip_T[t][x]);
		TBranch * bb1_uy_time = friend_tree -> Branch("BB1_UY_PEAKTIME", &strip_T[t][y]);
		TBranch * bb1_lx_time = friend_tree -> Branch("BB1_LX_PEAKTIME", &strip_T[b][x]);
		TBranch * bb1_ly_time = friend_tree -> Branch("BB1_LY_PEAKTIME", &strip_T[b][y]);
	}
	
	BB1Hit bb1_hit[2] = {BB1Hit(), BB1Hit()};              // 2 detectors.
	BB1Hit bb1_sechit[2] = {BB1Hit(), BB1Hit()};           // 2 detectors.
	BB1Result bb1_result[2] = {BB1Result(), BB1Result()};  // 2 detectors.
	//
	
	// Backscatter Event Classification:
/*
	// is_type1a_t:  
	// 	beta goes up through top SD into top PMT, 
	// 	scatters, back out through top SD, 
	// 	into lower SD, into lower PMT.
	// In the data, that looks like:
	//		1+ hit in top PMT, 2+ hits in top SD, AND
	//		1+ hit in bottom PMT, exactly 1 hit in bottom SD.
	Bool_t is_type1a_t = kFALSE;
	Bool_t is_type1a_b = kFALSE;
	TBranch *is_type1a_t_branch = friend_tree -> Branch("is_type1a_t", &is_type1a_t);  
	TBranch *is_type1a_b_branch = friend_tree -> Branch("is_type1a_b", &is_type1a_b);  
*/
	Bool_t is_type1_t = kFALSE;
	Bool_t is_type1_b = kFALSE;
	TBranch *is_type1_t_branch = friend_tree -> Branch("is_type1_t", &is_type1_t);  
	TBranch *is_type1_b_branch = friend_tree -> Branch("is_type1_b", &is_type1_b);  

/*
	// is_type1b_t:  
	// 	these events are physically similar to is_type1_t, 
	// 	but allows for the fact that SDs don't have perfect efficiency.  
	// 	will get more accidentals in this category though.
	// Physically:
	// 	beta goes up through top SD into top PMT, 
	// 	scatters, back out through top SD, 
	// 	through the lower SD which doesn't record it, 
	// 	into the lower PMT.
	// In the data, that looks like:
	//		1+ hit in top PMT, 2+ hits in top SD, AND
	//		1+ hit in bottom PMT, exactly 0 hits in bottom SD.
	Bool_t is_type1b_t = kFALSE;
	Bool_t is_type1b_b = kFALSE;
	TBranch *is_type1b_t_branch = friend_tree -> Branch("is_type1b_t", &is_type1b_t);  
	TBranch *is_type1b_b_branch = friend_tree -> Branch("is_type1b_b", &is_type1b_b);  

	// is_type1c:  
	// 	Accounting further for SD inefficiency.  
	// 	Can't immediately tell (without TOF info or something)
	// 	which detector was hit first. 
	// 	Even more accidentals in this category.
	// Note:  is_type1c events will include *all* events that are 
	// 	classified as any of the other is_type1* events, as well 
	// 	as some additional events.
	// In the data, that looks like: 
	//		1+ hit in top PMT, any number of top SD hits (including 0), AND
	//		1+ hit in bottom PMT, any number of bottom SD hits.
	Bool_t is_type1c = kFALSE;
	TBranch *is_type1c_branch = friend_tree -> Branch("is_type1c", &is_type1c);  
*/

/*
	// is_type2a_t:
	// 	beta goes up to top SD, scatters, 
	// 	goes down into the bottom SD and bottom PMT.
	// In the data, that looks like this:
	//		0 hits in top PMT, exactly 1 hit in top SD, AND
	//		1+ hit in bottom PMT, exactly 1 hit in bottom SD.
	Bool_t is_type2a_t = kFALSE;
	Bool_t is_type2a_b = kFALSE;
	TBranch *is_type2a_t_branch = friend_tree -> Branch("is_type2a_t", &is_type2a_t);  
	TBranch *is_type2a_b_branch = friend_tree -> Branch("is_type2a_b", &is_type2a_b);  
*/
	Bool_t is_type2_t = kFALSE;
	Bool_t is_type2_b = kFALSE;
	TBranch *is_type2_t_branch = friend_tree -> Branch("is_type2_t", &is_type2_t);  
	TBranch *is_type2_b_branch = friend_tree -> Branch("is_type2_b", &is_type2_b);  

/*
	// is_type2b_t:
	// Physically similar to is_type_2a_t events, but allows for 
	// imperfect SD efficiency.  Can also include accidentals.
	// 	beta goes up to top SD, scatters, 
	// 	passes through the bottom SD without detection,
	// 	and into the bottom PMT.
	// In the data, that looks like this:
	//		0 hits in the top PMT, exactly 1 hit in the top SD, AND
	//		1+ hit in the bottom PMT, exactly 0 hits in the bottom SD.
	Bool_t is_type2b_t = kFALSE;
	Bool_t is_type2b_b = kFALSE;
	TBranch *is_type2b_t_branch = friend_tree -> Branch("is_type2b_t", &is_type2b_t);  
	TBranch *is_type2b_b_branch = friend_tree -> Branch("is_type2b_b", &is_type2b_b);  
	
	// is_type3a_t:
	// 	beta goes up through the top SD, 
	// 	into the top PMT, scatters back down 
	// 	through the top SD again, then down 
	// 	into the bottom SD, where it's absorbed.
	// In the data, that looks like this:  
	//		1+ hit in top PMT, 2+ hits in top SD, 
	//		0 hits in bottom PMT, exactly 1 hit in bottom SD.
	Bool_t is_type3a_t = kFALSE;
	Bool_t is_type3a_b = kFALSE;
	TBranch *is_type3a_t_branch = friend_tree -> Branch("is_type3a_t", &is_type3a_t);  
	TBranch *is_type3a_b_branch = friend_tree -> Branch("is_type3a_b", &is_type3a_b);  
*/

	Bool_t is_type3_t = kFALSE;
	Bool_t is_type3_b = kFALSE;
	TBranch *is_type3_t_branch = friend_tree -> Branch("is_type3_t", &is_type3_t);  
	TBranch *is_type3_b_branch = friend_tree -> Branch("is_type3_b", &is_type3_b);  
/*
	// is_type3b_t:
	// Physically similar to is_type3a_t, but 
	// accounts for imperfect SD efficiency.  
	// Obviously, there'll be accidentals too.  
	// 	beta goes up through the top SD, 
	// 	into the top PMT, scatters back down 
	// 	through the top SD again, then down 
	// 	into the bottom SD, where it's absorbed.
	// In the data, that looks like this:  
	//		1+ hit in top PMT, exactly 1 hits in top SD, 
	//		0 hits in bottom PMT, exactly 1 hit in bottom SD.
	Bool_t is_type3b_t = kFALSE;
	Bool_t is_type3b_b = kFALSE;
	TBranch *is_type3b_t_branch = friend_tree -> Branch("is_type3b_t", &is_type3b_t);  
	TBranch *is_type3b_b_branch = friend_tree -> Branch("is_type3b_b", &is_type3b_b);  
*/

	// is_type4_t:
	// there is no equivalent in the UCNA paper.
	// in reality, this category can contains events that match
	// the type1 and type3 events but aren't recorded as such due
	// to detector inefficiency.  However, it also results from 
	// a 4th "physical scenario".  ie:
	// 	beta goes up through the top SD, into the top PMT, 
	// 	scatters back through the top SD, and gets 
	// 	absorbed/scattered somewhere else in the chamber. 
	// In the data, that looks like this:
	//		1+ hit in top PMT, 2+ hits in top SD.  
	//		0 hits in bottom PMT, exactly 0 hits in bottom SD.
	Bool_t is_type4_t = kFALSE;
	Bool_t is_type4_b = kFALSE;
	TBranch *is_type4_t_branch = friend_tree -> Branch("is_type4_t", &is_type4_t);  
	TBranch *is_type4_b_branch = friend_tree -> Branch("is_type4_b", &is_type4_b);  

	// is_normal_t:
	// Corresponds to an event without backscattering.  
	// In reality, this category can also contain events from 
	// physical scenarios 1, 3, and 4.
	// 	beta goes up, through the top SD, and is absorbed in the top PMT.
	// In the data, that looks like this:
	//		1+ hit in the top PMT, exactly 1 hit in the top SD, AND
	//		0 hits in the bottom PMT, exactly 0 hits in the bottom SD.
	Bool_t is_normal_t = kFALSE;
	Bool_t is_normal_b = kFALSE;
	TBranch *is_normal_t_branch = friend_tree -> Branch("is_normal_t", &is_normal_t);  
	TBranch *is_normal_b_branch = friend_tree -> Branch("is_normal_b", &is_normal_b);  
	
	// is_other_t:
	// Any event that hasn't already been described where the top PMT fires.
	// Mostly, this will be events where something hit the top PMT but
	// was not recorded in the strip detectors.  eg, gammas and LEDs.  
	// However this category will *also* contain some weird event types.
	// In the data, that (usually) looks like this:
	//		1+ hit in the top PMT, 0 hits in the top SD, AND
	//		0 hits in the bottom PMT, 0 hits in the bottom SD.
	// Or it might also look like this:
	//		1+ hit in the top PMT, 0 hits in the top SD, AND 
	//		0 hits in the bottom PMT, 2+ hits in the bottom SD. 
	// Or possibly it might look like this:
	//		1+ hit in the top PMT, 1 hit in the top SD, AND
	//		0 hits in the bottom PMT, 2+ hits in the bottom SD.
	// Or it might look like this:
	//		1+ hit in the top PMT, 2+ hit in the top SD, AND
	//		0 hits in the bottom PMT, 2+ hits in the bottom SD.
	Bool_t is_other = kFALSE;
	TBranch *is_other_branch = friend_tree -> Branch("is_other", &is_other);  
	// SEPARATE THESE INTO "T" AND "B" EVENTS?  WHAT EVEN?
	
	// This info was already available as the size argument in any of several branches, 
	// but just in case you want an explicitly available way to count BB1 hits in an event, 
	// this is now also its own branch.  We stop counting at 2 BB1 hits per detector though, 
	// so in scenarios where, eg, 3 betas hit the top strip detectors, we'll still only 
	// count 2 of them, and only have access (in the ntuples) to information about those. 
	int N_hits_scint_t = 0;
	int N_hits_scint_b = 0;
	TBranch * N_hits_scint_t_branch = friend_tree -> Branch("N_hits_scint_t", &N_hits_scint_t);
	TBranch * N_hits_scint_b_branch = friend_tree -> Branch("N_hits_scint_b", &N_hits_scint_b);
	int N_hits_bb1_t = 0;
	int N_hits_bb1_b = 0;
	TBranch * N_hits_bb1_t_branch = friend_tree -> Branch("N_hits_bb1_t", &N_hits_bb1_t);
	TBranch * N_hits_bb1_b_branch = friend_tree -> Branch("N_hits_bb1_b", &N_hits_bb1_b);
	
	
	
	// bad times overhead:
	vector<pair<UInt_t, UInt_t> > badtimesforrun;
	badtimesforrun = ProcessAllOkayForRunsInFile(runno);
	// cycle count overhead:
	my_prev_event dc_start = my_prev_event();
	my_prev_event ac_start = my_prev_event();
	int delta_ac = 0;
	int dccycle_count=0;
	int accycle_count=0;

	// DLD Calibration overhead:
	int nhits = 0;
	calibration * my_cals = new calibration(runno); 
	pair<double, double> coordinates = make_pair(0.0, 0.0);

	Long64_t nentries = tree->GetEntries();
	cout << "nentries = " << nentries << endl;
	int b=0;
	int skipped = 0;
	//
	
	// Use Metadata from the MetaTree.
	Int_t sigma_plus = -1;
	Double_t polarization = 0.0;
	if(is_g4)
	{
		TTree * MetaTree = load_metadata_tree(metadatafilename);
		int this_runno;
		MetaTree -> SetBranchAddress("Run", &this_runno);
		double this_pol;
		MetaTree -> SetBranchAddress("Polarization", &this_pol);
		double final_pol;
		int final_entry;
	
		int nmetaentries = MetaTree -> GetEntries();
		cout << "nmetaentries = " << nmetaentries << endl;
		for(int i=0; i<nmetaentries; i++)
		{
			MetaTree -> GetEntry(i);
			if(this_runno == runno)
			{
				final_pol = this_pol;
				final_entry = i;
				break;
			}
		}
		cout << "final_pol = " << final_pol << endl;
		cout << "final_entry = " << final_entry << endl;
		
		polarization = final_pol;
		if(final_pol >= 0.0)
		{
			sigma_plus = 0;
		}
		else
		{
			sigma_plus = 1;
		}
		
		TBranch *sigma_plus_branch = friend_tree -> Branch("TTLBit_SigmaPlus", &sigma_plus);  
		TBranch *sigma_plus_branch2 = friend_tree -> Branch("TTLBit_SigmaPlus2", &sigma_plus);  
		TBranch *pol_branch = friend_tree -> Branch("Polarization", &polarization);  
	}
	
	//
	for(int i=0; i<nentries; i++)
	{
		if( (i % 100000) == 0) { cout<<"Reached entry "<< i << endl; }
		tree -> GetEntry(i);
		//
		
		ion_count = ion_events->size();
		if(!is_g4)
		{
			upper_E = get_upper_E(upper_qdc_int, runno, is_g4);
			lower_E = get_lower_E(lower_qdc_int, runno, is_g4);
			upper_E_res = get_upper_E_res(upper_E, runno, is_g4);
			lower_E_res = get_lower_E_res(lower_E, runno, is_g4);
			
			all_okay = get_all_okay_for_event(unix_time, badtimesforrun, &b, &skipped);
			
			x1_count = x1_dla->size();
			x2_count = x2_dla->size();
			z1_count = z1_dla->size();
			z2_count = z2_dla->size();
			
			nhits = min(ion_count, min(min(x1_count, x2_count), min(z1_count, z2_count)));
			for(int j=0; j<nhits; j++)  // WONG ORDER?!? ...nah, it's fine.
			{
				coordinates = my_cals -> 
					apply_calibration((*x1_dla)[j], (*x2_dla)[j], (*z1_dla)[j], (*z2_dla)[j], 5);
				dl_x_pos -> push_back(coordinates.first);
				dl_z_pos -> push_back(coordinates.second);
			}
		}
		else // if(is_g4)
		{
			all_okay = kTRUE;
			
			upper_E = get_upper_E(upper_qdc_d, runno, is_g4);
			lower_E = get_lower_E(lower_qdc_d, runno, is_g4);
			upper_E_res = get_upper_E_res(upper_E, runno, is_g4);
			lower_E_res = get_lower_E_res(lower_E, runno, is_g4);
			
			x_count = prev_dlx->size();
			z_count = prev_dlz->size();
			nhits = min( ion_count, min(x_count, z_count) );
			for(int j=0; j<nhits; j++)  // 
			{
				dl_x_pos -> push_back((*prev_dlx)[j]);
				dl_z_pos -> push_back((*prev_dlz)[j]);
			}
		}
		
		// Set up event types:
		is_type1_t = kFALSE;
		is_type1_b = kFALSE;
		is_type2_t = kFALSE;
		is_type2_b = kFALSE;
		is_type3_t = kFALSE;
		is_type3_b = kFALSE;

		/*
		is_type1a_t = kFALSE;
		is_type1a_b = kFALSE;
		is_type1b_t = kFALSE;
		is_type1b_b = kFALSE;
		is_type1c = kFALSE;

		is_type2a_t = kFALSE;
		is_type2a_b = kFALSE;
		is_type2b_t = kFALSE;
		is_type2b_b = kFALSE;
	
		is_type3a_t = kFALSE;
		is_type3a_b = kFALSE;
		is_type3b_t = kFALSE;
		is_type3b_b = kFALSE;
		*/
		is_type4_t = kFALSE;
		is_type4_b = kFALSE;

		is_normal_t = kFALSE;
		is_normal_b = kFALSE;
	
		is_other = kFALSE;
		
		// BB1 shizzle:
		bb1_t_pass = kFALSE;
		bb1_b_pass = kFALSE;
		
		N_hits_bb1_t = 0;
		N_hits_bb1_b = 0;
		
		N_hits_scint_t = scint_time_t->size();
		N_hits_scint_b = scint_time_b->size();
		if( led_count==0 && photodiode_count==0 && (scint_time_t->size()>0 || scint_time_b->size()>0) )
		{
			is_other = kTRUE;
			for(int detector=0; detector <=1; detector++)
			{
				for (int axis = 0; axis<=1; axis++) 
				{ // loop over bb1 axes.
					if (strip_E[detector][axis] -> size() != 40) 
					{
						cout << "scint_time_t->size() = " << scint_time_t->size() << "; \tscint_time_b->size() = " << scint_time_b->size();
						cout << ";\tStrip [" << detector << "][" << axis << "] has size " << strip_E[detector][axis] -> size() << endl;
						continue;
					}               
					// calculate the energy for every single fucking strip.
					stripdetector[detector][axis].CalcEnergy(*strip_E[detector][axis]);  
					
					// load up peak time for every fucking strip.
					if(!is_g4)
						{ stripdetector[detector][axis].SetMaxT(*strip_T[detector][axis]); }
				//	else
				//	{
				//		// *set* the maxT time for each strip if it's a g4 thing too!  
				//		// ... wait, I did.  Already.  
				//	}
				}
				bb1_result[detector] = GetResult(stripdetector[detector][x], stripdetector[detector][y], threshold_index, sigma_cut); 
			
				bb1_hit[detector] = bb1_result[detector].hit;
				bb1_sechit[detector] = bb1_result[detector].secHit;
			
				if(bb1_hit[detector].pass == true)
				{
					if(detector == t)
					{
						bb1_t_pass = kTRUE;
						bb1_t_x -> push_back( bb1_hit[detector].xpos );
						bb1_t_y -> push_back( bb1_hit[detector].ypos );
						bb1_t_E -> push_back( bb1_hit[detector].energy );
						N_hits_bb1_t++;
					}
					else if(detector == b)
					{
						bb1_b_pass = kTRUE;
						bb1_b_x -> push_back( bb1_hit[detector].xpos );
						bb1_b_y -> push_back( bb1_hit[detector].ypos );
						bb1_b_E -> push_back( bb1_hit[detector].energy );
						N_hits_bb1_b++;
					}
					//
					if(bb1_result[detector].twoHits == true)
					{
						if(bb1_sechit[detector].pass == true)
						{
							if(detector == t)
							{
								bb1_t_x -> push_back( bb1_sechit[detector].xpos );
								bb1_t_y -> push_back( bb1_sechit[detector].ypos );
								bb1_t_E -> push_back( bb1_sechit[detector].energy );
								N_hits_bb1_t++;
							}
							else if(detector == b)
							{
								bb1_b_x -> push_back( bb1_sechit[detector].xpos );
								bb1_b_y -> push_back( bb1_sechit[detector].ypos );
								bb1_b_E -> push_back( bb1_sechit[detector].energy );
								N_hits_bb1_b++;
							}
						}
					}
				}
			}
			// that was the loop over BB1 detectors.  now these things have appropriate values:
			//	N_hits_scint_t, N_hits_scint_b.
			//	N_hits_bb1_t, N_hits_bb1_b,
			//	Still inside the loop to get rid of events we don't want.
			
			// 1A
			if( (N_hits_scint_t>=1 && N_hits_scint_b>=1) && (N_hits_bb1_t==2 && N_hits_bb1_b==1) )
			{
				is_type1_t = kTRUE;
				is_other = kFALSE;
			}
			if( (N_hits_scint_t>=1 && N_hits_scint_b>=1) && (N_hits_bb1_t==1 && N_hits_bb1_b==2) )
			{
				is_type1_b = kTRUE;
				is_other = kFALSE;
			}
			/*
			// 1B
			if( (N_hits_scint_t>=1 && N_hits_scint_b>=1) && (N_hits_bb1_t==2 && N_hits_bb1_b==0) )
			{
				is_type1b_t = kTRUE;
				is_other = kFALSE;
			}
			if( (N_hits_scint_t>=1 && N_hits_scint_b>=1) && (N_hits_bb1_t==0 && N_hits_bb1_b==2) )
			{
				is_type1b_b = kTRUE;
				is_other = kFALSE;
			}
			// 1C
			if( (N_hits_scint_t>=1 && N_hits_scint_b>=1) )
			{
				is_type1c = kTRUE;
				is_other = kFALSE;
			}
			*/
			// 2A
			if( (N_hits_scint_t==0 && N_hits_scint_b>=1) && (N_hits_bb1_t==1 && N_hits_bb1_b==1) )
			{
				is_type2_t = kTRUE;
				is_other = kFALSE;
			}
			if( (N_hits_scint_t>=1 && N_hits_scint_b==0) && (N_hits_bb1_t==1 && N_hits_bb1_b==1) )
			{
				is_type2_b = kTRUE;
				is_other = kFALSE;
			}
			/*
			// 2B
			if( (N_hits_scint_t==0 && N_hits_scint_b>=1) && (N_hits_bb1_t==1 && N_hits_bb1_b==0) )
			{
				is_type2b_t = kTRUE;
				is_other = kFALSE;
			}
			if( (N_hits_scint_t>=1 && N_hits_scint_b==0) && (N_hits_bb1_t==0 && N_hits_bb1_b==1) )
			{
				is_type2b_b = kTRUE;
				is_other = kFALSE;
			}
			*/
			// 3A
			if( (N_hits_scint_t>=1 && N_hits_scint_b==0) && (N_hits_bb1_t==2 && N_hits_bb1_b==1) )
			{
				is_type3_t = kTRUE;
				is_other = kFALSE;
			}
			if( (N_hits_scint_t==0 && N_hits_scint_b>=1) && (N_hits_bb1_t==1 && N_hits_bb1_b==2) )
			{
				is_type3_b = kTRUE;
				is_other = kFALSE;
			}
			/*
			// 3B
			if( (N_hits_scint_t>=1 && N_hits_scint_b==0) && (N_hits_bb1_t==1 && N_hits_bb1_b==1) )
			{
				is_type3b_t = kTRUE;
				is_other = kFALSE;
			}
			if( (N_hits_scint_t==0 && N_hits_scint_b>=1) && (N_hits_bb1_t==1 && N_hits_bb1_b==1) )
			{
				is_type3b_b = kTRUE;
				is_other = kFALSE;
			}
			*/
			// 4
			if( (N_hits_scint_t>=1 && N_hits_scint_b==0) && (N_hits_bb1_t==2 && N_hits_bb1_b==0) )
			{
				is_type4_t = kTRUE;
				is_other = kFALSE;
			}
			if( (N_hits_scint_t==0 && N_hits_scint_b>=1) && (N_hits_bb1_t==0 && N_hits_bb1_b==2) )
			{
				is_type4_b = kTRUE;
				is_other = kFALSE;
			}
			
			// Normal
			if( (N_hits_scint_t>=1 && N_hits_scint_b==0) && (N_hits_bb1_t==1 && N_hits_bb1_b==0) )
			{
				is_normal_t = kTRUE;
			}
			if( (N_hits_scint_t==0 && N_hits_scint_b>=1) && (N_hits_bb1_t==0 && N_hits_bb1_b==1) )
			{
				is_normal_b = kTRUE;
			}
		}
		
		// Is this event polarized?
		if(is_g4)
		{
			all_okay = kTRUE;  // covered.
			is_polarized = kTRUE;
			Bool_t is_unpolarized = kFALSE;
			Bool_t is_ac = kFALSE;
			int cyclecount = 50;
				
			Double_t upper_E;  // ok
			Double_t lower_E;  // ok
			Double_t upper_E_res;  // ok
			Double_t lower_E_res;  // ok
			
			vector<double> *dl_x_pos = 0;  // ok
			vector<double> *dl_z_pos = 0;  // ok
		}
		else // if(!is_g4)
		{
			is_polarized = check_pol(acmot_last, this_opdelay);
			is_unpolarized = check_unpol(acmot_last, this_opdelay);
			is_ac = check_ac(acmot_last, this_opdelay);
		
			if( (is_polarized==kTRUE) && (is_unpolarized==kTRUE) )
			{
				cout << "* I broke logic again." << endl;
			}
		
			// Cycle Counter:
			// First, how many counts in register since the last event?  
			acmot_count = acmot_all -> size();
			dcmot_count = dcmot_all -> size();
			if(dcmot_count>0)
			{
				dc_start.update( (*dcmot_all)[dcmot_count-1] ); // I only care about the last one.
				accycle_count=0; // I reset this below too, but do it again here to protect against rollover.
				dccycle_count++;
			}
			if(dc_start.started == true)
			{
				for (int j=0; j<acmot_count; j++)
				{
					ac_start.update( (*acmot_all)[j] );
					delta_ac = ac_start.ts() - ac_start.ts_prev();
				
					if( (delta_ac >= ac_cyclelength-3 && delta_ac <= ac_cyclelength+3)  \
					|| (delta_ac + rollover_count >= ac_cyclelength-3                   \
						&& delta_ac + rollover_count <= ac_cyclelength+3)               \
					|| (accycle_count == 0) )
					{
						accycle_count++;
					}
					else if(acmot_last == 0) // This shouldn't happen.
					{
						cout << "Bad." << endl;
					}
			//		else if(delta_ac == 0) // this happens sometimes.  don't count it as a cycle.
			//		{
			//			// don't do anything.
			//		}
					else if (delta_ac != 0)
					{
						cout << "I don't even.  delta_ac=" << delta_ac << endl;
					}
				}
				// no timestamp at the end of the 100th cycle, so remind it when it's done.
				if (accycle_count == 100) 
				{
					if( int(eventtime) - ac_start.ts() >= ac_cyclelength )  
					// if the most recent ac start time had acmot_last==0, then:
					// 	(a) it was right at the beginning of ac cycle 100, and 
					// 	(b) eventtime - ac_start.ts is now *huge*.  Also,
					// 	(c) int(eventtime) - ac_start.ts is small.
					// For these events, I *don't* want to trigger the cycle count to increment.
					{
						accycle_count=0;
					}
				}
			}
			cyclecount = accycle_count;
		}
		
		//
		friend_tree -> Fill();
		//
		dl_x_pos -> clear();
		dl_z_pos -> clear();
		
		bb1_t_x -> clear();
		bb1_t_y -> clear();
		bb1_t_E -> clear();
		
		bb1_b_x -> clear();
		bb1_b_y -> clear();
		bb1_b_E -> clear();
		
	}
	if(!is_g4)
	{
		cout << "final unix_time = " << unix_time << endl;
		cout << "Skipped " << skipped << " events in the last skipping." << endl;
	}
	
	friend_tree -> GetCurrentFile() -> Write("",TObject::kOverwrite);  
	version_string -> Write();
	friend_tree -> GetCurrentFile() -> Close();
	
	cout << "Done." << endl;
	return 0;
}