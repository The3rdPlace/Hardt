#include <stdio.h>
#include <iostream>

#include "test.h"

class HHilbertFilter_Test: public Test
{
public:

    void run()
    {
        UNITTEST(test_filter_as_writer);
        UNITTEST(test_filter_as_reader);
    }

    const char* name()
    {
        return "HHilbertFilter";
    }

private:

    void test_filter_as_writer()
    {
        HSineGenerator<int16_t> osc(H_SAMPLE_RATE_8K, 1000, 100);
        int16_t input[1024];
        osc.Read(input, 1024);

        TestWriter<int16_t> wr(1024);
        HHilbertFilter<int16_t> filter(wr.Writer(), 1024);
        ASSERT_IS_EQUAL(filter.Write(input, 1024), 1024);

        // Apply fft to compare spectrum of the inverse-fft'ed signal
        HFft<int16_t> fft(1024);
        double spectrum[1024];
        double phase[512];
        fft.FFT(wr.Received, spectrum, phase);

        // The small amount of data and integer rounding will move the center slightly
        // away from the mathematically correct bin numbers
        for(int i = 0; i < 512; i++ ) {
            if( i == 128 ) {
                ASSERT_IS_BETWEEN(spectrum[i], (double) 25471, (double) 25472);
                ASSERT_IS_BETWEEN(phase[i], (double) 1.5707, (double) 1.5708);
            } else if( i == 384 ) {
                // This is a small spike due to the generator not being totally correct
                ASSERT_IS_BETWEEN(spectrum[i], (double) 128, (double) 129);
            }else {
                ASSERT_IS_EQUAL(spectrum[i], (double) 0);
            }
        }

        try
        {
            filter.Write(input, 1025);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(filter.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_filter_as_reader()
    {
        HSineGenerator<int16_t> osc(H_SAMPLE_RATE_8K, 1000, 100);
        int16_t output[1024];
        osc.Read(output, 1024);

        TestReader<int16_t> rd(output, 1024);
        HHilbertFilter<int16_t> filter(rd.Reader(), 1024);

        int16_t received[1024];
        ASSERT_IS_EQUAL(filter.Read(received, 1024), 1024);

        // Apply fft to compare spectrum of the inverse-fft'ed signal
        HFft<int16_t> fft(1024);
        double spectrum[1024];
        double phase[512];
        fft.FFT(received, spectrum, phase);

        // The small amount of data and integer rounding will move the center slightly
        // away from the mathematically correct bin numbers
        for(int i = 0; i < 512; i++ ) {
            if( i == 128 ) {
                ASSERT_IS_BETWEEN(spectrum[i], (double) 25471, (double) 25472);
                ASSERT_IS_BETWEEN(phase[i], (double) 1.5707, (double) 1.5708);
            } else if( i == 384 ) {
                // This is a small spike due to the generator not being totally correct
                ASSERT_IS_BETWEEN(spectrum[i], (double) 128, (double) 129);
            }else {
                ASSERT_IS_EQUAL(spectrum[i], (double) 0);
            }
        }

        try
        {
            filter.Read(received, 1025);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(filter.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(rd.Commands, 1);
    }
} hhilberfilter_test;