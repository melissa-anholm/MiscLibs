#ifndef INCLUDE_HISTEXTRAS
#define INCLUDE_HISTEXTRAS 1


#include <vector>
#include <iostream>  // cout

using std::vector;
using std::cout;
using std::endl;

#include <TH1D.h>
#include <TH2D.h>

#include <TColor.h>

#include "HistExtras.h"

// Examples:
// TH1D* OP_Cycle_Events_all = CreateHist(string("Electron Events - All"), string("acmottime"), int(kRed));
// TH2D* tof_v_iqcd_ac  = CreateHist2d(string("tof_vs_iqdc_ac"),  string("i_qdc"), string("cal_tof"));

double v1192_to_ns = 100.0/1024.0;


// ====================================== //
// Histogram Functions 
/*
class hist_type
{
private:
	int set_other_parameters();
public:
	std::string type;
	std::string units;
	
	int nbins;
	double xmin;
	double xmax;
	double user_xmin;
	double user_xmax;
	
	hist_type(std::string my_type);
	int failed;
};
hist_type::hist_type(std::string my_type)
{
	type = my_type;
	failed = set_other_parameters();
}
*/
int hist_type::set_other_parameters()
{
	if (type == std::string("raw_tof"))
	{
		nbins = 1000;
		xmin = -100000.5+1.0;
		xmax =  100000.5;
		units = std::string("~100ps");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("rescaled_tof"))
	{
		nbins = 1500*2;
		xmin = (-150000.5+1.0)*v1192_to_ns;
		xmax =  (150000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("rescaled_toa"))
	{
		nbins = 800*2;
		xmin = (-20000.5+1.0)*v1192_to_ns;
		xmax = (140000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("cal_tof"))
	{
		nbins =   1400;
		xmin = (-80000.5+1.0)*v1192_to_ns;
		xmax = ( 60000.5)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin = -500;
		user_xmax = 5500;
	}
	else if (type == std::string("cal_tof_zoom"))
	{
		nbins =   400;
		xmin = (-5000.5+1.0)*v1192_to_ns;
		xmax = (35000.5)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin = 800;
		user_xmax = 1800;
	}
	else if (type == std::string("fine_cal_tof"))
	{
		nbins =  14000;
		xmin = (-80000.5+1.0)*v1192_to_ns;
		xmax = ( 60000.5)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin = -500;
		user_xmax = 5500;
	}
	else if (type == std::string("fine_cal_tof_zoom"))
	{
		nbins =  6000;
		xmin = (-5000.5)*v1192_to_ns;
		xmax = (25000.5-1.0)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin =  800;
		user_xmax = 2000;
	}
	else if (type == std::string("finer_cal_tof_zoom"))
	{
		nbins =  20000;
		xmin = ( 5000.5)*v1192_to_ns;
		xmax = (25000.5-1.0)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin = 1000;
		user_xmax = 1600;
	}
	else if (type == std::string("sim_PI_535"))
	{
		nbins =  3000;
		xmin = (-500.5);
		xmax = (2500.5-1.0);
		units = std::string("Simulated Photoion TOF (ns)");
		user_xmin = 1120;
		user_xmax = 1320;
	}
	else if (type == std::string("sim_PI_415"))
	{
		nbins =  3000;
		xmin = (-500.5);
		xmax = (2500.5-1.0);
		units = std::string("Simulated Photoion TOF (ns)");
		user_xmin = 1260;
		user_xmax = 1540;
	}
	else if (type == std::string("sim_PI_395"))
	{
		nbins =  3000;
		xmin = (-500.5);
		xmax = (2500.5-1.0);
		units = std::string("Simulated Photoion TOF (ns)");
		user_xmin = 1260;  // fix this.
		user_xmax = 1540;  // fix this.
	}
	else if (type == std::string("fine_cal_tof_zprompt"))
	{
		nbins =  6000;
		xmin = (-5000.5)*v1192_to_ns;
		xmax = (25000.5-1.0)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin = -200;
		user_xmax = -50;
	}
	else if (type == std::string("cal_e_tof"))
	{
		nbins = 20000;
		xmin = (-10000.5+1.0)*v1192_to_ns;
		xmax = (10000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = 50;
		user_xmax = 150;
	}
	else if (type == std::string("cal_e_scint_tof")) // electron tof w.r.t. scint time // + offset.
	{
		nbins = 400;
		xmin = (-200.5+1.0)*v1192_to_ns;
		xmax = (200.5)*v1192_to_ns;
		units = std::string("electron - scint - offset [ns]");
		user_xmin = -4.0;
		user_xmax = 4.0;
	}
	else if (type == std::string("cal_scint_tof"))  // for eg: scint_t - scint_b.
	{
		nbins = 2000;
		xmin = (-1000.5+1.0)*v1192_to_ns;
		xmax = (1000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = -40;
		user_xmax = 40;
	}
	else if (type == std::string("cal_toa"))  // ok for: 
	{
		nbins = 12000;
		xmin = ( 20000.5+1.0)*v1192_to_ns;
		xmax = (100000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("e_qdc") )
	{
		nbins = 500;  // Note:  500 bins is as finely as (-0.5 <-> 499.5) can be binned up.
		xmin = -0.5;
		xmax = 500.5-1.0;
		units = std::string("raw e- qdc");
		user_xmin = 49.5;
		user_xmax = 150.5;
	}
	else if (type == std::string("i_qdc") )
	{
		nbins = 600;
		xmin = -0.5;
		xmax = 600.5-1.0;
		units = std::string("rMCP QDC");
		user_xmin = 49.5;
		user_xmax = 400.5;
	}
	else if (type == std::string("scint_qdc") )
	{
		nbins = 3000;
		xmin = -0.5;
		xmax = 3000.5-1.0;
		units = std::string("raw scint qdc");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("scint_cal_qdc") )
	{
		nbins = 600;
		xmin = -0.5;
		xmax = 6000.5-1.0;
		units = std::string("Scintillator Energy (keV)");
		user_xmin = xmin;
		user_xmax = 5500.0;
	}
	else if (type == std::string("Rb_Ebeta") )
	{
		nbins = 900;
		xmin = -0.5;
		xmax = 9000.5-1.0;
		units = std::string("Scintillator Energy (keV)");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("E_scint") )
	{
		nbins = 650;
		xmin = -0.5;
		xmax = 6500.5-1.0;
		units = std::string("Scintillator Energy (keV)");
		user_xmin = xmin;
		user_xmax = 5500.0;
	}
	else if (type == std::string("E_summed") )
	{
		nbins = 650;
		xmin = -0.5;
		xmax = 6500.5-1.0;
		units = std::string("Scintillator+BB1 Energy (keV)");
		user_xmin = xmin;
		user_xmax = 5500.0;
	}
	else if (type == std::string("E_beta") )
	{
		nbins = 600;
		xmin = -0.5;
		xmax = 6000.5-1.0;
		units = std::string("Beta Energy (keV)");
		user_xmin = xmin;
		user_xmax = 5500.0;
	}
	else if (type==std::string("beta_energy"))
	{
		nbins = 300;
		xmin = -0.5;
		xmax = 30000.5-1.0;
		units = std::string("Beta Energy (keV)");
		user_xmin = xmin;
		user_xmax = 6000.5;
	}
	else if (type == std::string("energy_bins") )  // for SOE_TOF.
	{
		nbins = 7;
		xmin = -1.0;
		xmax = 6.0;
		units = std::string("Beta Energy (MeV)");
		user_xmin = xmin;
		user_xmax = 5.0;
	}
	else if (type == std::string("E_beta_mono") )
	{
		nbins = 650;
		xmin = -0.5;
		xmax = 6500.5-1.0;
		units = std::string("Scintillator Energy (keV)");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("Ben_Ebeta") )
	{
		nbins = 60;
		xmin = 0.0;
		xmax = 6000.0;
		units = std::string("Scintillator Energy [keV]");
		user_xmin = xmin;
		user_xmax = 5100.0;
	}
	else if (type == std::string("Ben_10Ebeta") )
	{
		nbins = 60*10;
		xmin = 0.0;
		xmax = 6000.0;
		units = std::string("Scintillator Energy [keV]");
		user_xmin = xmin;
		user_xmax = 5100.0;
	}
	else if (type == std::string("Ben_Ebeta_4096") )
	{
		nbins = 4096;
		xmin = -277.240646003795860;
		xmax = 9997.69048521763943;
		units = std::string("Scintillator Energy [keV]");
		user_xmin = xmin;
		user_xmax = 5100.0;
	}
	else if (type == std::string("Ben_Ebeta_256_t_AB") )
	{
		nbins = 256;
		xmin = -277.241;
		xmax = 9997.69;
		units = std::string("Scintillator Energy [keV] (Ben 256t - AB)");
		user_xmin = 0.0;
		user_xmax = 5100.0;
	}
	else if (type == std::string("Ben_Ebeta_256_b_AB") )
	{
		nbins = 256;
		xmin = -336.62;
		xmax = 9336.69;
		units = std::string("Scintillator Energy [keV] (Ben 256b - AB)");
		user_xmin = 0.0;
		user_xmax = 5100.0;
	}
	else if (type == std::string("Ben_Ebeta_256_t_CDE") )
	{
		nbins = 256;
		xmin = -286.453;
		xmax = 10261.3;
		units = std::string("Scintillator Energy [keV] (Ben 256t - CDE)");
		user_xmin = 0.0;
		user_xmax = 5100.0;
	}
	else if (type == std::string("Ben_Ebeta_256_b_CDE") )
	{
		nbins = 256;
		xmin = -347.27;
		xmax = 9565.69;
		units = std::string("Scintillator Energy [keV] (Ben 256b - CDE)");
		user_xmin = 0.0;
		user_xmax = 5100.0;
	}
	else if (type == std::string("Ben_SOE_tof_1200") )
	{
		nbins = 1200;
		xmin = -14.49;
		xmax = 571.447;
		units = std::string("SOE TOF [ns]");
		user_xmin = -5.0;
		user_xmax = 40.0;
	}
	else if (type==std::string("Mapping_Ebeta"))
	{
		nbins = 6000;
		xmin =  0.0-0.5;
		xmax = 6000.0-0.5;
		units = std::string("Scintillator Energy (keV)");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type==std::string("AmplitudeBinning_Ebeta"))
	{
	//	nbins = 6000;
		nbins = 50*9*5*3;  // so I can rebin by 3, 5, 9.  if I feel like doing so.  total bins = 6750.
		xmin =  0.0-125.0/2.0;
		xmax = 6000.0-125.0/2.0+250.0;  // max at 6187.5.  I think.
		units = std::string("Beta Kinetic Energy (keV)");
		user_xmin = xmin;
		user_xmax = 5270.0;  
	}
	else if (type == std::string("acmottime"))
	{
		nbins = 1000;
		xmin = -0.5;
		xmax = 5000.5-1.0;
		units = std::string("Time Since AC-MOT Start (microseconds)");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("events_per_time")) 
	{
		units = std::string("Unix Time (s) - 1402200000");
	//	xmin = 1402200000;
		xmin = 0.0 - 0.5;
		xmax = xmin + 600000.0 + 0.5;// - 1.0;
		nbins = int(xmax-xmin);
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type==std::string("x_sumdiff") )
	{
		nbins = 1000;
		xmin = -2000.5;
		xmax = -0.5;
		units = std::string("x1+x2 - 2*t_imcp");
		user_xmin = -1200.0;
		user_xmax = -700.0;
	}
	else if (type==std::string("z_sumdiff") )
	{
		nbins = 1000;
		xmin = -1500.5;
		xmax = 500.5-1.0;
		units = std::string("z1+z2 - 2*t_imcp");
		user_xmin = -600.0;
		user_xmax = -150.0;
	}
	//
	else if (type==std::string("danny_sumx") )
	{
		nbins = 1000;
		xmin = 68000.5;
		xmax = 78000.5-1.0;
		units = std::string("x1+x2 - 2*(t_imcp-t_photodiode)");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type==std::string("danny_sumz") )
	{
		nbins = 1000;
		xmin = 69000.5;
		xmax = 79000.5-1.0;
		units = std::string("z1+z2 - 2*(t_imcp-t_photodiode)");
		user_xmin = xmin;
		user_xmax = xmax;	
	}
	//
	else if (type==std::string("melsum") )
	{
		nbins = 5000;
		xmin = -4000.5;
		xmax = 1000.5-1.0;
		units = std::string("x1+x2 + z1+z2 - 4*t_imcp");
		user_xmin = -1800;
		user_xmax = -900;
	}
	else if (type==std::string("meldiff") )
	{
		nbins = 1000;
		xmin = -1000.5+1.0;
		xmax = 0.5;
		units = std::string("(x1+x2) - (z1+z2)");
		user_xmin = -750.0;
		user_xmax = -500.0;
	}
	//
	else if (type==std::string("x_sumdiff_9055") )
	{
		nbins = 500;
		xmin = -0.5;
		xmax = 1000.5-1.0;
		units = std::string("x1+x2 - 2*t_imcp");
		user_xmin = 100.0;
		user_xmax = 800.0;
	}
	else if (type==std::string("z_sumdiff_9055") )
	{
		nbins = 1000;
		xmin = -500.5;
		xmax = 1500.5-1.0;
		units = std::string("z1+z2 - 2*t_imcp");
		user_xmin = 200.0;
		user_xmax = 1000.0;
	}
	else if (type==std::string("sumdiff_combo") )
	{
		nbins = 1000;
		xmin = -0.5;
		xmax = 2000.5-1.0;
		units = std::string("( (x1+x2 - 2*t_imcp)^2 + (z1+z2 - 2*t_imcp)^2 )^(1/2)");
		user_xmin = 700.0;
		user_xmax = 1600.0;
	}
	else if(type==std::string("coarse_imcp_x")) // only for the 2Ds, really.
	{
		units = std::string("iMCP x (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 300;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("coarse_imcp_z")) // only for the 2Ds, really.
	{
		units = std::string("iMCP z (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 300;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("imcp_x")) // only for the 2Ds, really.
	{
		units = std::string("iMCP x (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 600;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("imcp_z")) // only for the 2Ds, really.
	{
		units = std::string("iMCP z (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 600;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("fine_imcp_x")) // only for the 2Ds, really.
	{
		units = std::string("iMCP x (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 1200;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("fine_imcp_z")) // only for the 2Ds, really.
	{
		units = std::string("iMCP z (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 1200;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("fine_imcp_x_PI")) // only for the 2Ds, really.
	{
		units = std::string("iMCP x (mm)");
		xmin = -22.5;
		xmax = xmin + 45.0;
//		nbins = 1200*2/7;
		nbins = 453*2;  // 412 ?
		user_xmin = -10.0;
		user_xmax = 10.0;
	}
	else if(type==std::string("fine_imcp_z_PI")) // only for the 2Ds, really.
	{
		units = std::string("iMCP z (mm)");
		xmin = -22.5;
		xmax = xmin + 45.0;
//		nbins = 1200*2/7;
		nbins = 453*2;
		user_xmin = -10.0;
		user_xmax = 10.0;
	}
	else if(type==std::string("emcp_x")) 
	{
		units = std::string("eMCP x (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 600;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("emcp_z")) 
	{
		units = std::string("eMCP z (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 600;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("bb1_x")) 
	{
		units = std::string("bb1 x (mm)");
		xmin = -20.5;
		xmax = 20.5;
		nbins = 82;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("bb1_y")) 
	{
		units = std::string("bb1 y (mm)");
		xmin = -20.5;
		xmax = 20.5;
		nbins = 82;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("bb1_strip")) 
	{
		units = std::string("bb1 strip number");
		xmin = -0.5;
		xmax = 40.5-1.0;
		nbins = 40;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("bb1_energy"))
	{
		units = std::string("Calibrated BB1 Energy (keV)");
		xmin = -0.5;
		xmax = xmin + 6000.0;
		nbins = 600*2;
		user_xmin = xmin;
		user_xmax = 400.5;
	}
	else if(type==std::string("runno")) 
	{
		units = std::string("Run Number");
		xmin = 0.0;
		xmax = 600.0;
		nbins = 600+1;
		user_xmin = 300;
		user_xmax = 520;
	}
	else if(type==std::string("N_hits")) // only for the 2Ds, really.
	{
		units = std::string("Event Count");
		xmin = -0.5;
		xmax = xmin + 50.0;
		nbins = 50;
		user_xmin = 0.5;
		user_xmax = 5.5;
	}
	else if(type==std::string("WSA_Sum"))
	{
		units = std::string("Wedge+Strip+Z");
		xmin = -99.5;
		xmax = xmin + 12500.0;
		nbins = 625;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("QDC_single"))
	{
		units = std::string("QDC (arb)");
		xmin = -99.5;
		xmax = xmin + 4400.0;
		nbins = 4400;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("WSA_Position"))
	{
		units = std::string("WSA_Position (uncalibrated)");
		xmin = -0.5;
		xmax = xmin + 2.0;
		nbins = 600;
		user_xmin = 0.0;
		user_xmax = 2.0;
	}
	else if(type==std::string("stupid_tof"))
	{
		nbins =   4000;
		xmin = (-100000.5+1.0)*v1192_to_ns;
		xmax = ( 100000.5)*v1192_to_ns;
		units = std::string("TOF [ns]");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("hitcounter"))
	{
		nbins = 51;
		xmin = -0.5;
		xmax = 50.5;
		units = std::string("Number of TDC Hits");
		user_xmin = 0.5;
		user_xmax = 8.5;
	}
	else if(type==std::string("scaled_probability"))
	{
		nbins = 170;
		xmin = 0.0;
		xmax = 0.17;
		units = std::string("Scaled Probability");
		user_xmin = 0.0;
		user_xmax = 0.17;
	}
	else if(type==std::string("delta_costheta"))
	{
		nbins = 4000;
		xmin = -2.0;
		xmax = 2.0;
		units = std::string("Delta Cos(theta)");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("costheta"))
	{
		nbins = 6000;
		xmin = -1.0;
		xmax = 1.0;
		units = std::string("Cos(theta)");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("mmcostheta"))
	{
		nbins = 4000;
		xmin = -1.0;
		xmax = -0.9;
		units = std::string("Cos(theta)");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("holstein_betatof"))
	{
		nbins = 600;
		xmin = 0.34e-6;
		xmax = 0.4e-6;
		units = std::string("ns?  maybe??");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("bb1_r")) 
	{
		units = std::string("bb1 x^2 + y^2 (mm)");
		xmin = -0.5;
		xmax = 40.5;
		nbins = 82;
		user_xmin = xmin;
		user_xmax = 20.5;
	}
	else if (type == std::string("holstein_pbeta"))  
	{
		nbins = 1200;
		xmin = -6000.0;
		xmax =  6000.0;
		units = std::string("beta momentum [keV]");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("holstein_pbeta_squared"))  
	{
		nbins = 1200;
		xmin = 0.0;
		xmax = 5500.0*5500.0;
		units = std::string("p_beta^2 [keV]");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if (type == std::string("holstein_pbeta_radial"))  
	{
		nbins = 6000;
		xmin = -0.5;
		xmax =  6000.0 - 1.0;
		units = std::string("Beta Perpendicular Momentum, sqrt(px^2 + py^2) [keV]");
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else
	{
		cout << endl;
		cout << "*  Could not recognize hist type: " << type << endl;
		cout << endl;
		return 1;
	}
	return 0;
}

//TH1D * CreateHist(std::string title, std::string type, int color, int rebin_factor=1)
TH1D * CreateHist(std::string title, std::string type, int color, int rebin_factor)
{
	hist_type my_hist_type = hist_type(type);
	TH1D * this_hist = new TH1D(title.c_str(), title.c_str(), my_hist_type.nbins, my_hist_type.xmin, my_hist_type.xmax);
	this_hist -> SetLineColor(color);
	this_hist -> SetMarkerColor(color);
	this_hist -> GetXaxis() -> SetTitle(my_hist_type.units.c_str());

	this_hist -> Rebin(rebin_factor);
	this_hist -> GetXaxis() -> SetRangeUser(my_hist_type.user_xmin, my_hist_type.user_xmax);
	
	return this_hist;
}


//TH1D * CreateHist(std::string title, std::string type, TColor my_color, int rebin_factor=1)
TH1D * CreateHist(std::string title, std::string type, TColor my_color, int rebin_factor)
{
//	TColor *this_color;
//	Int_t color = this_color -> GetFreeColorIndex(); // nope.  only in root 6.
//	Int_t color = TColor::GetFreeColorIndex();
//	TColor *this_color = new TColor(color, my_color.GetRed(), my_color.GetGreen(), my_color.GetBlue());
	Int_t color = TColor::GetColor(my_color.GetRed(), my_color.GetGreen(), my_color.GetBlue());
//	this_color = new TColor(color, my_color.GetRed(), my_color.GetGreen(), my_color.GetBlue());

	hist_type my_hist_type = hist_type(type);
	TH1D * this_hist = new TH1D(title.c_str(), title.c_str(), my_hist_type.nbins, my_hist_type.xmin, my_hist_type.xmax);
	this_hist -> SetLineColor(color);
	this_hist -> SetMarkerColor(color);
	this_hist -> GetXaxis() -> SetTitle(my_hist_type.units.c_str());

	this_hist -> Rebin(rebin_factor);
	this_hist -> GetXaxis() -> SetRangeUser(my_hist_type.user_xmin, my_hist_type.user_xmax);

	return this_hist;
}

// UnevenRebin:  the 'gift' from Ben.
TH1D * UnevenRebin(TH1D * horig, TH1D * htofill)
{
//	TH1D * htofill = new TH1D();
	int debug = 0;
	if (debug == 1)
	{
		printf("htofill: nbins = %d, low = %f, high = %f\n", htofill->GetNbinsX(),
			htofill->GetBinLowEdge(1), htofill->GetBinLowEdge(htofill->GetNbinsX()) 
			+ htofill->GetBinWidth(htofill->GetNbinsX()) );
	}
	//
	int bmax = horig->GetNbinsX();
	double xlo, xup;
	double cont;
	int b, b1, b2;
	double edge;
	double frac1, frac2;
	// for i in range(0, bmax+1) :
	for(int i=0; i<= bmax; i++)
	{
		xlo = horig->GetBinLowEdge(i);
		xup = xlo + horig->GetBinWidth(i);
		cont = horig->GetBinContent(i);
		if (cont == 0) { continue; }
		if (debug > 0 && cont > 0) 
			{ printf("Source bin %d: lo edge = %f\thi edge = %f\tcontent = %f\n", i, xlo, xup, cont); }
		if (htofill->FindBin(xlo) == htofill->FindBin(xup)) // all in one bin
		// MJA:  I don't think this (above) is the correct condition...
		{
			b = htofill->FindBin(xlo);
			htofill->SetBinContent(b, htofill->GetBinContent(b) + cont);
			if (debug > 0) { printf("Dest bin %d incremented by %f\n", b, cont); }
		//	if (debug > 0) { printf("Dest bin %f incremented by %f\n", b, cont); }
		}
		else // split it up (linearly)
		{
			b1 = htofill->FindBin(xlo);
			b2 = htofill->FindBin(xup);
			if ( !(b2 == b1 + 1) ) 
			{
				cout << "Uhoh, unsupported UnevenRebin!!" << endl;
				return htofill;
			}
			edge = htofill->GetBinLowEdge(b2);
			frac1 = (edge - xlo) / (xup - xlo);
			frac2 = 1.0 - frac1;
			
			htofill->SetBinContent(b1, htofill->GetBinContent(b1) + cont*frac1);
			htofill->SetBinContent(b2, htofill->GetBinContent(b2) + cont*frac2);
			if (debug > 0)
			{
				printf( "Dest bin %d incremented by %f\n", b1, cont*frac1);
				printf( "Dest bin %d incremented by %f\n", b2, cont*frac2);
				printf( "Total increment: %f\n", cont*(frac1+frac2));
			//	raw_input("e");
			}
		}
	}
	if (abs(horig->Integral(0, bmax) - htofill->Integral(0, bmax)) > 0.1)
	{
		cout << "Entries not conserved!" << endl;
		cout << "Source:      " << horig->Integral(0, bmax) << endl;
		cout << "Destination: " << htofill->Integral(0, bmax) << endl;
	}
	else
	{
		cout << "Entries conserved.  \t";// << endl;
		printf("Integral = %f\n", htofill->Integral(0, bmax) );
		//for i in range(100) :
		//print "%f\t%f" % (htofill.GetBinError(i), sqrt(htofill.GetBinContent(i)))
	}
	return htofill;
}

/*
class hist_type_2d
{
private:
	void set_other_parameters();
public:
	int rebin_x;
	int rebin_y;

	std::string type_x;
	std::string type_y;
	std::string option;
	std::string units_x;
	std::string units_y;
	
	int nbins_x;
	double xmin;
	double xmax;
	double user_xmin;
	double user_xmax;
	
	int nbins_y;
	double ymin;
	double ymax;
	double user_ymin;
	double user_ymax;
	
	hist_type_2d(std::string my_type_x, std::string my_type_y, int my_rebin_x, int my_rebin_y);
};
*/

hist_type_2d::hist_type_2d(std::string my_type_x, std::string my_type_y, int my_rebin_x, int my_rebin_y)
{
	type_x = my_type_x;
	type_y = my_type_y;
	rebin_x = my_rebin_x;
	rebin_y = my_rebin_y;
	set_other_parameters();
}

void hist_type_2d::set_other_parameters()
{
	hist_type x_hist_type = hist_type(type_x); 
	hist_type y_hist_type = hist_type(type_y); 
	
	if( !x_hist_type.failed && !y_hist_type.failed )
	{
		units_x = x_hist_type.units;
		nbins_x = x_hist_type.nbins/rebin_x;
		xmin = x_hist_type.xmin;
		xmax = x_hist_type.xmax;
		user_xmin = x_hist_type.user_xmin;
		user_xmax = x_hist_type.user_xmax;

		units_y = y_hist_type.units;
		nbins_y = y_hist_type.nbins/rebin_y;
		ymin = y_hist_type.xmin;
		ymax = y_hist_type.xmax;
		user_ymin = y_hist_type.user_xmin;
		user_ymax = y_hist_type.user_xmax;
	}
	else
	{
		cout << endl;
		cout << "*  Could not recognize hist type(s): " << type_x << ", " << type_y << endl;
		cout << endl;
		return;
	}
}

TH2D * CreateHist2d(std::string title, std::string x_type, std::string y_type, int rebin_x, int rebin_y)
{
	hist_type_2d my_hist_type_2d = hist_type_2d(x_type, y_type, rebin_x, rebin_y);
	
	TH2D * this_hist_2d = new TH2D(title.c_str(), title.c_str(), my_hist_type_2d.nbins_x, my_hist_type_2d.xmin, my_hist_type_2d.xmax, my_hist_type_2d.nbins_y, my_hist_type_2d.ymin, my_hist_type_2d.ymax );
	
	std::string option = std::string("colz");
	this_hist_2d -> SetOption( option.c_str() );
	
	this_hist_2d -> GetXaxis() -> SetTitle(my_hist_type_2d.units_x.c_str());
	this_hist_2d -> GetXaxis() -> SetRangeUser(my_hist_type_2d.user_xmin, my_hist_type_2d.user_xmax);
	this_hist_2d -> GetXaxis() -> SetLabelSize(0.025); 
	
	this_hist_2d -> GetYaxis() -> SetTitle(my_hist_type_2d.units_y.c_str());
	this_hist_2d -> GetYaxis() -> SetRangeUser(my_hist_type_2d.user_ymin, my_hist_type_2d.user_ymax);
	this_hist_2d -> GetYaxis() -> SetLabelSize(0.025); 

	this_hist_2d -> SetStats(0);
	
	return this_hist_2d;
}

TH2D * CreateHist2d(std::string title, std::string x_type, std::string y_type, int rebin)
{
	return CreateHist2d(title, x_type, y_type, rebin, rebin);
}

TH2D * CreateHist2d(std::string title, std::string x_type, std::string y_type)
{
	int rebin_x = 1;
	int rebin_y = 1;
	return CreateHist2d(title, x_type, y_type, rebin_x, rebin_y);
}



#endif
