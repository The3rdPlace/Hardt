#!/bin/bash
#
# Script that contains various tests with an RTL-2832 device and the
# incomming IQ data
#
# Tests may or may not work on your system, and with the available
# local RF spectrum. This is _NOT_ a part of the basic unittests



make -j 10



# Test IQ FIR filter
#hardt/dspcmd/dspcmd -flt HFirFilter lp150khz.dat -if 100khz.pcm -of 100khz_filtered.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 100khz_filtered.pcm -of 100khz_filtered_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 100khz_filtered_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -flt HFirFilter lp150khz.dat -if 200khz.pcm -of 200khz_filtered.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 200khz_filtered.pcm -of 200khz_filtered_real.pcm -force -v
#hardt/dspcmd/dspcmd -fmgp -if 200khz_filtered_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -flt HFirFilter lp150khz.dat -if 300khz.pcm -of 300khz_filtered.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 300khz_filtered.pcm -of 300khz_filtered_real.pcm -force -v
#hardt/dspcmd/dspcmd -fmgp -if 300khz_filtered_real.pcm -r 1152000



# Test IQ FIR decimator
#hardt/dspcmd/dspcmd -fdcm 4 lp150khz.dat -if 100khz.pcm -of 100khz_filtered_decimated.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 100khz_filtered_decimated.pcm -of 100khz_filtered_decimated_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 100khz_filtered_decimated_real.pcm -r 288000
#
#hardt/dspcmd/dspcmd -fdcm 4 lp150khz.dat -if 200khz.pcm -of 200khz_filtered_decimated.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 200khz_filtered_decimated.pcm -of 200khz_filtered_decimated_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 200khz_filtered_decimated_real.pcm -r 288000
#
#hardt/dspcmd/dspcmd -fdcm 4 lp150khz.dat -if 300khz.pcm -of 300khz_filtered_decimated.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 300khz_filtered_decimated.pcm -of 300khz_filtered_decimated_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 300khz_filtered_decimated_real.pcm -r 288000



# Test IQ translate-by-two
hardt/dspcmd/dspcmd -tr2 -if 100khz.pcm -of 100khz_filtered_tr2.pcm -iq -force && \
hardt/dspcmd/dspcmd -iqr -if 100khz_filtered_tr2.pcm -of 100khz_filtered_tr2_real.pcm -force -v && \
hardt/dspcmd/dspcmd -fmgp -if 100khz_filtered_tr2_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -tr2 -if 200khz.pcm -of 200khz_filtered_tr2.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 200khz_filtered_tr2.pcm -of 200khz_filtered_tr2_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 200khz_filtered_tr2_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -tr2 -if 300khz.pcm -of 300khz_filtered_tr2.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 300khz_filtered_tr2.pcm -of 300khz_filtered_tr2_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 300khz_filtered_tr2_real.pcm -r 1152000
#



# Test IQ translate-by-four I
#hardt/dspcmd/dspcmd -tr4i -if 100khz.pcm -of 100khz_filtered_tr4I.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 100khz_filtered_tr4I.pcm -of 100khz_filtered_tr4I_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 100khz_filtered_tr4I_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -tr4i -if 200khz.pcm -of 200khz_filtered_tr4I.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 200khz_filtered_tr4I.pcm -of 200khz_filtered_tr4I_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 200khz_filtered_tr4I_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -tr4i -if 300khz.pcm -of 300khz_filtered_tr4I.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 300khz_filtered_tr4I.pcm -of 300khz_filtered_tr4I_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 300khz_filtered_tr4I_real.pcm -r 1152000
#



# Test IQ translate-by-four Q
#hardt/dspcmd/dspcmd -tr4q -if 100khz.pcm -of 100khz_filtered_tr4Q.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 100khz_filtered_tr4Q.pcm -of 100khz_filtered_tr4Q_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 100khz_filtered_tr4Q_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -tr4q -if 200khz.pcm -of 200khz_filtered_tr4Q.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 200khz_filtered_tr4Q.pcm -of 200khz_filtered_tr4Q_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 200khz_filtered_tr4Q_real.pcm -r 1152000
#
#hardt/dspcmd/dspcmd -tr4q -if 300khz.pcm -of 300khz_filtered_tr4Q.pcm -iq -force && \
#hardt/dspcmd/dspcmd -iqr -if 300khz_filtered_tr4Q.pcm -of 300khz_filtered_tr4Q_real.pcm -force -v && \
#hardt/dspcmd/dspcmd -fmgp -if 300khz_filtered_tr4Q_real.pcm -r 1152000
#
