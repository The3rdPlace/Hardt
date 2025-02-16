#include <iostream>

#include "test.h"

class HFilter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_filter_as_writer);
            UNITTEST(test_filter_as_reader);
            UNITTEST(test_read_coeffs_single_line);
            UNITTEST(test_read_coeffs_separators);
            UNITTEST(test_read_coeffs_mixed);
            UNITTEST(test_filter_write_disable_enable);
            UNITTEST(test_filter_read_disable_enable);
        }

        const char* name() override
        {
            return "HFilter";
        }

    private:

        int8_t expected[6] = {2, 4, 8, 16, 32, 64};
        
        template <class T>
        class TestFilter : public HFilter<T>
        {
            private:

                void Filter(T* src, T* dest, size_t blocksize) override
                {
                    for( int i = 0; i < blocksize; i++ )
                    {
                        dest[i] = src[i] << 1;
                    }
                }

            public:

                TestFilter(std::string id, HWriter<T>* writer, size_t blocksize):
                    HFilter<T>(id, writer, blocksize)
                {}

                TestFilter(std::string id, HReader<T>* reader, size_t blocksize):
                    HFilter<T>(id, reader, blocksize)
                {}
        };

        void test_filter_as_writer()
        {
            TestWriter<int8_t> wr("hfilter_test_testwriter", 8);
            TestFilter<int8_t> filter("hfilter_test_as_writer", &wr, 6);

            int8_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(filter.Write(input, 2), 2);

            try
            {
                filter.Write(input, 8);
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
            int8_t output[8] = {1, 2, 4, 8, 16, 32, 0, 0};

            TestReader<int8_t> rd("hfilter_test_testreader", output, 8);
            TestFilter<int8_t> filter("hfilter_test_testfilter", &rd, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(filter.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(filter.Read(received, 2), 2);

            try
            {
                filter.Read(received, 8);
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

        void test_read_coeffs_single_line()
        {
            std::string source(Test::sourceDir);
            source += "/resources/hfilter_coeffs_single_line.dat";
            std::vector<float> coeffs = HFilter<int16_t>::ReadCoeffsFromFile(source);
            ASSERT_IS_EQUAL((int) coeffs.size(), 20);

            std::vector<float> expected;
            expected.push_back(0.06318758096656742);
            expected.push_back(-0.12637516193313483);
            expected.push_back(0.06318758096656742);
            expected.push_back(1.9822798942156608);
            expected.push_back(-0.9928732078139422);
            expected.push_back(0.0625);
            expected.push_back(-0.125);
            expected.push_back(0.0625);
            expected.push_back(1.986908612864604);
            expected.push_back(-0.9971024098670853);
            expected.push_back(0.000244140625);
            expected.push_back(0.00048828125);
            expected.push_back(0.000244140625);
            expected.push_back(1.9814486970269125);
            expected.push_back(-0.9926669567470747);
            expected.push_back(0.000244140625);
            expected.push_back(0.00048828125);
            expected.push_back(0.000244140625);
            expected.push_back(1.9851881609275535);
            expected.push_back(-0.996895491657892);
            for(int i = 0; i < expected.size(); i++ ) {
                ASSERT_IS_EQUAL(expected.at(i), coeffs.at(i));
            }
        }

        void test_read_coeffs_separators()
        {
            std::string source(Test::sourceDir);
            source += "/resources/hfilter_coeffs_separators.dat";
            std::vector<float> coeffs = HFilter<int16_t>::ReadCoeffsFromFile(source);
            ASSERT_IS_EQUAL((int) coeffs.size(), 20);

            std::vector<float> expected;
            expected.push_back(0.06318758096656742);
            expected.push_back(-0.12637516193313483);
            expected.push_back(0.06318758096656742);
            expected.push_back(1.9822798942156608);
            expected.push_back(-0.9928732078139422);
            expected.push_back(0.0625);
            expected.push_back(-0.125);
            expected.push_back(0.0625);
            expected.push_back(1.986908612864604);
            expected.push_back(-0.9971024098670853);
            expected.push_back(0.000244140625);
            expected.push_back(0.00048828125);
            expected.push_back(0.000244140625);
            expected.push_back(1.9814486970269125);
            expected.push_back(-0.9926669567470747);
            expected.push_back(0.000244140625);
            expected.push_back(0.00048828125);
            expected.push_back(0.000244140625);
            expected.push_back(1.9851881609275535);
            expected.push_back(-0.996895491657892);
            for(int i = 0; i < expected.size(); i++ ) {
                ASSERT_IS_EQUAL(expected.at(i), coeffs.at(i));
            }
        }

        void test_read_coeffs_mixed()
        {
            std::string source(Test::sourceDir);
            source += "/resources/hfilter_coeffs_mixed.dat";
            std::vector<float> coeffs = HFilter<int16_t>::ReadCoeffsFromFile(source);
            ASSERT_IS_EQUAL((int) coeffs.size(), 20);

            std::vector<float> expected;
            expected.push_back(0.06318758096656742);
            expected.push_back(-0.12637516193313483);
            expected.push_back(0.06318758096656742);
            expected.push_back(1.9822798942156608);
            expected.push_back(-0.9928732078139422);
            expected.push_back(0.0625);
            expected.push_back(-0.125);
            expected.push_back(0.0625);
            expected.push_back(1.986908612864604);
            expected.push_back(-0.9971024098670853);
            expected.push_back(0.000244140625);
            expected.push_back(0.00048828125);
            expected.push_back(0.000244140625);
            expected.push_back(1.9814486970269125);
            expected.push_back(-0.9926669567470747);
            expected.push_back(0.000244140625);
            expected.push_back(0.00048828125);
            expected.push_back(0.000244140625);
            expected.push_back(1.9851881609275535);
            expected.push_back(-0.996895491657892);
            for(int i = 0; i < expected.size(); i++ ) {
                ASSERT_IS_EQUAL(expected.at(i), coeffs.at(i));
            }
        }

        void test_filter_write_disable_enable()
        {
            TestWriter<int8_t> wr("hfilter_test_testwriter", 6);
            TestFilter<int8_t> filter("hfilter_test_write_disable_enabled", &wr, 6);

            int8_t input[6] = {1, 2, 4, 8, 16, 32};
            ASSERT_IS_TRUE(filter.GetEnabled());
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int8_t)), 0);

            filter.Disable();
            ASSERT_IS_TRUE(!filter.GetEnabled());
            int8_t disabled[6] = {99, 98, 97, 96, 95, 94};
            memcpy((void*) wr.Received, (void*) disabled, 6 * sizeof(int8_t));
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) input, 6 * sizeof(int8_t)), 0);

            filter.Enable();
            ASSERT_IS_TRUE(filter.GetEnabled());
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int8_t)), 0);
        }

        void test_filter_read_disable_enable()
        {
            int8_t output[6] = {1, 2, 4, 8, 16, 32};

            TestReader<int8_t> rd("hfilter_test_testreader", output, 6);
            TestFilter<int8_t> filter("hfilter_test_read_disable_enable", &rd, 6);

            int8_t received[6];
            ASSERT_IS_TRUE(filter.GetEnabled());
            ASSERT_IS_EQUAL(filter.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int8_t)), 0);

            filter.Disable();
            ASSERT_IS_TRUE(!filter.GetEnabled());
            ASSERT_IS_EQUAL(filter.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) output, 6 * sizeof(int8_t)), 0);

            filter.Enable();
            ASSERT_IS_TRUE(filter.GetEnabled());
            ASSERT_IS_EQUAL(filter.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int8_t)), 0);
        }

} hfilter_test;