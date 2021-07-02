for z in {1,4,5}
do
echo "z = " $z 

#        zoom BS  SS  BG    ind sig bbr  thr  namestub(optional)
./doasym z    1.0 1.0 1.0   0   3.0 15.5 50

# background
printf '%s%s\n' "Testing Background -- "; 
./doasym z    1.0 1.0 0.5   0   3.0 15.5 50
./doasym z    1.0 1.0 2.0   0   3.0 15.5 50

# backscatter:
printf '%s%s\n' "Testing Backscatter -- "; 
./doasym z    1.051 1.0 1.0 0   3.0 15.5 50
./doasym z    0.949 1.0 1.0 0   3.0 15.5 50
# sidescatter:
printf '%s%s\n' "Testing Sidescatter -- "; 
./doasym z    1.0 1.10 1.0  0   3.0 15.5 50
./doasym z    1.0 0.90 1.0  0   3.0 15.5 50
# backscatter and sidescatter together, 100% correlation:
printf '%s%s\n' "Testing Backscatter and Sidescatter Together -- "; 
./doasym z    1.051 1.10 1.0 0   3.0 15.5 50
./doasym z    0.949 0.90 1.0 0   3.0 15.5 50


# bb1 snr index:
printf '%s%s\n' "Testing BB1 SNR Index -- "; 
./doasym z    1.0 1.0 1.0   1   3.0 15.5 50
./doasym z    1.0 1.0 1.0   3   3.0 15.5 50

# bb1 sigma agreement:
printf '%s%s\n' "Testing BB1 Sigma Agreement -- "; 
./doasym z   1.0 1.0 1.0   0   2.0 15.5 50
./doasym z   1.0 1.0 1.0   0   4.0 15.5 50

# bb1 maxr:
printf '%s%s\n' "Testing BB1 Max. Radius -- "; 
./doasym z   1.0 1.0 1.0   0   3.0 12.5 50
./doasym z   1.0 1.0 1.0   0   3.0 18.5 50

# bb1 energy threshold:
printf '%s%s\n' "Testing BB1 Energy Threshold -- "; 
./doasym z    1.0 1.0 1.0   0   3.0 15.5 40
./doasym z    1.0 1.0 1.0   0   3.0 15.5 60

done
