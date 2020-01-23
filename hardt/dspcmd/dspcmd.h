/**
    \page dspcmd

	\section introduction_section_dspcmd About dspcmd
	todo


	\section overview_section_dspcmd Overview
	todo


	\section options_section_dspcmd Options	
    Usage: dspcmd [option [value...]]
    
    -a = Show a list of available audio devices

    -h = Show this help

    -v = Be verbose, dont write to logfiles but to stdout

    -\subpage bs = Blocksize used by readers and writers (default = 1024)

    -\subpage f = Sample format (H_SAMPLE_FORMAT_INT_8=Int8, H_SAMPLE_FORMAT_UINT_8=UInt8, H_SAMPLE_FORMAT_INT_16=Int16, H_SAMPLE_FORMAT_INT_32=Int32) (default H_SAMPLE_FORMAT_INT_16 = Int16)

    -\subpage ff = Type of filereader/filewriterwriter

    -\subpage id = Input audio device

    -\subpage if = Name and path of input file

    -\subpage od  = Output audio device

    -\subpage of = Name and path of output file

    -\subpage force = Force overwrite of existing file

    -\subpage r = Sample rate (H_SAMPLE_RATE_8K, H_SAMPLE_RATE_11K, H_SAMPLE_RATE_22K, H_SAMPLE_RATE_32K, H_SAMPLE_RATE_44K1, H_SAMPLE_RATE_48K, H_SAMPLE_RATE_96K, H_SAMPLE_RATE_192K) (default 48000)

    -\subpage start = Start time

    -\subpage stop = Stop time

    -\subpage sg = Run as signalgenerator, duration in seconds

    -\subpage nc = Run as network client, reading from the network and writing to a local writer

    -\subpage ns = Run as network server, reading from a local reader and writing to the network

    -\subpage rf = Record file

    -\subpage pf = Play file

    -\subpage fmp = Run FFT on a file and plot the magnitude spectrum on screen

    -\subpage fmgp = Run FFT on a file and plot the magnitude spectrum on screen using GnuPlot

    -\subpage avg = Take 'number' FFT's and return the average spectrum

    -\subpage pr = Set minimum and maximum on the x-axis when plotting (only GnuPlot)

    -\subpage pr = Set minimum and maximum on the x-axis when plotting (only GnuPlot)

    -\subpage fc = Convert input file format to output file format (same as in \subpage ff)

    -\subpage mx = Mix (linear) file1 and file2

    -\subpage mp = Multiply (mix nonlinear) with localoscilator signal running at frequency

    -\subpage flt = Read coefficients from coeffs and run file through filter name

    -\subpage flp = Read coefficients from coeffs and plot filter response for filter name

    -\subpage flgp = Read coefficients from coeffs and plot filter response for filter name using GnuPlot

    -\subpage bq = Create biquad filter, dump coefficients and run sweep

    -\subpage bqt = Create biquad filter and run file through the filter

    -\subpage bqc = Create biquad filter, dump coefficients and run sweep

    -\subpage bgct = Create biquad filter and run file through the filter

    -\subpage g Run = file through gain filter

    -\subpage cb = Sweep a comb filter with base frequency fBase and alpha (gain)

    -\subpage cbt = Run a file through a comb filter with base frequency fBase and alpha (gain)

    -\subpage hm = Sweep a hum filter with base frequency fBase and cutoff frequency fCutoff

    -\subpage hmt = Run a file through a hum filter with base frequency fBase and cutoff frequency fCutoff

    -\subpage gz = Sweep a file by running the individual bins through a Goertzl filter


    \page bs
    Syntax: dspcmd -bs blocksize

    Blocksize used by readers and writers (default = 1024)


    \page f
    Syntax: dspcmd -f

    Sample format (H_SAMPLE_FORMAT_INT_8=Int8, H_SAMPLE_FORMAT_UINT_8=UInt8, H_SAMPLE_FORMAT_INT_16=Int16, H_SAMPLE_FORMAT_INT_32=Int32) (default H_SAMPLE_FORMAT_INT_16 = Int16)


    \page ff
    Syntax: dspcmd -ff pcm|wav

    Type of filereader/filewriterwriter


    \page id
    Syntax: dspcmd -id device

    Input audio device


    \page if
    Syntax: dspcmd -if name

    Name and path of input file


    \page od
    Syntax: dspcmd -od device

    Output audio device


    \page of
    Syntax: dspcmd -of name

    Name and path of output file


    \page force
    Syntax: dspcmd -force

    Force overwrite of existing file


    \page r
    Syntax: dspcmd -r rate

    Sample rate (H_SAMPLE_RATE_8K, H_SAMPLE_RATE_11K, H_SAMPLE_RATE_22K, H_SAMPLE_RATE_32K, H_SAMPLE_RATE_44K1, H_SAMPLE_RATE_48K, H_SAMPLE_RATE_96K, H_SAMPLE_RATE_192K) (default 48000)


    \page start
    Syntax: dspcmd -start [YYYY-MM-DD]hh:mm

    Start time for scheduled operation


    \page stop
    Syntax: dspcmd -stop [YYYY-MM-DD]hh:mm

    Stop time for scheduled operation


    \page sg
    Syntax: dspcmd -fsg freq phase duration

    Run as signalgenerator, duration in seconds


    \page nc
    Syntax: dspcmd -nc server port

    Run as network client, reading from the network and writing to a local writer


    \page ns
    Syntax: dspcmd -ns port

    Run as network server, reading from a local reader and writing to the network


    \page rf
    Syntax: dspcmd -rf

    Record file


    \page pf
    Syntax: dspcmd -pf

    Play file


    \page fmp
    Syntax: dspcmd -fmp size

    Run FFT on a file and plot the magnitude spectrum on screen


    \page fmgp
    Syntax: dspcmd -fmgp size

    Run FFT on a file and plot the magnitude spectrum on screen using GnuPlot


    \page avg
    Syntax: dspcmd -avg number

    Take 'number' FFT's and return the average spectrum


    \page pr
    Syntax: dspcmd -pr xmin xmax [ymin ymax]

    Set minimum and maximum on the x-axis when plotting (only GnuPlot)


    \page fc
    Syntax: dspcmd -fc in-format out-format

    Convert input file format to output file format (same as in -ff)


    \page mx
    Syntax: dspcmd -mx file1 file2

    Mix (linear) file1 and file2


    \page mp
    Syntax. dspcmd -mp frequency

    Multiply (mix nonlinear) with localoscilator signal running at frequency


    \page flt
    Syntax: dspcmd -flt name coeffs

    Read coefficients from coeffs and run file through filter name


    \page flp
    Syntax: dspcmd -flp name coeffs

    Read coefficients from coeffs and plot filter response for filter name


    \page flgp
    Syntax: dspcmd -flgp name coeffs

    Read coefficients from coeffs and plot filter response for filter name using GnuPlot


    \page bq
    Syntax: dspcmd -bq name Fcutoff Q G

    Create biquad filter, dump coefficients and run sweep


    \page bqt
    Syntax: dspcmd -bqt name Fcutoff Q G

    Create biquad filter and run file through the filter


    \page bqc
    Syntax: dspcmd -bqc coeffs

    Create biquad filter, dump coefficients and run sweep


    \page bgct
    Syntax: dspcmd -bqct coeffs

    Create biquad filter and run file through the filter


    \page g
    Syntax: dspcmd -g gain

    Run file through gain filter


    \page cb
    Syntax: dspcmd -cb fBase alpha

    Sweep a comb filter with base frequency fBase and alpha (gain)


    \page cbt
    Syntax: dspcmd -cbt fBase alpha

    Run a file through a comb filter with base frequency fBase and alpha (gain)


    \page hm
    Syntax: dspcmd -hm fBase fCutoff

    Sweep a hum filter with base frequency fBase and cutoff frequency fCutoff


    \page hmt
    Syntax: dspcmd -hmt fBase F G Q

    Run a file through a hum filter with base frequency fBase and cutoff frequency fCutoff


    \page gz
    Syntax: dspcmd -gq

    Sweep a file by running the individual bins through a Goertzl filter
	
*/

#ifndef __DSPCMD_H
#define __DSPCMD_H

#include <stdio.h>

template <typename T>
void print_array(T* array, int sizeOfArray)
{
    for( int i = 0; i < sizeOfArray; i++ )
    {
        std::cout << i << " = " << array[i] << '\n';
    }

}

#endif

