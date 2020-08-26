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
        int16_t osc4000[1024];
        int16_t osc10000[1024];
        int16_t osc14000[1024];
        osc.Read(osc1000, 1024);
        osc.SetFrequency(4000);
        osc.Read(osc4000, 1024);
        osc.SetFrequency(10000);
        osc.Read(osc10000, 1024);
        osc.SetFrequency(14000);
        osc.Read(osc14000, 1024);

        // Mix inputs
        int16_t input[1024];
        for( int i = 0; i < 1024; i++ ) {
            input[i] = osc1000[i] + osc4000[i] + osc10000[i] + osc14000[i];
        }

        // Get the input signal spectrum
        std::complex<double> inputSpectrum[1024];
        HFft<int16_t> inputFft(1024);
        inputFft.FFT(input, inputSpectrum);

        // Verify that the input has peaks at bin 21, 85, 213, 299 (1KHz, 4KHZ, 10KHz, 14KHz)
        for( int i = 0; i < 512; i++ ) {
            if( i == 21 || i == 85 || i == 213 || i == 299 ) {
                if( std::abs(inputSpectrum[i]) < 42000 || std::abs(inputSpectrum[i]) > 43000 ) {
                    HError("Invalid value %ld in bin %d", std::abs(inputSpectrum[i]), i);
                    ASSERT_FAIL("Invalid value in input bin");
                    std::cout << "i = " << i << ", value = " << std::abs(inputSpectrum[i]) << std::endl;
                }
            }
            else if( std::abs(inputSpectrum[i]) >22000 ) {
                HError("Invalid value %ld in bin %d", std::abs(inputSpectrum[i]), i);
                std::cout << "i = " << i << ", value = " << std::abs(inputSpectrum[i]) << std::endl;
                ASSERT_FAIL("Invalid value in input bin");
            }
        }

        // Get a Kaiser-Bessel lowpass filter
        HLowpassKaiserBessel<int16_t> filter(6000, H_SAMPLE_RATE_48K, 115, 96);
        float* filtercoefs = filter.Calculate();

        // Get filter spectrum
        HFft<float> filterFft(1024);
        std::complex<double> filterSpectrum[1024];
        filterFft.SPECTRUM(filtercoefs, 115, filterSpectrum);

        // Filter input signal
        HInputWriter<std::complex<double>> inputWriter;
        HComplexFilter<double> cfilter(inputWriter.Consumer(), 1024, filterSpectrum);
        std::complex<double> filteredSpectrum[1024];
        HMemoryWriter<std::complex<double>> outputWriter(cfilter.Consumer(), filteredSpectrum, 1024);
        inputWriter.Write(inputSpectrum, 1024);

        // Verify that only the peaks at bin 21 and 85 (1KHz and 4KHz) remains
        for( int i = 0; i < 512; i++ ) {
            if( i == 21 || i == 85 ) {
                if( std::abs(filteredSpectrum[i]) < 42000 || std::abs(filteredSpectrum[i]) > 44000 ) {
                    HError("Invalid value %ld in bin %d", std::abs(inputSpectrum[i]), i);
                    std::cout << "i = " << i << ", value = " << std::abs(filteredSpectrum[i]) << std::endl;
                    ASSERT_FAIL("Invalid value in input bin");
                }
            }
            else if( std::abs(filteredSpectrum[i]) >22000 ) {
                HError("Invalid value %ld in bin %d", std::abs(inputSpectrum[i]), i);
                std::cout << "i = " << i << ", value = " << std::abs(filteredSpectrum[i]) << std::endl;
                ASSERT_FAIL("Invalid value in input bin");
            }
        }
    }
} hcomplexfilter_test;