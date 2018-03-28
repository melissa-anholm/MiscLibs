#!/usr/bin/python
#tiny change
from ROOT import TCanvas, TF1, TTree, TH1D, TH1F, gDirectory, gPad, TLatex
from ROOT import  TFile, Double, TArrayD, TRandom3, gDirectory, kBlue
from ROOT import std, TArrayI, TGraphErrors, TGraph, gStyle, TLine, kRed, TPad, TArrow
from array import array
from math import sqrt
from uncertainties import ufloat
from physics_calcs import *

import ROOT
import os.path
import os
import numpy as np
import sys


def MakeNoiseFileForStrip(uORl, xORy) :
    fname = "bb1_noise_%s%s.root" % (uORl, xORy)
    outf = TFile(fname, "RECREATE")
    theh = []
    for i in range(40) :
        h = GetNoiseSpectrumForStrip(uORl, xORy, i)
        if (h) :
            n = "%s%s_S%d_noise" % (uORl, xORy, i)
            h.SetNameTitle(n, n)
            h.SetDirectory(outf)
            theh.append(h)
    outf.Write()
    outf.Close()

def GetNoiseSpectrumForStrip(uORl, xORy, stripn) :
    calname = "bb1_calibration_%s%s.dat" % (uORl, xORy)

    n, s, p, c, dc, r, dr, t0, t1, t2, t3, t4 = np.loadtxt(calname, unpack=True)
    index = -1;
    for i in range(len(s)) :
        if (s[i] == stripn) :
            index = i
            break
    if (index == -1) :
        print "Strip ", stripn, " not found"
        return 0

    ntuplen = n[index]
    calibration = c[index]      # includes correction (ch/keV)


    print "Strip#: %d\tNtuple#: %d\tCalibration: %f" % (stripn, ntuplen,
                                                        calibration)

    dir = "~/Documents/Analysis/Calibrations/StripDetectors/FitResults"
    dir = "%s/SetallResults" % dir
    rfname = "%s/result_%s%s_%d.root" % (dir, uORl, xORy, ntuplen)
    print rfname
    f = TFile(rfname)
    d = f.Get("exp_data")
    sim_noise = f.Get("sim_noise")
    gaussian = f.Get("gaussian")
    
    sum = sim_noise.Clone()
    sum.Add(gaussian)
    sum_cal = ShiftXaxis(sum, 0., calibration)
    sum_cal.SetDirectory(0)
    return sum_cal

def PlotSimpleScin(tree, uORl, threshold) :
    if (InputIsBad(uORl, "X")) :
        return
    
    cxeve = "BB1_AMPLITUDE_%sX[0] > %f" % (uORl, threshold)
    cxodd = "BB1_AMPLITUDE_%sX[1] > %f" % (uORl, threshold)
    
    cyeve = "BB1_AMPLITUDE_%sY[0] > %f" % (uORl, threshold)
    cyodd = "BB1_AMPLITUDE_%sY[1] > %f" % (uORl, threshold)

    for i in range(2, 40, 2) :
        cxeve = "%s || BB1_AMPLITUDE_%sX[%d] > %f" % (cxeve, uORl, i, threshold)
        cyeve = "%s || BB1_AMPLITUDE_%sY[%d] > %f" % (cyeve, uORl, i, threshold)
    for i in range(3, 40, 2) :
        cxodd = "%s || BB1_AMPLITUDE_%sX[%d] > %f" % (cxodd, uORl, i, threshold)
        cyodd = "%s || BB1_AMPLITUDE_%sY[%d] > %f" % (cyodd, uORl, i, threshold)

    sc = "Upper"
    if (uORl == "L") :
        sc = "Lower"
    d = "QDC_%sPMT" % sc
    dcommand = "%s >> hxeve(512, -0.5, 4095.5)" % d
    print "Beep"
    tree.Draw(dcommand, cxeve)
    dcommand = "%s >> hxodd(512, -0.5, 4095.5)" % d
    print "Beep"
    tree.Draw(dcommand, cxodd, "SAME")
    dcommand = "%s >> hyeve(512, -0.5, 4095.5)" % d
    print "Beep"
    tree.Draw(dcommand, cyeve, "SAME")
    dcommand = "%s >> hyodd(512, -0.5, 4095.5)" % d
    print "Beep"
    tree.Draw(dcommand, cyodd, "SAME")
    gPad.Update()

    hxeve = gPad.GetPrimitive("hxeve")
    hxodd = gPad.GetPrimitive("hxodd")
    hyeve = gPad.GetPrimitive("hyeve")
    hyodd = gPad.GetPrimitive("hyodd")

    hxeve.SetLineColor(600)
    hxeve.SetTitle("X Even")
    hxodd.SetLineColor(632)
    hxodd.SetTitle("X Odd")
    hyeve.SetLineColor(418)
    hyeve.SetTitle("Y Even")
    hyodd.SetLineColor(617)
    hyodd.SetTitle("Y Odd")

    hxeve.Draw()
    for h in [hxodd, hyeve, hyodd] :
        h.Draw("SAME")
    gPad.Update()
    raw_input("Enter to end")

def GetNeighbors(tree, uORl, xORy) :
    if (InputIsBad(uORl, xORy)) :
        return
    base = "BB1_AMPLITUDE_" + uORl + xORy
    
    smax = 40

    l = [[0 for i in range(smax)] for j in range(smax)]
    for s in range(smax) :
#        if (s == 0 or s == 10 or s == 8) :
#            continue
        mains = base + "[" + str(s) + "]"
        for t in range(s+1, smax, 2) :
#            if (t == 0 or t == 10 or t == 8) :
#                continue
            secondary = base + "[" + str(t) + "]"
            cmd = mains + ":" + secondary

            main_lim = 18
            seco_lim = 12
            if (s % 2 == 0) :
                main_lim = 12
                seco_lim = 18

            cond = mains + " > " + str(main_lim) + " && " + secondary + " > " + str(seco_lim)
            print cmd, " ", cond
            n = tree.Draw(cmd, cond)
#            print n
            l[s][t] = int(n)
            l[t][s] = int(n)
        ll = l[s]
        nlist = [(t, ll[t]) for t in range(smax)]
        nlist.sort(key=lambda x: x[1], reverse = True)
        print "Strip ", s, ": ",
        for i in range(4) :
            print nlist[i], ",   ",
        print "\n"

    
    for s in range(smax) :
        ll = l[s]
        nlist = [(t, ll[t]) for t in range(smax)]
        nlist.sort(key=lambda x: x[1], reverse = True)
        print "Strip ", s, ": ",
        for i in range(4) :
            print nlist[i], ",   ",
        print "\n"
        

def GetCalibrationFile(uORl, xORy) :
    if (InputIsBad(uORl, xORy)) :
        return
    outf = "bb1_calibration_" + uORl + xORy + ".dat"
    outf = raw_input("Enter output file : %s   " % outf) or outf
    detlong = ""
    if (uORl == "U") :
        detlong = "Upper"
    else :
        detlong = "Lower"
    # Read initial calibration from fitSummary_XX.root
    ifile = "fitSummary_%s%s.dat" % (uORl, xORy) # ntuple number
    nn, chi2, ndf, slp, dslp, res, dres, j1, j2, j3, j4, j5, j6, j7, j8= np.loadtxt(ifile, unpack=True)

    # Increase dslp by sqrt(chi2/ndf)
    for i in range(len(nn)) :
        c = sqrt(chi2[i]/ndf[i])
        dslp[i] = dslp[i]*c

    # Apply corrections
    ifile = "Results/corrections_%s%s.dat" % (uORl, xORy)
    nn_temp, cor, dcor = np.loadtxt(ifile, unpack=True) # ntuple number
    if (not len(nn) == len(nn_temp)) :
        print "Calibration file and corrections file have different lengths!"
        return


    for i in range(len(nn)) :
        if (not nn[i] == nn_temp[i]) :
            print "Calibration and correction files not matched!"
            return
        dslp[i] = (slp[i]/cor[i]) * sqrt((dslp[i]/slp[i])**2+(dcor[i]/cor[i])**2)
        slp[i] = slp[i] / cor[i]

    
    # Get resolution from corrected (YX) data
    rfile = TFile("linearity_all.root") # strip number
#    rfile = TFile("Results/linearity_all_%s%s_correctedYX.root" % (uORl, xORy))

    rfunc = TF1("gaussian", "gaus(0)", -40, 40)
    res_final = [0]*len(nn)
    dres_final = [0]*len(nn)
    sn = [0]*len(nn)
    pos = [0]*len(slp)

    for i in range(len(nn)) :
        s = nn[i]
        if (uORl == "U") :
            if (nn[i] % 2 == 0) :
                s = nn[i]
            else :
                s = 40 - nn[i]
        elif (uORl == "L" and xORy == "Y") :
            if (nn[i] % 2 == 0) :
                s = (nn[i] + 2) % 40
            else :
                s = nn[i]
        sn[i] = s
        if (uORl == "U" and xORy == "Y") :
            pos[i] = 19.5 - sn[i]
        else :
            pos[i] = -19.5 + sn[i]

        

        zzz = "%s_BB1/%s_Plane/Ediff_1s/%s_BB1%s_Plane_Strip%d_Ediff_1s" % (detlong, xORy, detlong, xORy, s)
        h2d = rfile.Get(zzz)
        col = GetProjectionMeanSig(h2d,"Resolution")
        res_final[i] = col[3].nominal_value
        dres_final[i] = col[3].std_dev

    # Get threshold for min S/N = 1/2, 3/4, 1, 2 (S/N calculated per bin)
    # signoise = [0.25, 0.5, 0.75, 1.0, 2.0]
    signoise = [0.17, 0.25, 0.5, 0.75, 1.0]
    #signoise = [0.1, 0.17, 0.25, 0.5, 0.75]
    snlist = [[0]*len(signoise) for i in range(len(nn))]
    for i in range(len(nn)) :
        for s in range(len(signoise)) :
            fname = "~/Documents/Analysis/Calibrations/StripDetectors/FitResults/SetallResults/result_%s%s_%d.root" % (uORl, xORy, nn[i])
            snlist[i][s] = RecommendThreshold(fname, signoise[s])

    f = open(outf, 'w')
    for i in range(len(nn)) :
        f.write("%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n" %
                (nn[i],
                 sn[i],
                 pos[i],
                 slp[i],
                 dslp[i],
                 res_final[i],
                 dres_final[i],
                 snlist[i][0],
                 snlist[i][1],
                 snlist[i][2],
                 snlist[i][3],
                 snlist[i][4]))
    f.close()
    
def ShowAllFiringWithG4() :
    c = TCanvas()
    c.Divide(2, 2)
    p = 1
    for det in ["U", "L"] :
        for pl in ["X", "Y"] :
            c.cd(p)
            s = "Linearity/geant_%s%s.root" % (det, pl)
            print s
            gfile = TFile(s)
            s = "%s%s_Firing" % (det, pl)
            print s
            ghist = gfile.Get(s)

            s = "Linearity/linearity_all_%s%s_correctedY.root" % (det, pl)
            dfile = TFile(s)
            s = "%s%s_Firing" % (det, pl)
            dhist = dfile.Get(s)

            for b in range(1, dhist.GetNbinsX()+1) :
                if (dhist.GetBinContent(b) < 1) :
                    ghist.SetBinContent(b, 0)
            
            ghist.Scale(dhist.Integral()/ghist.Integral())
            ghist.SetLineColor(ROOT.kRed)
            dhist.DrawCopy("HIST")
            ghist.DrawCopy("SAME")
            
            p = p + 1
    
    gPad.Update()
    return c
def OverlayCollimatorOnMap(olap = 4.5) :
    l = [0]*4
    lo = -0.5+olap
    up = 39.5-olap
    l[0] = TLine(lo, lo, lo, up)
    l[1] = TLine(lo, up, up, up)
    l[2] = TLine(up, up, up, lo)
    l[3] = TLine(up, lo, lo, lo)
    gPad.Update()
    for i in range(4) :
        l[i].SetLineWidth(6)
        l[i].Draw()
    gPad.Update()
    return l

def MakeCorrections(fname, uORl, xORy) :
    if (InputIsBad(uORl, xORy)) :
        return

    outfname = raw_input("Enter output file\n")
    outf = open(outfname, 'w')
    f = TFile(fname)
    for i in range(40) :
        n = "%s%s%d-Scatter" % (uORl, xORy, i)
        if (f.GetListOfKeys().Contains(n)) :
            h = f.Get(n)
            if (h.GetEntries() < 1) :
                continue
            c = CorrectCalibration(h)
            outf.write("%d\t%g\t%g\n" % (i, c[0], c[1]))
    outf.close()

def CorrectCalibration(hist, merge = 5, htype = "-Scatter") :
    #FitSlicesY()
    if ((not htype == "-Scatter") and (not htype == "-Ediff")) :
        print htype, " not found"
        return 0
    
    g = TH1D("centroid", "centroid", hist.GetNbinsX(),
             hist.GetXaxis().GetBinLowEdge(1),
             hist.GetXaxis().GetBinLowEdge(hist.GetNbinsX()) +
             hist.GetXaxis().GetBinWidth(hist.GetNbinsX()))
    maxx = 0
    hist.GetXaxis().UnZoom()
    hist.GetYaxis().UnZoom()
    for i in range(hist.GetXaxis().FindBin(80), hist.GetNbinsX()+1, merge) :
        # This is DEFINITELY RIGHT PROJECTION-Y PROJECTION-Y (11/12/15)
        #print "i = %d" % i
        #print hist.GetName()
        t = hist.ProjectionY("_temp", i, i+merge-1, "ed")
        # print "Projecting from ", hist.GetXaxis().GetBinCenter(i), " to ",
        # print hist.GetXaxis().GetBinCenter(i+merge-1)

        if (t.GetEntries() < 30) :
            continue
        f = 0
        if (htype == "-Ediff") :
            f = TF1("g", "gaus(0)", -30., 30.)
            f.SetParameters(t.GetBinContent(t.GetMaximumBin()), 0.0, 10.0)
        else :
            f = TF1("g", "gaus(0)", g.GetXaxis().GetBinCenter(i) - 50,
                    g.GetXaxis().GetBinCenter(i+merge-1) + 50)
            f.SetParameters(t.GetBinContent(t.GetMaximumBin()),
                            g.GetBinCenter(i), 10)

        maxx = g.GetXaxis().GetBinCenter(i+merge-1)

        t.Fit("g", "LQR")
        bb = ((2*i)+merge-1)/2;
#        print "Filling bin centered at ", g.GetBinCenter(bb)
        g.SetBinContent(bb, f.GetParameter(1))
        g.SetBinError(bb, f.GetParError(1))
        gPad.Update()
#        raw_input()
        

    cor = TF1("cor", "pol1(0)", 100, maxx)
    cor.SetParameters(0.0, 1.0)
    if (htype == "-Scatter") :
        cor.FixParameter(0, 0.0)
        g.Draw("AP")
        g.Fit(cor, "WR")
    else :
#        cor.FixParameter(1, 0.0)
        g.Fit(cor, "R")
    g.SetLineColor(1)
    g.SetLineWidth(3)
    g.SetMarkerStyle(21)
    g.SetMarkerSize(0.75)
    g.SetMarkerColor(kRed)
    gPad.Update()
    return [cor.GetParameter(1),
            cor.GetParError(1)*sqrt(cor.GetChisquare()/cor.GetNDF()), g]

def RecommendThreshold(fname, min_sn) :
    f =TFile(fname)
    if (f.IsZombie()) :
        return 10000.0
    result = f.Get("fit_result")
    data = f.Get("exp_data")
    mc = f.Get("partial_mc")
    gaus = f.Get("gaussian")
    noise = f.Get("sim_noise")

    # data.Draw("HIST")
    # mc.Draw("SAME")
    # gaus.Draw("SAME")
    # noise.Draw("SAME")
    # gPad.Update()
    # raw_input()
    
    m = Double(0)
    e = Double(0)
    result.GetParameter(0, m, e)
    
    for b in range(0, 300) :

        bkg = gaus.GetBinContent(b) + noise.GetBinContent(b)
        sig = mc.GetBinContent(b)
        # print "bin with center = %f has bkg = %f, sig = %f" % (gaus.GetBinCenter(b), bkg, sig)

        if (sig > min_sn*bkg and b > 2) :
                break
    t_adc = mc.GetBinCenter(b)
    t_E = t_adc/m
    return t_E


def CheckFixedParams(m) :
    val = Double(0)
    err = Double(0)
    
    eps = 1E-6

    for i in [0, 2, 6, 9, 10, 11] :
        m.GetParameter(i, val, err)
        if (abs(err) < eps) :
            return False

    m.GetParameter(1, val, err)
    if (abs(val) > eps  or abs(err) > eps) :
        return False
    
    m.GetParameter(3, val, err)
    if (abs(val) > eps  or abs(err) > eps) :
        return False

    m.GetParameter(4, val, err)
    if (abs(val-12.9) > eps  or abs(err) > eps) :
        return False

    m.GetParameter(5, val, err)
    if (abs(val-4.13) > eps  or abs(err) > eps) :
        return False

    m.GetParameter(7, val, err)
    if (abs(val) > eps  or abs(err) > eps) :
        return False

    m.GetParameter(8, val, err)
    if (abs(val) > eps  or abs(err) > eps) :
        return False
    return True

def GenerateWaveform(nsamples, mean, sigma) :
    y = array('d',[0]*nsamples)
    r = TRandom3()
    r.SetSeed(0)
#    print r.GetSeed()
    for i in range(len(y)) :
        y[i] = r.Gaus()
#        print y[i]
    return y

def CalcNoiseStats(hist) :
    maxy = 0
    maxx = hist.GetMaximumBin()

    maxy = hist.GetBinContent(maxx)
    while (maxy < 500 and hist.GetNbinsX() % 2 == 0) :
        hist.Rebin(2)
        maxx = hist.GetMaximumBin()
        maxy = hist.GetBinContent(maxx)
        

    hwhm = hist.FindLastBinAbove(maxy/2.)
    print "HW bin = ", hwhm
    return [hist.GetBinCenter(maxx),
            hist.GetBinCenter(hwhm) - hist.GetBinCenter(maxx)]
    

def SimulateWFnoise(ntries, blength = 5, nsamples = 60, tmin = 25, tmax = 40,
                    mean = 0., sigma = 1.) :
    h = TH1F("Noise", "Noise", 50000, 0, 10)
    h.SetDirectory(0)
    for w in range(ntries) :
        if (w % 100000 == 0) :
            print "WF ", w
        y = GenerateWaveform(nsamples, mean, sigma)
        bl = 0
        for i in range(blength) :
            bl = bl + y[i]
            bl = bl/blength
#            print "baseline = ", bl
        
        ymax = -1
        xval = -1
        for i in range(nsamples) :
            if (y[i] > ymax) :
                ymax = y[i]
                xval = i
        if (xval > tmin and xval < tmax) :
            h.Fill(ymax - bl)

    h.Draw("HIST")
    raw_input("Enter to end")
    return h

def CompareFits(fname1, fname2) :
    sn_1, chi2_1, ndf_1, slp_1, dslp_1, res_1, dres_1, j1, j2, j3, j4, j5, j6, j7, j8= np.loadtxt(fname1, unpack=True)

    sn_2, chi2_2, ndf_2, slp_2, dslp_2, res_2, dres_2, j1, j2, j3, j4, j5, j6, j7, j8= np.loadtxt(fname2, unpack=True)
    
    x = array('d', [0]*len(sn_1))
    slp_r = array('d', [0]*len(sn_1))
    res_r = array('d', [0]*len(sn_1))

    c = TCanvas()
    c.Divide(1, 2)

    ymax = 2.

    z = TF1("line", "0", -1, 50)
    slp_h = TH1D("Dslope-hist", "Dslope-hist", 17, -8.5, 8.5)
    res_h = TH1D("Dres-hist", "Dres-hist", 17, -8.5, 8.5)
    for i in range(len(sn_1)) :
        if (not sn_1[i] == sn_2[i]) :
            print "MISMATCH!", sn_1[i], "\t", sn_2[i]
            continue
        x[i] = sn_1[i]
        err = sqrt(dslp_1[i]**2+dslp_2[i]**2)
        y = (slp_1[i] - slp_2[i]) / err
        slp_r[i] = y
        slp_h.Fill(y)
        if (abs(y) > ymax) :
            print "Slope change: ", sn_1[i], slp_1[i], "\t", slp_2[i]

        err = sqrt(dres_1[i]**2+dres_2[i]**2)
        y = (res_1[i] - res_2[i]) / err
        res_r[i] = y
        res_h.Fill(y)
        if (abs(y) > ymax) :
            print "Resolution change: ", sn_1[i], res_1[i], "\t", res_2[i]

    gslp = TGraph(len(x), x, slp_r)
    gres = TGraph(len(x), x, res_r)
    c.cd(1)
    gslp.Draw("A*")
    gslp.GetYaxis().SetTitle("(Cal[0] - Cal[1])/#sigma_{tot}")
    gslp.GetYaxis().SetTitleSize(0.09)
    gslp.GetYaxis().SetTitleOffset(0.5)
    gslp.GetYaxis().SetLabelSize(0.1)
    gslp.GetYaxis().SetRangeUser(-ymax, ymax)
    z.Draw("SAME")
    c.cd(2)
    gres.Draw("A*")
    gres.GetYaxis().SetTitleSize(0.09)
    gres.GetYaxis().SetTitleOffset(0.5)
    gres.GetYaxis().SetLabelSize(0.1)
    gres.GetYaxis().SetTitle("(Res[0] - Res[1])/#sigma_{tot}")
    gres.GetYaxis().SetRangeUser(-ymax, ymax)
    z.Draw("SAME")
    gPad.Update()
    raw_input("Enter to end")
    c.cd(1)
    slp_h.GetXaxis().SetTitle("(Cal[0] - Cal[1])/#sigma_{tot}")
    slp_h.GetYaxis().SetTitle("Number of strips")
    slp_h.GetYaxis().SetTitleSize(0.09)
    slp_h.GetYaxis().SetTitleOffset(0.5)
    slp_h.GetYaxis().SetLabelSize(0.1)

    slp_h.Draw("HSIT")
    c.cd(2)
    res_h.GetXaxis().SetTitle("(Cal[0] - Cal[1])/#sigma_{tot}")
    res_h.GetYaxis().SetTitle("Number of strips")
    res_h.GetYaxis().SetTitleSize(0.09)
    res_h.GetYaxis().SetTitleOffset(0.5)
    res_h.GetYaxis().SetLabelSize(0.1)

    res_h.Draw("HIST")
    
    gPad.Update()
    raw_input("Enter to end")
    
def PlotHistForAllStrips(fname, extension, uORl, xORy, opt = "", subdir = "") :

    xORy = xORy.upper()
    uORl = uORl.upper()
    if (InputIsBad(uORl, xORy)) :
        return
    c1 = TCanvas()
    c1.Divide(5,4)
    c2 = TCanvas()
    c2.Divide(5,4)
    f = TFile(fname)
    l = TF1("line", "x", 0, 1000)
    sn = array('d', [])
    sn2 = array('d', [])
    centroid = array('d', [])
    centroid2 = array('d', [])
    dcentroid = array('d', [])
    dcentroid2 = array('d', [])
    outf = 0
    res = [0]*40
    dres = [0]*40
    for i in range(20) :
        name = "%s%s%s%d%s" % (subdir, uORl, xORy, i, extension)
        h1 = f.Get(name)
        if (h1) :
            h1.SetDirectory(0)
            c1.cd(i+1)
            h1.Draw(opt)
            if(extension == "-Scatter") :
                l.DrawCopy("SAME")
            if(extension[:11] == "-Resolution" and h1.GetEntries() > 0) :
                func = TF1("gausfit", "gaus(0)", -30, 30)
                func.SetParameters(h1.GetBinContent(h1.FindBin(0.0)), 0.0, 10.0)
                h1.Fit("gausfit", "RQ")
                sn.append(i)
                centroid.append(func.GetParameter(1))
                dcentroid.append(func.GetParError(1))
                res[i] = func.GetParameter(2)
                dres[i] = func.GetParError(2)
            gPad.Update()
        name = "%s%s%s%d%s" % (subdir, uORl, xORy, i+20, extension)
        h2 = f.Get(name)
        if (h2) :
            h2.SetDirectory(0)
            c2.cd(i+1)
            h2.Draw(opt)
            if(extension == "-Scatter") :
                l.DrawCopy("SAME")
            if(extension[:11] == "-Resolution" and h2.GetEntries() > 0) :
                func = TF1("gausfit", "gaus(0)",-30, 30)
                func.SetParameters(h1.GetBinContent(h1.FindBin(0.0)), 0.0, 10.0)
                h2.Fit("gausfit", "RQ")
                sn2.append(i+20)
                centroid2.append(func.GetParameter(1))
                dcentroid2.append(func.GetParError(1))
                res[i+20] = func.GetParameter(2)
                dres[i+20] = func.GetParError(2)
            gPad.Update()

    rval = [c1,c2]
    if (extension[:11] == "-Resolution") :
        sn.extend(sn2)
        centroid.extend(centroid2)
        dcentroid.extend(dcentroid2)
        c3 = TCanvas()
        c3.cd()
        dsn = array('d', [0]*len(sn))
        g = TGraphErrors(len(sn), sn, centroid, dsn, dcentroid)
        g.GetXaxis().SetTitle("Strip Number")
        g.GetYaxis().SetTitle("Resolution centroid")
        g.Draw("A*")
        fline = TF1("line", "pol0(0)", 0, 40)
        fline.SetParameter(0, 0.0)
        g.Fit("line", "")
        fline.Draw("SAME")
        gPad.Update()
        rval.append(g)

        fn = "Linearity/resolution_%s%s.dat" % (uORl, xORy)
        inp = raw_input("Resolution file: %s [y to accept]" % fn)
        if (not inp == "NONE") :
            if (not inp.lower() == "y") :
                fn = inp
            outf = open(fn, 'w')
            for i in range(len(res)) :
                outf.write("%d\t%f\t%f\n" % (i, res[i], dres[i]))
            outf.close()
        
    return rval

def MakeResultsFileForPlane(uORl, xORy, result_dir, save_dir = ".") :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (result_dir[-1:] == "/") :
        result_dir = result_dir[:-1]
    if (save_dir[-1:] == "/") :
        save_dir = save_dir[:-1]

    home = os.environ['HOME']
    if (InputIsBad(uORl, xORy)) :
        return

    resultdir = home + "/Documents/Analysis/Calibrations/StripDetectors/FitResults/" + result_dir
    print resultdir
    fname = save_dir + "/fitSummary_" + uORl + xORy + ".dat"
    print fname
    outf = open (fname, 'w')
    if (not outf) :
        print "could not open ", fname
#        return 
    for i in range(40) :
        fname = resultdir + "/result_" + uORl + xORy
        fname = fname + "_" + str(i) + ".root"
#        print fname
        if (os.path.isfile(fname)) :
            f = TFile(fname)
            gmin = f.Get("fit_result")

            chi2 = gmin.fAmin
#            status = gmin.fStatus

            ivec = TArrayI()
            f.GetObject("return_status", ivec)
            status = ivec.At(0)
            if (status == 4) :
                print "Strip ", i,  " shows abnormal termination"
            if (not CheckFixedParams(gmin)) :
                print "Strip ", i, " has wrong fixed parameters"
            par_and_err = [Double(0),Double(0),Double(0),Double(0),Double(0),Double(0),
                           Double(0),Double(0),Double(0),Double(0),Double(0),Double(0),
                           Double(0),Double(0),Double(0),Double(0),Double(0),Double(0),
                           Double(0),Double(0),Double(0),Double(0),Double(0),Double(0)]
#            par_and_err = [Double(0)]*(12*2)

            for j in range(12) :
                gmin.GetParameter(j, par_and_err[2*j], par_and_err[(2*j)+1])


            dat = f.Get("exp_data")
            binw = dat.GetBinWidth(2)
            
            vec = TArrayD()
            f.GetObject("limits", vec)
            
            nfree = gmin.GetNumFreePars()
            ndf = ((vec[1] - vec[0]) / binw) - nfree
            #        print ndf

            outf.write("%d\t%g\t%g\t" % (i, chi2, ndf))
            for k in [1, 3, 7, 10, 11, 12] :
#                print i
                j = k - 1
                outf.write("%g\t%g\t" % (par_and_err[2*j], par_and_err[2*j+1]))

            outf.write("\n")
            f.Close()
#        else :
#            print "NOT FILE" 
#        print slp, " +/- ", slp_err, chi2, res, res_err, vec.At(0), vec.At(1)


    outf.close()

def PlotFitResultsForPlane(uORl = 'U', xORy = 'X', directory = ".") :
    gStyle.SetOptTitle(0)
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy)) :
        return

    fname = directory + "/fitSummary_" + uORl + xORy + ".dat"
    if (not os.path.isfile(fname)) :
        print fname + " does not exist"
        return

    c = TCanvas()
    c.Divide(1, 3)

    tmp = TGraph(fname, "%lg%*lg%lg")
    chi = array('d', [0])
    ndf = array('d', [0])
    ndf = tmp.GetY()
    tmp2 = TGraph(fname, "%lg%lg")
    chi = tmp2.GetY()

    markerInt = 8
    
    for i in range(tmp2.GetN()) : chi[i] = chi[i] / ndf[i]
    gchi = TGraph(tmp.GetN(), tmp.GetX(), chi)
    gchi.SetMarkerStyle(markerInt)

    gchi.GetXaxis().SetTitle("Strip number")
    gchi.GetXaxis().SetTitleSize(0.15)
    gchi.GetXaxis().SetLabelSize(0.15)
    gchi.GetXaxis().SetRangeUser(0, 40)
    gchi.GetYaxis().SetTitle("#chi^{2}/#nu")
    gchi.GetYaxis().SetTitleSize(0.15)
    gchi.GetYaxis().SetLabelSize(0.1)
    gchi.GetYaxis().SetTitleOffset(0.3)
    gchi.GetYaxis().CenterTitle(1)
    
    gslp = TGraphErrors(fname, "%lg%*lg%*lg%lg%lg")
    gslp.SetMarkerStyle(markerInt-1)
    gslp.GetXaxis().SetRangeUser(0, 40)
    gslp.GetYaxis().SetTitle("Cal. [channel/keV]")
    gslp.GetYaxis().SetLabelSize(0.09)
    gslp.GetYaxis().SetTitleSize(0.11)
    gslp.GetYaxis().SetTitleOffset(0.4)
    
    gres = TGraphErrors(fname, "%lg%*lg%*lg%*lg%*lg%lg%lg")
    gres.GetXaxis().SetRangeUser(0, 40)
    gres.SetMarkerStyle(markerInt)
    gres.GetYaxis().SetTitle("Resolution [keV]")
    gres.GetYaxis().SetLabelSize(0.1)
    gres.GetYaxis().SetTitleSize(0.15)
    gres.GetYaxis().SetTitleOffset(0.3)

    c.cd(1)
    gPad.SetBottomMargin(1E-5)
    gslp.Draw("AP")
    l = TLatex(5, 0.43, "BB1 Plane " + uORl + xORy)
    l.SetTextSize(3*l.GetTextSize())
    l.SetTextColor(ROOT.kRed)
#    l.SetTextFont(4)
    l.Draw("SAME")
    c.cd(2)
    gPad.SetBottomMargin(1E-5)
    gPad.SetTopMargin(1E-5)
    gres.Draw("AP")
    c.cd(3)    
    gPad.SetTopMargin(1E-5)
    gPad.SetBottomMargin(3.0*gPad.GetBottomMargin())
    gchi.Draw("AP")
    c.Update()

    raw_input()
#    return [gslp, gres]
def InputIsBad(uORl = 'U', xORy = 'X', stripn = 20) :
    if (uORl != "U" and uORl != "L") :
        print "Must indicate detector U or L"
        return True

    if (xORy != "X" and xORy != "Y") :
        print "Must indicate strip X or Y"
        return True

    if (stripn < 0 or stripn >= 40) :
        print "Strip number ", stripn, " is invalid"
        return True

    return False

def PlotBaselineRMSForStrip(tree, uORl, xORy, stripn, cut = "") :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, stripn)) :
        return

    sname = "BB1_" + uORl + xORy + "_BASELINE_RMS[" + str(stripn) + "]"
    print sname
    
    tree.Draw(sname + ">>thish(50, 0, 5)", cut)
    h = gDirectory.Get("thish")
    gDirectory.Clear()
    hclone = h
    gPad.Update()
#    print hclone

    return hclone

def PlotRMSvsEForStrip(tree, uORl, xORy, stripn, cut = "") :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, stripn)) :
        return

    sname = "BB1_" + uORl + xORy + "_BASELINE_RMS[" + str(stripn) + "]:BB1_AMPLITUDE_" + uORl + xORy + "[" + str(stripn) + "]"
    print sname
    
    tree.Draw(sname + ">>thish(50, 0, 400, 10, 0, 2)", cut)
    h = gDirectory.Get("thish")
    gPad.Update()
    gDirectory.Clear()
    hclone = h
#    print hclone
    return hclone

def PlotEvsPeaktimeForStrip(tree, uORl, xORy, stripn, cut = "") :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, stripn)) :
        return

    sname = "BB1_AMPLITUDE_" + uORl + xORy + "[" + str(stripn) + "]:BB1_" + uORl + xORy + "_PEAKTIME[" + str(stripn) + "]"
    print sname
    
    tree.Draw(sname + ">>thish(60, 0, 60, 550, -50, 500)", cut)
    h = gDirectory.Get("thish")
    gPad.Update()
    gDirectory.Clear()
    hclone = h
#    print hclone
    return hclone

def PlotRMSvsTForStrip(tree, uORl, xORy, stripn, cut = "") :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, stripn)) :
        return

    sname = "BB1_" + uORl + xORy + "_BASELINE_RMS[" + str(stripn) + "]:BB1_" + uORl + xORy + "_PEAKTIME[" + str(stripn) + "]"
    print sname
    
    tree.Draw(sname + ">>thish(55, 5, 60, 10, 0, 2)", cut)
    gPad.Update()
    h = gDirectory.Get("thish")
    gDirectory.Clear()
    hclone = h
#    print hclone
    return hclone

def PlotRMSvsAVGForStrip(tree, uORl, xORy, stripn, cut = "") :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, stripn)) :
        return

    sname = "BB1_" + uORl + xORy + "_BASELINE_RMS[" + str(stripn) + "]:BB1_" + uORl + xORy + "_BASELINE_AVG[" + str(stripn) + "]"
    print sname
    
    tree.Draw(sname + ">>thish(25, 525, 600, 10, 0, 2)", cut)
    gPad.Update()
    h = gDirectory.Get("thish")
    gDirectory.Clear()
    hclone = h
#    print hclone
    return hclone
           
              
def CreateFileWithSpectra(tree, fname, uORl, xORy, tmin = 0, tmax =
                          60, qdc_thresh = 0) :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, 20)) :
        return

    lname = ""
    if (uORl == 'U') :
        lname = "Upper"
    else :
        lname = "Lower"

    if (os.path.isfile(fname)) :
        yn = raw_input("File exists, overwrite it?\n")
        yn.upper()
        if (yn == "N") :
            return
        else :
            print "Okay, overwriting"
    outf = TFile(fname, "RECREATE")
    
    for i in range(40) :
        cc = "BB1_" + uORl + xORy + "_PEAKTIME[" + str(i) + "] > " + str(tmin)
        cc = cc + " && BB1_" + uORl + xORy + "_PEAKTIME[" + str(i) + "] < " + str(tmax)
        if (qdc_thresh > 0) :
            cc = cc + " && QDC_" + lname + "PMT > " + str(qdc_thresh)
        print cc
        h = PlotSpectrumForStrip(tree, uORl, xORy, i, cc)
        h.SetName("BB1_AMPLITUDE_" + uORl + xORy + "[" + str(i) + "]")
        h.Write()
    outf.Close()


def PlotSpectrumForStrip(fname, uORl, xORy, stripn) :
    
#def PlotSpectrumForStrip(tree, uORl, xORy, stripn, cut = "") :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, stripn)) :
        return

    f = TFile(fname)
    sname = "BB1_AMPLITUDE_%s%s_%02d" % (uORl, xORy, stripn)
#    sname = "BB1_AMPLITUDE_" + uORl + xORy + "_" + str(stripn)
    print sname

    h = f.Get(sname)
    h.SetDirectory(0)
    h.Rebin(5)
    f.Close()
#    tree.Draw(sname + ">>thish(2500, -0.1, 499.9)", cut)
#    h = gDirectory.Get("thish")

#    gPad.Update()
#    gDirectory.Clear()

 #   print hclone
    return h

def GetPeakFitForStrip(tree, uORl, xORy, stripn, cut = "") :
    h = PlotPeakTimeForStrip(tree, uORl, xORy, stripn, cut)
    
    f = TF1("gauss-fit", "gaus(0)", 25, 45)
    f.SetParameters(h.GetBinContent(h.GetMaximumBin()), 35, 5)
    h.Fit(f, "RL")
    gPad.Update()

def PlotPeakTimeForStrip(tree, uORl, xORy, stripn, cut = "") :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, stripn)) :
        return

    sname = "BB1_" + uORl + xORy + "_PEAKTIME[" + str(stripn) + "]"
    print sname
    
    cut = cut + "BB1_AMPLITUDE_" + uORl + xORy + "[" + str(stripn) + "] > 10"

    tree.Draw(sname + ">>thish(60, 0, 60)", cut)
    h = gDirectory.Get("thish")
    gPad.Update()
    gDirectory.Clear()
    hclone = h
    print hclone
    return hclone
    
def GetGausLandauForFitting() :
    g = TF1("landgaus", "gaus(0)+landau(3)", 5, 200)
    g.SetParNames("Gauss Amp.", "Gauss Mean", "Gauss #sigma", "Landau Amp.",
                  "Landau MPV", "Landau #sigma")
    g.SetParameters(10000, 1.4, 5, 500, 35, 4.4)
    return g
    
def PlotAllForNameCycle(f, name, ncyclemax = -1) :
#    c = TCanvas()

    if (ncyclemax < 0) :
        ncyclemax = f.GetNkeys()               #assumes all names are the same
        
    harray = [0]*(ncyclemax)

    imax = ncyclemax
    ncycles = ncyclemax
    for i in range(1, ncyclemax+1) :
        s = name + ";" + str(i)
        harray[i-1] = f.Get(s)
        if (harray[i-1]) :
            harray[i-1].GetYaxis().SetRangeUser(400, 1030)
            harray[i-1].SetDirectory(0)
        else :
            imax = i - 1
            ncycles = i - 1
            break

    print name, "   ", imax
    if (not harray[0]) :
        return
    harray[0].Draw("HIST")
    for i in range(1, imax) :
        harray[i].Draw("SAME")

    l = TLatex(10, 650, str(ncycles) + " waveforms")
    l.Draw()
 #   c.Update()
#    raw_input("Enter to end")

def PlotScintE_bb1early(t) :
    h = [TH1D("Scint Upper", "Scint Upper", 4096, -0.5, 4095.5),
         TH1D("Scint Lower", "Scint Lower", 4096, -0.5, 4095.5)]

    maxtime = 45                          #somewhat arbitrary
    minpeak = 30                          #arbitrary

    amp = [std.vector('double')()]*4
    time = [std.vector('double')()]*4
    for i in range(t.GetEntries()) :
        if (i % 10000 == 0) :
            print i
            
        t.GetEntry(i)

        amp = [t.BB1_AMPLITUDE_UX, t.BB1_AMPLITUDE_UY,
               t.BB1_AMPLITUDE_LX, t.BB1_AMPLITUDE_LY]
        time = [t.BB1_UX_PEAKTIME, t.BB1_UY_PEAKTIME,
                t.BB1_LX_PEAKTIME, t.BB1_LY_PEAKTIME]

        good = [False, False]

        for i in range(4) :
            if (len(amp[i]) < 40 or len(time[i]) < 40) :
                continue
            for s in range(40) :
                if (amp[i][s] > minpeak and time[i][s] > maxtime) :
                    good[i/2] = True

        if (good[0]) :
            h[0].Fill(t.QDC_UpperPMT)
        if (good[1]) :
            h[1].Fill(t.QDC_LowerPMT)
        i = i + 1

    c = TCanvas()
    c.Divide(1, 2)
    c.cd(1)
    h[0].Draw("HIST")
    c.cd(2)
    h[1].Draw("HIST")
    raw_input("Enter to end")
    return c

def LinBkg(x, par) :
    x = x[0]
    m = par[0]
    N = par[1]

    if (m >= 0.0 or N <= 0.0) :
        print "Param error"
        return 0.0

    cutoff = sqrt(-2.0*N/m)

#    print "Cutoff = ", cutoff
#    print "x = ", x
    if (x <= cutoff) :
        return (m*x) + sqrt(-2.0*m*N)
    else :
 #       print "Forced zero"
        return 0.0

                       
def GetResWithThreshold(fname, uORl, xORy, threshold = 100) :
    xORy = xORy.upper()
    uORl = uORl.upper()

    if (InputIsBad(uORl, xORy, 20)) :
        return

    f = TFile(fname, "UPDATE")
    if (not f.IsOpen()) :
        print fname, " not open!"
        return 0
    
    dname = "Resolution_%d" % threshold
    if (f.GetListOfKeys().Contains(dname)) :
        print "Removing existing ", dname
        f.rmdir(dname)
        
    f.mkdir(dname)
    f.cd(dname)


    for i in range(40) :
        hname = "%s%s%d%s" % (uORl, xORy, i, "-Ediff")
        h = f.Get(hname)
        if (not h) :
            continue
        if (h.GetEntries() < 100) :
            continue

        bmin = h.GetXaxis().FindBin(threshold)
        bmax = h.GetNbinsX()
        pname = "%s%s%d-Resolution_%d" % (uORl, xORy, i, threshold) 
        p = h.ProjectionY(pname, bmin, bmax)
        f.cd(dname)
        p.Write()
    f.Close()
    
def PlotUncalibratedForThesis() :
    fname = "Spectra/Setall_t25-40_500keVScint.root"
    U = ["U", "L", "L", "U"]
    X = ["Y", "Y", "X", "X"]
    n = [36 , 33 , 11 , 7]
    s = [36 , 33 , 11, 33]
    h = [0, 0, 0, 0]
    t = [0, 0, 0, 0]
    c = TCanvas()
    c.Divide(2, 2)
    for i in range(len(U)) :
        c.cd(i+1)
        h[i] = PlotSpectrumForStrip(fname, U[i], X[i], n[i])
        h[i].GetXaxis().SetRangeUser(0, 275)
        h[i].Draw("HIST")
        ul = ""
        if (U[i] == "U") :
            ul = "Upper"
        else :
            ul = "Lower"
        text = "%s - %s%d" % (ul, X[i], s[i])
        t[i] = TLatex(50, 25000, text)
        t[i].SetTextSize(0.09)
        t[i].Draw("SAME")
        gPad.SetLogy()
        if (i > 1) :
            gPad.SetTopMargin(1E-5)
            h[i].GetXaxis().SetTitle("Q_{ADC}")
            h[i].GetXaxis().SetTitleSize(0.08)
            h[i].GetXaxis().SetTitleOffset(1)
            h[i].GetXaxis().CenterTitle()
            h[i].GetXaxis().SetLabelSize(0.07)
        else :
            gPad.SetBottomMargin(1E-5)
            h[i].GetXaxis().SetLabelSize(0)
            h[i].GetXaxis().SetTitleSize(0)

        gPad.SetLeftMargin(1.1*gPad.GetLeftMargin())
        h[i].GetYaxis().SetLabelSize(0.07)
        
        if (i % 2 == 0) :
#            gPad.SetRightMargin(1E-5)
            h[i].GetYaxis().SetTitle("Counts")
            h[i].GetYaxis().SetTitleSize(0.09)
            h[i].GetYaxis().SetTitleOffset(1)

        else :
            gPad.SetRightMargin(1.2*gPad.GetRightMargin())
#            gPad.SetLeftMargin(1E-5)
#            h[i].GetYaxis().SetLabelSize(0)
            h[i].GetYaxis().SetTitleSize(0)
    
        
    gPad.Update()
    aa = raw_input("Enter \"SAVE\" to write this to .eps for thesis\n")
    if (aa == "SAVE") :
        c.SaveAs("~/Documents/Reports/Images/bb1_spectra_samples.eps")

def PlotLinearityForThesis() :

    gStyle.SetPalette(55)
    gStyle.SetOptFit(0)

#    f.ls()
    U = ["U", "L"]
    X = ["X", "X"]
    n = [2, 16]
    eth = [18.5, 23.1]
    res = [10.3, 9.2]
    gr = [0, 0]
    h = [0, 0]
    c = TCanvas("scatcan", "scatcan", 500*2, 556)
#    c.Divide(2, 1)
    c.cd(1)

    for i in range(len(U)) :
        fname = "Results/linearity_noCuts_all_%s%s_correctedYX.root" % (U[i], X[i])
        f = TFile(fname)
        name = "%s%s%d-Scatter" % (U[i], X[i], n[i])
        if (f.GetListOfKeys().Contains(name)) :
            h[i] = f.Get(name)
            h[i].SetDirectory(0)
            print h[i].GetName()
            if (h[i].GetEntries() > 1) :
                cor = CorrectCalibration(h[i])
                gr[i] = cor[2]
                gr[i].SetLineColor(kRed)
                gr[i].SetDirectory(0)
                gPad.Update()
                #gr[i].GetXaxis().SetRange(gr[i].FindBin(80),
                 #                         gr[i].FindBin(gPad.GetUxmax()))
    
        f.Close()


    c.Clear()
    c.Divide(2, 1)
    l = []
    emax = 525
    zmax = max(h[0].GetZaxis().GetXmax(), h[1].GetZaxis().GetXmax())
    zmax = 2500
    print "zmax = ", zmax
    for i in range(len(U)) :
        c.cd(i+1)
        h[i].Draw("COLZ")
        h[i].GetXaxis().SetRangeUser(0, emax)
        h[i].GetYaxis().SetRangeUser(0, emax)
        h[i].GetZaxis().SetRangeUser(0, zmax)
        h[i].GetXaxis().SetTitleOffset(1.2*h[i].GetXaxis().GetTitleOffset())
#        gr[i].SetMarkerSize(0.5)
        gr[i].Draw("SAME")
#        gr[i].GetParameter(1)
        # g = CorrectCalibration(h)[2]
        # g.Draw("SAME")
        l.append(TLine(eth[i], eth[i], eth[i]+3*res[i], eth[i]))
        l.append(TLine(eth[i], eth[i], eth[i], eth[i]+3*res[i]))
        l.append(TLine(eth[i], eth[i]+3*res[i], emax-3*res[i], emax))
        l.append(TLine(eth[i]+3*res[i], eth[i], emax, emax-3*res[i]))
        for j in range(4) :
            l[(4*i)+j].SetLineWidth(3)
            l[(4*i)+j].Draw("SAME")
        gPad.Update()

    aa = raw_input("Enter \"SAVE\" to write this to .eps for thesis\n")
    if (aa == "SAVE") :
        c.SaveAs("~/Documents/Reports/Images/bb1_linearity_samples.eps")

    raw_input("")

def PlotLinearityForDefense() :

    gStyle.SetPalette(55)
    gStyle.SetOptFit(0)

#    f.ls()
    U = ["U", "L"]
    X = ["X", "X"]
    n = [2, 16]
    eth = [18.5, 23.1]
    res = [10.3, 9.2]
    gr = [0, 0]
    h = [0, 0]
    c = TCanvas("scatcan", "scatcan", 500, 556)
#    c.Divide(2, 1)
#    c.cd(1)

    for i in range(1) :
        fname = "Results/linearity_noCuts_all_%s%s_correctedYX.root" % (U[i], X[i])
        f = TFile(fname)
        name = "%s%s%d-Scatter" % (U[i], X[i], n[i])
        if (f.GetListOfKeys().Contains(name)) :
            h[i] = f.Get(name)
            h[i].SetDirectory(0)
            print h[i].GetName()
            if (h[i].GetEntries() > 1) :
                cor = CorrectCalibration(h[i])
                gr[i] = cor[2]
                gr[i].SetLineColor(kRed)
                gr[i].SetDirectory(0)
                gPad.Update()
                #gr[i].GetXaxis().SetRange(gr[i].FindBin(80),
                 #                         gr[i].FindBin(gPad.GetUxmax()))
    
        f.Close()


    c.Clear()
    #c.Divide(2, 1)
    l = []
    emax = 525
    zmax = max(h[0].GetZaxis().GetXmax(), h[0].GetZaxis().GetXmax())
    zmax = 2500
    print "zmax = ", zmax
    for i in range(1) :
        #c.cd(i+1)
        h[i].Draw("COLZ")
        h[i].GetXaxis().SetRangeUser(0, emax)
        h[i].GetYaxis().SetRangeUser(0, emax)
        h[i].GetZaxis().SetRangeUser(0, zmax)
        h[i].GetXaxis().SetTitleOffset(1.2*h[i].GetXaxis().GetTitleOffset())
#        gr[i].SetMarkerSize(0.5)
        gr[i].Draw("SAME")
#        gr[i].GetParameter(1)
        # g = CorrectCalibration(h)[2]
        # g.Draw("SAME")
        l.append(TLine(0     , eth[i], gPad.GetUxmax(), eth[i]))
        l.append(TLine(eth[i],      0, eth[i]         , gPad.GetUymax()))
        l.append(TLine(eth[i], eth[i]+3*res[i], emax-3*res[i], emax))
        l.append(TLine(eth[i]+3*res[i], eth[i], emax, emax-3*res[i]))
        for j in range(4) :
            l[(4*i)+j].SetLineWidth(3)
            l[(4*i)+j].Draw("SAME")
        gPad.Update()

    aa = raw_input("Enter \"SAVE\" to write this to .eps for defense\n")
    if (aa == "SAVE") :
        c.SaveAs("~/Documents/Reports/Images/bb1_linearity_samples_defense.eps")

    raw_input("")

def GetProjectionMeanSig(h2d, newname, rbin = 2) :
    h = h2d.ProjectionY(newname, h2d.GetXaxis().FindBin(40.), -1, "e")
    h.Rebin(2)
    fit = TF1("gaus", "gaus(0)", -40, 40)
    print "Setting to ", h.GetBinContent(h.FindBin(0.0))
    fit.SetParameters(h.GetBinContent(h.FindBin(0.0)), 0.0, 11.5)
    h.Fit(fit, "R")
    
    m = fit.GetParameter(1)
    s = fit.GetParameter(2)

    dm = fit.GetParError(1)
    ds = fit.GetParError(2)

    chi2r = fit.GetChisquare()/fit.GetNDF()
    if (chi2r > 1.0) :
        dm = dm * sqrt(chi2r)
        ds = ds * sqrt(chi2r)
    mean = ufloat(m, dm)
    sigm = ufloat(s, ds)
    return [h, fit, mean, sigm]

def PlotResolutionForThesis() :
    gStyle.SetOptFit(0)
    U = ["Upper", "Upper", "Lower", "Lower"]
    X = ["X", "Y", "X", "Y"]
    n = [29, 25, 35, 3]
    t = [30.0, 21.5, 28.7, 24.9]
    text = [0]*4
    rtext = [0]*8
    gr = [0, 0, 0, 0]
    h = [0, 0, 0, 0]
    fit = [0, 0, 0, 0]
    c = TCanvas("scatcan", "scatcan", 550*2, 556*1)
    c.Divide(2, 2)
    temp = [0]*4

    for i in range(len(U)) :
        f = TFile("linearity_all.root")
        hname = "%s_BB1/%s_Plane/Ediff_1s/%s_BB1%s_Plane_Strip%d_Ediff_1s" % (U[i], X[i], U[i], X[i], n[i])
        print hname
        temp[i] = f.Get(hname)
        temp[i].SetDirectory(0)
        c.cd(i+1)
        resname = "Res " + hname
        col = GetProjectionMeanSig(temp[i],"Resolution")
        h[i] = col[0]
        hname = "%s_%s_%d" % (U[i], X[i], n[i])
        h[i].SetName(hname)
        h[i].SetDirectory(0)
        h[i].GetXaxis().SetRangeUser(-95, 95)
        h[i].GetXaxis().SetLabelSize(0.08)
        h[i].GetXaxis().SetTitleSize(0.07)
        h[i].GetYaxis().SetTitleSize(0.09)
        h[i].GetYaxis().SetLabelSize(0.08)
        h[i].GetYaxis().SetTitleOffset(0.8)
#        h[i].GetYaxis().SetRangeUser(0., 430)
        txt = "Events / %d keV" % h[i].GetBinWidth(h[i].FindBin(100))
        h[i].GetYaxis().SetTitle(txt)

        h[i].Draw("HIST")
        gPad.Update()
#        h[i].GetYaxis().SetRangeUser(0, gPad.GetUymax())
        fit[i] = col[1]
        fit[i].Draw("SAME")
        w = ["Mean", "\\sigma"]
        for p in range(1, 3) :
            pars = col[p+1]

            zzz = "%s = %3.1f #pm %3.1f" % (w[p-1], pars.nominal_value, pars.std_dev)
            zzz = w[p-1] + " = " + "{:L}".format(pars) + " keV"
            gPad.Update()
            rtext[p-1+(2*i)] = TLatex(10+(p-1)*3, (0.8-(p-1)*.15)*gPad.GetUymax(), zzz)
            rtext[p-1+(2*i)].SetTextSize(0.07)
            rtext[p-1+(2*i)].Draw("SAME")

                      
        gPad.Update()
        t = "Strip %s %s%d" % (U[i], X[i], n[i])
        text[i] = TLatex(-80, 0.8*gPad.GetUymax(), t)
        text[i].SetTextSize(0.08)
        text[i].Draw("SAME")
        gPad.Update()
        f.Close()

    
    for i in range(1, 5) :
        c.cd(i)
        gPad.Update()

    c.cd()
    gPad.Update()
    aa = raw_input("Enter \"SAVE\" to write this to .eps for thesis\n")
    if (aa == "SAVE") :
        c.SaveAs("~/Documents/Reports/Images/bb1_resolution_samples.eps")
    raw_input("Enter to end")
    f.Close()
    return h

def GetKey(a) :
    return a[1]

def CreateBB1CalTable(uORl) :
    xfile = "bb1_calibration_%sX.dat" % uORl
    xtemp = np.loadtxt(xfile)
    
    x = sorted(xtemp, key=GetKey)

    yfile = "bb1_calibration_%sY.dat" % uORl
    ytemp = np.loadtxt(yfile)

    y = sorted(ytemp, key=GetKey)
    
    home = os.environ['HOME']
    outf = open("%s/Documents/Reports/Tables/bb1_calibration_%s.dat" % (home,uORl), 'w')

    # outf.write("Strip \# & \\multicolumn{3}{c||}{$\\hat{x}$-strips} & \\multicolumn{3}{c|}{$\\hat{y}$-strips} \\\\ \\hline\n")
    # outf.write("         & Calib. & Resolution & $E_{\mathrm{min}}^i$ & Calib. & Resolution & $E_{\mathrm{min}}^i$ \\\\ \n")
    # outf.write("         & $\mathrm{chan}/\\si{\\kilo\\electronvolt}$ & $\\si{\\kilo\\electronvolt}$ & $\\si{\\kilo\\electronvolt}$ & $\mathrm{chan}/\\si{\\kilo\\electronvolt}$ & $\\si{\\kilo\\electronvolt}$ & $\\si{\\kilo\\electronvolt}$ \\\\ \\hline \\hline \\endhead \n")
    xline = 0
    yline = 0
    for i in range(40) :
        outf.write("%d &" % i)
        if (x[xline][1] == i) :
            xx = x[xline]
            outf.write(" %4.3f \\pm %4.3f  &  %3.1f \\pm %3.1f  & %4.1f &" % (xx[3], xx[4], xx[5], xx[6], xx[7]))
            xline = xline + 1
        else :
            outf.write("\\multicolumn{3}{c}{-} &")
        if (y[yline][1] == i) :
            yy = y[yline]
            if (yy[5] < 0.1) :
                outf.write(" %4.3f \\pm %4.3f  &  %4.2f \\pm %4.2f  & %4.1f" % (yy[3], yy[4], yy[5], yy[6], yy[7]))
            else :
                outf.write(" %4.3f \\pm %4.3f  &  %3.1f \\pm %3.1f  & %4.1f" % (yy[3], yy[4], yy[5], yy[6], yy[7]))
            yline = yline + 1
        else :
            outf.write("\\multicolumn{3}{c}{-}")
        outf.write("\\\\  \n")

    outf.close()
    
def CompareSSThreshold() :

    # fname = ["linearity_all.root", "linearity_all_1.root",
    #          "linearity_all_2.root","linearity_all_3.root",
    #          "linearity_all_4.root", "linearity_all_20keV.root",
    #          "linearity_all_30keV.root"]
#    fname = ["linearity_all.root", "linearity_all_30keV.root"]
#    fname = ["test_old.root", "test.root"]
    fname = ["linearity_C_threshold0-.5.root",
             "linearity_C_threshold0-.67.root", "linearity_C_threshold0.root",
            "linearity_C_30keV.root"]
        
    c = TCanvas(1)

    c.Divide(2, 2)
    c.SetTopMargin(5*c.GetTopMargin())
    for f in fname :
        print f
        p = 1

        for uORl in ["U", "L"] :
            for xORy in ["X", "Y"] :
                c.cd(p)
                if (p == 1) :
                    ll = TLatex(0.5, 0.5, f)
                    ll.Draw("SAME")
                det = "Upper"
                if (uORl == "L") :
                    det = "Lower"
                rfile = TFile(f)
                hname = "%s_BB1/%s_Plane/%s_BB1%s_Plane_FiringRate" % (det, xORy,
                                                                       det, xORy)
                h = rfile.Get(hname)
                h.SetDirectory(0)
#                h.GetYaxis().SetRangeUser(0, 100)
                h.Draw("E1X0")
                print h.GetEntries()
                gPad.Update()
                p = p + 1
        raw_input("Enter to continue")

def PlotHitPosForThesis() :
    fname = "linearity_all.root"
    gname = "linearity_geant.root"

    hname = "Upper_BB1/X_Plane/Upper_BB1X_Plane_FiringRate"

    c = TCanvas("poscan", "poscan",  600, 500)
    c.cd()
    c.SetRightMargin(2*c.GetRightMargin())
    c.SetTopMargin(1.5*c.GetTopMargin())
    f = TFile(fname)
    h = f.Get(hname)
    h.SetDirectory(0)
    h.SetTitle("Data (Upper Strip X Plane)")
    f.Close()

    hname = "Lower_BB1/X_Plane/Lower_BB1X_Plane_FiringRate"
    g = TFile(gname)
    sim = g.Get(hname)
    sim.SetDirectory(0)
    sim.SetTitle("Simulation")
    g.Close()

    scale = h.Integral()/sim.Integral()
    sim.Scale(scale)
    print "Scaling by ", scale

    h.Draw("HIST")
    sim.SetLineColor(kRed)
    sim.SetMarkerColor(kRed)
    sim.Draw("HIST SAME")

    l = c.BuildLegend(.23, .26, .80, .42)
    l.SetShadowColor(0)
    l.SetFillColor(18)
    c.Update()
    
    aa = raw_input("Enter \"SAVE\" to write this to .eps for thesis\n")
    if (aa == "SAVE") :
        c.SaveAs("~/Documents/Reports/Images/bb1_UX_firing.eps")

def ConvoluteWithGaussian(hist, sigma) :
    print "Convoluting with gaussian...",
    sys.stdout.flush()
    bmax = hist.GetNbinsX()

    xmin = hist.GetBinLowEdge(1)
    xmax = hist.GetBinLowEdge(bmax) + hist.GetBinWidth(bmax)

    initial_rea = hist.Integral(1, bmax)
    theclone = TH1D(hist)

    g = TF1("gaus", "gaus(0)", xmin, xmax)
    g.SetParameter(2, sigma)

    for i in range(1, bmax) :
        content = 0.0
        center = hist.GetBinCenter(i)
        for b in range(1, bmax) :
            g.SetParameter(0, theclone.GetBinContent(b))
            g.SetParameter(1, theclone.GetXaxis().GetBinCenter(b))
            contrib = g.Eval(center)
            content = content + contrib
        hist.SetBinContent(i, content)
    print " DONE"
    return hist

def GetTDiffSigma(fname, uORl) :
    f = TFile(fname)
    bname = "Upper"
    if (uORl == "L") :
        bname = "Lower"
    hname = "%s_BB1/%s_BB1_TimeDifference" % (bname, bname)
    h2d = f.Get(hname)
    h2d.SetDirectory(0)
    h = h2d.ProjectionY("proj" + uORl, h2d.GetXaxis().FindBin(40.), -1, "e")
    h.GetYaxis().SetTitle("Counts")
    h.GetYaxis().SetTitleOffset(1.4)
    h.GetYaxis().SetLabelSize(0.04)
    h.GetYaxis().SetTitleSize(0.06)
    h.GetXaxis().SetRangeUser(-23, 23)
    h.SetDirectory(0)
    h.Draw("HIST")

    func = TF1("gaus", "gaus(0)", -20, 20)
    func.SetParameters(h.GetBinContent(h.FindBin(0)), 0.0, 4.2)
    h.Fit(func, "")
    func.Draw("SAME")
    sigerr = func.GetParError(2)
    cenerr = func.GetParError(1)
    chi2r = func.GetChisquare()/func.GetNDF()
    if (chi2r > 1.0) :
        sigerr = sigerr  * sqrt(chi2r)
        cenerr = cenerr  * sqrt(chi2r)
    gPad.Update()
    f.Close()
    sig = [ufloat(func.GetParameter(1), cenerr),
           ufloat(func.GetParameter(2), sigerr)]
      
    return sig

def PlotTDiffForThesis(fname) :
    gStyle.SetOptFit(0)
    canx = 800
    ar = 1.7
    cany = int(canx/ar)
    tmargin = 0.1
    lmargin = 0.0
    x = 0.5 + (lmargin*0.5)
    print x
    c = TCanvas("tdiff", "tdiff", canx, cany)
    c.Divide(2, 1)
    p1 = TPad("p1", "p1", lmargin, 0, x, 1.0)
    p2 = TPad("p2", "p2", 0.5 + 0.5*lmargin      , 0, 1.0, 1.0)
    p1.Draw()
    p2.Draw()
    p1.cd()
    gPad.SetTopMargin(tmargin)
    res = GetTDiffSigma(fname, "U")
    p1.Update()
    ulatex = []
    ulatex.append(TLatex(-5, 900+gPad.GetUymax(), "Upper Detector"))
    ulatex.append(TLatex(7., 0.8*gPad.GetUymax(), "Mean = %4.2f #pm %4.2f" % (res[0].nominal_value, res[0].std_dev)))
    ulatex.append(TLatex(7., 0.7*gPad.GetUymax(), "#sigma = %4.2f #pm %4.2f" % (res[1].nominal_value, res[1].std_dev)))
    for u in ulatex :
        u.SetTextFont(132)
        u.Draw("SAME")
    p1.Update()
    p2.cd()
    
    gPad.SetTopMargin(tmargin)
    res = GetTDiffSigma(fname, "L")
    gPad.Update()
    llatex = []
    llatex.append(TLatex(-5.0, 900+gPad.GetUymax(), "Lower Detector"))
    llatex.append(TLatex(7., 0.8*gPad.GetUymax(), "Mean = %4.2f #pm %4.2f" % (res[0].nominal_value, res[0].std_dev)))
    llatex.append(TLatex(7., 0.7*gPad.GetUymax(), "#sigma = %4.2f #pm %4.2f" % (res[1].nominal_value, res[1].std_dev)))
    for l in llatex :
        l.SetTextFont(132)
        l.Draw("SAME")
    gPad.Update()
    p2.Update()
    
    aa = raw_input("Enter \"SAVE\" to write this to .eps for thesis\n")
    if (aa == "SAVE") :
        c.SaveAs("~/Documents/Reports/Images/bb1_timeComparison.eps")

    
    
def PlotDetectorEWithGeant(uORl = "U") :
    fname = ["linearity_all_SN.17.root", "linearity_geant_SN.17.root"]
    x = 1600
    ar = 1.7
    y = int(x/1.7)
    c = TCanvas("overlay", "overlay", x/2, y)
    c.SetTopMargin(1.2*c.GetTopMargin())
    sigma = 10.7
    pname = "Upper"
    if (uORl == "L") :
        pname = "Lower"
        sigma = 9.4

    hname = "%s_BB1/%s_BB1_Energy" % (pname, pname)
    hist = []
    for fn in fname :
        f = TFile(fn)
        h = f.Get(hname)
        h.SetDirectory(0)
        hist.append(h)

    hist[1] = ConvoluteWithGaussian(hist[1], sigma)
    sfactor = hist[0].GetBinContent(hist[0].GetMaximumBin())
    sfactor = sfactor / hist[1].GetBinContent(hist[1].GetMaximumBin())
    hist[0].SetTitle("Data")
    hist[1].SetTitle("Simulation (Geant4)")

    for i in range(2) :
        hist[i].SetLineWidth(2)
        hist[i].SetMarkerSize(0)

    hist[0].SetLineColor(kBlue)
    hist[1].SetLineColor(kRed)
    hist[1].Scale(sfactor)
    hist[0].Draw("HIST")
    hist[0].GetXaxis().SetRangeUser(40, 525)
    hist[0].GetXaxis().SetTitle(pname + " " + hist[0].GetXaxis().GetTitle())
    hist[1].Draw("SAME")


    leg = c.BuildLegend(.30, .69, .94, .90)
    leg.SetShadowColor(0)
    leg.SetFillColor(18)
    leg.SetTextFont(132)
    leg.SetTextSize(0.056)

    t1 = TLine(60, 0, 60, 110000)
    a1 = TArrow(60, 110000, 300, 110000)
    a1.SetArrowSize(0.02)
    for l in [a1, t1] :
        l.SetLineWidth(2)
        l.SetLineStyle(2)
        l.Draw()
        

    gPad.Update()  

    mean = [0]*2
    for i in range(2) :
        n = 0
        d = 0
        for b in range(hist[i].FindBin(60), hist[i].FindBin(525)) :
            n = n + hist[i].GetBinCenter(b)*hist[i].GetBinContent(b)
            d = d + hist[i].GetBinContent(b)
        mean[i] = n / d
    mdiff = abs(mean[1]-mean[0])
    print "Difference in mean = %f keV" % mdiff
    aa = raw_input("Enter \"SAVE\" to save canvas as .eps\t")
    if (aa == "SAVE") :
        outfname = "~/Documents/Reports/Images/bb1_overlay_%s.eps" % pname.lower()
        c.SaveAs(outfname)
    aa = raw_input("Enter \"SAVE\" to save canvas as .tex\t")
    if (aa == "SAVE") :
        outfname = "~/Documents/Reports/Images/bb1_overlay_%s.tex" % pname.lower()
        c.SaveAs(outfname)

    return c
    
    
def Make1DPositionPlot() :
    gStyle.SetOptTitle(0)
    gStyle.SetOptStat(0)
    c = TCanvas("c", "c", 900, int(900/1.7))
#    c.SetLeftMargin(2*c.GetLeftMargin())
    p1 = TPad("p1", "p1", 0.00, 0.40, 0.99, 0.95)
    p1.SetBottomMargin(0.0)
    p1.SetBorderMode(1)
#    p1.SetBorderSize(0)
    p1.Draw()
    p2 = TPad("p2", "p2", 0.0, 0.0, 0.99, 0.34)
    p2.SetTopMargin(0.)
    p2.SetBottomMargin(2*p2.GetBottomMargin())
#    p2.SetBorderMode(0)
#    p2.SetBorderSize(0)
    p2.Draw()

    #    f = TFile("linearity_all_30keV_multi.root")
    #    f = TFile("linearity_all_threshold0-.67.root")
    f = TFile("linearity_all_SN.17.root")
    gf = TFile("linearity_geant.root")
    h = f.Get("Lower_BB1/X_Plane/Lower_BB1X_Plane_FiringRate")
    h.SetDirectory(0)
    g = gf.Get("Lower_BB1/X_Plane/Lower_BB1X_Plane_FiringRate")
    g.SetDirectory(0)
    hc = f.Get("Lower_BB1/X_Plane/Lower_BB1X_Plane_FiringRate")
    gc = gf.Get("Lower_BB1/X_Plane/Lower_BB1X_Plane_FiringRate")
    hc.SetDirectory(0)
    gc.SetDirectory(0)
    print h.GetYaxis()
    print hc.GetYaxis()
    p1.cd()
    h.GetYaxis().SetRangeUser(30001, 1.1*h.GetBinContent(h.GetMaximumBin()))
    h.GetXaxis().SetTitle("")
    h.GetXaxis().SetLabelSize(0.)
    h.GetXaxis().SetTickLength(0.)
    h.GetYaxis().SetTitleSize(0.1)
    h.GetYaxis().SetTitleOffset(0.53)
    h.GetYaxis().SetLabelSize(0.05)
    h.GetYaxis().CenterTitle(1)
    h.GetYaxis().SetNoExponent(1)
    h.Draw("HIST")
    #g.Scale(h.GetBinContent(h.FindBin(0.5))/g.GetBinContent(h.FindBin(0.5)))
    g.Scale(h.Integral()/g.Integral())
    gc.Scale(h.Integral()/gc.Integral())
    g.SetLineColor(kRed)
    gc.SetLineColor(kRed)
#    g.Draw("SAME")
    p1.Update()

    p2.cd()
    p2.SetTickx(0)
    hc.Draw("HIST")
    hc.GetYaxis().SetRangeUser(0.,1700.)
    hc.GetYaxis().SetLabelSize(0.1)
    hc.GetYaxis().SetTitle("")
    hc.GetXaxis().SetTitleSize(0.12)
    hc.GetXaxis().SetLabelSize(0.1)
  #  gc.Draw("SAME")
    p2.Update()
    f.Close()
    c.cd()
    p3 = TPad("p3", "p3", 0.0, 0.34, .99, .41)
    p3.SetBorderMode(0)
    p3.Draw()
    p3.cd()
    la = []
    for x in [0.1, 0.98] :
        d = 0.02
        dy = 3*d
        la.append(TLine(x, 0., x, 0.35))
        la.append(TLine(x, 0.65, x, 1))
        la.append(TLine(x-d, 0.35-dy, x+d, 0.35+dy))
        la.append(TLine(x-d, 0.65-dy, x+d, 0.65+dy))
    la.append(TLine(0.1, 1, 0.9, 1))
    for l in la :
        l.Draw("SAME")
    c.Update()
    aa = raw_input("Enter \"SAVE\" to save as .eps\t")
    if (aa == "SAVE") :
        c.SaveAs("~/Documents/Reports/Images/bb1_hitPosition.eps")
    return c
