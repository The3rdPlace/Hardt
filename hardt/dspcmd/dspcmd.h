/**
    \page dspcmd

	\section introduction_section_dspcmd About dspcmd
	Dspcmd is a tool that allows you to test how individual parts of Hardt operates with
	a given dataset. You can test various filters and filter coefficients. Plot spectrum
	for the input and output, generate testfile with sinus signals etc.

	Some of the operations exists mostly to help developing and testing hardt, but
	there are several key features that will help you get through the dsp design
	such as spectrum plotting.

	Check the individual options listed here and read the detailed specification for how
	to use it.


	\section options_section_dspcmd Options	
    Usage: dspcmd [option [value...]]
    
    -a = Show a list of available audio devices

    -t = Show a list of available RTL2832 devices

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

    Sets the number of samples read or written per processing cycle (each read or write).
    It defaults to 1024 which is a fairly good choice when trying to balance cpu load vs
    overhead, but it may be too little for some operations, fine resolution fft's as
    one example. To get a more precise graph, you may want to increase the blocksize.

    The blocksize must be an even number, preferably, a power of 2, so 128, 256, 512, 1024, 2048, 4096, etc.


    \page f
    Syntax: dspcmd -f format
    where format is one of: 16=Int8, 32=UInt8, 8=Int16, 2=Int32 (default 8 = Int16)

    Selects the fundamental datatype for operations as 8, 16 or 32 bits. For 8 bits you can
    choose signed or unsigned allthough not all soundcards may support unsigned - nor is it guaranteed
    that all operations will function as expected (but they should). In general just stick to int8, int16 or int32
    depending on your needs. Do not forget that larger bitsize increases processing load!


    \page ff
    Syntax: dspcmd -ff pcm|wav

    When operating with files as in- or output, choose the type of file. 'pcm' expects raw pcm data
    in the selected samplerate and format, wav expects a wav file - also with the selected samplerate
    and format.

    For wav files, the normal thing is to im- and export signed 16 bit in either 44.1KHz og 48KHz samplerate,
    but other combinations may also exist. Dspcmd do not currently support large wav files (32 bit)


    \page id
    Syntax: dspcmd -id device

    Input audio device

    The device number of a soundcard device used as input for the operation. You can use 'dspcmd -a'
    to get a list of available sound devices on your system. Beware that on some OS's you need to
    be root to access a sound device (Raspberian as an example)


    \page if
    Syntax: dspcmd -if name

    Name and path of input file

    For operations that operate on a file and do not take the filename as the second parameter, this
    is the name of the file used for input


    \page od
    Syntax: dspcmd -od device

    Output audio device

    The device number of a soundcard device used as output for the operation. You can use 'dspcmd -a'
    to get a list of available sound devices on your system. Beware that on some OS's you need to
    be root to access a sound device (Raspberian as an example)


    \page of
    Syntax: dspcmd -of name

    Name and path of output file

    For operations that operate on a file and do not take the filename as the second parameter, this
    is the name of the file used for output


    \page force
    Syntax: dspcmd -force

    Force overwrite of existing file

    If the operation writes to a file, and the file exists, dspcmd will normally refuse to overwrite the
    file. Add this option to force overwriting the output file


    \page r
    Syntax: dspcmd -r rate
    Where rate is one of: 8000, 11025, 22050, 32000, 44100, 48000, 96000, 192000 (default 48000)

    The samplerate used for the operation. Beware that for operations using external operations,
    some samplerates may not be supported and will result in an error.

    For file operations, you can choose whatever you like, just remember that the higher samplerate,
    the more data produced and consumed.


    \page start
    Syntax: dspcmd -start [YYYY-MM-DD]hh:mm

    Start time for scheduled operations, for example recording an input pcm stream.

    You must provide a two-digit starting hour and minute, you can also provide a starting
    year, month and day should you need so - this is mostly needed if you want to schedule and
    operation 1 or dwo days in advance

    You do not need to provide a stop time, but dspcmd will then run untill you terminate it manually


    \page stop
    Syntax: dspcmd -stop [YYYY-MM-DD]hh:mm

    Stop time for scheduled operation, for example recording an input pcm stream.

    You must provide a two-digit stop hour and minute, you can also provide a stop
    year, month and day should you need so - this is mostly needed if you want to schedule and
    operation 1 or dwo days in advance.

    The stop time and date must be in the future in respect to the start time, in case a start time
    is given.

    You do not need to provide a starting time, dspcmd will then start immediately and run until
    the chosen stop time.


    \page sg
    Syntax: dspcmd -sg frequency phase amplitude duration

    Run as signalgenerator, duration is given in  seconds.

    YOu must provide the frequency of the generator, the phase (often this has no implication
    on the use of the output file, so just use 0 (cos) or 90 (sin). The duration given in second will
    control how many seconds of data at the given frequency with the given phase is generated.
    Amplitude gives the maximum possible (absolute) value of the signal. Corresponding to sin/cos = 1.

    \page nc
    Syntax: dspcmd -nc server port

    Run as network client, reading from the network and writing to a local writer

    'Server' is the server name, or ip address, and 'port' is the tcp port on which the
    server listens.

    This allows you to read samples from a soundcard on a remote host and then transfer
    the data to you local host. For example from a dedicated Raspberry Pi located in your
    basement or outside in a weathersealed box near an antenna.

    There are no special protocol overlay, no errorchecking and no commands flowing, just
    a stream of pcm data - which makes this method fast, but also a bit prone to network errors
    or timeouts. Despite these risks, the way of reading data from a remote soundcard is much faster
    and with less overhead than using a soundserver and with a minimum (none) of configuration needed
    on both ends.


    \page ns
    Syntax: dspcmd -ns port

    Run as network server, reading from a local reader and writing to the network

   'port' is the tcp port on which the server listens. Reading starts when a client connets and
   stops again when the client disconnets (or the connection drops). There can only be a single
   client connection at a time.

    This allows you to read samples from a soundcard on a remote host and then transfer
    the data to you local host. For example from a dedicated Raspberry Pi located in your
    basement or outside in a weathersealed box near an antenna.

    There are no special protocol overlay, no errorchecking and no commands flowing, just
    a stream of pcm data - which makes this method fast, but also a bit prone to network errors
    or timeouts. Despite these risks, the way of reading data from a remote soundcard is much faster
    and with less overhead than using a soundserver and with a minimum (none) of configuration needed
    on both ends.


    \page rf
    Syntax: dspcmd -rf

    Record file

    Read samples from the given input audio device (or file) and write them to the given output file
    in the selected output format.
    (see -id, -if, -of -ff)

    Recording starts immediately, or at a scheduled start time if given, and continues untill you stop
    by pressing ctrl+c, or if a scheduled stop time is reached.


    \page pf
    Syntax: dspcmd -pf

    Play file

    Read from the file given by -if with the format given by -ff and write to the selected output
    device or output file (-od, -of)


    \page fmgp
    Syntax: dspcmd -fmgp [size [fcenter factor]]

            std::cout << "Run FFT on a file and plot the magnitude spectrum on screen using GnuPlot" << std::endl;
            std::cout << "If a center frequency and a factor is given, zooming is applied." << std::endl;
            std::cout << std::endl;

    Run FFT on a file and plot the magnitude spectrum on screen using GnuPlot.
    If a center frequency and a factor is given, zooming is applied.

    The FFT is calculated for the number of bins given by the choosen blocksize (-bs), samples
    are read from the file given by the -if parameter.

    You need GnuPlot on your system for this option to work.


    \page avg
    Syntax: dspcmd -avg number

    Take 'number' FFT's and return the average spectrum

    Specify the number of ffts calculated before returning an averaged spectrum. If you do
    not give a number, then the result is returned for each block of samples.

    For any file, the FFT is being calculated for the entire input file, so the only difference
    you will find by setting a higher average, is a tiny performance boost, and you may get a little
    smother rounding of spectrum spikes.


    \page pr
    Syntax: dspcmd -pr xmin xmax [ymin ymax]

    Set minimum and maximum on the x- and y-axis when plotting (only GnuPlot)

    When plotting a spectrum using GnuPlot, this sets the minimum and maximum displayed x values.
    If you only need to see the bottom 2KHz, then use "-pr 0 2000'. THis will give you a plot faster,
    and if you at the same time raise the blocksize, you will get a more detailed plot (more bins within
    the same frequency range).

    You must provide the two x values, but you need to specify the y values.


    \page fc
    Syntax: dspcmd -fc in-format out-format

    Convert input file format to output file format (same as in -ff)

    Read the file given by the -if option and rewrite it to the file given by the -of option
    in the format selected (say, converting from pcm to wav or reverse).

    You can also use this function to write a file in the same format if you have a wav file with
    a damaged header. Just make sure you dont try to read from, and write to the same file.


    \page mx
    Syntax: dspcmd -mx file1 file2

    Mix (linear) file1 and file2

    Read the file given y 'file1' and mix it using a linear mixer (as by mixing two microphone inputs
    in a PA mixer to produce a combined output signal).

    The output is written to the file given by the -of option


    \page sb
    Syntax: dspcmd -sb file1 file2

    Subtract file1 and file2

    Read the file given y 'file1' and subtract samples from file2 from samples in file1.

    The output is written to the file given by the -of option

    \page mp
    Syntax. dspcmd -mp frequency

    Multiply (mix nonlinear) with localoscilator signal running at frequency

    Read the file given by the -if option and run it through a nonlinear mixer to produce a number
    of sum- and difference frequencies. The output is written to the file given by the -of option.

    'frequency' is the localoscillator frequency, so if you have a file containing a 10KHz sinus signal,
    and run it through the nonlinear mixer with a localoscillator at 9KHz, you will get the two sum-
    and difference frequencies 1KHz and 19KHz, as well as some nonlinear components. The file must be
    run through a filter to remove the unwanted components.

    The oscillator amplitude is fixed at '10', so you need to make sure that the input is not
    exceeding values that multiplied by 10 will produce a number larger than what you selected
    sample format supports. If you use 16 bit signed, then make sure the input amplitude do not
    exceed 3276 (3276 * 10 = 32.760 = 65536/2)


    \page flt
    Syntax: dspcmd -flt name coeffs

    Read coefficients from coeffs and run file through filter name

    Read a set of filter coefficients from the file 'coeffs', build a filter of type 'name' and
    then run the samples in the file given with the -if parameter through the filter. The output
    is written to the file given with the -of parameter.

    'name' is ghe object name from Hardt for either a iir or fir filter. 'HFirFilter' builds a Fir
     filter and HIirFilter builds an IIR filter.


    \page flp
    Syntax: dspcmd -flp name coeffs

    Read coefficients from coeffs and plot filter response for filter name

    Read a set of filter coefficients from the file 'coeffs', build a filter of type 'name' and
    then sweep the filter from 0 to samplerate/2 Hz. Then plot the filter response.


    \page flgp
    Syntax: dspcmd -flgp name coeffs

    Read coefficients from coeffs and plot filter response for filter name using GnuPlot

    Read a set of filter coefficients from the file 'coeffs', build a filter of type 'name' and
    then sweep the filter from 0 to samplerate/2 Hz. Then plot the filter response using GnuPlot


    \page bq
    Syntax: dspcmd -bq name Fcutoff Q G

    Create biquad filter, dump coefficients and run sweep

    Create a biquad filter of type 'name', where name is of one:
    'HLowpassBiQuad', 'HHighpassBiQuad', 'HBandpassBiQuad', 'HNotchBiQuad',
    'HAllpassBiQuad', 'HPeakingEQBiQuad', 'HLowShelvingBiQuad', 'HHighShelvingBiQuad'.
    Then dump the coefficients to the file given by the -of parameter (in case you want
    to build a IIR filter directly or simulate the filter in other programs).

    Finally, sweep the biquad and plot the response with GnuPlot.


    \page bqt
    Syntax: dspcmd -bqt name Fcutoff Q G

    Create biquad filter and run file through the filter

    Create a biquad filter of type 'name', where name is of one:
    'HLowpassBiQuad', 'HHighpassBiQuad', 'HBandpassBiQuad', 'HNotchBiQuad',
    'HAllpassBiQuad', 'HPeakingEQBiQuad', 'HLowShelvingBiQuad', 'HHighShelvingBiQuad'.
    Then dump the coefficients to the file given by the -of parameter (in case you want
    to build a IIR filter directly or simulate the filter in other programs).

    Finally, run samples from the file given with the -if parameter through the filter and
    write the result to the file given with the -of parameter


    \page bqc
    Syntax: dspcmd -bqc coeffs

    Create biquad filter, dump coefficients and run sweep

    Create a biquad filter from the coefficients in the file 'coeffs'. Then dump the
    coefficients to the file given with the -of parameter (so that you can check that the
    coefficients are being read correctly), then sweep the biquad and plot the response with gnuPlot


    \page bgct
    Syntax: dspcmd -bqct coeffs

    Create biquad filter and run file through the filter

    Create a biquad filter from the coefficients in the file 'coeffs', then run the samples from the file
    given with the -if parameter through the filter and write the results to the file given
    with the -of parameter.

    \page kb
    Syntax: dspcmd -kb name start stop att

    Create Kaiser-Bessel filter with F-start = 'start' and F-stop = 'stop' and attenuation 'add'.
    dump coefficients and run sweep.

     \page kbt
    Syntax: dspcmd -kbt name start stop att

    Create Kaiser-Bessel filter with F-start = 'start' and F-stop = 'stop' and attenuation 'add',
    then run the file given with '-if' through the filter.

    \page g
    Syntax: dspcmd -g gain

    Run file through gain filter

    Read sampes from the file given with the -if parameter and run them through a gain
    filter with gain 'gain' (integer number). Write the results to the file given with
    the -of parameter.


    \page cb
    Syntax: dspcmd -cb fBase alpha

    Sweep a comb filter with base frequency fBase and alpha (gain)

    Create a comb filter with base frequency 'fBase' and alpha 'alpha', then
    sweep the filter and plot the response using GnuPlot.


    \page cbt
    Syntax: dspcmd -cbt fBase alpha

    Run a file through a comb filter with base frequency fBase and alpha (gain)

    Create a comb filter with base frequency 'fbase' and alpha 'alpha', then run
    the samples from the file given with the -if parameter through the filter and
    write them to the file given with the -of parameter


    \page hm
    Syntax: dspcmd -hm fBase fCutoff

    Sweep a hum filter with base frequency fBase and cutoff frequency fCutoff

    Create a hum filter (HHumFilter) with base frequency 'fBase' and upper cutoff
    frequency 'fCutoff'. Then sweep the filter and plot the response with GnuPlot.


    \page hmt
    Syntax: dspcmd -hmt fBase fCutoff

    Run a file through a hum filter with base frequency fBase and cutoff frequency fCutoff

    Create a hum filter (HHumFilter) with base frequency 'fBase' and upper cutoff frequency
    'fCutoff'. Then run the samples from the file given with the -if parameter through the
    filter and write them to the file given with the -of parameter


    \page gz
    Syntax: dspcmd -gq

    Sweep a file by running the individual bins through a Goertzl filter

    Run samples from the file given with the -if parameter and sweep each block of
    samples through goertzl filters, then add to a final average result plot giving
    the average output for a goertzl filter at each frequency.

    Show the plot using GnuPlot

    \page ma
    Syntax: dspcmd -ma num

    Sweep a moving average filter with the avering factor M set to 'num'

    \page mat
    Syntax: dspcmd -mat num

    Run a file through a moving average filter with the averaging factor M set to 'num'

    \page riq
    Syntax: dspcmd -riq

    Convert realvalued samples to IQ samples

    \page fft
    Syntax: dspcmd -fft

    Convert input signal to FFT

    \page ifft
    Syntax: dspcmd -ifft

    Convert input FFT to IFFT (signal)


    \page dmx
    Syntax: dspcmd -dmx

    Demultiplex samples into two separate files

    \page dcm
    Syntax: dspcmd -dcm factor

    Decimate input samples by a factor

    \page fdcm
    Syntax: dpscmd -fdcm factor coeffs

    Filter input file through a FIR decimator

    \page tr2
    Syntax: dpscmd -tr2

    Translate input samples by two (invert spectrum)"

    \page tr4i
    Syntax: dpscmd -tr4i

    Translate input samples by four and output I branch

    \page tr4q
    dpscmd -tr4q

    Translate input samples by four and output Q branch

    \page up
    Syntax: dspcmd -up factor
    
    Upsample input samples by a factor

    \page ip
    Syntax: dspcmd -ip factor coeffs
    
    Interpolate input samples by a factor with FIR coefficients read from the file 'coeffs'

    \page bb
    Syntax: dpscmd -bb center width

    Move segment around 'center' with 'width' to baseband

    \page stc
    Syntax: dpscmd -stc in out [scale]

    Convert sample type. 'in' and 'out' must be one of the sample formats. (See '-f').
    If a scale factor is given (floating point), the samples is scaled (up or down)
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

