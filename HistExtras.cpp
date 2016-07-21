#include <TH1D.h>
#include <TH2D.h>

// Examples:
// TH1D* OP_Cycle_Events_all = CreateHist(string("Electron Events - All"), string("acmottime"), int(kRed));
// TH2D* tof_v_iqcd_ac  = CreateHist2d(string("tof_vs_iqdc_ac"),  string("i_qdc"), string("cal_tof"));

double v1192_to_ns = 100.0/1024.0;

// ====================================== //
// Histogram Functions 
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
	else if (type == std::string("cal_tof"))
	{
		nbins =   1400;
		xmin = (-80000.5+1.0)*v1192_to_ns;
		xmax = ( 60000.5)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin = -500;
		user_xmax = 5500;
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
		user_xmin = -400;
		user_xmax = 2000;
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
	/*
	else if (type == std::string("cal_tof_zoom"))
	{
		nbins = 300;
		xmin = (-5000.5)*v1192_to_ns;
		xmax = (25000.5-1.0)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin = 0;
		user_xmax = 3000;
	}
	*/
	/*
	else if (type == std::string("extrafine_cal_tof"))
	{
		nbins =  14000;
		xmin = (-10000.5+1.0)*v1192_to_ns;
		xmax = ( 60000.5)*v1192_to_ns;
		units = std::string("ion tof (ns)");
		user_xmin = -500;
		user_xmax = 5500;
	}
	*/
	else if (type == std::string("cal_e_tof"))
	{
		nbins = 20000;
		xmin = (-10000.5+1.0)*v1192_to_ns;
		xmax = (10000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = 50;
		user_xmax = 150;
	}
	else if (type == std::string("cal_e_scint_tof")) // electron tof w.r.t. scint time.
	{
		nbins = 4000;
		xmin = (-2000.5+1.0)*v1192_to_ns;
		xmax = (2000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = -100;
		user_xmax = -40;
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
	/*
	if (type == std::string("cal_tof_mus"))
	{
		nbins = 350;
		xmin = (-80000.5+1.0)*v1192_to_ns;
		xmax = (60000.5)*v1192_to_ns;
		units = std::string("microsec");
		user_xmin = -500;
		user_xmax = 5500;
	}
	*/
	else if (type == std::string("e_qdc") )
	{
		nbins = 500;
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
	//	nbins = 1000;
		nbins = 600;
		xmin = -0.5;
		xmax = 6000.5-1.0;
		units = std::string("Scint. Energy (keV)");
		user_xmin = xmin;
		user_xmax = 5500.0;
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
	else if (type==std::string("beta_energy"))
	{
		nbins = 300;
		xmin = -0.5;
		xmax = 30000.5-1.0;
		units = std::string("beta energy (keV)");
		user_xmin = xmin;
		user_xmax = 6000.5;
	}
	else if (type==std::string("events_per_time"))
	{
		nbins = 60*90;
		xmin = -0.5;
		xmax = 60*60*90-0.5;
		units = std::string("Time (min?)");
		user_xmin = xmin;
		user_xmax = 60*60*30;
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
		nbins = 1400;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("fine_imcp_z")) // only for the 2Ds, really.
	{
		units = std::string("iMCP z (mm)");
		xmin = -44.5;
		xmax = xmin + 90.0;
		nbins = 1400;
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
	
	/*
	else if(type==std::string("imcp_x_PI")) 
	{
		units = std::string("iMCP x (mm)");
		xmin = -20.5;
		xmax = xmin + 40.0;
		nbins = 600;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	else if(type==std::string("imcp_z_PI")) 
	{
		units = std::string("iMCP z (mm)");
		xmin = -20.5;
		xmax = xmin + 40.0;
		nbins = 600;
		user_xmin = xmin;
		user_xmax = xmax;
	}
	*/
	else if(type==std::string("N_hits")) // only for the 2Ds, really.
	{
		units = std::string("Event Count");
		xmin = -0.5;
		xmax = xmin + 50.0;
		nbins = 50;
		user_xmin = 0.5;
		user_xmax = 5.5;
	}
	/*
	else if (type == std::string("cal_tof_395"))
	{
		nbins = 1400;
		xmin = (-80000.5+1.0)*v1192_to_ns;
		xmax = (60000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = -500;
		user_xmax = 5500;
	}
	else if (type == std::string("cal_tof_415"))
	{
		nbins = 1400;
		xmin = (-80000.5+1.0)*v1192_to_ns;
		xmax = (60000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = -500;
		user_xmax = 5500;
	}
	else if (type == std::string("cal_tof_535"))
	{
		nbins = 1400;
		xmin = (-80000.5+1.0)*v1192_to_ns;
		xmax = (60000.5)*v1192_to_ns;
		units = std::string("ns");
		user_xmin = -500;
		user_xmax = 5500;
	}
	*/
	else
	{
		cout << endl;
		cout << "*  Could not recognize hist type: " << type << endl;
		cout << endl;
		return 1;
	}
	return 0;
}

TH1D * CreateHist(std::string title, std::string type, int color=1)
{
	hist_type my_hist_type = hist_type(type);
	TH1D * this_hist = new TH1D(title.c_str(), title.c_str(), my_hist_type.nbins, my_hist_type.xmin, my_hist_type.xmax);
	this_hist -> SetLineColor(color);
	this_hist -> GetXaxis() -> SetTitle(my_hist_type.units.c_str());
	this_hist -> GetXaxis() -> SetRangeUser(my_hist_type.user_xmin, my_hist_type.user_xmax);
	
	return this_hist;
}

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

TH2D * CreateHist2d(std::string title, std::string x_type, std::string y_type, int rebin_x=1, int rebin_y=1)
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
