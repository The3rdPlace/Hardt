#include <stdio.h>
#include <iostream>

#include "test.h"

class HBiQuads_Test: public Test
{
    public:

        void run()
        {
            // Test that parameters is assigned and propagated to the IIR filter
            UNITTEST(test_biquad_parameters);
            UNITTEST(test_biquad_parameters_normalized);

            // Test that a 2. order IIR filter has been created
            UNITTEST(test_filter_as_writer);
            UNITTEST(test_filter_as_reader);

            // Todo: Test specific biquad implementations
            //UNITTEST(test_lowpassbiquad);
        }

        const char* name()
        {
            return "HBiquad(s)";
        }

    private:

        int16_t expected[6] = {1, 3, 6, 10, 22, 46};

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                int16_t received[6];

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) received, src, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class T>
        class TestReader : public HReader<T>
        {
            private:

                int16_t output[6] = {1, 2, 4, 8, 16, 32};

            public:

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, output, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class T>
        class TestBiQuad : public HBiQuad
        {
            public:

                static HIirFilter<T>* Create(HWriter<T>* writer, size_t blocksize)
                {
                    a0 = 1.0;
                    a1 = 1.0;
                    a2 = 2.0;
                    b0 = 1.0;
                    b1 = 2.0;
                    b2 = 3.0;
                    return new HIirFilter<T>(writer, Normalize(), 5, blocksize);
                }

                static HIirFilter<T>* Create(HReader<T>* writer, size_t blocksize)
                {
                    a0 = 1.0;
                    a1 = 1.0;
                    a2 = 2.0;
                    b0 = 1.0;
                    b1 = 2.0;
                    b2 = 3.0;
                    return new HIirFilter<T>(writer, Normalize(), 5, blocksize);
                }

                static HIirFilter<T>* CreateNormalized(HWriter<T>* writer, size_t blocksize)
                {
                    a0 = 2.0;
                    a1 = 2.0;
                    a2 = 4.0;
                    b0 = 2.0;
                    b1 = 4.0;
                    b2 = 6.0;
                    return new HIirFilter<T>(writer, Normalize(), 5, blocksize);
                }
        };

        void test_biquad_parameters()
        {
            TestWriter<int16_t> wr;
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::Create(&wr, 6);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(c[0], 1.0f);
            ASSERT_IS_EQUAL(c[1], 2.0f);
            ASSERT_IS_EQUAL(c[2], 1.0f);
            ASSERT_IS_EQUAL(c[3], 2.0f);
            ASSERT_IS_EQUAL(c[4], 3.0f);
        }

        void test_biquad_parameters_normalized()
        {
            TestWriter<int16_t> wr;
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::CreateNormalized(&wr, 6);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(c[0], 1.0f);
            ASSERT_IS_EQUAL(c[1], 2.0f);
            ASSERT_IS_EQUAL(c[2], 1.0f);
            ASSERT_IS_EQUAL(c[3], 2.0f);
            ASSERT_IS_EQUAL(c[4], 3.0f);
        }

        void test_filter_as_writer()
        {
            TestWriter<int16_t> wr;
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::Create(&wr, 6);

            int16_t input[6] = {1, 2, 4, 8, 16, 32};
            ASSERT_IS_EQUAL(filter->Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected, 6 * sizeof(int16_t)), 0);
        }

        void test_filter_as_reader()
        {
            TestReader<int16_t> rd;
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::Create(&rd, 6);

            int16_t received[6];
            ASSERT_IS_EQUAL(filter->Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int16_t)), 0);
        }

        // Todo: test implementation of the lowpass biquad
        void test_lowpassbiquad()
        {
            TestWriter<int16_t> wr;
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::Create(&wr, 6);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(c[0], 1.0f);
            ASSERT_IS_EQUAL(c[1], 2.0f);
            ASSERT_IS_EQUAL(c[2], 1.0f);
            ASSERT_IS_EQUAL(c[3], 2.0f);
            ASSERT_IS_EQUAL(c[4], 3.0f);
        }
} hbiquads_test;