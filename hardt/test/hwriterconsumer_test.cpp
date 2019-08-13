#include "test.h"

class HWriterConsumer_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_bottom_to_top);
            UNITTEST(test_top_to_bottom);
        }

        const char* name()
        {
            return "HWriterConsumer";
        }

    private:

        template <class T>
        class TestWriter : public HWriter<T>, public HWriterConsumer<T>
        {
            public:

                int Writes;
                int Samples;
                HWriter<T>* _writer;

                TestWriter(HWriter<T>* writer):
                    Writes(0),
                    Samples(0),
                    _writer(writer)
                {}

                TestWriter(HWriterConsumer<T>* consumer):
                    Writes(0),
                    Samples(0)
                {
                    consumer->SetWriter(this);
                }

                int Write(T* src, size_t blocksize)
                {
                    Writes++;
                    Samples += blocksize;

                    _writer->Write(src, blocksize);

                    return blocksize;
                }

                void SetWriter(HWriter<T>* writer)
                {
                    _writer = writer;
                }
        };

        void test_bottom_to_top()
        {
            //----------------------------------------
            //  Reader chain
            //----------------------------------------

            // Source
            HSineGenerator<int8_t> rd1(H_SAMPLE_RATE_48K, 1000, 100);

            // Run through some random reader, a gain filter is simple
            HGain<int8_t> rd2(&rd1, 1, 10);

            //----------------------------------------
            //  Writer chain + processor
            //----------------------------------------

            // Write samples out into empty space
            HNullWriter<int8_t> wr4;

            // 3. writer
            TestWriter<int8_t> wr3(wr4.Writer());

            // 2. writer
            TestWriter<int8_t> wr2(wr3.Writer());

            // 1. writer
            TestWriter<int8_t> wr1(wr2.Writer());

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
            HSineGenerator<int8_t> rd1(H_SAMPLE_RATE_48K, 1000, 100);

            // Run through some random reader, a gain filter is simple
            HGain<int8_t> rd2(&rd1, 1, 10);

            //----------------------------------------
            //  Writer chain + processor
            //----------------------------------------

            // Processor
            bool terminated = false;
            HStreamProcessor<int8_t> proc(&rd1, 10, &terminated);

            // 1. writer
            TestWriter<int8_t> wr1(proc.Consumer());

            // 2. writer
            TestWriter<int8_t> wr2(wr1.Consumer());

            // 3. writer
            TestWriter<int8_t> wr3(wr2.Consumer());

            // Write samples out into empty space
            HNullWriter<int8_t> wr4(wr3.Consumer());

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