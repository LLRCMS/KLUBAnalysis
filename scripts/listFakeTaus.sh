for dir in ./*/
do
   cd $dir
   pwd
   if [ ! -f goodtaufakesfiles.txt ]; then # safe if already run on part of dataset
       touch goodtaufakesfiles.txt
       touch badtaufakesfiles.txt
       for logfile in taufakes_output_*.log
       do
           tmp=${logfile#*_}
	   tmp=${tmp#*_}
           idx=${tmp%.*}
	   if grep -q "R__unzip: error" "$logfile"; then
	      echo "job num $idx: file corrupted"
	      echo $PWD/taufakes_output_$idx.root >> badtaufakesfiles.txt
           elif ! grep -q "... SYST finished, exiting." "$logfile"; then
              echo "job num $idx not correctly finished"
              echo $PWD/taufakes_output_$idx.root >> badtaufakesfiles.txt
           else
              echo $PWD/taufakes_output_$idx.root >> goodtaufakesfiles.txt
           fi
       done
   fi
   cd -   
done

## this script will list all files and make a filelist of good ones
## to be used with new analysis histo filler
## no need for hadd now
