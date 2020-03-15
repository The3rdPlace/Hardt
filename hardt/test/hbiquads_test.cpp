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
            UNITTEST(test_allpassbiquad_as_writer);
            UNITTEST(test_allpassbiquad_as_reader);
            UNITTEST(test_peakingeqbiquad_as_writer);
            UNITTEST(test_peakingeqbiquad_as_reader);
            UNITTEST(test_lowshelvingbiquad_as_writer);
            UNITTEST(test_lowshelvingbiquad_as_reader);
            UNITTEST(test_highshelvingbiquad_as_writer);
            UNITTEST(test_highshelvingbiquad_as_reader);
        }

        const char* name()
        {
            return "HBiquad(s)";
        }

    private:

        int16_t expected[6] = {1, 3, 6, 10, 22, 46};

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
            TestWriter<int16_t> wr(6);
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::Create(&wr, 6);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(c[0], 1.0f); // b0
            ASSERT_IS_EQUAL(c[1], 2.0f); // b1
            ASSERT_IS_EQUAL(c[2], 3.0f); // b2
            ASSERT_IS_EQUAL(c[3], -1.0f); // a1
            ASSERT_IS_EQUAL(c[4], -2.0f); // a2
        }

        void test_biquad_parameters_normalized()
        {
            TestWriter<int16_t> wr(6);
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::CreateNormalized(&wr, 6);

            std::vector<float> c = ((HIirFilter<int16_t>*) filter)->GetCoefficients();
            ASSERT_IS_EQUAL(c[0], 1.0f); // b0
            ASSERT_IS_EQUAL(c[1], 2.0f); // b1
            ASSERT_IS_EQUAL(c[2], 3.0f); // b2
            ASSERT_IS_EQUAL(c[3], -1.0f); // a1
            ASSERT_IS_EQUAL(c[4], -2.0f); // a2
        }

        void test_filter_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::Create(&wr, 6);

            int16_t input[6] = {1, 2, 4, 8, 16, 32};
            ASSERT_IS_EQUAL(filter->Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int16_t)), 0);
        }

        void test_filter_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HFilter<int16_t>* filter = TestBiQuad<int16_t>::Create(&rd, 6);

            int16_t received[6];
            ASSERT_IS_EQUAL(filter->Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int16_t)), 0);
        }

        int ftrim(float value)
        {
            int intval = ceil(value * 1e6);
            return intval;
        }

        void test_lowpassbiquad_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HBiQuadFilter<HLowpassBiQuad<int16_t>, int16_t> filter(wr.Writer(), 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.154455f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.308910f));  // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.154455f));  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f)); // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f));  // a2
        }

        void test_lowpassbiquad_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HBiQuadFilter<HLowpassBiQuad<int16_t>, int16_t> filter(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.154455f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.308910f));  // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.154455f));  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f)); // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f));  // a2
        }

        void test_highpassbiquad_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HBiQuadFilter<HHighpassBiQuad<int16_t>, int16_t> filter(wr.Writer(), 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.463367f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(-0.926732f)); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.463367f));  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f)); // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f));  // a2
        }

        void test_highpassbiquad_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HBiQuadFilter<HHighpassBiQuad<int16_t>, int16_t> filter(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.463367f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(-0.926732f)); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.463367f));  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f)); // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f));  // a2
        }

        void test_bandpassbiquad_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HBiQuadFilter<HBandpassBiQuad<int16_t>, int16_t> filter(wr.Writer(), 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.267525f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.0f));       // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(-0.267524f)); // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f)); // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f));  // a2
        }

        void test_bandpassbiquad_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HBiQuadFilter<HBandpassBiQuad<int16_t>, int16_t> filter(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.267525f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(0.0f));       // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(-0.267524f)); // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f)); // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f));  // a2
        }

        void test_notchbiquad_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HBiQuadFilter<HNotchBiQuad<int16_t>, int16_t> filter(wr.Writer(), 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.617822f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(-0.617822f)); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.617822f));  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f));  // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f)); // a2
        }

        void test_notchbiquad_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HBiQuadFilter<HNotchBiQuad<int16_t>, int16_t> filter(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.617822f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(-0.617822f)); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.617822f));  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f));  // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f)); // a2
        }

        void test_allpassbiquad_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HBiQuadFilter<HAllpassBiQuad<int16_t>, int16_t> filter(wr.Writer(), 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.235644f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(-0.617822f)); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(1.0f));       // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f));  // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f)); // a2
        }

        void test_allpassbiquad_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HBiQuadFilter<HAllpassBiQuad<int16_t>, int16_t> filter(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), ftrim(0.235644f));  // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(-0.617822f)); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(1.0f));       // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.617822f));  // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), ftrim(-0.235644f)); // a2
        }

        void test_peakingeqbiquad_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HBiQuadFilter<HPeakingEQBiQuad<int16_t>, int16_t> filter(wr.Writer(), 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), 1044986);           // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(-0.631319f)); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.217652f));  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.631319f));  // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), -262637);           // a2
        }

        void test_peakingeqbiquad_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HBiQuadFilter<HPeakingEQBiQuad<int16_t>, int16_t> filter(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), 1044986);           // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), ftrim(-0.631319f)); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.217652f));  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), ftrim(0.631319f));  // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), -262637);           // a2
        }

        void test_lowshelvingbiquad_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HBiQuadFilter<HLowShelvingBiQuad<int16_t>, int16_t> filter(wr.Writer(), 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), 1040586);          // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), -611430);          // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.238278f)); // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), 647722);           // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), -242572);          // a2
        }

        void test_lowshelvingbiquad_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HBiQuadFilter<HLowShelvingBiQuad<int16_t>, int16_t> filter(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), 1040586);          // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), -611430);          // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), ftrim(0.238278f)); // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), 647722);           // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), -242572);          // a2
        }

        void test_highshelvingbiquad_as_writer()
        {
            TestWriter<int16_t> wr(6);
            HBiQuadFilter<HHighShelvingBiQuad<int16_t>, int16_t> filter(wr.Writer(), 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), 1078257); // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), -698410); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), 261556);  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), 587583);  // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), -228984); // a2
        }

        void test_highshelvingbiquad_as_reader()
        {
            int16_t output[6] = {1, 2, 4, 8, 16, 32};
            TestReader<int16_t> rd(output, 6);
            HBiQuadFilter<HHighShelvingBiQuad<int16_t>, int16_t> filter(&rd, 8000, 48000, 0.7, 1, 4096);

            std::vector<float> c = filter.GetCoefficients();
            ASSERT_IS_EQUAL(ftrim(c[0]), 1078257); // b0
            ASSERT_IS_EQUAL(ftrim(c[1]), -698410); // b1
            ASSERT_IS_EQUAL(ftrim(c[2]), 261556);  // b2
            ASSERT_IS_EQUAL(ftrim(c[3]), 587583);  // a1
            ASSERT_IS_EQUAL(ftrim(c[4]), -228984); // a2
        }

} hbiquads_test;