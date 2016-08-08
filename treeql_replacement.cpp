// Created by Melissa Anholm
// Oct 2015 - 
// 
// 
// The intention of this file is to store, as a C++ class, a
// whole bunch of variables and parameters that are specific
// to each individual run.  
// 
// You'd think I could just store them as a text file and 
// load it up every time, and I could, but that seems like a
// lot of extra work.  Every time I want these values, I'll
// want them to already be in a class so that I can call them
// run-by-run with minimal fuss.  I don't even want to make a
// class that reads and loads up a file full of parameters, 
// because that's extra overhead, and it's 100% fine if all 
// the values are static anyway.  
// 
// 

class set_of_runs
{
private:
public:
	const static int n_runs = 517;

	bool compton_edge_ok[n_runs];
	double OP_Delay[n_runs];
	//
	int sumdiff_set[n_runs];
	//
	double x_all_sumdiff_mode[n_runs];    
	double x_all_sumdiff_maxcount[n_runs];
	double x_all_sumdiff_01_min[n_runs];  
	double x_all_sumdiff_01_max[n_runs];  
	double x_all_sumdiff_10_min[n_runs];  
	double x_all_sumdiff_10_max[n_runs];  
	double z_all_sumdiff_mode[n_runs];    
	double z_all_sumdiff_maxcount[n_runs];
	double z_all_sumdiff_01_min[n_runs];  
	double z_all_sumdiff_01_max[n_runs];  
	double z_all_sumdiff_10_min[n_runs];  
	double z_all_sumdiff_10_max[n_runs];  

	double x_pi_ac_sumdiff_mode[n_runs];    
	double x_pi_ac_sumdiff_maxcount[n_runs];
	double x_pi_ac_sumdiff_01_min[n_runs];  
	double x_pi_ac_sumdiff_01_max[n_runs];  
	double x_pi_ac_sumdiff_10_min[n_runs];  
	double x_pi_ac_sumdiff_10_max[n_runs];  
	double z_pi_ac_sumdiff_mode[n_runs];    
	double z_pi_ac_sumdiff_maxcount[n_runs];
	double z_pi_ac_sumdiff_01_min[n_runs];  
	double z_pi_ac_sumdiff_01_max[n_runs];  
	double z_pi_ac_sumdiff_10_min[n_runs];  
	double z_pi_ac_sumdiff_10_max[n_runs];  

	double x_r_sumdiff_mode[n_runs];    
	double x_r_sumdiff_maxcount[n_runs];
	double x_r_sumdiff_01_min[n_runs];  
	double x_r_sumdiff_01_max[n_runs];  
	double x_r_sumdiff_10_min[n_runs];  
	double x_r_sumdiff_10_max[n_runs];  
	double z_r_sumdiff_mode[n_runs];    
	double z_r_sumdiff_maxcount[n_runs];
	double z_r_sumdiff_01_min[n_runs];  
	double z_r_sumdiff_01_max[n_runs];  
	double z_r_sumdiff_10_min[n_runs];  
	double z_r_sumdiff_10_max[n_runs];  

	double x_gr_sumdiff_mode[n_runs];    
	double x_gr_sumdiff_maxcount[n_runs];
	double x_gr_sumdiff_01_min[n_runs];  
	double x_gr_sumdiff_01_max[n_runs];  
	double x_gr_sumdiff_10_min[n_runs];  
	double x_gr_sumdiff_10_max[n_runs];  
	double z_gr_sumdiff_mode[n_runs];    
	double z_gr_sumdiff_maxcount[n_runs];
	double z_gr_sumdiff_01_min[n_runs];  
	double z_gr_sumdiff_01_max[n_runs];  
	double z_gr_sumdiff_10_min[n_runs];  
	double z_gr_sumdiff_10_max[n_runs];  

//	double x_sumdiff_sigma[n_runs];     // nothing.
//	double z_sumdiff_sigma[n_runs];     // nothing.
//	double x_sumdiff_mean_err[n_runs];  // nothing.
//	double z_sumdiff_mean_err[n_runs];  // nothing.
//	double x_sumdiff_sigma_err[n_runs]; // nothing.
//	double z_sumdiff_sigma_err[n_runs]; // nothing.
	
	bool good_recoil[n_runs];      
	bool good_electron[n_runs]; // done.
	int unix_start[n_runs];     // done.
	int unix_stop[n_runs];      // done.
	int t_offset = 1402200000;
	// runtime_avg[i] = ((runs.unix_start[i]-t_offset) + (runs.unix_stop[i]-t_offset) )/2.0;
	int runtime_avg[n_runs];         // in seconds.
	int runtime_length[n_runs];      // in seconds.
	
	double PI_x[n_runs];
	double PI_sigma_x[n_runs];
	double PI_z[n_runs];
	double PI_sigma_z[n_runs];
	double PI_tof[n_runs];
	double PI_sigma_tof[n_runs];
	
//	void init_sumdiffs_old();
	void init_sumdiffs();
	
	void init_goodrecoil();        
	void init_goodelectron();   // done.
	void init_unixtime();       // done.
	void init_trapposition();
	
	double iqdc_min[n_runs];
	double iqdc_max[n_runs];
	void init_iqdc();
	
	bool usable[n_runs];
	void init_usable();
	void set_sumdiff_threshold(int);
	
	double ElectricField[n_runs];
	void init_ElectricField();
	
	set_of_runs(); // done.
};

void set_of_runs::init_unixtime()
{
	for(int i=0; i<n_runs; i++)
	{
		unix_start[i] = 0;
		unix_stop[i] = 0;
		runtime_avg[i] = 0;
		runtime_length[i] = 0;
	}
	
	// int t_offset = 1402200000;
	unix_start[303] = 1402221986;
	unix_stop[303]  = 1402223369;
	unix_start[308] = 1402224052;
	unix_stop[308]  = 1402224712;
	unix_start[309] = 1402224852;
	unix_stop[309]  = 1402225972;
	unix_start[310] = 1402226086;
	unix_stop[310]  = 1402227035;
	unix_start[311] = 1402227610;
	unix_stop[311]  = 1402230530;
	unix_start[312] = 1402230640;
	unix_stop[312]  = 1402231149;
	unix_start[313] = 1402231184;
	unix_stop[313]  = 1402232757;
	unix_start[314] = 1402251821;
	unix_stop[314]  = 1402255014;
	unix_start[318] = 1402261843;
	unix_stop[318]  = 1402265603;
	unix_start[326] = 1402270263;
	unix_stop[326]  = 1402271614;
	unix_start[327] = 1402276252;
	unix_stop[327]  = 1402279408;
	unix_start[328] = 1402280134;
	unix_stop[328]  = 1402287542;
	unix_start[340] = 1402294760;
	unix_stop[340]  = 1402298450;
	unix_start[342] = 1402301604;
	unix_stop[342]  = 1402304029;
	unix_start[343] = 1402304606;
	unix_stop[343]  = 1402308315;
	unix_start[361] = 1402331343;
	unix_stop[361]  = 1402332839;
	unix_start[362] = 1402335507;
	unix_stop[362]  = 1402338055;
	unix_start[363] = 1402338124;
	unix_stop[363]  = 1402342800;
	unix_start[368] = 1402352126;
	unix_stop[368]  = 1402355177;
	unix_start[370] = 1402356790;
	unix_stop[370]  = 1402358747;
	unix_start[371] = 1402358878;
	unix_stop[371]  = 1402362095;
	unix_start[376] = 1402363468;
	unix_stop[376]  = 1402364762;
	unix_start[377] = 1402365168;
	unix_stop[377]  = 1402367035;
	unix_start[378] = 1402367258;
	unix_stop[378]  = 1402369291;
	unix_start[383] = 1402373907;
	unix_stop[383]  = 1402377593;
	unix_start[384] = 1402377802;
	unix_stop[384]  = 1402379836;
	unix_start[385] = 1402380497;
	unix_stop[385]  = 1402381574;
	unix_start[386] = 1402383062;
	unix_stop[386]  = 1402387897;
	unix_start[393] = 1402388633;
	unix_stop[393]  = 1402393117;
	unix_start[394] = 1402396832;
	unix_stop[394]  = 1402399042;
	unix_start[395] = 1402399065;
	unix_stop[395]  = 1402400944;
	unix_start[396] = 1402401038;
	unix_stop[396]  = 1402402931;
	unix_start[398] = 1402403329;
	unix_stop[398]  = 1402405526;
	unix_start[399] = 1402405641;
	unix_stop[399]  = 1402407770;
	unix_start[400] = 1402407897;
	unix_stop[400]  = 1402409742;
	unix_start[401] = 1402409856;
	unix_stop[401]  = 1402412055;
	unix_start[402] = 1402412173;
	unix_stop[402]  = 1402414371;
	unix_start[409] = 1402444610;
	unix_stop[409]  = 1402446224;
	unix_start[410] = 1402446330;
	unix_stop[410]  = 1402447420;
	unix_start[411] = 1402447436;
	unix_stop[411]  = 1402447980;
	unix_start[412] = 1402448151;
	unix_stop[412]  = 1402450167;
	unix_start[413] = 1402450286;
	unix_stop[413]  = 1402453958;
	unix_start[414] = 1402454208;
	unix_stop[414]  = 1402454729;
	unix_start[415] = 1402454798;
	unix_stop[415]  = 1402455865;
	unix_start[416] = 1402456772;
	unix_stop[416]  = 1402460499;
	unix_start[417] = 1402460557;
	unix_stop[417]  = 1402464269;
	unix_start[418] = 1402464425;
	unix_stop[418]  = 1402468369;
	unix_start[419] = 1402468392;
	unix_stop[419]  = 1402470580;
	unix_start[420] = 1402472894;
	unix_stop[420]  = 1402475427;
	unix_start[421] = 1402480094;
	unix_stop[421]  = 1402484085;
	unix_start[422] = 1402484218;
	unix_stop[422]  = 1402487768;
	unix_start[423] = 1402488391;
	unix_stop[423]  = 1402492122;
	unix_start[424] = 1402492552;
	unix_stop[424]  = 1402495920;
	unix_start[425] = 1402496034;
	unix_stop[425]  = 1402499370;
	unix_start[426] = 1402499468;
	unix_stop[426]  = 1402503201;
	unix_start[428] = 1402505809;
	unix_stop[428]  = 1402509564;
	unix_start[429] = 1402509818;
	unix_stop[429]  = 1402513476;
	unix_start[430] = 1402513631;
	unix_stop[430]  = 1402517437;
	unix_start[431] = 1402517591;
	unix_stop[431]  = 1402521328;
	unix_start[432] = 1402521736;
	unix_stop[432]  = 1402525146;
	unix_start[433] = 1402525546;
	unix_stop[433]  = 1402528431;
	unix_start[434] = 1402528651;
	unix_stop[434]  = 1402528831;
	unix_start[435] = 1402528852;
	unix_stop[435]  = 1402531511;
	unix_start[436] = 1402531757;
	unix_stop[436]  = 1402535852;
	unix_start[437] = 1402536087;
	unix_stop[437]  = 1402539774;
	unix_start[440] = 1402541402;
	unix_stop[440]  = 1402544993;
	unix_start[441] = 1402545180;
	unix_stop[441]  = 1402546613;
	unix_start[442] = 1402547323;
	unix_stop[442]  = 1402548647;
	unix_start[443] = 1402549499;
	unix_stop[443]  = 1402552199;
	unix_start[444] = 1402552513;
	unix_stop[444]  = 1402555846;
	unix_start[445] = 1402556667;
	unix_stop[445]  = 1402557692;
	unix_start[446] = 1402561972;
	unix_stop[446]  = 1402563534;
	unix_start[447] = 1402563701;
	unix_stop[447]  = 1402567325;
	unix_start[449] = 1402567801;
	unix_stop[449]  = 1402571625;
	unix_start[450] = 1402585821;
	unix_stop[450]  = 1402589949;
	unix_start[454] = 1402606618;
	unix_stop[454]  = 1402609355;
	unix_start[455] = 1402609556;
	unix_stop[455]  = 1402612834;
	unix_start[460] = 1402615531;
	unix_stop[460]  = 1402619104;
	unix_start[461] = 1402619485;
	unix_stop[461]  = 1402621330;
	unix_start[462] = 1402621525;
	unix_stop[462]  = 1402625073;
	unix_start[463] = 1402625689;
	unix_stop[463]  = 1402629293;
	unix_start[464] = 1402629945;
	unix_stop[464]  = 1402633663;
	unix_start[465] = 1402633797;
	unix_stop[465]  = 1402637285;
	unix_start[466] = 1402637500;
	unix_stop[466]  = 1402640966;
	unix_start[473] = 1402645998;
	unix_stop[473]  = 1402646765;
	unix_start[474] = 1402646901;
	unix_stop[474]  = 1402650169;
	unix_start[476] = 1402652506;
	unix_stop[476]  = 1402656888;
	unix_start[477] = 1402657024;
	unix_stop[477]  = 1402659220;
	unix_start[478] = 1402661060;
	unix_stop[478]  = 1402664567;
	unix_start[479] = 1402664846;
	unix_stop[479]  = 1402668372;
	unix_start[480] = 1402668469;
	unix_stop[480]  = 1402671716;
	unix_start[481] = 1402672248;
	unix_stop[481]  = 1402673043;
	unix_start[482] = 1402673575;
	unix_stop[482]  = 1402675699;
	unix_start[483] = 1402676686;
	unix_stop[483]  = 1402680376;
	unix_start[484] = 1402680528;
	unix_stop[484]  = 1402683736;
	unix_start[485] = 1402684424;
	unix_stop[485]  = 1402689062;
	unix_start[486] = 1402689265;
	unix_stop[486]  = 1402692423;
	unix_start[487] = 1402692663;
	unix_stop[487]  = 1402693425;
	unix_start[488] = 1402693471;
	unix_stop[488]  = 1402694223;
	unix_start[489] = 1402694243;
	unix_stop[489]  = 1402694948;
	unix_start[491] = 1402698356;
	unix_stop[491]  = 1402699476;
	unix_start[497] = 1402701711;
	unix_stop[497]  = 1402702088;
	unix_start[498] = 1402702113;
	unix_stop[498]  = 1402704078;
	unix_start[499] = 1402704287;
	unix_stop[499]  = 1402704845;
	unix_start[502] = 1402708215;
	unix_stop[502]  = 1402711889;
	unix_start[503] = 1402712078;
	unix_stop[503]  = 1402714093;
	unix_start[504] = 1402714338;
	unix_stop[504]  = 1402719734;
	unix_start[505] = 1402719832;
	unix_stop[505]  = 1402722613;
	unix_start[509] = 1402739164;
	unix_stop[509]  = 1402740801;
	unix_start[510] = 1402741781;
	unix_stop[510]  = 1402747671;
	unix_start[513] = 1402751156;
	unix_stop[513]  = 1402752497;
	
	runtime_avg[499] = 504566;
	runtime_avg[376] = 164115;
	runtime_avg[411] = 247708;
	runtime_avg[411] = 247708;
	runtime_avg[460] = 417317;
	runtime_avg[461] = 420407;
	runtime_avg[462] = 423299;
	runtime_avg[463] = 427491;
	runtime_avg[464] = 431804;
	runtime_avg[465] = 435541;
	runtime_avg[466] = 439233;
	runtime_avg[473] = 446381;
	runtime_avg[474] = 448535;
	runtime_avg[491] = 498916;
	runtime_avg[498] = 503095;
	runtime_avg[499] = 504566;
	runtime_avg[509] = 539982;
	runtime_avg[376] = 164115;
	runtime_avg[377] = 166101;
	runtime_avg[378] = 168274;
	runtime_avg[394] = 197937;
	runtime_avg[395] = 200004;
	runtime_avg[396] = 201984;
	runtime_avg[398] = 204427;
	runtime_avg[399] = 206705;
	runtime_avg[400] = 208819;
	runtime_avg[401] = 210955;
	runtime_avg[402] = 213272;
	runtime_avg[450] = 387885;
	runtime_avg[454] = 407986;
	runtime_avg[455] = 411195;
	runtime_avg[409] = 245417;
	runtime_avg[410] = 246875;
	runtime_avg[411] = 247708;
	runtime_avg[412] = 249159;
	runtime_avg[413] = 252122;
	runtime_avg[414] = 254468;
	runtime_avg[415] = 255331;
	runtime_avg[416] = 258635;
	runtime_avg[417] = 262413;
	runtime_avg[418] = 266397;
	runtime_avg[419] = 269486;
	runtime_avg[421] = 282089;
	runtime_avg[422] = 285993;
	runtime_avg[423] = 290256;
	runtime_avg[424] = 294236;
	runtime_avg[425] = 297702;
	runtime_avg[426] = 301334;
	runtime_avg[446] = 362753;
	runtime_avg[447] = 365513;
	runtime_avg[449] = 369713;
	runtime_avg[303] = 22677;
	runtime_avg[308] = 24382;
	runtime_avg[309] = 25412;
	runtime_avg[310] = 26560;
	runtime_avg[311] = 29070;
	runtime_avg[313] = 31970;
	runtime_avg[318] = 63723;
	runtime_avg[326] = 70938;
	runtime_avg[312] = 30894;
	runtime_avg[327] = 77830;
	runtime_avg[328] = 83838;
	runtime_avg[340] = 96605;
	runtime_avg[342] = 102816;
	runtime_avg[343] = 106460;
	runtime_avg[361] = 132091;
	runtime_avg[368] = 153651;
	runtime_avg[370] = 157768;
	runtime_avg[371] = 160486;
	runtime_avg[497] = 501899;

	runtime_length[499] = 558;
	runtime_length[376] = 1294;
	runtime_length[411] = 544;
	runtime_length[411] = 544;
	runtime_length[460] = 3573;
	runtime_length[461] = 1845;
	runtime_length[462] = 3548;
	runtime_length[463] = 3604;
	runtime_length[464] = 3718;
	runtime_length[465] = 3488;
	runtime_length[466] = 3466;
	runtime_length[473] = 767;
	runtime_length[474] = 3268;
	runtime_length[491] = 1120;
	runtime_length[498] = 1965;
	runtime_length[499] = 558;
	runtime_length[509] = 1637;
	runtime_length[376] = 1294;
	runtime_length[377] = 1867;
	runtime_length[378] = 2033;
	runtime_length[394] = 2210;
	runtime_length[395] = 1879;
	runtime_length[396] = 1893;
	runtime_length[398] = 2197;
	runtime_length[399] = 2129;
	runtime_length[400] = 1845;
	runtime_length[401] = 2199;
	runtime_length[402] = 2198;
	runtime_length[450] = 4128;
	runtime_length[454] = 2737;
	runtime_length[455] = 3278;
	runtime_length[409] = 1614;
	runtime_length[410] = 1090;
	runtime_length[411] = 544;
	runtime_length[412] = 2016;
	runtime_length[413] = 3672;
	runtime_length[414] = 521;
	runtime_length[415] = 1067;
	runtime_length[416] = 3727;
	runtime_length[417] = 3712;
	runtime_length[418] = 3944;
	runtime_length[419] = 2188;
	runtime_length[421] = 3991;
	runtime_length[422] = 3550;
	runtime_length[423] = 3731;
	runtime_length[424] = 3368;
	runtime_length[425] = 3336;
	runtime_length[426] = 3733;
	runtime_length[446] = 1562;
	runtime_length[447] = 3624;
	runtime_length[449] = 3824;
	runtime_length[303] = 1383;
	runtime_length[308] = 660;
	runtime_length[309] = 1120;
	runtime_length[310] = 949;
	runtime_length[311] = 2920;
	runtime_length[313] = 1573;
	runtime_length[318] = 3760;
	runtime_length[326] = 1351;
	runtime_length[312] = 509;
	runtime_length[327] = 3156;
	runtime_length[328] = 7408;
	runtime_length[340] = 3690;
	runtime_length[342] = 2425;
	runtime_length[343] = 3709;
	runtime_length[361] = 1496;
	runtime_length[368] = 3051;
	runtime_length[370] = 1957;
	runtime_length[371] = 3217;
	runtime_length[497] = 377;

}

void set_of_runs::init_trapposition()
{
	for(int i=0; i<n_runs; i++)
	{
		PI_x[i] = 0;
		PI_z[i] = 0;
		PI_tof[i] = 0;
		PI_sigma_x[i] = 0;
		PI_sigma_z[i] = 0;
		PI_sigma_tof[i] = 0;
	}
}

/*
void set_of_runs::init_sumdiffs_old()
{
	// cuts are created using the "recoils only" spectra.
	// ...or, maybe "all" ??
	// 
	// See:  Anal-Ysys/Trap_Position/imcp_trapposition_sdfits.cpp
	// and:  Anal-Ysis/Trap_Position/Output/
	// to see the code and files used.
	//
	for(int i=0; i<n_runs; i++)
	{
		sumdiff_set[i] = 0;
		x_sumdiff_01_min[i] = 0.0;
		x_sumdiff_01_max[i] = 0.0;
		z_sumdiff_01_min[i] = 0.0;
		z_sumdiff_01_max[i] = 0.0;
	//	x_sumdiff_mean[i] = 0.0; 
	//	z_sumdiff_mean[i] = 0.0; 
	//	x_sumdiff_mean_err[i] = 0.0; 
	//	z_sumdiff_mean_err[i] = 0.0; 
	//	x_sumdiff_sigma[i] = 0.0; 
	//	z_sumdiff_sigma[i] = 0.0; 
	//	x_sumdiff_sigma_err[i] = 0.0; 
	//	z_sumdiff_sigma_err[i] = 0.0; 
		
		if(i<342)
		{
			// did not actually check these here.  Now I did.  They bias TOF.
			x_sumdiff_01_min[i] = -1107.0;
			x_sumdiff_01_max[i] = -812.0;
			z_sumdiff_01_min[i] = -509.0;
			z_sumdiff_01_max[i] = -206.0;
	//		x_sumdiff_mean[i] = -930.598; 
	//		z_sumdiff_mean[i] = -322.556; 
		}
		else if(i>=342 && i<=368)
		{
			sumdiff_set[i] = 1;
			// did not actually check these here.  Now I did.  They bias TOF.
			x_sumdiff_01_min[i] = -1107.0;
			x_sumdiff_01_max[i] = -812.0;
			z_sumdiff_01_min[i] = -509.0;
			z_sumdiff_01_max[i] = -206.0;
	//		x_sumdiff_mean[i] = -930.598; 
	//		z_sumdiff_mean[i] = -322.556; 
		}
		else if(i>368 && i<=449)  // contains all of 535 + "most" of 395
		{
			sumdiff_set[i] = 2;   // split??  ... no, most are just double-peaked.
	//		
	//		// using "recoils only" sums.
	//		// x: max:  x~-904, y~7659.
	//		// x: 1% at 77:  -1107, -812
	//		1  Constant     7.97274e+03   1.45735e+01   0.00000e+00  -1.15903e-08
	//		2  Mean        -9.30598e+02   9.26961e-02   0.00000e+00  -1.20227e-05
	//		3  Sigma        4.50505e+01   4.78554e-02  -0.00000e+00   9.13269e-04
	//		
			x_sumdiff_01_min[i] = -1107.0;
			x_sumdiff_01_max[i] =  -812.0;
	//		
	//		// z: max:  x~-302, y~-7696.
	//		// z:  1% at 77:  -509, -206
	//		1  Constant     8.06795e+03   1.45258e+01  -0.00000e+00   1.77857e-08
	//		2  Mean        -3.22556e+02   9.07358e-02  -0.00000e+00   1.89920e-05
	//		3  Sigma        4.33450e+01   4.24046e-02   0.00000e+00  -4.77916e-03
	//		
			z_sumdiff_01_min[i] = -509.0;
			z_sumdiff_01_max[i] = -206.0;
			
		//	x_sumdiff_mean[i] = -930.598; 
		//	z_sumdiff_mean[i] = -322.556; 
		}
		else if(i==450)  // 395
		{
			sumdiff_set[i] = 3;
	//		
	//		// x:  max:  x~-883, y=2079.
	//		// x:  1% at 21:  -1095, -796.
	//		1  Constant     1.92812e+03   7.76429e+00   3.09076e-01   3.93576e-07
	//		2  Mean        -9.08836e+02   1.91320e-01   7.15720e-03   1.93540e-04
	//		3  Sigma        4.46323e+01   1.17634e-01   2.55442e-05   3.94055e-02
	//		
			x_sumdiff_01_min[i] = -1095.0;
			x_sumdiff_01_max[i] =  -796.0;
	//		
	//		// z:  max:  x~-272, y=1597.
	//		// z:  1% at 16:  -503, -185.
	//		1  Constant     1.52950e+03   6.65896e+00   0.00000e+00  -2.98355e-09
	//		2  Mean        -3.07042e+02   2.31308e-01   0.00000e+00   2.25597e-05
	//		3  Sigma        4.61571e+01   1.27832e-01  -0.00000e+00   9.29244e-03
	//		
			z_sumdiff_01_min[i] = -503.0;
			z_sumdiff_01_max[i] = -185.0;
			
		//	x_sumdiff_mean[i] = -908.836; 
		//	z_sumdiff_mean[i] = -307.042; 
		}
		else if(i>450 && i<=465) // 395, 415
		{
			sumdiff_set[i] = 4;   // split??  ..no, just double-peaked.
	//		
	//		// x:  max:  x~-865, y=5885
	//		// x:  1% at 59:  -1095, -784
	//		1  Constant     5.90446e+03   1.30596e+01   0.00000e+00   3.76755e-09
	//		2  Mean        -9.02161e+02   1.17982e-01   0.00000e+00  -2.13032e-06
	//		3  Sigma        4.47743e+01   6.15389e-02  -0.00000e+00   7.21397e-04
	//		
			x_sumdiff_01_min[i] = -1095.0;
			x_sumdiff_01_max[i] =  -784.0;
	//		
	//		// z:  max:  x~-261, y=5711
	//		// z:  1% at 57:  -479, -180
	//		1  Constant     5.94610e+03   1.23461e+01  -0.00000e+00   2.02776e-08
	//		2  Mean        -2.99396e+02   1.05357e-01  -0.00000e+00   6.76870e-06
	//		3  Sigma        4.47357e+01   5.12118e-02   0.00000e+00  -2.07758e-03
	//		
			z_sumdiff_01_min[i] = -479.0;
			z_sumdiff_01_max[i] = -180.0;
			
		//	x_sumdiff_mean[i] = -902.161; 
		//	z_sumdiff_mean[i] = -299.396; 
		}
		else if(i==466) // 415
		{
			sumdiff_set[i] = 5;  // "split?"  ...yeah, no.
	//		
	//		// x:  max:  x~-854, y=1762
	//		// x:  1% at 18:  -1095, -778
	//		1  Constant     1.61514e+03   6.87336e+00   0.00000e+00  -6.15822e-09
	//		2  Mean        -8.96444e+02   2.42204e-01   0.00000e+00   8.35705e-07
	//		3  Sigma        4.47080e+01   1.19322e-01  -0.00000e+00   1.92561e-02
	//		
			x_sumdiff_01_min[i] = -1095.0;
			x_sumdiff_01_max[i] =  -778.0;
	//		
	//		// z:  max:  x~-253, y=1717
	//		// z:  1% at 17:  -487, -174
	//		1  Constant     1.60307e+03   6.71826e+00  -0.00000e+00   2.26346e-08
	//		2  Mean        -2.90987e+02   2.27861e-01  -0.00000e+00   4.04235e-06
	//		3  Sigma        4.48301e+01   1.14282e-01   0.00000e+00  -8.69599e-04
	//		
			z_sumdiff_01_min[i] = -487.0;
			z_sumdiff_01_max[i] = -174.0;
			
		//	x_sumdiff_mean[i] = -896.444; 
		//	z_sumdiff_mean[i] = -290.987; 
		}
		else if(i==473)  // 415
		{
			sumdiff_set[i] = 6;
	//		
	//		// x:  max:  x~-907, y=1201
	//		// x:  1% at 12:  -1153, -812
	//		1  Constant     1.28859e+03   5.55548e+00   2.44310e-01  -3.60020e-06
	//		2  Mean        -9.41541e+02   2.54065e-01   9.68143e-03   8.92451e-05
	//		3  Sigma        5.07388e+01   1.28870e-01   2.59446e-05   7.31778e-04
	//		
			x_sumdiff_01_min[i] = -1153.0;
			x_sumdiff_01_max[i] =  -812.0;
	//		
	//		// z:  max:  x~-293, y=1255
	//		// z:  1% at 13:  -543, -202
	//		1  Constant     1.30643e+03   5.73690e+00   2.58211e-01   3.97760e-05
	//		2  Mean        -3.30089e+02   2.54647e-01   9.69031e-03  -6.45268e-04
	//		3  Sigma        4.86857e+01   1.27063e-01   2.60405e-05  -2.52527e-01
	//		
			z_sumdiff_01_min[i] = -543.0;
			z_sumdiff_01_max[i] = -202.0;
			
		//	x_sumdiff_mean[i] = -941.541; 
		//	z_sumdiff_mean[i] = -330.089; 
		}
		else if(i==474)  // 415
		{
			sumdiff_set[i] = 6;
	//		
	//		// x:  max:  x~-907, y=4429
	//		// x:  1% at 44:  -1155, -810
	//		1  Constant     4.87626e+03   1.08887e+01  -0.00000e+00  -8.70551e-09
	//		2  Mean        -9.41400e+02   1.38128e-01  -0.00000e+00  -2.03665e-07
	//		3  Sigma        4.92672e+01   6.39343e-02   0.00000e+00  -8.07806e-04
	//		
			x_sumdiff_01_min[i] = -1155.0;
			x_sumdiff_01_max[i] =  -810.0;
	//		
	//		// z:  max:  x~-293, y=4520
	//		// z:  1% at 45:  -565, -204
	//		1  Constant     4.86329e+03   1.12007e+01   0.00000e+00  -1.68468e-09
	//		2  Mean        -3.29100e+02   1.41500e-01   0.00000e+00  -1.61894e-06
	//		3  Sigma        4.69430e+01   6.31142e-02  -0.00000e+00   9.04146e-02
	//		
			z_sumdiff_01_min[i] = -565.0;
			z_sumdiff_01_max[i] = -204.0;
			
		//	x_sumdiff_mean[i] = -941.400; 
		//	z_sumdiff_mean[i] = -329.100; 
		}
		else if(i>474 && i<=498)  // 415
		{
			sumdiff_set[i] = 8;
	//		
	//		// x:  max:  x~-931, y=746
	//		// x:  1% at 7:  -1123, -808
	//		1  Constant     7.67461e+02   4.11984e+00   8.11540e-02  -5.01758e-06
	//		2  Mean        -9.44125e+02   2.63482e-01   5.51262e-03  -2.71914e-06
	//		3  Sigma        5.18139e+01   1.54422e-01   1.72440e-05   5.98827e-03
	//		
			x_sumdiff_01_min[i] = -1123.0;
			x_sumdiff_01_max[i] =  -808.0;
	//		
	//		// z:  max:  x~313, y=792
	//		// z:  1% at 8:  -507, -208
	//		1  Constant     8.23185e+02   4.44153e+00   7.58425e-02  -4.26713e-05
	//		2  Mean        -3.31241e+02   2.37341e-01   4.43004e-03   9.64609e-04
	//		3  Sigma        4.78637e+01   1.43332e-01   1.46786e-05  -5.47927e-03
	//		
			z_sumdiff_01_min[i] = -507.0;
			z_sumdiff_01_max[i] = -208.0;
			
		//	x_sumdiff_mean[i] = -944.125; 
		//	z_sumdiff_mean[i] = -331.241; 
		}
		else if(i==499) // nothing passes cuts.
		{
			sumdiff_set[i] = 9;  
		}
		else if(i==509)  // nothing passes cuts.
		{
			sumdiff_set[i] = 10;
		}
	}
}
*/

void set_of_runs::init_sumdiffs()
{
	// cuts are created using the "recoils only" spectra.
	// ...or, maybe "all" ??
	// 
	// See:  Anal-Ysys/Trap_Position/imcp_trapposition_sdfits_actual.cpp
	// and:  Anal-Ysis/Trap_Position/Output/
	// to see the code and files used.
	//
	for(int i=0; i<n_runs; i++)
	{
		sumdiff_set[i] = 0;
	//	x_sumdiff_01_min[i] = 0.0;
	//	x_sumdiff_01_max[i] = 0.0;
	//	x_sumdiff_mode[i] = 0.0;      
	//	x_sumdiff_maxcount[i] = 0.0;   
		
	//	z_sumdiff_01_min[i] = 0.0;
	//	z_sumdiff_01_max[i] = 0.0;
	//	z_sumdiff_mode[i] = 0.0;       
	//	z_sumdiff_maxcount[i] = 0.0;   
		
	//	x_sumdiff_mean[i] = 0.0; 
	//	z_sumdiff_mean[i] = 0.0; 
	//	x_sumdiff_mean_err[i] = 0.0; 
	//	z_sumdiff_mean_err[i] = 0.0; 
	//	x_sumdiff_sigma[i] = 0.0; 
	//	z_sumdiff_sigma[i] = 0.0; 
	//	x_sumdiff_sigma_err[i] = 0.0; 
	//	z_sumdiff_sigma_err[i] = 0.0; 
		
		x_all_sumdiff_mode[i] = 0.0;
		x_all_sumdiff_maxcount[i] = 0.0;
		x_all_sumdiff_01_min[i] = 0.0;
		x_all_sumdiff_01_max[i] = 0.0;
		x_all_sumdiff_10_min[i] = 0.0;
		x_all_sumdiff_10_max[i] = 0.0;
		z_all_sumdiff_mode[i] = 0.0;
		z_all_sumdiff_maxcount[i] = 0.0;
		z_all_sumdiff_01_min[i] = 0.0;
		z_all_sumdiff_01_max[i] = 0.0;
		z_all_sumdiff_10_min[i] = 0.0;
		z_all_sumdiff_10_max[i] = 0.0;
		x_pi_ac_sumdiff_mode[i] = 0.0;
		x_pi_ac_sumdiff_maxcount[i] = 0.0;
		x_pi_ac_sumdiff_01_min[i] = 0.0;
		x_pi_ac_sumdiff_01_max[i] = 0.0;
		x_pi_ac_sumdiff_10_min[i] = 0.0;
		x_pi_ac_sumdiff_10_max[i] = 0.0;
		z_pi_ac_sumdiff_mode[i] = 0.0;
		z_pi_ac_sumdiff_maxcount[i] = 0.0;
		z_pi_ac_sumdiff_01_min[i] = 0.0;
		z_pi_ac_sumdiff_01_max[i] = 0.0;
		z_pi_ac_sumdiff_10_min[i] = 0.0;
		z_pi_ac_sumdiff_10_max[i] = 0.0;
		x_r_sumdiff_mode[i] = 0.0;
		x_r_sumdiff_maxcount[i] = 0.0;
		x_r_sumdiff_01_min[i] = 0.0;
		x_r_sumdiff_01_max[i] = 0.0;
		x_r_sumdiff_10_min[i] = 0.0;
		x_r_sumdiff_10_max[i] = 0.0;
		z_r_sumdiff_mode[i] = 0.0;
		z_r_sumdiff_maxcount[i] = 0.0;
		z_r_sumdiff_01_min[i] = 0.0;
		z_r_sumdiff_01_max[i] = 0.0;
		z_r_sumdiff_10_min[i] = 0.0;
		z_r_sumdiff_10_max[i] = 0.0;
		x_gr_sumdiff_mode[i] = 0.0;
		x_gr_sumdiff_maxcount[i] = 0.0;
		x_gr_sumdiff_01_min[i] = 0.0;
		x_gr_sumdiff_01_max[i] = 0.0;
		x_gr_sumdiff_10_min[i] = 0.0;
		x_gr_sumdiff_10_max[i] = 0.0;
		z_gr_sumdiff_mode[i] = 0.0;
		z_gr_sumdiff_maxcount[i] = 0.0;
		z_gr_sumdiff_01_min[i] = 0.0;
		z_gr_sumdiff_01_max[i] = 0.0;
		z_gr_sumdiff_10_min[i] = 0.0;
		z_gr_sumdiff_10_max[i] = 0.0;
	}
	
	x_all_sumdiff_mode[303] = -935.5;
	x_all_sumdiff_maxcount[303] = 1768;
	x_all_sumdiff_01_min[303] = -1191.5;
	x_all_sumdiff_01_max[303] = -841.5;
	x_all_sumdiff_10_min[303] = -1049.5;
	x_all_sumdiff_10_max[303] = -859.5;
	z_all_sumdiff_mode[303] = -333.5;
	z_all_sumdiff_maxcount[303] = 1648;
	z_all_sumdiff_01_min[303] = -595.5;
	z_all_sumdiff_01_max[303] = -235.5;
	z_all_sumdiff_10_min[303] = -473.5;
	z_all_sumdiff_10_max[303] = -251.5;
	x_pi_ac_sumdiff_mode[303] = -921.5;
	x_pi_ac_sumdiff_maxcount[303] = 700;
	x_pi_ac_sumdiff_01_min[303] = -1087.5;
	x_pi_ac_sumdiff_01_max[303] = -847.5;
	x_pi_ac_sumdiff_10_min[303] = -1011.5;
	x_pi_ac_sumdiff_10_max[303] = -869.5;
	z_pi_ac_sumdiff_mode[303] = -321.5;
	z_pi_ac_sumdiff_maxcount[303] = 633;
	z_pi_ac_sumdiff_01_min[303] = -503.5;
	z_pi_ac_sumdiff_01_max[303] = -239.5;
	z_pi_ac_sumdiff_10_min[303] = -417.5;
	z_pi_ac_sumdiff_10_max[303] = -259.5;
	x_r_sumdiff_mode[303] = -981.5;
	x_r_sumdiff_maxcount[303] = 644;
	x_r_sumdiff_01_min[303] = -1187.5;
	x_r_sumdiff_01_max[303] = -839.5;
	x_r_sumdiff_10_min[303] = -1067.5;
	x_r_sumdiff_10_max[303] = -859.5;
	z_r_sumdiff_mode[303] = -375.5;
	z_r_sumdiff_maxcount[303] = 584;
	z_r_sumdiff_01_min[303] = -619.5;
	z_r_sumdiff_01_max[303] = -235.5;
	z_r_sumdiff_10_min[303] = -485.5;
	z_r_sumdiff_10_max[303] = -247.5;
	x_gr_sumdiff_mode[303] = -1199.5;
	x_gr_sumdiff_maxcount[303] = 0;
	x_gr_sumdiff_01_min[303] = -1199.5;
	x_gr_sumdiff_01_max[303] = -1199.5;
	x_gr_sumdiff_10_min[303] = -1199.5;
	x_gr_sumdiff_10_max[303] = -1199.5;
	z_gr_sumdiff_mode[303] = -599.5;
	z_gr_sumdiff_maxcount[303] = 0;
	z_gr_sumdiff_01_min[303] = -599.5;
	z_gr_sumdiff_01_max[303] = -599.5;
	z_gr_sumdiff_10_min[303] = -599.5;
	z_gr_sumdiff_10_max[303] = -599.5;

	x_all_sumdiff_mode[308] = -933.5;
	x_all_sumdiff_maxcount[308] = 1172;
	x_all_sumdiff_01_min[308] = -1113.5;
	x_all_sumdiff_01_max[308] = -839.5;
	x_all_sumdiff_10_min[308] = -1029.5;
	x_all_sumdiff_10_max[308] = -861.5;
	z_all_sumdiff_mode[308] = -333.5;
	z_all_sumdiff_maxcount[308] = 1032;
	z_all_sumdiff_01_min[308] = -559.5;
	z_all_sumdiff_01_max[308] = -233.5;
	z_all_sumdiff_10_min[308] = -441.5;
	z_all_sumdiff_10_max[308] = -249.5;
	x_pi_ac_sumdiff_mode[308] = -919.5;
	x_pi_ac_sumdiff_maxcount[308] = 505;
	x_pi_ac_sumdiff_01_min[308] = -1049.5;
	x_pi_ac_sumdiff_01_max[308] = -847.5;
	x_pi_ac_sumdiff_10_min[308] = -1005.5;
	x_pi_ac_sumdiff_10_max[308] = -865.5;
	z_pi_ac_sumdiff_mode[308] = -319.5;
	z_pi_ac_sumdiff_maxcount[308] = 451;
	z_pi_ac_sumdiff_01_min[308] = -461.5;
	z_pi_ac_sumdiff_01_max[308] = -237.5;
	z_pi_ac_sumdiff_10_min[308] = -411.5;
	z_pi_ac_sumdiff_10_max[308] = -255.5;
	x_r_sumdiff_mode[308] = -969.5;
	x_r_sumdiff_maxcount[308] = 380;
	x_r_sumdiff_01_min[308] = -1125.5;
	x_r_sumdiff_01_max[308] = -837.5;
	x_r_sumdiff_10_min[308] = -1037.5;
	x_r_sumdiff_10_max[308] = -855.5;
	z_r_sumdiff_mode[308] = -365.5;
	z_r_sumdiff_maxcount[308] = 329;
	z_r_sumdiff_01_min[308] = -583.5;
	z_r_sumdiff_01_max[308] = -233.5;
	z_r_sumdiff_10_min[308] = -453.5;
	z_r_sumdiff_10_max[308] = -245.5;
	x_gr_sumdiff_mode[308] = -1199.5;
	x_gr_sumdiff_maxcount[308] = 0;
	x_gr_sumdiff_01_min[308] = -1199.5;
	x_gr_sumdiff_01_max[308] = -1199.5;
	x_gr_sumdiff_10_min[308] = -1199.5;
	x_gr_sumdiff_10_max[308] = -1199.5;
	z_gr_sumdiff_mode[308] = -599.5;
	z_gr_sumdiff_maxcount[308] = 0;
	z_gr_sumdiff_01_min[308] = -599.5;
	z_gr_sumdiff_01_max[308] = -599.5;
	z_gr_sumdiff_10_min[308] = -599.5;
	z_gr_sumdiff_10_max[308] = -599.5;

	x_all_sumdiff_mode[309] = -927.5;
	x_all_sumdiff_maxcount[309] = 1965;
	x_all_sumdiff_01_min[309] = -1113.5;
	x_all_sumdiff_01_max[309] = -839.5;
	x_all_sumdiff_10_min[309] = -1025.5;
	x_all_sumdiff_10_max[309] = -859.5;
	z_all_sumdiff_mode[309] = -325.5;
	z_all_sumdiff_maxcount[309] = 1749;
	z_all_sumdiff_01_min[309] = -513.5;
	z_all_sumdiff_01_max[309] = -231.5;
	z_all_sumdiff_10_min[309] = -437.5;
	z_all_sumdiff_10_max[309] = -247.5;
	x_pi_ac_sumdiff_mode[309] = -925.5;
	x_pi_ac_sumdiff_maxcount[309] = 883;
	x_pi_ac_sumdiff_01_min[309] = -1049.5;
	x_pi_ac_sumdiff_01_max[309] = -845.5;
	x_pi_ac_sumdiff_10_min[309] = -1005.5;
	x_pi_ac_sumdiff_10_max[309] = -869.5;
	z_pi_ac_sumdiff_mode[309] = -325.5;
	z_pi_ac_sumdiff_maxcount[309] = 824;
	z_pi_ac_sumdiff_01_min[309] = -469.5;
	z_pi_ac_sumdiff_01_max[309] = -237.5;
	z_pi_ac_sumdiff_10_min[309] = -403.5;
	z_pi_ac_sumdiff_10_max[309] = -259.5;
	x_r_sumdiff_mode[309] = -965.5;
	x_r_sumdiff_maxcount[309] = 530;
	x_r_sumdiff_01_min[309] = -1163.5;
	x_r_sumdiff_01_max[309] = -839.5;
	x_r_sumdiff_10_min[309] = -1037.5;
	x_r_sumdiff_10_max[309] = -853.5;
	z_r_sumdiff_mode[309] = -371.5;
	z_r_sumdiff_maxcount[309] = 512;
	z_r_sumdiff_01_min[309] = -513.5;
	z_r_sumdiff_01_max[309] = -233.5;
	z_r_sumdiff_10_min[309] = -449.5;
	z_r_sumdiff_10_max[309] = -243.5;
	x_gr_sumdiff_mode[309] = -1199.5;
	x_gr_sumdiff_maxcount[309] = 0;
	x_gr_sumdiff_01_min[309] = -1199.5;
	x_gr_sumdiff_01_max[309] = -1199.5;
	x_gr_sumdiff_10_min[309] = -1199.5;
	x_gr_sumdiff_10_max[309] = -1199.5;
	z_gr_sumdiff_mode[309] = -599.5;
	z_gr_sumdiff_maxcount[309] = 0;
	z_gr_sumdiff_01_min[309] = -599.5;
	z_gr_sumdiff_01_max[309] = -599.5;
	z_gr_sumdiff_10_min[309] = -599.5;
	z_gr_sumdiff_10_max[309] = -599.5;

	x_all_sumdiff_mode[310] = -931.5;
	x_all_sumdiff_maxcount[310] = 1319;
	x_all_sumdiff_01_min[310] = -1107.5;
	x_all_sumdiff_01_max[310] = -841.5;
	x_all_sumdiff_10_min[310] = -1025.5;
	x_all_sumdiff_10_max[310] = -859.5;
	z_all_sumdiff_mode[310] = -319.5;
	z_all_sumdiff_maxcount[310] = 1202;
	z_all_sumdiff_01_min[310] = -531.5;
	z_all_sumdiff_01_max[310] = -231.5;
	z_all_sumdiff_10_min[310] = -439.5;
	z_all_sumdiff_10_max[310] = -249.5;
	x_pi_ac_sumdiff_mode[310] = -919.5;
	x_pi_ac_sumdiff_maxcount[310] = 632;
	x_pi_ac_sumdiff_01_min[310] = -1045.5;
	x_pi_ac_sumdiff_01_max[310] = -845.5;
	x_pi_ac_sumdiff_10_min[310] = -1003.5;
	x_pi_ac_sumdiff_10_max[310] = -873.5;
	z_pi_ac_sumdiff_mode[310] = -319.5;
	z_pi_ac_sumdiff_maxcount[310] = 580;
	z_pi_ac_sumdiff_01_min[310] = -475.5;
	z_pi_ac_sumdiff_01_max[310] = -237.5;
	z_pi_ac_sumdiff_10_min[310] = -411.5;
	z_pi_ac_sumdiff_10_max[310] = -257.5;
	x_r_sumdiff_mode[310] = -973.5;
	x_r_sumdiff_maxcount[310] = 380;
	x_r_sumdiff_01_min[310] = -1129.5;
	x_r_sumdiff_01_max[310] = -841.5;
	x_r_sumdiff_10_min[310] = -1043.5;
	x_r_sumdiff_10_max[310] = -853.5;
	z_r_sumdiff_mode[310] = -373.5;
	z_r_sumdiff_maxcount[310] = 357;
	z_r_sumdiff_01_min[310] = -517.5;
	z_r_sumdiff_01_max[310] = -231.5;
	z_r_sumdiff_10_min[310] = -451.5;
	z_r_sumdiff_10_max[310] = -245.5;
	x_gr_sumdiff_mode[310] = -1199.5;
	x_gr_sumdiff_maxcount[310] = 0;
	x_gr_sumdiff_01_min[310] = -1199.5;
	x_gr_sumdiff_01_max[310] = -1199.5;
	x_gr_sumdiff_10_min[310] = -1199.5;
	x_gr_sumdiff_10_max[310] = -1199.5;
	z_gr_sumdiff_mode[310] = -599.5;
	z_gr_sumdiff_maxcount[310] = 0;
	z_gr_sumdiff_01_min[310] = -599.5;
	z_gr_sumdiff_01_max[310] = -599.5;
	z_gr_sumdiff_10_min[310] = -599.5;
	z_gr_sumdiff_10_max[310] = -599.5;

	x_all_sumdiff_mode[311] = -929.5;
	x_all_sumdiff_maxcount[311] = 4478;
	x_all_sumdiff_01_min[311] = -1111.5;
	x_all_sumdiff_01_max[311] = -839.5;
	x_all_sumdiff_10_min[311] = -1025.5;
	x_all_sumdiff_10_max[311] = -857.5;
	z_all_sumdiff_mode[311] = -315.5;
	z_all_sumdiff_maxcount[311] = 3886;
	z_all_sumdiff_01_min[311] = -559.5;
	z_all_sumdiff_01_max[311] = -229.5;
	z_all_sumdiff_10_min[311] = -441.5;
	z_all_sumdiff_10_max[311] = -247.5;
	x_pi_ac_sumdiff_mode[311] = -929.5;
	x_pi_ac_sumdiff_maxcount[311] = 1756;
	x_pi_ac_sumdiff_01_min[311] = -1043.5;
	x_pi_ac_sumdiff_01_max[311] = -845.5;
	x_pi_ac_sumdiff_10_min[311] = -1005.5;
	x_pi_ac_sumdiff_10_max[311] = -869.5;
	z_pi_ac_sumdiff_mode[311] = -319.5;
	z_pi_ac_sumdiff_maxcount[311] = 1662;
	z_pi_ac_sumdiff_01_min[311] = -485.5;
	z_pi_ac_sumdiff_01_max[311] = -235.5;
	z_pi_ac_sumdiff_10_min[311] = -413.5;
	z_pi_ac_sumdiff_10_max[311] = -255.5;
	x_r_sumdiff_mode[311] = -971.5;
	x_r_sumdiff_maxcount[311] = 1261;
	x_r_sumdiff_01_min[311] = -1137.5;
	x_r_sumdiff_01_max[311] = -835.5;
	x_r_sumdiff_10_min[311] = -1041.5;
	x_r_sumdiff_10_max[311] = -855.5;
	z_r_sumdiff_mode[311] = -363.5;
	z_r_sumdiff_maxcount[311] = 1219;
	z_r_sumdiff_01_min[311] = -579.5;
	z_r_sumdiff_01_max[311] = -225.5;
	z_r_sumdiff_10_min[311] = -455.5;
	z_r_sumdiff_10_max[311] = -243.5;
	x_gr_sumdiff_mode[311] = -1199.5;
	x_gr_sumdiff_maxcount[311] = 0;
	x_gr_sumdiff_01_min[311] = -1199.5;
	x_gr_sumdiff_01_max[311] = -1199.5;
	x_gr_sumdiff_10_min[311] = -1199.5;
	x_gr_sumdiff_10_max[311] = -1199.5;
	z_gr_sumdiff_mode[311] = -599.5;
	z_gr_sumdiff_maxcount[311] = 0;
	z_gr_sumdiff_01_min[311] = -599.5;
	z_gr_sumdiff_01_max[311] = -599.5;
	z_gr_sumdiff_10_min[311] = -599.5;
	z_gr_sumdiff_10_max[311] = -599.5;

	x_all_sumdiff_mode[312] = -925.5;
	x_all_sumdiff_maxcount[312] = 572;
	x_all_sumdiff_01_min[312] = -1103.5;
	x_all_sumdiff_01_max[312] = -839.5;
	x_all_sumdiff_10_min[312] = -1029.5;
	x_all_sumdiff_10_max[312] = -861.5;
	z_all_sumdiff_mode[312] = -321.5;
	z_all_sumdiff_maxcount[312] = 510;
	z_all_sumdiff_01_min[312] = -527.5;
	z_all_sumdiff_01_max[312] = -221.5;
	z_all_sumdiff_10_min[312] = -445.5;
	z_all_sumdiff_10_max[312] = -249.5;
	x_pi_ac_sumdiff_mode[312] = -923.5;
	x_pi_ac_sumdiff_maxcount[312] = 188;
	x_pi_ac_sumdiff_01_min[312] = -1069.5;
	x_pi_ac_sumdiff_01_max[312] = -845.5;
	x_pi_ac_sumdiff_10_min[312] = -1011.5;
	x_pi_ac_sumdiff_10_max[312] = -861.5;
	z_pi_ac_sumdiff_mode[312] = -331.5;
	z_pi_ac_sumdiff_maxcount[312] = 192;
	z_pi_ac_sumdiff_01_min[312] = -481.5;
	z_pi_ac_sumdiff_01_max[312] = -239.5;
	z_pi_ac_sumdiff_10_min[312] = -419.5;
	z_pi_ac_sumdiff_10_max[312] = -257.5;
	x_r_sumdiff_mode[312] = -921.5;
	x_r_sumdiff_maxcount[312] = 217;
	x_r_sumdiff_01_min[312] = -1073.5;
	x_r_sumdiff_01_max[312] = -839.5;
	x_r_sumdiff_10_min[312] = -1033.5;
	x_r_sumdiff_10_max[312] = -855.5;
	z_r_sumdiff_mode[312] = -291.5;
	z_r_sumdiff_maxcount[312] = 209;
	z_r_sumdiff_01_min[312] = -523.5;
	z_r_sumdiff_01_max[312] = -229.5;
	z_r_sumdiff_10_min[312] = -443.5;
	z_r_sumdiff_10_max[312] = -249.5;
	x_gr_sumdiff_mode[312] = -1199.5;
	x_gr_sumdiff_maxcount[312] = 0;
	x_gr_sumdiff_01_min[312] = -1199.5;
	x_gr_sumdiff_01_max[312] = -1199.5;
	x_gr_sumdiff_10_min[312] = -1199.5;
	x_gr_sumdiff_10_max[312] = -1199.5;
	z_gr_sumdiff_mode[312] = -599.5;
	z_gr_sumdiff_maxcount[312] = 0;
	z_gr_sumdiff_01_min[312] = -599.5;
	z_gr_sumdiff_01_max[312] = -599.5;
	z_gr_sumdiff_10_min[312] = -599.5;
	z_gr_sumdiff_10_max[312] = -599.5;

	x_all_sumdiff_mode[313] = -937.5;
	x_all_sumdiff_maxcount[313] = 2328;
	x_all_sumdiff_01_min[313] = -1305.5;
	x_all_sumdiff_01_max[313] = -839.5;
	x_all_sumdiff_10_min[313] = -1137.5;
	x_all_sumdiff_10_max[313] = -855.5;
	z_all_sumdiff_mode[313] = -359.5;
	z_all_sumdiff_maxcount[313] = 1986;
	z_all_sumdiff_01_min[313] = -715.5;
	z_all_sumdiff_01_max[313] = -227.5;
	z_all_sumdiff_10_min[313] = -557.5;
	z_all_sumdiff_10_max[313] = -245.5;
	x_pi_ac_sumdiff_mode[313] = -943.5;
	x_pi_ac_sumdiff_maxcount[313] = 719;
	x_pi_ac_sumdiff_01_min[313] = -1289.5;
	x_pi_ac_sumdiff_01_max[313] = -843.5;
	x_pi_ac_sumdiff_10_min[313] = -1109.5;
	x_pi_ac_sumdiff_10_max[313] = -863.5;
	z_pi_ac_sumdiff_mode[313] = -333.5;
	z_pi_ac_sumdiff_maxcount[313] = 639;
	z_pi_ac_sumdiff_01_min[313] = -691.5;
	z_pi_ac_sumdiff_01_max[313] = -235.5;
	z_pi_ac_sumdiff_10_min[313] = -523.5;
	z_pi_ac_sumdiff_10_max[313] = -251.5;
	x_r_sumdiff_mode[313] = -933.5;
	x_r_sumdiff_maxcount[313] = 807;
	x_r_sumdiff_01_min[313] = -1261.5;
	x_r_sumdiff_01_max[313] = -833.5;
	x_r_sumdiff_10_min[313] = -1107.5;
	x_r_sumdiff_10_max[313] = -853.5;
	z_r_sumdiff_mode[313] = -291.5;
	z_r_sumdiff_maxcount[313] = 716;
	z_r_sumdiff_01_min[313] = -671.5;
	z_r_sumdiff_01_max[313] = -221.5;
	z_r_sumdiff_10_min[313] = -529.5;
	z_r_sumdiff_10_max[313] = -241.5;
	x_gr_sumdiff_mode[313] = -1199.5;
	x_gr_sumdiff_maxcount[313] = 0;
	x_gr_sumdiff_01_min[313] = -1199.5;
	x_gr_sumdiff_01_max[313] = -1199.5;
	x_gr_sumdiff_10_min[313] = -1199.5;
	x_gr_sumdiff_10_max[313] = -1199.5;
	z_gr_sumdiff_mode[313] = -599.5;
	z_gr_sumdiff_maxcount[313] = 0;
	z_gr_sumdiff_01_min[313] = -599.5;
	z_gr_sumdiff_01_max[313] = -599.5;
	z_gr_sumdiff_10_min[313] = -599.5;
	z_gr_sumdiff_10_max[313] = -599.5;

	x_all_sumdiff_mode[318] = -975.5;
	x_all_sumdiff_maxcount[318] = 78909;
	x_all_sumdiff_01_min[318] = -1203.5;
	x_all_sumdiff_01_max[318] = -839.5;
	x_all_sumdiff_10_min[318] = -1061.5;
	x_all_sumdiff_10_max[318] = -855.5;
	z_all_sumdiff_mode[318] = -367.5;
	z_all_sumdiff_maxcount[318] = 72816;
	z_all_sumdiff_01_min[318] = -619.5;
	z_all_sumdiff_01_max[318] = -231.5;
	z_all_sumdiff_10_min[318] = -481.5;
	z_all_sumdiff_10_max[318] = -245.5;
	x_pi_ac_sumdiff_mode[318] = -969.5;
	x_pi_ac_sumdiff_maxcount[318] = 3091;
	x_pi_ac_sumdiff_01_min[318] = -1151.5;
	x_pi_ac_sumdiff_01_max[318] = -835.5;
	x_pi_ac_sumdiff_10_min[318] = -1043.5;
	x_pi_ac_sumdiff_10_max[318] = -853.5;
	z_pi_ac_sumdiff_mode[318] = -361.5;
	z_pi_ac_sumdiff_maxcount[318] = 2910;
	z_pi_ac_sumdiff_01_min[318] = -575.5;
	z_pi_ac_sumdiff_01_max[318] = -231.5;
	z_pi_ac_sumdiff_10_min[318] = -461.5;
	z_pi_ac_sumdiff_10_max[318] = -243.5;
	x_r_sumdiff_mode[318] = -971.5;
	x_r_sumdiff_maxcount[318] = 1279;
	x_r_sumdiff_01_min[318] = -1179.5;
	x_r_sumdiff_01_max[318] = -841.5;
	x_r_sumdiff_10_min[318] = -1061.5;
	x_r_sumdiff_10_max[318] = -855.5;
	z_r_sumdiff_mode[318] = -371.5;
	z_r_sumdiff_maxcount[318] = 1160;
	z_r_sumdiff_01_min[318] = -605.5;
	z_r_sumdiff_01_max[318] = -231.5;
	z_r_sumdiff_10_min[318] = -475.5;
	z_r_sumdiff_10_max[318] = -245.5;
	x_gr_sumdiff_mode[318] = -1199.5;
	x_gr_sumdiff_maxcount[318] = 0;
	x_gr_sumdiff_01_min[318] = -1199.5;
	x_gr_sumdiff_01_max[318] = -1199.5;
	x_gr_sumdiff_10_min[318] = -1199.5;
	x_gr_sumdiff_10_max[318] = -1199.5;
	z_gr_sumdiff_mode[318] = -599.5;
	z_gr_sumdiff_maxcount[318] = 0;
	z_gr_sumdiff_01_min[318] = -599.5;
	z_gr_sumdiff_01_max[318] = -599.5;
	z_gr_sumdiff_10_min[318] = -599.5;
	z_gr_sumdiff_10_max[318] = -599.5;

	x_all_sumdiff_mode[326] = -915.5;
	x_all_sumdiff_maxcount[326] = 789;
	x_all_sumdiff_01_min[326] = -1109.5;
	x_all_sumdiff_01_max[326] = -837.5;
	x_all_sumdiff_10_min[326] = -1025.5;
	x_all_sumdiff_10_max[326] = -859.5;
	z_all_sumdiff_mode[326] = -319.5;
	z_all_sumdiff_maxcount[326] = 735;
	z_all_sumdiff_01_min[326] = -531.5;
	z_all_sumdiff_01_max[326] = -233.5;
	z_all_sumdiff_10_min[326] = -435.5;
	z_all_sumdiff_10_max[326] = -247.5;
	x_pi_ac_sumdiff_mode[326] = -917.5;
	x_pi_ac_sumdiff_maxcount[326] = 347;
	x_pi_ac_sumdiff_01_min[326] = -1019.5;
	x_pi_ac_sumdiff_01_max[326] = -853.5;
	x_pi_ac_sumdiff_10_min[326] = -991.5;
	x_pi_ac_sumdiff_10_max[326] = -877.5;
	z_pi_ac_sumdiff_mode[326] = -313.5;
	z_pi_ac_sumdiff_maxcount[326] = 327;
	z_pi_ac_sumdiff_01_min[326] = -439.5;
	z_pi_ac_sumdiff_01_max[326] = -237.5;
	z_pi_ac_sumdiff_10_min[326] = -395.5;
	z_pi_ac_sumdiff_10_max[326] = -269.5;
	x_r_sumdiff_mode[326] = -971.5;
	x_r_sumdiff_maxcount[326] = 276;
	x_r_sumdiff_01_min[326] = -1127.5;
	x_r_sumdiff_01_max[326] = -837.5;
	x_r_sumdiff_10_min[326] = -1037.5;
	x_r_sumdiff_10_max[326] = -855.5;
	z_r_sumdiff_mode[326] = -363.5;
	z_r_sumdiff_maxcount[326] = 252;
	z_r_sumdiff_01_min[326] = -575.5;
	z_r_sumdiff_01_max[326] = -229.5;
	z_r_sumdiff_10_min[326] = -457.5;
	z_r_sumdiff_10_max[326] = -243.5;
	x_gr_sumdiff_mode[326] = -1199.5;
	x_gr_sumdiff_maxcount[326] = 0;
	x_gr_sumdiff_01_min[326] = -1199.5;
	x_gr_sumdiff_01_max[326] = -1199.5;
	x_gr_sumdiff_10_min[326] = -1199.5;
	x_gr_sumdiff_10_max[326] = -1199.5;
	z_gr_sumdiff_mode[326] = -599.5;
	z_gr_sumdiff_maxcount[326] = 0;
	z_gr_sumdiff_01_min[326] = -599.5;
	z_gr_sumdiff_01_max[326] = -599.5;
	z_gr_sumdiff_10_min[326] = -599.5;
	z_gr_sumdiff_10_max[326] = -599.5;

	x_all_sumdiff_mode[327] = -1055.5;
	x_all_sumdiff_maxcount[327] = 3820;
	x_all_sumdiff_01_min[327] = -1267.5;
	x_all_sumdiff_01_max[327] = -939.5;
	x_all_sumdiff_10_min[327] = -1207.5;
	x_all_sumdiff_10_max[327] = -971.5;
	z_all_sumdiff_mode[327] = -467.5;
	z_all_sumdiff_maxcount[327] = 4479;
	z_all_sumdiff_01_min[327] = -669.5;
	z_all_sumdiff_01_max[327] = -351.5;
	z_all_sumdiff_10_min[327] = -589.5;
	z_all_sumdiff_10_max[327] = -385.5;
	x_pi_ac_sumdiff_mode[327] = -1051.5;
	x_pi_ac_sumdiff_maxcount[327] = 1516;
	x_pi_ac_sumdiff_01_min[327] = -1231.5;
	x_pi_ac_sumdiff_01_max[327] = -945.5;
	x_pi_ac_sumdiff_10_min[327] = -1145.5;
	x_pi_ac_sumdiff_10_max[327] = -973.5;
	z_pi_ac_sumdiff_mode[327] = -467.5;
	z_pi_ac_sumdiff_maxcount[327] = 1934;
	z_pi_ac_sumdiff_01_min[327] = -621.5;
	z_pi_ac_sumdiff_01_max[327] = -365.5;
	z_pi_ac_sumdiff_10_min[327] = -553.5;
	z_pi_ac_sumdiff_10_max[327] = -393.5;
	x_r_sumdiff_mode[327] = -1075.5;
	x_r_sumdiff_maxcount[327] = 1215;
	x_r_sumdiff_01_min[327] = -1283.5;
	x_r_sumdiff_01_max[327] = -939.5;
	x_r_sumdiff_10_min[327] = -1229.5;
	x_r_sumdiff_10_max[327] = -969.5;
	z_r_sumdiff_mode[327] = -503.5;
	z_r_sumdiff_maxcount[327] = 1259;
	z_r_sumdiff_01_min[327] = -679.5;
	z_r_sumdiff_01_max[327] = -345.5;
	z_r_sumdiff_10_min[327] = -603.5;
	z_r_sumdiff_10_max[327] = -375.5;
	x_gr_sumdiff_mode[327] = -1199.5;
	x_gr_sumdiff_maxcount[327] = 0;
	x_gr_sumdiff_01_min[327] = -1199.5;
	x_gr_sumdiff_01_max[327] = -1199.5;
	x_gr_sumdiff_10_min[327] = -1199.5;
	x_gr_sumdiff_10_max[327] = -1199.5;
	z_gr_sumdiff_mode[327] = -599.5;
	z_gr_sumdiff_maxcount[327] = 0;
	z_gr_sumdiff_01_min[327] = -599.5;
	z_gr_sumdiff_01_max[327] = -599.5;
	z_gr_sumdiff_10_min[327] = -599.5;
	z_gr_sumdiff_10_max[327] = -599.5;

	x_all_sumdiff_mode[328] = -1055.5;
	x_all_sumdiff_maxcount[328] = 47582;
	x_all_sumdiff_01_min[328] = -1277.5;
	x_all_sumdiff_01_max[328] = -939.5;
	x_all_sumdiff_10_min[328] = -1217.5;
	x_all_sumdiff_10_max[328] = -967.5;
	z_all_sumdiff_mode[328] = -449.5;
	z_all_sumdiff_maxcount[328] = 45213;
	z_all_sumdiff_01_min[328] = -683.5;
	z_all_sumdiff_01_max[328] = -345.5;
	z_all_sumdiff_10_min[328] = -597.5;
	z_all_sumdiff_10_max[328] = -371.5;
	x_pi_ac_sumdiff_mode[328] = -1049.5;
	x_pi_ac_sumdiff_maxcount[328] = 4709;
	x_pi_ac_sumdiff_01_min[328] = -1251.5;
	x_pi_ac_sumdiff_01_max[328] = -939.5;
	x_pi_ac_sumdiff_10_min[328] = -1185.5;
	x_pi_ac_sumdiff_10_max[328] = -969.5;
	z_pi_ac_sumdiff_mode[328] = -459.5;
	z_pi_ac_sumdiff_maxcount[328] = 5712;
	z_pi_ac_sumdiff_01_min[328] = -635.5;
	z_pi_ac_sumdiff_01_max[328] = -351.5;
	z_pi_ac_sumdiff_10_min[328] = -575.5;
	z_pi_ac_sumdiff_10_max[328] = -381.5;
	x_r_sumdiff_mode[328] = -1055.5;
	x_r_sumdiff_maxcount[328] = 3522;
	x_r_sumdiff_01_min[328] = -1283.5;
	x_r_sumdiff_01_max[328] = -935.5;
	x_r_sumdiff_10_min[328] = -1223.5;
	x_r_sumdiff_10_max[328] = -967.5;
	z_r_sumdiff_mode[328] = -503.5;
	z_r_sumdiff_maxcount[328] = 3411;
	z_r_sumdiff_01_min[328] = -679.5;
	z_r_sumdiff_01_max[328] = -349.5;
	z_r_sumdiff_10_min[328] = -601.5;
	z_r_sumdiff_10_max[328] = -369.5;
	x_gr_sumdiff_mode[328] = -1199.5;
	x_gr_sumdiff_maxcount[328] = 0;
	x_gr_sumdiff_01_min[328] = -1199.5;
	x_gr_sumdiff_01_max[328] = -1199.5;
	x_gr_sumdiff_10_min[328] = -1199.5;
	x_gr_sumdiff_10_max[328] = -1199.5;
	z_gr_sumdiff_mode[328] = -599.5;
	z_gr_sumdiff_maxcount[328] = 0;
	z_gr_sumdiff_01_min[328] = -599.5;
	z_gr_sumdiff_01_max[328] = -599.5;
	z_gr_sumdiff_10_min[328] = -599.5;
	z_gr_sumdiff_10_max[328] = -599.5;

	x_all_sumdiff_mode[340] = -1025.5;
	x_all_sumdiff_maxcount[340] = 25901;
	x_all_sumdiff_01_min[340] = -1241.5;
	x_all_sumdiff_01_max[340] = -917.5;
	x_all_sumdiff_10_min[340] = -1171.5;
	x_all_sumdiff_10_max[340] = -957.5;
	z_all_sumdiff_mode[340] = -447.5;
	z_all_sumdiff_maxcount[340] = 21069;
	z_all_sumdiff_01_min[340] = -697.5;
	z_all_sumdiff_01_max[340] = -337.5;
	z_all_sumdiff_10_min[340] = -581.5;
	z_all_sumdiff_10_max[340] = -363.5;
	x_pi_ac_sumdiff_mode[340] = -1025.5;
	x_pi_ac_sumdiff_maxcount[340] = 1878;
	x_pi_ac_sumdiff_01_min[340] = -1219.5;
	x_pi_ac_sumdiff_01_max[340] = -911.5;
	x_pi_ac_sumdiff_10_min[340] = -1161.5;
	x_pi_ac_sumdiff_10_max[340] = -955.5;
	z_pi_ac_sumdiff_mode[340] = -437.5;
	z_pi_ac_sumdiff_maxcount[340] = 1702;
	z_pi_ac_sumdiff_01_min[340] = -677.5;
	z_pi_ac_sumdiff_01_max[340] = -331.5;
	z_pi_ac_sumdiff_10_min[340] = -573.5;
	z_pi_ac_sumdiff_10_max[340] = -363.5;
	x_r_sumdiff_mode[340] = -1025.5;
	x_r_sumdiff_maxcount[340] = 2925;
	x_r_sumdiff_01_min[340] = -1241.5;
	x_r_sumdiff_01_max[340] = -917.5;
	x_r_sumdiff_10_min[340] = -1177.5;
	x_r_sumdiff_10_max[340] = -955.5;
	z_r_sumdiff_mode[340] = -431.5;
	z_r_sumdiff_maxcount[340] = 2394;
	z_r_sumdiff_01_min[340] = -687.5;
	z_r_sumdiff_01_max[340] = -329.5;
	z_r_sumdiff_10_min[340] = -579.5;
	z_r_sumdiff_10_max[340] = -353.5;
	x_gr_sumdiff_mode[340] = -1199.5;
	x_gr_sumdiff_maxcount[340] = 0;
	x_gr_sumdiff_01_min[340] = -1199.5;
	x_gr_sumdiff_01_max[340] = -1199.5;
	x_gr_sumdiff_10_min[340] = -1199.5;
	x_gr_sumdiff_10_max[340] = -1199.5;
	z_gr_sumdiff_mode[340] = -599.5;
	z_gr_sumdiff_maxcount[340] = 0;
	z_gr_sumdiff_01_min[340] = -599.5;
	z_gr_sumdiff_01_max[340] = -599.5;
	z_gr_sumdiff_10_min[340] = -599.5;
	z_gr_sumdiff_10_max[340] = -599.5;

	x_all_sumdiff_mode[342] = -1085.5;
	x_all_sumdiff_maxcount[342] = 12086;
	x_all_sumdiff_01_min[342] = -1291.5;
	x_all_sumdiff_01_max[342] = -965.5;
	x_all_sumdiff_10_min[342] = -1229.5;
	x_all_sumdiff_10_max[342] = -1001.5;
	z_all_sumdiff_mode[342] = -475.5;
	z_all_sumdiff_maxcount[342] = 12740;
	z_all_sumdiff_01_min[342] = -669.5;
	z_all_sumdiff_01_max[342] = -349.5;
	z_all_sumdiff_10_min[342] = -607.5;
	z_all_sumdiff_10_max[342] = -403.5;
	x_pi_ac_sumdiff_mode[342] = -1089.5;
	x_pi_ac_sumdiff_maxcount[342] = 1741;
	x_pi_ac_sumdiff_01_min[342] = -1259.5;
	x_pi_ac_sumdiff_01_max[342] = -967.5;
	x_pi_ac_sumdiff_10_min[342] = -1169.5;
	x_pi_ac_sumdiff_10_max[342] = -1007.5;
	z_pi_ac_sumdiff_mode[342] = -475.5;
	z_pi_ac_sumdiff_maxcount[342] = 2284;
	z_pi_ac_sumdiff_01_min[342] = -623.5;
	z_pi_ac_sumdiff_01_max[342] = -365.5;
	z_pi_ac_sumdiff_10_min[342] = -571.5;
	z_pi_ac_sumdiff_10_max[342] = -421.5;
	x_r_sumdiff_mode[342] = -1077.5;
	x_r_sumdiff_maxcount[342] = 318;
	x_r_sumdiff_01_min[342] = -1277.5;
	x_r_sumdiff_01_max[342] = -971.5;
	x_r_sumdiff_10_min[342] = -1231.5;
	x_r_sumdiff_10_max[342] = -997.5;
	z_r_sumdiff_mode[342] = -491.5;
	z_r_sumdiff_maxcount[342] = 315;
	z_r_sumdiff_01_min[342] = -663.5;
	z_r_sumdiff_01_max[342] = -365.5;
	z_r_sumdiff_10_min[342] = -607.5;
	z_r_sumdiff_10_max[342] = -403.5;
	x_gr_sumdiff_mode[342] = -1095.5;
	x_gr_sumdiff_maxcount[342] = 29;
	x_gr_sumdiff_01_min[342] = -1229.5;
	x_gr_sumdiff_01_max[342] = -1003.5;
	x_gr_sumdiff_10_min[342] = -1215.5;
	x_gr_sumdiff_10_max[342] = -1011.5;
	z_gr_sumdiff_mode[342] = -473.5;
	z_gr_sumdiff_maxcount[342] = 29;
	z_gr_sumdiff_01_min[342] = -623.5;
	z_gr_sumdiff_01_max[342] = -389.5;
	z_gr_sumdiff_10_min[342] = -593.5;
	z_gr_sumdiff_10_max[342] = -417.5;

	x_all_sumdiff_mode[343] = -1081.5;
	x_all_sumdiff_maxcount[343] = 17209;
	x_all_sumdiff_01_min[343] = -1281.5;
	x_all_sumdiff_01_max[343] = -961.5;
	x_all_sumdiff_10_min[343] = -1219.5;
	x_all_sumdiff_10_max[343] = -1001.5;
	z_all_sumdiff_mode[343] = -483.5;
	z_all_sumdiff_maxcount[343] = 18245;
	z_all_sumdiff_01_min[343] = -667.5;
	z_all_sumdiff_01_max[343] = -339.5;
	z_all_sumdiff_10_min[343] = -601.5;
	z_all_sumdiff_10_max[343] = -401.5;
	x_pi_ac_sumdiff_mode[343] = -1101.5;
	x_pi_ac_sumdiff_maxcount[343] = 2450;
	x_pi_ac_sumdiff_01_min[343] = -1247.5;
	x_pi_ac_sumdiff_01_max[343] = -969.5;
	x_pi_ac_sumdiff_10_min[343] = -1169.5;
	x_pi_ac_sumdiff_10_max[343] = -1007.5;
	z_pi_ac_sumdiff_mode[343] = -485.5;
	z_pi_ac_sumdiff_maxcount[343] = 3245;
	z_pi_ac_sumdiff_01_min[343] = -617.5;
	z_pi_ac_sumdiff_01_max[343] = -353.5;
	z_pi_ac_sumdiff_10_min[343] = -569.5;
	z_pi_ac_sumdiff_10_max[343] = -423.5;
	x_r_sumdiff_mode[343] = -1081.5;
	x_r_sumdiff_maxcount[343] = 441;
	x_r_sumdiff_01_min[343] = -1271.5;
	x_r_sumdiff_01_max[343] = -967.5;
	x_r_sumdiff_10_min[343] = -1233.5;
	x_r_sumdiff_10_max[343] = -999.5;
	z_r_sumdiff_mode[343] = -495.5;
	z_r_sumdiff_maxcount[343] = 438;
	z_r_sumdiff_01_min[343] = -651.5;
	z_r_sumdiff_01_max[343] = -339.5;
	z_r_sumdiff_10_min[343] = -601.5;
	z_r_sumdiff_10_max[343] = -401.5;
	x_gr_sumdiff_mode[343] = -1107.5;
	x_gr_sumdiff_maxcount[343] = 42;
	x_gr_sumdiff_01_min[343] = -1221.5;
	x_gr_sumdiff_01_max[343] = -989.5;
	x_gr_sumdiff_10_min[343] = -1201.5;
	x_gr_sumdiff_10_max[343] = -1005.5;
	z_gr_sumdiff_mode[343] = -495.5;
	z_gr_sumdiff_maxcount[343] = 43;
	z_gr_sumdiff_01_min[343] = -597.5;
	z_gr_sumdiff_01_max[343] = -409.5;
	z_gr_sumdiff_10_min[343] = -597.5;
	z_gr_sumdiff_10_max[343] = -413.5;

	x_all_sumdiff_mode[361] = -1077.5;
	x_all_sumdiff_maxcount[361] = 16143;
	x_all_sumdiff_01_min[361] = -1281.5;
	x_all_sumdiff_01_max[361] = -979.5;
	x_all_sumdiff_10_min[361] = -1219.5;
	x_all_sumdiff_10_max[361] = -1007.5;
	z_all_sumdiff_mode[361] = -477.5;
	z_all_sumdiff_maxcount[361] = 16911;
	z_all_sumdiff_01_min[361] = -655.5;
	z_all_sumdiff_01_max[361] = -391.5;
	z_all_sumdiff_10_min[361] = -605.5;
	z_all_sumdiff_10_max[361] = -415.5;
	x_pi_ac_sumdiff_mode[361] = -1083.5;
	x_pi_ac_sumdiff_maxcount[361] = 1372;
	x_pi_ac_sumdiff_01_min[361] = -1261.5;
	x_pi_ac_sumdiff_01_max[361] = -981.5;
	x_pi_ac_sumdiff_10_min[361] = -1193.5;
	x_pi_ac_sumdiff_10_max[361] = -1009.5;
	z_pi_ac_sumdiff_mode[361] = -485.5;
	z_pi_ac_sumdiff_maxcount[361] = 2067;
	z_pi_ac_sumdiff_01_min[361] = -623.5;
	z_pi_ac_sumdiff_01_max[361] = -401.5;
	z_pi_ac_sumdiff_10_min[361] = -581.5;
	z_pi_ac_sumdiff_10_max[361] = -431.5;
	x_r_sumdiff_mode[361] = -1085.5;
	x_r_sumdiff_maxcount[361] = 347;
	x_r_sumdiff_01_min[361] = -1277.5;
	x_r_sumdiff_01_max[361] = -985.5;
	x_r_sumdiff_10_min[361] = -1223.5;
	x_r_sumdiff_10_max[361] = -1005.5;
	z_r_sumdiff_mode[361] = -481.5;
	z_r_sumdiff_maxcount[361] = 348;
	z_r_sumdiff_01_min[361] = -633.5;
	z_r_sumdiff_01_max[361] = -393.5;
	z_r_sumdiff_10_min[361] = -601.5;
	z_r_sumdiff_10_max[361] = -413.5;
	x_gr_sumdiff_mode[361] = -1083.5;
	x_gr_sumdiff_maxcount[361] = 60;
	x_gr_sumdiff_01_min[361] = -1265.5;
	x_gr_sumdiff_01_max[361] = -985.5;
	x_gr_sumdiff_10_min[361] = -1193.5;
	x_gr_sumdiff_10_max[361] = -1019.5;
	z_gr_sumdiff_mode[361] = -493.5;
	z_gr_sumdiff_maxcount[361] = 67;
	z_gr_sumdiff_01_min[361] = -627.5;
	z_gr_sumdiff_01_max[361] = -395.5;
	z_gr_sumdiff_10_min[361] = -595.5;
	z_gr_sumdiff_10_max[361] = -421.5;

	x_all_sumdiff_mode[368] = -1079.5;
	x_all_sumdiff_maxcount[368] = 26692;
	x_all_sumdiff_01_min[368] = -1275.5;
	x_all_sumdiff_01_max[368] = -995.5;
	x_all_sumdiff_10_min[368] = -1209.5;
	x_all_sumdiff_10_max[368] = -1021.5;
	z_all_sumdiff_mode[368] = -485.5;
	z_all_sumdiff_maxcount[368] = 25334;
	z_all_sumdiff_01_min[368] = -683.5;
	z_all_sumdiff_01_max[368] = -399.5;
	z_all_sumdiff_10_min[368] = -605.5;
	z_all_sumdiff_10_max[368] = -421.5;
	x_pi_ac_sumdiff_mode[368] = -1075.5;
	x_pi_ac_sumdiff_maxcount[368] = 3185;
	x_pi_ac_sumdiff_01_min[368] = -1251.5;
	x_pi_ac_sumdiff_01_max[368] = -999.5;
	x_pi_ac_sumdiff_10_min[368] = -1173.5;
	x_pi_ac_sumdiff_10_max[368] = -1031.5;
	z_pi_ac_sumdiff_mode[368] = -485.5;
	z_pi_ac_sumdiff_maxcount[368] = 3664;
	z_pi_ac_sumdiff_01_min[368] = -635.5;
	z_pi_ac_sumdiff_01_max[368] = -411.5;
	z_pi_ac_sumdiff_10_min[368] = -573.5;
	z_pi_ac_sumdiff_10_max[368] = -435.5;
	x_r_sumdiff_mode[368] = -1077.5;
	x_r_sumdiff_maxcount[368] = 547;
	x_r_sumdiff_01_min[368] = -1277.5;
	x_r_sumdiff_01_max[368] = -993.5;
	x_r_sumdiff_10_min[368] = -1213.5;
	x_r_sumdiff_10_max[368] = -1021.5;
	z_r_sumdiff_mode[368] = -485.5;
	z_r_sumdiff_maxcount[368] = 531;
	z_r_sumdiff_01_min[368] = -665.5;
	z_r_sumdiff_01_max[368] = -401.5;
	z_r_sumdiff_10_min[368] = -607.5;
	z_r_sumdiff_10_max[368] = -419.5;
	x_gr_sumdiff_mode[368] = -1073.5;
	x_gr_sumdiff_maxcount[368] = 105;
	x_gr_sumdiff_01_min[368] = -1245.5;
	x_gr_sumdiff_01_max[368] = -1001.5;
	x_gr_sumdiff_10_min[368] = -1189.5;
	x_gr_sumdiff_10_max[368] = -1021.5;
	z_gr_sumdiff_mode[368] = -497.5;
	z_gr_sumdiff_maxcount[368] = 96;
	z_gr_sumdiff_01_min[368] = -621.5;
	z_gr_sumdiff_01_max[368] = -399.5;
	z_gr_sumdiff_10_min[368] = -605.5;
	z_gr_sumdiff_10_max[368] = -423.5;

	x_all_sumdiff_mode[370] = -901.5;
	x_all_sumdiff_maxcount[370] = 7538;
	x_all_sumdiff_01_min[370] = -1109.5;
	x_all_sumdiff_01_max[370] = -799.5;
	x_all_sumdiff_10_min[370] = -1041.5;
	x_all_sumdiff_10_max[370] = -823.5;
	z_all_sumdiff_mode[370] = -285.5;
	z_all_sumdiff_maxcount[370] = 7992;
	z_all_sumdiff_01_min[370] = -513.5;
	z_all_sumdiff_01_max[370] = -191.5;
	z_all_sumdiff_10_min[370] = -431.5;
	z_all_sumdiff_10_max[370] = -213.5;
	x_pi_ac_sumdiff_mode[370] = -903.5;
	x_pi_ac_sumdiff_maxcount[370] = 838;
	x_pi_ac_sumdiff_01_min[370] = -1101.5;
	x_pi_ac_sumdiff_01_max[370] = -803.5;
	x_pi_ac_sumdiff_10_min[370] = -1039.5;
	x_pi_ac_sumdiff_10_max[370] = -835.5;
	z_pi_ac_sumdiff_mode[370] = -281.5;
	z_pi_ac_sumdiff_maxcount[370] = 799;
	z_pi_ac_sumdiff_01_min[370] = -509.5;
	z_pi_ac_sumdiff_01_max[370] = -203.5;
	z_pi_ac_sumdiff_10_min[370] = -445.5;
	z_pi_ac_sumdiff_10_max[370] = -227.5;
	x_r_sumdiff_mode[370] = -919.5;
	x_r_sumdiff_maxcount[370] = 194;
	x_r_sumdiff_01_min[370] = -1093.5;
	x_r_sumdiff_01_max[370] = -801.5;
	x_r_sumdiff_10_min[370] = -1031.5;
	x_r_sumdiff_10_max[370] = -823.5;
	z_r_sumdiff_mode[370] = -283.5;
	z_r_sumdiff_maxcount[370] = 206;
	z_r_sumdiff_01_min[370] = -477.5;
	z_r_sumdiff_01_max[370] = -197.5;
	z_r_sumdiff_10_min[370] = -411.5;
	z_r_sumdiff_10_max[370] = -215.5;
	x_gr_sumdiff_mode[370] = -907.5;
	x_gr_sumdiff_maxcount[370] = 40;
	x_gr_sumdiff_01_min[370] = -1059.5;
	x_gr_sumdiff_01_max[370] = -815.5;
	x_gr_sumdiff_10_min[370] = -1009.5;
	x_gr_sumdiff_10_max[370] = -829.5;
	z_gr_sumdiff_mode[370] = -291.5;
	z_gr_sumdiff_maxcount[370] = 49;
	z_gr_sumdiff_01_min[370] = -455.5;
	z_gr_sumdiff_01_max[370] = -199.5;
	z_gr_sumdiff_10_min[370] = -383.5;
	z_gr_sumdiff_10_max[370] = -223.5;

	x_all_sumdiff_mode[371] = -899.5;
	x_all_sumdiff_maxcount[371] = 15113;
	x_all_sumdiff_01_min[371] = -1111.5;
	x_all_sumdiff_01_max[371] = -801.5;
	x_all_sumdiff_10_min[371] = -1043.5;
	x_all_sumdiff_10_max[371] = -823.5;
	z_all_sumdiff_mode[371] = -281.5;
	z_all_sumdiff_maxcount[371] = 16288;
	z_all_sumdiff_01_min[371] = -511.5;
	z_all_sumdiff_01_max[371] = -191.5;
	z_all_sumdiff_10_min[371] = -435.5;
	z_all_sumdiff_10_max[371] = -213.5;
	x_pi_ac_sumdiff_mode[371] = -899.5;
	x_pi_ac_sumdiff_maxcount[371] = 1509;
	x_pi_ac_sumdiff_01_min[371] = -1107.5;
	x_pi_ac_sumdiff_01_max[371] = -807.5;
	x_pi_ac_sumdiff_10_min[371] = -1041.5;
	x_pi_ac_sumdiff_10_max[371] = -831.5;
	z_pi_ac_sumdiff_mode[371] = -281.5;
	z_pi_ac_sumdiff_maxcount[371] = 1424;
	z_pi_ac_sumdiff_01_min[371] = -503.5;
	z_pi_ac_sumdiff_01_max[371] = -197.5;
	z_pi_ac_sumdiff_10_min[371] = -439.5;
	z_pi_ac_sumdiff_10_max[371] = -223.5;
	x_r_sumdiff_mode[371] = -905.5;
	x_r_sumdiff_maxcount[371] = 346;
	x_r_sumdiff_01_min[371] = -1099.5;
	x_r_sumdiff_01_max[371] = -801.5;
	x_r_sumdiff_10_min[371] = -1045.5;
	x_r_sumdiff_10_max[371] = -825.5;
	z_r_sumdiff_mode[371] = -287.5;
	z_r_sumdiff_maxcount[371] = 383;
	z_r_sumdiff_01_min[371] = -509.5;
	z_r_sumdiff_01_max[371] = -189.5;
	z_r_sumdiff_10_min[371] = -423.5;
	z_r_sumdiff_10_max[371] = -213.5;
	x_gr_sumdiff_mode[371] = -933.5;
	x_gr_sumdiff_maxcount[371] = 70;
	x_gr_sumdiff_01_min[371] = -1095.5;
	x_gr_sumdiff_01_max[371] = -801.5;
	x_gr_sumdiff_10_min[371] = -1011.5;
	x_gr_sumdiff_10_max[371] = -829.5;
	z_gr_sumdiff_mode[371] = -301.5;
	z_gr_sumdiff_maxcount[371] = 71;
	z_gr_sumdiff_01_min[371] = -457.5;
	z_gr_sumdiff_01_max[371] = -201.5;
	z_gr_sumdiff_10_min[371] = -387.5;
	z_gr_sumdiff_10_max[371] = -223.5;

	x_all_sumdiff_mode[376] = -909.5;
	x_all_sumdiff_maxcount[376] = 735;
	x_all_sumdiff_01_min[376] = -1079.5;
	x_all_sumdiff_01_max[376] = -817.5;
	x_all_sumdiff_10_min[376] = -1035.5;
	x_all_sumdiff_10_max[376] = -841.5;
	z_all_sumdiff_mode[376] = -275.5;
	z_all_sumdiff_maxcount[376] = 658;
	z_all_sumdiff_01_min[376] = -505.5;
	z_all_sumdiff_01_max[376] = -209.5;
	z_all_sumdiff_10_min[376] = -439.5;
	z_all_sumdiff_10_max[376] = -231.5;
	x_pi_ac_sumdiff_mode[376] = -897.5;
	x_pi_ac_sumdiff_maxcount[376] = 412;
	x_pi_ac_sumdiff_01_min[376] = -1079.5;
	x_pi_ac_sumdiff_01_max[376] = -833.5;
	x_pi_ac_sumdiff_10_min[376] = -1021.5;
	x_pi_ac_sumdiff_10_max[376] = -855.5;
	z_pi_ac_sumdiff_mode[376] = -285.5;
	z_pi_ac_sumdiff_maxcount[376] = 344;
	z_pi_ac_sumdiff_01_min[376] = -493.5;
	z_pi_ac_sumdiff_01_max[376] = -223.5;
	z_pi_ac_sumdiff_10_min[376] = -441.5;
	z_pi_ac_sumdiff_10_max[376] = -239.5;
	x_r_sumdiff_mode[376] = -903.5;
	x_r_sumdiff_maxcount[376] = 82;
	x_r_sumdiff_01_min[376] = -1079.5;
	x_r_sumdiff_01_max[376] = -811.5;
	x_r_sumdiff_10_min[376] = -1029.5;
	x_r_sumdiff_10_max[376] = -833.5;
	z_r_sumdiff_mode[376] = -279.5;
	z_r_sumdiff_maxcount[376] = 98;
	z_r_sumdiff_01_min[376] = -465.5;
	z_r_sumdiff_01_max[376] = -201.5;
	z_r_sumdiff_10_min[376] = -409.5;
	z_r_sumdiff_10_max[376] = -219.5;
	x_gr_sumdiff_mode[376] = -935.5;
	x_gr_sumdiff_maxcount[376] = 17;
	x_gr_sumdiff_01_min[376] = -1019.5;
	x_gr_sumdiff_01_max[376] = -833.5;
	x_gr_sumdiff_10_min[376] = -997.5;
	x_gr_sumdiff_10_max[376] = -845.5;
	z_gr_sumdiff_mode[376] = -283.5;
	z_gr_sumdiff_maxcount[376] = 22;
	z_gr_sumdiff_01_min[376] = -415.5;
	z_gr_sumdiff_01_max[376] = -219.5;
	z_gr_sumdiff_10_min[376] = -387.5;
	z_gr_sumdiff_10_max[376] = -247.5;

	x_all_sumdiff_mode[377] = -909.5;
	x_all_sumdiff_maxcount[377] = 995;
	x_all_sumdiff_01_min[377] = -1095.5;
	x_all_sumdiff_01_max[377] = -821.5;
	x_all_sumdiff_10_min[377] = -1031.5;
	x_all_sumdiff_10_max[377] = -845.5;
	z_all_sumdiff_mode[377] = -285.5;
	z_all_sumdiff_maxcount[377] = 894;
	z_all_sumdiff_01_min[377] = -511.5;
	z_all_sumdiff_01_max[377] = -211.5;
	z_all_sumdiff_10_min[377] = -445.5;
	z_all_sumdiff_10_max[377] = -233.5;
	x_pi_ac_sumdiff_mode[377] = -911.5;
	x_pi_ac_sumdiff_maxcount[377] = 565;
	x_pi_ac_sumdiff_01_min[377] = -1091.5;
	x_pi_ac_sumdiff_01_max[377] = -821.5;
	x_pi_ac_sumdiff_10_min[377] = -1031.5;
	x_pi_ac_sumdiff_10_max[377] = -861.5;
	z_pi_ac_sumdiff_mode[377] = -285.5;
	z_pi_ac_sumdiff_maxcount[377] = 475;
	z_pi_ac_sumdiff_01_min[377] = -495.5;
	z_pi_ac_sumdiff_01_max[377] = -217.5;
	z_pi_ac_sumdiff_10_min[377] = -443.5;
	z_pi_ac_sumdiff_10_max[377] = -243.5;
	x_r_sumdiff_mode[377] = -909.5;
	x_r_sumdiff_maxcount[377] = 102;
	x_r_sumdiff_01_min[377] = -1067.5;
	x_r_sumdiff_01_max[377] = -821.5;
	x_r_sumdiff_10_min[377] = -1031.5;
	x_r_sumdiff_10_max[377] = -827.5;
	z_r_sumdiff_mode[377] = -289.5;
	z_r_sumdiff_maxcount[377] = 111;
	z_r_sumdiff_01_min[377] = -451.5;
	z_r_sumdiff_01_max[377] = -209.5;
	z_r_sumdiff_10_min[377] = -407.5;
	z_r_sumdiff_10_max[377] = -219.5;
	x_gr_sumdiff_mode[377] = -933.5;
	x_gr_sumdiff_maxcount[377] = 23;
	x_gr_sumdiff_01_min[377] = -1015.5;
	x_gr_sumdiff_01_max[377] = -833.5;
	x_gr_sumdiff_10_min[377] = -1007.5;
	x_gr_sumdiff_10_max[377] = -833.5;
	z_gr_sumdiff_mode[377] = -301.5;
	z_gr_sumdiff_maxcount[377] = 25;
	z_gr_sumdiff_01_min[377] = -415.5;
	z_gr_sumdiff_01_max[377] = -211.5;
	z_gr_sumdiff_10_min[377] = -385.5;
	z_gr_sumdiff_10_max[377] = -225.5;

	x_all_sumdiff_mode[378] = -903.5;
	x_all_sumdiff_maxcount[378] = 1103;
	x_all_sumdiff_01_min[378] = -1097.5;
	x_all_sumdiff_01_max[378] = -821.5;
	x_all_sumdiff_10_min[378] = -1037.5;
	x_all_sumdiff_10_max[378] = -843.5;
	z_all_sumdiff_mode[378] = -281.5;
	z_all_sumdiff_maxcount[378] = 954;
	z_all_sumdiff_01_min[378] = -499.5;
	z_all_sumdiff_01_max[378] = -213.5;
	z_all_sumdiff_10_min[378] = -445.5;
	z_all_sumdiff_10_max[378] = -235.5;
	x_pi_ac_sumdiff_mode[378] = -903.5;
	x_pi_ac_sumdiff_maxcount[378] = 636;
	x_pi_ac_sumdiff_01_min[378] = -1081.5;
	x_pi_ac_sumdiff_01_max[378] = -831.5;
	x_pi_ac_sumdiff_10_min[378] = -1029.5;
	x_pi_ac_sumdiff_10_max[378] = -863.5;
	z_pi_ac_sumdiff_mode[378] = -277.5;
	z_pi_ac_sumdiff_maxcount[378] = 527;
	z_pi_ac_sumdiff_01_min[378] = -499.5;
	z_pi_ac_sumdiff_01_max[378] = -225.5;
	z_pi_ac_sumdiff_10_min[378] = -441.5;
	z_pi_ac_sumdiff_10_max[378] = -243.5;
	x_r_sumdiff_mode[378] = -899.5;
	x_r_sumdiff_maxcount[378] = 104;
	x_r_sumdiff_01_min[378] = -1087.5;
	x_r_sumdiff_01_max[378] = -821.5;
	x_r_sumdiff_10_min[378] = -1037.5;
	x_r_sumdiff_10_max[378] = -831.5;
	z_r_sumdiff_mode[378] = -271.5;
	z_r_sumdiff_maxcount[378] = 110;
	z_r_sumdiff_01_min[378] = -489.5;
	z_r_sumdiff_01_max[378] = -213.5;
	z_r_sumdiff_10_min[378] = -405.5;
	z_r_sumdiff_10_max[378] = -223.5;
	x_gr_sumdiff_mode[378] = -897.5;
	x_gr_sumdiff_maxcount[378] = 23;
	x_gr_sumdiff_01_min[378] = -1027.5;
	x_gr_sumdiff_01_max[378] = -831.5;
	x_gr_sumdiff_10_min[378] = -993.5;
	x_gr_sumdiff_10_max[378] = -835.5;
	z_gr_sumdiff_mode[378] = -341.5;
	z_gr_sumdiff_maxcount[378] = 24;
	z_gr_sumdiff_01_min[378] = -419.5;
	z_gr_sumdiff_01_max[378] = -215.5;
	z_gr_sumdiff_10_min[378] = -377.5;
	z_gr_sumdiff_10_max[378] = -227.5;

	x_all_sumdiff_mode[394] = -893.5;
	x_all_sumdiff_maxcount[394] = 1428;
	x_all_sumdiff_01_min[394] = -1091.5;
	x_all_sumdiff_01_max[394] = -821.5;
	x_all_sumdiff_10_min[394] = -1021.5;
	x_all_sumdiff_10_max[394] = -845.5;
	z_all_sumdiff_mode[394] = -277.5;
	z_all_sumdiff_maxcount[394] = 1229;
	z_all_sumdiff_01_min[394] = -497.5;
	z_all_sumdiff_01_max[394] = -207.5;
	z_all_sumdiff_10_min[394] = -431.5;
	z_all_sumdiff_10_max[394] = -235.5;
	x_pi_ac_sumdiff_mode[394] = -893.5;
	x_pi_ac_sumdiff_maxcount[394] = 864;
	x_pi_ac_sumdiff_01_min[394] = -1077.5;
	x_pi_ac_sumdiff_01_max[394] = -827.5;
	x_pi_ac_sumdiff_10_min[394] = -1013.5;
	x_pi_ac_sumdiff_10_max[394] = -859.5;
	z_pi_ac_sumdiff_mode[394] = -275.5;
	z_pi_ac_sumdiff_maxcount[394] = 723;
	z_pi_ac_sumdiff_01_min[394] = -495.5;
	z_pi_ac_sumdiff_01_max[394] = -223.5;
	z_pi_ac_sumdiff_10_min[394] = -425.5;
	z_pi_ac_sumdiff_10_max[394] = -239.5;
	x_r_sumdiff_mode[394] = -893.5;
	x_r_sumdiff_maxcount[394] = 108;
	x_r_sumdiff_01_min[394] = -1063.5;
	x_r_sumdiff_01_max[394] = -817.5;
	x_r_sumdiff_10_min[394] = -1049.5;
	x_r_sumdiff_10_max[394] = -825.5;
	z_r_sumdiff_mode[394] = -269.5;
	z_r_sumdiff_maxcount[394] = 121;
	z_r_sumdiff_01_min[394] = -455.5;
	z_r_sumdiff_01_max[394] = -207.5;
	z_r_sumdiff_10_min[394] = -403.5;
	z_r_sumdiff_10_max[394] = -219.5;
	x_gr_sumdiff_mode[394] = -927.5;
	x_gr_sumdiff_maxcount[394] = 22;
	x_gr_sumdiff_01_min[394] = -1043.5;
	x_gr_sumdiff_01_max[394] = -819.5;
	x_gr_sumdiff_10_min[394] = -975.5;
	x_gr_sumdiff_10_max[394] = -837.5;
	z_gr_sumdiff_mode[394] = -299.5;
	z_gr_sumdiff_maxcount[394] = 24;
	z_gr_sumdiff_01_min[394] = -381.5;
	z_gr_sumdiff_01_max[394] = -215.5;
	z_gr_sumdiff_10_min[394] = -381.5;
	z_gr_sumdiff_10_max[394] = -225.5;

	x_all_sumdiff_mode[395] = -895.5;
	x_all_sumdiff_maxcount[395] = 1106;
	x_all_sumdiff_01_min[395] = -1095.5;
	x_all_sumdiff_01_max[395] = -819.5;
	x_all_sumdiff_10_min[395] = -1031.5;
	x_all_sumdiff_10_max[395] = -847.5;
	z_all_sumdiff_mode[395] = -279.5;
	z_all_sumdiff_maxcount[395] = 981;
	z_all_sumdiff_01_min[395] = -505.5;
	z_all_sumdiff_01_max[395] = -211.5;
	z_all_sumdiff_10_min[395] = -437.5;
	z_all_sumdiff_10_max[395] = -235.5;
	x_pi_ac_sumdiff_mode[395] = -907.5;
	x_pi_ac_sumdiff_maxcount[395] = 653;
	x_pi_ac_sumdiff_01_min[395] = -1095.5;
	x_pi_ac_sumdiff_01_max[395] = -821.5;
	x_pi_ac_sumdiff_10_min[395] = -1023.5;
	x_pi_ac_sumdiff_10_max[395] = -859.5;
	z_pi_ac_sumdiff_mode[395] = -279.5;
	z_pi_ac_sumdiff_maxcount[395] = 548;
	z_pi_ac_sumdiff_01_min[395] = -507.5;
	z_pi_ac_sumdiff_01_max[395] = -223.5;
	z_pi_ac_sumdiff_10_min[395] = -435.5;
	z_pi_ac_sumdiff_10_max[395] = -245.5;
	x_r_sumdiff_mode[395] = -911.5;
	x_r_sumdiff_maxcount[395] = 89;
	x_r_sumdiff_01_min[395] = -1081.5;
	x_r_sumdiff_01_max[395] = -811.5;
	x_r_sumdiff_10_min[395] = -1031.5;
	x_r_sumdiff_10_max[395] = -827.5;
	z_r_sumdiff_mode[395] = -291.5;
	z_r_sumdiff_maxcount[395] = 94;
	z_r_sumdiff_01_min[395] = -469.5;
	z_r_sumdiff_01_max[395] = -207.5;
	z_r_sumdiff_10_min[395] = -417.5;
	z_r_sumdiff_10_max[395] = -217.5;
	x_gr_sumdiff_mode[395] = -891.5;
	x_gr_sumdiff_maxcount[395] = 22;
	x_gr_sumdiff_01_min[395] = -1007.5;
	x_gr_sumdiff_01_max[395] = -827.5;
	x_gr_sumdiff_10_min[395] = -1001.5;
	x_gr_sumdiff_10_max[395] = -835.5;
	z_gr_sumdiff_mode[395] = -319.5;
	z_gr_sumdiff_maxcount[395] = 19;
	z_gr_sumdiff_01_min[395] = -411.5;
	z_gr_sumdiff_01_max[395] = -211.5;
	z_gr_sumdiff_10_min[395] = -395.5;
	z_gr_sumdiff_10_max[395] = -229.5;

	x_all_sumdiff_mode[396] = -901.5;
	x_all_sumdiff_maxcount[396] = 1016;
	x_all_sumdiff_01_min[396] = -1095.5;
	x_all_sumdiff_01_max[396] = -813.5;
	x_all_sumdiff_10_min[396] = -1023.5;
	x_all_sumdiff_10_max[396] = -845.5;
	z_all_sumdiff_mode[396] = -277.5;
	z_all_sumdiff_maxcount[396] = 889;
	z_all_sumdiff_01_min[396] = -505.5;
	z_all_sumdiff_01_max[396] = -213.5;
	z_all_sumdiff_10_min[396] = -435.5;
	z_all_sumdiff_10_max[396] = -233.5;
	x_pi_ac_sumdiff_mode[396] = -905.5;
	x_pi_ac_sumdiff_maxcount[396] = 590;
	x_pi_ac_sumdiff_01_min[396] = -1085.5;
	x_pi_ac_sumdiff_01_max[396] = -823.5;
	x_pi_ac_sumdiff_10_min[396] = -1019.5;
	x_pi_ac_sumdiff_10_max[396] = -857.5;
	z_pi_ac_sumdiff_mode[396] = -283.5;
	z_pi_ac_sumdiff_maxcount[396] = 507;
	z_pi_ac_sumdiff_01_min[396] = -495.5;
	z_pi_ac_sumdiff_01_max[396] = -215.5;
	z_pi_ac_sumdiff_10_min[396] = -431.5;
	z_pi_ac_sumdiff_10_max[396] = -241.5;
	x_r_sumdiff_mode[396] = -903.5;
	x_r_sumdiff_maxcount[396] = 92;
	x_r_sumdiff_01_min[396] = -1073.5;
	x_r_sumdiff_01_max[396] = -815.5;
	x_r_sumdiff_10_min[396] = -1021.5;
	x_r_sumdiff_10_max[396] = -829.5;
	z_r_sumdiff_mode[396] = -287.5;
	z_r_sumdiff_maxcount[396] = 100;
	z_r_sumdiff_01_min[396] = -459.5;
	z_r_sumdiff_01_max[396] = -201.5;
	z_r_sumdiff_10_min[396] = -405.5;
	z_r_sumdiff_10_max[396] = -221.5;
	x_gr_sumdiff_mode[396] = -923.5;
	x_gr_sumdiff_maxcount[396] = 22;
	x_gr_sumdiff_01_min[396] = -1021.5;
	x_gr_sumdiff_01_max[396] = -833.5;
	x_gr_sumdiff_10_min[396] = -1001.5;
	x_gr_sumdiff_10_max[396] = -851.5;
	z_gr_sumdiff_mode[396] = -285.5;
	z_gr_sumdiff_maxcount[396] = 20;
	z_gr_sumdiff_01_min[396] = -397.5;
	z_gr_sumdiff_01_max[396] = -221.5;
	z_gr_sumdiff_10_min[396] = -379.5;
	z_gr_sumdiff_10_max[396] = -221.5;

	x_all_sumdiff_mode[398] = -901.5;
	x_all_sumdiff_maxcount[398] = 1246;
	x_all_sumdiff_01_min[398] = -1097.5;
	x_all_sumdiff_01_max[398] = -817.5;
	x_all_sumdiff_10_min[398] = -1031.5;
	x_all_sumdiff_10_max[398] = -845.5;
	z_all_sumdiff_mode[398] = -283.5;
	z_all_sumdiff_maxcount[398] = 1066;
	z_all_sumdiff_01_min[398] = -499.5;
	z_all_sumdiff_01_max[398] = -207.5;
	z_all_sumdiff_10_min[398] = -441.5;
	z_all_sumdiff_10_max[398] = -233.5;
	x_pi_ac_sumdiff_mode[398] = -905.5;
	x_pi_ac_sumdiff_maxcount[398] = 688;
	x_pi_ac_sumdiff_01_min[398] = -1093.5;
	x_pi_ac_sumdiff_01_max[398] = -817.5;
	x_pi_ac_sumdiff_10_min[398] = -1031.5;
	x_pi_ac_sumdiff_10_max[398] = -859.5;
	z_pi_ac_sumdiff_mode[398] = -283.5;
	z_pi_ac_sumdiff_maxcount[398] = 578;
	z_pi_ac_sumdiff_01_min[398] = -499.5;
	z_pi_ac_sumdiff_01_max[398] = -219.5;
	z_pi_ac_sumdiff_10_min[398] = -443.5;
	z_pi_ac_sumdiff_10_max[398] = -243.5;
	x_r_sumdiff_mode[398] = -901.5;
	x_r_sumdiff_maxcount[398] = 121;
	x_r_sumdiff_01_min[398] = -1085.5;
	x_r_sumdiff_01_max[398] = -819.5;
	x_r_sumdiff_10_min[398] = -1039.5;
	x_r_sumdiff_10_max[398] = -827.5;
	z_r_sumdiff_mode[398] = -283.5;
	z_r_sumdiff_maxcount[398] = 133;
	z_r_sumdiff_01_min[398] = -481.5;
	z_r_sumdiff_01_max[398] = -205.5;
	z_r_sumdiff_10_min[398] = -405.5;
	z_r_sumdiff_10_max[398] = -219.5;
	x_gr_sumdiff_mode[398] = -897.5;
	x_gr_sumdiff_maxcount[398] = 28;
	x_gr_sumdiff_01_min[398] = -1055.5;
	x_gr_sumdiff_01_max[398] = -825.5;
	x_gr_sumdiff_10_min[398] = -1013.5;
	x_gr_sumdiff_10_max[398] = -837.5;
	z_gr_sumdiff_mode[398] = -283.5;
	z_gr_sumdiff_maxcount[398] = 26;
	z_gr_sumdiff_01_min[398] = -431.5;
	z_gr_sumdiff_01_max[398] = -207.5;
	z_gr_sumdiff_10_min[398] = -375.5;
	z_gr_sumdiff_10_max[398] = -227.5;

	x_all_sumdiff_mode[399] = -909.5;
	x_all_sumdiff_maxcount[399] = 1284;
	x_all_sumdiff_01_min[399] = -1101.5;
	x_all_sumdiff_01_max[399] = -815.5;
	x_all_sumdiff_10_min[399] = -1037.5;
	x_all_sumdiff_10_max[399] = -837.5;
	z_all_sumdiff_mode[399] = -285.5;
	z_all_sumdiff_maxcount[399] = 1150;
	z_all_sumdiff_01_min[399] = -505.5;
	z_all_sumdiff_01_max[399] = -207.5;
	z_all_sumdiff_10_min[399] = -443.5;
	z_all_sumdiff_10_max[399] = -229.5;
	x_pi_ac_sumdiff_mode[399] = -909.5;
	x_pi_ac_sumdiff_maxcount[399] = 720;
	x_pi_ac_sumdiff_01_min[399] = -1097.5;
	x_pi_ac_sumdiff_01_max[399] = -825.5;
	x_pi_ac_sumdiff_10_min[399] = -1031.5;
	x_pi_ac_sumdiff_10_max[399] = -853.5;
	z_pi_ac_sumdiff_mode[399] = -279.5;
	z_pi_ac_sumdiff_maxcount[399] = 583;
	z_pi_ac_sumdiff_01_min[399] = -507.5;
	z_pi_ac_sumdiff_01_max[399] = -219.5;
	z_pi_ac_sumdiff_10_min[399] = -445.5;
	z_pi_ac_sumdiff_10_max[399] = -239.5;
	x_r_sumdiff_mode[399] = -905.5;
	x_r_sumdiff_maxcount[399] = 167;
	x_r_sumdiff_01_min[399] = -1103.5;
	x_r_sumdiff_01_max[399] = -815.5;
	x_r_sumdiff_10_min[399] = -1033.5;
	x_r_sumdiff_10_max[399] = -827.5;
	z_r_sumdiff_mode[399] = -275.5;
	z_r_sumdiff_maxcount[399] = 168;
	z_r_sumdiff_01_min[399] = -493.5;
	z_r_sumdiff_01_max[399] = -207.5;
	z_r_sumdiff_10_min[399] = -425.5;
	z_r_sumdiff_10_max[399] = -221.5;
	x_gr_sumdiff_mode[399] = -905.5;
	x_gr_sumdiff_maxcount[399] = 42;
	x_gr_sumdiff_01_min[399] = -1041.5;
	x_gr_sumdiff_01_max[399] = -825.5;
	x_gr_sumdiff_10_min[399] = -1013.5;
	x_gr_sumdiff_10_max[399] = -837.5;
	z_gr_sumdiff_mode[399] = -301.5;
	z_gr_sumdiff_maxcount[399] = 38;
	z_gr_sumdiff_01_min[399] = -447.5;
	z_gr_sumdiff_01_max[399] = -209.5;
	z_gr_sumdiff_10_min[399] = -381.5;
	z_gr_sumdiff_10_max[399] = -225.5;

	x_all_sumdiff_mode[400] = -907.5;
	x_all_sumdiff_maxcount[400] = 1088;
	x_all_sumdiff_01_min[400] = -1101.5;
	x_all_sumdiff_01_max[400] = -815.5;
	x_all_sumdiff_10_min[400] = -1039.5;
	x_all_sumdiff_10_max[400] = -839.5;
	z_all_sumdiff_mode[400] = -289.5;
	z_all_sumdiff_maxcount[400] = 978;
	z_all_sumdiff_01_min[400] = -509.5;
	z_all_sumdiff_01_max[400] = -207.5;
	z_all_sumdiff_10_min[400] = -435.5;
	z_all_sumdiff_10_max[400] = -227.5;
	x_pi_ac_sumdiff_mode[400] = -905.5;
	x_pi_ac_sumdiff_maxcount[400] = 602;
	x_pi_ac_sumdiff_01_min[400] = -1087.5;
	x_pi_ac_sumdiff_01_max[400] = -821.5;
	x_pi_ac_sumdiff_10_min[400] = -1027.5;
	x_pi_ac_sumdiff_10_max[400] = -857.5;
	z_pi_ac_sumdiff_mode[400] = -279.5;
	z_pi_ac_sumdiff_maxcount[400] = 498;
	z_pi_ac_sumdiff_01_min[400] = -509.5;
	z_pi_ac_sumdiff_01_max[400] = -211.5;
	z_pi_ac_sumdiff_10_min[400] = -433.5;
	z_pi_ac_sumdiff_10_max[400] = -241.5;
	x_r_sumdiff_mode[400] = -917.5;
	x_r_sumdiff_maxcount[400] = 133;
	x_r_sumdiff_01_min[400] = -1079.5;
	x_r_sumdiff_01_max[400] = -823.5;
	x_r_sumdiff_10_min[400] = -1039.5;
	x_r_sumdiff_10_max[400] = -827.5;
	z_r_sumdiff_mode[400] = -293.5;
	z_r_sumdiff_maxcount[400] = 143;
	z_r_sumdiff_01_min[400] = -461.5;
	z_r_sumdiff_01_max[400] = -207.5;
	z_r_sumdiff_10_min[400] = -417.5;
	z_r_sumdiff_10_max[400] = -217.5;
	x_gr_sumdiff_mode[400] = -923.5;
	x_gr_sumdiff_maxcount[400] = 35;
	x_gr_sumdiff_01_min[400] = -1043.5;
	x_gr_sumdiff_01_max[400] = -823.5;
	x_gr_sumdiff_10_min[400] = -1017.5;
	x_gr_sumdiff_10_max[400] = -833.5;
	z_gr_sumdiff_mode[400] = -293.5;
	z_gr_sumdiff_maxcount[400] = 33;
	z_gr_sumdiff_01_min[400] = -437.5;
	z_gr_sumdiff_01_max[400] = -207.5;
	z_gr_sumdiff_10_min[400] = -389.5;
	z_gr_sumdiff_10_max[400] = -237.5;

	x_all_sumdiff_mode[401] = -907.5;
	x_all_sumdiff_maxcount[401] = 1282;
	x_all_sumdiff_01_min[401] = -1101.5;
	x_all_sumdiff_01_max[401] = -813.5;
	x_all_sumdiff_10_min[401] = -1037.5;
	x_all_sumdiff_10_max[401] = -839.5;
	z_all_sumdiff_mode[401] = -279.5;
	z_all_sumdiff_maxcount[401] = 1094;
	z_all_sumdiff_01_min[401] = -517.5;
	z_all_sumdiff_01_max[401] = -205.5;
	z_all_sumdiff_10_min[401] = -445.5;
	z_all_sumdiff_10_max[401] = -227.5;
	x_pi_ac_sumdiff_mode[401] = -907.5;
	x_pi_ac_sumdiff_maxcount[401] = 680;
	x_pi_ac_sumdiff_01_min[401] = -1091.5;
	x_pi_ac_sumdiff_01_max[401] = -819.5;
	x_pi_ac_sumdiff_10_min[401] = -1029.5;
	x_pi_ac_sumdiff_10_max[401] = -851.5;
	z_pi_ac_sumdiff_mode[401] = -283.5;
	z_pi_ac_sumdiff_maxcount[401] = 549;
	z_pi_ac_sumdiff_01_min[401] = -507.5;
	z_pi_ac_sumdiff_01_max[401] = -217.5;
	z_pi_ac_sumdiff_10_min[401] = -437.5;
	z_pi_ac_sumdiff_10_max[401] = -239.5;
	x_r_sumdiff_mode[401] = -907.5;
	x_r_sumdiff_maxcount[401] = 165;
	x_r_sumdiff_01_min[401] = -1101.5;
	x_r_sumdiff_01_max[401] = -815.5;
	x_r_sumdiff_10_min[401] = -1043.5;
	x_r_sumdiff_10_max[401] = -831.5;
	z_r_sumdiff_mode[401] = -295.5;
	z_r_sumdiff_maxcount[401] = 170;
	z_r_sumdiff_01_min[401] = -495.5;
	z_r_sumdiff_01_max[401] = -199.5;
	z_r_sumdiff_10_min[401] = -413.5;
	z_r_sumdiff_10_max[401] = -219.5;
	x_gr_sumdiff_mode[401] = -919.5;
	x_gr_sumdiff_maxcount[401] = 38;
	x_gr_sumdiff_01_min[401] = -1035.5;
	x_gr_sumdiff_01_max[401] = -831.5;
	x_gr_sumdiff_10_min[401] = -1031.5;
	x_gr_sumdiff_10_max[401] = -837.5;
	z_gr_sumdiff_mode[401] = -307.5;
	z_gr_sumdiff_maxcount[401] = 35;
	z_gr_sumdiff_01_min[401] = -449.5;
	z_gr_sumdiff_01_max[401] = -217.5;
	z_gr_sumdiff_10_min[401] = -395.5;
	z_gr_sumdiff_10_max[401] = -225.5;

	x_all_sumdiff_mode[402] = -909.5;
	x_all_sumdiff_maxcount[402] = 1125;
	x_all_sumdiff_01_min[402] = -1093.5;
	x_all_sumdiff_01_max[402] = -817.5;
	x_all_sumdiff_10_min[402] = -1035.5;
	x_all_sumdiff_10_max[402] = -837.5;
	z_all_sumdiff_mode[402] = -283.5;
	z_all_sumdiff_maxcount[402] = 1042;
	z_all_sumdiff_01_min[402] = -505.5;
	z_all_sumdiff_01_max[402] = -209.5;
	z_all_sumdiff_10_min[402] = -439.5;
	z_all_sumdiff_10_max[402] = -229.5;
	x_pi_ac_sumdiff_mode[402] = -897.5;
	x_pi_ac_sumdiff_maxcount[402] = 599;
	x_pi_ac_sumdiff_01_min[402] = -1093.5;
	x_pi_ac_sumdiff_01_max[402] = -823.5;
	x_pi_ac_sumdiff_10_min[402] = -1033.5;
	x_pi_ac_sumdiff_10_max[402] = -847.5;
	z_pi_ac_sumdiff_mode[402] = -281.5;
	z_pi_ac_sumdiff_maxcount[402] = 515;
	z_pi_ac_sumdiff_01_min[402] = -505.5;
	z_pi_ac_sumdiff_01_max[402] = -213.5;
	z_pi_ac_sumdiff_10_min[402] = -439.5;
	z_pi_ac_sumdiff_10_max[402] = -237.5;
	x_r_sumdiff_mode[402] = -909.5;
	x_r_sumdiff_maxcount[402] = 139;
	x_r_sumdiff_01_min[402] = -1083.5;
	x_r_sumdiff_01_max[402] = -807.5;
	x_r_sumdiff_10_min[402] = -1033.5;
	x_r_sumdiff_10_max[402] = -831.5;
	z_r_sumdiff_mode[402] = -283.5;
	z_r_sumdiff_maxcount[402] = 158;
	z_r_sumdiff_01_min[402] = -475.5;
	z_r_sumdiff_01_max[402] = -205.5;
	z_r_sumdiff_10_min[402] = -411.5;
	z_r_sumdiff_10_max[402] = -219.5;
	x_gr_sumdiff_mode[402] = -905.5;
	x_gr_sumdiff_maxcount[402] = 33;
	x_gr_sumdiff_01_min[402] = -1017.5;
	x_gr_sumdiff_01_max[402] = -821.5;
	x_gr_sumdiff_10_min[402] = -1009.5;
	x_gr_sumdiff_10_max[402] = -833.5;
	z_gr_sumdiff_mode[402] = -285.5;
	z_gr_sumdiff_maxcount[402] = 35;
	z_gr_sumdiff_01_min[402] = -459.5;
	z_gr_sumdiff_01_max[402] = -215.5;
	z_gr_sumdiff_10_min[402] = -377.5;
	z_gr_sumdiff_10_max[402] = -229.5;

	x_all_sumdiff_mode[409] = -907.5;
	x_all_sumdiff_maxcount[409] = 1137;
	x_all_sumdiff_01_min[409] = -1083.5;
	x_all_sumdiff_01_max[409] = -823.5;
	x_all_sumdiff_10_min[409] = -1011.5;
	x_all_sumdiff_10_max[409] = -853.5;
	z_all_sumdiff_mode[409] = -299.5;
	z_all_sumdiff_maxcount[409] = 1051;
	z_all_sumdiff_01_min[409] = -495.5;
	z_all_sumdiff_01_max[409] = -219.5;
	z_all_sumdiff_10_min[409] = -421.5;
	z_all_sumdiff_10_max[409] = -243.5;
	x_pi_ac_sumdiff_mode[409] = -903.5;
	x_pi_ac_sumdiff_maxcount[409] = 627;
	x_pi_ac_sumdiff_01_min[409] = -1069.5;
	x_pi_ac_sumdiff_01_max[409] = -831.5;
	x_pi_ac_sumdiff_10_min[409] = -1001.5;
	x_pi_ac_sumdiff_10_max[409] = -859.5;
	z_pi_ac_sumdiff_mode[409] = -295.5;
	z_pi_ac_sumdiff_maxcount[409] = 577;
	z_pi_ac_sumdiff_01_min[409] = -489.5;
	z_pi_ac_sumdiff_01_max[409] = -229.5;
	z_pi_ac_sumdiff_10_min[409] = -407.5;
	z_pi_ac_sumdiff_10_max[409] = -255.5;
	x_r_sumdiff_mode[409] = -905.5;
	x_r_sumdiff_maxcount[409] = 126;
	x_r_sumdiff_01_min[409] = -1083.5;
	x_r_sumdiff_01_max[409] = -827.5;
	x_r_sumdiff_10_min[409] = -1043.5;
	x_r_sumdiff_10_max[409] = -841.5;
	z_r_sumdiff_mode[409] = -315.5;
	z_r_sumdiff_maxcount[409] = 130;
	z_r_sumdiff_01_min[409] = -483.5;
	z_r_sumdiff_01_max[409] = -219.5;
	z_r_sumdiff_10_min[409] = -413.5;
	z_r_sumdiff_10_max[409] = -233.5;
	x_gr_sumdiff_mode[409] = -895.5;
	x_gr_sumdiff_maxcount[409] = 32;
	x_gr_sumdiff_01_min[409] = -1045.5;
	x_gr_sumdiff_01_max[409] = -841.5;
	x_gr_sumdiff_10_min[409] = -1017.5;
	x_gr_sumdiff_10_max[409] = -845.5;
	z_gr_sumdiff_mode[409] = -305.5;
	z_gr_sumdiff_maxcount[409] = 27;
	z_gr_sumdiff_01_min[409] = -427.5;
	z_gr_sumdiff_01_max[409] = -233.5;
	z_gr_sumdiff_10_min[409] = -413.5;
	z_gr_sumdiff_10_max[409] = -241.5;

	x_all_sumdiff_mode[410] = -907.5;
	x_all_sumdiff_maxcount[410] = 1707;
	x_all_sumdiff_01_min[410] = -1083.5;
	x_all_sumdiff_01_max[410] = -827.5;
	x_all_sumdiff_10_min[410] = -1009.5;
	x_all_sumdiff_10_max[410] = -853.5;
	z_all_sumdiff_mode[410] = -295.5;
	z_all_sumdiff_maxcount[410] = 1532;
	z_all_sumdiff_01_min[410] = -499.5;
	z_all_sumdiff_01_max[410] = -225.5;
	z_all_sumdiff_10_min[410] = -419.5;
	z_all_sumdiff_10_max[410] = -245.5;
	x_pi_ac_sumdiff_mode[410] = -903.5;
	x_pi_ac_sumdiff_maxcount[410] = 1007;
	x_pi_ac_sumdiff_01_min[410] = -1051.5;
	x_pi_ac_sumdiff_01_max[410] = -833.5;
	x_pi_ac_sumdiff_10_min[410] = -993.5;
	x_pi_ac_sumdiff_10_max[410] = -863.5;
	z_pi_ac_sumdiff_mode[410] = -293.5;
	z_pi_ac_sumdiff_maxcount[410] = 893;
	z_pi_ac_sumdiff_01_min[410] = -481.5;
	z_pi_ac_sumdiff_01_max[410] = -229.5;
	z_pi_ac_sumdiff_10_min[410] = -409.5;
	z_pi_ac_sumdiff_10_max[410] = -255.5;
	x_r_sumdiff_mode[410] = -913.5;
	x_r_sumdiff_maxcount[410] = 148;
	x_r_sumdiff_01_min[410] = -1085.5;
	x_r_sumdiff_01_max[410] = -819.5;
	x_r_sumdiff_10_min[410] = -1041.5;
	x_r_sumdiff_10_max[410] = -841.5;
	z_r_sumdiff_mode[410] = -287.5;
	z_r_sumdiff_maxcount[410] = 146;
	z_r_sumdiff_01_min[410] = -493.5;
	z_r_sumdiff_01_max[410] = -219.5;
	z_r_sumdiff_10_min[410] = -425.5;
	z_r_sumdiff_10_max[410] = -233.5;
	x_gr_sumdiff_mode[410] = -911.5;
	x_gr_sumdiff_maxcount[410] = 37;
	x_gr_sumdiff_01_min[410] = -1009.5;
	x_gr_sumdiff_01_max[410] = -835.5;
	x_gr_sumdiff_10_min[410] = -1003.5;
	x_gr_sumdiff_10_max[410] = -845.5;
	z_gr_sumdiff_mode[410] = -297.5;
	z_gr_sumdiff_maxcount[410] = 37;
	z_gr_sumdiff_01_min[410] = -443.5;
	z_gr_sumdiff_01_max[410] = -227.5;
	z_gr_sumdiff_10_min[410] = -401.5;
	z_gr_sumdiff_10_max[410] = -235.5;

	x_all_sumdiff_mode[411] = -905.5;
	x_all_sumdiff_maxcount[411] = 999;
	x_all_sumdiff_01_min[411] = -1069.5;
	x_all_sumdiff_01_max[411] = -825.5;
	x_all_sumdiff_10_min[411] = -1005.5;
	x_all_sumdiff_10_max[411] = -853.5;
	z_all_sumdiff_mode[411] = -295.5;
	z_all_sumdiff_maxcount[411] = 897;
	z_all_sumdiff_01_min[411] = -489.5;
	z_all_sumdiff_01_max[411] = -225.5;
	z_all_sumdiff_10_min[411] = -419.5;
	z_all_sumdiff_10_max[411] = -249.5;
	x_pi_ac_sumdiff_mode[411] = -901.5;
	x_pi_ac_sumdiff_maxcount[411] = 581;
	x_pi_ac_sumdiff_01_min[411] = -1063.5;
	x_pi_ac_sumdiff_01_max[411] = -829.5;
	x_pi_ac_sumdiff_10_min[411] = -997.5;
	x_pi_ac_sumdiff_10_max[411] = -861.5;
	z_pi_ac_sumdiff_mode[411] = -295.5;
	z_pi_ac_sumdiff_maxcount[411] = 517;
	z_pi_ac_sumdiff_01_min[411] = -481.5;
	z_pi_ac_sumdiff_01_max[411] = -233.5;
	z_pi_ac_sumdiff_10_min[411] = -403.5;
	z_pi_ac_sumdiff_10_max[411] = -255.5;
	x_r_sumdiff_mode[411] = -905.5;
	x_r_sumdiff_maxcount[411] = 86;
	x_r_sumdiff_01_min[411] = -1079.5;
	x_r_sumdiff_01_max[411] = -825.5;
	x_r_sumdiff_10_min[411] = -1035.5;
	x_r_sumdiff_10_max[411] = -839.5;
	z_r_sumdiff_mode[411] = -295.5;
	z_r_sumdiff_maxcount[411] = 82;
	z_r_sumdiff_01_min[411] = -477.5;
	z_r_sumdiff_01_max[411] = -217.5;
	z_r_sumdiff_10_min[411] = -419.5;
	z_r_sumdiff_10_max[411] = -231.5;
	x_gr_sumdiff_mode[411] = -885.5;
	x_gr_sumdiff_maxcount[411] = 22;
	x_gr_sumdiff_01_min[411] = -1057.5;
	x_gr_sumdiff_01_max[411] = -833.5;
	x_gr_sumdiff_10_min[411] = -1003.5;
	x_gr_sumdiff_10_max[411] = -843.5;
	z_gr_sumdiff_mode[411] = -357.5;
	z_gr_sumdiff_maxcount[411] = 18;
	z_gr_sumdiff_01_min[411] = -413.5;
	z_gr_sumdiff_01_max[411] = -231.5;
	z_gr_sumdiff_10_min[411] = -399.5;
	z_gr_sumdiff_10_max[411] = -235.5;

	x_all_sumdiff_mode[412] = -903.5;
	x_all_sumdiff_maxcount[412] = 3608;
	x_all_sumdiff_01_min[412] = -1079.5;
	x_all_sumdiff_01_max[412] = -825.5;
	x_all_sumdiff_10_min[412] = -1007.5;
	x_all_sumdiff_10_max[412] = -855.5;
	z_all_sumdiff_mode[412] = -295.5;
	z_all_sumdiff_maxcount[412] = 3423;
	z_all_sumdiff_01_min[412] = -495.5;
	z_all_sumdiff_01_max[412] = -225.5;
	z_all_sumdiff_10_min[412] = -413.5;
	z_all_sumdiff_10_max[412] = -249.5;
	x_pi_ac_sumdiff_mode[412] = -903.5;
	x_pi_ac_sumdiff_maxcount[412] = 2117;
	x_pi_ac_sumdiff_01_min[412] = -1061.5;
	x_pi_ac_sumdiff_01_max[412] = -835.5;
	x_pi_ac_sumdiff_10_min[412] = -995.5;
	x_pi_ac_sumdiff_10_max[412] = -863.5;
	z_pi_ac_sumdiff_mode[412] = -295.5;
	z_pi_ac_sumdiff_maxcount[412] = 2004;
	z_pi_ac_sumdiff_01_min[412] = -491.5;
	z_pi_ac_sumdiff_01_max[412] = -235.5;
	z_pi_ac_sumdiff_10_min[412] = -409.5;
	z_pi_ac_sumdiff_10_max[412] = -257.5;
	x_r_sumdiff_mode[412] = -901.5;
	x_r_sumdiff_maxcount[412] = 274;
	x_r_sumdiff_01_min[412] = -1099.5;
	x_r_sumdiff_01_max[412] = -817.5;
	x_r_sumdiff_10_min[412] = -1041.5;
	x_r_sumdiff_10_max[412] = -839.5;
	z_r_sumdiff_mode[412] = -305.5;
	z_r_sumdiff_maxcount[412] = 280;
	z_r_sumdiff_01_min[412] = -501.5;
	z_r_sumdiff_01_max[412] = -213.5;
	z_r_sumdiff_10_min[412] = -417.5;
	z_r_sumdiff_10_max[412] = -229.5;
	x_gr_sumdiff_mode[412] = -917.5;
	x_gr_sumdiff_maxcount[412] = 54;
	x_gr_sumdiff_01_min[412] = -1063.5;
	x_gr_sumdiff_01_max[412] = -827.5;
	x_gr_sumdiff_10_min[412] = -1035.5;
	x_gr_sumdiff_10_max[412] = -839.5;
	z_gr_sumdiff_mode[412] = -305.5;
	z_gr_sumdiff_maxcount[412] = 57;
	z_gr_sumdiff_01_min[412] = -481.5;
	z_gr_sumdiff_01_max[412] = -213.5;
	z_gr_sumdiff_10_min[412] = -415.5;
	z_gr_sumdiff_10_max[412] = -229.5;

	x_all_sumdiff_mode[413] = -905.5;
	x_all_sumdiff_maxcount[413] = 6847;
	x_all_sumdiff_01_min[413] = -1077.5;
	x_all_sumdiff_01_max[413] = -823.5;
	x_all_sumdiff_10_min[413] = -1009.5;
	x_all_sumdiff_10_max[413] = -855.5;
	z_all_sumdiff_mode[413] = -297.5;
	z_all_sumdiff_maxcount[413] = 6225;
	z_all_sumdiff_01_min[413] = -495.5;
	z_all_sumdiff_01_max[413] = -223.5;
	z_all_sumdiff_10_min[413] = -419.5;
	z_all_sumdiff_10_max[413] = -247.5;
	x_pi_ac_sumdiff_mode[413] = -905.5;
	x_pi_ac_sumdiff_maxcount[413] = 3966;
	x_pi_ac_sumdiff_01_min[413] = -1063.5;
	x_pi_ac_sumdiff_01_max[413] = -835.5;
	x_pi_ac_sumdiff_10_min[413] = -991.5;
	x_pi_ac_sumdiff_10_max[413] = -863.5;
	z_pi_ac_sumdiff_mode[413] = -297.5;
	z_pi_ac_sumdiff_maxcount[413] = 3664;
	z_pi_ac_sumdiff_01_min[413] = -491.5;
	z_pi_ac_sumdiff_01_max[413] = -233.5;
	z_pi_ac_sumdiff_10_min[413] = -405.5;
	z_pi_ac_sumdiff_10_max[413] = -257.5;
	x_r_sumdiff_mode[413] = -897.5;
	x_r_sumdiff_maxcount[413] = 531;
	x_r_sumdiff_01_min[413] = -1095.5;
	x_r_sumdiff_01_max[413] = -817.5;
	x_r_sumdiff_10_min[413] = -1039.5;
	x_r_sumdiff_10_max[413] = -837.5;
	z_r_sumdiff_mode[413] = -301.5;
	z_r_sumdiff_maxcount[413] = 515;
	z_r_sumdiff_01_min[413] = -501.5;
	z_r_sumdiff_01_max[413] = -211.5;
	z_r_sumdiff_10_min[413] = -435.5;
	z_r_sumdiff_10_max[413] = -229.5;
	x_gr_sumdiff_mode[413] = -903.5;
	x_gr_sumdiff_maxcount[413] = 112;
	x_gr_sumdiff_01_min[413] = -1073.5;
	x_gr_sumdiff_01_max[413] = -833.5;
	x_gr_sumdiff_10_min[413] = -1039.5;
	x_gr_sumdiff_10_max[413] = -845.5;
	z_gr_sumdiff_mode[413] = -319.5;
	z_gr_sumdiff_maxcount[413] = 106;
	z_gr_sumdiff_01_min[413] = -473.5;
	z_gr_sumdiff_01_max[413] = -221.5;
	z_gr_sumdiff_10_min[413] = -421.5;
	z_gr_sumdiff_10_max[413] = -233.5;

	x_all_sumdiff_mode[414] = -901.5;
	x_all_sumdiff_maxcount[414] = 952;
	x_all_sumdiff_01_min[414] = -1069.5;
	x_all_sumdiff_01_max[414] = -823.5;
	x_all_sumdiff_10_min[414] = -1005.5;
	x_all_sumdiff_10_max[414] = -853.5;
	z_all_sumdiff_mode[414] = -295.5;
	z_all_sumdiff_maxcount[414] = 962;
	z_all_sumdiff_01_min[414] = -497.5;
	z_all_sumdiff_01_max[414] = -223.5;
	z_all_sumdiff_10_min[414] = -415.5;
	z_all_sumdiff_10_max[414] = -249.5;
	x_pi_ac_sumdiff_mode[414] = -897.5;
	x_pi_ac_sumdiff_maxcount[414] = 549;
	x_pi_ac_sumdiff_01_min[414] = -1063.5;
	x_pi_ac_sumdiff_01_max[414] = -839.5;
	x_pi_ac_sumdiff_10_min[414] = -993.5;
	x_pi_ac_sumdiff_10_max[414] = -861.5;
	z_pi_ac_sumdiff_mode[414] = -295.5;
	z_pi_ac_sumdiff_maxcount[414] = 570;
	z_pi_ac_sumdiff_01_min[414] = -479.5;
	z_pi_ac_sumdiff_01_max[414] = -233.5;
	z_pi_ac_sumdiff_10_min[414] = -397.5;
	z_pi_ac_sumdiff_10_max[414] = -257.5;
	x_r_sumdiff_mode[414] = -893.5;
	x_r_sumdiff_maxcount[414] = 87;
	x_r_sumdiff_01_min[414] = -1069.5;
	x_r_sumdiff_01_max[414] = -821.5;
	x_r_sumdiff_10_min[414] = -1025.5;
	x_r_sumdiff_10_max[414] = -841.5;
	z_r_sumdiff_mode[414] = -287.5;
	z_r_sumdiff_maxcount[414] = 90;
	z_r_sumdiff_01_min[414] = -499.5;
	z_r_sumdiff_01_max[414] = -221.5;
	z_r_sumdiff_10_min[414] = -415.5;
	z_r_sumdiff_10_max[414] = -233.5;
	x_gr_sumdiff_mode[414] = -875.5;
	x_gr_sumdiff_maxcount[414] = 23;
	x_gr_sumdiff_01_min[414] = -1033.5;
	x_gr_sumdiff_01_max[414] = -847.5;
	x_gr_sumdiff_10_min[414] = -993.5;
	x_gr_sumdiff_10_max[414] = -851.5;
	z_gr_sumdiff_mode[414] = -307.5;
	z_gr_sumdiff_maxcount[414] = 23;
	z_gr_sumdiff_01_min[414] = -425.5;
	z_gr_sumdiff_01_max[414] = -237.5;
	z_gr_sumdiff_10_min[414] = -381.5;
	z_gr_sumdiff_10_max[414] = -237.5;

	x_all_sumdiff_mode[415] = -903.5;
	x_all_sumdiff_maxcount[415] = 1815;
	x_all_sumdiff_01_min[415] = -1073.5;
	x_all_sumdiff_01_max[415] = -831.5;
	x_all_sumdiff_10_min[415] = -1003.5;
	x_all_sumdiff_10_max[415] = -857.5;
	z_all_sumdiff_mode[415] = -297.5;
	z_all_sumdiff_maxcount[415] = 1659;
	z_all_sumdiff_01_min[415] = -491.5;
	z_all_sumdiff_01_max[415] = -225.5;
	z_all_sumdiff_10_min[415] = -413.5;
	z_all_sumdiff_10_max[415] = -253.5;
	x_pi_ac_sumdiff_mode[415] = -903.5;
	x_pi_ac_sumdiff_maxcount[415] = 1068;
	x_pi_ac_sumdiff_01_min[415] = -1061.5;
	x_pi_ac_sumdiff_01_max[415] = -833.5;
	x_pi_ac_sumdiff_10_min[415] = -989.5;
	x_pi_ac_sumdiff_10_max[415] = -863.5;
	z_pi_ac_sumdiff_mode[415] = -297.5;
	z_pi_ac_sumdiff_maxcount[415] = 999;
	z_pi_ac_sumdiff_01_min[415] = -483.5;
	z_pi_ac_sumdiff_01_max[415] = -239.5;
	z_pi_ac_sumdiff_10_min[415] = -399.5;
	z_pi_ac_sumdiff_10_max[415] = -259.5;
	x_r_sumdiff_mode[415] = -903.5;
	x_r_sumdiff_maxcount[415] = 116;
	x_r_sumdiff_01_min[415] = -1075.5;
	x_r_sumdiff_01_max[415] = -815.5;
	x_r_sumdiff_10_min[415] = -1037.5;
	x_r_sumdiff_10_max[415] = -839.5;
	z_r_sumdiff_mode[415] = -285.5;
	z_r_sumdiff_maxcount[415] = 116;
	z_r_sumdiff_01_min[415] = -451.5;
	z_r_sumdiff_01_max[415] = -217.5;
	z_r_sumdiff_10_min[415] = -413.5;
	z_r_sumdiff_10_max[415] = -233.5;
	x_gr_sumdiff_mode[415] = -949.5;
	x_gr_sumdiff_maxcount[415] = 23;
	x_gr_sumdiff_01_min[415] = -1037.5;
	x_gr_sumdiff_01_max[415] = -839.5;
	x_gr_sumdiff_10_min[415] = -1007.5;
	x_gr_sumdiff_10_max[415] = -845.5;
	z_gr_sumdiff_mode[415] = -299.5;
	z_gr_sumdiff_maxcount[415] = 25;
	z_gr_sumdiff_01_min[415] = -413.5;
	z_gr_sumdiff_01_max[415] = -225.5;
	z_gr_sumdiff_10_min[415] = -397.5;
	z_gr_sumdiff_10_max[415] = -239.5;

	x_all_sumdiff_mode[416] = -903.5;
	x_all_sumdiff_maxcount[416] = 7165;
	x_all_sumdiff_01_min[416] = -1073.5;
	x_all_sumdiff_01_max[416] = -825.5;
	x_all_sumdiff_10_min[416] = -1003.5;
	x_all_sumdiff_10_max[416] = -857.5;
	z_all_sumdiff_mode[416] = -293.5;
	z_all_sumdiff_maxcount[416] = 6935;
	z_all_sumdiff_01_min[416] = -491.5;
	z_all_sumdiff_01_max[416] = -225.5;
	z_all_sumdiff_10_min[416] = -409.5;
	z_all_sumdiff_10_max[416] = -251.5;
	x_pi_ac_sumdiff_mode[416] = -903.5;
	x_pi_ac_sumdiff_maxcount[416] = 4296;
	x_pi_ac_sumdiff_01_min[416] = -1063.5;
	x_pi_ac_sumdiff_01_max[416] = -837.5;
	x_pi_ac_sumdiff_10_min[416] = -987.5;
	x_pi_ac_sumdiff_10_max[416] = -861.5;
	z_pi_ac_sumdiff_mode[416] = -293.5;
	z_pi_ac_sumdiff_maxcount[416] = 4191;
	z_pi_ac_sumdiff_01_min[416] = -485.5;
	z_pi_ac_sumdiff_01_max[416] = -237.5;
	z_pi_ac_sumdiff_10_min[416] = -395.5;
	z_pi_ac_sumdiff_10_max[416] = -257.5;
	x_r_sumdiff_mode[416] = -915.5;
	x_r_sumdiff_maxcount[416] = 454;
	x_r_sumdiff_01_min[416] = -1091.5;
	x_r_sumdiff_01_max[416] = -813.5;
	x_r_sumdiff_10_min[416] = -1043.5;
	x_r_sumdiff_10_max[416] = -837.5;
	z_r_sumdiff_mode[416] = -295.5;
	z_r_sumdiff_maxcount[416] = 465;
	z_r_sumdiff_01_min[416] = -495.5;
	z_r_sumdiff_01_max[416] = -205.5;
	z_r_sumdiff_10_min[416] = -427.5;
	z_r_sumdiff_10_max[416] = -233.5;
	x_gr_sumdiff_mode[416] = -921.5;
	x_gr_sumdiff_maxcount[416] = 95;
	x_gr_sumdiff_01_min[416] = -1079.5;
	x_gr_sumdiff_01_max[416] = -823.5;
	x_gr_sumdiff_10_min[416] = -1035.5;
	x_gr_sumdiff_10_max[416] = -847.5;
	z_gr_sumdiff_mode[416] = -307.5;
	z_gr_sumdiff_maxcount[416] = 98;
	z_gr_sumdiff_01_min[416] = -493.5;
	z_gr_sumdiff_01_max[416] = -215.5;
	z_gr_sumdiff_10_min[416] = -415.5;
	z_gr_sumdiff_10_max[416] = -233.5;

	x_all_sumdiff_mode[417] = -903.5;
	x_all_sumdiff_maxcount[417] = 6883;
	x_all_sumdiff_01_min[417] = -1075.5;
	x_all_sumdiff_01_max[417] = -825.5;
	x_all_sumdiff_10_min[417] = -1003.5;
	x_all_sumdiff_10_max[417] = -857.5;
	z_all_sumdiff_mode[417] = -295.5;
	z_all_sumdiff_maxcount[417] = 6502;
	z_all_sumdiff_01_min[417] = -491.5;
	z_all_sumdiff_01_max[417] = -225.5;
	z_all_sumdiff_10_min[417] = -411.5;
	z_all_sumdiff_10_max[417] = -253.5;
	x_pi_ac_sumdiff_mode[417] = -903.5;
	x_pi_ac_sumdiff_maxcount[417] = 4094;
	x_pi_ac_sumdiff_01_min[417] = -1063.5;
	x_pi_ac_sumdiff_01_max[417] = -839.5;
	x_pi_ac_sumdiff_10_min[417] = -989.5;
	x_pi_ac_sumdiff_10_max[417] = -863.5;
	z_pi_ac_sumdiff_mode[417] = -295.5;
	z_pi_ac_sumdiff_maxcount[417] = 3900;
	z_pi_ac_sumdiff_01_min[417] = -485.5;
	z_pi_ac_sumdiff_01_max[417] = -235.5;
	z_pi_ac_sumdiff_10_min[417] = -401.5;
	z_pi_ac_sumdiff_10_max[417] = -257.5;
	x_r_sumdiff_mode[417] = -921.5;
	x_r_sumdiff_maxcount[417] = 427;
	x_r_sumdiff_01_min[417] = -1085.5;
	x_r_sumdiff_01_max[417] = -813.5;
	x_r_sumdiff_10_min[417] = -1039.5;
	x_r_sumdiff_10_max[417] = -837.5;
	z_r_sumdiff_mode[417] = -303.5;
	z_r_sumdiff_maxcount[417] = 415;
	z_r_sumdiff_01_min[417] = -499.5;
	z_r_sumdiff_01_max[417] = -213.5;
	z_r_sumdiff_10_min[417] = -425.5;
	z_r_sumdiff_10_max[417] = -229.5;
	x_gr_sumdiff_mode[417] = -897.5;
	x_gr_sumdiff_maxcount[417] = 83;
	x_gr_sumdiff_01_min[417] = -1083.5;
	x_gr_sumdiff_01_max[417] = -827.5;
	x_gr_sumdiff_10_min[417] = -1027.5;
	x_gr_sumdiff_10_max[417] = -843.5;
	z_gr_sumdiff_mode[417] = -303.5;
	z_gr_sumdiff_maxcount[417] = 85;
	z_gr_sumdiff_01_min[417] = -489.5;
	z_gr_sumdiff_01_max[417] = -213.5;
	z_gr_sumdiff_10_min[417] = -407.5;
	z_gr_sumdiff_10_max[417] = -235.5;

	x_all_sumdiff_mode[418] = -903.5;
	x_all_sumdiff_maxcount[418] = 7271;
	x_all_sumdiff_01_min[418] = -1073.5;
	x_all_sumdiff_01_max[418] = -825.5;
	x_all_sumdiff_10_min[418] = -1007.5;
	x_all_sumdiff_10_max[418] = -859.5;
	z_all_sumdiff_mode[418] = -295.5;
	z_all_sumdiff_maxcount[418] = 6696;
	z_all_sumdiff_01_min[418] = -497.5;
	z_all_sumdiff_01_max[418] = -227.5;
	z_all_sumdiff_10_min[418] = -415.5;
	z_all_sumdiff_10_max[418] = -251.5;
	x_pi_ac_sumdiff_mode[418] = -903.5;
	x_pi_ac_sumdiff_maxcount[418] = 4314;
	x_pi_ac_sumdiff_01_min[418] = -1061.5;
	x_pi_ac_sumdiff_01_max[418] = -835.5;
	x_pi_ac_sumdiff_10_min[418] = -991.5;
	x_pi_ac_sumdiff_10_max[418] = -863.5;
	z_pi_ac_sumdiff_mode[418] = -293.5;
	z_pi_ac_sumdiff_maxcount[418] = 3984;
	z_pi_ac_sumdiff_01_min[418] = -489.5;
	z_pi_ac_sumdiff_01_max[418] = -233.5;
	z_pi_ac_sumdiff_10_min[418] = -403.5;
	z_pi_ac_sumdiff_10_max[418] = -259.5;
	x_r_sumdiff_mode[418] = -905.5;
	x_r_sumdiff_maxcount[418] = 509;
	x_r_sumdiff_01_min[418] = -1095.5;
	x_r_sumdiff_01_max[418] = -817.5;
	x_r_sumdiff_10_min[418] = -1041.5;
	x_r_sumdiff_10_max[418] = -839.5;
	z_r_sumdiff_mode[418] = -301.5;
	z_r_sumdiff_maxcount[418] = 487;
	z_r_sumdiff_01_min[418] = -505.5;
	z_r_sumdiff_01_max[418] = -211.5;
	z_r_sumdiff_10_min[418] = -423.5;
	z_r_sumdiff_10_max[418] = -231.5;
	x_gr_sumdiff_mode[418] = -921.5;
	x_gr_sumdiff_maxcount[418] = 104;
	x_gr_sumdiff_01_min[418] = -1069.5;
	x_gr_sumdiff_01_max[418] = -827.5;
	x_gr_sumdiff_10_min[418] = -1037.5;
	x_gr_sumdiff_10_max[418] = -849.5;
	z_gr_sumdiff_mode[418] = -301.5;
	z_gr_sumdiff_maxcount[418] = 106;
	z_gr_sumdiff_01_min[418] = -467.5;
	z_gr_sumdiff_01_max[418] = -215.5;
	z_gr_sumdiff_10_min[418] = -411.5;
	z_gr_sumdiff_10_max[418] = -229.5;

	x_all_sumdiff_mode[419] = -909.5;
	x_all_sumdiff_maxcount[419] = 3703;
	x_all_sumdiff_01_min[419] = -1081.5;
	x_all_sumdiff_01_max[419] = -827.5;
	x_all_sumdiff_10_min[419] = -1005.5;
	x_all_sumdiff_10_max[419] = -855.5;
	z_all_sumdiff_mode[419] = -295.5;
	z_all_sumdiff_maxcount[419] = 3571;
	z_all_sumdiff_01_min[419] = -489.5;
	z_all_sumdiff_01_max[419] = -223.5;
	z_all_sumdiff_10_min[419] = -415.5;
	z_all_sumdiff_10_max[419] = -249.5;
	x_pi_ac_sumdiff_mode[419] = -903.5;
	x_pi_ac_sumdiff_maxcount[419] = 2187;
	x_pi_ac_sumdiff_01_min[419] = -1069.5;
	x_pi_ac_sumdiff_01_max[419] = -835.5;
	x_pi_ac_sumdiff_10_min[419] = -991.5;
	x_pi_ac_sumdiff_10_max[419] = -863.5;
	z_pi_ac_sumdiff_mode[419] = -295.5;
	z_pi_ac_sumdiff_maxcount[419] = 2120;
	z_pi_ac_sumdiff_01_min[419] = -485.5;
	z_pi_ac_sumdiff_01_max[419] = -235.5;
	z_pi_ac_sumdiff_10_min[419] = -403.5;
	z_pi_ac_sumdiff_10_max[419] = -259.5;
	x_r_sumdiff_mode[419] = -901.5;
	x_r_sumdiff_maxcount[419] = 264;
	x_r_sumdiff_01_min[419] = -1083.5;
	x_r_sumdiff_01_max[419] = -815.5;
	x_r_sumdiff_10_min[419] = -1033.5;
	x_r_sumdiff_10_max[419] = -841.5;
	z_r_sumdiff_mode[419] = -297.5;
	z_r_sumdiff_maxcount[419] = 265;
	z_r_sumdiff_01_min[419] = -491.5;
	z_r_sumdiff_01_max[419] = -207.5;
	z_r_sumdiff_10_min[419] = -427.5;
	z_r_sumdiff_10_max[419] = -229.5;
	x_gr_sumdiff_mode[419] = -899.5;
	x_gr_sumdiff_maxcount[419] = 53;
	x_gr_sumdiff_01_min[419] = -1059.5;
	x_gr_sumdiff_01_max[419] = -831.5;
	x_gr_sumdiff_10_min[419] = -1033.5;
	x_gr_sumdiff_10_max[419] = -841.5;
	z_gr_sumdiff_mode[419] = -307.5;
	z_gr_sumdiff_maxcount[419] = 51;
	z_gr_sumdiff_01_min[419] = -463.5;
	z_gr_sumdiff_01_max[419] = -217.5;
	z_gr_sumdiff_10_min[419] = -417.5;
	z_gr_sumdiff_10_max[419] = -235.5;

	x_all_sumdiff_mode[421] = -905.5;
	x_all_sumdiff_maxcount[421] = 6108;
	x_all_sumdiff_01_min[421] = -1087.5;
	x_all_sumdiff_01_max[421] = -825.5;
	x_all_sumdiff_10_min[421] = -1019.5;
	x_all_sumdiff_10_max[421] = -851.5;
	z_all_sumdiff_mode[421] = -295.5;
	z_all_sumdiff_maxcount[421] = 5774;
	z_all_sumdiff_01_min[421] = -503.5;
	z_all_sumdiff_01_max[421] = -219.5;
	z_all_sumdiff_10_min[421] = -429.5;
	z_all_sumdiff_10_max[421] = -243.5;
	x_pi_ac_sumdiff_mode[421] = -907.5;
	x_pi_ac_sumdiff_maxcount[421] = 3444;
	x_pi_ac_sumdiff_01_min[421] = -1073.5;
	x_pi_ac_sumdiff_01_max[421] = -827.5;
	x_pi_ac_sumdiff_10_min[421] = -1011.5;
	x_pi_ac_sumdiff_10_max[421] = -861.5;
	z_pi_ac_sumdiff_mode[421] = -295.5;
	z_pi_ac_sumdiff_maxcount[421] = 3266;
	z_pi_ac_sumdiff_01_min[421] = -495.5;
	z_pi_ac_sumdiff_01_max[421] = -229.5;
	z_pi_ac_sumdiff_10_min[421] = -419.5;
	z_pi_ac_sumdiff_10_max[421] = -255.5;
	x_r_sumdiff_mode[421] = -903.5;
	x_r_sumdiff_maxcount[421] = 516;
	x_r_sumdiff_01_min[421] = -1099.5;
	x_r_sumdiff_01_max[421] = -815.5;
	x_r_sumdiff_10_min[421] = -1041.5;
	x_r_sumdiff_10_max[421] = -835.5;
	z_r_sumdiff_mode[421] = -293.5;
	z_r_sumdiff_maxcount[421] = 553;
	z_r_sumdiff_01_min[421] = -503.5;
	z_r_sumdiff_01_max[421] = -211.5;
	z_r_sumdiff_10_min[421] = -429.5;
	z_r_sumdiff_10_max[421] = -227.5;
	x_gr_sumdiff_mode[421] = -913.5;
	x_gr_sumdiff_maxcount[421] = 97;
	x_gr_sumdiff_01_min[421] = -1089.5;
	x_gr_sumdiff_01_max[421] = -823.5;
	x_gr_sumdiff_10_min[421] = -1037.5;
	x_gr_sumdiff_10_max[421] = -841.5;
	z_gr_sumdiff_mode[421] = -283.5;
	z_gr_sumdiff_maxcount[421] = 106;
	z_gr_sumdiff_01_min[421] = -471.5;
	z_gr_sumdiff_01_max[421] = -219.5;
	z_gr_sumdiff_10_min[421] = -409.5;
	z_gr_sumdiff_10_max[421] = -227.5;

	x_all_sumdiff_mode[422] = -903.5;
	x_all_sumdiff_maxcount[422] = 5587;
	x_all_sumdiff_01_min[422] = -1081.5;
	x_all_sumdiff_01_max[422] = -827.5;
	x_all_sumdiff_10_min[422] = -1013.5;
	x_all_sumdiff_10_max[422] = -859.5;
	z_all_sumdiff_mode[422] = -291.5;
	z_all_sumdiff_maxcount[422] = 5220;
	z_all_sumdiff_01_min[422] = -501.5;
	z_all_sumdiff_01_max[422] = -225.5;
	z_all_sumdiff_10_min[422] = -423.5;
	z_all_sumdiff_10_max[422] = -253.5;
	x_pi_ac_sumdiff_mode[422] = -903.5;
	x_pi_ac_sumdiff_maxcount[422] = 3304;
	x_pi_ac_sumdiff_01_min[422] = -1077.5;
	x_pi_ac_sumdiff_01_max[422] = -831.5;
	x_pi_ac_sumdiff_10_min[422] = -999.5;
	x_pi_ac_sumdiff_10_max[422] = -863.5;
	z_pi_ac_sumdiff_mode[422] = -293.5;
	z_pi_ac_sumdiff_maxcount[422] = 3145;
	z_pi_ac_sumdiff_01_min[422] = -491.5;
	z_pi_ac_sumdiff_01_max[422] = -239.5;
	z_pi_ac_sumdiff_10_min[422] = -415.5;
	z_pi_ac_sumdiff_10_max[422] = -259.5;
	x_r_sumdiff_mode[422] = -905.5;
	x_r_sumdiff_maxcount[422] = 360;
	x_r_sumdiff_01_min[422] = -1099.5;
	x_r_sumdiff_01_max[422] = -815.5;
	x_r_sumdiff_10_min[422] = -1041.5;
	x_r_sumdiff_10_max[422] = -841.5;
	z_r_sumdiff_mode[422] = -299.5;
	z_r_sumdiff_maxcount[422] = 363;
	z_r_sumdiff_01_min[422] = -481.5;
	z_r_sumdiff_01_max[422] = -207.5;
	z_r_sumdiff_10_min[422] = -427.5;
	z_r_sumdiff_10_max[422] = -231.5;
	x_gr_sumdiff_mode[422] = -905.5;
	x_gr_sumdiff_maxcount[422] = 74;
	x_gr_sumdiff_01_min[422] = -1069.5;
	x_gr_sumdiff_01_max[422] = -833.5;
	x_gr_sumdiff_10_min[422] = -1023.5;
	x_gr_sumdiff_10_max[422] = -839.5;
	z_gr_sumdiff_mode[422] = -311.5;
	z_gr_sumdiff_maxcount[422] = 74;
	z_gr_sumdiff_01_min[422] = -457.5;
	z_gr_sumdiff_01_max[422] = -217.5;
	z_gr_sumdiff_10_min[422] = -415.5;
	z_gr_sumdiff_10_max[422] = -235.5;

	x_all_sumdiff_mode[423] = -907.5;
	x_all_sumdiff_maxcount[423] = 7494;
	x_all_sumdiff_01_min[423] = -1087.5;
	x_all_sumdiff_01_max[423] = -827.5;
	x_all_sumdiff_10_min[423] = -1017.5;
	x_all_sumdiff_10_max[423] = -855.5;
	z_all_sumdiff_mode[423] = -293.5;
	z_all_sumdiff_maxcount[423] = 7193;
	z_all_sumdiff_01_min[423] = -503.5;
	z_all_sumdiff_01_max[423] = -223.5;
	z_all_sumdiff_10_min[423] = -427.5;
	z_all_sumdiff_10_max[423] = -249.5;
	x_pi_ac_sumdiff_mode[423] = -905.5;
	x_pi_ac_sumdiff_maxcount[423] = 4358;
	x_pi_ac_sumdiff_01_min[423] = -1073.5;
	x_pi_ac_sumdiff_01_max[423] = -827.5;
	x_pi_ac_sumdiff_10_min[423] = -1005.5;
	x_pi_ac_sumdiff_10_max[423] = -863.5;
	z_pi_ac_sumdiff_mode[423] = -297.5;
	z_pi_ac_sumdiff_maxcount[423] = 4196;
	z_pi_ac_sumdiff_01_min[423] = -495.5;
	z_pi_ac_sumdiff_01_max[423] = -233.5;
	z_pi_ac_sumdiff_10_min[423] = -419.5;
	z_pi_ac_sumdiff_10_max[423] = -259.5;
	x_r_sumdiff_mode[423] = -909.5;
	x_r_sumdiff_maxcount[423] = 610;
	x_r_sumdiff_01_min[423] = -1093.5;
	x_r_sumdiff_01_max[423] = -815.5;
	x_r_sumdiff_10_min[423] = -1045.5;
	x_r_sumdiff_10_max[423] = -837.5;
	z_r_sumdiff_mode[423] = -293.5;
	z_r_sumdiff_maxcount[423] = 614;
	z_r_sumdiff_01_min[423] = -495.5;
	z_r_sumdiff_01_max[423] = -209.5;
	z_r_sumdiff_10_min[423] = -425.5;
	z_r_sumdiff_10_max[423] = -231.5;
	x_gr_sumdiff_mode[423] = -917.5;
	x_gr_sumdiff_maxcount[423] = 123;
	x_gr_sumdiff_01_min[423] = -1067.5;
	x_gr_sumdiff_01_max[423] = -829.5;
	x_gr_sumdiff_10_min[423] = -1043.5;
	x_gr_sumdiff_10_max[423] = -841.5;
	z_gr_sumdiff_mode[423] = -305.5;
	z_gr_sumdiff_maxcount[423] = 111;
	z_gr_sumdiff_01_min[423] = -455.5;
	z_gr_sumdiff_01_max[423] = -219.5;
	z_gr_sumdiff_10_min[423] = -429.5;
	z_gr_sumdiff_10_max[423] = -231.5;

	x_all_sumdiff_mode[424] = -907.5;
	x_all_sumdiff_maxcount[424] = 6502;
	x_all_sumdiff_01_min[424] = -1087.5;
	x_all_sumdiff_01_max[424] = -821.5;
	x_all_sumdiff_10_min[424] = -1019.5;
	x_all_sumdiff_10_max[424] = -857.5;
	z_all_sumdiff_mode[424] = -295.5;
	z_all_sumdiff_maxcount[424] = 6169;
	z_all_sumdiff_01_min[424] = -503.5;
	z_all_sumdiff_01_max[424] = -223.5;
	z_all_sumdiff_10_min[424] = -427.5;
	z_all_sumdiff_10_max[424] = -251.5;
	x_pi_ac_sumdiff_mode[424] = -907.5;
	x_pi_ac_sumdiff_maxcount[424] = 3851;
	x_pi_ac_sumdiff_01_min[424] = -1077.5;
	x_pi_ac_sumdiff_01_max[424] = -829.5;
	x_pi_ac_sumdiff_10_min[424] = -1007.5;
	x_pi_ac_sumdiff_10_max[424] = -863.5;
	z_pi_ac_sumdiff_mode[424] = -295.5;
	z_pi_ac_sumdiff_maxcount[424] = 3664;
	z_pi_ac_sumdiff_01_min[424] = -499.5;
	z_pi_ac_sumdiff_01_max[424] = -235.5;
	z_pi_ac_sumdiff_10_min[424] = -421.5;
	z_pi_ac_sumdiff_10_max[424] = -259.5;
	x_r_sumdiff_mode[424] = -903.5;
	x_r_sumdiff_maxcount[424] = 493;
	x_r_sumdiff_01_min[424] = -1089.5;
	x_r_sumdiff_01_max[424] = -809.5;
	x_r_sumdiff_10_min[424] = -1051.5;
	x_r_sumdiff_10_max[424] = -839.5;
	z_r_sumdiff_mode[424] = -301.5;
	z_r_sumdiff_maxcount[424] = 502;
	z_r_sumdiff_01_min[424] = -497.5;
	z_r_sumdiff_01_max[424] = -211.5;
	z_r_sumdiff_10_min[424] = -429.5;
	z_r_sumdiff_10_max[424] = -229.5;
	x_gr_sumdiff_mode[424] = -917.5;
	x_gr_sumdiff_maxcount[424] = 98;
	x_gr_sumdiff_01_min[424] = -1075.5;
	x_gr_sumdiff_01_max[424] = -805.5;
	x_gr_sumdiff_10_min[424] = -1033.5;
	x_gr_sumdiff_10_max[424] = -839.5;
	z_gr_sumdiff_mode[424] = -317.5;
	z_gr_sumdiff_maxcount[424] = 107;
	z_gr_sumdiff_01_min[424] = -455.5;
	z_gr_sumdiff_01_max[424] = -217.5;
	z_gr_sumdiff_10_min[424] = -405.5;
	z_gr_sumdiff_10_max[424] = -237.5;

	x_all_sumdiff_mode[425] = -909.5;
	x_all_sumdiff_maxcount[425] = 5188;
	x_all_sumdiff_01_min[425] = -1085.5;
	x_all_sumdiff_01_max[425] = -823.5;
	x_all_sumdiff_10_min[425] = -1017.5;
	x_all_sumdiff_10_max[425] = -857.5;
	z_all_sumdiff_mode[425] = -297.5;
	z_all_sumdiff_maxcount[425] = 4928;
	z_all_sumdiff_01_min[425] = -501.5;
	z_all_sumdiff_01_max[425] = -223.5;
	z_all_sumdiff_10_min[425] = -425.5;
	z_all_sumdiff_10_max[425] = -251.5;
	x_pi_ac_sumdiff_mode[425] = -909.5;
	x_pi_ac_sumdiff_maxcount[425] = 3081;
	x_pi_ac_sumdiff_01_min[425] = -1077.5;
	x_pi_ac_sumdiff_01_max[425] = -823.5;
	x_pi_ac_sumdiff_10_min[425] = -1001.5;
	x_pi_ac_sumdiff_10_max[425] = -865.5;
	z_pi_ac_sumdiff_mode[425] = -297.5;
	z_pi_ac_sumdiff_maxcount[425] = 2859;
	z_pi_ac_sumdiff_01_min[425] = -501.5;
	z_pi_ac_sumdiff_01_max[425] = -235.5;
	z_pi_ac_sumdiff_10_min[425] = -419.5;
	z_pi_ac_sumdiff_10_max[425] = -259.5;
	x_r_sumdiff_mode[425] = -905.5;
	x_r_sumdiff_maxcount[425] = 391;
	x_r_sumdiff_01_min[425] = -1107.5;
	x_r_sumdiff_01_max[425] = -819.5;
	x_r_sumdiff_10_min[425] = -1047.5;
	x_r_sumdiff_10_max[425] = -837.5;
	z_r_sumdiff_mode[425] = -307.5;
	z_r_sumdiff_maxcount[425] = 390;
	z_r_sumdiff_01_min[425] = -499.5;
	z_r_sumdiff_01_max[425] = -205.5;
	z_r_sumdiff_10_min[425] = -423.5;
	z_r_sumdiff_10_max[425] = -231.5;
	x_gr_sumdiff_mode[425] = -905.5;
	x_gr_sumdiff_maxcount[425] = 83;
	x_gr_sumdiff_01_min[425] = -1085.5;
	x_gr_sumdiff_01_max[425] = -823.5;
	x_gr_sumdiff_10_min[425] = -1031.5;
	x_gr_sumdiff_10_max[425] = -841.5;
	z_gr_sumdiff_mode[425] = -295.5;
	z_gr_sumdiff_maxcount[425] = 78;
	z_gr_sumdiff_01_min[425] = -455.5;
	z_gr_sumdiff_01_max[425] = -211.5;
	z_gr_sumdiff_10_min[425] = -409.5;
	z_gr_sumdiff_10_max[425] = -235.5;

	x_all_sumdiff_mode[426] = -903.5;
	x_all_sumdiff_maxcount[426] = 6903;
	x_all_sumdiff_01_min[426] = -1087.5;
	x_all_sumdiff_01_max[426] = -823.5;
	x_all_sumdiff_10_min[426] = -1015.5;
	x_all_sumdiff_10_max[426] = -857.5;
	z_all_sumdiff_mode[426] = -297.5;
	z_all_sumdiff_maxcount[426] = 6508;
	z_all_sumdiff_01_min[426] = -501.5;
	z_all_sumdiff_01_max[426] = -223.5;
	z_all_sumdiff_10_min[426] = -429.5;
	z_all_sumdiff_10_max[426] = -251.5;
	x_pi_ac_sumdiff_mode[426] = -907.5;
	x_pi_ac_sumdiff_maxcount[426] = 3983;
	x_pi_ac_sumdiff_01_min[426] = -1079.5;
	x_pi_ac_sumdiff_01_max[426] = -825.5;
	x_pi_ac_sumdiff_10_min[426] = -1005.5;
	x_pi_ac_sumdiff_10_max[426] = -863.5;
	z_pi_ac_sumdiff_mode[426] = -295.5;
	z_pi_ac_sumdiff_maxcount[426] = 3798;
	z_pi_ac_sumdiff_01_min[426] = -499.5;
	z_pi_ac_sumdiff_01_max[426] = -235.5;
	z_pi_ac_sumdiff_10_min[426] = -429.5;
	z_pi_ac_sumdiff_10_max[426] = -259.5;
	x_r_sumdiff_mode[426] = -907.5;
	x_r_sumdiff_maxcount[426] = 509;
	x_r_sumdiff_01_min[426] = -1093.5;
	x_r_sumdiff_01_max[426] = -817.5;
	x_r_sumdiff_10_min[426] = -1045.5;
	x_r_sumdiff_10_max[426] = -837.5;
	z_r_sumdiff_mode[426] = -293.5;
	z_r_sumdiff_maxcount[426] = 536;
	z_r_sumdiff_01_min[426] = -509.5;
	z_r_sumdiff_01_max[426] = -207.5;
	z_r_sumdiff_10_min[426] = -423.5;
	z_r_sumdiff_10_max[426] = -231.5;
	x_gr_sumdiff_mode[426] = -893.5;
	x_gr_sumdiff_maxcount[426] = 101;
	x_gr_sumdiff_01_min[426] = -1077.5;
	x_gr_sumdiff_01_max[426] = -831.5;
	x_gr_sumdiff_10_min[426] = -1039.5;
	x_gr_sumdiff_10_max[426] = -841.5;
	z_gr_sumdiff_mode[426] = -293.5;
	z_gr_sumdiff_maxcount[426] = 125;
	z_gr_sumdiff_01_min[426] = -461.5;
	z_gr_sumdiff_01_max[426] = -221.5;
	z_gr_sumdiff_10_min[426] = -409.5;
	z_gr_sumdiff_10_max[426] = -235.5;

	x_all_sumdiff_mode[446] = -909.5;
	x_all_sumdiff_maxcount[446] = 2492;
	x_all_sumdiff_01_min[446] = -1083.5;
	x_all_sumdiff_01_max[446] = -825.5;
	x_all_sumdiff_10_min[446] = -1021.5;
	x_all_sumdiff_10_max[446] = -853.5;
	z_all_sumdiff_mode[446] = -293.5;
	z_all_sumdiff_maxcount[446] = 2270;
	z_all_sumdiff_01_min[446] = -501.5;
	z_all_sumdiff_01_max[446] = -221.5;
	z_all_sumdiff_10_min[446] = -431.5;
	z_all_sumdiff_10_max[446] = -247.5;
	x_pi_ac_sumdiff_mode[446] = -909.5;
	x_pi_ac_sumdiff_maxcount[446] = 1440;
	x_pi_ac_sumdiff_01_min[446] = -1073.5;
	x_pi_ac_sumdiff_01_max[446] = -827.5;
	x_pi_ac_sumdiff_10_min[446] = -1009.5;
	x_pi_ac_sumdiff_10_max[446] = -861.5;
	z_pi_ac_sumdiff_mode[446] = -293.5;
	z_pi_ac_sumdiff_maxcount[446] = 1303;
	z_pi_ac_sumdiff_01_min[446] = -493.5;
	z_pi_ac_sumdiff_01_max[446] = -235.5;
	z_pi_ac_sumdiff_10_min[446] = -431.5;
	z_pi_ac_sumdiff_10_max[446] = -257.5;
	x_r_sumdiff_mode[446] = -917.5;
	x_r_sumdiff_maxcount[446] = 209;
	x_r_sumdiff_01_min[446] = -1079.5;
	x_r_sumdiff_01_max[446] = -825.5;
	x_r_sumdiff_10_min[446] = -1037.5;
	x_r_sumdiff_10_max[446] = -839.5;
	z_r_sumdiff_mode[446] = -299.5;
	z_r_sumdiff_maxcount[446] = 214;
	z_r_sumdiff_01_min[446] = -483.5;
	z_r_sumdiff_01_max[446] = -215.5;
	z_r_sumdiff_10_min[446] = -415.5;
	z_r_sumdiff_10_max[446] = -229.5;
	x_gr_sumdiff_mode[446] = -935.5;
	x_gr_sumdiff_maxcount[446] = 40;
	x_gr_sumdiff_01_min[446] = -1049.5;
	x_gr_sumdiff_01_max[446] = -827.5;
	x_gr_sumdiff_10_min[446] = -1019.5;
	x_gr_sumdiff_10_max[446] = -849.5;
	z_gr_sumdiff_mode[446] = -277.5;
	z_gr_sumdiff_maxcount[446] = 46;
	z_gr_sumdiff_01_min[446] = -457.5;
	z_gr_sumdiff_01_max[446] = -219.5;
	z_gr_sumdiff_10_min[446] = -403.5;
	z_gr_sumdiff_10_max[446] = -231.5;

	x_all_sumdiff_mode[447] = -901.5;
	x_all_sumdiff_maxcount[447] = 5664;
	x_all_sumdiff_01_min[447] = -1081.5;
	x_all_sumdiff_01_max[447] = -821.5;
	x_all_sumdiff_10_min[447] = -1017.5;
	x_all_sumdiff_10_max[447] = -855.5;
	z_all_sumdiff_mode[447] = -293.5;
	z_all_sumdiff_maxcount[447] = 5329;
	z_all_sumdiff_01_min[447] = -503.5;
	z_all_sumdiff_01_max[447] = -223.5;
	z_all_sumdiff_10_min[447] = -427.5;
	z_all_sumdiff_10_max[447] = -251.5;
	x_pi_ac_sumdiff_mode[447] = -907.5;
	x_pi_ac_sumdiff_maxcount[447] = 3284;
	x_pi_ac_sumdiff_01_min[447] = -1073.5;
	x_pi_ac_sumdiff_01_max[447] = -819.5;
	x_pi_ac_sumdiff_10_min[447] = -1007.5;
	x_pi_ac_sumdiff_10_max[447] = -861.5;
	z_pi_ac_sumdiff_mode[447] = -291.5;
	z_pi_ac_sumdiff_maxcount[447] = 3069;
	z_pi_ac_sumdiff_01_min[447] = -493.5;
	z_pi_ac_sumdiff_01_max[447] = -237.5;
	z_pi_ac_sumdiff_10_min[447] = -427.5;
	z_pi_ac_sumdiff_10_max[447] = -257.5;
	x_r_sumdiff_mode[447] = -895.5;
	x_r_sumdiff_maxcount[447] = 394;
	x_r_sumdiff_01_min[447] = -1097.5;
	x_r_sumdiff_01_max[447] = -817.5;
	x_r_sumdiff_10_min[447] = -1045.5;
	x_r_sumdiff_10_max[447] = -835.5;
	z_r_sumdiff_mode[447] = -293.5;
	z_r_sumdiff_maxcount[447] = 419;
	z_r_sumdiff_01_min[447] = -493.5;
	z_r_sumdiff_01_max[447] = -209.5;
	z_r_sumdiff_10_min[447] = -425.5;
	z_r_sumdiff_10_max[447] = -231.5;
	x_gr_sumdiff_mode[447] = -917.5;
	x_gr_sumdiff_maxcount[447] = 71;
	x_gr_sumdiff_01_min[447] = -1075.5;
	x_gr_sumdiff_01_max[447] = -825.5;
	x_gr_sumdiff_10_min[447] = -1039.5;
	x_gr_sumdiff_10_max[447] = -845.5;
	z_gr_sumdiff_mode[447] = -317.5;
	z_gr_sumdiff_maxcount[447] = 73;
	z_gr_sumdiff_01_min[447] = -469.5;
	z_gr_sumdiff_01_max[447] = -209.5;
	z_gr_sumdiff_10_min[447] = -427.5;
	z_gr_sumdiff_10_max[447] = -235.5;

	x_all_sumdiff_mode[449] = -905.5;
	x_all_sumdiff_maxcount[449] = 6335;
	x_all_sumdiff_01_min[449] = -1085.5;
	x_all_sumdiff_01_max[449] = -819.5;
	x_all_sumdiff_10_min[449] = -1015.5;
	x_all_sumdiff_10_max[449] = -855.5;
	z_all_sumdiff_mode[449] = -297.5;
	z_all_sumdiff_maxcount[449] = 5976;
	z_all_sumdiff_01_min[449] = -499.5;
	z_all_sumdiff_01_max[449] = -225.5;
	z_all_sumdiff_10_min[449] = -427.5;
	z_all_sumdiff_10_max[449] = -251.5;
	x_pi_ac_sumdiff_mode[449] = -905.5;
	x_pi_ac_sumdiff_maxcount[449] = 3659;
	x_pi_ac_sumdiff_01_min[449] = -1081.5;
	x_pi_ac_sumdiff_01_max[449] = -823.5;
	x_pi_ac_sumdiff_10_min[449] = -1007.5;
	x_pi_ac_sumdiff_10_max[449] = -859.5;
	z_pi_ac_sumdiff_mode[449] = -297.5;
	z_pi_ac_sumdiff_maxcount[449] = 3475;
	z_pi_ac_sumdiff_01_min[449] = -493.5;
	z_pi_ac_sumdiff_01_max[449] = -235.5;
	z_pi_ac_sumdiff_10_min[449] = -425.5;
	z_pi_ac_sumdiff_10_max[449] = -257.5;
	x_r_sumdiff_mode[449] = -903.5;
	x_r_sumdiff_maxcount[449] = 467;
	x_r_sumdiff_01_min[449] = -1103.5;
	x_r_sumdiff_01_max[449] = -811.5;
	x_r_sumdiff_10_min[449] = -1047.5;
	x_r_sumdiff_10_max[449] = -837.5;
	z_r_sumdiff_mode[449] = -295.5;
	z_r_sumdiff_maxcount[449] = 469;
	z_r_sumdiff_01_min[449] = -509.5;
	z_r_sumdiff_01_max[449] = -215.5;
	z_r_sumdiff_10_min[449] = -429.5;
	z_r_sumdiff_10_max[449] = -231.5;
	x_gr_sumdiff_mode[449] = -919.5;
	x_gr_sumdiff_maxcount[449] = 90;
	x_gr_sumdiff_01_min[449] = -1071.5;
	x_gr_sumdiff_01_max[449] = -817.5;
	x_gr_sumdiff_10_min[449] = -1029.5;
	x_gr_sumdiff_10_max[449] = -843.5;
	z_gr_sumdiff_mode[449] = -297.5;
	z_gr_sumdiff_maxcount[449] = 107;
	z_gr_sumdiff_01_min[449] = -457.5;
	z_gr_sumdiff_01_max[449] = -217.5;
	z_gr_sumdiff_10_min[449] = -419.5;
	z_gr_sumdiff_10_max[449] = -233.5;



	x_all_sumdiff_mode[450] = -883.5;
	x_all_sumdiff_maxcount[450] = 6171;
	x_all_sumdiff_01_min[450] = -1097.5;
	x_all_sumdiff_01_max[450] = -789.5;
	x_all_sumdiff_10_min[450] = -1027.5;
	x_all_sumdiff_10_max[450] = -817.5;
	z_all_sumdiff_mode[450] = -275.5;
	z_all_sumdiff_maxcount[450] = 4849;
	z_all_sumdiff_01_min[450] = -509.5;
	z_all_sumdiff_01_max[450] = -185.5;
	z_all_sumdiff_10_min[450] = -441.5;
	z_all_sumdiff_10_max[450] = -209.5;
	x_pi_ac_sumdiff_mode[450] = -891.5;
	x_pi_ac_sumdiff_maxcount[450] = 1479;
	x_pi_ac_sumdiff_01_min[450] = -1107.5;
	x_pi_ac_sumdiff_01_max[450] = -783.5;
	x_pi_ac_sumdiff_10_min[450] = -1029.5;
	x_pi_ac_sumdiff_10_max[450] = -817.5;
	z_pi_ac_sumdiff_mode[450] = -281.5;
	z_pi_ac_sumdiff_maxcount[450] = 1206;
	z_pi_ac_sumdiff_01_min[450] = -505.5;
	z_pi_ac_sumdiff_01_max[450] = -185.5;
	z_pi_ac_sumdiff_10_min[450] = -439.5;
	z_pi_ac_sumdiff_10_max[450] = -211.5;
	x_r_sumdiff_mode[450] = -883.5;
	x_r_sumdiff_maxcount[450] = 2098;
	x_r_sumdiff_01_min[450] = -1095.5;
	x_r_sumdiff_01_max[450] = -791.5;
	x_r_sumdiff_10_min[450] = -1027.5;
	x_r_sumdiff_10_max[450] = -817.5;
	z_r_sumdiff_mode[450] = -271.5;
	z_r_sumdiff_maxcount[450] = 1608;
	z_r_sumdiff_01_min[450] = -503.5;
	z_r_sumdiff_01_max[450] = -185.5;
	z_r_sumdiff_10_min[450] = -439.5;
	z_r_sumdiff_10_max[450] = -209.5;
	x_gr_sumdiff_mode[450] = -881.5;
	x_gr_sumdiff_maxcount[450] = 302;
	x_gr_sumdiff_01_min[450] = -1077.5;
	x_gr_sumdiff_01_max[450] = -795.5;
	x_gr_sumdiff_10_min[450] = -1021.5;
	x_gr_sumdiff_10_max[450] = -821.5;
	z_gr_sumdiff_mode[450] = -265.5;
	z_gr_sumdiff_maxcount[450] = 235;
	z_gr_sumdiff_01_min[450] = -501.5;
	z_gr_sumdiff_01_max[450] = -185.5;
	z_gr_sumdiff_10_min[450] = -429.5;
	z_gr_sumdiff_10_max[450] = -205.5;

	x_all_sumdiff_mode[454] = -873.5;
	x_all_sumdiff_maxcount[454] = 6256;
	x_all_sumdiff_01_min[454] = -1057.5;
	x_all_sumdiff_01_max[454] = -797.5;
	x_all_sumdiff_10_min[454] = -989.5;
	x_all_sumdiff_10_max[454] = -819.5;
	z_all_sumdiff_mode[454] = -267.5;
	z_all_sumdiff_maxcount[454] = 5383;
	z_all_sumdiff_01_min[454] = -481.5;
	z_all_sumdiff_01_max[454] = -189.5;
	z_all_sumdiff_10_min[454] = -405.5;
	z_all_sumdiff_10_max[454] = -209.5;
	x_pi_ac_sumdiff_mode[454] = -883.5;
	x_pi_ac_sumdiff_maxcount[454] = 2629;
	x_pi_ac_sumdiff_01_min[454] = -1053.5;
	x_pi_ac_sumdiff_01_max[454] = -801.5;
	x_pi_ac_sumdiff_10_min[454] = -985.5;
	x_pi_ac_sumdiff_10_max[454] = -827.5;
	z_pi_ac_sumdiff_mode[454] = -267.5;
	z_pi_ac_sumdiff_maxcount[454] = 2257;
	z_pi_ac_sumdiff_01_min[454] = -481.5;
	z_pi_ac_sumdiff_01_max[454] = -193.5;
	z_pi_ac_sumdiff_10_min[454] = -405.5;
	z_pi_ac_sumdiff_10_max[454] = -219.5;
	x_r_sumdiff_mode[454] = -865.5;
	x_r_sumdiff_maxcount[454] = 1247;
	x_r_sumdiff_01_min[454] = -1051.5;
	x_r_sumdiff_01_max[454] = -797.5;
	x_r_sumdiff_10_min[454] = -995.5;
	x_r_sumdiff_10_max[454] = -815.5;
	z_r_sumdiff_mode[454] = -263.5;
	z_r_sumdiff_maxcount[454] = 1129;
	z_r_sumdiff_01_min[454] = -465.5;
	z_r_sumdiff_01_max[454] = -187.5;
	z_r_sumdiff_10_min[454] = -393.5;
	z_r_sumdiff_10_max[454] = -205.5;
	x_gr_sumdiff_mode[454] = -863.5;
	x_gr_sumdiff_maxcount[454] = 167;
	x_gr_sumdiff_01_min[454] = -1041.5;
	x_gr_sumdiff_01_max[454] = -797.5;
	x_gr_sumdiff_10_min[454] = -979.5;
	x_gr_sumdiff_10_max[454] = -817.5;
	z_gr_sumdiff_mode[454] = -261.5;
	z_gr_sumdiff_maxcount[454] = 156;
	z_gr_sumdiff_01_min[454] = -449.5;
	z_gr_sumdiff_01_max[454] = -197.5;
	z_gr_sumdiff_10_min[454] = -389.5;
	z_gr_sumdiff_10_max[454] = -207.5;

	x_all_sumdiff_mode[455] = -877.5;
	x_all_sumdiff_maxcount[455] = 7714;
	x_all_sumdiff_01_min[455] = -1055.5;
	x_all_sumdiff_01_max[455] = -795.5;
	x_all_sumdiff_10_min[455] = -987.5;
	x_all_sumdiff_10_max[455] = -819.5;
	z_all_sumdiff_mode[455] = -265.5;
	z_all_sumdiff_maxcount[455] = 6654;
	z_all_sumdiff_01_min[455] = -479.5;
	z_all_sumdiff_01_max[455] = -189.5;
	z_all_sumdiff_10_min[455] = -409.5;
	z_all_sumdiff_10_max[455] = -209.5;
	x_pi_ac_sumdiff_mode[455] = -877.5;
	x_pi_ac_sumdiff_maxcount[455] = 3304;
	x_pi_ac_sumdiff_01_min[455] = -1047.5;
	x_pi_ac_sumdiff_01_max[455] = -795.5;
	x_pi_ac_sumdiff_10_min[455] = -983.5;
	x_pi_ac_sumdiff_10_max[455] = -827.5;
	z_pi_ac_sumdiff_mode[455] = -271.5;
	z_pi_ac_sumdiff_maxcount[455] = 2777;
	z_pi_ac_sumdiff_01_min[455] = -481.5;
	z_pi_ac_sumdiff_01_max[455] = -195.5;
	z_pi_ac_sumdiff_10_min[455] = -409.5;
	z_pi_ac_sumdiff_10_max[455] = -217.5;
	x_r_sumdiff_mode[455] = -861.5;
	x_r_sumdiff_maxcount[455] = 1575;
	x_r_sumdiff_01_min[455] = -1055.5;
	x_r_sumdiff_01_max[455] = -795.5;
	x_r_sumdiff_10_min[455] = -989.5;
	x_r_sumdiff_10_max[455] = -815.5;
	z_r_sumdiff_mode[455] = -261.5;
	z_r_sumdiff_maxcount[455] = 1372;
	z_r_sumdiff_01_min[455] = -471.5;
	z_r_sumdiff_01_max[455] = -185.5;
	z_r_sumdiff_10_min[455] = -397.5;
	z_r_sumdiff_10_max[455] = -203.5;
	x_gr_sumdiff_mode[455] = -879.5;
	x_gr_sumdiff_maxcount[455] = 217;
	x_gr_sumdiff_01_min[455] = -1043.5;
	x_gr_sumdiff_01_max[455] = -801.5;
	x_gr_sumdiff_10_min[455] = -983.5;
	x_gr_sumdiff_10_max[455] = -815.5;
	z_gr_sumdiff_mode[455] = -253.5;
	z_gr_sumdiff_maxcount[455] = 189;
	z_gr_sumdiff_01_min[455] = -471.5;
	z_gr_sumdiff_01_max[455] = -183.5;
	z_gr_sumdiff_10_min[455] = -385.5;
	z_gr_sumdiff_10_max[455] = -203.5;

	x_all_sumdiff_mode[460] = -881.5;
	x_all_sumdiff_maxcount[460] = 10883;
	x_all_sumdiff_01_min[460] = -1045.5;
	x_all_sumdiff_01_max[460] = -793.5;
	x_all_sumdiff_10_min[460] = -967.5;
	x_all_sumdiff_10_max[460] = -817.5;
	z_all_sumdiff_mode[460] = -265.5;
	z_all_sumdiff_maxcount[460] = 13396;
	z_all_sumdiff_01_min[460] = -427.5;
	z_all_sumdiff_01_max[460] = -193.5;
	z_all_sumdiff_10_min[460] = -351.5;
	z_all_sumdiff_10_max[460] = -219.5;
	x_pi_ac_sumdiff_mode[460] = -881.5;
	x_pi_ac_sumdiff_maxcount[460] = 6220;
	x_pi_ac_sumdiff_01_min[460] = -999.5;
	x_pi_ac_sumdiff_01_max[460] = -799.5;
	x_pi_ac_sumdiff_10_min[460] = -959.5;
	x_pi_ac_sumdiff_10_max[460] = -821.5;
	z_pi_ac_sumdiff_mode[460] = -265.5;
	z_pi_ac_sumdiff_maxcount[460] = 7907;
	z_pi_ac_sumdiff_01_min[460] = -401.5;
	z_pi_ac_sumdiff_01_max[460] = -199.5;
	z_pi_ac_sumdiff_10_min[460] = -335.5;
	z_pi_ac_sumdiff_10_max[460] = -225.5;
	x_r_sumdiff_mode[460] = -889.5;
	x_r_sumdiff_maxcount[460] = 673;
	x_r_sumdiff_01_min[460] = -1085.5;
	x_r_sumdiff_01_max[460] = -781.5;
	x_r_sumdiff_10_min[460] = -1031.5;
	x_r_sumdiff_10_max[460] = -803.5;
	z_r_sumdiff_mode[460] = -261.5;
	z_r_sumdiff_maxcount[460] = 689;
	z_r_sumdiff_01_min[460] = -467.5;
	z_r_sumdiff_01_max[460] = -183.5;
	z_r_sumdiff_10_min[460] = -411.5;
	z_r_sumdiff_10_max[460] = -197.5;
	x_gr_sumdiff_mode[460] = -855.5;
	x_gr_sumdiff_maxcount[460] = 95;
	x_gr_sumdiff_01_min[460] = -1081.5;
	x_gr_sumdiff_01_max[460] = -781.5;
	x_gr_sumdiff_10_min[460] = -1031.5;
	x_gr_sumdiff_10_max[460] = -811.5;
	z_gr_sumdiff_mode[460] = -249.5;
	z_gr_sumdiff_maxcount[460] = 105;
	z_gr_sumdiff_01_min[460] = -435.5;
	z_gr_sumdiff_01_max[460] = -189.5;
	z_gr_sumdiff_10_min[460] = -401.5;
	z_gr_sumdiff_10_max[460] = -199.5;

	x_all_sumdiff_mode[461] = -881.5;
	x_all_sumdiff_maxcount[461] = 4720;
	x_all_sumdiff_01_min[461] = -1041.5;
	x_all_sumdiff_01_max[461] = -793.5;
	x_all_sumdiff_10_min[461] = -969.5;
	x_all_sumdiff_10_max[461] = -817.5;
	z_all_sumdiff_mode[461] = -263.5;
	z_all_sumdiff_maxcount[461] = 5738;
	z_all_sumdiff_01_min[461] = -421.5;
	z_all_sumdiff_01_max[461] = -195.5;
	z_all_sumdiff_10_min[461] = -349.5;
	z_all_sumdiff_10_max[461] = -221.5;
	x_pi_ac_sumdiff_mode[461] = -881.5;
	x_pi_ac_sumdiff_maxcount[461] = 2729;
	x_pi_ac_sumdiff_01_min[461] = -999.5;
	x_pi_ac_sumdiff_01_max[461] = -801.5;
	x_pi_ac_sumdiff_10_min[461] = -959.5;
	x_pi_ac_sumdiff_10_max[461] = -823.5;
	z_pi_ac_sumdiff_mode[461] = -263.5;
	z_pi_ac_sumdiff_maxcount[461] = 3459;
	z_pi_ac_sumdiff_01_min[461] = -397.5;
	z_pi_ac_sumdiff_01_max[461] = -201.5;
	z_pi_ac_sumdiff_10_min[461] = -333.5;
	z_pi_ac_sumdiff_10_max[461] = -225.5;
	x_r_sumdiff_mode[461] = -903.5;
	x_r_sumdiff_maxcount[461] = 285;
	x_r_sumdiff_01_min[461] = -1083.5;
	x_r_sumdiff_01_max[461] = -779.5;
	x_r_sumdiff_10_min[461] = -1035.5;
	x_r_sumdiff_10_max[461] = -803.5;
	z_r_sumdiff_mode[461] = -253.5;
	z_r_sumdiff_maxcount[461] = 288;
	z_r_sumdiff_01_min[461] = -445.5;
	z_r_sumdiff_01_max[461] = -175.5;
	z_r_sumdiff_10_min[461] = -409.5;
	z_r_sumdiff_10_max[461] = -199.5;
	x_gr_sumdiff_mode[461] = -903.5;
	x_gr_sumdiff_maxcount[461] = 48;
	x_gr_sumdiff_01_min[461] = -1057.5;
	x_gr_sumdiff_01_max[461] = -797.5;
	x_gr_sumdiff_10_min[461] = -1015.5;
	x_gr_sumdiff_10_max[461] = -809.5;
	z_gr_sumdiff_mode[461] = -271.5;
	z_gr_sumdiff_maxcount[461] = 44;
	z_gr_sumdiff_01_min[461] = -441.5;
	z_gr_sumdiff_01_max[461] = -181.5;
	z_gr_sumdiff_10_min[461] = -401.5;
	z_gr_sumdiff_10_max[461] = -209.5;

	x_all_sumdiff_mode[462] = -881.5;
	x_all_sumdiff_maxcount[462] = 10629;
	x_all_sumdiff_01_min[462] = -1043.5;
	x_all_sumdiff_01_max[462] = -793.5;
	x_all_sumdiff_10_min[462] = -969.5;
	x_all_sumdiff_10_max[462] = -817.5;
	z_all_sumdiff_mode[462] = -267.5;
	z_all_sumdiff_maxcount[462] = 12949;
	z_all_sumdiff_01_min[462] = -427.5;
	z_all_sumdiff_01_max[462] = -197.5;
	z_all_sumdiff_10_min[462] = -351.5;
	z_all_sumdiff_10_max[462] = -221.5;
	x_pi_ac_sumdiff_mode[462] = -881.5;
	x_pi_ac_sumdiff_maxcount[462] = 6067;
	x_pi_ac_sumdiff_01_min[462] = -1001.5;
	x_pi_ac_sumdiff_01_max[462] = -795.5;
	x_pi_ac_sumdiff_10_min[462] = -961.5;
	x_pi_ac_sumdiff_10_max[462] = -819.5;
	z_pi_ac_sumdiff_mode[462] = -267.5;
	z_pi_ac_sumdiff_maxcount[462] = 7695;
	z_pi_ac_sumdiff_01_min[462] = -403.5;
	z_pi_ac_sumdiff_01_max[462] = -203.5;
	z_pi_ac_sumdiff_10_min[462] = -335.5;
	z_pi_ac_sumdiff_10_max[462] = -225.5;
	x_r_sumdiff_mode[462] = -871.5;
	x_r_sumdiff_maxcount[462] = 591;
	x_r_sumdiff_01_min[462] = -1099.5;
	x_r_sumdiff_01_max[462] = -779.5;
	x_r_sumdiff_10_min[462] = -1039.5;
	x_r_sumdiff_10_max[462] = -801.5;
	z_r_sumdiff_mode[462] = -313.5;
	z_r_sumdiff_maxcount[462] = 623;
	z_r_sumdiff_01_min[462] = -461.5;
	z_r_sumdiff_01_max[462] = -177.5;
	z_r_sumdiff_10_min[462] = -407.5;
	z_r_sumdiff_10_max[462] = -199.5;
	x_gr_sumdiff_mode[462] = -869.5;
	x_gr_sumdiff_maxcount[462] = 82;
	x_gr_sumdiff_01_min[462] = -1077.5;
	x_gr_sumdiff_01_max[462] = -783.5;
	x_gr_sumdiff_10_min[462] = -1039.5;
	x_gr_sumdiff_10_max[462] = -807.5;
	z_gr_sumdiff_mode[462] = -267.5;
	z_gr_sumdiff_maxcount[462] = 91;
	z_gr_sumdiff_01_min[462] = -457.5;
	z_gr_sumdiff_01_max[462] = -175.5;
	z_gr_sumdiff_10_min[462] = -397.5;
	z_gr_sumdiff_10_max[462] = -203.5;

	x_all_sumdiff_mode[463] = -881.5;
	x_all_sumdiff_maxcount[463] = 10844;
	x_all_sumdiff_01_min[463] = -1041.5;
	x_all_sumdiff_01_max[463] = -793.5;
	x_all_sumdiff_10_min[463] = -969.5;
	x_all_sumdiff_10_max[463] = -815.5;
	z_all_sumdiff_mode[463] = -267.5;
	z_all_sumdiff_maxcount[463] = 13224;
	z_all_sumdiff_01_min[463] = -429.5;
	z_all_sumdiff_01_max[463] = -193.5;
	z_all_sumdiff_10_min[463] = -351.5;
	z_all_sumdiff_10_max[463] = -219.5;
	x_pi_ac_sumdiff_mode[463] = -881.5;
	x_pi_ac_sumdiff_maxcount[463] = 6197;
	x_pi_ac_sumdiff_01_min[463] = -1003.5;
	x_pi_ac_sumdiff_01_max[463] = -795.5;
	x_pi_ac_sumdiff_10_min[463] = -961.5;
	x_pi_ac_sumdiff_10_max[463] = -819.5;
	z_pi_ac_sumdiff_mode[463] = -267.5;
	z_pi_ac_sumdiff_maxcount[463] = 7674;
	z_pi_ac_sumdiff_01_min[463] = -401.5;
	z_pi_ac_sumdiff_01_max[463] = -199.5;
	z_pi_ac_sumdiff_10_min[463] = -335.5;
	z_pi_ac_sumdiff_10_max[463] = -225.5;
	x_r_sumdiff_mode[463] = -873.5;
	x_r_sumdiff_maxcount[463] = 648;
	x_r_sumdiff_01_min[463] = -1095.5;
	x_r_sumdiff_01_max[463] = -781.5;
	x_r_sumdiff_10_min[463] = -1039.5;
	x_r_sumdiff_10_max[463] = -803.5;
	z_r_sumdiff_mode[463] = -261.5;
	z_r_sumdiff_maxcount[463] = 650;
	z_r_sumdiff_01_min[463] = -465.5;
	z_r_sumdiff_01_max[463] = -179.5;
	z_r_sumdiff_10_min[463] = -413.5;
	z_r_sumdiff_10_max[463] = -197.5;
	x_gr_sumdiff_mode[463] = -873.5;
	x_gr_sumdiff_maxcount[463] = 93;
	x_gr_sumdiff_01_min[463] = -1069.5;
	x_gr_sumdiff_01_max[463] = -777.5;
	x_gr_sumdiff_10_min[463] = -1027.5;
	x_gr_sumdiff_10_max[463] = -809.5;
	z_gr_sumdiff_mode[463] = -317.5;
	z_gr_sumdiff_maxcount[463] = 93;
	z_gr_sumdiff_01_min[463] = -455.5;
	z_gr_sumdiff_01_max[463] = -177.5;
	z_gr_sumdiff_10_min[463] = -397.5;
	z_gr_sumdiff_10_max[463] = -203.5;

	x_all_sumdiff_mode[464] = -885.5;
	x_all_sumdiff_maxcount[464] = 11760;
	x_all_sumdiff_01_min[464] = -1045.5;
	x_all_sumdiff_01_max[464] = -793.5;
	x_all_sumdiff_10_min[464] = -971.5;
	x_all_sumdiff_10_max[464] = -817.5;
	z_all_sumdiff_mode[464] = -269.5;
	z_all_sumdiff_maxcount[464] = 14303;
	z_all_sumdiff_01_min[464] = -433.5;
	z_all_sumdiff_01_max[464] = -193.5;
	z_all_sumdiff_10_min[464] = -355.5;
	z_all_sumdiff_10_max[464] = -221.5;
	x_pi_ac_sumdiff_mode[464] = -881.5;
	x_pi_ac_sumdiff_maxcount[464] = 6664;
	x_pi_ac_sumdiff_01_min[464] = -1005.5;
	x_pi_ac_sumdiff_01_max[464] = -797.5;
	x_pi_ac_sumdiff_10_min[464] = -965.5;
	x_pi_ac_sumdiff_10_max[464] = -819.5;
	z_pi_ac_sumdiff_mode[464] = -269.5;
	z_pi_ac_sumdiff_maxcount[464] = 8481;
	z_pi_ac_sumdiff_01_min[464] = -405.5;
	z_pi_ac_sumdiff_01_max[464] = -203.5;
	z_pi_ac_sumdiff_10_min[464] = -335.5;
	z_pi_ac_sumdiff_10_max[464] = -225.5;
	x_r_sumdiff_mode[464] = -905.5;
	x_r_sumdiff_maxcount[464] = 678;
	x_r_sumdiff_01_min[464] = -1097.5;
	x_r_sumdiff_01_max[464] = -779.5;
	x_r_sumdiff_10_min[464] = -1035.5;
	x_r_sumdiff_10_max[464] = -803.5;
	z_r_sumdiff_mode[464] = -311.5;
	z_r_sumdiff_maxcount[464] = 722;
	z_r_sumdiff_01_min[464] = -475.5;
	z_r_sumdiff_01_max[464] = -177.5;
	z_r_sumdiff_10_min[464] = -411.5;
	z_r_sumdiff_10_max[464] = -199.5;
	x_gr_sumdiff_mode[464] = -913.5;
	x_gr_sumdiff_maxcount[464] = 100;
	x_gr_sumdiff_01_min[464] = -1065.5;
	x_gr_sumdiff_01_max[464] = -795.5;
	x_gr_sumdiff_10_min[464] = -1027.5;
	x_gr_sumdiff_10_max[464] = -811.5;
	z_gr_sumdiff_mode[464] = -297.5;
	z_gr_sumdiff_maxcount[464] = 104;
	z_gr_sumdiff_01_min[464] = -453.5;
	z_gr_sumdiff_01_max[464] = -197.5;
	z_gr_sumdiff_10_min[464] = -397.5;
	z_gr_sumdiff_10_max[464] = -199.5;

	x_all_sumdiff_mode[465] = -885.5;
	x_all_sumdiff_maxcount[465] = 10450;
	x_all_sumdiff_01_min[465] = -1039.5;
	x_all_sumdiff_01_max[465] = -795.5;
	x_all_sumdiff_10_min[465] = -971.5;
	x_all_sumdiff_10_max[465] = -819.5;
	z_all_sumdiff_mode[465] = -265.5;
	z_all_sumdiff_maxcount[465] = 12742;
	z_all_sumdiff_01_min[465] = -427.5;
	z_all_sumdiff_01_max[465] = -195.5;
	z_all_sumdiff_10_min[465] = -355.5;
	z_all_sumdiff_10_max[465] = -223.5;
	x_pi_ac_sumdiff_mode[465] = -883.5;
	x_pi_ac_sumdiff_maxcount[465] = 6068;
	x_pi_ac_sumdiff_01_min[465] = -1001.5;
	x_pi_ac_sumdiff_01_max[465] = -797.5;
	x_pi_ac_sumdiff_10_min[465] = -961.5;
	x_pi_ac_sumdiff_10_max[465] = -819.5;
	z_pi_ac_sumdiff_mode[465] = -267.5;
	z_pi_ac_sumdiff_maxcount[465] = 7652;
	z_pi_ac_sumdiff_01_min[465] = -407.5;
	z_pi_ac_sumdiff_01_max[465] = -205.5;
	z_pi_ac_sumdiff_10_min[465] = -335.5;
	z_pi_ac_sumdiff_10_max[465] = -227.5;
	x_r_sumdiff_mode[465] = -889.5;
	x_r_sumdiff_maxcount[465] = 535;
	x_r_sumdiff_01_min[465] = -1075.5;
	x_r_sumdiff_01_max[465] = -783.5;
	x_r_sumdiff_10_min[465] = -1039.5;
	x_r_sumdiff_10_max[465] = -805.5;
	z_r_sumdiff_mode[465] = -319.5;
	z_r_sumdiff_maxcount[465] = 572;
	z_r_sumdiff_01_min[465] = -457.5;
	z_r_sumdiff_01_max[465] = -179.5;
	z_r_sumdiff_10_min[465] = -411.5;
	z_r_sumdiff_10_max[465] = -203.5;
	x_gr_sumdiff_mode[465] = -867.5;
	x_gr_sumdiff_maxcount[465] = 79;
	x_gr_sumdiff_01_min[465] = -1079.5;
	x_gr_sumdiff_01_max[465] = -789.5;
	x_gr_sumdiff_10_min[465] = -1021.5;
	x_gr_sumdiff_10_max[465] = -811.5;
	z_gr_sumdiff_mode[465] = -319.5;
	z_gr_sumdiff_maxcount[465] = 87;
	z_gr_sumdiff_01_min[465] = -447.5;
	z_gr_sumdiff_01_max[465] = -183.5;
	z_gr_sumdiff_10_min[465] = -409.5;
	z_gr_sumdiff_10_max[465] = -207.5;

	x_all_sumdiff_mode[466] = -877.5;
	x_all_sumdiff_maxcount[466] = 13115;
	x_all_sumdiff_01_min[466] = -1063.5;
	x_all_sumdiff_01_max[466] = -787.5;
	x_all_sumdiff_10_min[466] = -981.5;
	x_all_sumdiff_10_max[466] = -809.5;
	z_all_sumdiff_mode[466] = -265.5;
	z_all_sumdiff_maxcount[466] = 15918;
	z_all_sumdiff_01_min[466] = -451.5;
	z_all_sumdiff_01_max[466] = -183.5;
	z_all_sumdiff_10_min[466] = -367.5;
	z_all_sumdiff_10_max[466] = -207.5;
	x_pi_ac_sumdiff_mode[466] = -883.5;
	x_pi_ac_sumdiff_maxcount[466] = 6574;
	x_pi_ac_sumdiff_01_min[466] = -1031.5;
	x_pi_ac_sumdiff_01_max[466] = -791.5;
	x_pi_ac_sumdiff_10_min[466] = -967.5;
	x_pi_ac_sumdiff_10_max[466] = -815.5;
	z_pi_ac_sumdiff_mode[466] = -267.5;
	z_pi_ac_sumdiff_maxcount[466] = 8244;
	z_pi_ac_sumdiff_01_min[466] = -425.5;
	z_pi_ac_sumdiff_01_max[466] = -193.5;
	z_pi_ac_sumdiff_10_min[466] = -343.5;
	z_pi_ac_sumdiff_10_max[466] = -217.5;
	x_r_sumdiff_mode[466] = -853.5;
	x_r_sumdiff_maxcount[466] = 1775;
	x_r_sumdiff_01_min[466] = -1095.5;
	x_r_sumdiff_01_max[466] = -777.5;
	x_r_sumdiff_10_min[466] = -1019.5;
	x_r_sumdiff_10_max[466] = -801.5;
	z_r_sumdiff_mode[466] = -253.5;
	z_r_sumdiff_maxcount[466] = 1737;
	z_r_sumdiff_01_min[466] = -487.5;
	z_r_sumdiff_01_max[466] = -173.5;
	z_r_sumdiff_10_min[466] = -403.5;
	z_r_sumdiff_10_max[466] = -195.5;
	x_gr_sumdiff_mode[466] = -855.5;
	x_gr_sumdiff_maxcount[466] = 220;
	x_gr_sumdiff_01_min[466] = -1083.5;
	x_gr_sumdiff_01_max[466] = -787.5;
	x_gr_sumdiff_10_min[466] = -1015.5;
	x_gr_sumdiff_10_max[466] = -801.5;
	z_gr_sumdiff_mode[466] = -249.5;
	z_gr_sumdiff_maxcount[466] = 235;
	z_gr_sumdiff_01_min[466] = -471.5;
	z_gr_sumdiff_01_max[466] = -181.5;
	z_gr_sumdiff_10_min[466] = -393.5;
	z_gr_sumdiff_10_max[466] = -193.5;

	x_all_sumdiff_mode[473] = -907.5;
	x_all_sumdiff_maxcount[473] = 5015;
	x_all_sumdiff_01_min[473] = -1151.5;
	x_all_sumdiff_01_max[473] = -809.5;
	x_all_sumdiff_10_min[473] = -1067.5;
	x_all_sumdiff_10_max[473] = -827.5;
	z_all_sumdiff_mode[473] = -295.5;
	z_all_sumdiff_maxcount[473] = 5256;
	z_all_sumdiff_01_min[473] = -547.5;
	z_all_sumdiff_01_max[473] = -203.5;
	z_all_sumdiff_10_min[473] = -455.5;
	z_all_sumdiff_10_max[473] = -219.5;
	x_pi_ac_sumdiff_mode[473] = -913.5;
	x_pi_ac_sumdiff_maxcount[473] = 1720;
	x_pi_ac_sumdiff_01_min[473] = -1129.5;
	x_pi_ac_sumdiff_01_max[473] = -813.5;
	x_pi_ac_sumdiff_10_min[473] = -1055.5;
	x_pi_ac_sumdiff_10_max[473] = -833.5;
	z_pi_ac_sumdiff_mode[473] = -285.5;
	z_pi_ac_sumdiff_maxcount[473] = 1826;
	z_pi_ac_sumdiff_01_min[473] = -527.5;
	z_pi_ac_sumdiff_01_max[473] = -207.5;
	z_pi_ac_sumdiff_10_min[473] = -439.5;
	z_pi_ac_sumdiff_10_max[473] = -225.5;
	x_r_sumdiff_mode[473] = -907.5;
	x_r_sumdiff_maxcount[473] = 1217;
	x_r_sumdiff_01_min[473] = -1153.5;
	x_r_sumdiff_01_max[473] = -811.5;
	x_r_sumdiff_10_min[473] = -1079.5;
	x_r_sumdiff_10_max[473] = -825.5;
	z_r_sumdiff_mode[473] = -293.5;
	z_r_sumdiff_maxcount[473] = 1267;
	z_r_sumdiff_01_min[473] = -543.5;
	z_r_sumdiff_01_max[473] = -201.5;
	z_r_sumdiff_10_min[473] = -461.5;
	z_r_sumdiff_10_max[473] = -217.5;
	x_gr_sumdiff_mode[473] = -923.5;
	x_gr_sumdiff_maxcount[473] = 173;
	x_gr_sumdiff_01_min[473] = -1133.5;
	x_gr_sumdiff_01_max[473] = -819.5;
	x_gr_sumdiff_10_min[473] = -1057.5;
	x_gr_sumdiff_10_max[473] = -831.5;
	z_gr_sumdiff_mode[473] = -275.5;
	z_gr_sumdiff_maxcount[473] = 181;
	z_gr_sumdiff_01_min[473] = -531.5;
	z_gr_sumdiff_01_max[473] = -209.5;
	z_gr_sumdiff_10_min[473] = -435.5;
	z_gr_sumdiff_10_max[473] = -217.5;

	x_all_sumdiff_mode[474] = -909.5;
	x_all_sumdiff_maxcount[474] = 18546;
	x_all_sumdiff_01_min[474] = -1151.5;
	x_all_sumdiff_01_max[474] = -811.5;
	x_all_sumdiff_10_min[474] = -1069.5;
	x_all_sumdiff_10_max[474] = -827.5;
	z_all_sumdiff_mode[474] = -289.5;
	z_all_sumdiff_maxcount[474] = 18997;
	z_all_sumdiff_01_min[474] = -557.5;
	z_all_sumdiff_01_max[474] = -203.5;
	z_all_sumdiff_10_min[474] = -459.5;
	z_all_sumdiff_10_max[474] = -219.5;
	x_pi_ac_sumdiff_mode[474] = -909.5;
	x_pi_ac_sumdiff_maxcount[474] = 6505;
	x_pi_ac_sumdiff_01_min[474] = -1137.5;
	x_pi_ac_sumdiff_01_max[474] = -815.5;
	x_pi_ac_sumdiff_10_min[474] = -1055.5;
	x_pi_ac_sumdiff_10_max[474] = -833.5;
	z_pi_ac_sumdiff_mode[474] = -285.5;
	z_pi_ac_sumdiff_maxcount[474] = 6697;
	z_pi_ac_sumdiff_01_min[474] = -533.5;
	z_pi_ac_sumdiff_01_max[474] = -205.5;
	z_pi_ac_sumdiff_10_min[474] = -439.5;
	z_pi_ac_sumdiff_10_max[474] = -223.5;
	x_r_sumdiff_mode[474] = -907.5;
	x_r_sumdiff_maxcount[474] = 4472;
	x_r_sumdiff_01_min[474] = -1155.5;
	x_r_sumdiff_01_max[474] = -809.5;
	x_r_sumdiff_10_min[474] = -1077.5;
	x_r_sumdiff_10_max[474] = -825.5;
	z_r_sumdiff_mode[474] = -293.5;
	z_r_sumdiff_maxcount[474] = 4566;
	z_r_sumdiff_01_min[474] = -565.5;
	z_r_sumdiff_01_max[474] = -203.5;
	z_r_sumdiff_10_min[474] = -467.5;
	z_r_sumdiff_10_max[474] = -217.5;
	x_gr_sumdiff_mode[474] = -939.5;
	x_gr_sumdiff_maxcount[474] = 637;
	x_gr_sumdiff_01_min[474] = -1135.5;
	x_gr_sumdiff_01_max[474] = -815.5;
	x_gr_sumdiff_10_min[474] = -1063.5;
	x_gr_sumdiff_10_max[474] = -825.5;
	z_gr_sumdiff_mode[474] = -293.5;
	z_gr_sumdiff_maxcount[474] = 641;
	z_gr_sumdiff_01_min[474] = -549.5;
	z_gr_sumdiff_01_max[474] = -203.5;
	z_gr_sumdiff_10_min[474] = -457.5;
	z_gr_sumdiff_10_max[474] = -217.5;

	x_all_sumdiff_mode[491] = -927.5;
	x_all_sumdiff_maxcount[491] = 3467;
	x_all_sumdiff_01_min[491] = -1117.5;
	x_all_sumdiff_01_max[491] = -821.5;
	x_all_sumdiff_10_min[491] = -1037.5;
	x_all_sumdiff_10_max[491] = -857.5;
	z_all_sumdiff_mode[491] = -295.5;
	z_all_sumdiff_maxcount[491] = 3774;
	z_all_sumdiff_01_min[491] = -499.5;
	z_all_sumdiff_01_max[491] = -219.5;
	z_all_sumdiff_10_min[491] = -417.5;
	z_all_sumdiff_10_max[491] = -251.5;
	x_pi_ac_sumdiff_mode[491] = -925.5;
	x_pi_ac_sumdiff_maxcount[491] = 2084;
	x_pi_ac_sumdiff_01_min[491] = -1099.5;
	x_pi_ac_sumdiff_01_max[491] = -837.5;
	x_pi_ac_sumdiff_10_min[491] = -1021.5;
	x_pi_ac_sumdiff_10_max[491] = -871.5;
	z_pi_ac_sumdiff_mode[491] = -303.5;
	z_pi_ac_sumdiff_maxcount[491] = 2206;
	z_pi_ac_sumdiff_01_min[491] = -493.5;
	z_pi_ac_sumdiff_01_max[491] = -233.5;
	z_pi_ac_sumdiff_10_min[491] = -401.5;
	z_pi_ac_sumdiff_10_max[491] = -255.5;
	x_r_sumdiff_mode[491] = -935.5;
	x_r_sumdiff_maxcount[491] = 315;
	x_r_sumdiff_01_min[491] = -1123.5;
	x_r_sumdiff_01_max[491] = -811.5;
	x_r_sumdiff_10_min[491] = -1059.5;
	x_r_sumdiff_10_max[491] = -831.5;
	z_r_sumdiff_mode[491] = -313.5;
	z_r_sumdiff_maxcount[491] = 329;
	z_r_sumdiff_01_min[491] = -491.5;
	z_r_sumdiff_01_max[491] = -207.5;
	z_r_sumdiff_10_min[491] = -439.5;
	z_r_sumdiff_10_max[491] = -225.5;
	x_gr_sumdiff_mode[491] = -931.5;
	x_gr_sumdiff_maxcount[491] = 63;
	x_gr_sumdiff_01_min[491] = -1085.5;
	x_gr_sumdiff_01_max[491] = -813.5;
	x_gr_sumdiff_10_min[491] = -1051.5;
	x_gr_sumdiff_10_max[491] = -845.5;
	z_gr_sumdiff_mode[491] = -343.5;
	z_gr_sumdiff_maxcount[491] = 67;
	z_gr_sumdiff_01_min[491] = -475.5;
	z_gr_sumdiff_01_max[491] = -217.5;
	z_gr_sumdiff_10_min[491] = -437.5;
	z_gr_sumdiff_10_max[491] = -237.5;

	x_all_sumdiff_mode[497] = -923.5;
	x_all_sumdiff_maxcount[497] = 1122;
	x_all_sumdiff_01_min[497] = -1097.5;
	x_all_sumdiff_01_max[497] = -831.5;
	x_all_sumdiff_10_min[497] = -1029.5;
	x_all_sumdiff_10_max[497] = -863.5;
	z_all_sumdiff_mode[497] = -295.5;
	z_all_sumdiff_maxcount[497] = 1207;
	z_all_sumdiff_01_min[497] = -485.5;
	z_all_sumdiff_01_max[497] = -223.5;
	z_all_sumdiff_10_min[497] = -409.5;
	z_all_sumdiff_10_max[497] = -251.5;
	x_pi_ac_sumdiff_mode[497] = -927.5;
	x_pi_ac_sumdiff_maxcount[497] = 642;
	x_pi_ac_sumdiff_01_min[497] = -1081.5;
	x_pi_ac_sumdiff_01_max[497] = -847.5;
	x_pi_ac_sumdiff_10_min[497] = -1009.5;
	x_pi_ac_sumdiff_10_max[497] = -871.5;
	z_pi_ac_sumdiff_mode[497] = -295.5;
	z_pi_ac_sumdiff_maxcount[497] = 713;
	z_pi_ac_sumdiff_01_min[497] = -485.5;
	z_pi_ac_sumdiff_01_max[497] = -235.5;
	z_pi_ac_sumdiff_10_min[497] = -391.5;
	z_pi_ac_sumdiff_10_max[497] = -255.5;
	x_r_sumdiff_mode[497] = -949.5;
	x_r_sumdiff_maxcount[497] = 107;
	x_r_sumdiff_01_min[497] = -1105.5;
	x_r_sumdiff_01_max[497] = -827.5;
	x_r_sumdiff_10_min[497] = -1049.5;
	x_r_sumdiff_10_max[497] = -833.5;
	z_r_sumdiff_mode[497] = -307.5;
	z_r_sumdiff_maxcount[497] = 111;
	z_r_sumdiff_01_min[497] = -465.5;
	z_r_sumdiff_01_max[497] = -211.5;
	z_r_sumdiff_10_min[497] = -425.5;
	z_r_sumdiff_10_max[497] = -231.5;
	x_gr_sumdiff_mode[497] = -957.5;
	x_gr_sumdiff_maxcount[497] = 22;
	x_gr_sumdiff_01_min[497] = -1061.5;
	x_gr_sumdiff_01_max[497] = -837.5;
	x_gr_sumdiff_10_min[497] = -1027.5;
	x_gr_sumdiff_10_max[497] = -861.5;
	z_gr_sumdiff_mode[497] = -325.5;
	z_gr_sumdiff_maxcount[497] = 24;
	z_gr_sumdiff_01_min[497] = -425.5;
	z_gr_sumdiff_01_max[497] = -247.5;
	z_gr_sumdiff_10_min[497] = -409.5;
	z_gr_sumdiff_10_max[497] = -247.5;

	x_all_sumdiff_mode[498] = -921.5;
	x_all_sumdiff_maxcount[498] = 5505;
	x_all_sumdiff_01_min[498] = -1109.5;
	x_all_sumdiff_01_max[498] = -825.5;
	x_all_sumdiff_10_min[498] = -1033.5;
	x_all_sumdiff_10_max[498] = -863.5;
	z_all_sumdiff_mode[498] = -295.5;
	z_all_sumdiff_maxcount[498] = 6083;
	z_all_sumdiff_01_min[498] = -493.5;
	z_all_sumdiff_01_max[498] = -221.5;
	z_all_sumdiff_10_min[498] = -413.5;
	z_all_sumdiff_10_max[498] = -251.5;
	x_pi_ac_sumdiff_mode[498] = -921.5;
	x_pi_ac_sumdiff_maxcount[498] = 3281;
	x_pi_ac_sumdiff_01_min[498] = -1097.5;
	x_pi_ac_sumdiff_01_max[498] = -845.5;
	x_pi_ac_sumdiff_10_min[498] = -1019.5;
	x_pi_ac_sumdiff_10_max[498] = -871.5;
	z_pi_ac_sumdiff_mode[498] = -301.5;
	z_pi_ac_sumdiff_maxcount[498] = 3525;
	z_pi_ac_sumdiff_01_min[498] = -487.5;
	z_pi_ac_sumdiff_01_max[498] = -235.5;
	z_pi_ac_sumdiff_10_min[498] = -397.5;
	z_pi_ac_sumdiff_10_max[498] = -255.5;
	x_r_sumdiff_mode[498] = -937.5;
	x_r_sumdiff_maxcount[498] = 459;
	x_r_sumdiff_01_min[498] = -1107.5;
	x_r_sumdiff_01_max[498] = -805.5;
	x_r_sumdiff_10_min[498] = -1065.5;
	x_r_sumdiff_10_max[498] = -831.5;
	z_r_sumdiff_mode[498] = -325.5;
	z_r_sumdiff_maxcount[498] = 491;
	z_r_sumdiff_01_min[498] = -489.5;
	z_r_sumdiff_01_max[498] = -207.5;
	z_r_sumdiff_10_min[498] = -439.5;
	z_r_sumdiff_10_max[498] = -229.5;
	x_gr_sumdiff_mode[498] = -939.5;
	x_gr_sumdiff_maxcount[498] = 86;
	x_gr_sumdiff_01_min[498] = -1103.5;
	x_gr_sumdiff_01_max[498] = -817.5;
	x_gr_sumdiff_10_min[498] = -1053.5;
	x_gr_sumdiff_10_max[498] = -841.5;
	z_gr_sumdiff_mode[498] = -303.5;
	z_gr_sumdiff_maxcount[498] = 93;
	z_gr_sumdiff_01_min[498] = -479.5;
	z_gr_sumdiff_01_max[498] = -217.5;
	z_gr_sumdiff_10_min[498] = -437.5;
	z_gr_sumdiff_10_max[498] = -233.5;

	x_all_sumdiff_mode[499] = -921.5;
	x_all_sumdiff_maxcount[499] = 1880;
	x_all_sumdiff_01_min[499] = -1111.5;
	x_all_sumdiff_01_max[499] = -815.5;
	x_all_sumdiff_10_min[499] = -1047.5;
	x_all_sumdiff_10_max[499] = -843.5;
	z_all_sumdiff_mode[499] = -299.5;
	z_all_sumdiff_maxcount[499] = 2122;
	z_all_sumdiff_01_min[499] = -509.5;
	z_all_sumdiff_01_max[499] = -215.5;
	z_all_sumdiff_10_min[499] = -417.5;
	z_all_sumdiff_10_max[499] = -241.5;
	x_pi_ac_sumdiff_mode[499] = -921.5;
	x_pi_ac_sumdiff_maxcount[499] = 1058;
	x_pi_ac_sumdiff_01_min[499] = -1107.5;
	x_pi_ac_sumdiff_01_max[499] = -833.5;
	x_pi_ac_sumdiff_10_min[499] = -1025.5;
	x_pi_ac_sumdiff_10_max[499] = -863.5;
	z_pi_ac_sumdiff_mode[499] = -299.5;
	z_pi_ac_sumdiff_maxcount[499] = 1163;
	z_pi_ac_sumdiff_01_min[499] = -489.5;
	z_pi_ac_sumdiff_01_max[499] = -223.5;
	z_pi_ac_sumdiff_10_min[499] = -401.5;
	z_pi_ac_sumdiff_10_max[499] = -255.5;
	x_r_sumdiff_mode[499] = -949.5;
	x_r_sumdiff_maxcount[499] = 261;
	x_r_sumdiff_01_min[499] = -1111.5;
	x_r_sumdiff_01_max[499] = -809.5;
	x_r_sumdiff_10_min[499] = -1063.5;
	x_r_sumdiff_10_max[499] = -831.5;
	z_r_sumdiff_mode[499] = -319.5;
	z_r_sumdiff_maxcount[499] = 273;
	z_r_sumdiff_01_min[499] = -525.5;
	z_r_sumdiff_01_max[499] = -201.5;
	z_r_sumdiff_10_min[499] = -455.5;
	z_r_sumdiff_10_max[499] = -223.5;
	x_gr_sumdiff_mode[499] = -955.5;
	x_gr_sumdiff_maxcount[499] = 51;
	x_gr_sumdiff_01_min[499] = -1081.5;
	x_gr_sumdiff_01_max[499] = -815.5;
	x_gr_sumdiff_10_min[499] = -1053.5;
	x_gr_sumdiff_10_max[499] = -835.5;
	z_gr_sumdiff_mode[499] = -307.5;
	z_gr_sumdiff_maxcount[499] = 56;
	z_gr_sumdiff_01_min[499] = -481.5;
	z_gr_sumdiff_01_max[499] = -201.5;
	z_gr_sumdiff_10_min[499] = -433.5;
	z_gr_sumdiff_10_max[499] = -223.5;

	x_all_sumdiff_mode[509] = -917.5;
	x_all_sumdiff_maxcount[509] = 11458;
	x_all_sumdiff_01_min[509] = -1143.5;
	x_all_sumdiff_01_max[509] = -811.5;
	x_all_sumdiff_10_min[509] = -1071.5;
	x_all_sumdiff_10_max[509] = -825.5;
	z_all_sumdiff_mode[509] = -291.5;
	z_all_sumdiff_maxcount[509] = 12170;
	z_all_sumdiff_01_min[509] = -541.5;
	z_all_sumdiff_01_max[509] = -203.5;
	z_all_sumdiff_10_min[509] = -455.5;
	z_all_sumdiff_10_max[509] = -219.5;
	x_pi_ac_sumdiff_mode[509] = -917.5;
	x_pi_ac_sumdiff_maxcount[509] = 3103;
	x_pi_ac_sumdiff_01_min[509] = -1133.5;
	x_pi_ac_sumdiff_01_max[509] = -813.5;
	x_pi_ac_sumdiff_10_min[509] = -1057.5;
	x_pi_ac_sumdiff_10_max[509] = -833.5;
	z_pi_ac_sumdiff_mode[509] = -297.5;
	z_pi_ac_sumdiff_maxcount[509] = 3298;
	z_pi_ac_sumdiff_01_min[509] = -521.5;
	z_pi_ac_sumdiff_01_max[509] = -205.5;
	z_pi_ac_sumdiff_10_min[509] = -445.5;
	z_pi_ac_sumdiff_10_max[509] = -225.5;
	x_r_sumdiff_mode[509] = -909.5;
	x_r_sumdiff_maxcount[509] = 4591;
	x_r_sumdiff_01_min[509] = -1147.5;
	x_r_sumdiff_01_max[509] = -811.5;
	x_r_sumdiff_10_min[509] = -1077.5;
	x_r_sumdiff_10_max[509] = -825.5;
	z_r_sumdiff_mode[509] = -299.5;
	z_r_sumdiff_maxcount[509] = 4793;
	z_r_sumdiff_01_min[509] = -549.5;
	z_r_sumdiff_01_max[509] = -203.5;
	z_r_sumdiff_10_min[509] = -463.5;
	z_r_sumdiff_10_max[509] = -219.5;
	x_gr_sumdiff_mode[509] = -917.5;
	x_gr_sumdiff_maxcount[509] = 851;
	x_gr_sumdiff_01_min[509] = -1133.5;
	x_gr_sumdiff_01_max[509] = -811.5;
	x_gr_sumdiff_10_min[509] = -1071.5;
	x_gr_sumdiff_10_max[509] = -827.5;
	z_gr_sumdiff_mode[509] = -295.5;
	z_gr_sumdiff_maxcount[509] = 900;
	z_gr_sumdiff_01_min[509] = -523.5;
	z_gr_sumdiff_01_max[509] = -201.5;
	z_gr_sumdiff_10_min[509] = -455.5;
	z_gr_sumdiff_10_max[509] = -219.5;
	
}

/*
void set_of_runs::init_xsumdiff_min()
{
	double arr[517] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1080.0, 0.0, 0.0, 0.0, 0.0, -1050.0, -1050.0, -1050.0, -1050.0, -1050.0, -1050.0, 0.0, 0.0, 0.0, 0.0, -1150.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1150.0, -1300.0, -1340.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1270.0, 0.0, -1300.0, -1300.0, -1300.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1300.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1300.0, 0.0, -1100.0, -1100.0, 0.0, 0.0, 0.0, 0.0, -1100.0, -1100.0, -1100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1100.0, -1100.0, -1100.0, 0.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, 0.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, -1100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1100.0, -1100.0, 0.0, -1100.0, -1120.0, 0.0, 0.0, 0.0, -1100.0, -1100.0, 0.0, 0.0, 0.0, 0.0, -1120.0, -1120.0, -1120.0, -1120.0, -1120.0, -1120.0, -1140.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1160.0, -1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1120.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1120.0, -1120.0, -1120.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1160.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
	for (int i=0; i <517; i++)
	{
		x_sumdiff_min[i] = arr[i];
	}
}
void set_of_runs::init_xsumdiff_max()
{
	double arr[517] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -840.0, 0.0, 0.0, 0.0, 0.0, -840.0, -840.0, -840.0, -840.0, -840.0, -840.0, 0.0, 0.0, 0.0, 0.0, -830.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -840.0, -910.0, -910.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -890.0, 0.0, -980.0, -980.0, -980.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -980.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1000.0, 0.0, -810.0, -810.0, 0.0, 0.0, 0.0, 0.0, -810.0, -810.0, -810.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -810.0, -800.0, -810.0, 0.0, -810.0, -810.0, -810.0, -810.0, -810.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -810.0, -810.0, -810.0, -810.0, -810.0, -810.0, -810.0, -810.0, -810.0, -810.0, -810.0, 0.0, -810.0, -810.0, -810.0, -810.0, -810.0, -810.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -810.0, -810.0, 0.0, -810.0, -770.0, 0.0, 0.0, 0.0, -770.0, -770.0, 0.0, 0.0, 0.0, 0.0, -770.0, -770.0, -770.0, -770.0, -770.0, -770.0, -770.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -780.0, -790.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -770.0, 0.0, 0.0, 0.0, 0.0, 0.0, -770.0, -770.0, -800.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -790.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
	for (int i=0; i <517; i++)
	{
		x_sumdiff_max[i] = arr[i];
	}
}
void set_of_runs::init_zsumdiff_min()
{
	double arr[517] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -470.0, 0.0, 0.0, 0.0, 0.0, -470.0, -470.0, -470.0, -470.0, -450.0, -450.0, 0.0, 0.0, 0.0, 0.0, -530.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -530.0, -700.0, -740.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -670.0, 0.0, -660.0, -660.0, -660.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -660.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -660.0, 0.0, -500.0, -500.0, 0.0, 0.0, 0.0, 0.0, -500.0, -500.0, -500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -500.0, -500.0, -500.0, 0.0, -500.0, -500.0, -500.0, -500.0, -500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -500.0, -500.0, -500.0, -500.0, -500.0, -500.0, -500.0, -500.0, -500.0, -500.0, -500.0, 0.0, -500.0, -500.0, -500.0, -500.0, -500.0, -500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -500.0, -500.0, 0.0, -500.0, -530.0, 0.0, 0.0, 0.0, -500.0, -500.0, 0.0, 0.0, 0.0, 0.0, -500.0, -500.0, -500.0, -500.0, -500.0, -500.0, -540.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -550.0, -600.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -500.0, 0.0, 0.0, 0.0, 0.0, 0.0, -500.0, -500.0, -500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -580.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
	for (int i=0; i <517; i++)
	{
		z_sumdiff_min[i] = arr[i];
	}
}
void set_of_runs::init_zsumdiff_max()
{
	double arr[517] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -230.0, 0.0, 0.0, 0.0, 0.0, -230.0, -220.0, -230.0, -220.0, -230.0, -230.0, 0.0, 0.0, 0.0, 0.0, -230.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -230.0, -320.0, -320.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -310.0, 0.0, -380.0, -370.0, -380.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -390.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -400.0, 0.0, -200.0, -200.0, 0.0, 0.0, 0.0, 0.0, -200.0, -200.0, -200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -200.0, -200.0, -200.0, 0.0, -200.0, -200.0, -200.0, -200.0, -200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -200.0, -200.0, -200.0, -200.0, -200.0, -200.0, -200.0, -200.0, -200.0, -200.0, -200.0, 0.0, -200.0, -200.0, -200.0, -200.0, -200.0, -200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -200.0, -200.0, 0.0, -200.0, -170.0, 0.0, 0.0, 0.0, -170.0, -170.0, 0.0, 0.0, 0.0, 0.0, -170.0, -170.0, -170.0, -170.0, -170.0, -170.0, -160.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -180.0, -180.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -170.0, 0.0, 0.0, 0.0, 0.0, 0.0, -170.0, -170.0, -200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -170.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
	for (int i=0; i <517; i++)
	{
		z_sumdiff_max[i] = arr[i];
	}
}
*/

void set_of_runs::init_goodelectron()
{
	for(int i=0; i<n_runs; i++)
	{
		good_electron[i] = false;
	}
	
	good_electron[314] = true;
	good_electron[362] = true;
	good_electron[363] = true;
	
	for(int i=383; i<=386; i++)
	{
		good_electron[i] = true;
	}
	good_electron[393] = true;
	good_electron[420] = true;
	for(int i=428; i<=437; i++)
	{
		good_electron[i] = true;
	}	
	for(int i=440; i<=445; i++)
	{
		good_electron[i] = true;
	}	
	for(int i=476; i<=489; i++)
	{
		good_electron[i] = true;
	}	
	for(int i=502; i<=505; i++)
	{
		good_electron[i] = true;
	}	
	good_electron[510] = true;
	good_electron[513] = true;
}

void set_of_runs::init_goodrecoil()
{
	for(int i=0; i<n_runs; i++)
	{
		good_recoil[i] = false;
	}	
	good_recoil[303] = true;
	good_recoil[308] = true;
	good_recoil[309] = true;
	good_recoil[310] = true;
	good_recoil[311] = true;
	good_recoil[312] = true;
	good_recoil[313] = true;
	good_recoil[318] = true;
	good_recoil[326] = true;
	good_recoil[327] = true;
	good_recoil[328] = true;
	good_recoil[340] = true;
	good_recoil[342] = true;
	good_recoil[343] = true;
	good_recoil[361] = true;
	good_recoil[368] = true;
	good_recoil[370] = true;
	good_recoil[371] = true;
	good_recoil[376] = true;
	good_recoil[377] = true;
	good_recoil[378] = true;  // really??  is this ok??
	good_recoil[394] = true;
	good_recoil[395] = true;
	good_recoil[396] = true;
	for(int i=398; i<=402; i++)
	{
		good_recoil[i] = true;
	}
	for(int i=409; i<=419; i++)
	{
		good_recoil[i] = true;
	}
	for(int i=421; i<=426; i++)
	{
		good_recoil[i] = true;
	}
	good_recoil[446] = true;
	good_recoil[447] = true;
	good_recoil[449] = true;
	good_recoil[450] = true;
	good_recoil[454] = true;
	good_recoil[455] = true;
	for(int i=460; i<=466; i++)
	{
		good_recoil[i] = true;
	}
	good_recoil[473] = true;
	good_recoil[474] = true;
	good_recoil[491] = true;
	good_recoil[497] = true;
	good_recoil[498] = true;
	good_recoil[499] = true;
	good_recoil[509] = true;
}

void set_of_runs::init_ElectricField()
{
	for(int i=0; i<n_runs; i++)
	{
		ElectricField[i] = 0.0;
	}
	
	for(int i=298; i<=313; i++)
	{
		ElectricField[i] = 395.0;
	}
	ElectricField[314] = 66.7;
	for(int i=316; i<=361; i++)
	{
		ElectricField[i] = 395.0;
	}
	for(int i=362; i<=367; i++)
	{
		ElectricField[i] = 66.7;
	}
	for(int i=368; i<=378; i++)
	{
		ElectricField[i] = 395.0;
	}
	for(int i=379; i<=393; i++)
	{
		ElectricField[i] = 66.7;
	}
	for(int i=394; i<=405; i++)
	{
		ElectricField[i] = 395.0;
	}

	for(int i=409; i<=419; i++)
	{
		ElectricField[i] = 535.0;
	}
	ElectricField[420] = 66.7;
	for(int i=421; i<=426; i++)
	{
		ElectricField[i] = 535.0;
	}
	for(int i=427; i<=445; i++)
	{
		ElectricField[i] = 150.0;
	}
	for(int i=446; i<=449; i++)
	{
		ElectricField[i] = 535.0;
	}
	for(int i=450; i<=457; i++)
	{
		ElectricField[i] = 395.0;
	}
	for(int i=459; i<=475; i++)
	{
		ElectricField[i] = 415.0;
	}
	for(int i=476; i<=489; i++)
	{
		ElectricField[i] = 150.0;
	}
	for(int i=491; i<=499; i++)
	{
		ElectricField[i] = 415.0;
	}
	for(int i=502; i<=507; i++)
	{
		ElectricField[i] = 150.0;
	}
	ElectricField[509] = 415.0;
	for(int i=510; i<=516; i++)
	{
		ElectricField[i] = 150.0;
	}
}

void set_of_runs::init_usable()
{
	for(int i=0; i<n_runs; i++)
	{
		usable[i] = true;
		if(/*i==361 || i==368 || i==370 || i==371*/ i<=371 || i==497 \
		|| (good_recoil[i]==false && good_electron[i] == false))
		{
			usable[i] = false;
		}
	}
}

void set_of_runs::init_iqdc()
{
	for(int i=0; i<n_runs; i++)
	{
		// Defaults:
		iqdc_min[i] = 0.0;
		iqdc_max[i] = 500.0;
		
		if(i>=376 && i<=449)
		{
			iqdc_min[i] = 192;
			iqdc_max[i] = 280;
		}
	}
}

set_of_runs::set_of_runs()
{
	// Compton Edge.
	for(int i=0; i<361; i++)
	{
		compton_edge_ok[i] = false;
	}	
	for(int i=361; i<n_runs; i++)
	{
		compton_edge_ok[i] = true;
	}
	
	// OP Delay.
	for(int i=0; i<=449; i++)
	{
		OP_Delay[i] = 300.0;
	}
	for(int i=450; i<=477; i++)
	{
		OP_Delay[i] = 700.0;
	}
	for(int i=478; i<n_runs; i++)
	{
		OP_Delay[i] = 400.0;
	}
	//
	init_ElectricField();
	init_sumdiffs();
//	init_sumdiffs_old();
	init_goodrecoil();
	init_goodelectron();
	init_iqdc();
	init_usable();
	init_unixtime();
	
//	set_sumdiff_threshold(1);  
//	// Do that by default, just to make sure x_sumdiff_min[] etc are initialized.
	
	return;
}







