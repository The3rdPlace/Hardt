#include <stdio.h>
#include <iostream>

#include "test.h"

class HBaseband_Test: public Test
{
public:

    void run()
    {
        // Full test to check the output of the HBaseband component
        UNITTEST(test_baseband_as_writer);

        // Simple tests, just to check that read and write behaves as expected
        UNITTEST(test_baseband_as_consumer);
        UNITTEST(test_baseband_as_reader);
    }

    const char* name()
    {
        return "HBaseband";
    }

private:

    void test_baseband_as_writer()
    {
        // Input signal
        HCosineGenerator<int8_t> inputGenerator(H_SAMPLE_RATE_48K, 6000, 100);
        int8_t input[257];
        inputGenerator.Read(input, 256);

        TestWriter<int8_t> wr(256);
        HBaseband<int8_t> bb(wr.Writer(), H_SAMPLE_RATE_48K, 8000, 12000, 256);

        ASSERT_IS_EQUAL(bb.Write(input, 256), 256);
        ASSERT_IS_EQUAL(wr.Writes, 1);

        HFft<int8_t> fft(256);
        double inputSpectrum[256];
        fft.FFT(input, inputSpectrum);

        double outputSpectrum[256];
        fft.FFT(wr.Received, outputSpectrum);

        /*for(int i = 0; i < 128; i++ ) {
            std::cout << "[" << i << "] = " << std::to_string(inputSpectrum[i]) << "  <-->  " << std::to_string(outputSpectrum[i]) << std::endl;
        }
        ASSERT_FAIL("Stop: Debug info");*/

        double max = 0;
        int inputMaxBin = -1;
        for( int i = 0; i < 128; i++ ) {
            if( inputSpectrum[i] > max ) {
                inputMaxBin = i;
                max = inputSpectrum[i];
            }
        }
        ASSERT_IS_EQUAL(inputMaxBin, 32); // 6000 / (48000 / 256) =~ 32

        max = 0;
        int outputMaxBin = -1;
        for( int i = 0; i < 128; i++ ) {
            if( outputSpectrum[i] > max ) {
                outputMaxBin = i;
                max = outputSpectrum[i];
            }
        }
        ASSERT_IS_EQUAL(outputMaxBin, 21); // 4000 / (48000 / 256) =~ 21

        try
        {
            bb.Write(input, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        try
        {
            bb.Write(input, 257);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(bb.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_baseband_as_consumer()
    {
        TestWriter<int8_t> srcWr(8);
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        HBaseband<int8_t> bb(srcWr.Consumer(), H_SAMPLE_RATE_8K, 0, 1000, 6);
        TestWriter<int8_t> wr(bb.Consumer(),8);

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

        try
        {
            srcWr.Write(input, 4);
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

    void test_baseband_as_reader()
    {
        int8_t output[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        TestReader<int8_t> rd(output, 8);
        HBaseband<int8_t> bb(&rd, H_SAMPLE_RATE_8K, 0, 1000, 6);

        int8_t received[6];
        ASSERT_IS_EQUAL(bb.Read(received, 6), 6);
        ASSERT_IS_EQUAL(rd.Reads, 1);

        try
        {
            bb.Read(received, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        try
        {
            bb.Read(received, 4);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(bb.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(rd.Commands, 1);
    }
} hbaseband_test;