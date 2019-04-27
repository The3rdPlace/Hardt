#include <generator.h>
#include <stdio.h>
#include <math.h>

std::complex<double>* Generator::GetSamples(int sampleFreq, int duration, int amplitude, int* frequencies, int num_frequencies)
{
    int length = sampleFreq * duration;

    // generate samples
    std::complex<double>* data = new std::complex<double>[length];
    for ( int sec = 0; sec < duration; sec++ )
    {
        for ( int i = 0; i < length; i++ )
        {
            data[i] = 0.0;

            // sum several known sinusoids into data[i]
            for ( int j = 0; j < num_frequencies; j++ )
            {
                data[i] += (amplitude * sin(2 * M_PI * frequencies[j] * i / sampleFreq));
            }
        }
    }
    return data;
}

/* Usage
Generator* g = new Generator();
int freqs[1];
freqs[0] = 25;
std::complex<double>* samples = g->GetSamples(1000, 1, 1, freqs, 1);
//print_array< std::complex<double> >(samples, 1000);
delete g;*/

