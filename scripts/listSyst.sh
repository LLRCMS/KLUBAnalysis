for dir in ./*/
do
   cd $dir
   pwd
   if [ ! -f goodsystfiles.txt ]; then # safe if already run on part of dataset
       touch goodsystfiles.txt
       touch badsystfiles.txt
       for logfile in syst_output_*.log
       do
           tmp=${logfile#*_}
	   tmp=${tmp#*_}
           idx=${tmp%.*}
	   if grep -q "R__unzip: error" "$logfile"; then
	      echo "job num $idx: file corrupted"
	      echo $PWD/syst_output_$idx.root >> badsystfiles.txt
           elif ! grep -q "... SYST finished, exiting." "$logfile"; then
              echo "job num $idx not correctly finished"
              echo $PWD/syst_output_$idx.root >> badsystfiles.txt
           else
              echo $PWD/syst_output_$idx.root >> goodsystfiles.txt
           fi
       done
   fi
   cd -   
done

## this script will list all files and make a filelist of good ones
## to be used with new analysis histo filler
## no need for hadd now
