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
// 
// 14.11.2017:  version 6 includes BB1 hit data, which was actually 
// already there in version 5.  New this version is backscatter 
// event classification.
// 
// 11.12.2017:  version 7: use overall BB1 threshold too!  60 keV.
// 
// 26.4.2018:  version 8:  added scintillator resolution 'blur' to g4 
// output.  At some point during version 7, backscatter classifications 
// were also added.  Also at some point the setup_location() function
// was added specifically to manage the clusterfuck that arose from 
// trying to use this code from multiple different computers.
// 
// 12.6.2018:  version 9:  added BB1 detector noise and resolution to 
// g4 data processing.
// 
// 5.11.2018:  version 10:  added dl_x_pos and dl_z_pos branches to G4 output.  
// previously the branches were there, but zero, and G4 position info
// was (also still is) stored in only the DL_X_Pos (etc) branches.  
// Also added at version 10:  partial LE/TE support.  Breaks retupling on 
// files analyzed before the LE/TE analyzer, but LE/TE doesn't crash it.
// 
// still needs to just add all the "orig_LE" branch names back as "orig".
// 
// 14.8.2020:  version 12:  re-did the hex position stuff into a new set 
// of branches, now using the LE data to calculate position.  
// Updated the scintillator walk adjustments to implement a second walkfit 
// adjustment based on the runset.  We also return to using Melissa's 
// polarization check convention after having used Ben's for a while.
// Melissa's probably salvages slightly more data.
// ==================================================================== //

#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream> 
#include <utility>

#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>
#include <TObjString.h>
#include <TRandom3.h>
#include <TF1.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::make_pair;  // unused?
using std::pair;
using std::min;

//
bool is_blinded            = false;
bool is_g4                 = true;
bool use_g4_metadata       = true;
bool apply_scint_res_on_g4 = true;
bool doEmpirical           = true;   // empirical noise on BB1s.  for G4 data.
bool do_rubidium           = false;  // Rb
bool is_old                = false;  // before trailing edge/leading edge madness.
bool loosecut_for_soe      = false;

int version = 12;

//#define XSTR(x) #x
//#define STR(x) XSTR(x)

#include "location.cpp"
#include "MetaChain.cpp"
#include "treeql_replacement.cpp"
#include "BB1/bb1_strip.h"
#include "mini_cal_maker.cpp"
#include "HistExtras.cpp" // v1192_to_ns.

#include "ReHexPos.cpp"
//string bb1_prefix = "/home/trinat/anholm/MiscLibs/BB1/";

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
	string btimes_file = "badTimes.txt";
	vector<pair<UInt_t, UInt_t> > time;
	if(is_g4) 
	{
		return time;
	}

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
		if (run < 450) // A, B
		{
			offset = 110.0;  // +/- 0.3
			slope  = 398.5;  // +/- 0.4
		} 
		else // C, D, (E)
		{
			offset = 110.7;  // +/- 0.2
			slope  = 388.3;  // +/- 0.4
		}
		E = 1000.0*(qdc - offset) / slope;
	}
	else
	{
		E = qdc;
	}
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
		E = 1000.0*(qdc - offset) / slope;
	}
	else
	{
		E = qdc;
	}
	return E;
}

Double_t get_upper_DeltaE(int qdc, int run, bool g4data=false)  // it just returns 0 for G4.  
{
	double DeltaE = 0;
	if(g4data) { return DeltaE; }
	//
	double slope;
	double delta_m;
	double offset;
	double delta_b;
	
	if (run < 450) // A, B
	{
		offset = 110.0;  // +/- 0.3
		slope  = 398.5;  // +/- 0.4
		
		delta_m = 0.4;
		delta_b = 0.3;
	} 
	else // C, D, (E)
	{
		offset = 110.7;  // +/- 0.2
		slope  = 388.3;  // +/- 0.4
		
		delta_m = 0.4;
		delta_b = 0.2;
		
	}
	double E = 1000.0*(qdc - offset) / slope;
	double dEdm = 1000.0*(qdc - offset) / (slope + delta_m) - E;
	double dEdb = 1000.0*(qdc - (offset+delta_b) ) / slope - E;
	//
	DeltaE = sqrt( pow(delta_m*dEdm, 2) + pow(delta_b*dEdb, 2) );
	return DeltaE;
}
Double_t get_lower_DeltaE(int qdc, int run, bool g4data=false)  // just returns 0 for G4.  Might want to fix that sometime.
{
	double DeltaE = 0;
	if(g4data) { return DeltaE; }
	//
	double slope;
	double delta_m;
	double offset;
	double delta_b;
	
	if (run < 450) 
	{
		offset = 142.0;  // +/- 0.3
		slope  = 423.4;  // +/- 0.4
		
		delta_m = 0.4;
		delta_b = 0.3;
	} 
	else 
	{
		offset = 143.0;  // +/- 0.3
		slope  = 413.2;  // +/- 0.4
		
		delta_m = 0.4;
		delta_b = 0.3;
	}
	double E = 1000.0*(qdc - offset) / slope;
	double dEdm = 1000.0*(qdc - offset) / (slope + delta_m) - E;
	double dEdb = 1000.0*(qdc - (offset+delta_b) ) / slope - E;
	//
	DeltaE = sqrt( pow(delta_m*dEdm, 2) + pow(delta_b*dEdb, 2) );
	return DeltaE;
}
/*
Double_t get_upper_DeltaE(int qdc, int run, bool g4data=false)  // it just returns 0 for G4.  Might want to fix that sometime.
{
	double DeltaE = 0;
	if(g4data) { return DeltaE; }
	//
	double offset;
	double slope;
	double DeltaLambda;
	double Deltax0;
	if (run < 450) // A, B
	{
		offset = 110.0;  // +/- 0.3
		Deltax0 = 1000.0*0.3;
		
		slope  = 398.5;  // +/- 0.4
		DeltaLambda = 0.4;
	} 
	else // C, D, (E)
	{
		offset = 110.7;  // +/- 0.2
		Deltax0 = 1000.0*0.2;
		
		slope  = 388.3;  // +/- 0.4
		DeltaLambda = 0.4;
	}
	double x0 = 1000.0*offset;
	double E = (1000.0*(double)(qdc) - 1000.0*offset) / slope;
	
	double DeltaE2 = (-Deltax0/slope)*(-Deltax0/slope) + (-E/slope*DeltaLambda)*(-E/slope*DeltaLambda);
	DeltaE = sqrt(DeltaE2);
	//
	return DeltaE;
}
Double_t get_lower_DeltaE(int qdc, int run, bool g4data=false)  // just returns 0 for G4.  Might want to fix that sometime.
{
	double DeltaE = 0;
	if(g4data) { return DeltaE; }
	//
	double offset;
	double slope;
	double DeltaLambda;
	double Deltax0;
	if (run < 450) 
	{
		offset = 142.0;  // +/- 0.3
		Deltax0 = 1000.0*0.3;
		
		slope  = 423.4;  // +/- 0.4
		DeltaLambda = 0.4;
	} 
	else 
	{
		offset = 143.0;  // +/- 0.3
		Deltax0 = 1000.0*0.3;
		
		slope  = 413.2;  // +/- 0.4
		DeltaLambda = 0.4;
	}
	double E = 1000.0*( (double)(qdc) - offset) / slope;
	double x0 = 1000.0*offset;
	
	double DeltaE2 = (-Deltax0/slope)*(-Deltax0/slope) + (-E/slope*DeltaLambda)*(-E/slope*DeltaLambda);
	DeltaE = sqrt(DeltaE2);
	//
	return DeltaE;
}
*/

Double_t get_upper_E_Rb(double ch)  // preliminary calibrations from James  (what units?)
{
	double E;
	
	double ch0=  -7519.72;
	double A  =  10867.51; 
	double E0 = -12641.56;
	double w  =  13591.53;
	
	E = E0 + w*log( (ch-ch0)/(A + ch -ch0) );
	return E;
}
Double_t get_lower_E_Rb(double ch)  // preliminary calibrations from James  (what units?)
{
	double E;
	
	double ch0= -17700.94;
	double A  =  20616.53; 
	double E0 = -20920.88;
	double w  =  15194.39;
	
	E = E0 + w*log( (ch-ch0)/(A + ch -ch0) );
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
}

Double_t getE_withresolution(double E, double lambda)
{
//	int verbose=1;
	
	double better_E =-1.0;
	double E_res = sqrt(lambda*E);
	while(better_E<0)
	{
	//	double this_random = 0;
		better_E = gRandom->Gaus(E, E_res);
//		if(verbose) {cout << "this random number:  " << better_E << endl; }
	}
	return better_E;
}

Double_t getres_withresolution(double E, double lambda)  
// call this function *after* you've applied the resolution to "E"
{
	double E_res = sqrt(lambda*E);
	return E_res;
}

// Cycle Counter:
double time_to_polarize = 100.0;   // microsec.
long rollover_count = 1073741824;  // acmot/op seems to loop at ~1038e6.  "Usually."
int ac_cyclelength = 97260;
//int dc_slop = int(12000/50); // events read out 12 microseconds?  I think that's right...
class my_prev_event
{
// kludge to copy timestamps.
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

bool check_pol_orig(int acmottime, double op_delay)  // what MJA expected.  but actually, use the thing Ben did below instead.
{
	int ac_cycle_mus = 97260*50/1000; // in microseconds.  4863 mus.
	int actime = 2956;
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

bool check_pol_benversion(int acmottime, double op_delay)  // this version of check_pol tries to follow Ben's timing cuts convention...
{
	int ac_cycle_mus = 97260*50/1000; // in microseconds.  4863 mus.
	int actime = 2956;  // 4863 - 2956 = 1907
//	double time_to_polarize = 100.0;
	double time_anomaly = 32.0;  // it's, what, 200?  33?  ...32.
	double timing_jitter = 5.0;  // 
	
	bool polarized = false;
	if( ( double(acmottime)/1000.0 > actime + time_to_polarize + op_delay + time_anomaly
			&& double(acmottime)/1000.0 < /* ac_cycle_mus + time_anomaly - timing_jitter*/ actime + 1900 ) 
		/* || double(acmottime)/1000.0 < time_anomaly-timing_jitter */ )
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
//	t = 1,
//	b = 0
	t = 0,
	b = 1
};
enum bb1_axis
{
	x = 0,
	y = 1
};

int threshold_index = 0;             // Usually use 0, to get a SNR of 0.25.

// regular cuts:
double sigma_cut = 3.0;              // Usually use 3.
double bb1_energy_threshold = 60.0;  // Ben uses 60 keV.  I usually use 60 keV too.

//if(loosecut_for_soe)
//double sigma_cut = 1.5;
//double bb1_energy_threshold = 40.0;

double get_r(double x, double y)
{
	double r2 = pow(x, 2) + pow(y, 2);
	return sqrt(r2);
}

//
//double etof_offset_le[] = {84.41+122.6, 86.61+120.4};  // not good values atm.  also, this was set up using the other detector numbering convention...
//double etof_offset_le_t = 86.61+120.4;
//double etof_offset_le_b = 84.41+122.6;


/*
double adjust_tof_t(double raw_tof_in, double Ebeta, TF1* fitfunc, int run)  
// what units for Ebeta? ...whatever gets saved to the new friendtuple.
// raw_tof_in is just electron_time - scint_time, in TDC units.
{
	double etof_offset_le_t = 86.61+120.4;
	double extra_offset_t_byrun = 0;
	//
	raw_tof_in*=v1192_to_ns;
	raw_tof_in+=etof_offset_le_t;
	if(run>420)  // after set A.
	{
		if (run<=445)  // B
		{
			extra_offset_t_byrun = 3.24675e-02;
		}
		else if(run<=477) // C
		{
			extra_offset_t_byrun = -5.03717e-02;
		}
		else if(run<=513) // D
		{
			extra_offset_t_byrun = -5.81678e-02;
		}
	}
	raw_tof_in+=extra_offset_t_byrun;
	
//	e_tof_in = ( electron_time - scint_time_t->at(j) )*v1192_to_ns + etof_offset_le_t;
//	the_tof  = adjust_tof(tmp_e_tof, upper_E, f_t_le);

	double adjustment;
	if(!fitfunc) { adjustment = 9.8; }
	else         { adjustment = fitfunc -> Eval(Ebeta); }
	double tof_out = raw_tof_in - adjustment;
	return tof_out;
}
double adjust_tof_b(double raw_tof_in, double Ebeta, TF1* fitfunc, int run)
{
	double etof_offset_le_b = 84.41+122.6;
	double extra_offset_b_byrun = 0;
	//
	raw_tof_in*=v1192_to_ns;
	raw_tof_in+=etof_offset_le_b;
	if(run>420) // after set A.
	{
		if (run<=445)  // B
		{
		extra_offset_b_byrun = -4.73204e-03;
		}
		else if(run<=477) // C
		{
			extra_offset_b_byrun = -3.07881e-02;
		}
		else if(run<=513) // D
		{
			extra_offset_b_byrun = -4.44450e-02;
		}
	}
	raw_tof_in+=extra_offset_b_byrun;

	double adjustment;
	if(!fitfunc) { adjustment = 9.8; }
	else         { adjustment = fitfunc -> Eval(Ebeta); }
	double tof_out = raw_tof_in - adjustment;
	return tof_out;
}
*/

TF1* makefunc_top_adjall()
{
	double xmin = 0.0;
	double xmax = 5300.;
	
	TF1* f_quartic = new TF1("f_quartic_t", "[0]*(1-exp([1]*x*1.0e-3)) + [2] + [3]*x*1.0e-3 + [4]*x*x*1.0e-6 + [5]*x*x*x*1.0e-9 + [6]*x*x*x*x*1.0e-12", 0, 6000.0);
	f_quartic -> SetName("Top LE Quartic");
	f_quartic -> SetLineColor(kBlue);
	f_quartic -> SetRange(xmin, xmax);
	f_quartic -> SetParName(0, "A");
	f_quartic -> SetParName(1, "B");
	f_quartic -> SetParName(2, "C");
	f_quartic -> SetParName(3, "D");
	f_quartic -> SetParName(4, "E");
	f_quartic -> SetParName(5, "F");
	f_quartic -> SetParName(6, "G");
	
	f_quartic -> SetParameters(3.35317, -6.60308, 5.48190, 9.26080e-01, -3.71163e-01, 7.00819e-02, -5.03369e-03);  // fitparams for "all".
	f_quartic -> FixParameter(f_quartic->GetParNumber("A"), f_quartic->GetParameter("A") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("B"), f_quartic->GetParameter("B") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("C"), f_quartic->GetParameter("C") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("D"), f_quartic->GetParameter("D") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("E"), f_quartic->GetParameter("E") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("F"), f_quartic->GetParameter("F") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("G"), f_quartic->GetParameter("G") );
	
	return f_quartic;
}
TF1* makefunc_bottom_adjall()
{
	double xmin = 0.0;
	double xmax = 5300.;
	
	TF1* f_quartic = new TF1("f_quartic_b", "[0]*(1-exp([1]*x*1.0e-3)) + [2] + [3]*x*1.0e-3 + [4]*x*x*1.0e-6 + [5]*x*x*x*1.0e-9 + [6]*x*x*x*x*1.0e-12", 0, 6000.0);
	f_quartic -> SetName("Bottom LE Quartic");
	f_quartic -> SetLineColor(kBlue);
	f_quartic -> SetRange(xmin, xmax);
	f_quartic -> SetParName(0, "A");
	f_quartic -> SetParName(1, "B");
	f_quartic -> SetParName(2, "C");
	f_quartic -> SetParName(3, "D");
	f_quartic -> SetParName(4, "E");
	f_quartic -> SetParName(5, "F");
	f_quartic -> SetParName(6, "G");
	
	f_quartic -> SetParameters(3.79812, -6.19521, 5.15725, 1.12594,  -4.57808e-01, 8.88178e-02, -6.46651e-03);  // fitparams for "all".
	f_quartic -> FixParameter(f_quartic->GetParNumber("A"), f_quartic->GetParameter("A") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("B"), f_quartic->GetParameter("B") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("C"), f_quartic->GetParameter("C") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("D"), f_quartic->GetParameter("D") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("E"), f_quartic->GetParameter("E") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("F"), f_quartic->GetParameter("F") );
	f_quartic -> FixParameter(f_quartic->GetParNumber("G"), f_quartic->GetParameter("G") );
	
	return f_quartic;
}
//
TF1 * makefunc_top_adj_B()
{
	double xmin = 0.0;
	double xmax = 5300.;
	
	TF1* f_pol2 = new TF1("f_pol2B_t", "[b] + [m]*x", 0, 6000.0);
	f_pol2 -> SetName("SetB - Top LinearAdjust");
	f_pol2 -> SetLineColor(kRed);
	f_pol2 -> SetRange(xmin, xmax);
	f_pol2 -> FixParameter(f_pol2->GetParNumber("b"), 0.0113079 );
	f_pol2 -> FixParameter(f_pol2->GetParNumber("m"), 7.53775e-06 );
	
	return f_pol2;
}
TF1 * makefunc_bottom_adj_B()
{
	double xmin = 0.0;
	double xmax = 5300.;
	
	TF1* f_pol2 = new TF1("f_pol2B_b", "[b] + [m]*x", 0, 6000.0);
	f_pol2 -> SetName("SetB - Bottom LinearAdjust");
	f_pol2 -> SetLineColor(kRed);
	f_pol2 -> SetRange(xmin, xmax);
	f_pol2 -> FixParameter(f_pol2->GetParNumber("b"),  0.0104313 );
	f_pol2 -> FixParameter(f_pol2->GetParNumber("m"), -2.91273e-06 );
	
	return f_pol2;
}
TF1 * makefunc_top_adj_C()
{
	double xmin = 0.0;
	double xmax = 5300.;
	
	TF1* f_pol2 = new TF1("f_pol2C_t", "[b] + [m]*x", 0, 6000.0);
	f_pol2 -> SetName("SetC - Top LinearAdjust");
	f_pol2 -> SetLineColor(kRed);
	f_pol2 -> SetRange(xmin, xmax);
	f_pol2 -> FixParameter(f_pol2->GetParNumber("b"), -0.076612 );
	f_pol2 -> FixParameter(f_pol2->GetParNumber("m"),  6.99425e-06 );
	
	return f_pol2;
}
TF1 * makefunc_bottom_adj_C()
{
	double xmin = 0.0;
	double xmax = 5300.;
	
	TF1* f_pol2 = new TF1("f_pol2C_b", "[b] + [m]*x", 0, 6000.0);
	f_pol2 -> SetName("SetC - Bottom LinearAdjust");
	f_pol2 -> SetLineColor(kRed);
	f_pol2 -> SetRange(xmin, xmax);
	f_pol2 -> FixParameter(f_pol2->GetParNumber("b"), -0.0226554 );
	f_pol2 -> FixParameter(f_pol2->GetParNumber("m"), -5.59529e-06 );
	
	return f_pol2;
}
TF1 * makefunc_top_adj_D()
{
	double xmin = 0.0;
	double xmax = 5300.;
	
	TF1* f_pol2 = new TF1("f_pol2D_t", "[b] + [m]*x", 0, 6000.0);
	f_pol2 -> SetName("SetD - Top LinearAdjust");
	f_pol2 -> SetLineColor(kRed);
	f_pol2 -> SetRange(xmin, xmax);
	f_pol2 -> FixParameter(f_pol2->GetParNumber("b"), -0.0756345 );
	f_pol2 -> FixParameter(f_pol2->GetParNumber("m"),  3.3005e-06 );
	
	return f_pol2;
}
TF1 * makefunc_bottom_adj_D()
{
	double xmin = 0.0;
	double xmax = 5300.;
	
	TF1* f_pol2 = new TF1("f_pol2D_b", "[b] + [m]*x", 0, 6000.0);
	f_pol2 -> SetName("SetD - Bottom LinearAdjust");
	f_pol2 -> SetLineColor(kRed);
	f_pol2 -> SetRange(xmin, xmax);
	f_pol2 -> FixParameter(f_pol2->GetParNumber("b"), -0.0511189 );
	f_pol2 -> FixParameter(f_pol2->GetParNumber("m"),  4.76431e-06 );
	
	return f_pol2;
}
//
double adjust_tof_t(double tof_in_ns, double Ebeta, TF1 * f_quartic_t)  
{
//	TF1* f_quartic_t = makefunc_top_adjall();
	double etof_offset_le_t = 86.61+120.4;
	tof_in_ns+=etof_offset_le_t;
	
	double adjustment = f_quartic_t -> Eval(Ebeta);
	double tof_out = tof_in_ns - adjustment;
	return tof_out;
}
double adjust_tof_b(double tof_in_ns, double Ebeta, TF1* f_quartic_b)
{
//	TF1 * f_quartic_b = makefunc_bottom_adjall();
	double etof_offset_le_b = 84.41+122.6;
	tof_in_ns+=etof_offset_le_b;

	double adjustment = f_quartic_b -> Eval(Ebeta);
	double tof_out = tof_in_ns - adjustment;
	return tof_out;
}
double second_adjust_t(double tof_in_ns, double Ebeta, TF1 * f)
{
	double adjustment = f -> Eval(Ebeta);
	double tof_out = tof_in_ns - adjustment;
	//
	return tof_out;
}
double second_adjust_b(double tof_in_ns, double Ebeta, TF1 * f)
{
	double adjustment = f -> Eval(Ebeta);
	double tof_out = tof_in_ns - adjustment;
	//
	return tof_out;
}

//
int main(int argc, char *argv[]) 
{
	setup_location();
	
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
	cout << "For BB1s, we use a " << sigma_cut << " sigma energy agreement cut." << endl;   // 
	cout << "For BB1s, we use SNR threshold \'index\' " << threshold_index << "." << endl;  // indices:  SNR=0.25@i=0, SNR=0.50@i=1, SNR=0.75@i=2, SNR=1.0@i=3, SNR=2.0@i=4.  Supposedly.
	cout << "For BB1s, we use an energy threshold of " << bb1_energy_threshold << " keV." << endl;
	
	if(is_g4)
	{
		cout << "We are using simulated data." << endl;
	}
	else
	{
		cout << "We are using experimental data." << endl;
	}
	
	string fname;
	string friend_fname;
	double this_opdelay;
	string matched_runset = "";
	double lambda_g4_res_t = 0.0;
	double lambda_g4_res_b = 0.0;
	TTree * MetaTree;
	gRandom = new TRandom3();
	
	TF1 * f_quartic_t = makefunc_top_adjall();
	TF1 * f_quartic_b = makefunc_bottom_adjall();
	TF1 * f_adjset_t = new TF1();
	TF1 * f_adjset_b = new TF1();
	if(!is_g4)
	{	
		if(do_rubidium)
		{
			// for rubidium, it's neither blinded nor polarized...
			cout << "Running retuple on Rubidium data." << endl;
			is_blinded = false;
			this_opdelay = 0.0;  //  this really shouldn't ever get used..
			fname  = make_rootfilename(rb_path+"output0", runno);
			cout << "Using file:  " << fname << endl;
			//
			friend_fname = make_rootfilename(rbfpath+"friend0",runno);
		}
		else
		{
			// polarization classification overhead:
			set_of_runs runs;  // Do NOT use a '*'.  It breaks.  Dunno why.
			this_opdelay = runs.OP_Delay[runno];
			
			if( (runs.runset_letter[runno]).compare(string("B"))==0  )
			{
				f_adjset_t = makefunc_top_adj_B();
				f_adjset_b = makefunc_bottom_adj_B();
				cout << "Using a run from Data Set B." << endl;
			}
			else if( (runs.runset_letter[runno]).compare(string("C"))==0  )
			{
				f_adjset_t = makefunc_top_adj_C();
				f_adjset_b = makefunc_bottom_adj_C();
				cout << "Using a run from Data Set C." << endl;
			}
			else if( (runs.runset_letter[runno]).compare(string("D"))==0 )
			{
				f_adjset_t = makefunc_top_adj_D();
				f_adjset_b = makefunc_bottom_adj_D();
				cout << "Using a run from Data Set D." << endl;
			}
			else if( (runs.runset_letter[runno]).compare(string("A"))==0  )
			{
				cout << "***" << endl;
				cout << "WARNING!  You are attempting to retuple a run from Set A, using the scintillator walk correction from Set B!!" << endl;
				cout << "          This will inevitably turn out poorly, but it won't segfault." << endl;
				cout << "***" << endl;
				f_adjset_t = makefunc_top_adj_B();
				f_adjset_b = makefunc_bottom_adj_B();
			}
			else 
			{
				cout << "That's bad." << endl;
				cout << "(runs.runset_letter[runno]) = " << (runs.runset_letter[runno]) << endl;
				return 0;
			}
			//
			if(is_blinded)
			{
				cout << "Running retuple on blinded datasets." << endl;
				if( runs.good_recoil[runno]==true )
				{
					cout << "Run " << runno << " is a recoil run." << endl;
					fname  = make_rootfilename(br_path+"output00", runno, "_blinded");
					cout << "Using file:  " << fname << endl;
				}
				else if(runs.good_electron[runno]==true)
				{
					cout << "Run " << runno << " is an electron run." << endl;
					fname  = make_rootfilename(be_path+"output00", runno, "_blinded");
					cout << "Using file:  " << fname << endl;
				}
				else
				{
					cout << "BAD.  Check run number." << endl;
					return 0;
				}
				friend_fname = make_rootfilename(bf_path+"friend00",runno, "_blinded");
			}
			else
			{
				if( runs.good_recoil[runno]==true )
				{
					cout << "Run " << runno << " is a recoil run." << endl;
					fname  = make_rootfilename(ur_path+"output00", runno);
					cout << "Using file:  " << fname << endl;
				}
				else if(runs.good_electron[runno]==true)
				{
					cout << "Run " << runno << " is an electron run." << endl;
					fname  = make_rootfilename(ue_path+"output00", runno);
					cout << "Using file:  " << fname << endl;
				}
				else
				{
					cout << "BAD.  Check run number." << endl;
					return 0;
				}
				friend_fname = make_rootfilename(uf_path+"friend00",runno);
			}
		}
	}
	else // it's g4
	{
		cout << "Original Rand. Seed:  " << endl;
		cout << gRandom->GetSeed() << endl;
		gRandom->SetSeed(0);  // sets seed to something something machine time.
		cout << "New Rand. Seed:  " << endl;
		cout << gRandom->GetSeed() << endl;
		
		
		is_blinded = false; 
		this_opdelay = 0.0;  // bc it's g4.
		// FIX THESE.
		if(use_g4_metadata)
		{
			cout << "Looking at the metadata..." << endl;
			MetaTree = load_metadata_tree(metadata_name);
			fname = get_simfilename(MetaTree, runno);
			if(fname==string(""))
			{
				cout << "Exiting..." << endl;
				return 0;
			}
			
			// also, check what runset it's supposed to match.
			matched_runset = get_matched_runletter(MetaTree, runno);  //
			if( matched_runset==string("EA") || matched_runset==string("EB") || matched_runset==string("RA") || matched_runset==string("RB") )
			{
				lambda_g4_res_t = 1.55;  // +/- 0.09
				lambda_g4_res_b = 1.28;  // +/- 0.08
			}
			else if( matched_runset==string("EC") || matched_runset==string("ED") || matched_runset==string("RC") || matched_runset==string("RD") || matched_runset==string("RE") )
			{
				lambda_g4_res_t = 1.42;  // +/- 0.08
				lambda_g4_res_b = 1.32;  // +/- 0.08
			}
			else
			{
				cout << "************************************************************************" << endl;
				cout << "WARNING!!  Could not find matched runset:  " << matched_runset << endl;
				cout << "Applying the late runsets' resolution for the scintillators anyway, " << endl;
				cout << "but it's not *really* the correct thing to do..." << endl;
				cout << "************************************************************************" << endl;
				cout << endl;
			//	cout << "Scintillator resolution will not be applied." << endl;
			//	apply_scint_res_on_g4 = false;
				lambda_g4_res_t = 1.42;  // +/- 0.08
				lambda_g4_res_b = 1.32;  // +/- 0.08
			}
		}
		else
		{
			fname  = make_rootfilename(g4_path+"output_", runno);
		}
		friend_fname = make_rootfilename(g4f_path+"friend_", runno);
	}
	cout << "fname = " << fname << endl;
	cout << "friend_fname = " << friend_fname << endl;
	
	// Original Tree:
	TFile *f = new TFile(fname.c_str(), "READ");
	if(!(f->IsOpen()))
	{
		cout << "File can't be opened." << endl;
		return 0;
	}
	TTree *tree = new TTree;  // Fix this.  This needs to be a TChain now?  ... No, no it doesn't.
	tree = (TTree*)f->Get("ntuple");

	TFile *friendfile = new TFile(friend_fname.c_str(), "RECREATE");
	TTree *friend_tree = new TTree("friendtuple", "friendtuple");
	cout << "Loaded up the ntuple and created the new friendtuple." << endl;
	
	// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	
	UInt_t runnumber = 0;
	TBranch *runbranch = friend_tree -> Branch("RUN", &runnumber);
	
	
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

	if(!is_g4)
	{
		tree -> SetBranchAddress("QDC_UpperPMT", &upper_qdc_int);
		tree -> SetBranchAddress("QDC_LowerPMT", &lower_qdc_int);
	}
	else if(is_g4)
	{
		tree -> SetBranchAddress("QDC_UpperPMT", &upper_qdc_d);
		tree -> SetBranchAddress("QDC_LowerPMT", &lower_qdc_d);
	}
	
	if(!is_g4)
	{
		tree -> SetBranchAddress("Unix_time",         &unix_time);
		tree -> SetBranchAddress("TNIM_ACMOT_ON",     &acmot_last);
		tree -> SetBranchAddress("TNIM_DCMOT_OFF",    &dcmot_last);
		tree -> SetBranchAddress("TNIM_ACMOT_ON_all", &acmot_all);
		tree -> SetBranchAddress("TNIM_DCMOT_OFF_all",&dcmot_all);
		tree -> SetBranchAddress("TNIM_TIMESTAMP",    &eventtime);  // 
	}
	int acmot_count;
	int dcmot_count;
	// Some Friend Tree Stuff:
	Bool_t all_okay       = kTRUE;
	Bool_t is_polarized   = kFALSE;
	Bool_t is_unpolarized = kFALSE;
	Bool_t is_ac          = kFALSE;
	if(do_rubidium) 
	{ 
		is_unpolarized = kTRUE; 
	}
	TBranch *all_okay_b       = friend_tree -> Branch("all_okay", &all_okay);  
	TBranch *is_polarized_b   = friend_tree -> Branch("is_polarized", &is_polarized);  
	TBranch *is_unpolarized_b = friend_tree -> Branch("is_unpolarized", &is_unpolarized);  
	TBranch *is_ac_b          = friend_tree -> Branch("is_ac", &is_ac);  
	int cyclecount = 50;
	TBranch * cyclecount_branch = friend_tree -> Branch("AC_CycleCount", &cyclecount);  
	
	Double_t upper_E;
	Double_t lower_E;
	Double_t upper_E_res;
	Double_t lower_E_res;
	TBranch *upper_e_b          = friend_tree -> Branch("upper_scint_E", &upper_E);
	TBranch *lower_e_b          = friend_tree -> Branch("lower_scint_E", &lower_E);
	TBranch *upper_e_res_branch = friend_tree -> Branch("upper_scint_E_res", &upper_E_res); // detector resolution
	TBranch *lower_e_res_branch = friend_tree -> Branch("lower_scint_E_res", &lower_E_res); // 
	//
	Double_t upper_DeltaE;
	Double_t lower_DeltaE;
	TBranch *upper_deltaE_branch= friend_tree -> Branch("upper_scint_DeltaE", &upper_DeltaE); // how much do we change the energy if we change calibration by "one sigma"?
	TBranch *lower_deltaE_branch= friend_tree -> Branch("lower_scint_DeltaE", &lower_DeltaE); // 
	
	//
	UInt_t led_count = 0;
	UInt_t photodiode_count = 0;
	vector<double> *tdc_photodiode = 0;
	vector<double> *tdc_pulser_led = 0;
	if(!is_g4) // 37K or Rb, run through the new analyzer.
	{
		if(!is_old)
		{
			tree -> SetBranchAddress("TDC_PULSER_LED_LE_Count",  &led_count);
			tree -> SetBranchAddress("TDC_PHOTO_DIODE_LE_Count", &photodiode_count);
		}
		else if(is_old)
		{
			tree -> SetBranchAddress("TDC_PULSER_LED_Count",  &led_count);
			tree -> SetBranchAddress("TDC_PHOTO_DIODE_Count", &photodiode_count);
		}
	}
	else if(is_g4)
	{
		// these are defined above, but only need to create new branches for g4 runs.  it's to prevent segfaults later.
		TBranch *led_count_branch        = friend_tree -> Branch("TDC_PULSER_LED_LE_Count",  &led_count);
		TBranch *photodiode_count_branch = friend_tree -> Branch("TDC_PHOTO_DIODE_LE_Count", &photodiode_count);
		
		TBranch *photodiode_vec_branch   = friend_tree -> Branch("TDC_PHOTO_DIODE_LE",       &tdc_photodiode);
		TBranch *led_vec_branch          = friend_tree -> Branch("TDC_PULSER_LED_LE",        &tdc_pulser_led);
	}
	//
	vector<double> * scint_time_t = 0;  
	vector<double> * scint_time_b = 0;  
	if(is_g4 || is_old)
	{
		tree -> SetBranchAddress("TDC_SCINT_TOP",    &scint_time_t);  
		tree -> SetBranchAddress("TDC_SCINT_BOTTOM", &scint_time_b);  
	}
	else
	{
		tree -> SetBranchAddress("TDC_SCINT_TOP_LE",    &scint_time_t);  
		tree -> SetBranchAddress("TDC_SCINT_BOTTOM_LE", &scint_time_b);  
	}
	
	
	// --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // 
	// rmcp delay line stuff:

	vector<double> *ion_events = 0;
	vector<double> *electron_events = 0;
	if(is_g4 || is_old)
	{
		tree -> SetBranchAddress("TDC_ION_MCP", &ion_events);
		tree -> SetBranchAddress("TDC_ELECTRON_MCP", &electron_events);
	}
	else
	{
		tree -> SetBranchAddress("TDC_ION_MCP_LE", &ion_events);
		tree -> SetBranchAddress("TDC_ELECTRON_MCP_LE", &electron_events);
	}
	int ion_count = 0;
	int electron_count = 0;
	
	vector<double> *dl_x_pos = 0;
	vector<double> *dl_z_pos = 0;
	TBranch *x_branch = friend_tree -> Branch("dl_x_pos", &dl_x_pos);
	TBranch *z_branch = friend_tree -> Branch("dl_z_pos", &dl_z_pos);
	dl_x_pos -> clear();
	dl_z_pos -> clear();
	
	vector<double> *x1_dla = 0;
	vector<double> *x2_dla = 0;
	vector<double> *z1_dla = 0;
	vector<double> *z2_dla = 0;
	vector<double> *prev_dlx = 0;
	vector<double> *prev_dlz = 0;
	if(!is_g4)
	{
		if(is_old)
		{
			tree -> SetBranchAddress("TDC_DL_X1",&x1_dla);
			tree -> SetBranchAddress("TDC_DL_X2",&x2_dla);
			tree -> SetBranchAddress("TDC_DL_Z1",&z1_dla);
			tree -> SetBranchAddress("TDC_DL_Z2",&z2_dla);
		}
		else if( !is_old )
		{
			tree -> SetBranchAddress("TDC_DL_X1_LE",&x1_dla);
			tree -> SetBranchAddress("TDC_DL_X2_LE",&x2_dla);
			tree -> SetBranchAddress("TDC_DL_Z1_LE",&z1_dla);
			tree -> SetBranchAddress("TDC_DL_Z2_LE",&z2_dla);
		}
	}
	else if(is_g4)
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
	
	// ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // 
	// Correct for walk: 
	vector<double> * scint_t_walk = 0;
	vector<double> * scint_b_walk = 0;
	TBranch *scint_t_walk_branch = friend_tree -> Branch("tdc_scint_t_corrected", &scint_t_walk);
	TBranch *scint_b_walk_branch = friend_tree -> Branch("tdc_scint_b_corrected", &scint_b_walk);
	scint_t_walk -> clear();
	scint_b_walk -> clear();
		
	// ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // 
	// BB1s:  
	BB1Detector stripdetector[2][2];
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
			// below:  sets in stone the t/b, x/y convention, as a point of calibration.
			stripdetector[detector][axis].det = Detector(detector);  // t or b.
			stripdetector[detector][axis].pl = Plane(axis);          // x or y.
		}
	}
	tree -> SetBranchAddress("BB1_AMPLITUDE_UX", &strip_E[t][x]);
	tree -> SetBranchAddress("BB1_AMPLITUDE_UY", &strip_E[t][y]);
	tree -> SetBranchAddress("BB1_AMPLITUDE_LX", &strip_E[b][x]);
	tree -> SetBranchAddress("BB1_AMPLITUDE_LY", &strip_E[b][y]);
	
	if(is_g4)
	{
		// set noise file?  and load up the noise histograms.
		// path?  same detector numbering?
		stripdetector[t][x].SetupNoiseFromFile(bb1_prefix+"bb1_noise_UX.root");
		stripdetector[t][y].SetupNoiseFromFile(bb1_prefix+"bb1_noise_UY.root");
		stripdetector[b][x].SetupNoiseFromFile(bb1_prefix+"bb1_noise_LX.root");
		stripdetector[b][y].SetupNoiseFromFile(bb1_prefix+"bb1_noise_LY.root");
	}
	vector<double> adc_with_res(40, 0.0);
	
	
	if(!is_g4)
	{
		tree -> SetBranchAddress("BB1_UX_PEAKTIME", &strip_T[t][x]);
		tree -> SetBranchAddress("BB1_UY_PEAKTIME", &strip_T[t][y]);
		tree -> SetBranchAddress("BB1_LX_PEAKTIME", &strip_T[b][x]);
		tree -> SetBranchAddress("BB1_LY_PEAKTIME", &strip_T[b][y]);
	}
	else if(is_g4)
	{
		TBranch * bb1_ux_time = friend_tree -> Branch("BB1_UX_PEAKTIME", &strip_T[t][x]);
		TBranch * bb1_uy_time = friend_tree -> Branch("BB1_UY_PEAKTIME", &strip_T[t][y]);
		TBranch * bb1_lx_time = friend_tree -> Branch("BB1_LX_PEAKTIME", &strip_T[b][x]);
		TBranch * bb1_ly_time = friend_tree -> Branch("BB1_LY_PEAKTIME", &strip_T[b][y]);
	}
	
	// HEX 75:
	Hex75 * the_hex = new Hex75();
	
//	string xName = "HEX75_XPos_LE";
//	string zName = "HEX75_ZPos_LE";
//	string cName = "HEX75_PosCode_LE";
//	vector<double> xPos;
//	vector<double> zPos;
//	vector<int> rcode;
//	rcode = the_hex -> GetXZ(xPos, zPos, *u1, *u2, *v1, *v2, *w1, *w2, *electron_events, false);
	
	vector<double> xPos;// = 0;
	vector<double> zPos;// = 0;
	vector<int>    rcode;//= 0;
	TBranch *xb = friend_tree -> Branch("HEX75_XPos_LE",    &xPos);
	TBranch *zb = friend_tree -> Branch("HEX75_ZPos_LE",    &zPos);
	TBranch *cb = friend_tree -> Branch("HEX75_PosCode_LE", &rcode);
	xPos.clear();
	zPos.clear();
	rcode.clear();
	
	// HEX:
	vector<double> *u1 = 0;
	vector<double> *u2 = 0;
	vector<double> *v1 = 0;
	vector<double> *v2 = 0;
	vector<double> *w1 = 0;
	vector<double> *w2 = 0;
	if(!is_g4)
	{
		tree -> SetBranchAddress("TDC_HEX75_U1_LE", &u1);
		tree -> SetBranchAddress("TDC_HEX75_U2_LE", &u2);
		tree -> SetBranchAddress("TDC_HEX75_V1_LE", &v1);
		tree -> SetBranchAddress("TDC_HEX75_V2_LE", &v2);
		tree -> SetBranchAddress("TDC_HEX75_W1_LE", &w1);
		tree -> SetBranchAddress("TDC_HEX75_W2_LE", &w2);
	}
	//
	
	
	// BB1s:  
	vector<double> * bb1_t_x = 0;
	vector<double> * bb1_t_y = 0;
	vector<double> * bb1_t_E = 0;
	vector<double> * bb1_t_r = 0;
	TBranch *bb1_t_x_branch = friend_tree -> Branch("bb1_top_x", &bb1_t_x);
	TBranch *bb1_t_y_branch = friend_tree -> Branch("bb1_top_y", &bb1_t_y);
	TBranch *bb1_t_E_branch = friend_tree -> Branch("bb1_top_E", &bb1_t_E);
	TBranch *bb1_t_r_branch = friend_tree -> Branch("bb1_top_r", &bb1_t_r);
	bb1_t_x -> clear();
	bb1_t_y -> clear();
	bb1_t_E -> clear();
	bb1_t_r -> clear();

	vector<double> * bb1_b_x = 0;
	vector<double> * bb1_b_y = 0;
	vector<double> * bb1_b_E = 0;
	vector<double> * bb1_b_r = 0;
	TBranch *bb1_b_x_branch = friend_tree -> Branch("bb1_bottom_x", &bb1_b_x);
	TBranch *bb1_b_y_branch = friend_tree -> Branch("bb1_bottom_y", &bb1_b_y);
	TBranch *bb1_b_E_branch = friend_tree -> Branch("bb1_bottom_E", &bb1_b_E);
	TBranch *bb1_b_r_branch = friend_tree -> Branch("bb1_bottom_r", &bb1_b_r);
	bb1_b_x -> clear();
	bb1_b_y -> clear();
	bb1_b_E -> clear();
	bb1_b_r -> clear();
	
	BB1Hit    bb1_hit[2]    = {BB1Hit(), BB1Hit()};        // 2 detectors.
	BB1Hit    bb1_sechit[2] = {BB1Hit(), BB1Hit()};        // 2 detectors.
	BB1Result bb1_result[2] = {BB1Result(), BB1Result()};  // 2 detectors.
	//
	
	// Backscatter Event Classification:
	// is_type1_t:  
	// 	beta goes up through top SD into top PMT, 
	// 	scatters, back out through top SD, 
	// 	into lower SD, into lower PMT.
	// In the data, that looks like:
	//		1+ hit in top PMT, 2+ hits in top SD, AND
	//		1+ hit in bottom PMT, exactly 1 hit in bottom SD.
	//		0 LED hits, 0 photodiode hits.
	Bool_t is_type1_t = kFALSE;
	Bool_t is_type1_b = kFALSE;
	TBranch *is_type1_t_branch = friend_tree -> Branch("is_type1_t", &is_type1_t);  
	TBranch *is_type1_b_branch = friend_tree -> Branch("is_type1_b", &is_type1_b);  

	// is_type2_t:
	// 	beta goes up to top SD, scatters, 
	// 	goes down into the bottom SD and bottom PMT.
	// In the data, that looks like this:
	//		0 hits in top PMT, exactly 1 hit in top SD, AND
	//		1+ hit in bottom PMT, exactly 1 hit in bottom SD.
	//		0 LED hits, 0 photodiode hits.
	Bool_t is_type2_t = kFALSE;
	Bool_t is_type2_b = kFALSE;
	TBranch *is_type2_t_branch = friend_tree -> Branch("is_type2_t", &is_type2_t);  
	TBranch *is_type2_b_branch = friend_tree -> Branch("is_type2_b", &is_type2_b);  

	// is_type3_t:
	// 	beta goes up through the top SD, 
	// 	into the top PMT, scatters back down 
	// 	through the top SD again, then down 
	// 	into the bottom SD, where it's absorbed.
	// In the data, that looks like this:  
	//		1+ hit in top PMT, 2+ hits in top SD, 
	//		0 hits in bottom PMT, exactly 1 hit in bottom SD.
	//		0 LED hits, 0 photodiode hits.
	Bool_t is_type3_t = kFALSE;
	Bool_t is_type3_b = kFALSE;
	TBranch *is_type3_t_branch = friend_tree -> Branch("is_type3_t", &is_type3_t);  
	TBranch *is_type3_b_branch = friend_tree -> Branch("is_type3_b", &is_type3_b);  

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
	//		0 LED hits, 0 photodiode hits.
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
	//		0 LED hits, 0 photodiode hits.
	Bool_t is_normal_t = kFALSE;
	Bool_t is_normal_b = kFALSE;
	TBranch *is_normal_t_branch = friend_tree -> Branch("is_normal_t", &is_normal_t);  
	TBranch *is_normal_b_branch = friend_tree -> Branch("is_normal_b", &is_normal_b);  
	
	/*
	// is_other_t:
	// Any event that hasn't already been described where the top PMT fires.
	// Mostly, this will probably be events where something hit the top PMT 
	// but was not recorded in the strip detectors.  eg, gammas and LEDs.  
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
	*/
	
	// is_other:  
	// Any scint. event that hasn't already been described.
	// Includes events with LED and/or photodiode hits.
	Bool_t is_other = kFALSE;
	TBranch *is_other_branch = friend_tree -> Branch("is_other", &is_other);  
	
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
	
	int had_Nhits_bb1_t = 0;
	int had_Nhits_bb1_b = 0;
	TBranch * twohits_bb1_t_branch = friend_tree -> Branch("had_Nhits_bb1_t", &had_Nhits_bb1_t);
	TBranch * twohits_bb1_b_branch = friend_tree -> Branch("had_Nhits_bb1_b", &had_Nhits_bb1_b);
	
	
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
//	cout << "nentries = " << nentries << endl;
	int badint=0;
	int skipped = 0;
	//
	
	// Use Metadata from the MetaTree.
	Int_t sigma_plus = -1;
	Double_t polarization = 0.0;
	if(is_g4)
	{
		MetaTree = load_metadata_tree(metadata_name);
		int this_runno;
		MetaTree -> SetBranchAddress("Run", &this_runno);
		double this_pol;
		MetaTree -> SetBranchAddress("Polarization", &this_pol);
		double final_pol;
	
		int nmetaentries = MetaTree -> GetEntries();
		cout << "nmetaentries = " << nmetaentries << endl;
		for(int i=0; i<nmetaentries; i++)
		{
			MetaTree -> GetEntry(i);
			if(this_runno == runno)
			{
				final_pol = this_pol;
				cout << "Looking at entry i = " << i << ":  run = " << runno << endl;
				break;
			}
		}
		
		polarization = final_pol;
		if(final_pol >= 0.0) { sigma_plus = 1; }
		else                 { sigma_plus = 0; }
	
		TBranch *sigma_plus_branch = friend_tree -> Branch("TTLBit_SigmaPlus", &sigma_plus);  
		TBranch *pol_branch = friend_tree -> Branch("Polarization", &polarization);  
	}
	
	//
	cout << "nentries = " << nentries << endl;
	for(int i=0; i<nentries; i++)
	{
		if( (i % 100000) == 0) { cout<<"Reached entry "<< i << endl; }
		tree -> GetEntry(i);
		//
		
		// photodiode and led events, for g4:
		if(is_g4)
		{ // I may not even need to do all this stuff explicitly, bc it never changes.
			led_count = 0;
			photodiode_count = 0;
			tdc_photodiode -> clear();
			tdc_pulser_led -> clear();
		}
		runnumber = runno;
		
		ion_count = ion_events->size();
		electron_count = electron_events->size();
		N_hits_scint_t = scint_time_t->size();
		N_hits_scint_b = scint_time_b->size();
		
		// rMCP position stuff, scint. calibration, SOE TOF walk correction:
		if(!is_g4)
		{
			//
			if(!do_rubidium)  // 37K real data
			{
				upper_E     = get_upper_E(upper_qdc_int, runno, is_g4);
				lower_E     = get_lower_E(lower_qdc_int, runno, is_g4);
				upper_E_res = get_upper_E_res(upper_E, runno, is_g4);
				lower_E_res = get_lower_E_res(lower_E, runno, is_g4);
				//
				upper_DeltaE = get_upper_DeltaE(upper_qdc_int, runno, is_g4);
				lower_DeltaE = get_lower_DeltaE(lower_qdc_int, runno, is_g4);
				//
				all_okay = get_all_okay_for_event(unix_time, badtimesforrun, &badint, &skipped);
			
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
			else if(do_rubidium)
			{
				upper_E = get_upper_E_Rb(upper_qdc_int);
				lower_E = get_lower_E_Rb(lower_qdc_int);
				upper_E_res = 0.0;  // doesn't really work for Rb, but again, I don't care atm.
				lower_E_res = 0.0;  
				//
				upper_DeltaE = 0.0;
				lower_DeltaE = 0.0;
				
				all_okay = kTRUE;	
				
				x1_count = x1_dla->size();
				x2_count = x2_dla->size();
				z1_count = z1_dla->size();
				z2_count = z2_dla->size();
				
				nhits = min(ion_count, min(min(x1_count, x2_count), min(z1_count, z2_count)));
				for(int j=0; j<nhits; j++)  // this shit isn't going to work well for Rb data, but wev.  
				{
					coordinates = my_cals -> 
						apply_calibration((*x1_dla)[j], (*x2_dla)[j], (*z1_dla)[j], (*z2_dla)[j], 5);
					dl_x_pos -> push_back(coordinates.first);
					dl_z_pos -> push_back(coordinates.second);
				}
			}
		}
		else if(is_g4)
		{
			all_okay = kTRUE;	
			
			upper_E = get_upper_E(upper_qdc_d, runno, is_g4);
			lower_E = get_lower_E(lower_qdc_d, runno, is_g4);
			upper_E_res = get_upper_E_res(upper_E, runno, is_g4);
			lower_E_res = get_lower_E_res(lower_E, runno, is_g4);
			//
			upper_DeltaE = 0.0;
			lower_DeltaE = 0.0;

			if(is_g4 && apply_scint_res_on_g4)
			{
				// apply a scint resolution on the G4 data.
				upper_E = getE_withresolution(upper_E, lambda_g4_res_t);
				lower_E = getE_withresolution(lower_E, lambda_g4_res_b);
				upper_E_res = getres_withresolution(upper_E, lambda_g4_res_t);
				lower_E_res = getres_withresolution(lower_E, lambda_g4_res_b);
			}
			if( ion_count>0 && prev_dlx->size() >0 && prev_dlz->size() > 0)
			{
				dl_x_pos -> push_back( (*prev_dlx)[0] );
				dl_z_pos -> push_back( (*prev_dlz)[0] );
			}
		}
		
		// ok, apply the SOE TOF walk calibration.  we can re-use nhits.  
		// we already figured out upper_E and lower_E.  if it's g4 or old, just fill with zeroes.
		if(electron_count>0)
		{
			double electron_time = (*electron_events)[0];
			double e_tof_in = 0;
			double the_tof = 0;
			// Top:
			if(N_hits_scint_t>0)
			{
				nhits = N_hits_scint_t;
				for(int j=0; j<nhits; j++) 
				{
					if(!is_g4)
					{
						the_tof = adjust_tof_t( (electron_time - scint_time_t->at(j))*v1192_to_ns, upper_E, f_quartic_t);
						the_tof = second_adjust_t(the_tof, upper_E, f_adjset_t);
					}
					scint_t_walk -> push_back(the_tof);
				}
			}
			// Bottom:
			if(N_hits_scint_b>0)
			{
				nhits = N_hits_scint_b;
				for(int j=0; j<nhits; j++) 
				{
					if(!is_g4)
					{
						the_tof = adjust_tof_b( (electron_time - scint_time_b->at(j))*v1192_to_ns, lower_E, f_quartic_b);
						the_tof = second_adjust_b(the_tof, lower_E, f_adjset_b);
					}
					scint_b_walk -> push_back(the_tof);
				}
			}
		}
		
		// HEX 75 calibrations:
		if(!is_g4)
		{
			xPos.clear();
			zPos.clear();
			rcode.clear();
			rcode = the_hex -> GetXZ(xPos, zPos, *u1, *u2, *v1, *v2, *w1, *w2, *electron_events, false);
		}
		
		// Set up event types:
		is_type1_t = kFALSE;
		is_type1_b = kFALSE;
		is_type2_t = kFALSE;
		is_type2_b = kFALSE;
		is_type3_t = kFALSE;
		is_type3_b = kFALSE;
		
		is_type4_t = kFALSE;
		is_type4_b = kFALSE;
		
		is_normal_t = kFALSE;
		is_normal_b = kFALSE;
		
		is_other = kFALSE;
		
		// BB1 shizzle:
		N_hits_bb1_t = 0;
		N_hits_bb1_b = 0;
		
		had_Nhits_bb1_t = 0;
		had_Nhits_bb1_b = 0;
		
		if(!do_rubidium)  // this is broken for Rb, because the scint calibrations are still all wrong.
		{
			if(upper_E < 10.0) { N_hits_scint_t=0; }
			if(lower_E < 10.0) { N_hits_scint_b=0; }
		}
		
		if(N_hits_scint_t>0 || N_hits_scint_b>0)
		{
			// include LED and photodiode events here.
			is_other = kTRUE;
		}
		
		if( led_count==0 && photodiode_count==0 && (N_hits_scint_t>0 || N_hits_scint_b>0) )
		{
			for(int detector=0; detector <=1; detector++)
			{ // loop over top/bottom
				for (int axis = 0; axis<=1; axis++) 
				{ // loop over bb1 axes.
					if (strip_E[detector][axis] -> size() != 40) 
					{
						cout << "scint_time_t->size() = " << scint_time_t->size() << "; \tscint_time_b->size() = " << scint_time_b->size();
						cout << ";\tStrip [" << detector << "][" << axis << "] has size " << strip_E[detector][axis] -> size() << endl;
						continue;
					}
					// calculate the energy for every single fucking strip.
					if(is_g4)
					{
						adc_with_res = stripdetector[detector][axis].ApplyResolution(*strip_E[detector][axis], gRandom, doEmpirical);
						stripdetector[detector][axis].CalcEnergy(adc_with_res);  
					}
					else
					{
						stripdetector[detector][axis].CalcEnergy(*strip_E[detector][axis]);  
						// strip_E[detector][axis] is a vector<double>*.
					}
					
					
					// load up peak time for every fucking strip.  if it's real data.  
					if(!is_g4)
					{ 
						stripdetector[detector][axis].SetMaxT(*strip_T[detector][axis]); 
					}
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
				//	cout << "Got here at least though." << endl;
					if(detector == t)
					{
						had_Nhits_bb1_t=1;
					}
					if(detector == b)
					{
						had_Nhits_bb1_b=1;
					}
					if( bb1_hit[detector].energy >= bb1_energy_threshold )
					{
						if(detector == t)
						{
							bb1_t_x -> push_back( bb1_hit[detector].xpos );
							bb1_t_y -> push_back( bb1_hit[detector].ypos );
							bb1_t_E -> push_back( bb1_hit[detector].energy );
							bb1_t_r -> push_back( get_r(bb1_hit[detector].xpos, bb1_hit[detector].ypos) );
							N_hits_bb1_t++;
						}
						else if(detector == b)
						{
							bb1_b_x -> push_back( bb1_hit[detector].xpos );
							bb1_b_y -> push_back( bb1_hit[detector].ypos );
							bb1_b_E -> push_back( bb1_hit[detector].energy );
							bb1_b_r -> push_back( get_r(bb1_hit[detector].xpos, bb1_hit[detector].ypos) );
							N_hits_bb1_b++;
						}
					}
				}
				//
				if(bb1_result[detector].twoHits == true)
				{
					if(detector == t)
					{
						had_Nhits_bb1_t=2;
					}
					if(detector == b)
					{
						had_Nhits_bb1_b=2;
					}
					//
					if(bb1_sechit[detector].pass == true && bb1_hit[detector].energy >= bb1_energy_threshold )
					{
						if(detector == t)
						{
							bb1_t_x -> push_back( bb1_sechit[detector].xpos );
							bb1_t_y -> push_back( bb1_sechit[detector].ypos );
							bb1_t_E -> push_back( bb1_sechit[detector].energy );
							bb1_t_r -> push_back( get_r(bb1_sechit[detector].xpos, bb1_sechit[detector].ypos) );
							N_hits_bb1_t++;
						}
						else if(detector == b)
						{
							bb1_b_x -> push_back( bb1_sechit[detector].xpos );
							bb1_b_y -> push_back( bb1_sechit[detector].ypos );
							bb1_b_E -> push_back( bb1_sechit[detector].energy );
							bb1_b_r -> push_back( get_r(bb1_sechit[detector].xpos, bb1_sechit[detector].ypos) );
							N_hits_bb1_b++;
						}
					}
				}
			//	}
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
				is_other = kFALSE;
			}
			if( (N_hits_scint_t==0 && N_hits_scint_b>=1) && (N_hits_bb1_t==0 && N_hits_bb1_b==1) )
			{
				is_normal_b = kTRUE;
				is_other = kFALSE;
			}
		}
		
		// Is this event polarized?
		if(is_g4)
		{
			all_okay = kTRUE;  // covered.
			is_polarized = kTRUE;
			is_unpolarized = kFALSE;
			is_ac = kFALSE;
			cyclecount = 50;
		}
		else if( do_rubidium )
		{
			all_okay = kTRUE;  // because we don't have "all_okay" data for rubidium.
			is_polarized = kFALSE;
			is_unpolarized = kTRUE;
			is_ac = kFALSE;
			cyclecount = 50;  // doesn't actually have cycles, but we want this branch filled with something innocuous anyway.
		}
		else // 37K run, and not from G4.  Must figure out if the event is polarized, and what op/ac cycle it's on.
		{
			is_polarized = check_pol_orig(acmot_last, this_opdelay);
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
		bb1_t_r -> clear();
		
		bb1_b_x -> clear();
		bb1_b_y -> clear();
		bb1_b_E -> clear();
		bb1_b_r -> clear();
		
		scint_t_walk -> clear();
		scint_b_walk -> clear();
	}
	if(!is_g4)
	{
		cout << "final unix_time = " << unix_time << endl;
		cout << "Skipped " << skipped << " events in the last skipping." << endl;
	}
	
	friend_tree -> GetCurrentFile() -> Write("",TObject::kOverwrite);  
	friendfile -> cd();
	version_string -> Write();
	friend_tree -> GetCurrentFile() -> Close();
	
	cout << "Output saved to file:  " << friend_fname << endl;
	cout << "Done." << endl;
	return 0;
}