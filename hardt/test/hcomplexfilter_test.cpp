#include <stdio.h>
#include <iostream>

#include "test.h"

class HComplexFilter_Test: public Test
{
public:

    void run()
    {
        // Full test to check the output of the HComplexFilter component
        UNITTEST(test_filter_as_writer);

        // Simple tests, just to check that read and write behaves as expected
        UNITTEST(test_filter_as_consumer);
        UNITTEST(test_filter_as_reader);

        // Test some input signals filtered by a bessel FIR
        TEST(test_filter_with_real_signal);
    }

    const char* name()
    {
        return "HComplexFilter";
    }

private:

    void test_filter_as_writer()
    {
        // Input signal
        std::complex<int8_t> input[8]    = {
            std::complex<int8_t>(1, 8),
            std::complex<int8_t>(2, 7),
            std::complex<int8_t>(3, 6),
            std::complex<int8_t>(4, 5),
            std::complex<int8_t>(5, 4),
            std::complex<int8_t>(6, 3),
            std::complex<int8_t>(7, 2),
            std::complex<int8_t>(8, 1)
        };

        // Filter response
        std::complex<int8_t> response[6] = {
                std::complex<int8_t>(1, 2),
                std::complex<int8_t>(2, 1),
                std::complex<int8_t>(1, 3),
                std::complex<int8_t>(3, 1),
                std::complex<int8_t>(1, 4),
                std::complex<int8_t>(4, 1)
        };

        // Expected
        std::complex<int8_t> expected[6] = {
                std::complex<int8_t>(17, -6),
                std::complex<int8_t>(11, -12),
                std::complex<int8_t>(21, 3),
                std::complex<int8_t>(17, -11),
                std::complex<int8_t>(21, 16),
                std::complex<int8_t>(27, -6),
        };

        TestWriter<std::complex<int8_t>> wr(6);
        HComplexFilter<int8_t> flt(wr.Writer(), 6, response);

        ASSERT_IS_EQUAL(flt.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 1);

        /* for( int i = 0; i < 6; i++ ) {
            std::cout << "[" << i << "] = " << std::to_string(wr.Received[i].real()) << ", i" << std::to_string(wr.Received[i].imag()) << std::endl;
        } */

        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(int8_t) * 6), 0);

        try
        {
            flt.Write(input, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(flt.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_filter_as_consumer()
    {
        // Input signal
        std::complex<int8_t> input[8]    = {
                std::complex<int8_t>(1, 8),
                std::complex<int8_t>(2, 7),
                std::complex<int8_t>(3, 6),
                std::complex<int8_t>(4, 5),
                std::complex<int8_t>(5, 4),
                std::complex<int8_t>(6, 3),
                std::complex<int8_t>(7, 2),
                std::complex<int8_t>(8, 1)
        };

        // Filter response
        std::complex<int8_t> response[6] = {
                std::complex<int8_t>(1, 2),
                std::complex<int8_t>(2, 1),
                std::complex<int8_t>(1, 3),
                std::complex<int8_t>(3, 1),
                std::complex<int8_t>(1, 4),
                std::complex<int8_t>(4, 1)
        };

        TestWriter<std::complex<int8_t>> srcWr(8);
        HComplexFilter<int8_t> flt(srcWr.Consumer(), 6, response);
        TestWriter<std::complex<int8_t>> wr(flt.Consumer(),8);

        ASSERT_IS_EQUAL(srcWr.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 1);

        try
        {
            srcWr.Write(input, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(srcWr.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_filter_as_reader()
    {
        // Input signal
        std::complex<int8_t> input[8]    = {
                std::complex<int8_t>(1, 8),
                std::complex<int8_t>(2, 7),
                std::complex<int8_t>(3, 6),
                std::complex<int8_t>(4, 5),
                std::complex<int8_t>(5, 4),
                std::complex<int8_t>(6, 3),
                std::complex<int8_t>(7, 2),
                std::complex<int8_t>(8, 1)
        };

        // Filter response
        std::complex<int8_t> response[6] = {
                std::complex<int8_t>(1, 2),
                std::complex<int8_t>(2, 1),
                std::complex<int8_t>(1, 3),
                std::complex<int8_t>(3, 1),
                std::complex<int8_t>(1, 4),
                std::complex<int8_t>(4, 1)
        };

        TestReader<std::complex<int8_t>> rd(input, 8);
        HComplexFilter<int8_t> flt(rd.Reader(), 6, response);

        std::complex<int8_t> received[6];
        ASSERT_IS_EQUAL(flt.Read(received, 6), 6);
        ASSERT_IS_EQUAL(rd.Reads, 1);

        try
        {
            flt.Read(received, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(flt.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(rd.Commands, 1);
    }

    void test_filter_with_real_signal() {

        // Build input signals
        HVfo<int16_t> osc(H_SAMPLE_RATE_48K, 1000, 100);
        int16_t osc1000[1024];
        int16_t osc1500[1024];
        int16_t osc2500[1024];
        int16_t osc3500[1024];
        osc.Read(osc1000, 1024);
        osc.SetFrequency(1500);
        osc.Read(osc1500, 1024);
        osc.SetFrequency(2500);
        osc.Read(osc2500, 1024);
        osc.SetFrequency(3500);
        osc.Read(osc3500, 1024);

        // Mix inputs
        int16_t input[1024];
        for( int i = 0; i < 1024; i++ ) {
            input[i] = osc1000[i] + osc1500[i] + osc2500[i] + osc3500[i];
        }

        HFileWriter<int16_t> fw("/home/henrik/Git/Hardt/build/input.pcm");
        fw.Start();
        fw.Write(input, 1024);
        fw.Stop();

        // Get the input signal spectrum
        std::complex<double> inputSpectrum[1024];
        HFft<int16_t> inputFft(1024);
        inputFft.FFT(input, inputSpectrum);

        // Get a Kaiser-Bessel lowpass filter
        HLowpassKaiserBessel<int16_t> filter(2000, H_SAMPLE_RATE_48K, 115, 96);
        float* filtercoefs = filter.Calculate();

        // Get filter spectrum
        HFft<float> filterFft(1024);
        std::complex<double> filterSpectrum[1024];
        filterFft.SPECTRUM(filtercoefs, 115, filterSpectrum);

        // Filter input signal
        HInputWriter<std::complex<double>> inputWriter;
        HComplexFilter<double> cfilter(inputWriter.Consumer(), 1024, filterSpectrum);
        std::complex<double> filteredSpectrum[1024];
        HMemoryWriter<std::complex<double>> outputWriter(filteredSpectrum, 1024);
        inputWriter.Write(inputSpectrum, 1024);

        int16_t output[1024];
        inputFft.IFFT(filteredSpectrum, output);

        HFileWriter<int16_t> fwOut("/home/henrik/Git/Hardt/build/output.pcm");
        fwOut.Start();
        fwOut.Write(output, 1024);
        fwOut.Stop();

    }
} hcomplexfilter_test;