/**
 * \brief Export Single Histogram into ASCII file
*/

//#include <sstream>
#include <fstream>
using std::ofstream;

Bool_t SingleExportAscii(TH1* hist, TString &filename, TString folder="", TString separator="\t") 
{
	Int_t i,j;
	Double_t xcenter, xwidth;
	Bool_t success=kFALSE;
//	filename = folder + hist->GetName() + ".dat";
	if(filename=="")
	{
		filename = folder + hist->GetName() + ".dat";
	}
	ofstream file_out(filename);
	file_out << "# Output " << hist->ClassName() << ": " << hist->GetName() << " (" << hist->GetTitle() << ")\n";
	if (hist->GetDimension()==1)
	{
		file_out << "# BinCenter" << separator << "Content" << separator << "BinHalfWidth" << separator << "Error\n";
		for (i=1; i<=hist->GetNbinsX(); i++)
			file_out << hist->GetBinCenter(i) << separator << hist->GetBinContent(i) << separator << hist->GetBinWidth(i)/2 << separator /* << hist->GetBinError(i) */ << endl;
		if (i>1)
			success=kTRUE;
	}
	else if (hist->GetDimension()==2)
	{
		file_out << "# xBinCenter" << separator << "yBinCenter" << separator << "Content" << separator << "xBinHalfWidth" << separator << "yBinHalfWidth" << separator /* << "Error"*/ << endl;
		for (i=1; i <= hist->GetNbinsX(); i++)
		{
			xcenter = hist->GetXaxis()->GetBinCenter(i);
			xwidth = hist->GetXaxis()->GetBinWidth(i)/2;
			for (j=1; j <= hist->GetNbinsY(); j++)
				file_out << xcenter << separator << hist->GetYaxis()->GetBinCenter(j) << separator << hist->GetBinContent(i,j) << separator << xwidth << separator << hist->GetYaxis()->GetBinWidth(j)/2 << separator /* << hist->GetBinError(i,j) */ << endl;
			if (j>1)
				file_out << endl; // produce a blank line after each set of Ybins for a certain Xbin. Gnuplot likes this.
		}
		if (i>1)
			success=kTRUE;
	}
	file_out.close();
	if (success == kTRUE)
		cout << "*** SingleExportAscii: Histogram " << hist->GetName() << " written to " << filename << endl;
	return success;
}

Bool_t AsciiExport(string in_filenamestub, string out_filenamestub="", string histname="Runsets BCD Chi^2 Map")
{
	TFile * f = new TFile( (in_filenamestub+".root").c_str());
//	TH2D * h = (TH2D*)f->Get("Runsets BCD Chi^2 Map");
	TH2D * h = (TH2D*)f->Get(histname.c_str());
	if(out_filenamestub=="") { out_filenamestub=in_filenamestub; }
	
	TString s = TString( (out_filenamestub+".dat").c_str() );
	
	return SingleExportAscii(h, s);
}
