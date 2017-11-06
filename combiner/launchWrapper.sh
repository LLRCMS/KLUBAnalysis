#!/bin/bash
# make cards with all vars/selections

#./makeCategories.sh lmr70 
./makeCategories.sh lmr90 "-r" &
./makeCategories.sh hmr90 "-r" &


#./makeCategoriesParallel.sh lmr70 cards 0
#./makeCategoriesParallel.sh lmr70 cards 1
#./makeCategoriesParallel.sh lmr70 cards 2
#./makeCategoriesParallel.sh lmr70 cards 3
#./makeCategoriesParallel.sh lmr70 cards 4
#./makeCategoriesParallel.sh lmr70 cards 5

#./makeCategoriesParallel.sh lmr90 cards "-r"
#./makeCategoriesParallel.sh hmr90 cards "-r"

#./makeCategoriesParallel.sh lmr70 MuTau &
#./makeCategoriesParallel.sh lmr70 ETau &
#./makeCategoriesParallel.sh lmr70 TauTau &
#./makeCategoriesParallel.sh lmr70 combined  0 &
#./makeCategoriesParallel.sh lmr70 combined  1 &
#./makeCategoriesParallel.sh lmr70 combined  2 &
#./makeCategoriesParallel.sh lmr70 combined  3 &
#./makeCategoriesParallel.sh lmr70 combined  4 &
#./makeCategoriesParallel.sh lmr70 combined  5 &
#./makeCategories.sh lmr90 &
#./makeCategories.sh hmr90 &
#./makeCategories.sh LepTauKine &
#
#./makeCategoriesParallel.sh lmr90 MuTau "-r" &
#./makeCategoriesParallel.sh lmr90 ETau "-r" &
#./makeCategoriesParallel.sh lmr90 TauTau "-r" &
#./makeCategoriesParallel.sh lmr90 combined "-r" &
#
#./makeCategoriesParallel.sh hmr90 MuTau "-r" &
#./makeCategoriesParallel.sh hmr90 ETau "-r" &
#./makeCategoriesParallel.sh hmr90 TauTau "-r" &
#./makeCategoriesParallel.sh hmr90 combined "-r" &


#./makeCategories.sh "-r" &
#./makeCategories.sh &
