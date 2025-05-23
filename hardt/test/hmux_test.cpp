#include <iostream>

#include "test.h"

class HMux_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_mux_as_readers);
            UNITTEST(test_mux_as_writers);
            UNITTEST(test_mux_as_consumers);

            UNITTEST(test_mux_as_duplex_reader);
            UNITTEST(test_mux_as_duplex_writer);
            UNITTEST(test_mux_as_duplex_consumer);
        }

        const char* name() override
        {
            return "HMux";
        }

    private:


        void test_mux_as_readers()
        {
            int8_t stream_1[4] = {1, 3, 5, 7};
            int8_t stream_2[4] = {2, 4, 6, 8};
            TestReader<int8_t> rd_1("hmux_test_testreader_1", stream_1, 4);
            TestReader<int8_t> rd_2("hmux_test_testreader_2", stream_2, 4);
            std::vector< HReader<int8_t>* > readers = { &rd_1, &rd_2 };
            HMux<int8_t> mux("hmux_test_as_readers", readers, 8);

            int8_t input[8];
            int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            ASSERT_IS_EQUAL(mux.Read(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) input, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd_1.Commands, 1);
            ASSERT_IS_EQUAL(rd_2.Commands, 1);
        }

        void test_mux_as_writers()
        {
            int8_t stream_1[4] = {1, 3, 5, 7};
            int8_t stream_2[4] = {2, 4, 6, 8};
            TestWriter<int8_t> wr("hmux_test_testwriter", 8);
            HMux<int8_t> mux("hmux_test_as_writer", &wr, 2, 4);

            int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            ASSERT_IS_EQUAL(mux.Write(stream_1, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(mux.Write(stream_2, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(mux.Write(stream_1, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(mux.Write(stream_2, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 2);
            ASSERT_IS_EQUAL(wr.Samples, 16);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_mux_as_consumers()
        {
            int8_t stream_1[4] = {1, 3, 5, 7};
            int8_t stream_2[4] = {2, 4, 6, 8};

            TestWriter<int8_t> srcWr1("hmux_test_testwriter_src_1", 4);
            TestWriter<int8_t> srcWr2("hmux_test_testwriter_src_2", 4);
            std::vector< HWriterConsumer<int8_t>* > consumers;
            consumers.push_back(srcWr1.Consumer());
            consumers.push_back(srcWr2.Consumer());
            HMux<int8_t> mux("hmux_test_as_consumers", consumers, 4);
            TestWriter<int8_t> wr("hmux_test_testwriter_wr", mux.Consumer(), 8);

            int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            ASSERT_IS_EQUAL(srcWr1.Write(stream_1, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(wr.Samples, 0);
            ASSERT_IS_EQUAL(srcWr2.Write(stream_2, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(srcWr1.Write(stream_1, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(srcWr2.Write(stream_2, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 2);
            ASSERT_IS_EQUAL(wr.Samples, 16);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_mux_as_duplex_reader()
        {
            int8_t stream_1[4] = {1, 2, 3, 4};
            TestReader<int8_t> rd_1("hmux_test_testreader", stream_1, 4);
            std::vector< HReader<int8_t>* > readers = { &rd_1 };
            HMux<int8_t> mux("hmux_test_as_duplex_reader", readers, 8, true);

            int8_t input[8];
            int8_t expected[8] = {1, 1, 2, 2, 3, 3, 4, 4};
            ASSERT_IS_EQUAL(mux.Read(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) input, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd_1.Commands, 1);
        }

        void test_mux_as_duplex_writer()
        {
            int8_t stream_1[4] = {1, 2, 3, 4};
            TestWriter<int8_t> wr("hmux_test_testwriter", 8);
            HMux<int8_t> mux("hmux_test_as_duplex_writer", &wr, 1, 4, true);

            int8_t expected[8] = {1, 1, 2, 2, 3, 3, 4, 4};
            ASSERT_IS_EQUAL(mux.Write(stream_1, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_mux_as_duplex_consumer()
        {
            int8_t stream_1[4] = {1, 2, 3, 4};

            TestWriter<int8_t> srcWr1("hmux_test_testwriter_src_1", 4);
            std::vector< HWriterConsumer<int8_t>* > consumers;
            consumers.push_back(srcWr1.Consumer());
            HMux<int8_t> mux("hmux_test_as_duplex_consumer", consumers, 4, true);
            TestWriter<int8_t> wr("hmux_test_testwriter_wr", mux.Consumer(), 8);

            int8_t expected[8] = {1, 1, 2, 2, 3, 3, 4, 4};
            ASSERT_IS_EQUAL(srcWr1.Write(stream_1, 4), 4);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

} hmux_test;