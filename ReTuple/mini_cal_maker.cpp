#ifndef CAL_INCLUDED
#define CAL_INCLUDED

#include <vector>
#include <map>
#include <cmath>  // trinatdaq

// -- * -- // -- * -- // -- * -- // -- * -- // -- * -- // -- * -- // -- * -- // 

class calibration
{
private:
	double phi, cos_phi, sin_phi;
	double slide_x, slide_z;
	
	double stretch_top, stretch_bottom, stretch_left, stretch_right;
	
	double rescale_x(double x1, double x2);
	double rescale_z(double z1, double z2);
	
	double offset_x(double);
	double offset_z(double);
	
	void load_position_offsets();
	void load_nominal();
	void load_expected();
	void load_stretchparams();

public:
	double nominal_x0, nominal_x1, nominal_x2, nominal_x3, nominal_x4, nominal_x5, \
		nominal_x6, nominal_x7, nominal_x8, nominal_x9, nominal_x10, nominal_x11, \
		nominal_x12, nominal_x13, nominal_x14, nominal_x15, nominal_x16, nominal_x17;
	double nominal_zA, nominal_zB, nominal_zC, nominal_zD, nominal_zE, nominal_zF, \
		nominal_zG, nominal_zH, nominal_zI, nominal_zJ, nominal_zK, nominal_zL, \
		nominal_zM, nominal_zN, nominal_zO, nominal_zP, nominal_zQ, nominal_zR;
	
	double expected_x0, expected_x1, expected_x2, expected_x3, expected_x4, expected_x5, \
		expected_x6, expected_x7, expected_x8, expected_x9, expected_x10, expected_x11, \
		expected_x12, expected_x13, expected_x14, expected_x15, expected_x16, expected_x17;
	double expected_zA, expected_zB, expected_zC, expected_zD, expected_zE, expected_zF, \
		expected_zG, expected_zH, expected_zI, expected_zJ, expected_zK, expected_zL, \
		expected_zM, expected_zN, expected_zO, expected_zP, expected_zQ, expected_zR;

	calibration(int run);

	double x_scale;
	double z_scale;
	
	double x_offset;
	double z_offset;

	double mismatch_x_9055;
	double mismatch_z_9055;
	
	int run_to_calibrate;
	
	pair<double, double> offset_xz(double, double);

	void rotate(pair<double, double> & );
	void cal_slide(pair<double, double> & );
	void big_squeeze(pair<double, double> & );
	void micro_squeeze_x(pair<double, double> & );
	void micro_squeeze_z(pair<double, double> & );
	void edge_stretch(pair<double, double> & );
	
	void flip_z(pair<double, double> & );
	void flip_x(pair<double, double> & );
	
	pair<double, double> cal_0(double x1, double x2, double z1, double z2);
	pair<double, double> apply_calibration(double x1, double x2, double z1, double z2, int level);
};

calibration::calibration(int run)
{
	run_to_calibrate = run;

	x_scale= 0.0525977;
	z_scale= 0.0502919;

	mismatch_x_9055 = 0.661175;
	mismatch_z_9055 = 0.246667;
	
	load_position_offsets();
	load_expected();
	load_nominal();
	load_stretchparams();
	
	slide_x = nominal_x1 - expected_x1;
	slide_z = nominal_zQ - expected_zQ;
}

void calibration::load_position_offsets()
{
	double base_x_offset =   8.70;
	double base_z_offset = -43.80;
	
	if(run_to_calibrate==9055)
	{
		x_offset = -1.3;
		z_offset = -1.2;
	}
	else
	{
		x_offset = base_x_offset;
		z_offset = base_z_offset;
	}
	return;
}
void calibration::load_expected()
{
	expected_x0 = -40.0;
	expected_x1 = -35.0;
	expected_x2 = -30.0;
	expected_x3 = -25.0;
	expected_x4 = -20.0;
	expected_x5 = -15.0;
	expected_x6 = -10.0;
	expected_x7 = -5.0;
	expected_x8 = 0.0;
	expected_x9 = 5.0;
	expected_x10 = 10.0;
	expected_x11 = 15.0;
	expected_x12 = 20.0;
	expected_x13 = 25.0;
	expected_x14 = 30.0;
	expected_x15 = 35.0;
	expected_x16 = 40.0;
	expected_x17 = 45.0;

	expected_zR = -40.0;
	expected_zQ = -35.0;
	expected_zP = -30.0;
	expected_zO = -25.0;
	expected_zN = -20.0;
	expected_zM = -15.0;
	expected_zL = -10.0;
	expected_zK = -5.0;
	expected_zJ = 0.0;
	expected_zI = 5.0;
	expected_zH = 10.0;
	expected_zG = 15.0;
	expected_zF = 20.0;
	expected_zE = 25.0;
	expected_zD = 30.0;
	expected_zC = 35.0;
	expected_zB = 40.0;
	expected_zA = 45.0;	
}
void calibration::load_nominal()
{
	phi= 4.0E-03; 	// spencer2012:  phi= -1.27843092E-03;
	cos_phi = std::cos(phi);
	sin_phi = std::sin(phi);
	
	// before cal_slide, use these nominal values:
	nominal_x1 = -35.91;
	nominal_x15=  34.09;
	nominal_zQ = -35.64;
	nominal_zC =  34.36;

//	// after 'cal_slide', the nominal values above will be obsolete, and will match expected.
//	// we'll have a new nominal values set up though:
	nominal_zL = -8.61;
	nominal_zH = 11.87;
	nominal_x6 = -9.05;
	nominal_x10 = 11.27;
	
//	// After 'big_squeeze', the above will be obsolete, and we'll have some more:
	nominal_x2 = -30.38;
	nominal_x3 = -25.30;
	nominal_x4 = -20.20;
	nominal_x5 = -15.05;
	//
	nominal_x7 = -4.76;
	nominal_x8 =  0.13;
	nominal_x9 =  5.38;
	//
	nominal_x11 = 15.22;
	nominal_x12 = 20.02;
	nominal_x13 = 24.85;
	nominal_x14 = 30.03;

//	// ...And z values too!
	nominal_zD = 29.68;
	nominal_zE = 24.61;
	nominal_zF = 19.83;
	nominal_zG = 15.01;
	//
	nominal_zI =  5.08;
	nominal_zJ =  0.15;
	nominal_zK = -4.90;
	//
	nominal_zM = -15.12;
	nominal_zN = -20.08;
	nominal_zO = -25.07;
	nominal_zP = -30.08;
	//
}
void calibration::load_stretchparams()
{
	// must have already called load_expected().
	double second_top    =  30.12;
	double second_bottom = -29.76;
	double second_left   = -30.23;
	double second_right  =  30.38;
	
	double scale_top    = (expected_zD - second_top + 5.0)/5.0;
	double scale_bottom = (second_bottom - expected_zP + 5.0)/5.0;
	double scale_left   = (second_left - expected_x2 + 5.0)/5.0;
	double scale_right  = (expected_x14 - second_right + 5.0)/5.0;
	
	stretch_top    = 1.0/scale_top;
	stretch_bottom = 1.0/scale_bottom;
	stretch_left   = 1.0/scale_left;
	stretch_right  = 1.0/scale_right;
}
double calibration::rescale_x(double x1, double x2)
{
	double x = x1 - x2;
	x = x*x_scale;
	return x;
}
double calibration::rescale_z(double z1, double z2)
{
	double z = z1 - z2;
	z = z*z_scale;
	return z;
}

pair<double, double> calibration::offset_xz(double x_in, double z_in)
{
	double z = z_in - z_offset;
	double x = x_in - x_offset;

	pair<double, double> coordinates = make_pair(x, z);
	return coordinates;
}

void calibration::rotate(pair<double, double> & coords)
{
	double x_out = coords.first*cos_phi - coords.second*sin_phi;
	double z_out = coords.first*sin_phi + coords.second*cos_phi;
	
	coords.first = x_out;
	coords.second = z_out;
	return;
}

void calibration::cal_slide(pair<double, double> & coords)
{
	coords.first = coords.first - slide_x;
	coords.second = coords.second - slide_z;
}

void calibration::big_squeeze(pair<double, double> & coords)
{
	double orig_interval;
	double orig_start;
	
	double new_interval;
	double new_start;
	
	double frac_offset;
	
	// do NOT use nominal_x1, nominal_x15, nominal_zQ, nominal_zC.
	if(coords.first > expected_x1 && coords.first <= expected_x15)
	{
		if( coords.first <= nominal_x6) // left.
		{
			orig_start = expected_x1;
			new_start = expected_x1;
			orig_interval = nominal_x6 - orig_start;
			new_interval = expected_x6 - new_start;
		}
		else if( coords.first <= nominal_x10) // center.
		{
			orig_start = nominal_x6;
			new_start = expected_x6;
			orig_interval = nominal_x10 - orig_start;
			new_interval = expected_x10 - new_start;
		}
		else // right.
		{
			orig_start = nominal_x10;
			new_start = expected_x10;
			orig_interval = expected_x15 - orig_start;
			new_interval = expected_x15 - new_start;
		}
		frac_offset = (coords.first - orig_start)/orig_interval;
		coords.first = frac_offset*new_interval + new_start;
	}
	//
	if(coords.second > expected_zQ && coords.second <= expected_zC)
	{
		if(coords.second <= nominal_zL) // bottom.
		{
			orig_start = expected_zQ;
			new_start = expected_zQ;
			orig_interval = nominal_zL - orig_start;
			new_interval = expected_zL - new_start;
		}
		else if( coords.second <= nominal_zH) // middle.
		{
			orig_start = nominal_zL;
			new_start = expected_zL;
			orig_interval = nominal_zH - orig_start;
			new_interval = expected_zH - new_start;			
		}
		else // top.
		{
			orig_start = nominal_zH;
			new_start = expected_zH;
			orig_interval = expected_zC - orig_start;
			new_interval = expected_zC - new_start;			
		}
		frac_offset = (coords.second - orig_start)/orig_interval;
		coords.second = frac_offset*new_interval + new_start;
	}
}

void calibration::micro_squeeze_x(pair<double, double> & coords)
{
	double x = coords.first;
	double z = coords.second;
	
	double orig_interval;
	double orig_start;
	
	double new_interval;
	double new_start;
	
	double frac_offset;
	double new_x;
	
	// do NOT use nominal_x1, nominal_x15, nominal_zQ, nominal_zC.
	// Now, also don't use nominal_x6, nominal_x10, nominal_zL, nominal_zH.
	if(x > expected_x1 && x <= expected_x15)
	{
		if( x <= expected_x6) // left.
		{ // x1 - x6.
			if( x <= nominal_x2)
			{
				orig_interval = nominal_x2 - expected_x1;
				new_interval = expected_x2 - expected_x1;
				orig_start = expected_x1;
				new_start = expected_x1;
			}
			else if( x <= nominal_x3)
			{
				orig_interval = nominal_x3 - nominal_x2;
				new_interval = expected_x3 - expected_x2;
				orig_start = nominal_x2;
				new_start = expected_x2;
			}
			else if( x <= nominal_x4)
			{
				orig_interval = nominal_x4 - nominal_x3;
				new_interval = expected_x4 - expected_x3;
				orig_start = nominal_x3;
				new_start = expected_x3;
			}
			else if( x <= nominal_x5)
			{
				orig_interval = nominal_x5 - nominal_x4;
				new_interval = expected_x5 - expected_x4;
				orig_start = nominal_x4;
				new_start = expected_x4;
			}
			else
			{
				orig_interval = expected_x6 - nominal_x5;
				new_interval = expected_x6 - expected_x5;
				orig_start = nominal_x5;
				new_start = expected_x5;
			}
		}
		else if( x <= expected_x10) // center.
		{ // x6 - x10.
			if( x <= nominal_x7)
			{
				orig_interval = nominal_x7 - expected_x6;
				new_interval = expected_x7 - expected_x6;
				orig_start = expected_x6;
				new_start = expected_x6;
			}
			else if( x <= nominal_x8)
			{
				orig_interval = nominal_x8 - nominal_x7;
				new_interval = expected_x8 - expected_x7;
				orig_start = nominal_x7;
				new_start = expected_x7;
			}
			else if( x <= nominal_x9)
			{
				orig_interval = nominal_x9 - nominal_x8;
				new_interval = expected_x9 - expected_x8;
				orig_start = nominal_x8;
				new_start = expected_x8;
			}
			else
			{
				orig_interval = expected_x10 - nominal_x9;
				new_interval = expected_x10 - expected_x9;
				orig_start = nominal_x9;
				new_start = expected_x9;
			}
		}
		else // right.
		{ // x10 - x15.
			if( x <= nominal_x11)
			{
				orig_interval = nominal_x11 - expected_x10;
				new_interval = expected_x11 - expected_x10;
				orig_start = expected_x10;
				new_start = expected_x10;
			}
			else if( x <= nominal_x12)
			{
				orig_interval = nominal_x12 - nominal_x11;
				new_interval = expected_x12 - expected_x11;
				orig_start = nominal_x11;
				new_start = expected_x11;
			}
			else if( x <= nominal_x13)
			{
				orig_interval = nominal_x13 - nominal_x12;
				new_interval = expected_x13 - expected_x12;
				orig_start = nominal_x12;
				new_start = expected_x12;
			}
			else if( x <= nominal_x14)
			{
				orig_interval = nominal_x14 - nominal_x13;
				new_interval = expected_x14 - expected_x13;
				orig_start = nominal_x13;
				new_start = expected_x13;
			}
			else
			{
				orig_interval = expected_x15 - nominal_x14;
				new_interval = expected_x15 - expected_x14;
				orig_start = nominal_x14;
				new_start = expected_x14;
			}
		}
		frac_offset = (x - orig_start)/orig_interval;
		new_x = frac_offset*new_interval + new_start;
	}
	else
	{
		new_x = x;
	}

	coords.first = new_x;
}

void calibration::micro_squeeze_z(pair<double, double> & coords)
{
	double x = coords.first;
	double z = coords.second;
	
	double orig_interval;
	double orig_start;
	
	double new_interval;
	double new_start;
	
	double frac_offset;
	double new_z;
	
	// Do not use nominal_zQ, nominal_zC, nominal_zL, nominal_zH.
	if(z > expected_zQ && z <= expected_zC)
	{
		if (expected_zH <= z) // top.
		{ // zH - zC
			if (nominal_zD <= z ) // zD <= z < zC
			{
				orig_interval = nominal_zD - expected_zC;
				new_interval = expected_zD - expected_zC;
				orig_start = expected_zC;
				new_start = expected_zC;
			}
			else if(nominal_zE <= z) // zE <= z < zD
			{
				orig_interval = nominal_zE - nominal_zD;
				new_interval = expected_zE - expected_zD;
				orig_start = nominal_zD;
				new_start = expected_zD;
			}
			else if(nominal_zF <= z) // zF <= z < zE
			{
				orig_interval = nominal_zF - nominal_zE;
				new_interval = expected_zF - expected_zE;
				orig_start = nominal_zE;
				new_start = expected_zE;
			}
			else if(nominal_zG <= z) // zG <= z < zF
			{
				orig_interval = nominal_zG - nominal_zF;
				new_interval = expected_zG - expected_zF;
				orig_start = nominal_zF;
				new_start = expected_zF;
			}
			else // zH <= z < zG.
			{
				orig_interval = expected_zH - nominal_zG;
				new_interval = expected_zH - expected_zG;
				orig_start = nominal_zG;
				new_start = expected_zG;
			}
			frac_offset = (z - orig_start)/orig_interval;
			new_z = frac_offset*new_interval + new_start;
		}
		else if(expected_zL <= z) // middle.
		{ // zL - zH
			if(nominal_zI <= z) // zI <= z < zH
			{
				orig_interval = nominal_zI - expected_zH;
				new_interval = expected_zI - expected_zH;
				orig_start = expected_zH;
				new_start = expected_zH;
			}
			else if(nominal_zJ <= z) // zJ <= z < zI
			{
				orig_interval = nominal_zJ - nominal_zI;
				new_interval = expected_zJ - expected_zI;
				orig_start = nominal_zI;
				new_start = expected_zI;
			}
			else if(nominal_zK <= z) // zK <= z < zJ
			{
				orig_interval = nominal_zK - nominal_zJ;
				new_interval = expected_zK - expected_zJ;
				orig_start = nominal_zJ;
				new_start = expected_zJ;
			}
			else // zL <= z < zK
			{
				orig_interval = expected_zL - nominal_zK;
				new_interval = expected_zL - expected_zK;
				orig_start = nominal_zK;
				new_start = expected_zK;
			}
		}
		//if(z < expected_zL) // bottom.
		else   // bottom.
		{ // zQ - zL (z < zL.)
			if (nominal_zM <= z) // zM <= z < zL
			{
				orig_interval = nominal_zM - expected_zL;
				new_interval = expected_zM - expected_zL;
				orig_start = expected_zL;
				new_start = expected_zL;
			}
			else if (nominal_zN <= z) // zN <= z < zM
			{
				orig_interval = nominal_zN - nominal_zM;
				new_interval = expected_zN - expected_zM;
				orig_start = nominal_zM;
				new_start = expected_zM;
			}
			else if(nominal_zO <= z) // zO <= z < zN
			{
				orig_interval = nominal_zO - nominal_zN;
				new_interval = expected_zO - expected_zN;
				orig_start = nominal_zN;
				new_start = expected_zN;
			}
			else if(nominal_zP <= z) // zP <= z < zO
			{
				orig_interval = nominal_zP - nominal_zO;
				new_interval = expected_zP - expected_zO;
				orig_start = nominal_zO;
				new_start = expected_zO;
			}
			else // zQ <= z < zP
			{
				orig_interval = expected_zQ - nominal_zP;
				new_interval = expected_zQ - expected_zP;
				orig_start = nominal_zP;
				new_start = expected_zP;
			}
		}
		frac_offset = (z - orig_start)/orig_interval;
		new_z = frac_offset*new_interval + new_start;
	}
	else
	{
		new_z = z;
	}
	coords.second = new_z;
}

void calibration::edge_stretch(pair<double, double> & coords)
{
	double x = coords.first;
	double z = coords.second;

	double x_out, z_out;
	double x_over, z_over;
	
	if( x < expected_x1) // left.
	{
		x_over = x - expected_x1;
		x_out = x_over*stretch_left + expected_x1;
	}
	else if( x > expected_x15) // right.
	{
		x_over = x - expected_x15; // this will be -.
		x_out = x_over*stretch_right + expected_x15;
	}
	else
	{
		x_out = x;
	}
	//
	if (expected_zC < z ) // top.
	{
		z_over = z - expected_zC;
		z_out = z_over*stretch_top + expected_zC;
	}
	else if( z < expected_zQ ) // bottom.
	{
		z_over = z - expected_zQ;  // this will be -.
		z_out = z_over*stretch_bottom + expected_zQ;
	}
	else
	{
		z_out = z;
	}
	
	coords.first  = x_out;
	coords.second = z_out;
}

void calibration::flip_z(pair<double, double> & coords)
{
	coords.second = -1.0*coords.second;
}
void calibration::flip_x(pair<double, double> & coords)
{
	coords.first = -1.0*coords.first;
}

pair<double, double> calibration::cal_0(double x1, double x2, double z1, double z2)
{
	// cal_0:  rescale, then center.  (Don't flip!)
	double x = rescale_x(x1, x2);
	double z = rescale_z(z1, z2);
	pair<double, double> coordinates = make_pair(x,z);
	coordinates = offset_xz(coordinates.first, coordinates.second);
	return coordinates;
}

pair<double, double> calibration::apply_calibration(double x1, double x2, double z1, double z2, int cal_level)
{
	// cal_0: rescale and center.
	pair<double, double> coordinates = cal_0(x1, x2, z1, z2);  
	if (cal_level >= 1)
		{ rotate(coordinates); }
	if (cal_level >= 2)
		{ cal_slide(coordinates); }
	if (cal_level >= 3)
		{ big_squeeze(coordinates); }
	if (cal_level >= 4)
		{ micro_squeeze_x(coordinates);
		  micro_squeeze_z(coordinates); }
	if (cal_level >= 5)
		{ edge_stretch(coordinates); }

	// Flip here, no matter what the cal level is.  ....unless it's run 9055.
	if(run_to_calibrate!=9055)
	{
		flip_z(coordinates);
	}

	if (cal_level >= 6)
	{ 
		if(run_to_calibrate==9055)
		{
			coordinates.first  -= mismatch_x_9055/2.0; 
			coordinates.second -= mismatch_z_9055/2.0;
		}
	}
		
	return coordinates;
}

//-- * -- //-- * -- //-- * -- //-- * -- //-- * -- //-- * -- //-- * -- //-- * -- //-- * -- //
#endif