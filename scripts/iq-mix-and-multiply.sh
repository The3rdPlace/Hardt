#!/bin/bash
#
# Script that contains various tests with an RTL-2832 device and the
# incomming IQ data
#
# Tests may or may not work on your system, and with the available
# local RF spectrum. This is _NOT_ a part of the basic unittests



make -j 10



# Test IQ Mixing - convert to real with negative frequencies = nonsens output spectrum
#hardt/dspcmd/dspcmd -mx -50khz.pcm 100khz.pcm -of -50-100khz_mixed.pcm -force && \
#hardt/dspcmd/dspcmd -fmgp -if -50-100khz_mixed.pcm -r 1152000 -iq && \
#hardt/dspcmd/dspcmd -iqa -if -50-100khz_mixed.pcm -of  -50-100khz_mixed_abs.pcm && \
#hardt/dspcmd/dspcmd -fmgp -if -50-100khz_mixed_abs.pcm -r 1152000



# Test IQ to abs - convert to real without negative frequencies = nonsens output spectrum
#hardt/dspcmd/dspcmd -iqa -if 100khz.pcm -of  100khz_abs.pcm && \
#hardt/dspcmd/dspcmd -fmgp -if 100khz_abs.pcm -r 1152000



# Test IQ Mixing - Mix 100KHz signal with 50KHz LO = 150KHz
hardt/dspcmd/dspcmd -mp 50000 -if 100khz.pcm -of 100khz_multiplied.pcm -force -r 1152000 -iq && \
hardt/dspcmd/dspcmd -fmgp -if 100khz_multiplied.pcm -r 1152000 -iq

# Test IQ Mixing - Mix 100KHz signal with -50KHz LO = 50KHz
hardt/dspcmd/dspcmd -mp -50000 -if 100khz.pcm -of 100khz_multiplied.pcm -force -r 1152000 -iq && \
hardt/dspcmd/dspcmd -fmgp -if 100khz_multiplied.pcm -r 1152000 -iq
