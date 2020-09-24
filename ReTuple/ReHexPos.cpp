#define HAVE_ROOT 1

#include <vector>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;
using std::string;

#include <TTree.h>
#include <TFile.h>

int AddHex75PositionToTree(TTree *t, std::string calibration_directory);
int AddHex75PositionToTree_LE(TTree *t, std::string calibration_directory);

// Based on calibration data, must be 60 + n*90
// Based on off-center spot in polarized times, n must be even
double rotation_angle = 60.0;	// degrees
double to_ns = 0.1;             // ns/tick


class Hex75 
{
public:
	Hex75();			/* Default constructor */
	Hex75(std::string calibration_directory);
//	Hex75(bool have_tof);
//	void Init_with_tof(std::string calibration_directory);
//	void Init_without_tof(std::string calibration_directory);
	void Init(std::string calibration_directory);
	
	void LoadCalibrations(std::string dir);
	void LoadCalibrations_LE(std::string dir);
	//
	vector<int> GetXZ(vector<double> &x, vector<double> &z,
	     vector<double> u1, vector<double> u2,
	     vector<double> v1, vector<double> v2,
	     vector<double> w1, vector<double> w2,
	     vector<double> mcp, bool sumCut = false);
	int GetXZSingle(vector<double> &x, vector<double> &z,
	     double u1, double u2,
	     double v1, double v2,
	     double w1, double w2,
	     double mcp, bool sumCut = false);
	void GetXZfromUV(double &x, double &z, double u, double v);
	void GetXZfromUW(double &x, double &z, double u, double w);
	void GetXZfromVW(double &x, double &z, double v, double w);
	void GetXZfromUVW(double &x, double &z, double u, double v, double w);
	void GetXZfromUVW_avg(double &x, double &z, double u, double v, double w);
	bool SumCutU(double u) {return (u > usum_min && u < usum_max);}
	bool SumCutV(double v) {return (v > vsum_min && v < vsum_max);}
	bool SumCutW(double w) {return (w > wsum_min && w < wsum_max);}
	
private:
	double vu, vv, vw;	/* Appearing in equation 1.3 of MCP manual */
	double offset;		/* o in equation 1.3 of MCP manual */
	double ox, oz;		/* Offsets appearing in equation 1.4 */
	
//	void LoadCalibrations(std::string dir);  // public now.
	/* My calibration procedure variables */
	double x0uv, z0uv, mxuv, mzuv;
	double x0uw, z0uw, mxuw, mzuw;
	double x0vw, z0vw, mxvw, mzvw;

	double usum_min, usum_center, usum_max;
	double vsum_min, vsum_center, vsum_max;
	double wsum_min, wsum_center, wsum_max;
};

//#endif
//#include "Hex75.h"

void Rotate(double &x, double &z, double alpha_deg) 
{
	double xt = x;
	double zt = z;
	double alpha = alpha_deg*(2.*M_PI/360.0);
	//  cout << "(" << alpha << ") --> \t";
	x = xt*cos(alpha) - zt*sin(alpha);
	z = xt*sin(alpha) + zt*cos(alpha);
	//  cout << x << "\t" << z << " ---> ";
}

Hex75::Hex75() 
{
//	std::string temp;
//	std::string calibration_directory = ".";
//	cout << "Enter calibration_directory [" << calibration_directory << "]: ";
//	std::getline(std::cin, temp);
//	if (!temp.empty()) 
//	{
//		calibration_directory = temp;
//	}
	
	std::string calibration_directory = "/Users/anholm/Packages/MiscLibs/ReTuple/HexCal";
	Init(calibration_directory);
}
Hex75::Hex75(std::string calibration_directory) 
{
	Init(calibration_directory);
}

/*
Hex75::Hex75(bool have_tof)
{
	std::string calibration_directory = "/Users/anholm/Packages/MiscLibs/ReTuple/HexCal";
	if(have_tof)
	{
		Init_with_tof(calibration_directory);
	}
	else
	{
		Init_without_tof(calibration_directory);
	}
}
void Hex75::Init_with_tof(std::string calibration_directory) 
{
	vu = 1.0;
	vv = 1.0;
	vw = 1.0;
	offset = 0.0;			// To-be-calibrated

	// sept 2020 calibrations with LE and TOF adjust:
	usum_min    = 116.0;
	usum_center = 123.2;
	usum_max    = 137.0;
	
	vsum_min    = 116.0;
	vsum_center = 123.4;
	vsum_max    = 141.0;
	
	wsum_min    = 118.0;
	wsum_center = 123.7;
	wsum_max    = 140.0;
	
	LoadCalibrations(calibration_directory);
}
void Hex75::Init_without_tof(std::string calibration_directory) 
{
	vu = 1.0;
	vv = 1.0;
	vw = 1.0;
	offset = 0.0;			// To-be-calibrated

	// sept 2020 calibrations with LE and TOF adjust:
	usum_min    = 116.0;
	usum_center = 123.2;
	usum_max    = 137.0;
	
	vsum_min    = 116.0;
	vsum_center = 123.4;
	vsum_max    = 141.0;
	
	wsum_min    = 118.0;
	wsum_center = 123.7;
	wsum_max    = 140.0;
	
	LoadCalibrations(calibration_directory);
}
*/

void Hex75::Init(std::string calibration_directory) 
{
	vu = 1.0;
	vv = 1.0;
	vw = 1.0;
	offset = 0.0;			// To-be-calibrated

	// Main Jun2014 run
	//usum_min = 235.0;
	//usum_center = 244.67;
	//usum_max = 255.0;

	//vsum_min = 222.0;
	//vsum_center = 230.92;
	//vsum_max = 242.0;

	//wsum_min = 230.0;
	//wsum_center = 235.42;
	//wsum_max = 250.0;

	// May 2014 run
	// usum_min = 255.0;
	// usum_center = 244.67;
	// usum_max = 263.0;

	// vsum_min = 130.0;
	// vsum_center = 141.;
	// vsum_max = 150.0;

	// wsum_min = -20.0;
	// wsum_center = -15.;
	// wsum_max = 0.0;
  
	// Jan 2015 Calibrations
	// (addition by mja)
//	usum_min = 233.0;
//	usum_center = 238.5; // ?
//	usum_max = 243.0;
//	//
//	vsum_min = 218.0;
//	vsum_center = 222.3; // ?
//	vsum_max = 228.0;
//	//
//	wsum_min = 227.0;
//	wsum_center = 231.2; // ?
//	wsum_max = 239.0;
	
	// sept 2020 calibrations with LE and TOF adjust:
	usum_min    = 116.0;
	usum_center = 123.2;
	usum_max    = 137.0;
	
	vsum_min    = 116.0;
	vsum_center = 123.4;
	vsum_max    = 141.0;
	
	wsum_min    = 118.0;
	wsum_center = 123.7;
	wsum_max    = 140.0;
	
	LoadCalibrations(calibration_directory);
}

// These equations are modified from equation 1.4 of the Roent-Dek MCP
// manual.  They are modified to correspond with our coordinate
// system.  They require that the X1 corner of the HEX75 is oriented
// vertically.  Starting at this point and proceeding clock-wise, the
// order of the corners should be: X1, Y1, Z2, X2, Y2, Z1

// The calibration is done differently here than in the roentdek
// manual and I am not claiming it is done better.  I have chosen to
// calibrate each pair of planes independently to a simple linear fit
// with independent slopes, offsets for x, y in each plane.  This is 3
// pairs of 4-parameter fits for 12 total parameters.  The RD
// calibration has only 6 parameters.  It is important to note that
// even if the calibration is done with the RD parameters, the
// coordinates are not guaranteed to be the same for each pair of planes.  

// From a programming standpoint, I have left the stub of the RD
// calibration in place with the variables set to "uncalibrated."  To
// switch to this method, it should be enough to set the current
// calibration values to "uncalibrated" and derive appropriate values
// for the RD calibration.

// RD calibration variables: vu, vv, vw, offset, ox, oz
// My calibration variables:
// x0uv, z0uv, mxuv, mzuv, x0uw, z0uw, mxuw, mzuw, x0vw, z0vw, mxvw, mzvw

// Consolidated function to determine the best way to calculate
// position and then calculate the position and put the result in x, z
// Return value is how position was calculated (if calculated at all)

// To check if a position is determined: if (result != 0)
// To check if recovery of at least one signal from MCP is required: if (result < 0)
// To check if no recovery was needed: if (result > 0)
vector<int> Hex75::GetXZ(vector<double> &xv, vector<double> &zv,
			 vector<double> u1, vector<double> u2,
			 vector<double> v1, vector<double> v2,
			 vector<double> w1, vector<double> w2,
			 vector<double> mcp, bool doSumCut) 
{

	double u1d = -10, u2d = -10, v1d = -10, v2d = -10, w1d = -10, w2d = -10;
	vector<int> result(0, 0);
	//  printf("mcp size: %lu\n", mcp.size());
	if (mcp.size() == 0) return result;

	if (mcp.size() == 1) 
	{
		if (u1.size() > 0) u1d = u1[0];
		if (u2.size() > 0) u2d = u2[0];
		if (v1.size() > 0) v1d = v1[0];
		if (v2.size() > 0) v2d = v2[0];
		if (w1.size() > 0) w1d = w1[0];
		if (w2.size() > 0) w2d = w2[0];

		int r = GetXZSingle(xv, zv, u1d, u2d, v1d, v2d, w1d, w2d, mcp[0], doSumCut);
		if (r != 0) result.push_back(r);
		return result;
	}
	
	// If there are multpile hits, things got hairy quickly.  The
	// following is known to not be perfect.  It will ocassionally
	// choose less-than-optimal combinations of planes from which to
	// calculate position.  That being said, here's the basic things it
	// tries.  (Note that I say u, but v, w work identically)

	// 1) If tpp > tsum then ordering is unambiguous and no sum cut is
	// required.  If this is not the case, skip to step 5

	// 2) If u1 and u2 both have hits for every mcp hit, no reordering
	// is necessary

	// 3) If for every mcp hit, there is a u1 but u2 fired 0 times,
	// reconstruction is always possible

	// 4) If for every mcp hit, there is a u1 but u2 is missing
	// sometimes, use the tsum to determine which hit each u2 goes with
	// and reconstruct the rest

	// 5) If all this has failed use the tsum to match stuff by brute force

	// One known case where this falls is this:
	// Signal     TDC_times
	// MCP:	42744	50335	
	// U1:	51462	
	// U2:	51669	
	// V1:	
	// V2:	43823	51265	
	// W1:	43822	51828	
	// W2:	51194 

	// Here, the second hit clearly has both U1 and U2 so should use
	// this plane with either v or w reconstructed to give the position.
	// However, U fails 2-4 while V passes 3 and W passes 4 so the
	// algorithm uses V, W rather than U.  Note that this still
	// caclulates a (probably) correct position, its just not the
	// optimal choice of planes.

	// Some special cases of double hits can be reconstructed
	// These represent correctly ordered, arranged copies of u1, etc.
	vector<bool> can_reconstruct(mcp.size(), false);
	double tsum = (usum_center + vsum_center + wsum_center)/3.0;

	for (unsigned int i = 0; i < mcp.size()-1; i++) 
	{
		double tpp = mcp[i+1]-mcp[i];
		if (tpp > tsum) { can_reconstruct[i] = true; }
	}
	can_reconstruct[can_reconstruct.size()-1] = true;

	vector<double> u1v(mcp.size(), -10.0);
	vector<double> u2v(mcp.size(), -10.0);
	vector<double> v1v(mcp.size(), -10.0);
	vector<double> v2v(mcp.size(), -10.0);
	vector<double> w1v(mcp.size(), -10.0);
	vector<double> w2v(mcp.size(), -10.0);
	// If all signals are present, order is known
	if (u1.size() == mcp.size() && u2.size() == mcp.size()) 
	{
	u1v = u1;
	u2v = u2;
	}
	// If all signals are present, order is known
	if (u1.size() == mcp.size() && u2.size() == 0) 
	{
	u1v = u1;
	}
	if (u1.size() == 0 && u2.size() == mcp.size()) 
	{
	u2v = u2;
	}
	// If all signals are present in one direction, that order is known
	// and can be matched with an event in the other direction passing
	// tsum cut
	if (u1.size() == mcp.size() && u2.size() > 0 && u2.size() < mcp.size()) 
	{
		u1v = u1;
		for (unsigned int i1 = 0; i1 < u1.size(); i1++) 
		{
			for (unsigned int i2 = 0; i2 < u2.size(); i2++) 
			{
				double sum = (u1[i1] + u2[i2] - 2.0*mcp[i1])*to_ns;
				if (SumCutU(sum)) 
				{
					u2v[i1] = u2[i2];
				}
			}
		}
	}
	if (u2.size() == mcp.size() && u1.size() > 0 && u1.size() < mcp.size()) 
	{
		u2v = u2;
		for (unsigned int i2 = 0; i2 < u2.size(); i2++) 
		{
			for (unsigned int i1 = 0; i1 < u1.size(); i1++) 
			{
				double sum = (u1[i1] + u2[i2] - 2.0*mcp[i2])*to_ns;
				if (SumCutU(sum)) 
				{
					u1v[i2] = u1[i1];
				}
			}
		}
	}
	if (v1.size() == mcp.size() && v2.size() == mcp.size()) 
	{
		v1v = v1;
		v2v = v2;
	}
	if (v1.size() == mcp.size() && v2.size() == 0) 
	{
		v1v = v1;
	}
	if (v1.size() == 0 && v2.size() == mcp.size()) 
	{
		v2v = v2;
	}
	if (v1.size() == mcp.size() && v2.size() > 0 && v2.size() < mcp.size()) 
	{
		v1v = v1;
		for (unsigned int i1 = 0; i1 < v1.size(); i1++) 
		{
			for (unsigned int i2 = 0; i2 < v2.size(); i2++) 
			{
				double sum = (v1[i1] + v2[i2] - 2.0*mcp[i1])*to_ns;
				if (SumCutV(sum)) 
				{
					v2v[i1] = v2[i2];
				}
			}
		}
	}
	if (v2.size() == mcp.size() && v1.size() > 0 && v1.size() < mcp.size()) 
	{
		v2v = v2;
		for (unsigned int i2 = 0; i2 < v2.size(); i2++) 
		{
			for (unsigned int i1 = 0; i1 < v1.size(); i1++) 
			{
				double sum = (v1[i1] + v2[i2] - 2.0*mcp[i2])*to_ns;
				if (SumCutV(sum)) 
				{
					v1v[i2] = v1[i1];
				}
			}
		}
	}
	if (w1.size() == mcp.size() && w2.size() == mcp.size()) 
	{
		w1v = w1;
		w2v = w2;
	}
	if (w1.size() == mcp.size() && w2.size() == 0) 
	{
		w1v = w1;
	}
	if (w1.size() == 0 && w2.size() == mcp.size()) 
	{
		w2v = w2;
	}
	if (w1.size() == mcp.size() && w2.size() > 0 && w2.size() < mcp.size()) 
	{
		w1v = w1;
		for (unsigned int i1 = 0; i1 < w1.size(); i1++) 
		{
			for (unsigned int i2 = 0; i2 < w2.size(); i2++) 
			{
				double sum = (w1[i1] + w2[i2] - 2.0*mcp[i1])*to_ns;
				if (SumCutW(sum)) 
				{
					w2v[i1] = w2[i2];
				}
			}
		}
	}
	if (w2.size() == mcp.size() && w1.size() > 0 && w1.size() < mcp.size()) 
	{
		w2v = w2;
		for (unsigned int i2 = 0; i2 < w2.size(); i2++) 
		{
			for (unsigned int i1 = 0; i1 < w1.size(); i1++) 
			{
				double sum = (w1[i1] + w2[i2] - 2.0*mcp[i2])*to_ns;
				if (SumCutW(sum)) 
				{
					w1v[i2] = w1[i1];
				}
			}
		}
	}
	// Given the above, the correct ordering is only true if tpp > tsum
	// If passing this cut, requiring tsum is actuall unncecessary but
	// may be forced by calling function
	for (unsigned int i = 0; i < mcp.size(); i++) 
	{
		if (can_reconstruct[i]) 
		{
			int r = GetXZSingle(xv, zv, u1v[i], u2v[i], v1v[i], v2v[i], w1v[i], w2v[i], mcp[i], doSumCut);
			if (r != 0) result.push_back(r);
		}
	}
	if (result.size() > 0) return result;

	// Do not allow reconstruction for double hit events!  This is a
	// last-gasp effort to get some events.  Will always forbid
	// reconstruction and require tsum.  
	unsigned int ui = 0, vi = 0, wi = 0;
	for (unsigned long e = 0; e < mcp.size(); e++) 
	{
		//    printf("MCP %lu: (%g)\t", e, mcp[e]*to_ns);
		int nplanes = 0;
		u1d = -10.;
		u2d = -10.;
		v1d = -10.;
		v2d = -10.;
		w1d = -10.;
		w2d = -10.;
		bool uokay = false;
		bool vokay = false;
		bool wokay = false;
		for (unsigned int i1 = 0; i1 < u1.size(); i1++) 
		{
			for (unsigned int i2 = 0; i2 < u2.size(); i2++) 
			{
				if (!uokay) 
				{
					double sum = (u1[ui] + u2[ui] - 2.0*mcp[e])*to_ns;
					if (SumCutU(sum)) 
					{
						u1d = u1[i1];
						u2d = u2[i2];
						nplanes++;
						uokay = true;
						//            printf("\tFound a U: %d, %d", i1, i2);
					}
				}
			}
		}
		for (unsigned int i1 = 0; i1 < v1.size(); i1++) 
		{
			for (unsigned int i2 = 0; i2 < v2.size(); i2++) 
			{
				if (!vokay) 
				{
					double sum = (v1[vi] + v2[vi] - 2.0*mcp[e])*to_ns;
					if (SumCutV(sum)) 
					{
						v1d = v1[i1];
						v2d = v2[i2];
						nplanes++;
						vokay = true;
						//            printf("\tFound a V: %d, %d", i1, i2);
					}
				}
			}
		}
		for (unsigned int i1 = 0; i1 < w1.size(); i1++) 
		{
			for (unsigned int i2 = 0; i2 < w2.size(); i2++) 
			{
				if (!wokay) 
				{
					double sum = (w1[wi] + w2[wi] - 2.0*mcp[e])*to_ns;
					if (SumCutW(sum)) 
					{
						w1d = w1[i1];
						w2d = w2[i2];
						nplanes++;
						wokay = true;
						//            printf("\tFound a W: %d, %d", i1, i2);
					}
				}
			}
		}
		if (nplanes >= 2) 
		{
			//      printf("\tGetting a result...");
			int r = GetXZSingle(xv, zv, u1d, u2d, v1d, v2d, w1d, w2d, mcp[e], true);
			if (r != 0) result.push_back(r);
		}
		//    printf("\n");
	}
	// printf("MCP: %lu\t U: %lu, %lu\t V: %lu, %lu\t W: %lu, %lu\n", mcp.size(),
	//        u1.size(), u2.size(), v1.size(), v2.size(), w1.size(), w2.size());
	// for (unsigned long i = 0; i < xv.size(); i++) {
	//   printf("Hit %lu --> %g , %g\n", i, xv[i], zv[i]);
	// }
	// if (result != 0) {
	//   cout << "Last ditch result = " << result << endl;
	//   int j;
	//   cin >> j;
	// }
	return result;
}

int Hex75::GetXZSingle(vector<double> &xv, vector<double> &zv,
                 double u1, double u2,
                 double v1, double v2,
                 double w1, double w2,
                 double mcp, bool doSumCut) 
{
	const double ecode = -2000.0;

	double udif = ecode, vdif = ecode, wdif = ecode;
	double usum = ecode, vsum = ecode, wsum = ecode;

	double x, z;
	// Setup diff and sum, anything left as ecode will be empty
	if (u1 > 0 && u2 > 0) 
	{
		//    printf("U1: %g\t U2: %g\n", u1, u2);
		udif = (u2 - u1)*to_ns;
		if (mcp > 0) usum = (u2 + u1 - (2.0*mcp))*to_ns;
	}
	if (v1 > 0 && v2 > 0) 
	{
		vdif = (v2 - v1)*to_ns;
		if (mcp > 0) vsum = (v2 + v1 - (2.0*mcp))*to_ns;
	}
	if (w1 > 0 && w2 > 0) 
	{
		wdif = (w2 - w1)*to_ns;
		if (mcp > 0) wsum = (w2 + w1 - (2.0*mcp))*to_ns;
	}
	// If all 6 signals present, use them
	if (udif != ecode && vdif != ecode && wdif != ecode) 
	{
		if (!doSumCut || (SumCutU(usum) && SumCutV(vsum) && SumCutW(wsum))) 
		{
			GetXZfromUVW(x, z, udif, vdif, wdif);
			xv.push_back(x);
			zv.push_back(z);
			return 1;
		}
	}

	// If 2/3 pairs of signals are present, use those two
	if (udif != ecode && vdif != ecode) 
	{
		if (!doSumCut || (SumCutU(usum) && SumCutV(vsum))) 
		{
			GetXZfromUV(x, z, udif, vdif);
			xv.push_back(x);
			zv.push_back(z);
			return 2;
		}
	}
	if (udif != ecode && wdif != ecode) 
	{
		if (!doSumCut || (SumCutU(usum) && SumCutW(wsum))) 
		{
			GetXZfromUW(x, z, udif, wdif);
			xv.push_back(x);
			zv.push_back(z);
			return 2;
		}
	}
	if (vdif != ecode && wdif != ecode) 
	{
		if (!doSumCut || (SumCutV(vsum) && SumCutW(wsum))) 
		{
			GetXZfromVW(x, z, vdif, wdif);
			xv.push_back(x);
			zv.push_back(z);
			return 2;
		}
	}

	// Beyond this point, we will recover udif (and others) by assuming
	// the sum cut and using the MCP signal.

	// If mcp.size() is zero, there's no way this works
	if (mcp < 0) return 0;

	// If forcing sum cuts, give up here because there is no way to force it
	if (doSumCut) return 0;

	// Attempt to recover udif
	if (udif == ecode) 
	{
		if (u1 < 0 && u2 > 0) 
		{
			udif = (2.0*(u2-mcp))*to_ns - usum_center;
		} 
		else if (u1 > 0 && u2 < 0) 
		{
			udif = (2.0*(mcp-u1))*to_ns + usum_center;
		}
	}
	// Attempt to recover vdif
	if (vdif == ecode) 
	{
		if (v1 < 0 && v2 > 0) 
		{
			vdif = (2.0*(v2-mcp))*to_ns - vsum_center;
		} 
		else if (v1 > 0 && v2 < 0) 
		{
			vdif = (2.0*(mcp-v1))*to_ns + vsum_center;
		}
	}
	// Attempt to recover wdif
	if (wdif == ecode) 
	{
		if (w1 < 0 && w2 > 0) 
		{
			wdif = (2.0*(w2-mcp))*to_ns - wsum_center;
		} 
		else if (w1 > 0 && w2 < 0) 
		{
			wdif = (2.0*(mcp-w1))*to_ns + wsum_center;
		}
	}
	
	// Now, again use the most possible information.  No way to do sum
	// cuts.  Note that at this point, at least one side has been
	// reconstructed.
	if (udif != ecode && vdif != ecode && wdif != ecode) 
	{
		if (u1 > 0 && u2 > 0) 
		{
			// U has not been reconstructed.  Therefore, neither v nor w was
			// also reconstructed or we would have not needed to do any
			// recovery
			if (fabs(vdif) < fabs(wdif)) 
			{
				GetXZfromUV(x, z, udif, vdif);
			} 
			else 
			{
				GetXZfromUW(x, z, udif, wdif);
			}
			xv.push_back(x);
			zv.push_back(z);
			return -1;
		}
		if (v1 > 0 && v2 > 0) 
		{
			// V has not been reconstructed, U, W have
			if (fabs(udif) < fabs(wdif)) 
			{
				GetXZfromUV(x, z, udif, vdif);
			} 
			else 
			{
				GetXZfromVW(x, z, vdif, wdif);
			}
			xv.push_back(x);
			zv.push_back(z);
			return -1;
		}
		if (w1 > 0 && w2 > 0) 
		{
			// W has not been reconstructed, U, V have
			if (fabs(udif) < fabs(vdif)) 
			{
				GetXZfromUW(x, z, udif, wdif);
			} 
			else 
			{
				GetXZfromVW(x, z, vdif, wdif);
			}
			xv.push_back(x);
			zv.push_back(z);
			return -1;
		}
		// Else, all three (U, V, W) have been reconstructed
		GetXZfromUVW(x, z, udif, vdif, wdif);
		xv.push_back(x);
		zv.push_back(z);
		return -1;
	}

	if (udif != ecode && vdif != ecode) 
	{
		GetXZfromUV(x, z, udif, vdif);
		xv.push_back(x);
		zv.push_back(z);
		return -2;
	}
	if (udif != ecode && wdif != ecode) 
	{
		GetXZfromUW(x, z, udif, wdif);
		xv.push_back(x);
		zv.push_back(z);    
		return -2;
	}
	if (vdif != ecode && wdif != ecode) 
	{
		GetXZfromVW(x, z, vdif, wdif);
		xv.push_back(x);
		zv.push_back(z);      
		return -2;
	}
	// Out of ways to calculate the position, give up.
	return 0;                             // aka false
}

void Hex75::GetXZfromUV(double &x, double &z, double u, double v) 
{
	// Here u,v are raw time differences and so first convert them to
	// the u, v of the MCP manual
	// *** WHAT DO I DO ABOUT THE DELTA_T??? *************************************
	double up = u * vu;
	double vp = v * vv;

	// Modified Equation 1.4 - Guessing rotation in test chamber
	x = ((1.0/sqrt(3.0)) * (up+vp)) + ox;
	z = up - vp + oz;

	Rotate(x, z, rotation_angle);

	x =  x0uv + (mxuv*x);
	z =  z0uv + (mzuv*z);
	//  printf("Calculating UV, got %g, %g\n", x, z);
}

void Hex75::GetXZfromUW(double &x, double &z, double u, double w) 
{
	// Here u,w are raw time differences and so first convert them to
	// the u, w of the MCP manual
	// *** WHAT DO I DO ABOUT THE DELTA_T??? *************************************
	double up = u * vu;
	double wp = (w * vw) + offset;

	// Modified Equation 1.4 - Guessing rotation in test chamber
	x = ((1.0/sqrt(3.0))*(2.0*up + wp)) + ox;
	z = -1.0*wp + oz;

	Rotate(x, z, rotation_angle);              

	x = x0uw + (mxuw*x);
	z = z0uw + (mzuw*z);
	//  printf("Calculating UW, got %g, %g\n", x, z);
}

void Hex75::GetXZfromVW(double &x, double &z, double v, double w) 
{
	// Here v, w are raw time differences and so first convert them to
	// the v, w of the MCP manual
	// *** WHAT DO I DO ABOUT THE DELTA_T??? *************************************
	double vp = v * vv;
	double wp = (w  * vw) + offset;

	// Modified Equation 1.4 - Guessing rotation in test chamber
	x = ((-1.0/sqrt(3.0))*(wp - 2.0*vp)) + ox;
	z = -1.0*wp + oz;

	Rotate(x, z, rotation_angle);

	x = x0vw + (mxvw*x);
	z = z0vw + (mzvw*z);
	//  printf("Calculating VW, got %g, %g\n", x, z);
}

void Hex75::GetXZfromUVW(double &x, double &z, double u, double v, double w) 
{
	double up = u * vu;
	double vp = v * vv;
	double wp = (w * vw) + offset;

	// Use the two most central DLs.  That means, discard the delay line
	// with the largest difference.
	if (up >= vp && up >= wp)      // Discard u, use v, w
	{
		GetXZfromVW(x, z, v, w);
	}
	else if (vp >= up && vp >= wp) // Discard v, use u, w
	{
		GetXZfromUW(x, z, u, w);
	} 
	else if (wp >= up && wp >= vp) // Discard w, use u, v
	{
		GetXZfromUV(x, z, u, v);
	}
	else 
	{
		cout << "ERROR.  Impossible logic in Hex75::GetXZfromUVW" << endl;
		x = 90.;
		z = 90.;
	}
}

void Hex75::GetXZfromUVW_avg(double &x, double &z, double u, double v, double w) 
{
	double x_avg = 0, z_avg = 0;
	double xt, zt;

	GetXZfromUV(xt, zt, u, v);
	x_avg += xt;
	z_avg += zt;
	GetXZfromUW(xt, zt, u, w);
	x_avg += xt;
	z_avg += zt;
	GetXZfromVW(xt, zt, v, w);
	x_avg += xt;
	z_avg += zt;

	x_avg /= 3.0;
	z_avg /= 3.0;
	x = x_avg;
	z = z_avg;
}

void Hex75::LoadCalibrations(string dir) 
{
	// x0uv and z0uv (etc) are linear offsets -- get the whole damn image centered.  these probably will be different with the LE.
	// mxuv and mzuv (etc) is proportional to the rotation of the DLA wrt the axis we care about, or something.  it might very well still be the same.
	
	FILE *ifile;
	char fname[100];
	int n;

	cout << "Loading HEX75 calibrations from " << dir << endl << endl;
	sprintf(fname, "%s/uv.calib", dir.c_str());

	ifile = fopen(fname, "r");
	if (ifile == NULL) { cout << "failed to open file" << endl; }
	n = fscanf(ifile, "%lf\t%lf\t%lf\t%lf", &x0uv, &mxuv, &z0uv, &mzuv);
	if (n != 4) 
	{
		cout << "HEX75 Could not read " << fname << endl;
		//    exit(1);
	}
	fclose(ifile);

	sprintf(fname, "%s/uw.calib", dir.c_str());
	ifile = fopen(fname, "r");
	n = fscanf(ifile, "%lf\t%lf\t%lf\t%lf", &x0uw, &mxuw, &z0uw, &mzuw);
	if (n != 4) 
	{
		cout << "HEX75 Could not read " << fname << endl;
		//    exit(1);
	}
	fclose(ifile);

	sprintf(fname, "%s/vw.calib", dir.c_str());
	ifile = fopen(fname, "r");
	n = fscanf(ifile, "%lf\t%lf\t%lf\t%lf", &x0vw, &mxvw, &z0vw, &mzvw);
	if (n != 4) 
	{
		cout << "HEX75 Could not read " << fname << endl;
		//    exit(1);
	}
	fclose(ifile);


	// cout << x0uv << "\t" << mxuv << "\t" << z0uv << "\t" << mzuv << endl;
	// cout << x0uw << "\t" << mxuw << "\t" << z0uw << "\t" << mzuw << endl;
	// cout << x0vw << "\t" << mxvw << "\t" << z0vw << "\t" << mzvw << endl;
}

//#if defined (HAVE_ROOT) || defined(__CINT__)
int AddHex75PositionToTree(TTree *t, std::string calibration_directory) 
{
	string xName = "HEX75_XPos";
	string zName = "HEX75_ZPos";
	string cName = "HEX75_PosCode";
	if (t -> GetBranch(xName.c_str()) || t -> GetBranch(zName.c_str()) || t -> GetBranch(cName.c_str()) ) 
	{
		printf("ALREADY HAVE HEX75 POSITION INFORMATION. No action taken\n");
		return 1;
	}
	printf("Adding positions to the TTree.....");
	vector<double> xPos;
	vector<double> zPos;
	vector<int> rcode;
	TBranch *xb = t -> Branch(xName.c_str(), &xPos);
	TBranch *zb = t -> Branch(zName.c_str(), &zPos);
	TBranch *cb = t -> Branch(cName.c_str(), &rcode);

	vector<double> *u1 = 0;
	vector<double> *u2 = 0;
	vector<double> *v1 = 0;
	vector<double> *v2 = 0;
	vector<double> *w1 = 0;
	vector<double> *w2 = 0;
	vector<double> *mcp = 0;
	t -> SetBranchAddress("TDC_HEX75_U1", &u1);
	t -> SetBranchAddress("TDC_HEX75_U2", &u2);
	t -> SetBranchAddress("TDC_HEX75_V1", &v1);
	t -> SetBranchAddress("TDC_HEX75_V2", &v2);
	t -> SetBranchAddress("TDC_HEX75_W1", &w1);
	t -> SetBranchAddress("TDC_HEX75_W2", &w2);
	t -> SetBranchAddress("TDC_ELECTRON_MCP", &mcp);

	Hex75 *hex = new Hex75(calibration_directory);

	for (unsigned int i = 0; i < t -> GetEntries(); i++) 
	{
		xPos.clear();
		zPos.clear();
		rcode.clear();
		t -> GetEntry(i);

		rcode = hex -> GetXZ(xPos, zPos, *u1, *u2, *v1, *v2, *w1, *w2, *mcp, false);
		//    cout << xPos.size() << "\t" << zPos.size() << "\t" << rcode.size() << endl;
		xb -> Fill();
		zb -> Fill();
		cb -> Fill();
	}
	printf("Done!\n");
	return 0;
}


void Hex75::LoadCalibrations_LE(string dir) 
{
	FILE *ifile;
	char fname[100];
	int n;

	cout << "Loading HEX75 calibrations from " << dir << endl << endl;
	sprintf(fname, "%s/uv.calib", dir.c_str());

	ifile = fopen(fname, "r");
	if (ifile == NULL) { cout << "failed to open file" << endl; }
	n = fscanf(ifile, "%lf\t%lf\t%lf\t%lf", &x0uv, &mxuv, &z0uv, &mzuv);
	if (n != 4) 
	{
		cout << "HEX75 Could not read " << fname << endl;
		//    exit(1);
	}
	fclose(ifile);

	sprintf(fname, "%s/uw.calib", dir.c_str());
	ifile = fopen(fname, "r");
	n = fscanf(ifile, "%lf\t%lf\t%lf\t%lf", &x0uw, &mxuw, &z0uw, &mzuw);
	if (n != 4) 
	{
		cout << "HEX75 Could not read " << fname << endl;
		//    exit(1);
	}
	fclose(ifile);

	sprintf(fname, "%s/vw.calib", dir.c_str());
	ifile = fopen(fname, "r");
	n = fscanf(ifile, "%lf\t%lf\t%lf\t%lf", &x0vw, &mxvw, &z0vw, &mzvw);
	if (n != 4) 
	{
		cout << "HEX75 Could not read " << fname << endl;
		//    exit(1);
	}
	fclose(ifile);


	// cout << x0uv << "\t" << mxuv << "\t" << z0uv << "\t" << mzuv << endl;
	// cout << x0uw << "\t" << mxuw << "\t" << z0uw << "\t" << mzuw << endl;
	// cout << x0vw << "\t" << mxvw << "\t" << z0vw << "\t" << mzvw << endl;
}

int AddHex75PositionToTree_LE(TTree *t, std::string calibration_directory) 
{
	string xName = "HEX75_XPos_LE";
	string zName = "HEX75_ZPos_LE";
	string cName = "HEX75_PosCode_LE";
	if (t -> GetBranch(xName.c_str()) || t -> GetBranch(zName.c_str()) || t -> GetBranch(cName.c_str()) ) 
	{
		printf("ALREADY HAVE HEX75 POSITION INFORMATION. No action taken\n");
		return 1;
	}
	printf("Adding positions to the TTree.....");
	vector<double> xPos;
	vector<double> zPos;
	vector<int> rcode;
	TBranch *xb = t -> Branch(xName.c_str(), &xPos);
	TBranch *zb = t -> Branch(zName.c_str(), &zPos);
	TBranch *cb = t -> Branch(cName.c_str(), &rcode);

	vector<double> *u1 = 0;
	vector<double> *u2 = 0;
	vector<double> *v1 = 0;
	vector<double> *v2 = 0;
	vector<double> *w1 = 0;
	vector<double> *w2 = 0;
	vector<double> *mcp = 0;
	t -> SetBranchAddress("TDC_HEX75_U1_LE", &u1);
	t -> SetBranchAddress("TDC_HEX75_U2_LE", &u2);
	t -> SetBranchAddress("TDC_HEX75_V1_LE", &v1);
	t -> SetBranchAddress("TDC_HEX75_V2_LE", &v2);
	t -> SetBranchAddress("TDC_HEX75_W1_LE", &w1);
	t -> SetBranchAddress("TDC_HEX75_W2_LE", &w2);
	t -> SetBranchAddress("TDC_ELECTRON_MCP_LE", &mcp);

	Hex75 *hex = new Hex75(calibration_directory);

	for (unsigned int i = 0; i < t -> GetEntries(); i++) 
	{
		xPos.clear();
		zPos.clear();
		rcode.clear();
		t -> GetEntry(i);

		rcode = hex -> GetXZ(xPos, zPos, *u1, *u2, *v1, *v2, *w1, *w2, *mcp, false);
		//    cout << xPos.size() << "\t" << zPos.size() << "\t" << rcode.size() << endl;
		xb -> Fill();
		zb -> Fill();
		cb -> Fill();
	}
	printf("Done!\n");
	return 0;
}


/*
void ProcessListHex75(string fname, string ntuple_directory, string calibration_directory) 
{
	std::ifstream input;
	std::ofstream fail;
	fail.open("redo.txt", std::ofstream::out);
	input.open(fname.c_str());
	int run;
	if (ntuple_directory[ntuple_directory.size() -1] == '/') 
	{
		ntuple_directory = ntuple_directory.substr(0, ntuple_directory.size() - 1);
	}
	while (input >> run) 
	{
		char rfile[200];
		snprintf(rfile, sizeof(rfile), "%s/output%05d.root", ntuple_directory.c_str(), run);
		TFile *f = new TFile(rfile, "UPDATE");
		TTree *t = (TTree*)f->Get("ntuple");
		int status = AddHex75PositionToTree(t, calibration_directory);
		if (status != 0) 
		{
			cout << "Run " << run << " failed with exit code " << status << endl;
			fail << run << "\t" << status << endl;
		}
		t -> Write(0, TObject::kWriteDelete);
		f -> Close();
		delete f;
	}
	input.close();
	fail.close();
}
*/

//#endif
