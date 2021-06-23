# The default set:
printf '%s%s\n' "The Default Set. SNR=0.25@i=0, and sigma_agreement = 3.0."; 
./multituple 10001 1 0 3.0  # SM
./multituple 10002 1 0 3.0  # SM
./multituple 10005 1 0 3.0  # P
./multituple 10006 1 0 3.0  # P
./multituple 10007 1 0 3.0  # P
./multituple 10008 1 0 3.0  # P
./multituple 10009 1 0 3.0  # M
./multituple 10010 1 0 3.0  # M
./multituple 10011 1 0 3.0  # M
./multituple 10012 1 0 3.0  # M
./multituple 10013 1 0 3.0  # SM
./multituple 10014 1 0 3.0  # SM
./multituple 10065 1 0 3.0  # P
./multituple 10066 1 0 3.0  # P
printf '%s%s\n' "Done with multi-set 1!"; 

# The default set:
printf '%s%s\n' "The Default Set. SNR=0.25@i=0, and sigma_agreement = 3.0."; 
printf '%s%s\n' "Starting on the Set B Electrons: "; 
./multituple 428 0 0 3.0
./multituple 429 0 0 3.0
./multituple 430 0 0 3.0
./multituple 431 0 0 3.0
./multituple 432 0 0 3.0
./multituple 433 0 0 3.0
./multituple 434 0 0 3.0
./multituple 435 0 0 3.0
./multituple 436 0 0 3.0  #
./multituple 437 0 0 3.0
./multituple 440 0 0 3.0
./multituple 441 0 0 3.0
./multituple 442 0 0 3.0
./multituple 443 0 0 3.0
./multituple 444 0 0 3.0
./multituple 445 0 0 3.0
printf '%s%s\n' "Starting on the Set C Electrons: "; 
./multituple 476 0 0 3.0
./multituple 477 0 0 3.0
printf '%s%s\n' "Starting on the Set D Electrons: "; 
printf '%s%s\n' "(Runs 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 502, 503, 504, 505, 510, 513)"; 
./multituple 478 0 0 3.0
./multituple 479 0 0 3.0
./multituple 480 0 0 3.0
./multituple 481 0 0 3.0
./multituple 482 0 0 3.0
./multituple 483 0 0 3.0
./multituple 484 0 0 3.0
./multituple 485 0 0 3.0
./multituple 486 0 0 3.0
./multituple 487 0 0 3.0  # short run.  but sometimes I don't even.
./multituple 488 0 0 3.0  # short run.  but sometimes I don't even.
./multituple 489 0 0 3.0  # short run.  but sometimes I don't even.
./multituple 502 0 0 3.0
./multituple 503 0 0 3.0
./multituple 504 0 0 3.0
./multituple 505 0 0 3.0
./multituple 510 0 0 3.0
./multituple 513 0 0 3.0
printf '%s%s\n' "Done with multi-set 1!"; 
