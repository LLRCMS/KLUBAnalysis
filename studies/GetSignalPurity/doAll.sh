## resonant
for i in 250 270 280 300 350 400 450 500 550 600 650 750 900 ; do python getPurity.py --mass $i; done

## non-resonant
for i in {0..51} ; do python getPurity.py --mass $i --nonres ; done