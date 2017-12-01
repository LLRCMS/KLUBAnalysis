for dir in ./*/
do
   cd $dir
   pwd
   if [ ! -f goodfiles.txt ]; then # safe if already run on part of dataset
       touch goodfiles.txt
       touch badfiles.txt
       for logfile in output_*.log
       do
           tmp=${logfile#*_}
           idx=${tmp%.*}
           if grep -q "R__unzip: error" "$logfile"; then
              echo "job num $idx: file corrupted"
              echo $PWD/output_$idx.root >> badfiles.txt
           elif ! grep -q "... SKIM finished, exiting." "$logfile"; then
              echo "job num $idx not correctly finished"
              # mv "output_$idx.root" corrupted
              # mv $logfile corrupted
              echo $PWD/output_$idx.root >> badfiles.txt
            else
              echo $PWD/output_$idx.root >> goodfiles.txt
           fi
       done           
       # hadd total.root output_*.root
       # mkdir singleFiles
       # mv output_*.root singleFiles
       # mv output_*.log  singleFiles
   fi
   cd -   
done

## this script will list all files and make a filelist of good ones
## to be used with new analysis histo filler
## no need for hadd now
