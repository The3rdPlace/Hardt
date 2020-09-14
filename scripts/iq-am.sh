#!/bin/bash
#
# Script that contains various tests with an RTL-2832 device and the
# incomming IQ data
#
# Tests may or may not work on your system, and with the available
# local RF spectrum. This is _NOT_ a part of the basic unittests



make -j 10



# Test am demoulation of IQ signal at 0 (center)
hardt/dspcmd/dspcmd -id 0 -it RTL 127000000 IQ -rf -of 0khz.pcm -force -bc 24000 -r 1152000
hardt/dspcmd/dspcmd -dcm 24 -if 0khz.pcm -of 0khz_decimated.pcm -force -bs 2400
#hardt/dspcmd/dspcmd -fmgp -if 0khz_decimated.pcm -r 48000 -iq
hardt/dspcmd/dspcmd -iqa -if 0khz_decimated.pcm -of 0khz_decimated_abs.pcm -force
#hardt/dspcmd/dspcmd -fmgp -if 0khz_decimated_abs.pcm -r 48000
hardt/dspcmd/dspcmd -pf -if 0khz_decimated_abs.pcm -od 0



#
#hardt/dspcmd/dspcmd -flt HFirFilter lp150khz.dat -if 200khz.pcm -of 200khz_filtered.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 200khz_filtered.pcm -of 200khz_filtered_real.pcm -force -v
#hardt/dspcmd/dspcmd -fmgp -if 200khz_filtered_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -flt HFirFilter lp150khz.dat -if 300khz.pcm -of 300khz_filtered.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 300khz_filtered.pcm -of 300khz_filtered_real.pcm -force -v
#hardt/dspcmd/dspcmd -fmgp -if 300khz_filtered_real.pcm -r 1152000
