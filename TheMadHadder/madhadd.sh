printf '%s%s\n' "We've called the Mad Hadder!"; 
r1=$1
r2=$2
iteration=$3
#result = $(./madhadder 200 212); 
##result=`./madhadder 200 212`; 
###r1=200;
###r2=212;
datapath="/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";
#./madhadder 200 212;
./madhadder $r1 $r2;
result=$?;
echo "Here's the result:  "$result
#multiresults="$(./madhadder 200 212)"
#result=$multiresults.2
#echo "Here's the result again:  "$result


if [ $result != "255" ]  # kludge!  :(
then
    if [ $iteration == "0" ]
    then
    #    echo "hadd $datapath'summedoutput_'$result.root $datapath output_$r1.root $datapath output_$r2.root"
        echo hadd $datapath"summedoutput_"$result.root $datapath"output_"$r1.root $datapath"output_"$r2.root
        hadd $datapath"summedoutput_"$result.root $datapath"output_"$r1.root $datapath"output_"$r2.root 
    else
        if [ $iteration == "1" ]
        then
            echo hadd $datapath"summedoutput_"$result.root $datapath"summedoutput_"$r1.root $datapath"summedoutput_"$r2.root
            hadd $datapath"summedoutput_"$result.root $datapath"summedoutput_"$r1.root $datapath"summedoutput_"$r2.root 
        else # "2"
            echo hadd $datapath"summedoutput_"$result.root $datapath"output_"$r1.root $datapath"summedoutput_"$r2.root
            hadd $datapath"summedoutput_"$result.root $datapath"output_"$r1.root $datapath"summedoutput_"$r2.root 
        fi
    fi
else
    echo "Not gonna do it."  
fi
#echo "Here's the result!"
#echo $result