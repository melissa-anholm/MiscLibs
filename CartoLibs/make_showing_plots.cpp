// make_showing_plots.cpp

#include<iostream>

#include "ColorExtras.h"

class energy_specifier;
extern vector<energy_specifier> the_energyset;


void input_plot()  // from the rootscript input_plot.cpp.
{
	double energy;
	MapSetup * my_new_map;
	
	TCanvas * c = new TCanvas("c", "c", 100, 0, 900, 700);
	c->Divide(2,2);
	
	for(int i=the_energyset.size()-1; i>=0; i--)  // count down instead.  (am I off by one?)
	{
		energy = the_energyset.at(i).the_int;
		
		my_new_map = new MapSetup();
		my_new_map -> LoadFromFile( make_mapname_from_monoenergy(the_energyset.at(i).the_double) );
		
		if(i== the_energyset.size()-1)
		{
			c->cd(1);
			my_new_map -> naive_EnergyT_p_hist->Draw("hist");
		
			c->cd(2);
			my_new_map -> naive_EnergyT_m_hist->Draw("hist");
			
			c->cd(3);
			my_new_map -> naive_EnergyB_p_hist->Draw("hist");
			
			c->cd(4);
			my_new_map -> naive_EnergyB_m_hist->Draw("hist");
		}
		
		c->cd(1);
		my_new_map -> naive_EnergyT_p_hist->Draw("same");
		gPad->Update();
		
		c->cd(2);
		my_new_map -> naive_EnergyT_m_hist->Draw("same");
		gPad->Update();
		
		c->cd(3);
		my_new_map -> naive_EnergyB_p_hist->Draw("same");
		gPad->Update();
		
		c->cd(4);
		my_new_map -> naive_EnergyB_m_hist->Draw("same");
		gPad->Update();
		
		
		cout << "Loaded for i = " << i << endl;
	}
}


TCanvas * plot_lineshapeint_compare_p(TH1D * h_tp, TH1D* h_bp)
{
	TH1D* h_sum = (TH1D*)h_tp->Clone();
	h_sum -> Add(h_bp);
	h_sum->Sumw2(false);
	h_sum->SetLineColor(kCyan);
	h_sum->SetMarkerColor(kCyan);
	h_sum->SetFillColor(kCyan);
	h_sum->SetTitle("Lineshape Integrals, Pol+");
	
	h_bp=(TH1D*)h_bp->Clone();
	h_bp->Sumw2(false);
	h_bp->SetLineColor(int(mOrange));
	h_bp->SetMarkerColor(int(mOrange));
	h_bp->SetFillColor(int(mOrange));
	
	// print out the bins:
	cout << std::fixed;
	cout << std::setprecision(0);
	
	for(int i=1; i<h_sum->GetNbinsX(); i++)
	{
		if(h_sum->GetBinContent(i) > 0 )
		{
			cout << std::fixed;
			cout << std::setprecision(0);
			cout << "Energy (+) = " << h_sum->GetBinCenter(i) << endl;
			cout << "N_tp = " << h_tp->GetBinContent(i) << ";\tN_bp = " << h_bp->GetBinContent(i) << endl;
			cout << "N_total = " << h_sum->GetBinContent(i) << endl;
			cout << std::setprecision(4);
			cout << "Percent B = " << 100.0*((double)h_bp->GetBinContent(i))/((double)h_sum->GetBinContent(i)) << endl;
			cout << endl;
		}
	}
	
	
	TCanvas * cp_lineint = new TCanvas("cp_lineint", "cp_lineint", 100, 0, 900, 700);
	h_sum->GetXaxis()->SetRangeUser(0, 5200);
	h_sum -> Draw("b");
	h_bp -> Draw("b same");
	gPad->Update();
	
	return cp_lineint;
}
TCanvas * plot_lineshapeint_compare_m(TH1D * h_tm, TH1D* h_bm)
{
	TH1D* h_sum = (TH1D*)h_tm->Clone();
	h_sum -> Add(h_bm);
	h_sum->Sumw2(false);
	h_sum->SetLineColor(kCyan);
	h_sum->SetMarkerColor(kCyan);
	h_sum->SetFillColor(kCyan);
	h_sum->SetTitle("Lineshape Integrals, Pol-");
	
	h_bm = (TH1D*)h_bm->Clone();
	h_bm->Sumw2(false);
	h_bm->SetLineColor(int(mOrange));
	h_bm->SetMarkerColor(int(mOrange));
	h_bm->SetFillColor(int(mOrange));

	// print out the bins:
	cout << std::fixed;
	cout << std::setprecision(0);
	
	for(int i=1; i<h_sum->GetNbinsX(); i++)
	{
		if(h_sum->GetBinContent(i) > 0 )
		{
			cout << std::fixed;
			cout << std::setprecision(0);
			cout << "Energy (-) = " << h_sum->GetBinCenter(i) << endl;
			cout << "N_tm = " << h_tm->GetBinContent(i) << ";\tN_bm = " << h_bm->GetBinContent(i) << endl;
			cout << "N_total = " << h_sum->GetBinContent(i) << endl;
			cout << std::setprecision(4);
			cout << "Percent T = " << 100.0*((double)h_tm->GetBinContent(i))/((double)h_sum->GetBinContent(i)) << endl;
			cout << endl;
		}
	}
	
	TCanvas * cm_lineint = new TCanvas("cm_lineint", "cm_lineint", 100, 0, 900, 700);
	h_sum->GetXaxis()->SetRangeUser(0, 5200);
	h_sum -> Draw("b");
	h_bm -> Draw("b same");
	gPad->Update();
	
	return cm_lineint;
}


TH1D * make_total_inputcount_hist_p(TH1D * h_tp, TH1D* h_bp)
{
	TH1D * h_tot = (TH1D*)h_tp->Clone();
	h_tot->Add(h_bp);
	h_tot->Sumw2(kFALSE);
	h_tot->Sumw2(kTRUE);
	
	h_tot -> SetTitle("Naive Integrated Counts (+)");
	h_tot -> SetName("Naive Integrated Counts (+)");
	return h_tot;
}
TH1D * make_total_inputcount_hist_m(TH1D * h_tm, TH1D* h_bm)
{
	TH1D * h_tot = (TH1D*)h_tm->Clone();
	h_tot->Add(h_bm);
	h_tot->Sumw2(kFALSE);
	h_tot->Sumw2(kTRUE);
	
	h_tot -> SetTitle("Naive Integrated Counts (-)");
	h_tot -> SetName("Naive Integrated Counts (-)");
	return h_tot;
}

TH1D * make_total_lineshapecount_hist_p(TH1D * h_tp, TH1D* h_bp)
{
	TH1D * h_tot = (TH1D*)h_tp->Clone();
	h_tot->Add(h_bp);
	h_tot->Sumw2(kFALSE);
	h_tot->Sumw2(kTRUE);
	
	h_tot -> SetTitle("Post-Lineshape Integrated Counts (+)");
	h_tot -> SetName("Post-Lineshape Integrated Counts (+)");
	return h_tot;
}
TH1D * make_total_lineshapecount_hist_m(TH1D * h_tm, TH1D* h_bm)
{
	TH1D * h_tot = (TH1D*)h_tm->Clone();
	h_tot->Add(h_bm);
	h_tot->Sumw2(kFALSE);
	h_tot->Sumw2(kTRUE);
	
	h_tot -> SetTitle("Post-Lineshape Integrated Counts (-)");
	h_tot -> SetName("Post-Lineshape Integrated Counts (-)");
	return h_tot;
}

TH1D * make_fractionout_total_p(TH1D* hcount_naive_p, TH1D* hcount_lineshape_p)
{
	TH1D * h_frac = (TH1D*)hcount_lineshape_p->Clone();
	h_frac -> Divide(hcount_naive_p);
	h_frac -> SetName("Fraction of Events Recovered by Lineshape in Both Detectors (+) -- [(N_out_tp+N_out_bp)/(N_in_tp+N_in_bp)]");
	h_frac -> SetTitle("Fraction of Events Recovered by Lineshape in Both Detectors (+) -- [(N_out_tp+N_out_bp)/(N_in_tp+N_in_bp)]");
	
	return h_frac;
}
TH1D * make_fractionout_total_m(TH1D* hcount_naive_m, TH1D* hcount_lineshape_m)
{
	TH1D * h_frac = (TH1D*)hcount_lineshape_m->Clone();
	h_frac -> Divide(hcount_naive_m);
	h_frac -> SetName("Fraction of Events Recovered by Lineshape in Both Detectors (-) -- [(N_out_tm+N_out_bm)/(N_in_tm+N_in_bm)]");
	h_frac -> SetTitle("Fraction of Events Recovered by Lineshape in Both Detectors (-) -- [(N_out_tm+N_out_bm)/(N_in_tm+N_in_bm)]");
	
	return h_frac;
}





















