for i in {0..11}
do
   #CMM="cp lesshhV2nonresonant.txt lesshhV2nonresonant_forlambda_$i.txt"
   CMM="cp lesshhV2nonresonant.txt lesshhV2nonresonant_forbench_$i.txt"
   echo $CMM
   $CMM
done
