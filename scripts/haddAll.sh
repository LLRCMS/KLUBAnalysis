HASHADD=true
type root >/dev/null 2>&1 || { HASHADD=false; }

if [ $HASHADD = true ] ; then
   for dir in ./*/
   do
       cd $dir
       pwd
       if [ ! -f total.root ]; then # safe if already run on part of dataset
           mkdir corrupted
           for logfile in output_*.log
           do
               tmp=${logfile#*_}
               idx=${tmp%.*}
               if ! grep -q "... SKIM finished, exiting." "$logfile"; then
                  echo "job num $idx not correctly finished"
                  mv "output_$idx.root" corrupted
                  mv $logfile corrupted
               fi
           done           

           hadd total.root output_*.root
           mkdir singleFiles
           mv output_*.root singleFiles
           mv output_*.log  singleFiles
       fi
       cd -   
   done
else
   echo "hadd not available, please do cmsenv in a CMSSW release"
fi
