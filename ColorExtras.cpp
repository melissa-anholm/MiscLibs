#ifndef INCLUDE_COLOREXTRAS
#define INCLUDE_COLOREXTRAS 1


//#include <vector>
#include "TColor.h"
#include "Riostream.h"  // defines cout, etc.
#include <TStyle.h>  // includes "gStyle".
//#include "TPaletteAxis.h"

using std::string;

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
/*
enum EColorPalette {kImprovedRainbow=1,   kTropicalBird=2,
                    kDeepSea=51,          kGreyScale=52,    kDarkBodyRadiator=53,
                    kBlueYellow= 54,      kRainBow=55,      kInvertedDarkBodyRadiator=56,
                    kBird=57,             kCubehelix=58,    kGreenRedViolet=59,
                    kBlueRedYellow=60,    kOcean=61,        kColorPrintableOnGrey=62,
                    kAlpine=63,           kAquamarine=64,   kArmy=65,
                    kAtlantic=66,         kAurora=67,       kAvocado=68,
                    kBeach=69,            kBlackBody=70,    kBlueGreenYellow=71,
                    kBrownCyan=72,        kCMYK=73,         kCandy=74,
                    kCherry=75,           kCoffee=76,       kDarkRainBow=77,
                    kDarkTerrain=78,      kFall=79,         kFruitPunch=80,
                    kFuchsia=81,          kGreyYellow=82,   kGreenBrownTerrain=83,
                    kGreenPink=84,        kIsland=85,       kLake=86,
                    kLightTemperature=87, kLightTerrain=88, kMint=89,
                    kNeon=90,             kPastel=91,       kPearl=92,
                    kPigeon=93,           kPlum=94,         kRedBlue=95,
                    kRose=96,             kRust=97,         kSandyTerrain=98,
                    kSienna=99,           kSolar=100,       kSouthWest=101,
                    kStarryNight=102,     kSunset=103,      kTemperatureMap=104,
                    kThermometer=105,     kValentine=106,   kVisibleSpectrum=107,
                    kWaterMelon=108,      kCool=109,        kCopper=110,
                    kGistEarth=111,       kViridis=112};
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void palette_inversion(int N_stops, Double_t stops[], Double_t red[], Double_t green[], Double_t blue[], int N_contours, double alpha)
{
	Double_t rev_red[9]   = {red[8], red[7], red[6], red[5], red[4], red[3], red[2], red[1], red[0]};
	Double_t rev_green[9] = {green[8], green[7], green[6], green[5], green[4], green[3], green[2], green[1], green[0]};
	Double_t rev_blue[9]  = {blue[8], blue[7], blue[6], blue[5], blue[4], blue[3], blue[2], blue[1], blue[0]};
	
	TColor::CreateGradientColorTable(N_stops, stops, rev_red, rev_green, rev_blue, N_contours, alpha);
}

void use_hq_palette(int palettenumber, bool invert=false)
{
	double alpha=1;
	Double_t stops[9] = {0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
	switch(palettenumber)
	{
		// kImprovedRainbow:  Brighter, Darker Rainbow.
		case 1:
		{
			const Int_t NRGBs = 5;
			Double_t stops_t[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
			
			Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
			Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
			Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
			TColor::CreateGradientColorTable(NRGBs, stops_t, red, green, blue, 255, alpha);
			if(invert==true) 
			{ 
				Double_t rev_red[5]   = {red[4], red[3], red[2], red[1], red[0]};
				Double_t rev_green[5] = {green[4], green[3], green[2], green[1], green[0]};
				Double_t rev_blue[5]  = {blue[4], blue[3], blue[2], blue[1], blue[0]};
				TColor::CreateGradientColorTable(NRGBs, stops_t, rev_red, rev_green, rev_blue, 255, alpha);
			}
			break;
		}
		// kTropicalBird
		case 2:
		{ 
			const Int_t NRGBs = 3;
			Double_t stops_t[NRGBs] = { 0.00, 0.50, 1.00 };

			Double_t Red[NRGBs]    = { 1.00, 0.00, 0.00};
			Double_t Green[NRGBs]  = { 0.00, 1.00, 0.00};
			Double_t Blue[NRGBs]   = { 1.00, 0.00, 1.00};
			Int_t nb=50;
			TColor::CreateGradientColorTable(NRGBs,stops_t,Red,Green,Blue,nb, alpha);
			if(invert==true) 
			{ 
				Double_t rev_red[3]   = {Red[2], Red[1], Red[0]};
				Double_t rev_green[3] = {Green[2], Green[1], Green[0]};
				Double_t rev_blue[3]  = {Blue[2], Blue[1], Blue[0]};
				TColor::CreateGradientColorTable(NRGBs, stops_t, rev_red, rev_green, rev_blue, nb, alpha);
			}
		}
		// kDeepSea
		case 51:
		{
			Double_t red[9]   = {  0./255.,  9./255., 13./255., 17./255., 24./255.,  32./255.,  27./255.,  25./255.,  29./255.};
			Double_t green[9] = {  0./255.,  0./255.,  0./255.,  2./255., 37./255.,  74./255., 113./255., 160./255., 221./255.};
			Double_t blue[9]  = { 28./255., 42./255., 59./255., 78./255., 98./255., 129./255., 154./255., 184./255., 221./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kGreyScale
		case 52:
		{
			Double_t red[9]   = { 0./255., 32./255., 64./255., 96./255., 128./255., 160./255., 192./255., 224./255., 255./255.};
			Double_t green[9] = { 0./255., 32./255., 64./255., 96./255., 128./255., 160./255., 192./255., 224./255., 255./255.};
			Double_t blue[9]  = { 0./255., 32./255., 64./255., 96./255., 128./255., 160./255., 192./255., 224./255., 255./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kDarkBodyRadiator
		case 53:
		{
			Double_t red[9]   = { 0./255., 45./255., 99./255., 156./255., 212./255., 230./255., 237./255., 234./255., 242./255.};
			Double_t green[9] = { 0./255.,  0./255.,  0./255.,  45./255., 101./255., 168./255., 238./255., 238./255., 243./255.};
			Double_t blue[9]  = { 0./255.,  1./255.,  1./255.,   3./255.,   9./255.,   8./255.,  11./255.,  95./255., 230./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kBlueYellow:  Two-color hue (dark blue through neutral gray to bright yellow)
		case 54:
		{
			Double_t red[9]   = {  0./255.,  22./255., 44./255., 68./255., 93./255., 124./255., 160./255., 192./255., 237./255.};
			Double_t green[9] = {  0./255.,  16./255., 41./255., 67./255., 93./255., 125./255., 162./255., 194./255., 241./255.};
			Double_t blue[9]  = { 97./255., 100./255., 99./255., 99./255., 93./255.,  68./255.,  44./255.,  26./255.,  74./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kRainBow
		case 55:
		{
			Double_t red[9]   = {  0./255.,   5./255.,  15./255.,  35./255., 102./255., 196./255., 208./255., 199./255., 110./255.};
			Double_t green[9] = {  0./255.,  48./255., 124./255., 192./255., 206./255., 226./255.,  97./255.,  16./255.,   0./255.};
			Double_t blue[9]  = { 99./255., 142./255., 198./255., 201./255.,  90./255.,  22./255.,  13./255.,   8./255.,   2./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kInvertedDarkBodyRadiator
		case 56:
		{
			Double_t red[9]   = { 242./255., 234./255., 237./255., 230./255., 212./255., 156./255., 99./255., 45./255., 0./255.};
			Double_t green[9] = { 243./255., 238./255., 238./255., 168./255., 101./255.,  45./255.,  0./255.,  0./255., 0./255.};
			Double_t blue[9]  = { 230./255.,  95./255.,  11./255.,   8./255.,   9./255.,   3./255.,  1./255.,  1./255., 0./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kBird
		case 57:
		{
			Double_t red[9]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
			Double_t green[9] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
			Double_t blue[9]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kCubehelix
		case 58:
		{
			Double_t red[9]   = { 0.0000, 0.0956, 0.0098, 0.2124, 0.6905, 0.9242, 0.7914, 0.7596, 1.0000};
			Double_t green[9] = { 0.0000, 0.1147, 0.3616, 0.5041, 0.4577, 0.4691, 0.6905, 0.9237, 1.0000};
			Double_t blue[9]  = { 0.0000, 0.2669, 0.3121, 0.1318, 0.2236, 0.6741, 0.9882, 0.9593, 1.0000};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kGreenRedViolet
		case 59:
		{
			Double_t red[9]   = {13./255., 23./255., 25./255., 63./255., 76./255., 104./255., 137./255., 161./255., 206./255.};
			Double_t green[9] = {95./255., 67./255., 37./255., 21./255.,  0./255.,  12./255.,  35./255.,  52./255.,  79./255.};
			Double_t blue[9]  = { 4./255.,  3./255.,  2./255.,  6./255., 11./255.,  22./255.,  49./255.,  98./255., 208./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kBlueRedYellow
		case 60:
		{
			Double_t red[9]   = {0./255.,  61./255.,  89./255., 122./255., 143./255., 160./255., 185./255., 204./255., 231./255.};
			Double_t green[9] = {0./255.,   0./255.,   0./255.,   0./255.,  14./255.,  37./255.,  72./255., 132./255., 235./255.};
			Double_t blue[9]  = {0./255., 140./255., 224./255., 144./255.,   4./255.,   5./255.,   6./255.,   9./255.,  13./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kOcean
		case 61:
		{
			Double_t red[9]   = { 14./255.,  7./255.,  2./255.,  0./255.,  5./255.,  11./255.,  55./255., 131./255., 229./255.};
			Double_t green[9] = {105./255., 56./255., 26./255.,  1./255., 42./255.,  74./255., 131./255., 171./255., 229./255.};
			Double_t blue[9]  = {  2./255., 21./255., 35./255., 60./255., 92./255., 113./255., 160./255., 185./255., 229./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kColorPrintableOnGrey
		case 62:
		{
			Double_t red[9]   = { 0./255.,   0./255.,   0./255.,  70./255., 148./255., 231./255., 235./255., 237./255., 244./255.};
			Double_t green[9] = { 0./255.,   0./255.,   0./255.,   0./255.,   0./255.,  69./255.,  67./255., 216./255., 244./255.};
			Double_t blue[9]  = { 0./255., 102./255., 228./255., 231./255., 177./255., 124./255., 137./255.,  20./255., 244./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kAlpine
		case 63:
		{
			Double_t red[9]   = { 50./255., 56./255., 63./255., 68./255.,  93./255., 121./255., 165./255., 192./255., 241./255.};
			Double_t green[9] = { 66./255., 81./255., 91./255., 96./255., 111./255., 128./255., 155./255., 189./255., 241./255.};
			Double_t blue[9]  = { 97./255., 91./255., 75./255., 65./255.,  77./255., 103./255., 143./255., 167./255., 217./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kAquamarine
		case 64:
		{
			Double_t red[9]   = { 145./255., 166./255., 167./255., 156./255., 131./255., 114./255., 101./255., 112./255., 132./255.};
			Double_t green[9] = { 158./255., 178./255., 179./255., 181./255., 163./255., 154./255., 144./255., 152./255., 159./255.};
			Double_t blue[9]  = { 190./255., 199./255., 201./255., 192./255., 176./255., 169./255., 160./255., 166./255., 190./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kArmy
		case 65:
		{
			Double_t red[9]   = { 93./255.,   91./255.,  99./255., 108./255., 130./255., 125./255., 132./255., 155./255., 174./255.};
			Double_t green[9] = { 126./255., 124./255., 128./255., 129./255., 131./255., 121./255., 119./255., 153./255., 173./255.};
			Double_t blue[9]  = { 103./255.,  94./255.,  87./255.,  85./255.,  80./255.,  85./255., 107./255., 120./255., 146./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kAtlantic
		case 66:
		{
			Double_t red[9]   = { 24./255., 40./255., 69./255.,  90./255., 104./255., 114./255., 120./255., 132./255., 103./255.};
			Double_t green[9] = { 29./255., 52./255., 94./255., 127./255., 150./255., 162./255., 159./255., 151./255., 101./255.};
			Double_t blue[9]  = { 29./255., 52./255., 96./255., 132./255., 162./255., 181./255., 184./255., 186./255., 131./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kAurora
		case 67:
		{
			Double_t red[9]   = { 46./255., 38./255., 61./255., 92./255., 113./255., 121./255., 132./255., 150./255., 191./255.};
			Double_t green[9] = { 46./255., 36./255., 40./255., 69./255., 110./255., 135./255., 131./255.,  92./255.,  34./255.};
			Double_t blue[9]  = { 46./255., 80./255., 74./255., 70./255.,  81./255., 105./255., 165./255., 211./255., 225./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kAvocado
		case 68:
		{
			Double_t red[9]   = { 0./255.,  4./255., 12./255.,  30./255.,  52./255., 101./255., 142./255., 190./255., 237./255.};
			Double_t green[9] = { 0./255., 40./255., 86./255., 121./255., 140./255., 172./255., 187./255., 213./255., 240./255.};
			Double_t blue[9]  = { 0./255.,  9./255., 14./255.,  18./255.,  21./255.,  23./255.,  27./255.,  35./255., 101./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kBeach
		case 69:
		{
			Double_t red[9]   = { 198./255., 206./255., 206./255., 211./255., 198./255., 181./255., 161./255., 171./255., 244./255.};
			Double_t green[9] = { 103./255., 133./255., 150./255., 172./255., 178./255., 174./255., 163./255., 175./255., 244./255.};
			Double_t blue[9]  = {  49./255.,  54./255.,  55./255.,  66./255.,  91./255., 130./255., 184./255., 224./255., 244./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kBlackBody
		case 70:
		{
			Double_t red[9]   = { 243./255., 243./255., 240./255., 240./255., 241./255., 239./255., 186./255., 151./255., 129./255.};
			Double_t green[9] = {   0./255.,  46./255.,  99./255., 149./255., 194./255., 220./255., 183./255., 166./255., 147./255.};
			Double_t blue[9]  = {   6./255.,   8./255.,  36./255.,  91./255., 169./255., 235./255., 246./255., 240./255., 233./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kBlueGreenYellow
		case 71:
		{
			Double_t red[9]   = { 22./255., 19./255.,  19./255.,  25./255.,  35./255.,  53./255.,  88./255., 139./255., 210./255.};
			Double_t green[9] = {  0./255., 32./255.,  69./255., 108./255., 135./255., 159./255., 183./255., 198./255., 215./255.};
			Double_t blue[9]  = { 77./255., 96./255., 110./255., 116./255., 110./255., 100./255.,  90./255.,  78./255.,  70./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kBrownCyan
		case 72:
		{
			Double_t red[9]   = { 68./255., 116./255., 165./255., 182./255., 189./255., 180./255., 145./255., 111./255.,  71./255.};
			Double_t green[9] = { 37./255.,  82./255., 135./255., 178./255., 204./255., 225./255., 221./255., 202./255., 147./255.};
			Double_t blue[9]  = { 16./255.,  55./255., 105./255., 147./255., 196./255., 226./255., 232./255., 224./255., 178./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kCMYK
		case 73:
		{
			Double_t red[9]   = {  61./255.,  99./255., 136./255., 181./255., 213./255., 225./255., 198./255., 136./255., 24./255.};
			Double_t green[9] = { 149./255., 140./255.,  96./255.,  83./255., 132./255., 178./255., 190./255., 135./255., 22./255.};
			Double_t blue[9]  = { 214./255., 203./255., 168./255., 135./255., 110./255., 100./255., 111./255., 113./255., 22./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kCandy
		case 74:
		{
			Double_t red[9]   = { 76./255., 120./255., 156./255., 183./255., 197./255., 180./255., 162./255., 154./255., 140./255.};
			Double_t green[9] = { 34./255.,  35./255.,  42./255.,  69./255., 102./255., 137./255., 164./255., 188./255., 197./255.};
			Double_t blue[9]  = { 64./255.,  69./255.,  78./255., 105./255., 142./255., 177./255., 205./255., 217./255., 198./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kCherry
		case 75:
		{
			Double_t red[9]   = { 37./255., 102./255., 157./255., 188./255., 196./255., 214./255., 223./255., 235./255., 251./255.};
			Double_t green[9] = { 37./255.,  29./255.,  25./255.,  37./255.,  67./255.,  91./255., 132./255., 185./255., 251./255.};
			Double_t blue[9]  = { 37./255.,  32./255.,  33./255.,  45./255.,  66./255.,  98./255., 137./255., 187./255., 251./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kCoffee
		case 76:
		{
			Double_t red[9]   = { 79./255., 100./255., 119./255., 137./255., 153./255., 172./255., 192./255., 205./255., 250./255.};
			Double_t green[9] = { 63./255.,  79./255.,  93./255., 103./255., 115./255., 135./255., 167./255., 196./255., 250./255.};
			Double_t blue[9]  = { 51./255.,  59./255.,  66./255.,  61./255.,  62./255.,  70./255., 110./255., 160./255., 250./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kDarkRainBow
		case 77:
		{
			Double_t red[9]   = {  43./255.,  44./255., 50./255.,  66./255., 125./255., 172./255., 178./255., 155./255., 157./255.};
			Double_t green[9] = {  63./255.,  63./255., 85./255., 101./255., 138./255., 163./255., 122./255.,  51./255.,  39./255.};
			Double_t blue[9]  = { 121./255., 101./255., 58./255.,  44./255.,  47./255.,  55./255.,  57./255.,  44./255.,  43./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kDarkTerrain
		case 78:
		{
			Double_t red[9]   = {  0./255., 41./255., 62./255., 79./255., 90./255., 87./255., 99./255., 140./255., 228./255.};
			Double_t green[9] = {  0./255., 57./255., 81./255., 93./255., 85./255., 70./255., 71./255., 125./255., 228./255.};
			Double_t blue[9]  = { 95./255., 91./255., 91./255., 82./255., 60./255., 43./255., 44./255., 112./255., 228./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kFall
		case 79:
		{
			Double_t red[9]   = { 49./255., 59./255., 72./255., 88./255., 114./255., 141./255., 176./255., 205./255., 222./255.};
			Double_t green[9] = { 78./255., 72./255., 66./255., 57./255.,  59./255.,  75./255., 106./255., 142./255., 173./255.};
			Double_t blue[9]  = { 78./255., 55./255., 46./255., 40./255.,  39./255.,  39./255.,  40./255.,  41./255.,  47./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kFruitPunch
		case 80:
		{
			Double_t red[9]   = { 243./255., 222./255., 201./255., 185./255., 165./255., 158./255., 166./255., 187./255., 219./255.};
			Double_t green[9] = {  94./255., 108./255., 132./255., 135./255., 125./255.,  96./255.,  68./255.,  51./255.,  61./255.};
			Double_t blue[9]  = {   7./255.,  9./255.,   12./255.,  19./255.,  45./255.,  89./255., 118./255., 146./255., 118./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kFuchsia
		case 81:
		{
			Double_t red[9]   = { 19./255., 44./255., 74./255., 105./255., 137./255., 166./255., 194./255., 206./255., 220./255.};
			Double_t green[9] = { 19./255., 28./255., 40./255.,  55./255.,  82./255., 110./255., 159./255., 181./255., 220./255.};
			Double_t blue[9]  = { 19./255., 42./255., 68./255.,  96./255., 129./255., 157./255., 188./255., 203./255., 220./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kGreyYellow
		case 82:
		{
			Double_t red[9]   = { 33./255., 44./255., 70./255.,  99./255., 140./255., 165./255., 199./255., 211./255., 216./255.};
			Double_t green[9] = { 38./255., 50./255., 76./255., 105./255., 140./255., 165./255., 191./255., 189./255., 167./255.};
			Double_t blue[9]  = { 55./255., 67./255., 97./255., 124./255., 140./255., 166./255., 163./255., 129./255.,  52./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kGreenBrownTerrain
		case 83:
		{
			Double_t red[9]   = { 0./255., 33./255., 73./255., 124./255., 136./255., 152./255., 159./255., 171./255., 223./255.};
			Double_t green[9] = { 0./255., 43./255., 92./255., 124./255., 134./255., 126./255., 121./255., 144./255., 223./255.};
			Double_t blue[9]  = { 0./255., 43./255., 68./255.,  76./255.,  73./255.,  64./255.,  72./255., 114./255., 223./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kGreenPink
		case 84:
		{
			Double_t red[9]   = {  5./255.,  18./255.,  45./255., 124./255., 193./255., 223./255., 205./255., 128./255., 49./255.};
			Double_t green[9] = { 48./255., 134./255., 207./255., 230./255., 193./255., 113./255.,  28./255.,   0./255.,  7./255.};
			Double_t blue[9]  = {  6./255.,  15./255.,  41./255., 121./255., 193./255., 226./255., 208./255., 130./255., 49./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kIsland
		case 85:
		{
			Double_t red[9]   = { 180./255., 106./255., 104./255., 135./255., 164./255., 188./255., 189./255., 165./255., 144./255.};
			Double_t green[9] = {  72./255., 126./255., 154./255., 184./255., 198./255., 207./255., 205./255., 190./255., 179./255.};
			Double_t blue[9]  = {  41./255., 120./255., 158./255., 188./255., 194./255., 181./255., 145./255., 100./255.,  62./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kLake
		case 86:
		{
			Double_t red[9]   = {  57./255.,  72./255.,  94./255., 117./255., 136./255., 154./255., 174./255., 192./255., 215./255.};
			Double_t green[9] = {   0./255.,  33./255.,  68./255., 109./255., 140./255., 171./255., 192./255., 196./255., 209./255.};
			Double_t blue[9]  = { 116./255., 137./255., 173./255., 201./255., 200./255., 201./255., 203./255., 190./255., 187./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kLightTemperature
		case 87:
		{
			Double_t red[9]   = {  31./255.,  71./255., 123./255., 160./255., 210./255., 222./255., 214./255., 199./255., 183./255.};
			Double_t green[9] = {  40./255., 117./255., 171./255., 211./255., 231./255., 220./255., 190./255., 132./255.,  65./255.};
			Double_t blue[9]  = { 234./255., 214./255., 228./255., 222./255., 210./255., 160./255., 105./255.,  60./255.,  34./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kLightTerrain
		case 88:
		{
			Double_t red[9]   = { 123./255., 108./255., 109./255., 126./255., 154./255., 172./255., 188./255., 196./255., 218./255.};
			Double_t green[9] = { 184./255., 138./255., 130./255., 133./255., 154./255., 175./255., 188./255., 196./255., 218./255.};
			Double_t blue[9]  = { 208./255., 130./255., 109./255.,  99./255., 110./255., 122./255., 150./255., 171./255., 218./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kMint
		case 89:
		{
			Double_t red[9]   = { 105./255., 106./255., 122./255., 143./255., 159./255., 172./255., 176./255., 181./255., 207./255.};
			Double_t green[9] = { 252./255., 197./255., 194./255., 187./255., 174./255., 162./255., 153./255., 136./255., 125./255.};
			Double_t blue[9]  = { 146./255., 133./255., 144./255., 155./255., 163./255., 167./255., 166./255., 162./255., 174./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kNeon
		case 90:
		{
			Double_t red[9]   = { 171./255., 141./255., 145./255., 152./255., 154./255., 159./255., 163./255., 158./255., 177./255.};
			Double_t green[9] = { 236./255., 143./255., 100./255.,  63./255.,  53./255.,  55./255.,  44./255.,  31./255.,   6./255.};
			Double_t blue[9]  = {  59./255.,  48./255.,  46./255.,  44./255.,  42./255.,  54./255.,  82./255., 112./255., 179./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kPastel
		case 91:
		{
			Double_t red[9]   = { 180./255., 190./255., 209./255., 223./255., 204./255., 228./255., 205./255., 152./255.,  91./255.};
			Double_t green[9] = {  93./255., 125./255., 147./255., 172./255., 181./255., 224./255., 233./255., 198./255., 158./255.};
			Double_t blue[9]  = { 236./255., 218./255., 160./255., 133./255., 114./255., 132./255., 162./255., 220./255., 218./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kPearl
		case 92:
		{
			Double_t red[9]   = { 225./255., 183./255., 162./255., 135./255., 115./255., 111./255., 119./255., 145./255., 211./255.};
			Double_t green[9] = { 205./255., 177./255., 166./255., 135./255., 124./255., 117./255., 117./255., 132./255., 172./255.};
			Double_t blue[9]  = { 186./255., 165./255., 155./255., 135./255., 126./255., 130./255., 150./255., 178./255., 226./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kPigeon
		case 93:
		{
			Double_t red[9]   = { 39./255., 43./255., 59./255., 63./255., 80./255., 116./255., 153./255., 177./255., 223./255.};
			Double_t green[9] = { 39./255., 43./255., 59./255., 74./255., 91./255., 114./255., 139./255., 165./255., 223./255.};
			Double_t blue[9]  = { 39./255., 50./255., 59./255., 70./255., 85./255., 115./255., 151./255., 176./255., 223./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kPlum
		case 94:
		{
			Double_t red[9]   = { 0./255., 38./255., 60./255., 76./255., 84./255., 89./255., 101./255., 128./255., 204./255.};
			Double_t green[9] = { 0./255., 10./255., 15./255., 23./255., 35./255., 57./255.,  83./255., 123./255., 199./255.};
			Double_t blue[9]  = { 0./255., 11./255., 22./255., 40./255., 63./255., 86./255.,  97./255.,  94./255.,  85./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kRedBlue
		case 95:
		{
			Double_t red[9]   = { 94./255., 112./255., 141./255., 165./255., 167./255., 140./255.,  91./255.,  49./255.,  27./255.};
			Double_t green[9] = { 27./255.,  46./255.,  88./255., 135./255., 166./255., 161./255., 135./255.,  97./255.,  58./255.};
			Double_t blue[9]  = { 42./255.,  52./255.,  81./255., 106./255., 139./255., 158./255., 155./255., 137./255., 116./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kRose
		case 96:
		{
			Double_t red[9]   = { 30./255., 49./255., 79./255., 117./255., 135./255., 151./255., 146./255., 138./255., 147./255.};
			Double_t green[9] = { 63./255., 60./255., 72./255.,  90./255.,  94./255.,  94./255.,  68./255.,  46./255.,  16./255.};
			Double_t blue[9]  = { 18./255., 28./255., 41./255.,  56./255.,  62./255.,  63./255.,  50./255.,  36./255.,  21./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kRust
		case 97:
		{
			Double_t red[9]   = {  0./255., 30./255., 63./255., 101./255., 143./255., 152./255., 169./255., 187./255., 230./255.};
			Double_t green[9] = {  0./255., 14./255., 28./255.,  42./255.,  58./255.,  61./255.,  67./255.,  74./255.,  91./255.};
			Double_t blue[9]  = { 39./255., 26./255., 21./255.,  18./255.,  15./255.,  14./255.,  14./255.,  13./255.,  13./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kSandyTerrain
		case 98:
		{
			Double_t red[9]   = { 149./255., 140./255., 164./255., 179./255., 182./255., 181./255., 131./255., 87./255., 61./255.};
			Double_t green[9] = {  62./255.,  70./255., 107./255., 136./255., 144./255., 138./255., 117./255., 87./255., 74./255.};
			Double_t blue[9]  = {  40./255.,  38./255.,  45./255.,  49./255.,  49./255.,  49./255.,  38./255., 32./255., 34./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kSienna
		case 99:
		{
			Double_t red[9]   = { 99./255., 112./255., 148./255., 165./255., 179./255., 182./255., 183./255., 183./255., 208./255.};
			Double_t green[9] = { 39./255.,  40./255.,  57./255.,  79./255., 104./255., 127./255., 148./255., 161./255., 198./255.};
			Double_t blue[9]  = { 15./255.,  16./255.,  18./255.,  33./255.,  51./255.,  79./255., 103./255., 129./255., 177./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kSolar
		case 100:
		{
			Double_t red[9]   = { 99./255., 116./255., 154./255., 174./255., 200./255., 196./255., 201./255., 201./255., 230./255.};
			Double_t green[9] = {  0./255.,   0./255.,   8./255.,  32./255.,  58./255.,  83./255., 119./255., 136./255., 173./255.};
			Double_t blue[9]  = {  5./255.,   6./255.,   7./255.,   9./255.,   9./255.,  14./255.,  17./255.,  19./255.,  24./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kSouthWest
		case 101:
		{
			Double_t red[9]   = { 82./255., 106./255., 126./255., 141./255., 155./255., 163./255., 142./255., 107./255.,  66./255.};
			Double_t green[9] = { 62./255.,  44./255.,  69./255., 107./255., 135./255., 152./255., 149./255., 132./255., 119./255.};
			Double_t blue[9]  = { 39./255.,  25./255.,  31./255.,  60./255.,  73./255.,  68./255.,  49./255.,  72./255., 188./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kStarryNight
		case 102:
		{
			Double_t red[9]   = { 18./255., 29./255., 44./255.,  72./255., 116./255., 158./255., 184./255., 208./255., 221./255.};
			Double_t green[9] = { 27./255., 46./255., 71./255., 105./255., 146./255., 177./255., 189./255., 190./255., 183./255.};
			Double_t blue[9]  = { 39./255., 55./255., 80./255., 108./255., 130./255., 133./255., 124./255., 100./255.,  76./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kSunset
		case 103:
		{
			Double_t red[9]   = { 0./255., 48./255., 119./255., 173./255., 212./255., 224./255., 228./255., 228./255., 245./255.};
			Double_t green[9] = { 0./255., 13./255.,  30./255.,  47./255.,  79./255., 127./255., 167./255., 205./255., 245./255.};
			Double_t blue[9]  = { 0./255., 68./255.,  75./255.,  43./255.,  16./255.,  22./255.,  55./255., 128./255., 245./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kTemperatureMap
		case 104:
		{
			Double_t red[9]   = {  34./255.,  70./255., 129./255., 187./255., 225./255., 226./255., 216./255., 193./255., 179./255.};
			Double_t green[9] = {  48./255.,  91./255., 147./255., 194./255., 226./255., 229./255., 196./255., 110./255.,  12./255.};
			Double_t blue[9]  = { 234./255., 212./255., 216./255., 224./255., 206./255., 110./255.,  53./255.,  40./255.,  29./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kThermometer
		case 105:
		{
			Double_t red[9]   = {  30./255.,  55./255., 103./255., 147./255., 174./255., 203./255., 188./255., 151./255., 105./255.};
			Double_t green[9] = {   0./255.,  65./255., 138./255., 182./255., 187./255., 175./255., 121./255.,  53./255.,   9./255.};
			Double_t blue[9]  = { 191./255., 202./255., 212./255., 208./255., 171./255., 140./255.,  97./255.,  57./255.,  30./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kValentine
		case 106:
		{
			Double_t red[9]   = { 112./255., 97./255., 113./255., 125./255., 138./255., 159./255., 178./255., 188./255., 225./255.};
			Double_t green[9] = {  16./255., 17./255.,  24./255.,  37./255.,  56./255.,  81./255., 110./255., 136./255., 189./255.};
			Double_t blue[9]  = {  38./255., 35./255.,  46./255.,  59./255.,  78./255., 103./255., 130./255., 152./255., 201./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kVisibleSpectrum
		case 107:
		{
			Double_t red[9]   = { 18./255.,  72./255.,   5./255.,  23./255.,  29./255., 201./255., 200./255., 98./255., 29./255.};
			Double_t green[9] = {  0./255.,   0./255.,  43./255., 167./255., 211./255., 117./255.,   0./255.,  0./255.,  0./255.};
			Double_t blue[9]  = { 51./255., 203./255., 177./255.,  26./255.,  10./255.,   9./255.,   8./255.,  3./255.,  0./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kWaterMelon
		case 108:
		{
			Double_t red[9]   = { 19./255., 42./255., 64./255.,  88./255., 118./255., 147./255., 175./255., 187./255., 205./255.};
			Double_t green[9] = { 19./255., 55./255., 89./255., 125./255., 154./255., 169./255., 161./255., 129./255.,  70./255.};
			Double_t blue[9]  = { 19./255., 32./255., 47./255.,  70./255., 100./255., 128./255., 145./255., 130./255.,  75./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kCool
		case 109:
		{
			Double_t red[9]   = {  33./255.,  31./255.,  42./255.,  68./255.,  86./255., 111./255., 141./255., 172./255., 227./255.};
			Double_t green[9] = { 255./255., 175./255., 145./255., 106./255.,  88./255.,  55./255.,  15./255.,   0./255.,   0./255.};
			Double_t blue[9]  = { 255./255., 205./255., 202./255., 203./255., 208./255., 205./255., 203./255., 206./255., 231./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kCopper
		case 110:
		{
			Double_t red[9]   = { 0./255., 25./255., 50./255., 79./255., 110./255., 145./255., 181./255., 201./255., 254./255.};
			Double_t green[9] = { 0./255., 16./255., 30./255., 46./255.,  63./255.,  82./255., 101./255., 124./255., 179./255.};
			Double_t blue[9]  = { 0./255., 12./255., 21./255., 29./255.,  39./255.,  49./255.,  61./255.,  74./255., 103./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kGistEarth
		case 111:
		{
			Double_t red[9]   = { 0./255., 13./255.,  30./255.,  44./255.,  72./255., 120./255., 156./255., 200./255., 247./255.};
			Double_t green[9] = { 0./255., 36./255.,  84./255., 117./255., 141./255., 153./255., 151./255., 158./255., 247./255.};
			Double_t blue[9]  = { 0./255., 94./255., 100./255.,  82./255.,  56./255.,  66./255.,  76./255., 131./255., 247./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		// kViridis
		case 112:
		{
			Double_t red[9]   = { 26./255., 51./255.,  43./255.,  33./255.,  28./255.,  35./255.,  74./255., 144./255., 246./255.};
			Double_t green[9] = {  9./255., 24./255.,  55./255.,  87./255., 118./255., 150./255., 180./255., 200./255., 222./255.};
			Double_t blue[9]  = { 30./255., 96./255., 112./255., 114./255., 112./255., 101./255.,  72./255.,  35./255.,   0./255.};
			TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
			if(invert==true) { palette_inversion(9, stops, red, green, blue, 255, alpha); }
			break;
		}
		//
		default:  
		{
			cout << "Must enter a palette number betwee 51 - 112 (inclusive)" << endl;
			return;
		}
	}
	gStyle->SetNumberContours(255);
	return;
}
*/

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
int mBlack       = int(kBlack);

int mBabyPink    = int(kPink+1);
int mPink        = int(kPink+6);

int mDarkRed     = int(kRed+2);

int mDarkOrange  = int(kOrange+3);
int mOrange      = int(kOrange+7);
int mLightOrange = int(kOrange-3);

int mYellow      = int(kOrange);

int mSpring      = int(kSpring+10);
int mCyan        = int(kAzure+10);

int mGreen       = int(kGreen+2);
int mDarkGreen   = int(kSpring-7);

int mTeal        = int(kAzure+4);
int mDarkTeal    = int(kCyan+4);

int mDarkBlue    = int(kBlue+2);
int mSkyBlue     = int(kAzure+1);
int mPeriwinkle  = int(kBlue-7);

int mLavender    = int(kViolet+6);
int mPurple      = int(kViolet+2);
int mDarkPurple  = int(kViolet+3);


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#include <TH2D.h>
#include <TCanvas.h>
#include <TRandom3.h>
TH2D * get_testplot()
{
	int N_counts = 10000;
	TRandom *event_generator = new TRandom();
	event_generator -> SetSeed();
	
	int nbins_x = 256;
	double xmin = 0;
	double xmax = 256.0;
	int nbins_y = 10;
	double ymin = 0; 
	double ymax = 10.0;
	
	TH2D * this_hist = new TH2D("this_hist", "this_hist", nbins_x, xmin, xmax, nbins_y, ymin, ymax);
	
	double this_x;
	double this_y;
	int ncounts;
	for(int i=0; i<N_counts; i++)
	{
		this_x = 256.0*(event_generator->Rndm());
		if(this_x > 256.0) cout << "x=" << this_x << endl;
		ncounts = int( 1000*(this_x)/256.0 );
		for(int j=0; j<ncounts; j++)
		{
			this_y = 10.0*(event_generator->Rndm());
			this_hist -> Fill(this_x, this_y);
		}
	}
	return this_hist;
}

/*
#include <TApplication.h>
int main()
{
	cout << "Hello, world!" << endl;
	TApplication* rootapp = 0;
	char ** mychar = NULL;
	rootapp = new TApplication("blarg",0, mychar);
	
	TH2D * testplot = get_testplot();
	gStyle -> SetOptStat(0);
	double palettelabelsize = 0.02;
	
	
	
//	use_hq_palette(kViridis);
//	use_hq_palette(kBlueGreenYellow);
//	gStyle->SetPalette(kBird);
	
	
	
	use_hq_palette(kVisibleSpectrum);
//	use_hq_palette(kImprovedRainbow, true);
//	use_hq_palette(kAlpine);
//	use_hq_palette(kAvocado, true);
//	use_hq_palette(kDeepSea, false);
//	use_hq_palette(kAvocado);
//	use_hq_palette(kSunset);
//	use_hq_palette(kDarkRainBow);
//	use_hq_palette(kFuchsia);
//	use_hq_palette(kLake);
//	set_plot_style2();
//	use_hq_palette(kSolar);
//	use_hq_palette(kPlum);
//	use_hq_palette(kAtlantic);
	
	TCanvas * canvas1 = new TCanvas("canvas1", "canvas1", 100.0, 100.0,800,800);
	testplot -> Draw("colz");
//	((TPaletteAxis*)testplot->GetListOfFunctions()->FindObject("palette"))->GetAxis()->SetLabelSize(palettelabelsize);
	gPad->Update();

	rootapp->Run();
	return 0;
}
*/
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#endif