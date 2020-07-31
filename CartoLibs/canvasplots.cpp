#ifndef INCLUDE_CANVASPLOTS
#define INCLUDE_CANVASPLOTS 1

#include "canvasplots.h"

//#include "TCanvas.h"
//#include <TFile.h>

#include "GraphExtras.h"
#include "TGraphPainter.h"

//#include "FitUtility.cpp"
#include "FitUtility.h"

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

extern vector<energy_specifier> the_energyset;
class plotfuncs;
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
TCanvas * make_better_residucan(TH1D* datahist, TH1D* monohist, string namestub, energy_specifier the_monospecifier, int N_rebin)  // with functional components plotted now, too.
{
	double xmin = 0;
	TLegend * myLegend;
	TText *datalabel2 = new TText();
	datalabel2 -> SetNDC();
	datalabel2 -> SetTextColor(1);
	datalabel2 -> SetTextSize(0.018*2);
	TCanvas * c;
	
	string namestring = the_monospecifier.the_string + " keV -- " + namestub;
	
	TF1 * R = datahist -> GetFunction("R");
	if(!R) { cout << "The residucan fails." << endl;  return c; }
	
	// Rebin??
	if(N_rebin != 1)
	{
		datahist -> RebinX(N_rebin);
		R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm")); 
		R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); 
	}
	
	// Now make the plot of residuals.
	int nbins = datahist->GetNbinsX();
	vector<double> x_avg;
	vector<double> y_avg;
	vector<double> delta_x;
	vector<double> delta_y;
	x_avg.clear();
	y_avg.clear();
	delta_x.clear();
	delta_y.clear();
	double tmp_dy;
	
	for(int i=2; i<nbins; i++)  // skip the zeroth bin?  and also, apparently, the first bin.
	{
		x_avg.push_back(datahist->GetBinCenter(i));
		delta_x.push_back( datahist->GetBinWidth(i)/2.0 );
		y_avg.push_back( datahist->GetBinContent(i) - R->Eval(datahist->GetBinCenter(i)) );
		
		tmp_dy = sqrt(datahist->GetBinContent(i));
		if(tmp_dy==0) { tmp_dy=1.0; }
		delta_y.push_back( tmp_dy );
		
	}
	TGraphErrors * the_residuals = make_TGraphErrors(x_avg, y_avg, delta_x, delta_y, datahist->GetLineColor(), "Residuals");
	the_residuals ->GetXaxis() -> SetRangeUser(0, 6000);
	
	plotfuncs * my_plotfuncs_set = new plotfuncs();
	my_plotfuncs_set -> load_parameters(R);
	
	// --- // --- // 
		// Plot it.
	//
	gStyle->SetEndErrorSize(0);
	
	c = new TCanvas(namestring.c_str(), namestring.c_str(), 100, 0, 900, 700);
	c -> Divide(1,2);
	c -> cd(1);
//	datahist->SetTitle("");
	datahist->Draw();  // probably draws with fit function.
	monohist->Draw("same");
	my_plotfuncs_set->f_landau()->Draw("lsame");
	my_plotfuncs_set->f1_clifford()->Draw("lsame");
	my_plotfuncs_set->f2_clifford()->Draw("lsame");
	my_plotfuncs_set->f3_clifford()->Draw("lsame");
	my_plotfuncs_set->f4_clifford()->Draw("lsame");
	my_plotfuncs_set->f5_clifford()->Draw("lsame");
	my_plotfuncs_set->f_511()->Draw("lsame");
	my_plotfuncs_set->R_total()->Draw("lsame");
	
	
	if(the_monospecifier.the_int >= 3000)
	{
		myLegend = new TLegend(.16,.31,.45,.90,"");
	}
	else
	{
	//	datalabel2 -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
		myLegend = new TLegend(.70,.40,.99,.99,"");
	}
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(my_plotfuncs_set->R_total(),     "Full Response Function", "l" );
	myLegend -> AddEntry(my_plotfuncs_set->f_landau(),    "Landau/Moyal", "l" );
	myLegend -> AddEntry(my_plotfuncs_set->f1_clifford(), "f1 (gaussian)", "l" );
	myLegend -> AddEntry(my_plotfuncs_set->f2_clifford(), "f2 (flat) ~ alpha", "l" );
	myLegend -> AddEntry(my_plotfuncs_set->f3_clifford(), "f3 (exponential) ~ beta", "l" );
	myLegend -> AddEntry(my_plotfuncs_set->f4_clifford(), "f4 (shelf) ~ gamma", "l" );
	myLegend -> AddEntry(my_plotfuncs_set->f5_clifford(), "f5 (triangle) ~ delta", "l" );
	myLegend -> AddEntry(my_plotfuncs_set->f_511(),       "511s", "l" );
	
	
	myLegend -> Draw();
	datahist -> GetXaxis()->SetRangeUser(xmin, the_monospecifier.the_int+800);
	datahist -> GetYaxis()->SetRangeUser(-0.05*(datahist->GetMaximum() - datahist->GetMinimum()), 1.1*(datahist->GetMaximum() - datahist->GetMinimum()) );
	
	gPad->Update();
	c -> cd(2);
	the_residuals -> Draw();  // root says "Z" will do a thing I want,but it just makes the plot invisible.
//	the_residuals -> Draw("Z"); 
//	the_residuals ->GetXaxis() ->SetRangeUser(0, 6000);
	the_residuals ->GetXaxis() ->SetRangeUser(xmin, the_monospecifier.the_int+800);
	gPad -> SetGridy();
	gPad -> Update();
	
	return c;
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
//TCanvas * make_plotcanvas_p(MapSetup * the_map, int N_rebin=1, bool plot_fitfuncs=false, string canvasname="cp", int monoenergy_int=0)
TCanvas * make_plotcanvas_p(MapSetup* the_map, int N_rebin, bool plot_fitfuncs, string canvasname, int monoenergy_int)
{
	// Make plot titles...
	string top_title = "Generated KE = "+int_to_string(monoenergy_int)+" keV -- Top +";
	string bot_title = "Generated KE = "+int_to_string(monoenergy_int)+" keV -- Bottom +";
	
	gStyle->SetOptStat(0);
	TText *datalabel2 = new TText();
	datalabel2 -> SetNDC();
	datalabel2 -> SetTextColor(1);
	datalabel2 -> SetTextSize(0.018*2);
	
	const char* drawopt_first;
	const char* drawopt_later;
	if(plot_fitfuncs)
	{
		drawopt_first = "";
		drawopt_later = "same";
	}
	else
	{
		drawopt_first = "hist";
		drawopt_later = "histsame";
	}
	
	TH1D* naive_EnergyT_p_hist        = the_map->naive_EnergyT_p_hist;
	TH1D* measured_EnergyT_p_hist     = the_map->measured_EnergyT_p_hist;
	TH1D* measured_EnergyT_p_bb1agree = the_map->measured_EnergyT_p_bb1agree;
	TH1D* measured_EnergyT_p_bb1_r155 = the_map->measured_EnergyT_p_bb1_r155;
	TH1D* measured_EnergyT_p_bb1_r105 = the_map->measured_EnergyT_p_bb1_r105;
	
	TH1D* naive_EnergyB_p_hist        = the_map->naive_EnergyB_p_hist;
	TH1D* measured_EnergyB_p_hist     = the_map->measured_EnergyB_p_hist;
	TH1D* measured_EnergyB_p_bb1agree = the_map->measured_EnergyB_p_bb1agree;
	TH1D* measured_EnergyB_p_bb1_r155 = the_map->measured_EnergyB_p_bb1_r155;
	TH1D* measured_EnergyB_p_bb1_r105 = the_map->measured_EnergyB_p_bb1_r105;
	
	// count the counts:
	double count_naive_t, count_meas_t, count_bb1agree_t, count_r155_t, count_r105_t;
	double count_naive_b, count_meas_b, count_bb1agree_b, count_r155_b, count_r105_b;
	
	count_naive_t    = naive_EnergyT_p_hist        -> Integral();
	count_meas_t     = measured_EnergyT_p_hist     -> Integral();
	count_bb1agree_t = measured_EnergyT_p_bb1agree -> Integral();
	count_r155_t     = measured_EnergyT_p_bb1_r155 -> Integral();
	count_r105_t     = measured_EnergyT_p_bb1_r105 -> Integral();
	
	count_naive_b    = naive_EnergyB_p_hist        -> Integral();
	count_meas_b     = measured_EnergyB_p_hist     -> Integral();
	count_bb1agree_b = measured_EnergyB_p_bb1agree -> Integral();
	count_r155_b     = measured_EnergyB_p_bb1_r155 -> Integral();
	count_r105_b     = measured_EnergyB_p_bb1_r105 -> Integral();
	
	// Rebin for plotting:
	if(N_rebin!=1)
	{
		// don't rebin the originals.  only rebin the clones.
		measured_EnergyT_p_hist     = (TH1D*)the_map->measured_EnergyT_p_hist    ->Clone();
		naive_EnergyT_p_hist        = (TH1D*)the_map->naive_EnergyT_p_hist       ->Clone();
		measured_EnergyT_p_bb1agree = (TH1D*)the_map->measured_EnergyT_p_bb1agree->Clone();
		measured_EnergyT_p_bb1_r155 = (TH1D*)the_map->measured_EnergyT_p_bb1_r155->Clone();
		measured_EnergyT_p_bb1_r105 = (TH1D*)the_map->measured_EnergyT_p_bb1_r105->Clone();
		
		measured_EnergyB_p_hist     = (TH1D*)the_map->measured_EnergyB_p_hist    ->Clone();
		naive_EnergyB_p_hist        = (TH1D*)the_map->naive_EnergyB_p_hist       ->Clone();
		measured_EnergyB_p_bb1agree = (TH1D*)the_map->measured_EnergyB_p_bb1agree->Clone();
		measured_EnergyB_p_bb1_r155 = (TH1D*)the_map->measured_EnergyB_p_bb1_r155->Clone();
		measured_EnergyB_p_bb1_r105 = (TH1D*)the_map->measured_EnergyB_p_bb1_r105->Clone();
		
		//
		measured_EnergyT_p_hist    ->RebinX(N_rebin);
		naive_EnergyT_p_hist       ->RebinX(N_rebin);
		measured_EnergyT_p_bb1agree->RebinX(N_rebin);
		measured_EnergyT_p_bb1_r155->RebinX(N_rebin);
		measured_EnergyT_p_bb1_r105->RebinX(N_rebin);

		measured_EnergyB_p_hist    ->RebinX(N_rebin);
		naive_EnergyB_p_hist       ->RebinX(N_rebin);
		measured_EnergyB_p_bb1agree->RebinX(N_rebin);
		measured_EnergyB_p_bb1_r155->RebinX(N_rebin);
		measured_EnergyB_p_bb1_r105->RebinX(N_rebin);
		
		// "rebin" the fit functions too.
		if(plot_fitfuncs)
		{
			TF1 * R;
		
			R = measured_EnergyT_p_hist    ->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = naive_EnergyT_p_hist       ->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyT_p_bb1agree->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyT_p_bb1_r155->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyT_p_bb1_r105->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }

			R = measured_EnergyB_p_hist    ->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = naive_EnergyB_p_hist       ->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyB_p_bb1agree->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyB_p_bb1_r155->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyB_p_bb1_r105->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
		}
	}
	
	//
	TLegend * myLegend;
	string namestring;

	TCanvas * cp = new TCanvas(canvasname.c_str(), canvasname.c_str(), 100, 0, 900, 700);
	cp -> Divide(1,2);
	cp -> cd(1);
	measured_EnergyT_p_bb1agree->SetTitle("");
	measured_EnergyT_p_bb1agree->Draw(drawopt_first);
	measured_EnergyT_p_hist    ->Draw(drawopt_later);
	naive_EnergyT_p_hist       ->Draw(drawopt_later);
	measured_EnergyT_p_bb1_r155->Draw(drawopt_later);
	measured_EnergyT_p_bb1_r105->Draw(drawopt_later);
	
	if(monoenergy_int>=4000)
	{
		myLegend = new TLegend(.16,.61,.55,.90,"");
	}
	else
	{
		myLegend = new TLegend(.60,.70,.99,.99,"");
	}
	myLegend -> SetFillColor(0);
	namestring = int_to_string( (int)count_naive_t)+" Events -- T+:  Generated KE";
	myLegend -> AddEntry(naive_EnergyT_p_hist,        namestring.c_str(), "l" );
	namestring = int_to_string( (int)count_meas_t)+" Events -- T+:  Observed";
	myLegend -> AddEntry(measured_EnergyT_p_hist,     namestring.c_str(), "l" );
	namestring = int_to_string( (int)count_bb1agree_t)+" Events -- T+:  BB1 Hit";
	myLegend -> AddEntry(measured_EnergyT_p_bb1agree, namestring.c_str(), "l" );
	namestring = int_to_string( (int)count_r155_t)+" Events -- T+:  rBB1 <= 15.5mm";
	myLegend -> AddEntry(measured_EnergyT_p_bb1_r155, namestring.c_str(), "l" );
	namestring = int_to_string( (int)count_r105_t)+" Events -- T+:  rBB1 <= 10.5mm";
	myLegend -> AddEntry(measured_EnergyT_p_bb1_r105, namestring.c_str(), "l" );
	
	
	myLegend -> Draw();
//	datalabel2 -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	gStyle->SetOptStat(0);
	gPad->Update();
	cp -> cd(2);
	measured_EnergyB_p_bb1agree->SetTitle("");
	measured_EnergyB_p_bb1agree->Draw(drawopt_first);
	measured_EnergyB_p_hist->Draw(drawopt_later);
	naive_EnergyB_p_hist->Draw(drawopt_later);
	measured_EnergyB_p_bb1_r155->Draw(drawopt_later);
	measured_EnergyB_p_bb1_r105->Draw(drawopt_later);
	
	if(monoenergy_int>=4000)
	{
		myLegend = new TLegend(.16,.61,.55,.90,"");
	}
	else
	{
		myLegend = new TLegend(.60,.70,.99,.99,"");
	}
	myLegend -> SetFillColor(0);
	namestring = int_to_string( (int)count_naive_b)+" Events -- B+:  Generated KE";
	myLegend -> AddEntry(naive_EnergyB_p_hist,        namestring.c_str(), "l" );
	namestring = int_to_string( (int)count_meas_b)+" Events -- B+:  Observed";
	myLegend -> AddEntry(measured_EnergyB_p_hist,     namestring.c_str(), "l" );
	namestring = int_to_string( (int)count_bb1agree_b)+" Events -- B+:  BB1 Hit";
	myLegend -> AddEntry(measured_EnergyB_p_bb1agree, namestring.c_str(), "l" );
	namestring = int_to_string( (int)count_r155_b)+" Events -- B+:  rBB1 <= 15.5mm";
	myLegend -> AddEntry(measured_EnergyB_p_bb1_r155, namestring.c_str(), "l" );
	namestring = int_to_string( (int)count_r105_b)+" Events -- B+:  rBB1 <= 10.5mm";
	myLegend -> AddEntry(measured_EnergyB_p_bb1_r105, namestring.c_str(), "l" );

	myLegend -> Draw();
	gPad -> Update();
	
	return cp;
	//
}
TCanvas * make_plotcanvas_m(MapSetup* the_map, int N_rebin, bool plot_fitfuncs, string canvasname, int monoenergy_int)
{
	// Make plot titles...
	string top_title = "Generated KE = "+int_to_string(monoenergy_int)+" keV -- Top -";
	string bot_title = "Generated KE = "+int_to_string(monoenergy_int)+" keV -- Bottom -";

	gStyle->SetOptStat(0);
	TText *datalabel2 = new TText();
	datalabel2 -> SetNDC();
	datalabel2 -> SetTextColor(1);
	datalabel2 -> SetTextSize(0.018*2);
	
	const char* drawopt_first;
	const char* drawopt_later;
	if(plot_fitfuncs)
	{
		drawopt_first = "";
		drawopt_later = "same";
	}
	else
	{
		drawopt_first = "hist";
		drawopt_later = "histsame";
	}
		
	TH1D* measured_EnergyT_m_hist     = the_map->measured_EnergyT_m_hist;
	TH1D* naive_EnergyT_m_hist        = the_map->naive_EnergyT_m_hist;
	TH1D* measured_EnergyT_m_bb1agree = the_map->measured_EnergyT_m_bb1agree;
	TH1D* measured_EnergyT_m_bb1_r155 = the_map->measured_EnergyT_m_bb1_r155;
	TH1D* measured_EnergyT_m_bb1_r105 = the_map->measured_EnergyT_m_bb1_r105;
	
	TH1D* measured_EnergyB_m_hist     = the_map->measured_EnergyB_m_hist;
	TH1D* naive_EnergyB_m_hist        = the_map->naive_EnergyB_m_hist;
	TH1D* measured_EnergyB_m_bb1agree = the_map->measured_EnergyB_m_bb1agree;
	TH1D* measured_EnergyB_m_bb1_r155 = the_map->measured_EnergyB_m_bb1_r155;
	TH1D* measured_EnergyB_m_bb1_r105 = the_map->measured_EnergyB_m_bb1_r105;
	
	// count the counts:
	double count_naive_t, count_meas_t, count_bb1agree_t, count_r155_t, count_r105_t;
	double count_naive_b, count_meas_b, count_bb1agree_b, count_r155_b, count_r105_b;
	
	count_naive_t    = naive_EnergyT_m_hist        -> Integral();
	count_meas_t     = measured_EnergyT_m_hist     -> Integral();
	count_bb1agree_t = measured_EnergyT_m_bb1agree -> Integral();
	count_r155_t     = measured_EnergyT_m_bb1_r155 -> Integral();
	count_r105_t     = measured_EnergyT_m_bb1_r105 -> Integral();
	
	count_naive_b    = naive_EnergyB_m_hist        -> Integral();
	count_meas_b     = measured_EnergyB_m_hist     -> Integral();
	count_bb1agree_b = measured_EnergyB_m_bb1agree -> Integral();
	count_r155_b     = measured_EnergyB_m_bb1_r155 -> Integral();
	count_r105_b     = measured_EnergyB_m_bb1_r105 -> Integral();
	
	// Rebin for plotting:
	if(N_rebin!=1)
	{
		// don't rebin the originals.  only rebin the clones.
		measured_EnergyT_m_hist     = (TH1D*)the_map->measured_EnergyT_m_hist    ->Clone();
		naive_EnergyT_m_hist        = (TH1D*)the_map->naive_EnergyT_m_hist       ->Clone();
		measured_EnergyT_m_bb1agree = (TH1D*)the_map->measured_EnergyT_m_bb1agree->Clone();
		measured_EnergyT_m_bb1_r155 = (TH1D*)the_map->measured_EnergyT_m_bb1_r155->Clone();
		measured_EnergyT_m_bb1_r105 = (TH1D*)the_map->measured_EnergyT_m_bb1_r105->Clone();
		
		measured_EnergyB_m_hist     = (TH1D*)the_map->measured_EnergyB_m_hist    ->Clone();
		naive_EnergyB_m_hist        = (TH1D*)the_map->naive_EnergyB_m_hist       ->Clone();
		measured_EnergyB_m_bb1agree = (TH1D*)the_map->measured_EnergyB_m_bb1agree->Clone();
		measured_EnergyB_m_bb1_r155 = (TH1D*)the_map->measured_EnergyB_m_bb1_r155->Clone();
		measured_EnergyB_m_bb1_r105 = (TH1D*)the_map->measured_EnergyB_m_bb1_r105->Clone();
		
		//
		measured_EnergyT_m_hist    ->RebinX(N_rebin);
		naive_EnergyT_m_hist       ->RebinX(N_rebin);
		measured_EnergyT_m_bb1agree->RebinX(N_rebin);
		measured_EnergyT_m_bb1_r155->RebinX(N_rebin);
		measured_EnergyT_m_bb1_r105->RebinX(N_rebin);

		measured_EnergyB_m_hist    ->RebinX(N_rebin);
		naive_EnergyB_m_hist       ->RebinX(N_rebin);
		measured_EnergyB_m_bb1agree->RebinX(N_rebin);
		measured_EnergyB_m_bb1_r155->RebinX(N_rebin);
		measured_EnergyB_m_bb1_r105->RebinX(N_rebin);
		
		// "rebin" the fit functions too.
		if(plot_fitfuncs)
		{
			TF1 * R;
		
			R = measured_EnergyT_m_hist    ->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = naive_EnergyT_m_hist       ->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyT_m_bb1agree->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyT_m_bb1_r155->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyT_m_bb1_r105->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }

			R = measured_EnergyB_m_hist    ->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = naive_EnergyB_m_hist       ->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyB_m_bb1agree->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyB_m_bb1_r155->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
			R = measured_EnergyB_m_bb1_r105->GetFunction("R");
			if(R) { R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm"));  }
			if(R) { R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); }
		}
	}
	
	TLegend * myLegend;
	string namestring;
	
	//
	TCanvas * cm = new TCanvas(canvasname.c_str(), canvasname.c_str(), 100, 0, 900, 700);
	cm -> Divide(1,2);
	cm -> cd(1);
	measured_EnergyT_m_bb1agree->SetTitle("");
	measured_EnergyT_m_bb1agree->Draw(drawopt_first);
	measured_EnergyT_m_hist    ->Draw(drawopt_later);
	naive_EnergyT_m_hist       ->Draw(drawopt_later);
	measured_EnergyT_m_bb1_r155->Draw(drawopt_later);
	measured_EnergyT_m_bb1_r105->Draw(drawopt_later);

	
	if(monoenergy_int>=4000)
	{
		myLegend = new TLegend(.16,.61,.55,.90,"");
	}
	else
	{
		myLegend = new TLegend(.60,.70,.99,.99,"");
	}
	myLegend -> SetFillColor(0);
	namestring = int_to_string( (int)count_naive_t)+" Events -- T-:  Generated KE";
	myLegend -> AddEntry(naive_EnergyT_m_hist,        namestring.c_str(), "l");
	namestring = int_to_string( (int)count_meas_t)+" Events -- T-:  Observed";
	myLegend -> AddEntry(measured_EnergyT_m_hist,     namestring.c_str(), "l");
	namestring = int_to_string( (int)count_bb1agree_t)+" Events -- T-:  BB1 Hit";
	myLegend -> AddEntry(measured_EnergyT_m_bb1agree, namestring.c_str(), "l");
	namestring = int_to_string( (int)count_r155_t)+" Events -- T-:  rBB1 <= 15.5mm";
	myLegend -> AddEntry(measured_EnergyT_m_bb1_r155, namestring.c_str(), "l");
	namestring = int_to_string( (int)count_r105_t)+" Events -- T-:  rBB1 <= 10.5mm";
	myLegend -> AddEntry(measured_EnergyT_m_bb1_r105, namestring.c_str(), "l");
		
	myLegend -> Draw();
//	datalabel2 -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	gPad->Update();
	cm -> cd(2);
	measured_EnergyB_m_bb1agree->SetTitle("");
	measured_EnergyB_m_bb1agree->Draw(drawopt_first);
	measured_EnergyB_m_hist    ->Draw(drawopt_later);
	naive_EnergyB_m_hist       ->Draw(drawopt_later);
	measured_EnergyB_m_bb1_r155->Draw(drawopt_later);
	measured_EnergyB_m_bb1_r105->Draw(drawopt_later);
	
	if(monoenergy_int>=4000)
	{
		myLegend = new TLegend(.16,.61,.55,.90,"");
	}
	else
	{
		myLegend = new TLegend(.60,.70,.99,.99,"");
	}
	myLegend -> SetFillColor(0);
	namestring = int_to_string( (int)count_naive_b)+" Events -- B-:  Generated KE";
	myLegend -> AddEntry(naive_EnergyB_m_hist,        namestring.c_str(), "l");
	namestring = int_to_string( (int)count_meas_b)+" Events -- B-:  Observed";
	myLegend -> AddEntry(measured_EnergyB_m_hist,     namestring.c_str(), "l");
	namestring = int_to_string( (int)count_bb1agree_b)+" Events -- B-:  BB1 Hit";
	myLegend -> AddEntry(measured_EnergyB_m_bb1agree, namestring.c_str(), "l");
	namestring = int_to_string( (int)count_r155_b)+" Events -- B-:  rBB1 <= 15.5mm";
	myLegend -> AddEntry(measured_EnergyB_m_bb1_r155, namestring.c_str(), "l");
	namestring = int_to_string( (int)count_r105_b)+" Events -- B-:  rBB1 <= 10.5mm";
	myLegend -> AddEntry(measured_EnergyB_m_bb1_r105, namestring.c_str(), "l");

	
	myLegend -> Draw();
	gPad -> Update();
	
	return cm;
	//
}

TCanvas * make_bb1agreecanvas_p(MapSetup* the_map, int N_rebin, bool plot_fitfuncs, string canvasname, int monoenergy_int) // bb1 agree plot
{
	gStyle->SetOptStat(0);
	TText *datalabel2 = new TText();
	datalabel2 -> SetNDC();
	datalabel2 -> SetTextColor(1);
	datalabel2 -> SetTextSize(0.018*2);
	
	const char* drawopt_first;
	const char* drawopt_later;
	if(plot_fitfuncs)
	{
		drawopt_first = "";
		drawopt_later = "same";
	}
	else
	{
		drawopt_first = "hist";
		drawopt_later = "histsame";
	}
	
	// don't rebin the originals.  only rebin the clones.  Also, don't re-range the originals.
	TH1D* naive_EnergyT_p_hist        = (TH1D*)the_map->naive_EnergyT_p_hist       ->Clone();
	TH1D* measured_EnergyT_p_bb1agree = (TH1D*)the_map->measured_EnergyT_p_bb1agree->Clone();

	TH1D* naive_EnergyB_p_hist        = (TH1D*)the_map->naive_EnergyB_p_hist       ->Clone();
	TH1D* measured_EnergyB_p_bb1agree = (TH1D*)the_map->measured_EnergyB_p_bb1agree->Clone();

	// Rebin for plotting:
	if(N_rebin!=1)
	{
		//
		naive_EnergyT_p_hist       ->RebinX(N_rebin);
		measured_EnergyT_p_bb1agree->RebinX(N_rebin);

		naive_EnergyB_p_hist       ->RebinX(N_rebin);
		measured_EnergyB_p_bb1agree->RebinX(N_rebin);
		
		// "rebin" the fit functions too.
		if(plot_fitfuncs)
		{
			TF1 * R;
			R = measured_EnergyT_p_bb1agree->GetFunction("R");
			if(R) 
			{ 
				R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm")); 
				R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); 
			}
//			//
			R = measured_EnergyB_p_bb1agree->GetFunction("R");
			if(R) 
			{ 
				R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm")); 
				R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); 
			}
		}
	}
	if(monoenergy_int != 0)
	{
	//	measured_EnergyT_p_bb1agree->GetXaxis()->SetRangeUser(monoenergy_int-1500.0, monoenergy_int+500.0);
	//	measured_EnergyB_p_bb1agree->GetXaxis()->SetRangeUser(monoenergy_int-1500.0, monoenergy_int+500.0);
	}
	
	
	TCanvas * cp = new TCanvas(canvasname.c_str(), canvasname.c_str(), 100, 0, 900, 700);
	cp -> Divide(1,2);
	cp -> cd(1);
	measured_EnergyT_p_bb1agree->Draw(drawopt_first);
	naive_EnergyT_p_hist->Draw(drawopt_later);

	gPad -> Update();
//	datalabel2 -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	cp -> cd(2);
	measured_EnergyB_p_bb1agree->Draw(drawopt_first);
	naive_EnergyB_p_hist->Draw(drawopt_later);
	
	gPad -> Update();
	
	return cp;
}
TCanvas * make_bb1agreecanvas_m(MapSetup* the_map, int N_rebin, bool plot_fitfuncs, string canvasname, int monoenergy_int) // bb1 agree plot
{
	gStyle->SetOptStat(0);
	TText *datalabel2 = new TText();
	datalabel2 -> SetNDC();
	datalabel2 -> SetTextColor(1);
	datalabel2 -> SetTextSize(0.018*2);
	
	const char* drawopt_first;
	const char* drawopt_later;
	if(plot_fitfuncs)
	{
		drawopt_first = "";
		drawopt_later = "same";
	}
	else
	{
		drawopt_first = "hist";
		drawopt_later = "histsame";
	}
	
	// don't rebin the originals.  only rebin the clones.  Also, don't re-range the originals.
	TH1D* naive_EnergyT_m_hist        = (TH1D*)the_map->naive_EnergyT_m_hist       ->Clone();
	TH1D* measured_EnergyT_m_bb1agree = (TH1D*)the_map->measured_EnergyT_m_bb1agree->Clone();
	
	TH1D* naive_EnergyB_m_hist        = (TH1D*)the_map->naive_EnergyB_m_hist       ->Clone();
	TH1D* measured_EnergyB_m_bb1agree = (TH1D*)the_map->measured_EnergyB_m_bb1agree->Clone();

	// Rebin for plotting:
	if(N_rebin!=1)
	{
		naive_EnergyT_m_hist       ->RebinX(N_rebin);
		measured_EnergyT_m_bb1agree->RebinX(N_rebin);

		naive_EnergyB_m_hist       ->RebinX(N_rebin);
		measured_EnergyB_m_bb1agree->RebinX(N_rebin);
		
		// "rebin" the fit functions too.
		if(plot_fitfuncs)
		{
			TF1 * R;

			R = measured_EnergyT_m_bb1agree->GetFunction("R");
			if(R) 
			{ 
				R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm")); 
				R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); 
			}
			R = measured_EnergyB_m_bb1agree->GetFunction("R");
			if(R) 
			{ 
				R -> SetParameter("01_norm",  (double)(N_rebin)*R->GetParameter("01_norm")); 
				R -> SetParameter("02_scale", (double)(N_rebin)*R->GetParameter("02_scale")); 
			}
		}
	}
	if(monoenergy_int != 0)
	{
	//	measured_EnergyT_m_bb1agree->GetXaxis()->SetRangeUser(monoenergy_int-1500.0, monoenergy_int+500.0);
	//	measured_EnergyB_m_bb1agree->GetXaxis()->SetRangeUser(monoenergy_int-1500.0, monoenergy_int+500.0);
	}

	//
	TCanvas * cm = new TCanvas(canvasname.c_str(), canvasname.c_str(), 100, 0, 900, 700);
	cm -> Divide(1,2);
	cm -> cd(1);
	measured_EnergyT_m_bb1agree->Draw(drawopt_first);
	naive_EnergyT_m_hist->Draw(drawopt_later);

//	datalabel2 -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	cm -> cd(2);
	measured_EnergyB_m_bb1agree->Draw(drawopt_first);
	naive_EnergyB_m_hist->Draw(drawopt_later);
	
	gPad -> Update();
	
	return cm;
	//
}



// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void makesave_E0ResiduCan(TFile * f, 
	set_of_fitvectors tp_fitvector, set_of_fitvectors bp_fitvector, set_of_fitvectors tm_fitvector, set_of_fitvectors bm_fitvector, 
	vector<double> x_avg, vector<double> x_err) 
{
	// --- // 
	// ok, so now I have lots of vectors of things that can have graphs made of them.	
	TGraph * graph_tp_E0 = make_TGraphErrors(x_avg, tp_fitvector.E0_vec, x_err, tp_fitvector.E0_err, tp_fitvector.color, "E0:  Top (+)");  //
	TGraph * graph_tm_E0 = make_TGraphErrors(x_avg, tm_fitvector.E0_vec, x_err, tm_fitvector.E0_err, tm_fitvector.color, "E0:  Top (-)");  //
	TGraph * graph_bp_E0 = make_TGraphErrors(x_avg, bp_fitvector.E0_vec, x_err, bp_fitvector.E0_err, bp_fitvector.color, "E0:  Bottom (+)");  //
	TGraph * graph_bm_E0 = make_TGraphErrors(x_avg, bm_fitvector.E0_vec, x_err, bm_fitvector.E0_err, bm_fitvector.color, "E0:  Bottom (-)");  //
	
//	TLegend * myLegend;
	TText *datalabelr = new TText();
	datalabelr -> SetNDC();
	datalabelr -> SetTextColor(1);
	datalabelr -> SetTextSize(0.018*1.4);
	
	TGraphErrors * graph_tp_E0_ = (TGraphErrors*)graph_tp_E0 -> Clone();
	TGraphErrors * graph_tm_E0_ = (TGraphErrors*)graph_tm_E0 -> Clone();
	TGraphErrors * graph_bp_E0_ = (TGraphErrors*)graph_bp_E0 -> Clone();
	TGraphErrors * graph_bm_E0_ = (TGraphErrors*)graph_bm_E0 -> Clone();
	
	TF1* fitline_tp = new TF1("fitline_tp", "[m]*x + [b]", 0.0, 5500.0);
	TF1* fitline_tm = new TF1("fitline_tm", "[m]*x + [b]", 0.0, 5500.0);
	TF1* fitline_bp = new TF1("fitline_bp", "[m]*x + [b]", 0.0, 5500.0);
	TF1* fitline_bm = new TF1("fitline_bm", "[m]*x + [b]", 0.0, 5500.0);
	fitline_tp->SetLineColor(graph_tp_E0->GetLineColor());
	fitline_tm->SetLineColor(graph_tm_E0->GetLineColor());
	fitline_bp->SetLineColor(graph_bp_E0->GetLineColor());
	fitline_bm->SetLineColor(graph_bm_E0->GetLineColor());
	
	fitline_tp -> SetParameter("m", 1.0);
	fitline_tm -> SetParameter("m", 1.0);
	fitline_bp -> SetParameter("m", 1.0);
	fitline_bm -> SetParameter("m", 1.0);
	fitline_tp -> SetParameter("b", -300.0);
	fitline_tm -> SetParameter("b", -300.0);
	fitline_bp -> SetParameter("b", -300.0);
	fitline_bm -> SetParameter("b", -300.0);
	
	
	// just skip fitting the 500 eV point:
	// ...and also the 750 eV point:
	graph_tp_E0->Fit(fitline_tp, "", "", 800.0, 5500.0);
	graph_tm_E0->Fit(fitline_tm, "", "", 800.0, 5500.0);
	graph_bp_E0->Fit(fitline_bp, "", "", 800.0, 5500.0);
	graph_bm_E0->Fit(fitline_bm, "", "", 800.0, 5500.0);
	
	double * x_points;
	double * y_points;
	int n_points;
	double * x_err_;
	double * y_err_;
	double * better_y;
	
	x_points = graph_tp_E0_->GetX();
	y_points = graph_tp_E0_->GetY();
	better_y = graph_tp_E0_->GetY();  // to prevent a segfault.
	n_points = graph_tp_E0_->GetN();
	x_err_ = graph_tp_E0_->GetEX();
	y_err_ = graph_tp_E0_->GetEY();
	for(int i=0; i<n_points; i++)
	{
		better_y[i] = y_points[i] - fitline_tp->Eval(x_points[i]);
	}
	TGraphErrors * resid_tp = new TGraphErrors(n_points, x_points, better_y, x_err_, y_err_);
	resid_tp -> SetLineColor(graph_tp_E0->GetLineColor());
	
	x_points = graph_tm_E0_->GetX();
	y_points = graph_tm_E0_->GetY();
	better_y = graph_tm_E0_->GetY();  // to prevent a segfault.
	n_points = graph_tm_E0_->GetN();
	x_err_ = graph_tm_E0_->GetEX();
	y_err_ = graph_tm_E0_->GetEY();
	for(int i=0; i<n_points; i++)
	{
		better_y[i] = y_points[i] - fitline_tm->Eval(x_points[i]);
	}
	TGraphErrors * resid_tm = new TGraphErrors(n_points, x_points, better_y, x_err_, y_err_);
	resid_tm -> SetLineColor(graph_tm_E0->GetLineColor());

	
	x_points = graph_bp_E0_->GetX();
	y_points = graph_bp_E0_->GetY();
	better_y = graph_bp_E0_->GetY();  // to prevent a segfault.
	n_points = graph_bp_E0_->GetN();
	x_err_ = graph_bp_E0_->GetEX();
	y_err_ = graph_bp_E0_->GetEY();
	for(int i=0; i<n_points; i++)
	{
		better_y[i] = y_points[i] - fitline_bp->Eval(x_points[i]);
	}
	TGraphErrors * resid_bp = new TGraphErrors(n_points, x_points, better_y, x_err_, y_err_);
	resid_bp -> SetLineColor(graph_bp_E0->GetLineColor());


	x_points = graph_bm_E0_->GetX();
	y_points = graph_bm_E0_->GetY();
	better_y = graph_bm_E0_->GetY();  // to prevent a segfault.
	n_points = graph_bm_E0_->GetN();
	x_err_ = graph_bm_E0_->GetEX();
	y_err_ = graph_bm_E0_->GetEY();
	for(int i=0; i<n_points; i++)
	{
		better_y[i] = y_points[i] - fitline_bm->Eval(x_points[i]);
	}
	TGraphErrors * resid_bm = new TGraphErrors(n_points, x_points, better_y, x_err_, y_err_);
	resid_bm -> SetLineColor(graph_bm_E0->GetLineColor());
	
	TCanvas * c_resid = new TCanvas("E0 Residuals Canvas", "E0 Residuals Canvas", 100.0, 0.0,900,700);
	c_resid->cd();  // needed?
	TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);  // pad2 is on bottom.
	pad2->SetTopMargin(0.00001);
	pad2->SetBottomMargin(0.3);
	pad2->SetBorderMode(0);
	pad2->SetGrid();
	pad2->Draw();

	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);  // pad1 is on top.
	pad1->SetBottomMargin(0.00001);
	pad1->SetBorderMode(0);
	pad1->Draw();
	
	pad1 -> cd();
	
	// draw on pad 1.
	graph_tp_E0->Draw();
	graph_tm_E0->Draw("same");
	graph_bp_E0->Draw("same");
	graph_bm_E0->Draw("same");

	TLegend * myLegend;
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_E0, graph_tp_E0->GetName(), "l");
	myLegend -> AddEntry(graph_tm_E0, graph_tm_E0->GetName(), "l");
	myLegend -> AddEntry(graph_bp_E0, graph_bp_E0->GetName(), "l");
	myLegend -> AddEntry(graph_bm_E0, graph_bm_E0->GetName(), "l");
	myLegend -> Draw();

	gPad -> Update();
	pad2 -> cd();
	// draw on pad 2.
	resid_tp ->Draw();
	double yplotmin = resid_tp->GetHistogram()->GetMinimum() - 0.1*abs(resid_tp->GetHistogram()->GetMinimum()) - 0.1;
	double yplotmax = resid_tp->GetHistogram()->GetMaximum() + 0.1*abs(resid_tp->GetHistogram()->GetMaximum()) + 0.1;
//	cout<< "In makesave_E0ResiduCan(...) and we have yplotrange:  (" << yplotmin << ", " << yplotmax << ")" << endl;
	
	resid_tp -> GetYaxis() -> SetRangeUser(yplotmin, yplotmax);
	
	resid_tm ->Draw("same");
	resid_bp ->Draw("same");
	resid_bm ->Draw("same");
	gPad -> Update();
	
	pad1 -> cd();
	pad1 -> SetGridx();
//	datalabelr -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	gPad -> Update();
	
	f->cd();
	c_resid -> Write("",TObject::kOverwrite);
	
	cout << "Residucan is saved to file!" << endl;
}

void makesave_PseudoE0_Can(TFile * f, 
	set_of_fitvectors tp_fitvector, set_of_fitvectors bp_fitvector, set_of_fitvectors tm_fitvector, set_of_fitvectors bm_fitvector, 
	vector<double> x_avg, vector<double> x_err) 
{
	// --- // 
	int N = tp_fitvector.E0_vec.size();
	
	double y_est;
	vector<double> y_best_tp;
	vector<double> y_best_bp;
	vector<double> y_best_tm;
	vector<double> y_best_bm;
	
	for(int i=0; i<N; i++)
	{
		y_est = x_avg.at(i) - 300.0;
		
		y_best_tp.push_back( tp_fitvector.E0_vec.at(i)-y_est );
		y_best_bp.push_back( bp_fitvector.E0_vec.at(i)-y_est );
		y_best_tm.push_back( tm_fitvector.E0_vec.at(i)-y_est );
		y_best_bm.push_back( bm_fitvector.E0_vec.at(i)-y_est );
		
	}
	
	// new:
	TGraph * graph_tp_E0 = make_TGraphErrors(x_avg, y_best_tp, x_err, tp_fitvector.E0_err, tp_fitvector.color, "Pseudo E0:  Top (+)");  //
	TGraph * graph_tm_E0 = make_TGraphErrors(x_avg, y_best_tm, x_err, tm_fitvector.E0_err, tm_fitvector.color, "Pseudo E0:  Top (-)");  //
	TGraph * graph_bp_E0 = make_TGraphErrors(x_avg, y_best_bp, x_err, bp_fitvector.E0_err, bp_fitvector.color, "Pseudo E0:  Bottom (+)");  //
	TGraph * graph_bm_E0 = make_TGraphErrors(x_avg, y_best_bm, x_err, bm_fitvector.E0_err, bm_fitvector.color, "Pseudo E0:  Bottom (-)");  //
	
	TF1 * f_pseudoE = makedist_E0();
	
	TCanvas * c_pseudoE = new TCanvas("PseudoE Canvas", "PseudoE Canvas", 100, 0, 900, 700);
	c_pseudoE->cd();
	graph_bp_E0 -> Draw();
	graph_bp_E0 -> SetTitle("PseudoE Results");
	graph_bp_E0 -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_E0 -> Draw("same");
	graph_bm_E0 -> Draw("same");
	graph_tm_E0 -> Draw("same");
	f_pseudoE -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	TLegend * myLegend;
	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_E0, graph_tp_E0->GetName(), "l");
	myLegend -> AddEntry(graph_tm_E0, graph_tm_E0->GetName(), "l");
	myLegend -> AddEntry(graph_bp_E0, graph_bp_E0->GetName(), "l");
	myLegend -> AddEntry(graph_bm_E0, graph_bm_E0->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();
	
	f->cd();
	c_pseudoE -> Write("",TObject::kOverwrite);
	graph_tp_E0 -> Write("",TObject::kOverwrite);
	graph_tm_E0 -> Write("",TObject::kOverwrite);
	graph_bp_E0 -> Write("",TObject::kOverwrite);
	graph_bm_E0 -> Write("",TObject::kOverwrite);
	
	cout << "PseudoE Canvas Saved to file!" << endl;
}


void makesave_fitresultgraphs_and_cans(TFile * f, bool rcut) 
{
	// --- // 
	vector<double> x_avg;
	vector<double> x_err;
	
	set_of_fitvectors tp_fitvector;
	set_of_fitvectors bp_fitvector;
	set_of_fitvectors tm_fitvector;
	set_of_fitvectors bm_fitvector;
	tp_fitvector.clear_all();
	bp_fitvector.clear_all();
	tm_fitvector.clear_all();
	bm_fitvector.clear_all();
	
	// set colors *after* they've been cleared.
	tp_fitvector.color = int(mOrange);
	bp_fitvector.color = int(mGreen);
	tm_fitvector.color = int(kAzure);
	bm_fitvector.color = int(kMagenta);
	
	// --- // 
	for(int i=1; i<the_energyset.size(); i++)  // don't use the *first* value in the thingy, which is "full spectrum".
	{
		set_of_fitresult_pointers the_fitresults;
		the_fitresults = get_fitresults(the_energyset.at(i).the_double, rcut);  // energy, rcut.
		
		TFitResultPtr fr_tp = the_fitresults.pointer_tp;
		TFitResultPtr fr_bp = the_fitresults.pointer_bp;
		TFitResultPtr fr_tm = the_fitresults.pointer_tm;
		TFitResultPtr fr_bm = the_fitresults.pointer_bm;
		
		// don't use this point for fucking anything if the MinFCN is infinity.  
		if( isinf( fr_tp->MinFcnValue()) ) { continue; }
		if( isinf( fr_bp->MinFcnValue()) ) { continue; }
		if( isinf( fr_tm->MinFcnValue()) ) { continue; }
		if( isinf( fr_bm->MinFcnValue()) ) { continue; }
				
		x_avg.push_back(the_energyset.at(i).the_int);
		x_err.push_back(0);
		
		tp_fitvector.increment_withfitpointer(fr_tp);
		bp_fitvector.increment_withfitpointer(fr_bp);
		tm_fitvector.increment_withfitpointer(fr_tm);
		bm_fitvector.increment_withfitpointer(fr_bm);
	}
	
	makesave_fitresultgraphs_and_cans(f, tp_fitvector, bp_fitvector, tm_fitvector, bm_fitvector, x_avg, x_err);
}

void makesave_fitresultgraphs_and_cans(TFile * f, 
	set_of_fitvectors tp_fitvector, set_of_fitvectors bp_fitvector, set_of_fitvectors tm_fitvector, set_of_fitvectors bm_fitvector, 
	vector<double> x_avg, vector<double> x_err) 
{
	// --- // 
	// ok, so now I have lots of vectors of things that can have graphs made of them.	
	TGraph * graph_tp_E0 = make_TGraphErrors(x_avg, tp_fitvector.E0_vec, x_err, tp_fitvector.E0_err, tp_fitvector.color, "E0:  Top (+)");  //
	TGraph * graph_tm_E0 = make_TGraphErrors(x_avg, tm_fitvector.E0_vec, x_err, tm_fitvector.E0_err, tm_fitvector.color, "E0:  Top (-)");  //
	TGraph * graph_bp_E0 = make_TGraphErrors(x_avg, bp_fitvector.E0_vec, x_err, bp_fitvector.E0_err, bp_fitvector.color, "E0:  Bottom (+)");  //
	TGraph * graph_bm_E0 = make_TGraphErrors(x_avg, bm_fitvector.E0_vec, x_err, bm_fitvector.E0_err, bm_fitvector.color, "E0:  Bottom (-)");  //
	//
	TGraph * graph_tp_norm = make_TGraphErrors(x_avg, tp_fitvector.norm_vec, x_err, tp_fitvector.norm_err, tp_fitvector.color, "Norm:  Top (+)");
	TGraph * graph_tm_norm = make_TGraphErrors(x_avg, tm_fitvector.norm_vec, x_err, tm_fitvector.norm_err, tm_fitvector.color, "Norm:  Top (-)");
	TGraph * graph_bp_norm = make_TGraphErrors(x_avg, bp_fitvector.norm_vec, x_err, bp_fitvector.norm_err, bp_fitvector.color, "Norm:  Bottom (+)");
	TGraph * graph_bm_norm = make_TGraphErrors(x_avg, bm_fitvector.norm_vec, x_err, bm_fitvector.norm_err, bm_fitvector.color, "Norm:  Bottom (-)");
	TF1 * f_norm_tp = makedist_norm_tp();
	TF1 * f_norm_bp = makedist_norm_bp();
	TF1 * f_norm_tm = makedist_norm_tm();
	TF1 * f_norm_bm = makedist_norm_bm();
	
	//
	TGraph * graph_tp_lres = make_TGraphErrors(x_avg, tp_fitvector.lres_vec, x_err, tp_fitvector.lres_err, tp_fitvector.color, "landau resolution:  Top (+)");
	TGraph * graph_tm_lres = make_TGraphErrors(x_avg, tm_fitvector.lres_vec, x_err, tm_fitvector.lres_err, tm_fitvector.color, "landau resolution:  Top (-)");
	TGraph * graph_bp_lres = make_TGraphErrors(x_avg, bp_fitvector.lres_vec, x_err, bp_fitvector.lres_err, bp_fitvector.color, "landau resolution:  Bottom (+)");
	TGraph * graph_bm_lres = make_TGraphErrors(x_avg, bm_fitvector.lres_vec, x_err, bm_fitvector.lres_err, bm_fitvector.color, "landau resolution:  Bottom (-)");
	TF1 * f_lres = makedist_lres();
	
	//
	TGraph * graph_tp_gres = make_TGraphErrors(x_avg, tp_fitvector.gres_vec, x_err, tp_fitvector.gres_err, tp_fitvector.color, "gres:  Top (+)");
	TGraph * graph_tm_gres = make_TGraphErrors(x_avg, tm_fitvector.gres_vec, x_err, tm_fitvector.gres_err, tm_fitvector.color, "gres:  Top (-)");
	TGraph * graph_bp_gres = make_TGraphErrors(x_avg, bp_fitvector.gres_vec, x_err, bp_fitvector.gres_err, bp_fitvector.color, "gres:  Bottom (+)");
	TGraph * graph_bm_gres = make_TGraphErrors(x_avg, bm_fitvector.gres_vec, x_err, bm_fitvector.gres_err, bm_fitvector.color, "gres:  Bottom (-)");
	TF1 * f_gres = makedist_gres();
	
	//
	TGraph * graph_tp_k = make_TGraphErrors(x_avg, tp_fitvector.k_vec, x_err, tp_fitvector.k_err, tp_fitvector.color, "k:  Top (+)");
	TGraph * graph_tm_k = make_TGraphErrors(x_avg, tm_fitvector.k_vec, x_err, tm_fitvector.k_err, tm_fitvector.color, "k:  Top (-)");
	TGraph * graph_bp_k = make_TGraphErrors(x_avg, bp_fitvector.k_vec, x_err, bp_fitvector.k_err, bp_fitvector.color, "k:  Bottom (+)");
	TGraph * graph_bm_k = make_TGraphErrors(x_avg, bm_fitvector.k_vec, x_err, bm_fitvector.k_err, bm_fitvector.color, "k:  Bottom (-)");
	TF1 * f_k = makedist_k();
	
	//
	TGraph * graph_tp_W = make_TGraphErrors(x_avg, tp_fitvector.W_vec, x_err, tp_fitvector.W_err, tp_fitvector.color, "W:  Top (+)");
	TGraph * graph_tm_W = make_TGraphErrors(x_avg, tm_fitvector.W_vec, x_err, tm_fitvector.W_err, tm_fitvector.color, "W:  Top (-)");
	TGraph * graph_bp_W = make_TGraphErrors(x_avg, bp_fitvector.W_vec, x_err, bp_fitvector.W_err, bp_fitvector.color, "W:  Bottom (+)");
	TGraph * graph_bm_W = make_TGraphErrors(x_avg, bm_fitvector.W_vec, x_err, bm_fitvector.W_err, bm_fitvector.color, "W:  Bottom (-)");
	TF1 * f_W = makedist_W();
	
	//
	TGraph * graph_tp_alpha = make_TGraphErrors(x_avg, tp_fitvector.alpha_vec, x_err, tp_fitvector.alpha_err, tp_fitvector.color, "alpha:  Top (+)");
	TGraph * graph_tm_alpha = make_TGraphErrors(x_avg, tm_fitvector.alpha_vec, x_err, tm_fitvector.alpha_err, tm_fitvector.color, "alpha:  Top (-)");
	TGraph * graph_bp_alpha = make_TGraphErrors(x_avg, bp_fitvector.alpha_vec, x_err, bp_fitvector.alpha_err, bp_fitvector.color, "alpha:  Bottom (+)");
	TGraph * graph_bm_alpha = make_TGraphErrors(x_avg, bm_fitvector.alpha_vec, x_err, bm_fitvector.alpha_err, bm_fitvector.color, "alpha:  Bottom (-)");
	TF1 * f_alpha_more = makedist_alpha_more();
	TF1 * f_alpha_less = makedist_alpha_less();
	//
	TGraph * graph_tp_beta = make_TGraphErrors(x_avg, tp_fitvector.beta_vec, x_err, tp_fitvector.beta_err, tp_fitvector.color, "beta:  Top (+)");
	TGraph * graph_tm_beta = make_TGraphErrors(x_avg, tm_fitvector.beta_vec, x_err, tm_fitvector.beta_err, tm_fitvector.color, "beta:  Top (-)");
	TGraph * graph_bp_beta = make_TGraphErrors(x_avg, bp_fitvector.beta_vec, x_err, bp_fitvector.beta_err, bp_fitvector.color, "beta:  Bottom (+)");
	TGraph * graph_bm_beta = make_TGraphErrors(x_avg, bm_fitvector.beta_vec, x_err, bm_fitvector.beta_err, bm_fitvector.color, "beta:  Bottom (-)");
	TF1 * f_beta = makedist_beta();
	//
	TGraph * graph_tp_gamma = make_TGraphErrors(x_avg, tp_fitvector.gamma_vec, x_err, tp_fitvector.gamma_err, tp_fitvector.color, "gamma:  Top (+)");
	TGraph * graph_tm_gamma = make_TGraphErrors(x_avg, tm_fitvector.gamma_vec, x_err, tm_fitvector.gamma_err, tm_fitvector.color, "gamma:  Top (-)");
	TGraph * graph_bp_gamma = make_TGraphErrors(x_avg, bp_fitvector.gamma_vec, x_err, bp_fitvector.gamma_err, bp_fitvector.color, "gamma:  Bottom (+)");
	TGraph * graph_bm_gamma = make_TGraphErrors(x_avg, bm_fitvector.gamma_vec, x_err, bm_fitvector.gamma_err, bm_fitvector.color, "gamma:  Bottom (-)");
	TF1 * f_gamma_more = makedist_gamma_more();
	TF1 * f_gamma_less = makedist_gamma_less();
	//
	TGraph * graph_tp_delta = make_TGraphErrors(x_avg, tp_fitvector.delta_vec, x_err, tp_fitvector.delta_err, tp_fitvector.color, "delta:  Top (+)");
	TGraph * graph_tm_delta = make_TGraphErrors(x_avg, tm_fitvector.delta_vec, x_err, tm_fitvector.delta_err, tm_fitvector.color, "delta:  Top (-)");
	TGraph * graph_bp_delta = make_TGraphErrors(x_avg, bp_fitvector.delta_vec, x_err, bp_fitvector.delta_err, bp_fitvector.color, "delta:  Bottom (+)");
	TGraph * graph_bm_delta = make_TGraphErrors(x_avg, bm_fitvector.delta_vec, x_err, bm_fitvector.delta_err, bm_fitvector.color, "delta:  Bottom (-)");
	TF1 * f_delta_top    = makedist_delta_top();
	TF1 * f_delta_bottom = makedist_delta_bottom();
	
	//
	TGraph * graph_tp_fcn = make_TGraphErrors(x_avg, tp_fitvector.fcn_vec, x_err, x_err, tp_fitvector.color, "MinFCN:  Top (+)");
	TGraph * graph_tm_fcn = make_TGraphErrors(x_avg, tm_fitvector.fcn_vec, x_err, x_err, tm_fitvector.color, "MinFCN:  Top (-)");
	TGraph * graph_bp_fcn = make_TGraphErrors(x_avg, bp_fitvector.fcn_vec, x_err, x_err, bp_fitvector.color, "MinFCN:  Bottom (+)");
	TGraph * graph_bm_fcn = make_TGraphErrors(x_avg, bm_fitvector.fcn_vec, x_err, x_err, bm_fitvector.color, "MinFCN:  Bottom (-)");
	//
	
	// 511s:
	TGraph * graph_tp_scale = make_TGraphErrors(x_avg, tp_fitvector.scale_vec, x_err, tp_fitvector.scale_err, tp_fitvector.color, "scale:  Top (+)");
	TGraph * graph_tm_scale = make_TGraphErrors(x_avg, tm_fitvector.scale_vec, x_err, tm_fitvector.scale_err, tm_fitvector.color, "scale:  Top (-)");
	TGraph * graph_bp_scale = make_TGraphErrors(x_avg, bp_fitvector.scale_vec, x_err, bp_fitvector.scale_err, bp_fitvector.color, "scale:  Bottom (+)");
	TGraph * graph_bm_scale = make_TGraphErrors(x_avg, bm_fitvector.scale_vec, x_err, bm_fitvector.scale_err, bm_fitvector.color, "scale:  Bottom (-)");
	TF1 * f_scale_more = makedist_scale_more();
	TF1 * f_scale_less = makedist_scale_less();
	
	TGraph * graph_tp_gsize = make_TGraphErrors(x_avg, tp_fitvector.gsize_vec, x_err, tp_fitvector.gsize_err, tp_fitvector.color, "Gaussian Size:  Top (+)");
	TGraph * graph_tm_gsize = make_TGraphErrors(x_avg, tm_fitvector.gsize_vec, x_err, tm_fitvector.gsize_err, tm_fitvector.color, "Gaussian Size:  Top (-)");
	TGraph * graph_bp_gsize = make_TGraphErrors(x_avg, bp_fitvector.gsize_vec, x_err, bp_fitvector.gsize_err, bp_fitvector.color, "Gaussian Size:  Bottom (+)");
	TGraph * graph_bm_gsize = make_TGraphErrors(x_avg, bm_fitvector.gsize_vec, x_err, bm_fitvector.gsize_err, bm_fitvector.color, "Gaussian Size:  Bottom (-)");
	TF1 * f_gfrac = makedist_gfrac();
	
	TGraph * graph_tp_dE = make_TGraphErrors(x_avg, tp_fitvector.dE_vec, x_err, tp_fitvector.dE_err, tp_fitvector.color, "dE:  Top (+)");
	TGraph * graph_tm_dE = make_TGraphErrors(x_avg, tm_fitvector.dE_vec, x_err, tm_fitvector.dE_err, tm_fitvector.color, "dE:  Top (-)");
	TGraph * graph_bp_dE = make_TGraphErrors(x_avg, bp_fitvector.dE_vec, x_err, bp_fitvector.dE_err, bp_fitvector.color, "dE:  Bottom (+)");
	TGraph * graph_bm_dE = make_TGraphErrors(x_avg, bm_fitvector.dE_vec, x_err, bm_fitvector.dE_err, bm_fitvector.color, "dE:  Bottom (-)");
	TF1 * f_dE0 = makedist_dE0();
	
	// DgE:  the extra offset.
	TGraph * graph_tp_Dgcenter = make_TGraphErrors(x_avg, tp_fitvector.Dgcenter_vec, x_err, tp_fitvector.Dgcenter_err, tp_fitvector.color, "Dgcenter:  Top (+)");
	TGraph * graph_tm_Dgcenter = make_TGraphErrors(x_avg, tm_fitvector.Dgcenter_vec, x_err, tm_fitvector.Dgcenter_err, tm_fitvector.color, "Dgcenter:  Top (-)");
	TGraph * graph_bp_Dgcenter = make_TGraphErrors(x_avg, bp_fitvector.Dgcenter_vec, x_err, bp_fitvector.Dgcenter_err, bp_fitvector.color, "Dgcenter:  Bottom (+)");
	TGraph * graph_bm_Dgcenter = make_TGraphErrors(x_avg, bm_fitvector.Dgcenter_vec, x_err, bm_fitvector.Dgcenter_err, bm_fitvector.color, "Dgcenter:  Bottom (-)");
	TF1 * f_DgE = makedist_DgE();
	
	// toeres:
	TGraph * graph_tp_toeres = make_TGraphErrors(x_avg, tp_fitvector.toeres_vec, x_err, tp_fitvector.toeres_err, tp_fitvector.color, "toeres:  Top (+)");
	TGraph * graph_tm_toeres = make_TGraphErrors(x_avg, tm_fitvector.toeres_vec, x_err, tm_fitvector.toeres_err, tm_fitvector.color, "toeres:  Top (-)");
	TGraph * graph_bp_toeres = make_TGraphErrors(x_avg, bp_fitvector.toeres_vec, x_err, bp_fitvector.toeres_err, bp_fitvector.color, "toeres:  Bottom (+)");
	TGraph * graph_bm_toeres = make_TGraphErrors(x_avg, bm_fitvector.toeres_vec, x_err, bm_fitvector.toeres_err, bm_fitvector.color, "toeres:  Bottom (-)");
	TF1 * f_toeres_top    = makedist_toeres_top();
	TF1 * f_toeres_bottom = makedist_toeres_bottom();
	
	// count params:
	TGraph * graph_tp_countin = make_TGraphErrors(x_avg, tp_fitvector.countin_vec, x_err, x_err, tp_fitvector.color, "countin:  Top (+)");
	TGraph * graph_tm_countin = make_TGraphErrors(x_avg, tm_fitvector.countin_vec, x_err, x_err, tm_fitvector.color, "countin:  Top (-)");
	TGraph * graph_bp_countin = make_TGraphErrors(x_avg, bp_fitvector.countin_vec, x_err, x_err, bp_fitvector.color, "countin:  Bottom (+)");
	TGraph * graph_bm_countin = make_TGraphErrors(x_avg, bm_fitvector.countin_vec, x_err, x_err, bm_fitvector.color, "countin:  Bottom (-)");

	TGraph * graph_tp_countout = make_TGraphErrors(x_avg, tp_fitvector.countout_vec, x_err, x_err, tp_fitvector.color, "countout:  Top (+)");
	TGraph * graph_tm_countout = make_TGraphErrors(x_avg, tm_fitvector.countout_vec, x_err, x_err, tm_fitvector.color, "countout:  Top (-)");
	TGraph * graph_bp_countout = make_TGraphErrors(x_avg, bp_fitvector.countout_vec, x_err, x_err, bp_fitvector.color, "countout:  Bottom (+)");
	TGraph * graph_bm_countout = make_TGraphErrors(x_avg, bm_fitvector.countout_vec, x_err, x_err, bm_fitvector.color, "countout:  Bottom (-)");

	TGraph * graph_tp_countint = make_TGraphErrors(x_avg, tp_fitvector.countint_vec, x_err, x_err, tp_fitvector.color, "countint:  Top (+)");
	TGraph * graph_tm_countint = make_TGraphErrors(x_avg, tm_fitvector.countint_vec, x_err, x_err, tm_fitvector.color, "countint:  Top (-)");
	TGraph * graph_bp_countint = make_TGraphErrors(x_avg, bp_fitvector.countint_vec, x_err, x_err, bp_fitvector.color, "countint:  Bottom (+)");
	TGraph * graph_bm_countint = make_TGraphErrors(x_avg, bm_fitvector.countint_vec, x_err, x_err, bm_fitvector.color, "countint:  Bottom (-)");
	
	TGraph * graph_tp_relnorm = make_TGraphErrors(x_avg, tp_fitvector.relnorm_vec, x_err, x_err, tp_fitvector.color, "relnorm:  Top (+)");
	TGraph * graph_tm_relnorm = make_TGraphErrors(x_avg, tm_fitvector.relnorm_vec, x_err, x_err, tm_fitvector.color, "relnorm:  Top (-)");
	TGraph * graph_bp_relnorm = make_TGraphErrors(x_avg, bp_fitvector.relnorm_vec, x_err, x_err, bp_fitvector.color, "relnorm:  Bottom (+)");
	TGraph * graph_bm_relnorm = make_TGraphErrors(x_avg, bm_fitvector.relnorm_vec, x_err, x_err, bm_fitvector.color, "relnorm:  Bottom (-)");
	TF1 * f_relnorm_tp = make_relnorm_tp();
	TF1 * f_relnorm_tm = make_relnorm_tm();
	TF1 * f_relnorm_bp = make_relnorm_bp();
	TF1 * f_relnorm_bm = make_relnorm_bm();
	
	// aaaand the extras:
	TGraph * graph_tp_chi2 = make_TGraphErrors(x_avg, tp_fitvector.chi2_vec, x_err, x_err, tp_fitvector.color, "Chi^2:  Top (+)");
	TGraph * graph_tm_chi2 = make_TGraphErrors(x_avg, tm_fitvector.chi2_vec, x_err, x_err, tm_fitvector.color, "Chi^2:  Top (-)");
	TGraph * graph_bp_chi2 = make_TGraphErrors(x_avg, bp_fitvector.chi2_vec, x_err, x_err, bp_fitvector.color, "Chi^2:  Bottom (+)");
	TGraph * graph_bm_chi2 = make_TGraphErrors(x_avg, bm_fitvector.chi2_vec, x_err, x_err, bm_fitvector.color, "Chi^2:  Bottom (-)");
	//
	TGraph * graph_tp_redchi2 = make_TGraphErrors(x_avg, tp_fitvector.redchi2_vec, x_err, x_err, tp_fitvector.color, "Chi^2/NDF:  Top (+)");
	TGraph * graph_tm_redchi2 = make_TGraphErrors(x_avg, tm_fitvector.redchi2_vec, x_err, x_err, tm_fitvector.color, "Chi^2/NDF:  Top (-)");
	TGraph * graph_bp_redchi2 = make_TGraphErrors(x_avg, bp_fitvector.redchi2_vec, x_err, x_err, bp_fitvector.color, "Chi^2/NDF:  Bottom (+)");
	TGraph * graph_bm_redchi2 = make_TGraphErrors(x_avg, bm_fitvector.redchi2_vec, x_err, x_err, bm_fitvector.color, "Chi^2/NDF:  Bottom (-)");
	
	TGraph * graph_tp_pval = make_TGraphErrors(x_avg, tp_fitvector.P_vec, x_err, x_err, tp_fitvector.color, "P-value:  Top (+)");
	TGraph * graph_tm_pval = make_TGraphErrors(x_avg, tm_fitvector.P_vec, x_err, x_err, tm_fitvector.color, "P-value:  Top (-)");
	TGraph * graph_bp_pval = make_TGraphErrors(x_avg, bp_fitvector.P_vec, x_err, x_err, bp_fitvector.color, "P-value:  Bottom (+)");
	TGraph * graph_bm_pval = make_TGraphErrors(x_avg, bm_fitvector.P_vec, x_err, x_err, bm_fitvector.color, "P-value:  Bottom (-)");
	
//	cout << "...and here we are about to draw a bunch of shit on canvases!" << endl;
	// --- // 
	TLegend * myLegend;
	TText *datalabel = new TText();
	datalabel -> SetNDC();
	datalabel -> SetTextColor(1);
	datalabel -> SetTextSize(0.018);
	
	TCanvas * c_norm = new TCanvas("Norm Canvas", "Norm Canvas", 100, 0, 900, 700);
	c_norm->cd();
	graph_tm_norm -> Draw();
	graph_tm_norm -> SetTitle("Norm Fits");
	graph_tm_norm -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_norm -> Draw("same");
	graph_bp_norm -> Draw("same");
	graph_bm_norm -> Draw("same");
	f_norm_tp -> Draw("same");
	f_norm_bp -> Draw("same");
	f_norm_tm -> Draw("same");
	f_norm_bm -> Draw("same");
	
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_norm, graph_tp_norm->GetName(), "l");
	myLegend -> AddEntry(graph_tm_norm, graph_tm_norm->GetName(), "l");
	myLegend -> AddEntry(graph_bp_norm, graph_bp_norm->GetName(), "l");
	myLegend -> AddEntry(graph_bm_norm, graph_bm_norm->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();
	
	double lambda_top    = 1.42;
	double lambda_bottom = 1.32;
	
	TCanvas * c_lres = new TCanvas("LandauLambda Canvas", "LandauLambda Canvas", 100, 0, 900, 700);
	c_lres->cd();
	graph_tm_lres -> Draw();
	graph_tm_lres -> SetTitle("Landau 'Resolution'");
//	graph_tm_lres -> GetYaxis() -> SetRangeUser(lambda_bottom*0.5, lambda_top*1.5);
	graph_tm_lres -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_lres -> Draw("same");
	graph_bp_lres -> Draw("same");
	graph_bm_lres -> Draw("same");
	f_lres -> Draw("same");
	
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_lres, graph_tp_lres->GetName(), "l");
	myLegend -> AddEntry(graph_tm_lres, graph_tm_lres->GetName(), "l");
	myLegend -> AddEntry(graph_bp_lres, graph_bp_lres->GetName(), "l");
	myLegend -> AddEntry(graph_bm_lres, graph_bm_lres->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();

	TCanvas * c_gres = new TCanvas("Gaussian Resolution Canvas", "Gaussian Resolution Canvas", 100, 0, 900, 700);
	c_gres->cd();
	graph_tm_gres -> Draw();
	graph_tm_gres -> SetTitle("Gaussian 'Resolution'");
//	graph_tm_gres -> GetYaxis() -> SetRangeUser(lambda_bottom*0.5, lambda_top*1.5);
	graph_tm_gres -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_gres -> Draw("same");
	graph_bp_gres -> Draw("same");
	graph_bm_gres -> Draw("same");
	f_gres -> Draw("same");
	
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_gres, graph_tp_gres->GetName(), "l");
	myLegend -> AddEntry(graph_tm_gres, graph_tm_gres->GetName(), "l");
	myLegend -> AddEntry(graph_bp_gres, graph_bp_gres->GetName(), "l");
	myLegend -> AddEntry(graph_bm_gres, graph_bm_gres->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();

	TCanvas * c_k = new TCanvas("k Canvas", "k Canvas", 100, 0, 900, 700);
	c_k->cd();
	graph_tm_k -> Draw();
	graph_tm_k -> SetTitle("k Fits");
	graph_tp_k -> Draw("same");
	graph_bp_k -> Draw("same");
	graph_bm_k -> Draw("same");
	f_k -> Draw("same");
	
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend = new TLegend(.80,.16,.99,.30,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_k, graph_tp_k->GetName(), "l");
	myLegend -> AddEntry(graph_tm_k, graph_tm_k->GetName(), "l");
	myLegend -> AddEntry(graph_bp_k, graph_bp_k->GetName(), "l");
	myLegend -> AddEntry(graph_bm_k, graph_bm_k->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();

	TCanvas * c_W = new TCanvas("W Canvas", "W Canvas", 100, 0, 900, 700);
	c_W->cd();
	graph_bm_W -> Draw();
	graph_bm_W -> SetTitle("W Fits");
	graph_bm_W -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tm_W -> Draw("same");
	graph_bp_W -> Draw("same");
	graph_tp_W -> Draw("same");
	f_W -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_W, graph_tp_W->GetName(), "l");
	myLegend -> AddEntry(graph_tm_W, graph_tm_W->GetName(), "l");
	myLegend -> AddEntry(graph_bp_W, graph_bp_W->GetName(), "l");
	myLegend -> AddEntry(graph_bm_W, graph_bm_W->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();

	TCanvas * c_alpha = new TCanvas("Alpha Canvas", "Alpha Canvas", 100, 0, 900, 700);
	c_alpha->cd();
	graph_bp_alpha -> Draw();
	graph_bp_alpha -> SetTitle("Alpha Fits");
	graph_bm_alpha -> Draw("same");
	graph_tp_alpha -> Draw("same");
	graph_bp_alpha -> Draw("same");
	graph_tm_alpha -> Draw("same");
	f_alpha_more -> Draw("same");
	f_alpha_less -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_alpha, graph_tp_alpha->GetName(), "l");
	myLegend -> AddEntry(graph_tm_alpha, graph_tm_alpha->GetName(), "l");
	myLegend -> AddEntry(graph_bp_alpha, graph_bp_alpha->GetName(), "l");
	myLegend -> AddEntry(graph_bm_alpha, graph_bm_alpha->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();
//	//
	TCanvas * c_beta = new TCanvas("Beta Canvas", "Beta Canvas", 100, 0, 900, 700);
	c_beta->cd();
	graph_tp_beta -> Draw();
	graph_tp_beta -> SetTitle("Beta Fits");
	graph_tm_beta -> Draw("same");
	graph_bp_beta -> Draw("same");
	graph_bm_beta -> Draw("same");
	f_beta -> Draw("same");
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend = new TLegend(.80,.16,.99,.30,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_beta, graph_tp_beta->GetName(), "l");
	myLegend -> AddEntry(graph_tm_beta, graph_tm_beta->GetName(), "l");
	myLegend -> AddEntry(graph_bp_beta, graph_bp_beta->GetName(), "l");
	myLegend -> AddEntry(graph_bm_beta, graph_bm_beta->GetName(), "l");
	myLegend -> Draw();
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	gPad->Update();
	//
	TCanvas * c_gamma = new TCanvas("Gamma Canvas", "Gamma Canvas", 100, 0, 900, 700);
	c_gamma->cd();
	graph_tp_gamma -> Draw();
	graph_tp_gamma -> SetTitle("Gamma Fits");
	graph_tp_gamma -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tm_gamma -> Draw("same");
	graph_bp_gamma -> Draw("same");
	graph_bm_gamma -> Draw("same");
	f_gamma_more -> Draw("same");
	f_gamma_less -> Draw("same");
	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
//	myLegend = new TLegend(.80,.16,.99,.30,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_gamma, graph_tp_gamma->GetName(), "l");
	myLegend -> AddEntry(graph_tm_gamma, graph_tm_gamma->GetName(), "l");
	myLegend -> AddEntry(graph_bp_gamma, graph_bp_gamma->GetName(), "l");
	myLegend -> AddEntry(graph_bm_gamma, graph_bm_gamma->GetName(), "l");
	myLegend -> Draw();
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	gPad->Update();

	TCanvas * c_delta = new TCanvas("Delta Canvas", "Delta Canvas", 100, 0, 900, 700);
	c_delta->cd();
	graph_bp_delta -> Draw();
	graph_bp_delta -> SetTitle("Delta Fits");
	graph_bp_delta -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_delta -> Draw("same");
	graph_bm_delta -> Draw("same");
	graph_tm_delta -> Draw("same");
	f_delta_top    -> Draw("same");
	f_delta_bottom -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_delta, graph_tp_delta->GetName(), "lp");
	myLegend -> AddEntry(graph_tm_delta, graph_tm_delta->GetName(), "lp");
	myLegend -> AddEntry(graph_bp_delta, graph_bp_delta->GetName(), "lp");
	myLegend -> AddEntry(graph_bm_delta, graph_bm_delta->GetName(), "lp");
	myLegend -> Draw();
	gPad->Update();
	
//	cout << "We've finished Delta Canvas and will now move on to Scale Canvas." << endl;
	
	TCanvas * c_scale = new TCanvas("Scale Canvas", "Scale Canvas", 100, 0, 900, 700);
	c_scale->cd();
	graph_tm_scale -> Draw();
	graph_tm_scale -> SetTitle("Scale Fits");
	graph_tm_scale -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_scale -> Draw("same");
	graph_bp_scale -> Draw("same");
	graph_bm_scale -> Draw("same");
	f_scale_more -> Draw("same");
	f_scale_less -> Draw("same");
	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_scale, graph_tp_scale->GetName(), "l");
	myLegend -> AddEntry(graph_tm_scale, graph_tm_scale->GetName(), "l");
	myLegend -> AddEntry(graph_bp_scale, graph_bp_scale->GetName(), "l");
	myLegend -> AddEntry(graph_bm_scale, graph_bm_scale->GetName(), "l");
	myLegend -> Draw();
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	gPad->Update();
	//
	
	TCanvas * c_gsize = new TCanvas("gsize Canvas", "gsize Canvas", 100, 0, 900, 700);
	c_gsize->cd();
	graph_bp_gsize -> Draw();
	graph_bp_gsize -> SetTitle("GausFrac Fits");
	graph_tp_gsize -> Draw("same");
	graph_tm_gsize -> Draw("same");
	graph_bm_gsize -> Draw("same");
	f_gfrac -> Draw("same");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend = new TLegend(.80,.16,.99,.30,"");
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_gsize, graph_tp_gsize->GetName(), "l");
	myLegend -> AddEntry(graph_tm_gsize, graph_tm_gsize->GetName(), "l");
	myLegend -> AddEntry(graph_bp_gsize, graph_bp_gsize->GetName(), "l");
	myLegend -> AddEntry(graph_bm_gsize, graph_bm_gsize->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();	
	
	//
	TCanvas * c_dE = new TCanvas("dE Canvas", "dE Canvas", 100, 0, 900, 700);
	c_dE->cd();
	graph_bp_dE -> Draw();
	graph_bp_dE -> SetTitle("dE0 Fits (separation between gaussian and moyal)");
	graph_bp_dE -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_dE -> Draw("same");
	graph_bm_dE -> Draw("same");
	graph_tm_dE -> Draw("same");
	f_dE0 -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.80,.16,.99,.30,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_dE, graph_tp_dE->GetName(), "l");
	myLegend -> AddEntry(graph_tm_dE, graph_tm_dE->GetName(), "l");
	myLegend -> AddEntry(graph_bp_dE, graph_bp_dE->GetName(), "l");
	myLegend -> AddEntry(graph_bm_dE, graph_bm_dE->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();	
	
	
	TCanvas * c_dgE = new TCanvas("dgE Canvas", "dgE Canvas", 100, 0, 900, 700);
	c_dgE->cd();
	graph_bp_Dgcenter -> Draw();
	graph_bp_Dgcenter -> SetTitle("DgE Fits (offset of central peak from E0)");
	graph_bp_Dgcenter -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_Dgcenter -> Draw("same");
	graph_bm_Dgcenter -> Draw("same");
	graph_tm_Dgcenter -> Draw("same");
	f_DgE -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend = new TLegend(.80,.16,.99,.30,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_Dgcenter, graph_tp_Dgcenter->GetName(), "l");
	myLegend -> AddEntry(graph_tm_Dgcenter, graph_tm_Dgcenter->GetName(), "l");
	myLegend -> AddEntry(graph_bp_Dgcenter, graph_bp_Dgcenter->GetName(), "l");
	myLegend -> AddEntry(graph_bm_Dgcenter, graph_bm_Dgcenter->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();	
	
	TCanvas * c_toeres = new TCanvas("Toe Resolution Canvas", "Toe Resolution Canvas", 100, 0, 900, 700);
	c_toeres->cd();
	graph_bp_toeres -> Draw();
	graph_bp_toeres -> SetTitle("Toe Resolution Fits");
	graph_bp_toeres -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_toeres -> Draw("same");
	graph_bm_toeres -> Draw("same");
	graph_tm_toeres -> Draw("same");
	f_toeres_top    -> Draw("same");
	f_toeres_bottom -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_toeres, graph_tp_toeres->GetName(), "l");
	myLegend -> AddEntry(graph_tm_toeres, graph_tm_toeres->GetName(), "l");
	myLegend -> AddEntry(graph_bp_toeres, graph_bp_toeres->GetName(), "l");
	myLegend -> AddEntry(graph_bm_toeres, graph_bm_toeres->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();	
	
	// // // // // 
	TCanvas * c_rchi = new TCanvas("Chi2 Canvas", "Chi2 Canvas", 100, 0, 900, 700);
	c_rchi->cd();
	graph_tm_redchi2 -> SetTitle("Reduced Chi2 Results");
	graph_tm_redchi2 -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tm_redchi2 -> Draw();
	graph_tp_redchi2 -> Draw("same");
	graph_bm_redchi2 -> Draw("same");
	graph_bp_redchi2 -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_redchi2, graph_tp_redchi2->GetName(), "l");
	myLegend -> AddEntry(graph_tm_redchi2, graph_tm_redchi2->GetName(), "l");
	myLegend -> AddEntry(graph_bp_redchi2, graph_bp_redchi2->GetName(), "l");
	myLegend -> AddEntry(graph_bm_redchi2, graph_bm_redchi2->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();
	
//	cout << " ... and *that* was the Chi2 Canvas." << endl;

	TCanvas * c_fcn = new TCanvas("MinFCN Canvas", "MinFCN Canvas", 100, 0, 900, 700);
	c_fcn->cd();
	graph_bp_fcn -> Draw();
	graph_bp_fcn -> SetTitle("MinFCN Results");
	graph_bp_fcn -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_fcn -> Draw("same");
	graph_bm_fcn -> Draw("same");
	graph_tm_fcn -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_fcn, graph_tp_fcn->GetName(), "l");
	myLegend -> AddEntry(graph_tm_fcn, graph_tm_fcn->GetName(), "l");
	myLegend -> AddEntry(graph_bp_fcn, graph_bp_fcn->GetName(), "l");
	myLegend -> AddEntry(graph_bm_fcn, graph_bm_fcn->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();
	
//	cout << "That was the MinFCN canvas." << endl;
	
	TCanvas * c_p = new TCanvas("P-Value Canvas", "P-Value Canvas", 100, 0, 900, 700);
	c_p->cd();
	c_p -> SetLogy();
	graph_bp_pval -> Draw();
	graph_bp_pval -> SetTitle("P-Value Results");
	graph_bp_pval -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_bp_pval -> GetYaxis() -> SetRangeUser(1e-3, 1.1);
	graph_tp_pval -> Draw("same");
	graph_bm_pval -> Draw("same");
	graph_tm_pval -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_pval, graph_tp_pval->GetName(), "l");
	myLegend -> AddEntry(graph_tm_pval, graph_tm_pval->GetName(), "l");
	myLegend -> AddEntry(graph_bp_pval, graph_bp_pval->GetName(), "l");
	myLegend -> AddEntry(graph_bm_pval, graph_bm_pval->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();
	
//	cout << "And that was the P-Value Canvas.  We're done making canvases, sort-of." << endl;
	
	TCanvas * c_in = new TCanvas("CountIn Canvas", "CountIn Canvas", 100, 0, 900, 700);
	c_in->cd();
//	c_in -> SetLogy();
	graph_bp_countin -> Draw();
	graph_bp_countin -> SetTitle("Naive Pyramid Counts In");
	graph_bp_countin -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_bp_countin -> GetYaxis() -> SetRangeUser(500e3, 2100e3);
	graph_tp_countin -> Draw("same");
	graph_bm_countin -> Draw("same");
	graph_tm_countin -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend = new TLegend(.80,.40,.99,.54,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_countin, graph_tp_countin->GetName(), "l");
	myLegend -> AddEntry(graph_tm_countin, graph_tm_countin->GetName(), "l");
	myLegend -> AddEntry(graph_bp_countin, graph_bp_countin->GetName(), "l");
	myLegend -> AddEntry(graph_bm_countin, graph_bm_countin->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();

	TCanvas * c_out = new TCanvas("CountOut Canvas", "CountOut Canvas", 100, 0, 900, 700);
	c_out->cd();
//	c_out -> SetLogy();
	graph_bp_countout -> Draw();
	graph_bp_countout -> SetTitle("Counts Out (from data)");
	graph_bp_countout -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_countout -> Draw("same");
	graph_bm_countout -> Draw("same");
	graph_tm_countout -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend = new TLegend(.80,.40,.99,.54,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_countout, graph_tp_countout->GetName(), "l");
	myLegend -> AddEntry(graph_tm_countout, graph_tm_countout->GetName(), "l");
	myLegend -> AddEntry(graph_bp_countout, graph_bp_countout->GetName(), "l");
	myLegend -> AddEntry(graph_bm_countout, graph_bm_countout->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();
	
	
	TCanvas * c_int = new TCanvas("Integrated CountOut Canvas", "Integrated CountOut Canvas", 100, 0, 900, 700);
	c_int->cd();
//	c_int -> SetLogy();
	graph_bp_countint -> Draw();
	graph_bp_countint -> SetTitle("Integrated Counts Out (from fit function)");
	graph_bp_countint -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_countint -> Draw("same");
	graph_bm_countint -> Draw("same");
	graph_tm_countint -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend = new TLegend(.80,.40,.99,.54,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_countint, graph_tp_countint->GetName(), "l");
	myLegend -> AddEntry(graph_tm_countint, graph_tm_countint->GetName(), "l");
	myLegend -> AddEntry(graph_bp_countint, graph_bp_countint->GetName(), "l");
	myLegend -> AddEntry(graph_bm_countint, graph_bm_countint->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();


	TCanvas * c_rn = new TCanvas("Relative Normalization Canvas", "Relative Normalization Canvas", 100, 0, 900, 700);
	c_rn->cd();
//	c_int -> SetLogy();
	graph_bp_relnorm -> Draw();
	graph_bp_relnorm -> SetTitle("Relative Normalization (N_out_integrated / N_in_pyramid)");
	graph_bp_relnorm -> GetXaxis() -> SetRangeUser(0, 5500);
	graph_tp_relnorm -> Draw("same");
	graph_bm_relnorm -> Draw("same");
	graph_tm_relnorm -> Draw("same");
	f_relnorm_tp -> Draw("same");
	f_relnorm_tm -> Draw("same");
	f_relnorm_bp -> Draw("same");
	f_relnorm_bm -> Draw("same");
//	datalabel -> DrawText(0.10, 0.908, __SHORT_FORM_OF_FILE__);
//	myLegend = new TLegend(.80,.80,.99,.99,"");
//	myLegend = new TLegend(.16,.80,.35,.99,"");
	myLegend = new TLegend(.80,.16,.99,.30,"");
	myLegend -> SetFillColor(0);
	myLegend -> AddEntry(graph_tp_relnorm, graph_tp_relnorm->GetName(), "l");
	myLegend -> AddEntry(graph_tm_relnorm, graph_tm_relnorm->GetName(), "l");
	myLegend -> AddEntry(graph_bp_relnorm, graph_bp_relnorm->GetName(), "l");
	myLegend -> AddEntry(graph_bm_relnorm, graph_bm_relnorm->GetName(), "l");
	myLegend -> Draw();
	gPad->Update();
	
	
	
	// --- // 
	// save 'em to a file now. 
	f->cd();
	
//	c_E0 -> Write("",TObject::kOverwrite);
	graph_tm_E0 -> Write("",TObject::kOverwrite);
	graph_tp_E0 -> Write("",TObject::kOverwrite);
	graph_bp_E0 -> Write("",TObject::kOverwrite);
	graph_bm_E0 -> Write("",TObject::kOverwrite);
	makesave_E0ResiduCan(f, tp_fitvector, bp_fitvector, tm_fitvector, bm_fitvector, x_avg, x_err);
	makesave_PseudoE0_Can(f, tp_fitvector, bp_fitvector, tm_fitvector, bm_fitvector, x_avg, x_err);
	cout << "E0 canvas(es) are done." << endl;
	
	
	// 
	c_norm -> Write("",TObject::kOverwrite);
	graph_tm_norm -> Write("",TObject::kOverwrite);
	graph_tp_norm -> Write("",TObject::kOverwrite);
	graph_bp_norm -> Write("",TObject::kOverwrite);
	graph_bm_norm -> Write("",TObject::kOverwrite);
	f_norm_tp -> Write("",TObject::kOverwrite);
	f_norm_bp -> Write("",TObject::kOverwrite);
	f_norm_tm -> Write("",TObject::kOverwrite);
	f_norm_bm -> Write("",TObject::kOverwrite);
	
	c_W -> Write("",TObject::kOverwrite);
	graph_tm_W -> Write("",TObject::kOverwrite);
	graph_tp_W -> Write("",TObject::kOverwrite);
	graph_bp_W -> Write("",TObject::kOverwrite);
	graph_bm_W -> Write("",TObject::kOverwrite);
	f_W -> Write("",TObject::kOverwrite);
	
	//
	c_k -> Write("",TObject::kOverwrite);
	graph_tm_k -> Write("",TObject::kOverwrite);
	graph_tp_k -> Write("",TObject::kOverwrite);
	graph_bp_k -> Write("",TObject::kOverwrite);
	graph_bm_k -> Write("",TObject::kOverwrite);
	f_k -> Write("",TObject::kOverwrite);

	c_scale->Write("",TObject::kOverwrite);
	graph_tm_scale -> Write("",TObject::kOverwrite);
	graph_tp_scale -> Write("",TObject::kOverwrite);
	graph_bp_scale -> Write("",TObject::kOverwrite);
	graph_bm_scale -> Write("",TObject::kOverwrite);
	f_scale_more -> Write("",TObject::kOverwrite);
	f_scale_less -> Write("",TObject::kOverwrite);
	
	c_alpha -> Write("",TObject::kOverwrite);
	graph_tm_alpha -> Write("",TObject::kOverwrite);
	graph_tp_alpha -> Write("",TObject::kOverwrite);
	graph_bp_alpha -> Write("",TObject::kOverwrite);
	graph_bm_alpha -> Write("",TObject::kOverwrite);
	f_alpha_more -> Write("",TObject::kOverwrite);
	f_alpha_less -> Write("",TObject::kOverwrite);
	
	c_beta -> Write("",TObject::kOverwrite);
	graph_tm_beta -> Write("",TObject::kOverwrite);
	graph_tp_beta -> Write("",TObject::kOverwrite);
	graph_bp_beta -> Write("",TObject::kOverwrite);
	graph_bm_beta -> Write("",TObject::kOverwrite);
	f_beta -> Write("",TObject::kOverwrite);
	
	c_gamma -> Write("",TObject::kOverwrite);
	graph_tm_gamma -> Write("",TObject::kOverwrite);
	graph_tp_gamma -> Write("",TObject::kOverwrite);
	graph_bp_gamma -> Write("",TObject::kOverwrite);
	graph_bm_gamma -> Write("",TObject::kOverwrite);
	f_gamma_more -> Write("",TObject::kOverwrite);
	f_gamma_less -> Write("",TObject::kOverwrite);
	
	c_delta -> Write("",TObject::kOverwrite);
	graph_tm_delta -> Write("",TObject::kOverwrite);
	graph_tp_delta -> Write("",TObject::kOverwrite);
	graph_bp_delta -> Write("",TObject::kOverwrite);
	graph_bm_delta -> Write("",TObject::kOverwrite);
	f_delta_top    -> Write("",TObject::kOverwrite);
	f_delta_bottom -> Write("",TObject::kOverwrite);
	
	c_gsize->Write("",TObject::kOverwrite);
	graph_tp_gsize->Write("",TObject::kOverwrite);
	graph_tm_gsize->Write("",TObject::kOverwrite);
	graph_bp_gsize->Write("",TObject::kOverwrite);
	graph_bm_gsize->Write("",TObject::kOverwrite);
	f_gfrac -> Write("",TObject::kOverwrite);
	
	c_toeres->Write("",TObject::kOverwrite);
	graph_tp_toeres->Write("",TObject::kOverwrite);
	graph_tm_toeres->Write("",TObject::kOverwrite);
	graph_bp_toeres->Write("",TObject::kOverwrite);
	graph_bm_toeres->Write("",TObject::kOverwrite);
	f_toeres_top    -> Write("",TObject::kOverwrite);
	f_toeres_bottom -> Write("",TObject::kOverwrite);
	
	c_lres -> Write("",TObject::kOverwrite);
	graph_tm_lres -> Write("",TObject::kOverwrite);
	graph_tp_lres -> Write("",TObject::kOverwrite);
	graph_bp_lres -> Write("",TObject::kOverwrite);
	graph_bm_lres -> Write("",TObject::kOverwrite);
	f_lres -> Write("",TObject::kOverwrite);
	
	c_gres -> Write("",TObject::kOverwrite);
	graph_tm_gres -> Write("",TObject::kOverwrite);
	graph_tp_gres -> Write("",TObject::kOverwrite);
	graph_bp_gres -> Write("",TObject::kOverwrite);
	graph_bm_gres -> Write("",TObject::kOverwrite);
	f_gres -> Write("",TObject::kOverwrite);
	
	c_dE->Write("",TObject::kOverwrite);
	graph_tp_dE->Write("",TObject::kOverwrite);
	graph_tm_dE->Write("",TObject::kOverwrite);
	graph_bp_dE->Write("",TObject::kOverwrite);
	graph_bm_dE->Write("",TObject::kOverwrite);
	f_dE0 -> Write("",TObject::kOverwrite);
	
	c_dgE->Write("",TObject::kOverwrite);
	graph_bp_Dgcenter->Write("",TObject::kOverwrite);
	graph_tp_Dgcenter->Write("",TObject::kOverwrite);
	graph_bm_Dgcenter->Write("",TObject::kOverwrite);
	graph_tm_Dgcenter->Write("",TObject::kOverwrite);
	f_DgE -> Write("",TObject::kOverwrite);
	
	c_rchi -> Write("",TObject::kOverwrite);
	graph_bp_redchi2 -> Write("",TObject::kOverwrite);
	graph_tp_redchi2 -> Write("",TObject::kOverwrite);
	graph_bm_redchi2 -> Write("",TObject::kOverwrite);
	graph_tm_redchi2 -> Write("",TObject::kOverwrite);
	
	c_fcn -> Write("",TObject::kOverwrite);
	graph_bp_fcn -> Write("",TObject::kOverwrite);
	graph_tp_fcn -> Write("",TObject::kOverwrite);
	graph_bm_fcn -> Write("",TObject::kOverwrite);
	graph_tm_fcn -> Write("",TObject::kOverwrite);
	
	c_p -> Write("",TObject::kOverwrite);
	graph_bp_pval -> Write("",TObject::kOverwrite);
	graph_tp_pval -> Write("",TObject::kOverwrite);
	graph_bm_pval -> Write("",TObject::kOverwrite);
	graph_tm_pval -> Write("",TObject::kOverwrite);
	
	c_in -> Write("",TObject::kOverwrite);
	graph_tp_countin -> Write("",TObject::kOverwrite);
	graph_tm_countin -> Write("",TObject::kOverwrite);
	graph_bp_countin -> Write("",TObject::kOverwrite);
	graph_bm_countin -> Write("",TObject::kOverwrite);
	
	c_out -> Write("",TObject::kOverwrite);
	graph_tp_countout -> Write("",TObject::kOverwrite);
	graph_tm_countout -> Write("",TObject::kOverwrite);
	graph_bp_countout -> Write("",TObject::kOverwrite);
	graph_bm_countout -> Write("",TObject::kOverwrite);

	c_int -> Write("",TObject::kOverwrite);
	graph_tp_countint -> Write("",TObject::kOverwrite);
	graph_tm_countint -> Write("",TObject::kOverwrite);
	graph_bp_countint -> Write("",TObject::kOverwrite);
	graph_bm_countint -> Write("",TObject::kOverwrite);

	c_rn-> Write("",TObject::kOverwrite);
	graph_bp_relnorm -> Write("",TObject::kOverwrite);
	graph_tp_relnorm -> Write("",TObject::kOverwrite);
	graph_bm_relnorm -> Write("",TObject::kOverwrite);
	graph_tm_relnorm -> Write("",TObject::kOverwrite);
	f_relnorm_tp -> Write("",TObject::kOverwrite);
	f_relnorm_tm -> Write("",TObject::kOverwrite);
	f_relnorm_bp -> Write("",TObject::kOverwrite);
	f_relnorm_bm -> Write("",TObject::kOverwrite);
	
	cout << "Fit param canvases have been saved to the file." << endl;
}


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#endif
