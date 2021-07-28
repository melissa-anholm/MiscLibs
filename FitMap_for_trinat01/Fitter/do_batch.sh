
#        zoom BS  SS  BG    ind sig bbr  thr  E_min(400) E_max(4800)  sigE_t sigE_b  namestub(optional)


#        zoom BS  SS  BG    ind sig bbr  thr  E_min(400) E_max(4800)  namestub(optional)
#        zoom BS  SS  BG    ind sig bbr  thr  namestub(optional)

#./doasym 3    1.0 1.0 1.0    0   3.0 15.5 50

printf '%s%s\n' "Default Params -- "; 
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800   0   0

printf '%s%s\n' "Testing E_scint range usage -- "; 
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  500  4800   0   0
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  300  4800   0   0
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  600  4800   0   0

./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  500  4700   0   0
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  500  4600   0   0
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  500  4900   0   0
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  500  5000   0   0


printf '%s%s\n' "Testing Scintillator Calibration -- "; 
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800   0   1
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800   1   0
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800   1   1
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800   0  -1
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800  -1   0
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800  -1  -1
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800   1  -1
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 50  400  4800  -1   1

# background
printf '%s%s\n' "Testing Background -- "; 
./doasym 7    1.0 1.0 0.5    0   3.0 15.5 50  400  4800   0   0
./doasym 7    1.0 1.0 2.0    0   3.0 15.5 50  400  4800   0   0

## backscatter:
#printf '%s%s\n' "Testing Backscatter -- "; 
#./doasym 7    1.051 1.0 1.0  0   3.0 15.5 50
#./doasym 7    0.949 1.0 1.0  0   3.0 15.5 50
## sidescatter:
#printf '%s%s\n' "Testing Sidescatter -- "; 
#./doasym 7    1.0 1.10 1.0   0   3.0 15.5 50
#./doasym 7    1.0 0.90 1.0   0   3.0 15.5 50
## backscatter and sidescatter together, 100% correlation:
printf '%s%s\n' "Testing Backscatter and Sidescatter Together -- "; 
./doasym 7    1.051 1.10 1.0  0   3.0 15.5 50  400  4800   0   0
./doasym 7    0.949 0.90 1.0  0   3.0 15.5 50  400  4800   0   0

# bb1 snr index:
printf '%s%s\n' "Testing BB1 SNR Index -- "; 
./doasym 7    1.0 1.0 1.0    1   3.0 15.5 50  400  4800   0   0
./doasym 7    1.0 1.0 1.0    2   3.0 15.5 50  400  4800   0   0
./doasym 7    1.0 1.0 1.0    3   3.0 15.5 50  400  4800   0   0


# bb1 sigma agreement:
printf '%s%s\n' "Testing BB1 Sigma Agreement -- "; 
./doasym 7   1.0 1.0 1.0    0   2.0 15.5 50  400  4800   0   0
./doasym 7   1.0 1.0 1.0    0   4.0 15.5 50  400  4800   0   0

# bb1 maxr:
printf '%s%s\n' "Testing BB1 Max. Radius -- "; 
./doasym 7   1.0 1.0 1.0    0   3.0 12.5 50  400  4800   0   0
./doasym 7   1.0 1.0 1.0    0   3.0 13.5 50  400  4800   0   0
./doasym 7   1.0 1.0 1.0    0   3.0 17.5 50  400  4800   0   0
./doasym 7   1.0 1.0 1.0    0   3.0 18.5 50  400  4800   0   0

# bb1 energy threshold:
printf '%s%s\n' "Testing BB1 Energy Threshold -- "; 
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 40  400  4800   0   0
./doasym 7    1.0 1.0 1.0    0   3.0 15.5 60  400  4800   0   0

