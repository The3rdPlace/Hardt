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

            // Test specific biquad implementations
            UNITTEST(test_lowpassbiquad_as_writer);
            UNITTEST(test_lowpassbiquad_as_reader);
            UNITTEST(test_highpassbiquad_as_writer);
            UNITTEST(test_highpassbiquad_as_reader);
            UNITTEST(test_bandpassbiquad_as_writer);
            UNITTEST(test_bandpassbiquad_as_reader);
            UNITTEST(test_notchbiquad_as_writer);
            UNITTEST(test_notchbiquad_as_reader);
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
        class TestBiQuad : public HBiQuad<T>
        {
            private:

                float _a0;
                float _a1;
                float _a2;
                float _b0;
                float _b1;
                float _b2;

                void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2)
                {
                    *a0 = _a0;
                    *a1 = _a1;
                    *a2 = _a2;
                    *b0 = _b0;
                    *b1 = _b1;
                    *b2 = _b2;
                }

            public:

                TestBiQuad(float a0, float a1, float a2, float b0, float b1, float b2):
                    HBiQuad<T>(0.0f, 0.0f, 0.0f, 0.0f),
                    _a0(a0),
                    _a1(a1),
                    _a2(a2),
                    _b0(b0),
                    _b1(b1),
                    _b2(b2)
                {}

                static HIirFilter<T>* Create(HWriter<T>* writer, size_t blocksize)
                {
                    TestBiQuad<T>* bq = new TestBiQuad(1, 1, 2, 1, 2, 3);
                    return new HIirFilter<T>(writer, ((HBiQuad<T>*) bq)->Calculate(), 5, blocksize);
                }

                static HIirFilter<T>* Create(HReader<T>* writer, size_t blocksize)
                {
                    TestBiQuad<T>* bq = new TestBiQuad(1, 1, 2, 1, 2, 3);
                    return new HIirFilter<T>(writer, ((HBiQuad<T>*) bq)->Calculate(), 5, blocksize);
                }

                static HIirFilter<T>* CreateNormalized(HWriter<T>* writer, size_t blocksize)
                {
                    TestBiQuad<T>* bq = new TestBiQuad(2, 2, 4, 2, 4, 6);
                    return new HIirFilter<T>(writer, ((HBiQuad<T>*) bq)->Calculate(), 5, blocksize);
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

        float ftrim(float value)
        {
            int intval = ceil(value * 1e6);
            float out = intval / 1e6;
            //std::cout << "in/out = " << value << "/" << out << std::endl;
            return out;
        }

        void test_lowpassbiquad_as_writer()
        {
            TestWriter<int16_t> wr;
            HFilter<int16_t>* filter = HBiQuadFactory<HLowpassBiQuad<int16_t>, int16_t>::Create(&wr, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.235644f));
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.154455f));
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.308910f));
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(0.154455f));
        }

        void test_lowpassbiquad_as_reader()
        {
            TestReader<int16_t> rd;
            HFilter<int16_t>* filter = HBiQuadFactory<HLowpassBiQuad<int16_t>, int16_t>::Create(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.235644f));
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.154455f));
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.308910f));
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(0.154455f));
        }

        void test_highpassbiquad_as_writer()
        {
            TestWriter<int16_t> wr;
            HFilter<int16_t>* filter = HBiQuadFactory<HHighpassBiQuad<int16_t>, int16_t>::Create(&wr, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.235644f));
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.463367f));
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(-0.926732f));
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(0.463367f));
        }

        void test_highpassbiquad_as_reader()
        {
            TestReader<int16_t> rd;
            HFilter<int16_t>* filter = HBiQuadFactory<HHighpassBiQuad<int16_t>, int16_t>::Create(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.235644f));
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.463367f));
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(-0.926732f));
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(0.463367f));
        }

        void test_bandpassbiquad_as_writer()
        {
            TestWriter<int16_t> wr;
            HFilter<int16_t>* filter = HBiQuadFactory<HBandpassBiQuad<int16_t>, int16_t>::Create(&wr, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.235644f));
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.267525f));
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.0f));
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.267524f));
        }

        void test_bandpassbiquad_as_reader()
        {
            TestReader<int16_t> rd;
            HFilter<int16_t>* filter = HBiQuadFactory<HBandpassBiQuad<int16_t>, int16_t>::Create(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.235644f));
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.267525f));
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.0f));
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.267524f));
        }

        void test_notchbiquad_as_writer()
        {
            TestWriter<int16_t> wr;
            HFilter<int16_t>* filter = HBiQuadFactory<HNotchBiQuad<int16_t>, int16_t>::Create(&wr, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.235644f));
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(0.617822f));
        }

        void test_notchbiquad_as_reader()
        {
            TestReader<int16_t> rd;
            HFilter<int16_t>* filter = HBiQuadFactory<HNotchBiQuad<int16_t>, int16_t>::Create(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.235644f));
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(-0.617822f));
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(0.617822f));
        }

} hbiquads_test;