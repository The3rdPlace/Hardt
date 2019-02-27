#include <samplegenerator.h>

using namespace Hardt;

std::complex<double>* SampleGenerator::GetSamples(int sampleFreq, int duration, int amplitude, int* frequencies, int num_frequencies)
{
    int length = sampleFreq * duration;

    // generate samples
    std::complex<double>* data = new std::complex<double>[length];
    for ( int sec = 0; sec < duration; sec++ )
    {
        for ( int i = 0; i < length; i++ )
        {
            //data[i] = (0.0, 0.0);

            // sum several known sinusoids into data[i]
            for ( int j = 0; j < num_frequencies; j++ )
            {
                data[i] += (amplitude * sin(2 * M_PI * frequencies[j] * i / sampleFreq), 0);
            }
        }
    }
    return data;
}
