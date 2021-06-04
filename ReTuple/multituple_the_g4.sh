printf '%s%s\n' "Here we go!"; 
#            runno g4 ind sig 

# The default set:
printf '%s%s\n' "The Default Set. SNR=0.25@i=0, and sigma_agreement = 3.0."; 
./multituple 10005 1 0 3.0
./multituple 10006 1 0 3.0
./multituple 10007 1 0 3.0
./multituple 10008 1 0 3.0
./multituple 10065 1 0 3.0
./multituple 10066 1 0 3.0
./multituple 10001 1 0 3.0
./multituple 10002 1 0 3.0
./multituple 10013 1 0 3.0
./multituple 10014 1 0 3.0
./multituple 10009 1 0 3.0
./multituple 10010 1 0 3.0
./multituple 10011 1 0 3.0
./multituple 10012 1 0 3.0
printf '%s%s\n' "Done with multi-set 1!"; 

#   ###   #
# Adjust SNR index. SNR=0.25@i=0, SNR=0.50@i=1, SNR=0.75@i=2, SNR=1.0@i=3, SNR=2.0@i=4.
printf '%s%s\n' "Adjust SNR index. SNR=0.5@i=1."; 
./multituple 10005 1 1 3.0
./multituple 10006 1 1 3.0
./multituple 10007 1 1 3.0
./multituple 10008 1 1 3.0
./multituple 10065 1 1 3.0
./multituple 10066 1 1 3.0
./multituple 10001 1 1 3.0
./multituple 10002 1 1 3.0
./multituple 10013 1 1 3.0
./multituple 10014 1 1 3.0
./multituple 10009 1 1 3.0
./multituple 10010 1 1 3.0
./multituple 10011 1 1 3.0
./multituple 10012 1 1 3.0
printf '%s%s\n' "Done with multi-set 2!"; 

# Adjust SNR index again. SNR=0.25@i=0, SNR=0.50@i=1, SNR=0.75@i=2, SNR=1.0@i=3, SNR=2.0@i=4.
printf '%s%s\n' "Adjust SNR index. SNR=1.0@i=3."; 
./multituple 10005 1 3 3.0
./multituple 10006 1 3 3.0
./multituple 10007 1 3 3.0
./multituple 10008 1 3 3.0
./multituple 10065 1 3 3.0
./multituple 10066 1 3 3.0
./multituple 10001 1 3 3.0
./multituple 10002 1 3 3.0
./multituple 10013 1 3 3.0
./multituple 10014 1 3 3.0
./multituple 10009 1 3 3.0
./multituple 10010 1 3 3.0
./multituple 10011 1 3 3.0
./multituple 10012 1 3 3.0
printf '%s%s\n' "Done with multi-set 3!"; 


# Adjust the sigma agreement.  Down first:
printf '%s%s\n' "Adjust the sigma agreement.  Down first.  "; 
./multituple 10005 1 0 2.0
./multituple 10006 1 0 2.0
./multituple 10007 1 0 2.0
./multituple 10008 1 0 2.0
./multituple 10065 1 0 2.0
./multituple 10066 1 0 2.0
./multituple 10001 1 0 2.0
./multituple 10002 1 0 2.0
./multituple 10013 1 0 2.0
./multituple 10014 1 0 2.0
./multituple 10009 1 0 2.0
./multituple 10010 1 0 2.0
./multituple 10011 1 0 2.0
./multituple 10012 1 0 2.0
printf '%s%s\n' "Done with multi-set 4!"; 

# Adjust the sigma agreement.  Down farther:
printf '%s%s\n' "Adjust the sigma agreement.  Down farther.  "; 
./multituple 10005 1 0 1.0
./multituple 10006 1 0 1.0
./multituple 10007 1 0 1.0
./multituple 10008 1 0 1.0
./multituple 10065 1 0 1.0
./multituple 10066 1 0 1.0
./multituple 10001 1 0 1.0
./multituple 10002 1 0 1.0
./multituple 10013 1 0 1.0
./multituple 10014 1 0 1.0
./multituple 10009 1 0 1.0
./multituple 10010 1 0 1.0
./multituple 10011 1 0 1.0
./multituple 10012 1 0 1.0
printf '%s%s\n' "Done with multi-set 5!"; 


# Adjust the sigma agreement.  Up next:
printf '%s%s\n' "Adjust the sigma agreement.  Up next.  "; 
./multituple 10005 1 0 4.0
./multituple 10006 1 0 4.0
./multituple 10007 1 0 4.0
./multituple 10008 1 0 4.0
./multituple 10065 1 0 4.0
./multituple 10066 1 0 4.0
./multituple 10001 1 0 4.0
./multituple 10002 1 0 4.0
./multituple 10013 1 0 4.0
./multituple 10014 1 0 4.0
./multituple 10009 1 0 4.0
./multituple 10010 1 0 4.0
./multituple 10011 1 0 4.0
./multituple 10012 1 0 4.0
printf '%s%s\n' "Done with multi-set 6!"; 

# Adjust the sigma agreement.  Up farther:
printf '%s%s\n' "Adjust the sigma agreement.  Up further.  "; 
./multituple 10005 1 0 5.0
./multituple 10006 1 0 5.0
./multituple 10007 1 0 5.0
./multituple 10008 1 0 5.0
./multituple 10065 1 0 5.0
./multituple 10066 1 0 5.0
./multituple 10001 1 0 5.0
./multituple 10002 1 0 5.0
./multituple 10013 1 0 5.0
./multituple 10014 1 0 5.0
./multituple 10009 1 0 5.0
./multituple 10010 1 0 5.0
./multituple 10011 1 0 5.0
./multituple 10012 1 0 5.0
printf '%s%s\n' "Done with multi-set 7!"; 

