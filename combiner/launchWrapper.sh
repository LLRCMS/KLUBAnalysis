#!/bin/bash
# make cards with all vars/selections

./makeCategories.sh lmr70 &
./makeCategories.sh lmr90 &
./makeCategories.sh hmr90 &
#./makeCategories.sh LepTauKine &

./makeCategories.sh lmr90 "-r" &
./makeCategories.sh hmr90 "-r" &


#./makeCategories.sh "-r" &
#./makeCategories.sh &
