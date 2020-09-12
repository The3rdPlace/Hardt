#!/bin/bash
#
# Script that contains various tests with an RTL-2832 device and the
# incomming IQ data
#
# Tests may or may not work on your system, and with the available
# local RF spectrum. This is _NOT_ a part of the basic unittests



make -j 10



# Test REAL output
#hardt/dspcmd/dspcmd -id 0 -it RTL 126850000 REAL -rf -of samples.pcm -force -bc 1000 -r 1152000 -v && \
#hardt/dspcmd/dspcmd -fmgp -if samples.pcm -r 1152000



# Capture 127MHz Kastrup Volmet - at -50KHz, 100KHz, 150KHz, 200KHz and 300KHz
hardt/dspcmd/dspcmd -id 0 -it RTL 127050000 IQ -rf -of -50khz.pcm -force -bc 1000 -r 1152000 -v && \
hardt/dspcmd/dspcmd -iqr -if -50khz.pcm -of -50khzreal.pcm -force -v
hardt/dspcmd/dspcmd -fmgp -if -50khzreal.pcm -r 1152000
#
hardt/dspcmd/dspcmd -id 0 -it RTL 126900000 IQ -rf -of 100khz.pcm -force -bc 1000 -r 1152000 -v && \
hardt/dspcmd/dspcmd -iqr -if 100khz.pcm -of 100khzreal.pcm -force -v
hardt/dspcmd/dspcmd -fmgp -if 100khzreal.pcm -r 1152000
#
hardt/dspcmd/dspcmd -id 0 -it RTL 126850000 IQ -rf -of 150khz.pcm -force -bc 1000 -r 1152000 -v && \
hardt/dspcmd/dspcmd -iqr -if 150khz.pcm -of 150khzreal.pcm -force -v
hardt/dspcmd/dspcmd -fmgp -if 150khzreal.pcm -r 1152000
#
hardt/dspcmd/dspcmd -id 0 -it RTL 126800000 IQ -rf -of 200khz.pcm -force -bc 1000 -r 1152000 -v && \
hardt/dspcmd/dspcmd -iqr -if 200khz.pcm -of 200khzreal.pcm -force -v
hardt/dspcmd/dspcmd -fmgp -if 200khzreal.pcm -r 1152000
#
hardt/dspcmd/dspcmd -id 0 -it RTL 126700000 IQ -rf -of 300khz.pcm -force -bc 1000 -r 1152000 -v && \
hardt/dspcmd/dspcmd -iqr -if 300khz.pcm -of 300khzreal.pcm -force -v
hardt/dspcmd/dspcmd -fmgp -if 300khzreal.pcm -r 1152000
