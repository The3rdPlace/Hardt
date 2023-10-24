#include "test.h"

class HWriterConsumer_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_bottom_to_top);
            UNITTEST(test_top_to_bottom);
        }

        const char* name() override
        {
            return "HWriterConsumer";
        }

    private:

        void test_bottom_to_top()
        {
            //----------------------------------------
            //  Reader chain
            //----------------------------------------

            // Source
            HSineGenerator<int8_t> rd1("hwriterconsumer_test_sine_generator", H_SAMPLE_RATE_48K, 1000, 100);

            // Run through some random reader, a gain filter is simple
            HGain<int8_t> rd2("hwriterconsumer_test_gain", &rd1, 1, 10);

            //----------------------------------------
            //  Writer chain + processor
            //----------------------------------------

            // Write samples out into empty space
            HNullWriter<int8_t> wr4("hwriterconsumer_test_nullwriter");

            // 3. writer
            TestWriter<int8_t> wr3("hwriterconsumer_test_testwriter_3", wr4.Writer(), 10);

            // 2. writer
            TestWriter<int8_t> wr2("hwriterconsumer_test_testwriter_2", wr3.Writer(), 10);

            // 1. writer
            TestWriter<int8_t> wr1("hwriterconsumer_test_testwriter_1", wr2.Writer(), 10);

            // Processor
            bool terminated = false;
            HStreamProcessor<int8_t> proc(wr1.Writer(), &rd1, 10, &terminated);

            //----------------------------------------
            //  Run the chain
            //----------------------------------------

            proc.Run(4);

            ASSERT_IS_EQUAL(wr1.Writes, 4);
            ASSERT_IS_EQUAL(wr1.Samples, 40);

            ASSERT_IS_EQUAL(wr2.Writes, 4);
            ASSERT_IS_EQUAL(wr2.Samples, 40);

            ASSERT_IS_EQUAL(wr3.Writes, 4);
            ASSERT_IS_EQUAL(wr3.Samples, 40);
        }

        void test_top_to_bottom()
        {
            //----------------------------------------
            //  Reader chain
            //----------------------------------------

            // Source
            HSineGenerator<int8_t> rd1("hwriterconsumer_test_sine_generator", H_SAMPLE_RATE_48K, 1000, 100);

            // Run through some random reader, a gain filter is simple
            HGain<int8_t> rd2("hwriterconsumer_test_gain", &rd1, 1, 10);

            //----------------------------------------
            //  Writer chain + processor
            //----------------------------------------

            // Processor
            bool terminated = false;
            HStreamProcessor<int8_t> proc("hwriterconsumer_test_processor", &rd1, 10, &terminated);

            // 1. writer
            TestWriter<int8_t> wr1("hwriterconsumer_test_testwriter_1", proc.Consumer(), 10);

            // 2. writer
            TestWriter<int8_t> wr2("hwriterconsumer_test_testwriter_2", wr1.Consumer(), 10);

            // 3. writer
            TestWriter<int8_t> wr3("hwriterconsumer_test_testwriter_3", wr2.Consumer(), 10);

            // Write samples out into empty space
            HNullWriter<int8_t> wr4("hwriterconsumer_test_testwriter_4", wr3.Consumer());

            //----------------------------------------
            //  Run the chain
            //----------------------------------------

            proc.Run(4);

            ASSERT_IS_EQUAL(wr1.Writes, 4);
            ASSERT_IS_EQUAL(wr1.Samples, 40);

            ASSERT_IS_EQUAL(wr2.Writes, 4);
            ASSERT_IS_EQUAL(wr2.Samples, 40);

            ASSERT_IS_EQUAL(wr3.Writes, 4);
            ASSERT_IS_EQUAL(wr3.Samples, 40);
        }

} hwriterconsumer_test;